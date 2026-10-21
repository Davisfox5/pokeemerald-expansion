# Johto editor consolidation — pending owner conflict choice

Separate project. Original apocrypha-src checkout and running Porymap are untouched.
Base: 1643f58a48. Exact source inventory: johto-editor-sources.json.

Nine current outdoor maps plus three unchanged Union Cave floors load and render
through native Porymap 6.3.1. All outdoor pixels equal their selected source,
except 47 explicitly recorded owner Route30 edits carried without approximation.
The earlier 34 owner cells were already integrated. Existing base map numbers
are unchanged; newly imported map names are appended.

Five cells at the southern Route30 path bend remain unresolved: saved dirt vs
newer grass. Do not mark migration complete or silently choose the grass.
Full original 86-edit snapshot is preserved in editor-owner-review, with its
source base aa8ea35eea. The original working checkout remains untouched.

Shared primary graphics are isolated for Routes29/30 because their pinned source
banks differ from the common newer-town bank. Do not replace one with another.
The source manifest hashes precede the explicit owner edit additions; those
changes and one exact added metatile are recorded in owner-edit-carry.json.

This is an editor consolidation, not a campaign release. No ROM build/runtime
verification has been performed for the combined project. Warp/door and story
changes are not implied. Selector categories and transparent-object brush cleanup
remain unfinished. Stock raw selector remains available until curated catalogs
are validated for this project's exact tilesets.
