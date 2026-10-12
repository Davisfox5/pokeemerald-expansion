MapProofSinnoh_MapScripts::
    .byte 0

MapProofSinnoh_Guide::
    lock
    faceplayer
    setvar VAR_TEMP_1, 77
    msgbox MapProofSinnoh_Text, MSGBOX_DEFAULT
    warp MAP_PROOF_UNOVA, 10, 11
    waitstate
    release
    end

MapProofSinnoh_Text:
    .string "SINNOH MAP PIPELINE TEST.\nNext region, please!$"
