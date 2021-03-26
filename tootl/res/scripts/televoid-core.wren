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

class Minigame {
  foreign static load(minigame_file, canExit)
  static load(minigame_file) {
    load(minigame_file, true)
  }
}

class Audio {
  foreign static playVoice(name)
  foreign static playBGM(name)
  foreign static playSFX(name)
}

class Animation {
  foreign static load(imv_file)
}

class Character {
  foreign static walk(x, y, character_name)
  static walk(vec2, character_name) {
    walk(vec2.x, vec2.y, character_name)
  }
}

class GameSaver {
  foreign static save()
  foreign static load()
}
