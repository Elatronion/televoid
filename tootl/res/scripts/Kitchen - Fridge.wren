import "televoid-core" for Scene, Inventory, Dialogue

var has_keyitem = Inventory.has("blow torch")
var has_reason_to_return = !(Inventory.has("tv dinner") || Inventory.has("cooked tv dinner"))

if(has_reason_to_return) {
	if(has_keyitem) {
		Dialogue.load("res/dialogue/Kitchen - Fridge complete.csv")
	} else {
		if(!Inventory.has("fridge ice")) {
			Dialogue.load("res/dialogue/Kitchen - Fridge no ice.csv")
		}
	}
} else {
	Dialogue.load("res/dialogue/Kitchen - Fridge no reason.csv")
}
