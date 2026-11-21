@ Hidden Grottoes (B2W2 style). Shared by every grotto; see src/hidden_grotto.c.

HiddenGrotto_EventScript_OnTransition::
	special HiddenGrotto_OnTransition
	end

@ The grotto's one Pokémon or item, standing in the middle of the clearing.
HiddenGrotto_EventScript_Content::
	lock
	specialvar VAR_RESULT, HiddenGrotto_GetContent
	goto_if_eq VAR_RESULT, GROTTO_CONTENT_POKEMON, HiddenGrotto_EventScript_Pokemon
	goto_if_eq VAR_RESULT, GROTTO_CONTENT_ITEM, HiddenGrotto_EventScript_Item
	release
	end

HiddenGrotto_EventScript_Pokemon::
	faceplayer
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_BULBASAUR, 1  @ placeholder, replaced by the grotto Pokémon below
	special HiddenGrotto_CreateWildMon
	special HiddenGrotto_Clear
	removeobject VAR_LAST_TALKED
	dowildbattle
	release
	end

HiddenGrotto_EventScript_Item::
	finditem VAR_0x8004
	goto_if_eq VAR_0x8007, FALSE, HiddenGrotto_EventScript_End
	special HiddenGrotto_Clear
HiddenGrotto_EventScript_End:
	end

@ Stepping back onto the path out of the clearing.
HiddenGrotto_EventScript_Exit::
	lockall
	playse SE_EXIT
	special HiddenGrotto_Exit
	waitstate
	releaseall
	end
