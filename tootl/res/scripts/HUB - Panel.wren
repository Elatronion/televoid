import "televoid-core" for Scene, Inventory, Dialogue, Audio

var has_all_key_items = Inventory.has("VHS") && Inventory.has("helmet") && Inventory.has("brutal moose")

if(has_all_key_items) {
	Dialogue.load("res/dialogue/HUB - Panel complete.csv")
} else {
	Dialogue.load("res/dialogue/HUB - Panel.csv")
}
