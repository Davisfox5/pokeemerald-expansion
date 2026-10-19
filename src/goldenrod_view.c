// Goldenrod-only native glyph streaming. Map/collision/event source remains
// ordinary Porymap data. Other maps keep their unmodified static renderer.
#include "global.h"
#include "malloc.h"
#include "fieldmap.h"
#include "constants/map_groups.h"
#include "goldenrod_view.h"

struct GoldenrodSection { u8 group, map, x, y; };
#include "data/goldenrod.h"
#define SLOTS 1008
struct GoldenrodCache {
    u16 ring[256][8];
    u16 references[SLOTS];
    u16 glyphs[SLOTS];
    u16 slots[GOLDENROD_GLYPH_COUNT];
    u16 cursor;
};
static EWRAM_DATA struct GoldenrodCache *sCache = NULL;
EWRAM_DATA u32 gGoldenrodCacheOverflows = 0;
EWRAM_DATA u32 gGoldenrodCacheUploads = 0;

static const struct GoldenrodSection *Section(void)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sGoldenrodSections); i++)
        if (gSaveBlock1Ptr->location.mapGroup == sGoldenrodSections[i].group
         && gSaveBlock1Ptr->location.mapNum == sGoldenrodSections[i].map)
            return &sGoldenrodSections[i];
    return NULL;
}
// Called before heap reinitialization; ownership disappears with that heap.
void GoldenrodViewForget(void) { sCache = NULL; }
bool32 GoldenrodViewActive(void) { return Section() != NULL; }
void GoldenrodViewBegin(void)
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
    if (sCache == NULL) { gGoldenrodCacheOverflows++; return; }
    // Native0 is always transparent; reserve it for all blank quadrants.
    CpuFastCopy(sGoldenrodGlyphs, (void *)BG_VRAM, 32);
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
            CpuFastCopy(sGoldenrodGlyphs + glyph * 8, (void *)(BG_VRAM + slot * 32), 32);
            gGoldenrodCacheUploads++;
            return slot;
        }
    }
    gGoldenrodCacheOverflows++;
    return 0;
}
bool32 GoldenrodViewMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layer)
{
    const struct GoldenrodSection *section = Section();
    const u32 *entries;
    u16 *ring;
    u32 i;

    if (section == NULL) return FALSE;
    if (sCache == NULL) GoldenrodViewBegin();
    if (sCache == NULL) return FALSE;
    x += section->x - 7;
    y += section->y - 7;
    // Directional native borders complete the camera margin, without opening
    // traversal beyond the authored city. Reuse ordinary editable source cells:
    // periodic ocean, complete conifers, lawn, east cliff and southern paving.
    if (x < 0) { x = (u32)x & 1; y = (u32)y & 1; }
    else if (x >= 84) { x = 83; y = (y % 56 + 56) % 56; }
    else if (y < 0)
    {
        if (x >= 20 && x < 56) { x = 20 + ((x - 20) & 1); y = (y % 3 + 3) % 3; }
        else if (x >= 65 && x < 77) { x = 65 + ((x - 65) & 1); y = (y % 3 + 3) % 3; }
        else { x = 63; y = 4; }
    }
    else if (y >= 56)
    {
        if (x < 51) { x &= 1; y &= 1; }
        else if (x < 55) { x = 51 + ((x - 51) & 3); y = 53 + (y - 53) % 3; }
        else if (x < 62) { y = 55; }
        else if (x < 82) { x = 62 + (x & 1); y = 54; }
        else { x = 83; y %= 56; }
    }
    entries = sGoldenrodEntries + (y * 84 + x) * 8;
    *layer = sGoldenrodLayers[y * 84 + x];
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
