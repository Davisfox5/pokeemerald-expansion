// Apocrypha: rustling grass for hidden Pokemon (DexNav).
// Wild Pokemon do not walk the overworld, so a hidden Pokemon is shown as a patch of grass
// that sways. The frames are built at runtime from the metatile under the effect: the
// sprite copies that square's own tiles and palette from VRAM, then shears the blades left
// and right. Every region's grass (Johto, Kanto, Hoenn, Sinnoh, Unova) gets a matching
// rustle with no per-tileset art.
#include "global.h"
#include "apoc_grass_rustle.h"
#include "bg.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "malloc.h"
#include "palette.h"
#include "sprite.h"
#include "constants/field_effects.h"

#define TAG_APOC_RUSTLE 0x1A70
#define RUSTLE_FRAMES   5
#define RUSTLE_TILES    4 // 16x16
#define FRAME_BYTES     (RUSTLE_TILES * TILE_SIZE_4BPP)

// Frame n holds the blades leaning by sLean[n] pixels at the top row.
static const s8 sLean[RUSTLE_FRAMES] = {0, 2, 4, -2, -4};

static const union AnimCmd sAnim_Rustle[] =
{
    ANIMCMD_FRAME(0 * RUSTLE_TILES, 6),
    ANIMCMD_FRAME(1 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(2 * RUSTLE_TILES, 4),
    ANIMCMD_FRAME(1 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(0 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(3 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(4 * RUSTLE_TILES, 4),
    ANIMCMD_FRAME(3 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(0 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(2 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(4 * RUSTLE_TILES, 3),
    ANIMCMD_FRAME(0 * RUSTLE_TILES, 20),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_Rustle[] = {sAnim_Rustle};

static const struct OamData sOam_Rustle =
{
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 2,
};

static const struct SpriteTemplate sSpriteTemplate_Rustle =
{
    .tileTag = TAG_APOC_RUSTLE,
    .paletteTag = TAG_APOC_RUSTLE,
    .oam = &sOam_Rustle,
    .anims = sAnims_Rustle,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static u8 FindColor(u16 *colors, u8 *count, u16 color)
{
    u32 i, best = 1, bestDist = 0xFFFFFFFF;

    for (i = 1; i < *count; i++)
    {
        if (colors[i] == color)
            return i;
    }
    if (*count < 16)
    {
        colors[*count] = color;
        return (*count)++;
    }
    // More than 15 colors across palettes: use the nearest one already present.
    for (i = 1; i < 16; i++)
    {
        s32 dr = (s32)(colors[i] & 31) - (s32)(color & 31);
        s32 dg = (s32)((colors[i] >> 5) & 31) - (s32)((color >> 5) & 31);
        s32 db = (s32)((colors[i] >> 10) & 31) - (s32)((color >> 10) & 31);
        u32 dist = dr * dr + dg * dg + db * db;
        if (dist < bestDist)
        {
            bestDist = dist;
            best = i;
        }
    }
    return best;
}

// Flattens the visible layers of the metatile at (x, y) into a 16x16 index map.
static void ReadSquare(s32 x, s32 y, u8 *pixels, u16 *colors)
{
    const struct MapLayout *layout = gMapHeader.mapLayout;
    u32 metatileId = MapGridGetMetatileIdAt(x, y);
    const u16 *entries;
    const u8 *charBase = (const u8 *)BG_CHAR_ADDR(GetBgAttribute(2, BG_ATTR_CHARBASEINDEX));
    u8 count = 1;
    u32 layer, i, px, py;

    if (metatileId < NUM_METATILES_IN_PRIMARY)
        entries = &layout->primaryTileset->metatiles[metatileId * NUM_TILES_PER_METATILE];
    else
        entries = &layout->secondaryTileset->metatiles[(metatileId - NUM_METATILES_IN_PRIMARY) * NUM_TILES_PER_METATILE];

    // Where no layer draws a pixel the backdrop shows, so start from the backdrop color.
    colors[0] = 0;
    for (i = 0; i < 16 * 16; i++)
        pixels[i] = FindColor(colors, &count, gPlttBufferUnfaded[0]);

    // A metatile's two entry halves are the two layers it draws, whatever its layer type.
    for (layer = 0; layer < 2; layer++)
    {
        for (i = 0; i < 4; i++)
        {
            u16 entry = entries[layer * 4 + i];
            u32 tile = entry & 0x3FF;
            bool32 hFlip = (entry >> 10) & 1;
            bool32 vFlip = (entry >> 11) & 1;
            u32 pal = entry >> 12;
            const u8 *src = charBase + tile * TILE_SIZE_4BPP;

            for (py = 0; py < 8; py++)
            {
                for (px = 0; px < 8; px++)
                {
                    u32 sx = hFlip ? 7 - px : px;
                    u32 sy = vFlip ? 7 - py : py;
                    u8 byte = src[sy * 4 + sx / 2];
                    u8 c = (sx & 1) ? byte >> 4 : byte & 0xF;
                    if (c == 0)
                        continue;
                    pixels[((i / 2) * 8 + py) * 16 + (i % 2) * 8 + px] = FindColor(colors, &count, gPlttBufferUnfaded[pal * 16 + c]);
                }
            }
        }
    }
}

// Writes one 16x16 frame in OBJ 1D tile order, shearing each row by its share of the lean.
static void BuildFrame(const u8 *pixels, u8 *dest, s32 lean)
{
    u32 x, y;

    for (y = 0; y < 16; y++)
    {
        // Blades are rooted at the bottom row and lean most at the top. Pixels that lean in from
        // outside the square stay transparent, so the still grass underneath shows through the
        // gap and the motion reads clearly.
        s32 shift = (lean * (s32)(15 - y) + (lean < 0 ? -7 : 7)) / 15;
        for (x = 0; x < 16; x++)
        {
            s32 sx = (s32)x - shift;
            u8 c = (sx < 0 || sx > 15) ? 0 : pixels[y * 16 + sx];
            u32 tile = (y / 8) * 2 + x / 8;
            u8 *byte = &dest[tile * TILE_SIZE_4BPP + (y % 8) * 4 + (x % 8) / 2];
            if (x & 1)
                *byte = (*byte & 0x0F) | (c << 4);
            else
                *byte = (*byte & 0xF0) | c;
        }
    }
}

u32 ApocGrassRustle_Start(void)
{
    s16 x = gFieldEffectArguments[0];
    s16 y = gFieldEffectArguments[1];
    u8 *pixels, *sheet;
    u16 colors[16];
    u32 i, paletteNum, spriteId;
    u16 tileStart;

    pixels = Alloc(16 * 16 + FRAME_BYTES * RUSTLE_FRAMES);
    if (pixels == NULL)
        return MAX_SPRITES;
    sheet = pixels + 16 * 16;
    CpuFill16(0, colors, sizeof(colors));
    ReadSquare(x, y, pixels, colors);
    CpuFill16(0, sheet, FRAME_BYTES * RUSTLE_FRAMES);
    for (i = 0; i < RUSTLE_FRAMES; i++)
        BuildFrame(pixels, sheet + i * FRAME_BYTES, sLean[i]);

    tileStart = GetSpriteTileStartByTag(TAG_APOC_RUSTLE);
    if (tileStart == 0xFFFF)
    {
        struct SpriteSheet spriteSheet = {sheet, FRAME_BYTES * RUSTLE_FRAMES, TAG_APOC_RUSTLE};
        tileStart = LoadSpriteSheet(&spriteSheet);
    }
    else
    {
        CpuCopy16(sheet, (void *)(OBJ_VRAM0 + tileStart * TILE_SIZE_4BPP), FRAME_BYTES * RUSTLE_FRAMES);
    }
    Free(pixels);
    if (tileStart == 0)
        return MAX_SPRITES;

    paletteNum = IndexOfSpritePaletteTag(TAG_APOC_RUSTLE);
    if (paletteNum == 0xFF)
        paletteNum = AllocSpritePalette(TAG_APOC_RUSTLE);
    if (paletteNum == 0xFF)
    {
        FreeSpriteTilesByTag(TAG_APOC_RUSTLE);
        return MAX_SPRITES;
    }
    LoadPalette(colors, OBJ_PLTT_ID(paletteNum), PLTT_SIZE_4BPP);
    UpdateSpritePaletteWithWeather(paletteNum, FALSE);

    SetSpritePosToOffsetMapCoords(&x, &y, 8, 8);
    spriteId = CreateSpriteAtEndUnchecked(&sSpriteTemplate_Rustle, x, y, gFieldEffectArguments[2]);
    if (spriteId == MAX_SPRITES)
        return MAX_SPRITES;

    gSprites[spriteId].coordOffsetEnabled = TRUE;
    // Grass drawn on the top BG (NORMAL layer type) would hide a priority-2 sprite.
    if (MapGridGetMetatileLayerTypeAt(gFieldEffectArguments[0], gFieldEffectArguments[1]) == METATILE_LAYER_TYPE_NORMAL)
        gSprites[spriteId].oam.priority = 1;
    else
        gSprites[spriteId].oam.priority = gFieldEffectArguments[3];
    return spriteId;
}
