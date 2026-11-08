KantoCeladonApartmentsFloor2_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, KantoCeladonApartmentsFloor2_OnTransition
    .byte 0

KantoCeladonApartmentsFloor2_OnTransition:
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 2
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR2, 16, 1
    end
