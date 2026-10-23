OlivineMart_MapScripts::
    .byte 0

OlivineMart_Clerk::
    lock
    faceplayer
    message gText_HowMayIServeYou
    waitmessage
    pokemart OlivineMart_Stock
    msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
    release
    end

    .align 2
OlivineMart_Stock:
    .2byte ITEM_POTION
    .2byte ITEM_ANTIDOTE
    .2byte ITEM_PARALYZE_HEAL
    pokemartlistend
