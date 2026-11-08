KantoCeladonApartmentsFloor4_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, KantoCeladonApartmentsFloor4_OnTransition
    .byte 0

KantoCeladonApartmentsFloor4_OnTransition:
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 4
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR4, 16, 1
    end
