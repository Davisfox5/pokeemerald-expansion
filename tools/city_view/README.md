# Sectioned city view

Some HGSS cities need more unique 8x8 tiles than one Gen 3 tileset pair holds
(1008 slots). Such a city is split into ordinary Porymap sections, each with a
private tileset pair inside the hardware limits, all sharing one set of 13 BG
palettes. `src/apocrypha_city_view.c` streams each section's own native tiles
into VRAM as the camera moves, so neighbouring sections draw with their own
art across the connection. Every other map uses the unmodified renderer.

| City | Sections | Generator (hub) |
| --- | --- | --- |
| Blackthorn | `BlackthornCity01`-`03` (rows 0-23, 24-43, 44-63) | `tools/gba/maps/blackthorn/stream.py` |

Edit the native sections in Porymap as usual. Then regenerate the view tables
from the Pokemon Apocrypha hub and commit both together:

```
python3 tools/gba/maps/blackthorn/stream.py /path/to/this/checkout
```

The generator reads the actual map.bin, metatiles, attributes, tiles.png and
palettes; it never writes a layout or tileset. It refuses palettes that differ
between sections and checks that the worst 16x16-metatile camera window fits
the 1007 streaming slots (the top 16 tiles stay reserved for door animations).
`make` runs `check.py`, which fails on tables left stale by a native edit.

Limits: metatiles changed at runtime by scripts are not reflected in the view
(no such script exists for these cities). No sectioned city connects to an
ordinary map yet; such a connection would also need the primary tiles,
palettes and BG buffers reloaded on entry and exit.
