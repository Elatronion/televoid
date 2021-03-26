import "televoid-core" for Dialogue, Scene, Audio, GameSaver
import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math, Options

class MainMenu {
  construct init() {
    Options.load()
    _scene_to_load = "res/scenes/intro.tmx"

    ResourceManager.loadTexture("res/HGE/BLACK.png", "SIDE BAR")
    ResourceManager.loadTexture("res/scripts/minigames/settings_assets/gear.png", "SETTINGS GEAR")

    ResourceManager.loadTexture("res/textures/sprites/MainMenu/wood.png", "WOOD TABLE BACKGROUND")
    ResourceManager.loadTexture("res/textures/sprites/MainMenu/TelevoidLogo.png", "VHS")

    ResourceManager.loadTexture("res/textures/sprites/MainMenu/VCR_top.png", "VCR top")
    ResourceManager.loadTexture("res/textures/sprites/MainMenu/VCR_bottom.png", "VCR bottom")

    ResourceManager.loadAudio("res/audio/sfx/vcr/Flap In.wav", "VCR Flap In")
    ResourceManager.loadAudio("res/audio/sfx/vcr/Flap Out.wav", "VCR Flap Out")


    // Other Resources //
    ResourceManager.loadAudio("res/audio/bgm/gone by Brutalmoose.wav", "Gone")
    ResourceManager.loadTexture("res/imv/intro_assets/ian-sit-1.png", "IMV INTRO Ian Sit 1")
    ResourceManager.loadTexture("res/imv/intro_assets/door-close.png", "IMV INTRO Door Close")
    ResourceManager.loadTexture("res/imv/intro_assets/door-open.png", "IMV INTRO Door Open")
    ResourceManager.loadTexture("res/imv/intro_assets/door-static.png", "IMV INTRO Door Static")
    ResourceManager.loadTexture("res/imv/intro_assets/white.png", "IMV INTRO WHITE")

    ResourceManager.loadTexture("res/imv/intro_assets/chair.png", "IMV INTRO CHAIR")
    ResourceManager.loadTexture("res/imv/intro_assets/tv-table.png", "IMV INTRO TV-TABLE")
    ResourceManager.loadTexture("res/imv/intro_assets/plant.png", "IMV INTRO PLANT")
    ResourceManager.loadTexture("res/imv/intro_assets/ex-bike.png", "IMV INTRO EX-BIKE")
    ResourceManager.loadTexture("res/imv/intro_assets/printer.png", "IMV INTRO PRINTER")

    ResourceManager.loadTexture("res/imv/intro_assets/ian-wind-1.png", "IMV INTRO IAN-WIND-frame1")
    ResourceManager.loadTexture("res/imv/intro_assets/ian-wind-2.png", "IMV INTRO IAN-WIND-frame2")
    ResourceManager.loadTexture("res/imv/intro_assets/ian-wind-3.png", "IMV INTRO IAN-WIND-frame3")

    _runtime = 0

    _vcr_flap_in = false
    _clicked_vhs = false
    _y_click_offset = 0

    _VHS_object = GameObject.new(
      Vec2.new(0, 0),
      Vec2.new(197 * 2, 116 * 2)
    )

    _gear_margin = 8
    _gear_object = GameObject.new(
      Vec2.new(0, 0),
      Vec2.new(32+16, 32+16)
    )
  }

  update(elapsedTime) {
    _runtime = _runtime + elapsedTime

    var vhs_desired_y = 0

    if(Input.getMouseDown(Input.MOUSE_LEFT)) {
      _y_click_offset = _VHS_object.position.y - Input.getMouseY()
      if(Math.AABB(_VHS_object.position.x, _VHS_object.position.y, _VHS_object.scale.x, _VHS_object.scale.y, Input.getMouseX(), Input.getMouseY(), 0, 0)) {
          _clicked_vhs = true
      }
    } else if(Input.getMouseUp(Input.MOUSE_LEFT)) {
      _clicked_vhs = false
    }

    if(Input.getMouse(Input.MOUSE_LEFT) && _clicked_vhs) {
      vhs_desired_y = Input.getMouseY() + _y_click_offset
    } else {
      vhs_desired_y = _runtime.cos*16
    }

    var smooth_speed = 10
    _VHS_object.position.y = _VHS_object.position.y + (vhs_desired_y - _VHS_object.position.y) * smooth_speed * elapsedTime

    // Flap Sound
    if(_VHS_object.position.y > 247 && !_vcr_flap_in) {
      Audio.playSFX("VCR Flap In")
      _vcr_flap_in = true
    } else if(_VHS_object.position.y < 247 && _vcr_flap_in) {
      Audio.playSFX("VCR Flap Out")
      _vcr_flap_in = false
    }

    // VHS has entered VCR
    if(_VHS_object.position.y > 415) {
      //Scene.load(_scene_to_load)
      GameSaver.load()
    }

    var mouse_position = Vec2.new(Input.getMouseX(), Input.getMouseY())
    var mouse_gameobject = GameObject.new(mouse_position, Vec2.new(0, 0))
    _gear_object.position.x = Window.width/2 - _gear_object.scale.x/2 - _gear_margin
    _gear_object.position.y = Window.height/2 - _gear_object.scale.y/2 - _gear_margin
    _gear_object.rotation = _gear_object.rotation + 50 * elapsedTime
    if(Math.AABB(_gear_object, mouse_gameobject) && Input.getMouseDown(Input.MOUSE_LEFT)) {
      Scene.load("res/scenes/settings_menu.tmx")
    }
  }

  render() {
    Window.render("SETTINGS GEAR", _gear_object)
    var side_bar_scale = Vec2.new(68, Window.height)
    Window.render("SIDE BAR", Window.width/2 - side_bar_scale.x/2, 0, side_bar_scale.x, side_bar_scale.y)

    var VHS_position = Vec2.new(0, 450)
    var VHS_scale = Vec2.new(143*4, 86*4)

    Window.render("VCR top",
      VHS_position.x, VHS_position.y,
      VHS_scale.x, VHS_scale.y
    )

    Window.render("VHS",
      _VHS_object.position.x, _VHS_object.position.y,
      _VHS_object.scale.x, _VHS_object.scale.y
    )

    Window.render("VCR bottom",
      VHS_position.x, VHS_position.y,
      VHS_scale.x, VHS_scale.y
    )


    Window.render("WOOD TABLE BACKGROUND",
      0, 0,
      282*8, 175*8
    )

  }
}

var game = MainMenu.init()
