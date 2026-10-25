ApocPalletLab_MapScripts::
    .byte 0

ApocPalletLab_Blue::
    lock
    faceplayer
    msgbox ApocPalletLab_Blue_Text, MSGBOX_DEFAULT
    release
    end

ApocPalletLab_Blue_Text:
    .string "BLUE: I'm KANTO'S professor now.\nMy grandfather has retired.\pPALLET is still home.$"

ApocPalletLab_Research::
    msgbox ApocPalletLab_Research_Text, MSGBOX_SIGN
    end

ApocPalletLab_Research_Text:
    .string "Research notes and field records.\nPALLET POKEMON LAB.$"

ApocPalletLab_Computer::
    msgbox ApocPalletLab_Computer_Text, MSGBOX_SIGN
    end

ApocPalletLab_Computer_Text:
    .string "The laboratory's research computer.$"
