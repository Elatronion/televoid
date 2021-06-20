import "televoid-core" for Scene, Inventory

var has_all_invitations = Inventory.has("party invitation (arcade)") && Inventory.has("party invitation (door 100)") && Inventory.has("party invitation (kitchen)") && Inventory.has("party invitation (stairs)")

if(has_all_invitations) {
	Scene.load("res/scenes/bowling.tmx")
} else {
	Scene.load("res/scenes/main_menu.tmx")
}
