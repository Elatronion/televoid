import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio, Inventory, Dialogue

class Alien {
    construct new(position, health, sprite) {
        _position = position
        _health = health
        _sprite = sprite
        _precision = 100000
        _shoot_speed = Math.rand(1 * _precision, 10 * _precision)/_precision
        _shoot_cooldown = _shoot_speed
        _can_shoot = false
        _destroyed = false
        _move_speed = 8
    }

    position { _position }
    can_shoot { _can_shoot }
    destroyed { _destroyed }
    health { _health }
    move_speed { _move_speed }

    can_shoot=(value) { _can_shoot = value }
    move_speed=(value) { _move_speed = value }

    destroy() {
        _destroyed = true
    }
    loose_health() {
        _health = _health - 1
    }

    update(delta) {
        _position.y = _position.y - _move_speed * delta
        _shoot_cooldown = _shoot_cooldown - delta
        if(_shoot_cooldown <= 0) {
          _can_shoot = true
          _shoot_cooldown = _shoot_speed
          _shoot_speed = Math.rand(5 * _precision, 25 * _precision)/_precision
        }
    }

    render() {
        Window.render(_sprite, _position.x, _position.y, 15*3, 15*3)
    }
}

class Bullet {
  construct new(position, direction, speed) {
    _position = position
    _direction = direction
    _speed = speed
    _destroyed = false
  }
  destroyed { _destroyed }
  position { _position }
  direction { _direction }

  destroy() {
    _destroyed = true
  }

  update(delta) {
    _position.x = _position.x + _direction.x * _speed * delta
    _position.y = _position.y + _direction.y * _speed * delta
    if(_position.y > Window.height/2 || _position.y < -Window.height/2) {
        destroy()
    }
  }

  render() {
      Window.render("INVADERS BULLET", _position.x, _position.y, 15, 15)
  }
}

class SpaceInvaders {
    static STATE_PLAY { 2 }
    static STATE_WIN { 3 }
    static STATE_LOOSE { 4 }

    static setGameState(state) { __gameState = state }
    static getGameState() { __gameState }

    construct init() {
        ResourceManager.loadTexture("res/scripts/minigames/invaders/assets/ship.png", "INVADERS SHIP")
        ResourceManager.loadTexture("res/scripts/minigames/invaders/assets/bullet.png", "INVADERS BULLET")
        ResourceManager.loadTexture("res/scripts/minigames/invaders/assets/alien.png", "INVADERS ALIEN")
        ResourceManager.loadTexture("res/scripts/minigames/invaders/assets/alien_2.png", "INVADERS ALIEN 2")
        ResourceManager.loadTexture("res/scripts/minigames/invaders/assets/ship.png", "INVADERS HEALTH")

        reset()
        Dialogue.load("res/dialogue/Arcade - Play Invaders.csv")
    }

    reset() {
      var scalar = 3

      _gameWidth = 800
      _gameHeight = 600

      _runtime = 0

      _player = GameObject.new(Vec2.new(0, -_gameHeight/2 + 15*2), Vec2.new(15 * scalar, 15 * scalar))
      _health = 3

      _shoot_speed = 0.35
      _shoot_cooldown = 0
      _can_shoot = false
      _destroyed = false

      _bullets = []
      _aliens = []
      for(y in 0..3) {
          for(i in 0..(_gameWidth / (15*4)) - 6) {
              _aliens.add(Alien.new(Vec2.new((-_gameWidth/2 + (15*4*3)) + i * 15*4, _gameHeight/2 - (y*15*4)), y >= 2 ? 1 : 2, y >= 2 ? "INVADERS ALIEN" : "INVADERS ALIEN 2"))
          }
      }
      SpaceInvaders.setGameState(SpaceInvaders.STATE_PLAY)
    }

    speed_up_aliens() {
      for(alien in _aliens) {
        alien.move_speed = alien.move_speed + 0.5
      }
    }

    win() {
        SpaceInvaders.setGameState(SpaceInvaders.STATE_WIN)
        if(Inventory.has("brutal moose")) {
          Inventory.add("invaders ticket")
        }
    }

    loose() {
        SpaceInvaders.setGameState(SpaceInvaders.STATE_LOOSE)
    }

    loose_health() {
        _health = _health - 1
        if(_health <= 0) {
            loose()
        }
    }

    clamp(value, min, max) {
      if(value < min) {
        value = min
      } else if(value > max) {
        value = max
      }
      return value
    }

    shoot(position, direction, speed) {
        _bullets.add(Bullet.new(Vec2.new(position.x, position.y), direction, speed))
    }

    destroy_bullets() {
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

    destroy_aliens() {
        if(_aliens.count > 0) {
            for(i in 0.._aliens.count-1) {
                if(_aliens[i].destroyed) {
                    _aliens.removeAt(i)
                    speed_up_aliens()
                    break
                }
            }
        } else {
            win()
        }
    }

    update_bullets(delta) {
      for(bullet in _bullets) {
        bullet.update(delta)
        if(bullet.direction.y == 1) {
            for(alien in _aliens) {
                if(Math.AABB(GameObject.new(alien.position, Vec2.new(15*3,15*3)), GameObject.new(bullet.position, Vec2.new(15, 15)))) {
                    bullet.destroy()
                    alien.loose_health()
                    if(alien.health <= 0) {
                        alien.destroy()
                    }
                    break
                }
            }
        }
        if(bullet.direction.y == -1) {
            var player_collision = GameObject.new(Vec2.new(_player.position.x, _player.position.y), Vec2.new(_player.scale.x/2, _player.scale.y/2))
            if(Math.AABB(player_collision, GameObject.new(bullet.position, Vec2.new(15,15)))) {
                bullet.destroy()
                loose_health()
                break
            }
        }
      }
      destroy_bullets()
    }

    update_aliens(delta) {
        for(alien in _aliens) {
            alien.update(delta)
            if(alien.can_shoot) {
                shoot(alien.position, Vec2.new(0, -1), 200)
                alien.can_shoot = false
            }
            var player_collision = GameObject.new(Vec2.new(_player.position.x, _player.position.y), Vec2.new(_player.scale.x/2, _player.scale.y/2))
            if(Math.AABB(GameObject.new(alien.position, Vec2.new(15*3,15*3)), player_collision)) {
                loose()
            }
        }
        destroy_aliens()
    }

    update(delta) {
        if(SpaceInvaders.getGameState() == SpaceInvaders.STATE_PLAY) {
            if(_shoot_cooldown > -1) {
                _shoot_cooldown = _shoot_cooldown - delta
            }
            if(_shoot_cooldown <= 0) {
              _can_shoot = true
            }


            var INPUT_RIGHT = Input.getKey(Input.KEY_D) || Input.getKey(Input.KEY_RIGHT)
            var INPUT_LEFT = Input.getKey(Input.KEY_A) || Input.getKey(Input.KEY_LEFT)
            var INPUT_SHOOT = Input.getKeyDown(Input.KEY_SPACE)

            var speed = 350

            if(INPUT_SHOOT) {
                if(_can_shoot) {
                    _shoot_cooldown = _shoot_speed
                    shoot(_player.position, Vec2.new(0, 1), 400)
                    _can_shoot = false
                }
            }
            //Runtime = Runtime + delta
            //System.print("%(_runtime)")
            _runtime = _runtime + delta
            if(INPUT_RIGHT) {
              _player.position.x = _player.position.x + speed * delta
            }

            if(INPUT_LEFT) {
                _player.position.x = _player.position.x - speed * delta
            }

            _player.position.x = clamp(_player.position.x, (-_gameWidth/2 + (15*4*3)), (_gameWidth/2 - (15*4*3)))

            update_bullets(delta)
            update_aliens(delta)
        }

    }

    render_bullets() {
      for(bullet in _bullets) {
        bullet.render()
      }
    }

    render_aliens() {
        for(alien in _aliens) {
            alien.render()
        }
    }

    render() {
      Window.render("arcade screen", 0, 0, 1920, 1080)

      if(SpaceInvaders.getGameState() == SpaceInvaders.STATE_LOOSE) {
        Window.renderText("You Loose!\nPress  'R'", 0, 0, 100)
        if(Input.getKey(Input.KEY_R)) {
          reset()
        }
      }
      if(SpaceInvaders.getGameState() == SpaceInvaders.STATE_WIN) {
        Window.renderText("You Win!", 0, 0, 100)
      }

        Window.render("INVADERS SHIP", _player)
        if(_health > 0) {
            for(i in 0.._health-1) {
                Window.render("INVADERS HEALTH", -_gameWidth/2 + (i*15*2 + 5) + 15, _gameHeight/2 - 15, 15*2, 15*2)
                //Window.render("INVADERS HEALTH", 0 + i *15*2, 0, 15*2, 15*2)
            }
        }
        render_bullets()
        render_aliens()
        Window.render("BLACK", 0, 0, _gameWidth, _gameHeight)
    }
}

var game = SpaceInvaders.init()
