IcePath_1F_MapScripts::
    map_script MAP_SCRIPT_ON_TRANSITION, IcePath_1F_OnTransition
    .byte 0

IcePath_1F_OnTransition:
    goto_if_set FLAG_ICE_PATH_FALLEN_BOULDERS_HIDDEN, IcePath_1F_EventScript_End
    setflag FLAG_HIDE_ICE_PATH_BOULDER_1_FALLEN
    setflag FLAG_HIDE_ICE_PATH_BOULDER_2_FALLEN
    setflag FLAG_HIDE_ICE_PATH_BOULDER_3_FALLEN
    setflag FLAG_HIDE_ICE_PATH_BOULDER_4_FALLEN
    setflag FLAG_ICE_PATH_FALLEN_BOULDERS_HIDDEN
IcePath_1F_EventScript_End:
    end
