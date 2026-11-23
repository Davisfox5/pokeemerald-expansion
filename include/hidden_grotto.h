#ifndef GUARD_HIDDEN_GROTTO_H
#define GUARD_HIDDEN_GROTTO_H

#include "constants/hidden_grotto.h"

struct HiddenGrottoSlot
{
    u16 species;   // SPECIES_NONE for an item slot
    u16 item;      // ITEM_NONE for a Pokémon slot
    u8 minLevel;
    u8 maxLevel;
    u8 gender;     // GROTTO_GENDER_*
    u8 weight;     // relative chance this slot is chosen when the grotto refills
};

struct HiddenGrotto
{
    u16 map;       // the map the entrance is on (MAP_* constant)
    u8 x;          // leftmost entrance warp tile on that map
    u8 y;
    u8 slotCount;
    u8 width;      // entrance warp tiles side by side from x (the two cells where two trees meet)
    const struct HiddenGrottoSlot *slots;
};

extern const struct HiddenGrotto gHiddenGrottoes[];

void UpdateHiddenGrottoStepCounter(void);
u32 GetCurrentHiddenGrottoId(void);
u32 GetHiddenGrottoState(u32 id);

// Specials
void HiddenGrotto_OnTransition(void);
u16 HiddenGrotto_GetContent(void);
void HiddenGrotto_CreateWildMon(void);
void HiddenGrotto_Clear(void);
void HiddenGrotto_Exit(void);
void HiddenGrotto_RefillAll(void);

#endif // GUARD_HIDDEN_GROTTO_H
