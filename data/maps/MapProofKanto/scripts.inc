MapProofKanto_MapScripts::
    .byte 0

MapProofKanto_Guide::
    lock
    faceplayer
    setvar VAR_TEMP_1, 77
    msgbox MapProofKanto_Text, MSGBOX_DEFAULT
    warp MAP_PROOF_SINNOH, 10, 11
    waitstate
    release
    end

MapProofKanto_Text:
    .string "KANTO MAP PIPELINE TEST.\nNext region, please!$"
