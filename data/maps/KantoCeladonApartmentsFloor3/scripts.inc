KantoCeladonApartmentsFloor3_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, KantoCeladonApartmentsFloor3_OnTransition
    .byte 0

KantoCeladonApartmentsFloor3_OnTransition:
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 3
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR3, 16, 1
    end
