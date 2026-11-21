#ifndef GUARD_CONSTANTS_HIDDEN_GROTTO_H
#define GUARD_CONSTANTS_HIDDEN_GROTTO_H

// Hidden Grottoes (B2W2 style). Each grotto is a fixed spot that holds one Pokémon with its
// Hidden Ability, or one item. Once emptied, it can refill as the player walks.
//
// The grotto list itself is generated from pokemon-apocrypha's gba/grottoes/grottoes.json by
// tools/gba/grottoes/grottoes.py into constants/hidden_grottoes.h and src/data/hidden_grottoes.h.

// Save bytes reserved in SaveBlock3, one per grotto. Grotto IDs index this array, so the size is
// fixed: adding a grotto takes the next free ID and never moves existing save data.
#define HIDDEN_GROTTO_CAPACITY        64

// Every 256 steps (one wrap of the u8 step counter) each empty grotto has this chance to refill.
#define HIDDEN_GROTTO_REFILL_PERCENT  5

// Per-grotto save byte. 0 means the player has never entered it: it counts as full and its
// content is rolled on the first visit. 1-254 is the filled slot index + 1.
#define GROTTO_STATE_UNVISITED  0
#define GROTTO_STATE_EMPTY      0xFF

// Returned to scripts by HiddenGrotto_GetContent.
#define GROTTO_CONTENT_NONE     0
#define GROTTO_CONTENT_POKEMON  1
#define GROTTO_CONTENT_ITEM     2

#define GROTTO_NONE             0xFF

// Slot gender, as written in the generated table.
#define GROTTO_GENDER_ANY       0
#define GROTTO_GENDER_MALE      1
#define GROTTO_GENDER_FEMALE    2

// Hidden Ability slot used for every grotto Pokémon.
#define GROTTO_ABILITY_NUM      2

// Object event flag that hides the grotto's content sprite when it is empty (the content
// object in data/maps/HiddenGrotto/map.json names FLAG_TEMP_1 directly).
#define FLAG_HIDDEN_GROTTO_EMPTY  FLAG_TEMP_1

#include "constants/hidden_grottoes.h"

#if HIDDEN_GROTTO_COUNT > HIDDEN_GROTTO_CAPACITY
#error "More Hidden Grottoes than HIDDEN_GROTTO_CAPACITY save bytes"
#endif

#endif // GUARD_CONSTANTS_HIDDEN_GROTTO_H
