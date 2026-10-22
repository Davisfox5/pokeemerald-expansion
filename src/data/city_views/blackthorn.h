// Generated FROM native Porymap files by the hub's tools/gba/maps/blackthorn/stream.py. Do not edit.
#define BLACKTHORN_GLYPH_COUNT 1884
static const u32 sBlackthornGlyphs[] = INCBIN_U32("data/city_views/blackthorn/glyphs.4bpp");
static const u32 sBlackthornEntries[64 * 64 * 8] = INCBIN_U32("data/city_views/blackthorn/entries.bin");
static const u8 sBlackthornLayers[64 * 64] = INCBIN_U8("data/city_views/blackthorn/layers.bin");
static const u8 sBlackthornMarginNorth[64] = INCBIN_U8("data/city_views/blackthorn/margin-north.bin");
static const u8 sBlackthornMarginSouth[64] = INCBIN_U8("data/city_views/blackthorn/margin-south.bin");
static const u8 sBlackthornMarginWest[64] = INCBIN_U8("data/city_views/blackthorn/margin-west.bin");
static const u8 sBlackthornMarginEast[64] = INCBIN_U8("data/city_views/blackthorn/margin-east.bin");
static const struct CityViewSection sBlackthornSections[] =
{
    { MAP_GROUP(MAP_BLACKTHORN_CITY01), MAP_NUM(MAP_BLACKTHORN_CITY01), 0, 0 },
    { MAP_GROUP(MAP_BLACKTHORN_CITY02), MAP_NUM(MAP_BLACKTHORN_CITY02), 0, 24 },
    { MAP_GROUP(MAP_BLACKTHORN_CITY03), MAP_NUM(MAP_BLACKTHORN_CITY03), 0, 44 },
};
#define CITY_VIEW_BLACKTHORN { .sections = sBlackthornSections, .sectionCount = ARRAY_COUNT(sBlackthornSections), .width = 64, .height = 64, .glyphCount = BLACKTHORN_GLYPH_COUNT, .glyphs = sBlackthornGlyphs, .entries = sBlackthornEntries, .layers = sBlackthornLayers, .marginNorth = sBlackthornMarginNorth, .marginSouth = sBlackthornMarginSouth, .marginWest = sBlackthornMarginWest, .marginEast = sBlackthornMarginEast, .rockX = 0, .rockY = 0 }
