class Scene {
  foreign static load(path)
}

class Inventory {
  foreign static add(item_name)
  foreign static remove(item_name)
  foreign static has(item_name)
}

class Dialogue {
  foreign static load(dialogue_file)
}
