CanalaveCity_MapScripts::
	.byte 0

CanalaveCity_Collector::
	msgbox CanalaveCity_Text_Collector, MSGBOX_NPC
	end

CanalaveCity_Text_Collector:
	.string "What's up? Your clothes look damp.\pMaybe you took the boat to Iron\nIsland to get a Hidden Machine,\lright?$"

CanalaveCity_Lass::
	msgbox CanalaveCity_Text_Lass, MSGBOX_NPC
	end

CanalaveCity_Text_Lass:
	.string "My PSYDUCK is very close to me. That\nmakes its Return move powerful!$"

CanalaveCity_Breeder::
	msgbox CanalaveCity_Text_Breeder, MSGBOX_NPC
	end

CanalaveCity_Text_Breeder:
	.string "Welcome to Canalave City, the Canal\nCity!\pIn the old days, this port used to\nbe a hub of cargo transportation.$"

CanalaveCity_Student::
	msgbox CanalaveCity_Text_Student, MSGBOX_NPC
	end

CanalaveCity_Text_Student:
	.string "I'm going to study at the library.\pI've taken an interest in the\nsubject of time and space.$"

CanalaveCity_OldMan::
	msgbox CanalaveCity_Text_OldMan, MSGBOX_NPC
	end

CanalaveCity_Text_OldMan:
	.string "The Gym Leader and Trainers of this\nGym often go out to Iron Island.\pThey not only train their POKéMON,\nbut they work out as well.$"

CanalaveCity_SailorEldritch::
	msgbox CanalaveCity_Text_SailorEldritch, MSGBOX_NPC
	end

CanalaveCity_Text_SailorEldritch:
	.string "You just tell me whenever you need\nto set sail!$"

CanalaveCity_Psyduck::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PSYDUCK, CRY_MODE_NORMAL
	msgbox CanalaveCity_Text_Psyduck, MSGBOX_DEFAULT
	waitmoncry
	release
	end

CanalaveCity_Text_Psyduck:
	.string "PSYDUCK: Gua?$"

CanalaveCity_Sign_Library::
	msgbox CanalaveCity_Sign_Library_Text, MSGBOX_SIGN
	end

CanalaveCity_Sign_Library_Text:
	.string "Canalave Library$"

CanalaveCity_Sign_Gym::
	msgbox CanalaveCity_Sign_Gym_Text, MSGBOX_SIGN
	end

CanalaveCity_Sign_Gym_Text:
	.string "Canalave City POKéMON Gym Leader:\nByron The Man with the Steel Body!$"

CanalaveCity_Sign_Dock::
	msgbox CanalaveCity_Sign_Dock_Text, MSGBOX_SIGN
	end

CanalaveCity_Sign_Dock_Text:
	.string "Canalave Dock$"

CanalaveCity_Sign_City::
	msgbox CanalaveCity_Sign_City_Text, MSGBOX_SIGN
	end

CanalaveCity_Sign_City_Text:
	.string "Canalave City Cargo Port$"

CanalaveCity_Sign_EldritchHouse::
	msgbox CanalaveCity_Sign_EldritchHouse_Text, MSGBOX_SIGN
	end

CanalaveCity_Sign_EldritchHouse_Text:
	.string "Sailor Eldritch's House$"

CanalaveCity_Sign_HarborInn::
	msgbox CanalaveCity_Sign_HarborInn_Text, MSGBOX_SIGN
	end

CanalaveCity_Sign_HarborInn_Text:
	.string "Harbor Inn... The text is too faded\nto read.$"

CanalaveCity_HarborInnDoor::
	msgbox CanalaveCity_HarborInnDoor_Text, MSGBOX_SIGN
	end

CanalaveCity_HarborInnDoor_Text:
	.string "The door is tightly shut. It appears\nimpossible to open.$"

CanalaveCity_Gate::
	msgbox CanalaveCity_Gate_Text, MSGBOX_SIGN
	end

CanalaveCity_Gate_Text:
	.string "ROUTE 218 GATE\nThe gate is closed for now.$"
