MapProofHoenn_MapScripts::
    .byte 0

MapProofHoenn_Guide::
    lock
    faceplayer
    setvar VAR_TEMP_1, 77
    msgbox MapProofHoenn_Text, MSGBOX_DEFAULT
    warp MAP_PROOF_JOHTO, 10, 11
    waitstate
    release
    end

MapProofHoenn_Text:
    .string "HOENN MAP PIPELINE TEST.\nNext region, please!$"
