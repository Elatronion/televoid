import "televoid-core" for Scene, Inventory, Dialogue

if(Inventory.has("tv dinner")) {
	Dialogue.load("res/dialogue/Kitchen - Microwave complete.csv")
	Inventory.remove("tv dinner")
	Inventory.add("cooked tv dinner")
}
