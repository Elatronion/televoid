import "televoid/core" for Scene, Inventory

if (Inventory.has("key") && Inventory.has("blue_key")) {
  Inventory.remove("key")
  Inventory.remove("blue_key")
  Scene.load("res/scenes/demo/Win.tmx")
} else {
  System.print("It's Locked, need keys.") // This will only print in terminal
}

// Dialogue not yet ready...
//Dialogue.load("some_dialogue.dlg") // dlg = dialogue
