import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio, Inventory, Dialogue

class Entity {
  construct new(position, velocity, z_velocity, sprite) {
    _sprite = sprite
    _grabbed = false
    _scalar = 8
    _object = GameObject.new(
        Vec2.new(position.x, position.y),
        Vec2.new(16*_scalar, 16*_scalar)
    )
    _height = 0
    _z_velocity = z_velocity
    _velocity = velocity
  }

  object { _object }
  sprite { _sprite }
  fall_speed { _z_velocity }
  speed { _velocity.x.abs + _velocity.y.abs }

  height { _height }
  height=(value) { _height = value }
  grabbed { _grabbed }
  grabbed=(value) { _grabbed = value }

  is_slice() {
    return _sprite.contains("Slice")
  }

  process_gravity(elapsedTime) {
    var gravity = 1700
    _z_velocity = _z_velocity -gravity * elapsedTime
    _height = _height + _z_velocity * elapsedTime
  }

  process_border_walls(elapsedTime) {
    if(_height < 0) {
      _velocity.x = _velocity.x/2
      _velocity.y = _velocity.y/2
      _z_velocity = -_z_velocity/1.5
      _height = 0
      if(_z_velocity.abs > 100) {
        Audio.playSFX("Kitchen Hit")
      }
    }
    if(_object.position.x < -Window.getWidth()/2) {
      _object.position.x = -Window.getWidth()/2
      _velocity.x = -_velocity.x/2
      if(_velocity.x.abs > 250) {
        Audio.playSFX("Kitchen Hit")
      }
    }
    if(_object.position.x > Window.getWidth()/2) {
      _object.position.x = Window.getWidth()/2
      _velocity.x = -_velocity.x/2
      if(_velocity.x.abs > 250) {
        Audio.playSFX("Kitchen Hit")
      }
    }
    if(_object.position.y < -Window.getHeight()/2) {
      _object.position.y = -Window.getHeight()/2
      _velocity.y = -_velocity.y/2
      if(_velocity.y.abs > 250) {
        Audio.playSFX("Kitchen Hit")
      }
    }
    if(_object.position.y > Window.getHeight()/2) {
      _object.position.y = Window.getHeight()/2
      _velocity.y = -_velocity.y/2
      if(_velocity.y.abs > 250) {
        Audio.playSFX("Kitchen Hit")
      }
    }
  }

  process_velocity(elapsedTime) {
    _object.position.x = _object.position.x + _velocity.x * elapsedTime
    _object.position.y = _object.position.y + _velocity.y * elapsedTime
  }

  update(elapsedTime) {

    process_gravity(elapsedTime)
    process_border_walls(elapsedTime)
    process_velocity(elapsedTime)

    if(Input.getMouseUp(Input.MOUSE_LEFT)) {
      _grabbed = false
    }

    if(_grabbed) {
      _z_velocity = 0
      var grab_speed = 5
      _height = _height + (150 - _height) * grab_speed * elapsedTime
      var delta_x = Input.getMouseX() - _object.position.x
      var delta_y = Input.getMouseY() - _object.position.y
      _object.position.x = _object.position.x + (Input.getMouseX() - _object.position.x) * grab_speed * elapsedTime
      _object.position.y = _object.position.y + (Input.getMouseY() - _object.position.y) * grab_speed * elapsedTime
      _velocity.x = _velocity.x/2 + delta_x*3
      _velocity.y = _velocity.y/2 + delta_y*3
    }
  }

  render() {
    Window.render("Kitchen %(_sprite)", _object.position.x, _object.position.y+_height, _object.scale.x, _object.scale.y)
    Window.render("Kitchen Shadow", _object.position.x, _object.position.y-_scalar, _object.scale.x, _object.scale.y)
  }
}

class Prep {
  load_food_sprites(name) {
    ResourceManager.loadTexture("res/textures/sprites/Kitchen/%(name).png", "Kitchen %(name)")
    ResourceManager.loadTexture("res/textures/sprites/Kitchen/%(name) Slice.png", "Kitchen %(name) Slice")
  }

  construct init() {
    ResourceManager.loadAudio("res/scripts/minigames/Kitchen - Prep/assets/hit.wav", "Kitchen Hit")

    ResourceManager.loadTexture("res/textures/sprites/Kitchen/Counter.png", "Kitchen counter top")
    ResourceManager.loadTexture("res/textures/sprites/Kitchen/Shadow.png", "Kitchen Shadow")
    //ResourceManager.loadTexture("res/textures/sprites/Kitchen/Apple.png", "Kitchen Apple")
    //ResourceManager.loadTexture("res/textures/sprites/Kitchen/Apple Slice.png", "Kitchen Apple Slice")
    load_food_sprites("Apple")
    load_food_sprites("Red Pepper")
    load_food_sprites("Lettuce")
    load_food_sprites("Chives")
    load_food_sprites("Mayonnaise")
    load_food_sprites("Red Onion")
    ResourceManager.loadTexture("res/textures/sprites/Kitchen/Knife.png", "Kitchen Knife")
    ResourceManager.loadTexture("res/textures/sprites/Kitchen/Bowl.png", "Kitchen Bowl")

    Audio.playBGM("Zazie by Kevin MacLeod")

    _scalar = 8
    _gameWidth = 800
    _gameHeight = 600

    _kitchenEntitiesSize = 0
    _kitchenEntities = []

    for(i in 0..2) {
      create_kitchen_entity(
          Vec2.new(-Window.getWidth()/2.5 + Math.rand(-15, 15), -Window.getHeight()/2.5 + i * 64),
          Vec2.new(0, 0),
          0,
          "Apple"
      )
    }

    for(i in 0..1) {
      create_kitchen_entity(
          Vec2.new(-Window.getWidth()/3.5 + Math.rand(-15, 15), -Window.getHeight()/2.5 + i * 64),
          Vec2.new(0, 0),
          0,
          "Red Pepper"
      )
    }

    for(i in 0..2) {
      create_kitchen_entity(
          Vec2.new(-Window.getWidth()/5 + Math.rand(-15, 15), -Window.getHeight()/2.5 + i * 64),
          Vec2.new(0, 0),
          0,
          "Lettuce"
      )
    }

    for(i in 0..2) {
      create_kitchen_entity(
          Vec2.new(Window.getWidth()/2.5 + Math.rand(-15, 15), -Window.getHeight()/2.5 + i * 64),
          Vec2.new(0, 0),
          0,
          "Chives"
      )
    }

    for(i in 0..1) {
      create_kitchen_entity(
          Vec2.new(Window.getWidth()/3.5 + Math.rand(-15, 15), -Window.getHeight()/2.5 + i * 64),
          Vec2.new(0, 0),
          0,
          "Red Onion"
      )
    }


    create_kitchen_entity(
        Vec2.new(Window.getWidth()/3, 0),
        Vec2.new(0, 0),
        0,
        "Knife"
    )

    _bowlAnimationTimer = 0
    _bowlAnimationRandAngle = 0
    _objectBowl = GameObject.new(
        Vec2.new(_gameWidth/2, _gameHeight/2),
        Vec2.new(32*_scalar, 32*_scalar)
    )

    _objectCuttingboard = GameObject.new(
        Vec2.new(0, 4 + _scalar),
        Vec2.new(68*_scalar, 42*_scalar)
    )

    _knifeCooldown = 0
  }

  create_kitchen_entity(position, velocity, z_velocity, sprite) {
    _kitchenEntities.add(
      Entity.new(
          position,
          velocity,
          z_velocity,
          sprite
      )
    )
    _kitchenEntitiesSize = _kitchenEntitiesSize + 1
  }

  remove_kitchen_entity(index) {
    _kitchenEntities.removeAt(index)
    _kitchenEntitiesSize = _kitchenEntitiesSize - 1
  }

  find_kitchen_entity_with_sprite(sprite) {
    for(kitchenEntity in _kitchenEntities) {
      if(kitchenEntity.sprite == sprite) {
        return kitchenEntity
      }
    }
  }

  swap_list_elements(list, index_a, index_b) {
    var tmp = list[index_a]
    list[index_a] = list[index_b]
    list[index_b] = tmp
    return list
  }

  order_kitchen_entities() {
    if(_kitchenEntitiesSize < 2) {
      return
    }
    var sorted = false
    while(!sorted) {
      sorted = true
      for (i in 0.._kitchenEntitiesSize-2) {
        var current = _kitchenEntities[i]
        var next = _kitchenEntities[i+1]
        if(current.object.position.y > next.object.position.y) {
          _kitchenEntities = swap_list_elements(_kitchenEntities, i, i+1)
          sorted = false
        }
      }
    }
  }

  knife_logic(elapsedTime) {
    if(_knifeCooldown > 0) {
        _knifeCooldown = _knifeCooldown - elapsedTime
        return
    }

    var knife = find_kitchen_entity_with_sprite("Knife")
    if(knife.fall_speed < -600) {
      for (i in 0.._kitchenEntitiesSize-1) {
        var kitchenEntity = _kitchenEntities[i]
        if(Math.AABB(_objectCuttingboard, knife.object) && Math.AABB(_objectCuttingboard, kitchenEntity.object)) {
          if(Math.AABB(kitchenEntity.object, knife.object)) {
            if(kitchenEntity.sprite != "Knife" && !kitchenEntity.is_slice()) {

              System.print(knife.speed)
              if(knife.speed > 500) {
                Audio.playSFX("crowd ooh")
              }

              if(knife.speed > 1300) {
                Audio.playSFX("knife slice heavy")
                Audio.playSFX("crowd applause")
              } else {
                Audio.playSFX("knife slice 1")
              }

              _knifeCooldown = 1

              for(a in 0..3) {
                create_kitchen_entity(
                    Vec2.new(kitchenEntity.object.position.x, kitchenEntity.object.position.y),
                    Vec2.new(Math.rand(-250, 250), Math.rand(-250, 250)),
                    Math.rand(500, 700),
                    "%(kitchenEntity.sprite) Slice"
                )
              }

              remove_kitchen_entity(i)

              break
            }
          }
        }
      }
    }
  }

num_slices() {
  var num = -1
  for(kitchenEntity in _kitchenEntities) {
    if(kitchenEntity.is_slice()) {
      num = num + 1
    }
  }
  return num
}

bowl_bounce() {
  _bowlAnimationTimer = 0.1
  _bowlAnimationRandAngle = Math.rand(-10, 10)
  Audio.playSFX("ian plop")
  var slices = num_slices()
  if(slices == 0) {
    Audio.playSFX("crowd applause")
  }
}

animate_bowl(elapsedTime) {
  var speed = 13
  if(_bowlAnimationTimer > 0) {
    _objectBowl.scale.x = _objectBowl.scale.x + ((1/2.5)*32*_scalar - _objectBowl.scale.x) * speed * elapsedTime
    _objectBowl.scale.y = _objectBowl.scale.y + (2.5*32*_scalar - _objectBowl.scale.y) * speed * elapsedTime

    _objectBowl.position.y = _objectBowl.position.y + (_objectBowl.scale.y/3 + _gameHeight/2 - _objectBowl.position.y) * speed * elapsedTime

    _objectBowl.rotation = _objectBowl.rotation + (_bowlAnimationRandAngle - _objectBowl.rotation) * speed * elapsedTime
  } else {
    _objectBowl.scale.x = _objectBowl.scale.x + (32*_scalar - _objectBowl.scale.x) * speed * elapsedTime
    _objectBowl.scale.y = _objectBowl.scale.y + (32*_scalar - _objectBowl.scale.y) * speed * elapsedTime

    _objectBowl.position.y = _objectBowl.position.y + (_gameHeight/2 - _objectBowl.position.y) * speed * elapsedTime

    _objectBowl.rotation = _objectBowl.rotation + (0 - _objectBowl.rotation) * speed * elapsedTime
  }

  _bowlAnimationTimer = _bowlAnimationTimer - elapsedTime
}

  update(elapsedTime) {
    animate_bowl(elapsedTime)

    var objectMouse = GameObject.new(
        Vec2.new(Input.getMouseX(), Input.getMouseY()),
        Vec2.new(0, 0)
    )

    for (i in 0.._kitchenEntitiesSize-1) {
      var kitchenEntity = _kitchenEntities[i]
      if(kitchenEntity.is_slice() && kitchenEntity.height < 5 && Math.AABB(_objectBowl, kitchenEntity.object)) {
        bowl_bounce()
        remove_kitchen_entity(i)
        break
      }
    }

    for(kitchenEntity in _kitchenEntities) {
      kitchenEntity.update(elapsedTime)
      if(Math.AABB(objectMouse, kitchenEntity.object)) {
        if(Input.getMouseDown(Input.MOUSE_LEFT)) {
          kitchenEntity.grabbed = true
          break
        }
      }
    }

    knife_logic(elapsedTime)
  }

  render() {
    if(_kitchenEntitiesSize == 1) {
      Inventory.add("unrefrigerated perfection salad")
      Window.renderText("You've made a Perfection Salad!", 0, 0, 100)
    }

    order_kitchen_entities()
    for(kitchenEntity in _kitchenEntities) {
      kitchenEntity.render()
    }

    Window.render("Kitchen Bowl", _objectBowl)

    //Window.render("BLACK", _objectCuttingboard)

    // Background
    Window.render("Kitchen counter top", 0, 0, 282*_scalar, 175*_scalar)
  }
}

var game = Prep.init()
