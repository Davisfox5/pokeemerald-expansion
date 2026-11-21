// Hidden Grottoes (B2W2 style).
//
// A grotto entrance is an ordinary warp on a route that leads to one of the interior maps
// (sGrottoInteriors). Each interior's own warp points to MAP_DYNAMIC, so entering sets the dynamic
// warp back to the entrance; the grotto is identified from that dynamic warp (source map + the
// entrance warp's tile). Any number of grottoes can share an interior, and the exit always returns
// to the right route.
//
// Each grotto holds one thing at a time: a Pokémon with its Hidden Ability, or an item. Taking it
// (or encountering the Pokémon, win or lose) empties the grotto. Every 256 steps each empty
// grotto has a HIDDEN_GROTTO_REFILL_PERCENT chance to refill with a new roll from its slot table.
// A grotto the player has never entered counts as full.

#include "global.h"
#include "hidden_grotto.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "overworld.h"
#include "ow_abilities.h"
#include "pokemon.h"
#include "random.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/species.h"
#include "data/hidden_grottoes.h"

// Interior layouts a grotto entrance can lead to (gba/grottoes/grottoes.json "interior").
static const u16 sGrottoInteriors[] =
{
    MAP_HIDDEN_GROTTO,
    MAP_HIDDEN_GROTTO_BLOSSOM,
    MAP_HIDDEN_GROTTO_WINDING,
};

static bool32 IsInGrottoInterior(void)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sGrottoInteriors); i++)
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(sGrottoInteriors[i])
         && gSaveBlock1Ptr->location.mapNum == MAP_NUM(sGrottoInteriors[i]))
            return TRUE;
    }
    return FALSE;
}

static u8 *GetStateByte(u32 id)
{
    return &gSaveBlock3Ptr->hiddenGrottoes[id];
}

static u32 RollSlot(u32 id)
{
    const struct HiddenGrotto *grotto = &gHiddenGrottoes[id];
    u32 total = 0, i, roll;

    for (i = 0; i < grotto->slotCount; i++)
        total += grotto->slots[i].weight;
    if (total == 0)
        return GROTTO_STATE_EMPTY;

    roll = Random() % total;
    for (i = 0; i < grotto->slotCount; i++)
    {
        if (roll < grotto->slots[i].weight)
            return i + 1;
        roll -= grotto->slots[i].weight;
    }
    return GROTTO_STATE_EMPTY;
}

u32 GetHiddenGrottoState(u32 id)
{
    if (id >= HIDDEN_GROTTO_COUNT)
        return GROTTO_STATE_EMPTY;
    return *GetStateByte(id);
}

// The slot the grotto currently holds, rolling it on the first visit. NULL when empty.
static const struct HiddenGrottoSlot *GetFilledSlot(u32 id)
{
    u8 *state;

    if (id >= HIDDEN_GROTTO_COUNT || gHiddenGrottoes[id].slotCount == 0)
        return NULL;

    state = GetStateByte(id);
    if (*state == GROTTO_STATE_UNVISITED)
        *state = RollSlot(id);
    if (*state == GROTTO_STATE_EMPTY || *state > gHiddenGrottoes[id].slotCount)
        return NULL;
    return &gHiddenGrottoes[id].slots[*state - 1];
}

void UpdateHiddenGrottoStepCounter(void)
{
    u32 id, inside = GROTTO_NONE;

    if (++gSaveBlock3Ptr->hiddenGrottoSteps != 0)
        return;

    // The grotto the player is standing in doesn't refill until they leave: its content
    // object is only shown on map entry.
    if (IsInGrottoInterior())
        inside = GetCurrentHiddenGrottoId();

    for (id = 0; id < HIDDEN_GROTTO_COUNT; id++)
    {
        u8 *state = GetStateByte(id);
        if (id != inside && *state == GROTTO_STATE_EMPTY && gHiddenGrottoes[id].slotCount != 0
         && (Random() % 100) < HIDDEN_GROTTO_REFILL_PERCENT)
            *state = RollSlot(id);
    }
}

// Which grotto the player is in, from the dynamic warp that leads back out.
u32 GetCurrentHiddenGrottoId(void)
{
    const struct WarpData *exit = &gSaveBlock1Ptr->dynamicWarp;
    const struct MapHeader *source;
    const struct WarpEvent *warp;
    u32 id, map;

    if (exit->mapGroup < 0 || exit->mapGroup >= MAP_GROUPS_COUNT || exit->warpId < 0)
        return GROTTO_NONE;

    source = Overworld_GetMapHeaderByGroupAndId(exit->mapGroup, exit->mapNum);
    if (source == NULL || source->events == NULL || exit->warpId >= source->events->warpCount)
        return GROTTO_NONE;

    warp = &source->events->warps[exit->warpId];
    map = (exit->mapGroup << 8) | exit->mapNum;
    for (id = 0; id < HIDDEN_GROTTO_COUNT; id++)
    {
        if (gHiddenGrottoes[id].map == map && gHiddenGrottoes[id].x == warp->x && gHiddenGrottoes[id].y == warp->y)
            return id;
    }
    return GROTTO_NONE;
}

static u16 GetSlotGraphicsId(const struct HiddenGrottoSlot *slot)
{
    if (slot->species == SPECIES_NONE)
        return OBJ_EVENT_GFX_ITEM_BALL;
    if (slot->gender == GROTTO_GENDER_FEMALE)
        return slot->species + OBJ_EVENT_MON + OBJ_EVENT_MON_FEMALE;
    return slot->species + OBJ_EVENT_MON;
}

// ON_TRANSITION and ON_RESUME of every grotto interior. The grotto borrows the route's map section
// (name popup, region, met location), battle scene and music, then shows or hides its content.
void HiddenGrotto_OnTransition(void)
{
    const struct WarpData *exit = &gSaveBlock1Ptr->dynamicWarp;
    const struct HiddenGrottoSlot *slot;
    u32 id = GetCurrentHiddenGrottoId();

    if (exit->mapGroup >= 0 && exit->mapGroup < MAP_GROUPS_COUNT)
    {
        const struct MapHeader *source = Overworld_GetMapHeaderByGroupAndId(exit->mapGroup, exit->mapNum);
        if (source != NULL)
        {
            gMapHeader.regionMapSectionId = source->regionMapSectionId;
            gMapHeader.battleType = source->battleType;
            Overworld_SetSavedMusic(source->music);
        }
    }

    slot = GetFilledSlot(id);
    if (slot == NULL)
    {
        FlagSet(FLAG_HIDDEN_GROTTO_EMPTY);
        return;
    }
    FlagClear(FLAG_HIDDEN_GROTTO_EMPTY);
    VarSet(VAR_OBJ_GFX_ID_0, GetSlotGraphicsId(slot));
}

// specialvar: GROTTO_CONTENT_*. VAR_0x8004 = species or item, VAR_0x8005 = quantity (1).
u16 HiddenGrotto_GetContent(void)
{
    const struct HiddenGrottoSlot *slot = GetFilledSlot(GetCurrentHiddenGrottoId());

    gSpecialVar_0x8004 = 0;
    gSpecialVar_0x8005 = 0;
    if (slot == NULL)
        return GROTTO_CONTENT_NONE;

    gSpecialVar_0x8005 = 1;
    if (slot->species != SPECIES_NONE)
    {
        gSpecialVar_0x8004 = slot->species;
        return GROTTO_CONTENT_POKEMON;
    }
    gSpecialVar_0x8004 = slot->item;
    return GROTTO_CONTENT_ITEM;
}

// Replaces the mon prepared by setwildbattle with the grotto's Pokémon: its Hidden Ability,
// the slot's gender (or Cute Charm/random), Synchronize-able nature, never shiny (as in B2W2).
void HiddenGrotto_CreateWildMon(void)
{
    const struct HiddenGrottoSlot *slot = GetFilledSlot(GetCurrentHiddenGrottoId());
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
    u32 personality, level, gender;
    u8 abilityNum = GROTTO_ABILITY_NUM;
    bool8 notShiny = FALSE;

    if (slot == NULL || slot->species == SPECIES_NONE)
        return;

    level = slot->minLevel;
    if (slot->maxLevel > slot->minLevel)
        level += Random() % (slot->maxLevel - slot->minLevel + 1);

    if (slot->gender == GROTTO_GENDER_MALE)
        gender = MON_MALE;
    else if (slot->gender == GROTTO_GENDER_FEMALE)
        gender = MON_FEMALE;
    else
        gender = GetSynchronizedGender(STATIC_WILDMON_ORIGIN, slot->species);

    ZeroEnemyPartyMons();
    personality = GetMonPersonality(slot->species, gender,
                                    GetSynchronizedNature(STATIC_WILDMON_ORIGIN, slot->species),
                                    RANDOM_UNOWN_LETTER);
    CreateMonWithIVs(mon, slot->species, level, personality, OTID_STRUCT_PLAYER_ID, USE_RANDOM_IVS);
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    SetMonData(mon, MON_DATA_IS_SHINY, &notShiny);
    GiveMonInitialMoveset(mon);
}

void HiddenGrotto_Clear(void)
{
    u32 id = GetCurrentHiddenGrottoId();
    if (id < HIDDEN_GROTTO_COUNT)
        *GetStateByte(id) = GROTTO_STATE_EMPTY;
}

// Leaves through the entrance the player came in by (the dynamic warp). Follow with waitstate.
void HiddenGrotto_Exit(void)
{
    SetWarpDestinationToDynamicWarp(0);
    DoWarp();
    ResetInitialPlayerAvatarState();
}

// Debug/test helper: refill every empty grotto now.
void HiddenGrotto_RefillAll(void)
{
    u32 id;
    for (id = 0; id < HIDDEN_GROTTO_COUNT; id++)
    {
        if (*GetStateByte(id) == GROTTO_STATE_EMPTY && gHiddenGrottoes[id].slotCount != 0)
            *GetStateByte(id) = RollSlot(id);
    }
}
