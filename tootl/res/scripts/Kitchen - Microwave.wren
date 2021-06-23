import "televoid-core" for Scene, Inventory, Dialogue

if(Inventory.has("frozen tv dinner")) {
	Dialogue.load("res/dialogue/Kitchen - Microwave complete.csv")
	Inventory.remove("frozen tv dinner")
	Inventory.add("cooked tv dinner")
}
