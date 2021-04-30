import "televoid-core" for Dialogue, Scene, Audio, GameSaver
import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math, Options

class Bar {
  construct new(position, value, min_value, max_value) {
    _bar = GameObject.new(position, Vec2.new(Window.width/2, 45))
    _left_most_x = _bar.position.x - _bar.scale.x/2
    _right_most_x = _bar.position.x + _bar.scale.x/2


    var handle_x_position = _left_most_x + (value / max_value) * (_right_most_x - _left_most_x)

    _handle = GameObject.new(Vec2.new(handle_x_position, _bar.position.y), Vec2.new(70, 70))

    _handle_grabbed = false

    _min_value = min_value
    _max_value = max_value
    _value = value
  }
  max_value { _max_value }
  min_value { _min_value }

  value { ((_left_most_x+_handle.position.x)/(_right_most_x-_left_most_x) + 1) * _max_value }

  limit_handle_position() {
    if (_handle.position.x < _left_most_x) {
      _handle.position.x = _left_most_x
    } else if(_handle.position.x > _right_most_x) {
      _handle.position.x = _right_most_x
    }
  }

  update(delta) {
    var mouse_position = Vec2.new(Input.getMouseX(), Input.getMouseY())
    var mouse_gameobject = GameObject.new(mouse_position, Vec2.new(0, 0))

    if(Math.AABB(_handle, mouse_gameobject) && Input.getMouseDown(Input.MOUSE_LEFT)) {
      _handle_grabbed = true
    }

    if(Input.getMouse(Input.MOUSE_LEFT) && _handle_grabbed) {
      _handle.position.x = mouse_gameobject.position.x
      limit_handle_position()
    }

    if(Input.getMouseUp(Input.MOUSE_LEFT)) {
      _handle_grabbed = false
    }
  }

  render() {
    Window.render("SETTINGS HANDLE", _handle)
    Window.render("SETTINGS BAR", _bar)
  }
}

class OptionsMenu {
  construct init() {
    Options.load()
    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/bar.png", "SETTINGS BAR")
    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/handle.png", "SETTINGS HANDLE")
    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/button_voice.png", "SETTINGS TEST VOICE")
    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/button_sfx.png", "SETTINGS TEST SFX")
    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/button_bgm.png", "SETTINGS TEST BGM")

    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/button_back.png", "SETTINGS BUTTON BACK")

    _runtime = 0

    _bar_volume_master = Bar.new(Vec2.new(0, 150), Options.get("volume master"), 0, 1)
    _bar_volume_voice = Bar.new(Vec2.new(0, 50),   Options.get("volume voice"), 0, 1)
    _bar_volume_sfx = Bar.new(Vec2.new(0, -50),    Options.get("volume sfx"), 0, 1)
    _bar_volume_bgm = Bar.new(Vec2.new(0, -150),   Options.get("volume bgm"), 0, 1)

    _button_test_voice = GameObject.new(Vec2.new(-150, -300), Vec2.new(80, 80))
    _button_test_sfx = GameObject.new(Vec2.new(0, -300), Vec2.new(80, 80))
    _button_test_bgm = GameObject.new(Vec2.new(150, -300), Vec2.new(80, 80))

    _button_back = GameObject.new(Vec2.new(0, 300), Vec2.new(160, 80))
  }

  update(elapsedTime) {
    var mouse_position = Vec2.new(Input.getMouseX(), Input.getMouseY())
    var mouse_gameobject = GameObject.new(mouse_position, Vec2.new(0, 0))
    _runtime = _runtime + elapsedTime
    _bar_volume_master.update(elapsedTime)
    _bar_volume_voice.update(elapsedTime)
    _bar_volume_sfx.update(elapsedTime)
    _bar_volume_bgm.update(elapsedTime)

    if(Input.getMouseUp(Input.MOUSE_LEFT)) {
      Options.set(_bar_volume_master.value, _bar_volume_voice.value, _bar_volume_sfx.value, _bar_volume_bgm.value)
    }

    if(Math.AABB(_button_test_voice, mouse_gameobject) && Input.getMouseDown(Input.MOUSE_LEFT)) {
      var rand_no = Math.rand(1, 5)
      Audio.playVoice("ian no%(rand_no)")
    }
    if(Math.AABB(_button_test_sfx, mouse_gameobject) && Input.getMouseDown(Input.MOUSE_LEFT)) {
      Audio.playSFX("step1")
    }
    if(Math.AABB(_button_test_bgm, mouse_gameobject) && Input.getMouseDown(Input.MOUSE_LEFT)) {
      Audio.playBGM("Townie Loop Sample")
    }

    if(Math.AABB(_button_back, mouse_gameobject) && Input.getMouseDown(Input.MOUSE_LEFT)) {
      Scene.load("res/scenes/main_menu.tmx")
    }
  }

  render() {
    _bar_volume_master.render()
    _bar_volume_voice.render()
    _bar_volume_sfx.render()
    _bar_volume_bgm.render()

    Window.render("SETTINGS TEST VOICE", _button_test_voice)
    Window.render("SETTINGS TEST SFX", _button_test_sfx)
    Window.render("SETTINGS TEST BGM", _button_test_bgm)
    Window.render("SETTINGS BUTTON BACK", _button_back)
  }
}

var game = OptionsMenu.init()
