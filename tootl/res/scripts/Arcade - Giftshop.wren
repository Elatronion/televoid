import "televoid-core" for Scene, Inventory, Dialogue

if(Inventory.has("brutal moose")) {
	Dialogue.load("res/dialogue/Arcade - Giftshop complete.csv")
} else {
	if(Inventory.has("breakout ticket") && Inventory.has("wheely-fun ticket") && Inventory.has("spacerocks ticket") && Inventory.has("invaders ticket")) {
		Dialogue.load("res/dialogue/Arcade - Giftshop win.csv")
	} else {
		Dialogue.load("res/dialogue/Arcade - Giftshop loose.csv")
	}
}
