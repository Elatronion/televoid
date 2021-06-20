import "televoid-core" for Scene, Inventory, Dialogue

if(Inventory.has("cooked tv dinner")) {
	Dialogue.load("res/dialogue/metavoid - tv complete.csv")
	Inventory.remove("cooked tv dinner")
} else if(Inventory.has("tv dinner")) {
	Dialogue.load("res/dialogue/metavoid - tv hint.csv")
} else {
	Dialogue.load("res/dialogue/metavoid - tv.csv")
}
