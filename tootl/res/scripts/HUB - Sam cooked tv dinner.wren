import "televoid-core" for Scene, Inventory, Dialogue, Audio

if(Inventory.has("recipe")) {

	if(Inventory.has("unrefrigerated perfection salad")) {
		Dialogue.load("res/dialogue/HUB - Sam unrefrigerated perfection salad.csv")
	} else if(Inventory.has("perfection salad")) {
		Dialogue.load("res/dialogue/HUB - Sam perfection salad.csv")
	} else {
		Audio.playBGM("warmup by brutalmoose")
	}

} else {
	Dialogue.load("res/dialogue/HUB - Sam.csv")
}
