VioletMart_MapScripts::
    .byte 0

VioletMart_Clerk::
    lock
    faceplayer
    message gText_HowMayIServeYou
    waitmessage
    pokemart VioletMart_Stock
    msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
    release
    end

    .align 2
VioletMart_Stock:
    .2byte ITEM_POKE_BALL
    .2byte ITEM_POTION
    .2byte ITEM_ANTIDOTE
    .2byte ITEM_PARALYZE_HEAL
    .2byte ITEM_AWAKENING
    .2byte ITEM_ESCAPE_ROPE
    .2byte ITEM_REPEL
    pokemartlistend
