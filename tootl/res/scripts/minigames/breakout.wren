import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio, Inventory, Dialogue

class Breakout {
  static STATE_MENU   { 0 }
  static STATE_START  { 1 }
  static STATE_PLAY   { 2 }
  static STATE_WIN    { 3 }

  static DIRECTION_UP     { 0 }
  static DIRECTION_RIGHT  { 1 }
  static DIRECTION_DOWN   { 2 }
  static DIRECTION_LEFT   { 3 }

  construct init() {
    ResourceManager.loadTexture("res/scripts/minigames/breakout_assets/paddle.png", "BREAKOUT PADDLE")
    ResourceManager.loadTexture("res/scripts/minigames/breakout_assets/block.png", "BREAKOUT BLOCK")
    ResourceManager.loadTexture("res/scripts/minigames/breakout_assets/ball.png", "BREAKOUT BALL")
    ResourceManager.loadTexture("res/scripts/minigames/breakout_assets/press start.png", "BREAKOUT GUI PRESS_START")
    ResourceManager.loadTexture("res/scripts/minigames/breakout_assets/you win.png", "BREAKOUT GUI YOU_WIN")

    ResourceManager.loadAudio("res/scripts/minigames/breakout_assets/miss.wav", "BREAKOUT MISS")
    ResourceManager.loadAudio("res/scripts/minigames/breakout_assets/hit.wav", "BREAKOUT HIT")
    ResourceManager.loadAudio("res/scripts/minigames/breakout_assets/pong.wav", "BREAKOUT PONG")
    ResourceManager.loadAudio("res/scripts/minigames/breakout_assets/launch.wav", "BREAKOUT LAUNCH")
    ResourceManager.loadAudio("res/scripts/minigames/breakout_assets/paddle hit.wav", "BREAKOUT PADDLE HIT")

    _gameWidth = 800
    _gameHeight = 600

    _initial_ball_direction = Vec2.new(100, 350)

    _gameState = Breakout.STATE_MENU

    _ballSpeed = 1
    reset()

    System.print("Wow, some interesting fact!")
    Dialogue.load("res/dialogue/breakout play.csv")
  }

  reset() {
    _points = 0
    _lives = 3

    _ballDirection = Vec2.new(0, 0)

    var scalar = 2

    _objectBall = GameObject.new(
        Vec2.new(0, 0),
        Vec2.new(15 * scalar, 15 * scalar)
    )

    _objectPaddle = GameObject.new(
      Vec2.new(0, -_gameHeight/2 + 15),
      Vec2.new(64 * scalar, 15 * scalar)
    )

    _numBlocks = 0
    _blocks = []

    var blocks_margin = 10
    for (y in 2..5) { //0..4
    for (x in 0..7) {
      _numBlocks = _numBlocks+1
      _blocks.add(
        GameObject.new(
          Vec2.new(-_gameWidth/2 + x * 50*scalar + 50*scalar/2, _gameHeight/2 - y * 15 * scalar - (15 * scalar) - blocks_margin),
          Vec2.new(50 * scalar, 15 * scalar)
        )
      )
    }
    }
  }

  win() {
    _gameState = Breakout.STATE_WIN
    Inventory.add("breakout ticket")
  }

  loose() {
    Audio.playSFX("BREAKOUT MISS")
    _lives = _lives - 1
    if(_lives >= 0) {
      _gameState = Breakout.STATE_START
      _ballDirection = Vec2.new(0, 0)
    } else {
      _gameState = Breakout.STATE_MENU
      reset()
    }
  }

  processInput(elapsedTime) {
    var pressingLaunchKey = Input.getKeyDown(Input.KEY_SPACE)
    var pressingLeftKey = Input.getKey(Input.KEY_A) || Input.getKey(Input.KEY_LEFT)
    var pressingRightKey = Input.getKey(Input.KEY_D) || Input.getKey(Input.KEY_RIGHT)
    var paddleSpeed = 650

    if(pressingRightKey) {
      _objectPaddle.position.x = _objectPaddle.position.x + paddleSpeed * elapsedTime
    } else if(pressingLeftKey) {
      _objectPaddle.position.x = _objectPaddle.position.x - paddleSpeed * elapsedTime
    }

    if(_objectPaddle.position.x + _objectPaddle.scale.x/2 > _gameWidth/2) {
      _objectPaddle.position.x = _gameWidth/2 - _objectPaddle.scale.x/2
    }
    if(_objectPaddle.position.x - _objectPaddle.scale.x/2 < -_gameWidth/2) {
      _objectPaddle.position.x = -_gameWidth/2 + _objectPaddle.scale.x/2
    }

    if(_gameState == Breakout.STATE_START) {
      if(pressingLaunchKey) {
        Audio.playSFX("BREAKOUT LAUNCH")
        _gameState = Breakout.STATE_PLAY
        _ballDirection.x = _initial_ball_direction.x
        _ballDirection.y = _initial_ball_direction.y
      }
    } else if(_gameState == Breakout.STATE_MENU) {
      if(pressingLaunchKey) {
        _gameState = Breakout.STATE_START
      }
    }
  }

  bounceBallOffObject(object) {
    var vector_from_object_to_ball = Vec2.new(_objectBall.position.x - object.position.x, _objectBall.position.y - object.position.y)
    vector_from_object_to_ball.normalize()

    // FUNCTION //
    var target = vector_from_object_to_ball
    var compass = [
      Vec2.new(0.0, 1.0),   // up
      Vec2.new(1.0, 0.0),   // right
      Vec2.new(0.0, -1.0),  // down
      Vec2.new(-1.0, 0.0)   // left
    ]
    var max = 0.0
    var best_match = -1
    for(i in 0..3) {
      var normalized_target = Vec2.new(target.x, target.y)
      normalized_target.normalize()
      var compass_dir = compass[i]
      var dot_product = normalized_target.x * compass_dir.x + normalized_target.y * compass_dir.y
      if(dot_product > max) {
        max = dot_product
        best_match = i
      }
    }
    var dir = best_match

    if (dir == Breakout.DIRECTION_LEFT || dir == Breakout.DIRECTION_RIGHT) {
      _ballDirection.x = -_ballDirection.x
    } else {
      _ballDirection.y = -_ballDirection.y
    }
  }

  processBall(elapsedTime) {
    if(_gameState == Breakout.STATE_START) {
      _objectBall.position.x = _objectPaddle.position.x
      _objectBall.position.y = _objectPaddle.position.y + _objectPaddle.scale.y/2 + _objectBall.scale.y/2
      return
    }

    _objectBall.position.x = _objectBall.position.x + _ballSpeed * _ballDirection.x * elapsedTime
    _objectBall.position.y = _objectBall.position.y + _ballSpeed * _ballDirection.y * elapsedTime

    if(_objectBall.position.x + _objectBall.scale.x/2 > _gameWidth/2) {
      Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.x = _gameWidth/2 - _objectBall.scale.x/2
      _ballDirection.x = -_ballDirection.x
    }
    if(_objectBall.position.x - _objectBall.scale.x/2 < -_gameWidth/2) {
      Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.x = -_gameWidth/2 + _objectBall.scale.x/2
      _ballDirection.x = -_ballDirection.x
    }

    if(_objectBall.position.y + _objectBall.scale.y/2 > _gameHeight/2) {
      Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.y = _gameHeight/2 - _objectBall.scale.y/2
      _ballDirection.y = -_ballDirection.y
    }
    if(_objectBall.position.y < -_gameHeight/2) {
      loose()
    }

    if(Math.AABB(_objectPaddle, _objectBall)) {
        Audio.playSFX("BREAKOUT PADDLE HIT")
        if(_objectBall.position.y > _objectPaddle.position.y){
            bounceBallOffPaddle()
        }
    }
  }

  bounceBallOffPaddle() {
      var center_board = _objectPaddle.position.x
      var distance = (_objectBall.position.x) - center_board
      var percentage =  distance / (_objectPaddle.scale.x / 2)

      var strength = 1
      var old_direction = _ballDirection

      _ballDirection.x = _initial_ball_direction.x * percentage * strength

      var normalized_ball_direction = Vec2.new(_ballDirection.x, _ballDirection.y)
      normalized_ball_direction.normalize()
      var old_direction_length = old_direction.length
      _ballDirection.x = normalized_ball_direction.x * old_direction.length
      _ballDirection.y = -normalized_ball_direction.y * old_direction.length
      _objectBall.position.y = _objectPaddle.position.y + _objectBall.scale.y
  }

  removeBlock(index) {
    Audio.playSFX("BREAKOUT HIT")
    _blocks.removeAt(index)
    _numBlocks = _numBlocks - 1
    if(_numBlocks == 0) { win() }
  }

  processBlocks() {
    if(_numBlocks > 0) {
    for (i in 0.._numBlocks-1) {
      var block = _blocks[i]
      if(Math.AABB(_objectBall, block)) {
        bounceBallOffObject(block)
        removeBlock(i)
        break
      }
    }
    }
  }

  update(elapsedTime) {
    if(_gameState == Breakout.STATE_WIN) {
      return
    }
    processInput(elapsedTime)
    processBall(elapsedTime)
    processBlocks()
  }

  render() {
    Window.render("arcade screen", 0, 0, 1920, 1080)
    if(_gameState == Breakout.STATE_MENU) {
      Window.render("BREAKOUT GUI PRESS_START", 0, 0, 64 * 4, 64 * 4)
      Window.render("BLACK", 0, 0, _gameWidth, _gameHeight)
      return
    } else if(_gameState == Breakout.STATE_WIN) {
      Window.render("BREAKOUT GUI YOU_WIN", 0, 0, 64 * 4, 64 * 4)
      Window.render("BLACK", 0, 0, _gameWidth, _gameHeight)
      return
    }

    if(_lives > 0) {
      var margin = 10
      for(i in 0.._lives-1) {
        Window.render("BREAKOUT BALL", -_gameWidth/2 + i * (_objectBall.scale.x + 5) + _objectBall.scale.x/2 + margin, _gameHeight/2 - 5 - _objectBall.scale.y/2 - margin, _objectBall.scale.x, _objectBall.scale.y)
      }
    }
    for (block in _blocks) {
      Window.render("BREAKOUT BLOCK", block)
    }
    Window.render("BREAKOUT BALL", _objectBall)
    Window.render("BREAKOUT PADDLE", _objectPaddle)

    Window.render("BLACK", 0, 0, _gameWidth, _gameHeight)
  }
}

var game = Breakout.init()
