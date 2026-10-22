// Native glyph streaming for cities split into several Porymap sections.
// Map, collision and event source stay ordinary Porymap data; the glyph,
// entry and layer tables are derived from those native files at authoring
// time and checked against them on every build (tools/city_view/check.py).
#include "global.h"
#include "malloc.h"
#include "fieldmap.h"
#include "apocrypha_city_view.h"
#include "constants/map_groups.h"

struct CityViewSection
{
    u8 mapGroup;
    u8 mapNum;
    u8 x; // section origin in the city grid, in metatiles
    u8 y;
};

struct CityView
{
    const struct CityViewSection *sections;
    u8 sectionCount;
    u8 width;
    u8 height;
    u16 glyphCount;
    const u32 *glyphs;  // 4bpp glyphs, 8 words each; glyph 0 is blank
    const u32 *entries; // width * height * 8: glyph | (flip and palette bits << 16)
    const u8 *layers;   // width * height metatile layer types
    // Camera margin beyond each edge: the start (even) of a two-cell block that
    // repeats inside the city, per edge column or row; 0xFF uses the open-rock block.
    const u8 *marginNorth;
    const u8 *marginSouth;
    const u8 *marginWest;
    const u8 *marginEast;
    u8 rockX;
    u8 rockY;
};

#include "data/apocrypha_city_views.h"

#define SLOTS (NUM_TILES_TOTAL - 16) // the top 16 tiles stay reserved for door animations
#define RING_CELLS 256               // the 32x32-tile BG buffers hold 16x16 metatiles

struct CityViewCache
{
    const struct CityView *city;
    u16 ring[RING_CELLS][NUM_TILES_PER_METATILE];
    u16 references[SLOTS];
    u16 glyphs[SLOTS];
    u16 cursor;
    u16 slots[]; // per glyph: the VRAM slot holding it, 0 if not resident
};

static EWRAM_DATA struct CityViewCache *sCache = NULL;
EWRAM_DATA u32 gCityViewOverflows = 0;
EWRAM_DATA u32 gCityViewUploads = 0;

static const struct CityView *FindCity(s32 *originX, s32 *originY)
{
    u32 i, j;

    for (i = 0; i < ARRAY_COUNT(sCityViews); i++)
    {
        for (j = 0; j < sCityViews[i].sectionCount; j++)
        {
            const struct CityViewSection *section = &sCityViews[i].sections[j];

            if (gSaveBlock1Ptr->location.mapGroup == section->mapGroup
             && gSaveBlock1Ptr->location.mapNum == section->mapNum)
            {
                *originX = section->x;
                *originY = section->y;
                return &sCityViews[i];
            }
        }
    }
    return NULL;
}

static u32 CacheSize(const struct CityView *city)
{
    return sizeof(struct CityViewCache) + city->glyphCount * sizeof(u16);
}

bool32 CityView_IsActive(void)
{
    s32 x, y;
    return FindCity(&x, &y) != NULL;
}

// Called before heap reinitialization; the cache disappears with that heap.
void CityView_Forget(void)
{
    sCache = NULL;
}

void CityView_Begin(void)
{
    s32 x, y;
    const struct CityView *city = FindCity(&x, &y);

    if (city == NULL)
    {
        FREE_AND_SET_NULL(sCache);
        return;
    }
    if (sCache != NULL && sCache->city != city)
        FREE_AND_SET_NULL(sCache);
    if (sCache == NULL)
        sCache = AllocZeroed(CacheSize(city));
    else
        memset(sCache, 0, CacheSize(city));
    if (sCache == NULL)
    {
        gCityViewOverflows++;
        return;
    }
    sCache->city = city;
    // Slot 0 is always the blank glyph, shared by every empty quadrant.
    CpuFastCopy(city->glyphs, (void *)BG_VRAM, 32);
    sCache->cursor = 1;
}

static u16 UseGlyph(u16 glyph)
{
    u16 slot;
    u32 checked;

    if (glyph == 0)
        return 0;
    slot = sCache->slots[glyph];
    if (slot != 0)
    {
        sCache->references[slot]++;
        return slot;
    }
    for (checked = 0; checked < SLOTS - 1; checked++)
    {
        slot = sCache->cursor++;
        if (sCache->cursor >= SLOTS)
            sCache->cursor = 1;
        if (sCache->references[slot] == 0)
        {
            sCache->slots[sCache->glyphs[slot]] = 0;
            sCache->glyphs[slot] = glyph;
            sCache->slots[glyph] = slot;
            sCache->references[slot] = 1;
            CpuFastCopy(sCache->city->glyphs + glyph * 8, (void *)(BG_VRAM + slot * 32), 32);
            gCityViewUploads++;
            return slot;
        }
    }
    gCityViewOverflows++;
    return 0;
}

static void MarginCell(const struct CityView *city, s32 *x, s32 *y)
{
    bool32 insideX = *x >= 0 && *x < city->width;
    bool32 insideY = *y >= 0 && *y < city->height;
    u8 start;

    if (insideX && insideY)
        return;
    if (!insideX && !insideY)
        start = 0xFF;
    else if (*x < 0)
        start = city->marginWest[*y];
    else if (*x >= city->width)
        start = city->marginEast[*y];
    else if (*y < 0)
        start = city->marginNorth[*x];
    else
        start = city->marginSouth[*x];
    if (start == 0xFF)
    {
        *x = city->rockX + (*x & 1);
        *y = city->rockY + (*y & 1);
    }
    else if (!insideX)
    {
        *x = start + (*x & 1);
    }
    else
    {
        *y = start + (*y & 1);
    }
}

bool32 CityView_DrawMetatile(u16 offset, s32 x, s32 y, u16 *tiles, s32 *layerType)
{
    s32 originX, originY;
    const struct CityView *city = FindCity(&originX, &originY);
    const u32 *entries;
    u16 *ring;
    u32 i;

    if (city == NULL)
        return FALSE;
    if (sCache == NULL || sCache->city != city)
        CityView_Begin();
    if (sCache == NULL)
        return FALSE;
    x += originX - MAP_OFFSET;
    y += originY - MAP_OFFSET;
    // The camera margin beyond the city continues real material: each edge
    // repeats a two-metatile block that already repeats inside the city.
    // Movement there is still closed by the ordinary border collision.
    MarginCell(city, &x, &y);
    entries = city->entries + (y * city->width + x) * NUM_TILES_PER_METATILE;
    *layerType = city->layers[y * city->width + x];
    ring = sCache->ring[(offset >> 6) * 16 + (offset & 31) / 2];
    // Release the outgoing cell before assigning its replacement's glyphs.
    for (i = 0; i < NUM_TILES_PER_METATILE; i++)
    {
        if (ring[i] != 0)
            sCache->references[ring[i]]--;
    }
    for (i = 0; i < NUM_TILES_PER_METATILE; i++)
    {
        ring[i] = UseGlyph(entries[i] & 0xFFFF);
        tiles[i] = ring[i] | (entries[i] >> 16);
    }
    return TRUE;
}
