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
    // Olivine geography: complete forest core north/west; ocean east/south.
    if (x < 0)
    {
        if (y < 28) { x = (u32)x & 1; y = ((y % 2 + 2) % 2) + 2; }
        else if (y < 34) { x = 0; y = 31; }
        else { x = 2 + ((u32)x & 1); y = 42 + ((u32)y & 1); }
    }
    else if (y < 0) { x = 2 + (x & 1); y = 2 + ((y % 2 + 2) % 2); }
    else if (x >= 65 || y >= 54) { x = 2 + (x & 1); y = 42 + (y & 1); }
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
