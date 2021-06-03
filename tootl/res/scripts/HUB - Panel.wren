import "televoid-core" for Scene, Inventory, Dialogue, Audio

var has_all_key_items = false // Inventory.has("item")

if(has_all_key_items) {
	Dialogue.load("res/dialogue/HUB - Panel complete.csv")
} else {
	Dialogue.load("res/dialogue/HUB - Panel.csv")
}
