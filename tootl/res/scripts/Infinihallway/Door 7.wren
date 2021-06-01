import "televoid-core" for Scene, Inventory, Dialogue

if (Inventory.has("recipe")) {
  Dialogue.load("res/dialogue/Infinihallway/Door 7 recipe.csv")
} else {
  Dialogue.load("res/dialogue/Infinihallway/Door 7.csv")
}
