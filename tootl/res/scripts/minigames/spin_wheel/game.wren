import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio, Inventory, Dialogue

class Breakout {
  construct init() {
    // ResourceManager.loadTexture("res/scripts/minigames/spin_wheel/assets/image.png", "")
    // ResourceManager.loadTexture("res/HGE/DEFAULT NORMAL.png", "WHEEL")
    ResourceManager.loadTexture("res/scripts/minigames/spin_wheel/assets/wheel.png", "spin_wheel WHEEL")
    ResourceManager.loadTexture("res/scripts/minigames/spin_wheel/assets/arrow.png", "spin_wheel ARROW")

    ResourceManager.loadAudio("res/scripts/minigames/spin_wheel/assets/arrow_bonk1.wav", "spin_wheel arrow bonk 1")
    ResourceManager.loadAudio("res/scripts/minigames/spin_wheel/assets/arrow_bonk2.wav", "spin_wheel arrow bonk 2")
    ResourceManager.loadAudio("res/scripts/minigames/spin_wheel/assets/arrow_bonk3.wav", "spin_wheel arrow bonk 3")
    ResourceManager.loadAudio("res/scripts/minigames/spin_wheel/assets/arrow_bonk4.wav", "spin_wheel arrow bonk 4")
    ResourceManager.loadAudio("res/scripts/minigames/spin_wheel/assets/arrow_bonk5.wav", "spin_wheel arrow bonk 5")


    var scalar = 2
    _objectWheel = GameObject.new(
        Vec2.new(0, 0),
        Vec2.new(300 * scalar, 300 * scalar)
    )

    _objectArrow = GameObject.new(
        Vec2.new(_objectWheel.scale.x/2, 0),
        Vec2.new(75 * scalar, 75 * scalar)
    )

    _turn_speed = 0
    _target_turn_speed = 0
    _spinning_to_play = false
    _num_sections = 8
    _last_section = -1

    _winning_section = 1
    _purple_section = 7

    _landed_section = 0
    _landed_countdown = 0

    _min_spin_speed = 550

    Dialogue.load("res/dialogue/wheelyfun play.csv")
  }

  landed(section) {
    _landed_section = section
    _landed_countdown = 5

    System.print("Stopped at section %(section)")
    if(section == _winning_section) {
      win()
    } else {
      loose()
    }
  }

  win() {
    System.print("YOU WIN!")
    Inventory.add("wheely-fun ticket")
  }

  loose() {
    System.print("YOU LOOSE!")
  }

  bonk_arrow() {
    var random_sound_id = Math.rand(1, 6)
    Audio.playSFX("spin_wheel arrow bonk %(random_sound_id)")
    _objectArrow.position.y = -25 * (0.25+_turn_speed/1000)
    _objectArrow.rotation = -15 * (0.25+_turn_speed/1000)
  }

  update(elapsedTime) {
    if (_landed_countdown > 0) {
      _landed_countdown = _landed_countdown - elapsedTime
    }

    if (Input.getKey(Input.KEY_SPACE) && !_spinning_to_play) {
      _target_turn_speed = 1000
    } else {
      _target_turn_speed = 0
    }

    if(Input.getKeyUp(Input.KEY_SPACE) && _turn_speed > _min_spin_speed) {
      _spinning_to_play = true
    }

    var section = (((_objectWheel.rotation-22.5).floor % 360) / (360/_num_sections)).floor
    //System.print("At section %(section)")

    if(_spinning_to_play && _turn_speed < 1) {
      _spinning_to_play = false
      landed(section)
    }

    _turn_speed = _turn_speed + (_target_turn_speed - _turn_speed) * 0.5 * elapsedTime
    _objectWheel.rotation = _objectWheel.rotation + _turn_speed * elapsedTime

    if(_last_section != section) {
      _last_section = section
      bonk_arrow()
    }

    _objectArrow.rotation = _objectArrow.rotation + (0 - _objectArrow.rotation) * 10.0 * elapsedTime
    _objectArrow.position.y = _objectArrow.position.y + (0 - _objectArrow.position.y) * 10.0 * elapsedTime
  }

  render() {
    Window.render("spin_wheel ARROW", _objectArrow)
    Window.render("spin_wheel WHEEL", _objectWheel)
    var padding = 10
    Window.renderText("WHEELY FUN", 0, _objectWheel.scale.y/2 + padding, 100)
    var string = "HOLD SPACE"
    if (_spinning_to_play) {
      string = "LET'S SEE WHAT YOU GOT!"
    } else if (!_spinning_to_play && _turn_speed > _min_spin_speed) {
      string = "RELEASE SPACE"
    } else if(_landed_countdown > 0) {
      if(_landed_section == _winning_section) {
        string = "YOU WIN"
      } else if(_landed_section == _purple_section) {
        string = "YOU WON AN INVITATION"
        Inventory.add("party invitation (arcade)")
      } else {
        string = "YOU LOOSE"
      }
    }
    Window.renderText(string, 0, -_objectWheel.scale.y/2 - padding - 35, 100)
  }
}

var game = Breakout.init()
