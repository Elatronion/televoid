import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio, Inventory, Dialogue

class Breakout {
  static STATE_MENU   { 0 }
  static STATE_START  { 1 }
  static STATE_PLAY   { 2 }
  static STATE_WIN    { 3 }

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

    _gameState = Breakout.STATE_MENU

    _ballSpeed = 500
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
      Vec2.new(0, -_gameHeight/2),
      Vec2.new(64 * scalar, 15 * scalar)
    )

    _numBlocks = 0
    _blocks = []

    var blocks_margin = 10
    for (y in 0..6) { //0..4
    for (x in 1..5) {
      _numBlocks = _numBlocks+1
      _blocks.add(
        GameObject.new(
          Vec2.new(-_gameWidth/2 + x * 64*scalar, _gameHeight/2 - y * 15 * scalar - (15 * scalar) - blocks_margin),
          Vec2.new(64 * scalar, 15 * scalar)
        )
      )
    }
    }
  }

  win() {
    _gameState = Breakout.STATE_WIN
    if(Inventory.has("brutal moose")) {
      Inventory.add("breakout ticket")
    }
  }

  loose() {
    //Audio.playSFX("BREAKOUT MISS")
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
        //Audio.playSFX("BREAKOUT LAUNCH")
        _gameState = Breakout.STATE_PLAY
        _ballDirection.y = 1
      }
    } else if(_gameState == Breakout.STATE_MENU) {
      if(pressingLaunchKey) {
        _gameState = Breakout.STATE_START
      }
    }
  }

  bounceBallOffObject(object) {
    _ballDirection.x = _objectBall.position.x - object.position.x
    _ballDirection.y = _objectBall.position.y - object.position.y
    _ballDirection.normalize()

    /*if (_ballDirection.x > 0.707) {
      _ballDirection.x = 0.707
      if(_ballDirection.y > 0) {
          _ballDirection.y = 0.707
      } else {
        _ballDirection.y = -0.707
      }
    } else if(_ballDirection.x < 0.707) {
      _ballDirection.x = -0.707
      if(_ballDirection.y > 0) {
          _ballDirection.y = 0.707
      } else {
        _ballDirection.y = -0.707
      }
    }*/
  }

  processBall(elapsedTime) {
    if(_gameState == Breakout.STATE_START) {
      _objectBall.position.x = _objectPaddle.position.x
      _objectBall.position.y = _objectPaddle.position.y + _objectPaddle.scale.y/2 + _objectBall.scale.y/2
      return
    }

    _objectBall.position.x = _objectBall.position.x + _ballSpeed * _ballDirection.x * elapsedTime
    _objectBall.position.y = _objectBall.position.y + _ballSpeed * _ballDirection.y * elapsedTime

    if(_objectBall.position.x > _gameWidth/2) {
      //Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.x = _gameWidth/2
      _ballDirection.x = -_ballDirection.x
    }
    if(_objectBall.position.x < -_gameWidth/2) {
      //Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.x = -_gameWidth/2
      _ballDirection.x = -_ballDirection.x
    }

    if(_objectBall.position.y > _gameHeight/2) {
      //Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.y = _gameHeight/2
      _ballDirection.y = -_ballDirection.y
    }
    if(_objectBall.position.y < -_gameHeight/2) {
      loose()
    }

    if(Math.AABB(_objectPaddle, _objectBall)) {
      //Audio.playSFX("BREAKOUT PADDLE HIT")
      bounceBallOffObject(_objectPaddle)
    }
  }

  removeBlock(index) {
    //Audio.playSFX("BREAKOUT HIT")
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
