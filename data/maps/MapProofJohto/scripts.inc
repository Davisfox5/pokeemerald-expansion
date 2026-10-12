MapProofJohto_MapScripts::
    .byte 0

MapProofJohto_Guide::
    lock
    faceplayer
    setvar VAR_TEMP_1, 77
    msgbox MapProofJohto_Text, MSGBOX_DEFAULT
    warp MAP_PROOF_KANTO, 10, 11
    waitstate
    release
    end

MapProofJohto_Text:
    .string "JOHTO MAP PIPELINE TEST.\nNext region, please!$"
