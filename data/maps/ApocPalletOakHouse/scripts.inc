ApocPalletOakHouse_MapScripts::
    .byte 0

ApocPalletOakHouse_Oak::
    lock
    faceplayer
    msgbox ApocPalletOakHouse_Oak_Text, MSGBOX_DEFAULT
    release
    end

ApocPalletOakHouse_Oak_Text:
    .string "OAK: I've retired from research.\nBLUE is KANTO'S professor now.\pIt's good to have him here in PALLET.$"

ApocPalletOakHouse_Family::
    msgbox ApocPalletOakHouse_Family_Text, MSGBOX_SIGN
    end

ApocPalletOakHouse_Family_Text:
    .string "The OAK family home.$"
