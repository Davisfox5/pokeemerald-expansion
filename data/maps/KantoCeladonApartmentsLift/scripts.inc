KantoCeladonApartmentsLift_MapScripts::
    .byte 0

KantoCeladonApartmentsLift_EventScript_Attendant::
    lock
    faceplayer
    message KantoCeladonApartmentsLift_Text_WhichFloor
    waitmessage
    setvar VAR_0x8004, 6
    subvar VAR_0x8004, VAR_APOC_CELADON_LIFT_FLOOR
    dynmultichoice 0, 0, FALSE, 7, VAR_0x8004, DYN_MULTICHOICE_CB_NONE, KantoCeladonApartmentsLift_Text_6F, KantoCeladonApartmentsLift_Text_5F, KantoCeladonApartmentsLift_Text_4F, KantoCeladonApartmentsLift_Text_3F, KantoCeladonApartmentsLift_Text_2F, KantoCeladonApartmentsLift_Text_1F, KantoCeladonApartmentsLift_Text_Exit
    switch VAR_RESULT
    case 0, KantoCeladonApartmentsLift_EventScript_6F
    case 1, KantoCeladonApartmentsLift_EventScript_5F
    case 2, KantoCeladonApartmentsLift_EventScript_4F
    case 3, KantoCeladonApartmentsLift_EventScript_3F
    case 4, KantoCeladonApartmentsLift_EventScript_2F
    case 5, KantoCeladonApartmentsLift_EventScript_1F
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_6F::
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR6, 16, 1
    goto_if_eq VAR_APOC_CELADON_LIFT_FLOOR, 6, KantoCeladonApartmentsLift_EventScript_Exit
    copyvar VAR_0x8005, VAR_APOC_CELADON_LIFT_FLOOR
    setvar VAR_0x8006, 6
    call KantoCeladonApartmentsLift_EventScript_Ride
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 6
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_5F::
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR5, 16, 1
    goto_if_eq VAR_APOC_CELADON_LIFT_FLOOR, 5, KantoCeladonApartmentsLift_EventScript_Exit
    copyvar VAR_0x8005, VAR_APOC_CELADON_LIFT_FLOOR
    setvar VAR_0x8006, 5
    call KantoCeladonApartmentsLift_EventScript_Ride
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 5
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_4F::
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR4, 16, 1
    goto_if_eq VAR_APOC_CELADON_LIFT_FLOOR, 4, KantoCeladonApartmentsLift_EventScript_Exit
    copyvar VAR_0x8005, VAR_APOC_CELADON_LIFT_FLOOR
    setvar VAR_0x8006, 4
    call KantoCeladonApartmentsLift_EventScript_Ride
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 4
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_3F::
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR3, 16, 1
    goto_if_eq VAR_APOC_CELADON_LIFT_FLOOR, 3, KantoCeladonApartmentsLift_EventScript_Exit
    copyvar VAR_0x8005, VAR_APOC_CELADON_LIFT_FLOOR
    setvar VAR_0x8006, 3
    call KantoCeladonApartmentsLift_EventScript_Ride
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 3
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_2F::
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS_FLOOR2, 16, 1
    goto_if_eq VAR_APOC_CELADON_LIFT_FLOOR, 2, KantoCeladonApartmentsLift_EventScript_Exit
    copyvar VAR_0x8005, VAR_APOC_CELADON_LIFT_FLOOR
    setvar VAR_0x8006, 2
    call KantoCeladonApartmentsLift_EventScript_Ride
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 2
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_1F::
    setdynamicwarp MAP_KANTO_CELADON_APARTMENTS, 16, 1
    goto_if_eq VAR_APOC_CELADON_LIFT_FLOOR, 1, KantoCeladonApartmentsLift_EventScript_Exit
    copyvar VAR_0x8005, VAR_APOC_CELADON_LIFT_FLOOR
    setvar VAR_0x8006, 1
    call KantoCeladonApartmentsLift_EventScript_Ride
    setvar VAR_APOC_CELADON_LIFT_FLOOR, 1
    goto KantoCeladonApartmentsLift_EventScript_Exit
    end

KantoCeladonApartmentsLift_EventScript_Ride::
    closemessage
    applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
    waitmovement 0
    waitse
    special MoveElevator
    waitstate
    return

KantoCeladonApartmentsLift_EventScript_Exit::
    closemessage
    release
    end

KantoCeladonApartmentsLift_Text_6F:
    .string "6F$"

KantoCeladonApartmentsLift_Text_5F:
    .string "5F$"

KantoCeladonApartmentsLift_Text_4F:
    .string "4F$"

KantoCeladonApartmentsLift_Text_3F:
    .string "3F$"

KantoCeladonApartmentsLift_Text_2F:
    .string "2F$"

KantoCeladonApartmentsLift_Text_1F:
    .string "1F$"

KantoCeladonApartmentsLift_Text_Exit:
    .string "EXIT$"

KantoCeladonApartmentsLift_Text_WhichFloor:
    .string "Which floor would you like?$"
