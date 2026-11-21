CherrygroveMart_MapScripts::
    .byte 0

CherrygroveMart_Clerk::
    lock
    faceplayer
    message gText_HowMayIServeYou
    waitmessage
    pokemart CherrygroveMart_Stock
    msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
    release
    end

CherrygroveMart_DiscountClerk::
    lock
    faceplayer
    msgbox CherrygroveMart_Text_Discount, MSGBOX_DEFAULT
    setflag FLAG_SHOP_DISCOUNT
    pokemart CherrygroveMart_SecondStock
    clearflag FLAG_SHOP_DISCOUNT
    msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
    release
    end

CherrygroveMart_Text_Discount:
    .string "Welcome to the DISCOUNT CORNER!\nEverything here is a quarter off.$"

    .align 2
CherrygroveMart_Stock:
    .2byte ITEM_POTION
    .2byte ITEM_ANTIDOTE
    .2byte ITEM_PARALYZE_HEAL
    pokemartlistend

    .align 2
CherrygroveMart_SecondStock:
    .2byte ITEM_POKE_BALL
    .2byte ITEM_POTION
    .2byte ITEM_ANTIDOTE
    .2byte ITEM_PARALYZE_HEAL
    pokemartlistend
