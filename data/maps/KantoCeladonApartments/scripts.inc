KantoCeladonApartments_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, KantoCeladonApartments_OnTransition
    .byte 0

KantoCeladonApartments_OnTransition:
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 1
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS, 16, 1
    end
