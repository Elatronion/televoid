import "televoid-core" for Scene, Inventory, Dialogue, Audio

Audio.playBGM("Too Cool by Kevin MacLeod")

if(Inventory.has("helmet")) {
	Dialogue.load("res/dialogue/HUB - Sam complete.csv")
} else {
	if(Inventory.has("recipe")) {

		if(Inventory.has("cooked tv dinner")) {
			Dialogue.load("res/dialogue/HUB - Sam cooked tv dinner.csv")
		} else if(Inventory.has("unrefrigerated perfection salad")) {
			Dialogue.load("res/dialogue/HUB - Sam unrefrigerated perfection salad.csv")
		} else if(Inventory.has("perfection salad")) {
			Dialogue.load("res/dialogue/HUB - Sam perfection salad.csv")
		} else {
			Dialogue.load("res/dialogue/HUB - Sam incomplete.csv")
		}

	} else {
		Dialogue.load("res/dialogue/HUB - Sam.csv")
	}

}
