// Olivine-only native glyph streaming. Map/collision/event source remains
// ordinary Porymap data. Other maps keep their unmodified static renderer.
#include "global.h"
#include "malloc.h"
#include "fieldmap.h"
#include "constants/map_groups.h"
#include "olivine_view.h"

struct OlivineSection { u8 group, map, x, y; };
#include "data/olivine.h"
#define SLOTS 1008
struct OlivineCache {
    u16 ring[256][8];
    u16 references[SLOTS];
    u16 glyphs[SLOTS];
    u16 slots[OLIVINE_GLYPH_COUNT];
    u16 cursor;
};
static EWRAM_DATA struct OlivineCache *sCache = NULL;
EWRAM_DATA u32 gOlivineCacheOverflows = 0;
EWRAM_DATA u32 gOlivineCacheUploads = 0;

static const struct OlivineSection *Section(void)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sOlivineSections); i++)
        if (gSaveBlock1Ptr->location.mapGroup == sOlivineSections[i].group
         && gSaveBlock1Ptr->location.mapNum == sOlivineSections[i].map)
            return &sOlivineSections[i];
    return NULL;
}
// Called before heap reinitialization; ownership disappears with that heap.
void OlivineViewForget(void) { sCache = NULL; }
bool32 OlivineViewActive(void) { return Section() != NULL; }
void OlivineViewBegin(void)
{
    if (Section() == NULL)
    {
        FREE_AND_SET_NULL(sCache);
        return;
    }
    if (sCache == NULL)
        sCache = AllocZeroed(sizeof(*sCache));
    else
        memset(sCache, 0, sizeof(*sCache));
    if (sCache == NULL) { gOlivineCacheOverflows++; return; }
    // Native0 is always transparent; reserve it for all blank quadrants.
    CpuFastCopy(sOlivineGlyphs, (void *)BG_VRAM, 32);
    sCache->cursor = 1;
}
static u16 Glyph(u16 glyph)
{
    u16 slot, old;
    u32 checked;
    if (glyph == 0) return 0;
    slot = sCache->slots[glyph];
    if (slot != 0)
    {
        sCache->references[slot]++;
        return slot;
    }
    for (checked = 0; checked < SLOTS - 1; checked++)
    {
        slot = sCache->cursor++;
        if (sCache->cursor >= SLOTS) sCache->cursor = 1;
        if (sCache->references[slot] == 0)
        {
            old = sCache->glyphs[slot];
            sCache->slots[old] = 0;
            sCache->glyphs[slot] = glyph;
            sCache->slots[glyph] = slot;
            sCache->references[slot] = 1;
            CpuFastCopy(sOlivineGlyphs + glyph * 8, (void *)(BG_VRAM + slot * 32), 32);
            gOlivineCacheUploads++;
            return slot;
        }
    }
    gOlivineCacheOverflows++;
    return 0;
}
// A camera slice is replaced as one batch before its BG tilemap is published.
// Release the entire outgoing slice first: keeping its other fifteen old cells
// alive while loading the first new cell can exceed the final view's budget.
void OlivineViewReleaseSlice(u16 offset, bool32 vertical)
{
    u32 row, col, i, q;
    if (sCache == NULL || !OlivineViewActive()) return;
    row = offset >> 6;
    col = (offset & 31) / 2;
    for (i = 0; i < 16; i++)
    {
        u16 *ring = sCache->ring[((row + (vertical ? i : 0)) & 15) * 16
                              + ((col + (vertical ? 0 : i)) & 15)];
        for (q = 0; q < 8; q++)
        {
            if (ring[q] != 0) sCache->references[ring[q]]--;
            ring[q] = 0;
        }
    }
}
bool32 OlivineViewMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layer)
{
    const struct OlivineSection *section = Section();
    const u32 *entries;
    u16 *ring;
    u32 i;

    if (section == NULL) return FALSE;
    if (sCache == NULL) OlivineViewBegin();
    if (sCache == NULL) return FALSE;
    x += section->x - 7;
    y += section->y - 7;
    // Continue the same native cliff courses/materials across camera borders.
    // Routes are still closed; this is visual padding, not a map connection.
    if (y < 0)
    {
        x = x < 0 ? 0 : x >= 65 ? 64 : x;
        y = (y % 2 + 2) % 2;
    }
    else if (y >= 54)
    {
        if (x >= 22 && x < 27) y = 52 + (y & 1);
        else { x = 2 + ((u32)x & 1); y = 42 + (y & 1); }
    }
    else if (x < 0)
    {
        if (y < 40) x = 0;
        else { x = 2 + ((u32)x & 1); y = 42 + (y & 1); }
    }
    else if (x >= 65)
    {
        if (y < 19) x = 64;
        else { x = 2 + (x & 1); y = 42 + (y & 1); }
    }
    entries = sOlivineEntries + (y * 65 + x) * 8;
    *layer = sOlivineLayers[y * 65 + x];
    ring = sCache->ring[(offset >> 6) * 16 + (offset & 31) / 2];
    // Release the outgoing ring cell before assigning replacement glyphs.
    for (i = 0; i < 8; i++)
        if (ring[i] != 0) sCache->references[ring[i]]--;
    for (i = 0; i < 8; i++)
    {
        u32 entry = entries[i];
        ring[i] = Glyph(entry & 0xFFFF);
        tiles[i] = ring[i] | ((entry >> 6) & 0xFC00);
    }
    return TRUE;
}
