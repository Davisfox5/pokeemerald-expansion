KantoCeladonApartmentsFloor6_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, KantoCeladonApartmentsFloor6_OnTransition
    .byte 0

KantoCeladonApartmentsFloor6_OnTransition:
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 6
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR6, 16, 1
    end
