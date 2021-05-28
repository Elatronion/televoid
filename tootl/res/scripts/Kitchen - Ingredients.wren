import "televoid-core" for Scene, Inventory, Dialogue

if(Inventory.has("perfection salad")) {
	Dialogue.load("res/dialogue/Kitchen - Ingredients perfection salad.csv")
} else {
	if(Inventory.has("recipe") && !Inventory.has("unrefrigerated perfection salad")) {
		Dialogue.load("res/dialogue/Kitchen - Ingredients recipe.csv")
	}
	if(Inventory.has("recipe") && Inventory.has("unrefrigerated perfection salad")) {
		Dialogue.load("res/dialogue/Kitchen - Ingredients complete.csv")
	}
}
