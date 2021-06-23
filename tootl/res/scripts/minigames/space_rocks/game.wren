import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio, Inventory, Dialogue

class SpaceEntity {
  construct new(position, velocity, scale, sprite) {
    _object = GameObject.new(
        position,
        Vec2.new(scale, scale)
    )
    _velocity = velocity
    _sprite = sprite
  }

  update(elapsedTime) {
    _object.position.x = _object.position.x + _velocity.x * elapsedTime
    _object.position.y = _object.position.y + _velocity.y * elapsedTime

    var game_width = 800
    var game_height = 600

    if (_object.position.x - _object.scale.x > game_width/2) {
      _object.position.x = -game_width/2 - _object.scale.x
    }
    if (_object.position.x + _object.scale.x < -game_width/2) {
      _object.position.x = game_width/2 + _object.scale.x
    }
    if (_object.position.y - _object.scale.y > game_height/2) {
      _object.position.y = -game_height/2 - _object.scale.y
    }
    if (_object.position.y + _object.scale.y < -game_height/2) {
      _object.position.y = game_height/2 + _object.scale.y
    }
  }

  render() {
    Window.render(_sprite, _object)
  }

  object { _object }

  position { _object.position }
  position=(value) { _object.position = value }
  rotation { _object.rotation }
  rotation=(value) { _object.rotation = value }
  scale { _object.scale.x }
  scale=(value) { _object.scale = Vec2.new(value, value) }
  velocity { _velocity }
  velocity=(value) { _velocity = value }
}


class SpaceBullet {

    construct new(position, velocity, scale, sprite) {
      _object = GameObject.new(
          position,
          Vec2.new(scale, scale)
      )
      _velocity = velocity
      _sprite = sprite
      _destroyed = false
      _timer = 0
    }
  destroyed { _destroyed }

  destroy() {
    _destroyed = true
  }

  update(elapsedTime) {
      _object.position.x = _object.position.x + _velocity.x * elapsedTime
      _object.position.y = _object.position.y + _velocity.y * elapsedTime
      var game_width = 800
      var game_height = 600

      if (_object.position.x - _object.scale.x > game_width/2) {
        _object.position.x = -game_width/2 - _object.scale.x
      }
      if (_object.position.x + _object.scale.x < -game_width/2) {
        _object.position.x = game_width/2 + _object.scale.x
      }
      if (_object.position.y - _object.scale.y > game_height/2) {
        _object.position.y = -game_height/2 - _object.scale.y
      }
      if (_object.position.y + _object.scale.y < -game_height/2) {
        _object.position.y = game_height/2 + _object.scale.y
      }
      _timer = _timer + elapsedTime
      if(_timer >= 3){
          destroy()
      }
  }


    render() {
      Window.render(_sprite, _object)
    }

    object { _object }

    position { _object.position }
    position=(value) { _object.position = value }
    rotation { _object.rotation }
    rotation=(value) { _object.rotation = value }
    scale { _object.scale.x }
    scale=(value) { _object.scale = Vec2.new(value, value) }
    velocity { _velocity }
    velocity=(value) { _velocity = value }
}

class SpaceRocks {
  construct init() {
    ResourceManager.loadTexture("res/scripts/minigames/space_rocks/assets/ship_F.png", "space_rocks player ship")
    ResourceManager.loadTexture("res/scripts/minigames/space_rocks/assets/meteor_large.png", "space_rocks meteor_large")
    ResourceManager.loadTexture("res/scripts/minigames/space_rocks/assets/ship_B.png", "space_rocks star_tiny")

    //ResourceManager.loadAudio("res/scripts/minigames/spin_wheel/assets/arrow_bonk1.wav", "spin_wheel arrow bonk 1")
    _gamerunning = true
    _player_won = false

    _gameWidth = 800
    _gameHeight = 600

    _entityShip = SpaceEntity.new(
        Vec2.new(0, 0),
        Vec2.new(0, 10),
        32,
        "space_rocks player ship"
    )

    _canShootTimer = 0
    _canShoot = true

    _spacerocks = []
    _numSpacerocks = 0
    _bullets = []

    generate_rock_field()


    Dialogue.load("res/dialogue/Arcade - Play Spacerocks.csv")
  }

  generate_rock_field() {
    for (i in 0..0) {
      generate_random_spacerock(128)
    }
    for (i in 0..1) {
      generate_random_spacerock(64)
    }
    for (i in 0..2) {
      generate_random_spacerock(32)
    }
  }

  reset() {
    _gamerunning = true
    _player_won = false

    _gameWidth = 800
    _gameHeight = 600
    _bullets = []

    _entityShip = SpaceEntity.new(
        Vec2.new(0, 0),
        Vec2.new(0, 10),
        32,
        "space_rocks player ship"
    )

    _canShootTimer = 0
    _canShoot = true
    _entityBullet = SpaceEntity.new(
        Vec2.new(0, 0),
        Vec2.new(0, 10),
        25,
        "space_rocks star_tiny"
    )

    _spacerocks = []
    _numSpacerocks = 0

    generate_rock_field()
  }

  remove_spacerock(index) {
    _spacerocks.removeAt(index)
    _numSpacerocks = _numSpacerocks - 1
  }

  add_spacerock(position, velocity, scale) {
    _numSpacerocks = _numSpacerocks + 1
    _spacerocks.add(
      SpaceEntity.new(
        position,
        velocity,
        scale,
        "space_rocks meteor_large"
      )
    )
  }

  generate_random_spacerock(scale) {
    var random_x = Math.rand(-_gameWidth/2, _gameWidth/2)
    var random_y = Math.rand(-_gameHeight/2, _gameHeight/2)
    var random_velx = Math.rand(-100, 100)
    var random_vely = Math.rand(-100, 100)
    add_spacerock(
        Vec2.new(random_x, random_y),
        Vec2.new(random_velx, random_vely),
        scale
    )
  }

  win() {
    _player_won = true
    _gamerunning = false
    System.print("YOU WIN!")
    Inventory.add("spacerocks ticket")
  }

  loose() {
    _gamerunning = false
    System.print("YOU LOOSE!")
  }

  shoot() {
    if(_canShoot) {


        var entityBullet = SpaceBullet.new(
            Vec2.new(0, 0),
            Vec2.new(0, 10),
            25,
            "space_rocks star_tiny"
        )
      var bullet_speed = 200
      entityBullet.position.x = _entityShip.position.x
      entityBullet.position.y = _entityShip.position.y
      var ship_rotation_rad = (90 - _entityShip.rotation) * (Num.pi/180)
      entityBullet.velocity.x = ship_rotation_rad.cos * bullet_speed
      entityBullet.velocity.y = ship_rotation_rad.sin * bullet_speed
      entityBullet.rotation = _entityShip.rotation
      _canShootTimer = 0.35
      _canShoot = false
      _bullets.add(entityBullet)
    }
  }

  handle_input(elapsedTime) {
    var acceleration = 175
    var turnSpeed = 248
    var pressingShoot = Input.getKeyDown(Input.KEY_SPACE)
    var pressingForward = Input.getKey(Input.KEY_W) || Input.getKey(Input.KEY_UP)
    var pressingLeft = Input.getKey(Input.KEY_A) || Input.getKey(Input.KEY_LEFT)
    var pressingRight = Input.getKey(Input.KEY_D) || Input.getKey(Input.KEY_RIGHT)

    if(pressingShoot) {
      shoot()
    }

    var ship_rotation_rad = (90 - _entityShip.rotation) * (Num.pi/180)

    if(pressingForward) {
      _entityShip.velocity.x = _entityShip.velocity.x + ship_rotation_rad.cos * acceleration * elapsedTime
      _entityShip.velocity.y = _entityShip.velocity.y + ship_rotation_rad.sin * acceleration * elapsedTime
    }
    if(pressingLeft) {
      _entityShip.rotation = _entityShip.rotation - turnSpeed * elapsedTime
    }
    if(pressingRight) {
      _entityShip.rotation = _entityShip.rotation + turnSpeed * elapsedTime
    }
  }

  player_hit_logic() {
    _objectShipCollision = GameObject.new(
        Vec2.new(_entityShip.position.x, _entityShip.position.y),
        Vec2.new(_entityShip.scale/3, _entityShip.scale/3)
    )
    for(spacerock in _spacerocks) {
      _objectSpacerockCollision = GameObject.new(
          Vec2.new(spacerock.position.x, spacerock.position.y),
          Vec2.new(spacerock.scale/2, spacerock.scale/2)
      )
      if(Math.AABB(_objectSpacerockCollision, _objectShipCollision)) {
        loose()
      }
    }
  }

  bullet_hit_logic(elapsedTime) {
      for(bullet in _bullets) {
          bullet.update(elapsedTime)
          for (i in 0.._numSpacerocks-1) {
            var spacerock = _spacerocks[i]
            if(Math.AABB(spacerock.object, bullet.object)) {
              if(spacerock.scale == 128) {
                for(i in 0..4) {
                  var random_velx = Math.rand(-100, 100)
                  var random_vely = Math.rand(-100, 100)
                  add_spacerock(Vec2.new(spacerock.position.x, spacerock.position.y), Vec2.new(random_velx, random_vely), 64)
                }
              } else if(spacerock.scale == 64) {
                for(i in 0..4) {
                  var random_velx = Math.rand(-100, 100)
                  var random_vely = Math.rand(-100, 100)
                  add_spacerock(Vec2.new(spacerock.position.x, spacerock.position.y), Vec2.new(random_velx, random_vely), 32)
                }
              }
              remove_spacerock(i)
              bullet.destroy()
              break
            }
          }
          destroy_bullets()
      }

  }

  destroy_bullets(){
      if(_bullets.count > 0) {
        for(i in 0.._bullets.count-1) {
          if(_bullets[i].destroyed) {
            //System.print("Bullet[%(i)] is destroyed.")
            _bullets.removeAt(i)
            break
          }
        }
      }

  }

  update(elapsedTime) {
    if(!_gamerunning) {
      if(Input.getKeyDown(Input.KEY_R)) {
        reset()
      }
      return
    }
    player_hit_logic()
    if(_numSpacerocks == 0) {
      win()
    }

    handle_input(elapsedTime)

    _entityShip.update(elapsedTime)

    bullet_hit_logic(elapsedTime)



    for (spacerock in _spacerocks) {
      spacerock.update(elapsedTime)
    }

    if(_canShootTimer > 0) {
      _canShootTimer = _canShootTimer - elapsedTime
    } else {
      _canShoot = true
    }

  }

  render() {
    if(!_gamerunning) {
      if(_player_won) {
        Window.renderText("You Win!\nYou Get A Ticket!", 0, 0, 100)
      } else {
        Window.renderText("You Loose!\nPress 'R' To Restart", 0, 0, 100)
      }
    }

    Window.render("arcade screen", 0, 0, 1920, 1080)
    _entityShip.render()
    for (bullet in _bullets) {
        bullet.render()
    }
    for (spacerock in _spacerocks) {
      spacerock.render()
    }

    Window.render("BLACK", 0, 0, _gameWidth, _gameHeight)
  }
}

var game = SpaceRocks.init()
