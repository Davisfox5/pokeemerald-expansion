KantoCeladonApartmentsFloor5_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, KantoCeladonApartmentsFloor5_OnTransition
    .byte 0

KantoCeladonApartmentsFloor5_OnTransition:
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 5
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR5, 16, 1
    end
