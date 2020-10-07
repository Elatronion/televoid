class Scene {
  foreign static load(path)
}

class Inventory {
  foreign static add(item_id)
  foreign static remove(item_id)
  foreign static has(item_id)
}
