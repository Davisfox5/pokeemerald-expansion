MapProofUnova_MapScripts::
    .byte 0

MapProofUnova_Guide::
    lock
    faceplayer
    setvar VAR_TEMP_1, 77
    msgbox MapProofUnova_Text, MSGBOX_DEFAULT
    warp MAP_PROOF_HOENN, 10, 11
    waitstate
    release
    end

MapProofUnova_Text:
    .string "UNOVA MAP PIPELINE TEST.\nNext region, please!$"
