# Goldenrod native rendering adapter

Goldenrod uses standard Porymap layouts and private tileset pairs. Its complete
HGSS-based city is divided into twelve connected native sections. Thirteen
identical BG palette banks and a bounded native glyph cache keep their camera
views continuous; no static cross-section tile-ID assumption is used.

Edit the native maps/tilesets. Then regenerate **only the adapter**, from the hub:

```
python3 tools/gba/maps/goldenrod_city/stream.py /path/to/engine
```

Do not replay the initial `author.py` over committed maps. `stream.py` reads the
actual native PNGs, palettes, map entries, metatiles and layers; it does not write
a native layout or tileset. It verifies shared palette equality and the worst
16x16 buffered view against the reserved1008glyph slots. Preserve global palette
coherence when deliberately changing a bank. Commit the native edits and atlas
outputs together. `make` runs the source-hash check, failing on a stale adapter.

The renderer only activates for Goldenrod. Its25KBheap allocation is volatile,
released on other-map rendering and invalidated on heap reset. Existing saves,
map IDs and other maps' tile/palette data are unchanged. The top16glyph slots
remain reserved. Future exterior door animation/events must respect that
reservation. Building interiors, train operation, adjacent routes and water/
fountain animation are not part of this exterior source.

Original map geography and game artwork: Game Freak / Nintendo / Creatures,
HeartGold/SoulSilver. The hub retains the original reference, canonical shared
Center/tree/lawn provenance, editable construction layers and review evidence:
https://github.com/Davisfox5/pokemon-apocrypha/tree/codex/goldenrod-city-evidence/gba/art/goldenrod-city

The camera margin uses directional decorative borders derived from native cells:
periodic ocean to the west, full conifers/lawn to the north, the existing cliff
face to the east, and ocean/forest/street/paving to the south. These extend the
cropped reference for camera coverage, without adding traversable geography.
Capacity checks include the margin. The original northern picket fence, gate
footprint and the eastern partial cliff column block their visible barriers.
