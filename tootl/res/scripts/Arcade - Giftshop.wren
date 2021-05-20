import "televoid-core" for Scene, Inventory, Dialogue

var has_keyitem = false // Inventory.has("ARCADE KEY")

if(has_keyitem) {
	Dialogue.load("res/dialogue/Arcade - Giftshop complete.csv")
} else {
	if(Inventory.has("breakout ticket") && Inventory.has("wheely-fun ticket")) {
		Dialogue.load("res/dialogue/Arcade - Giftshop win.csv")
	} else {
		Dialogue.load("res/dialogue/Arcade - Giftshop loose.csv")
	}
}
