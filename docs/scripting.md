# Scripting

[Wren](https://wren.io/) is the scripting language for tootl. Scripts are used to add logic to basic interactions.
The [tootl API](#Tootl API) for wren will allow you to interact with various game elements.

## Wren

Wren is a relatively simple yet powerful scripting language. If you aren't familiar with it already, it is suggested that you get up to speed following their [getting started](https://wren.io/getting-started.html) doc. It should be easy enough to get into the flow of things.

## Tootl API

There are two important wren files that handle, in part, communication between the game and scripting engines. 
First, and most prominently, `televoid-core.wren` which may be used to load scenes, dialogues, minigames and interact with the player's inventory. 
Finally, we have `minigame-core.wren` to create the aforementioned fully fledged minigames, microinteractions, etc.

Snippets will automatically exclusively import the televoid core and all it's classes.

You'll notice that within both core wren files, multiple classes and a large amount of undefined foreign static methods. This is because most of the logic for the API exists within the source code of the application.

```wren
class Inventory {
  foreign static add(item_name)
  foreign static remove(item_name)
  foreign static has(item_name)
}
```

Custom logic can, of course, be created. This is the case with the minigame core wren file. Where certain quality-of-life classes were created.

```wren
class Vec2 {
  construct new(x, y) {
    _x = x
    _y = y
  }
  x { _x }
  y { _y }
  x=(value) { _x = value }
  y=(value) { _y = value }
}
```

The above is an example of a Vec2 class that helps organize data when creating minigames.

Feel free to explore both files to try and understand what tools are offered to you, otherwise this doc will do it's best to explain the various features with examples.

### Televoid Core

Used to load scenes, dialogues, minigames and interact with the player's inventory. 

#### Scene

Used to load scenes from path.

```wren
Scene.load("res/scenes/test.tmx")
```

#### Inventory

Used to interact with the player's inventory.

```wren
Inventory.add("key")
```

This will add the item "key" using the sprite `res/textures/inventory/key.png`. Nothing will happen if the item is already in the inventory.

```wren
Inventory.remove("key")
```

This will remove the item "key" from the player's inventory. Nothing will happen no item with that name is found in the player's inventory.

```wren
Inventory.has("key")
```

This will return a Boolean value: `true` if the item is in the inventory, `false` if the item is not in the inventory.

#### Dialogue

Used to load and initiate a dialogue, loaded from path.

```wren
Dialogue.load("res/dialogues/test.csv")
```

#### Minigame

Used to load and initiate a minigame, loaded from path.

```wren
Minigame.load("res/scripts/minigames/test-minigame.wren")
```

#### Audio

Used to play audio SFX, BGM, etc.

```wren
Audio.playSFX("Pew")
```

#### Animation

Used to load and play IMVs.

```wren
Animation.load("res/imv/test.imv")
```

#### Character

Interact with the characters.

Ian, the player entity, is named "ian".

```wren
Character.walk(some_position, "ian")
```

The above code would make character `ian` walk to `some_position`.

#### Script Example

```wren
import "televoid-core" for Dialogue, Inventory

if(Inventory.has("booze")) {
	Dialogue.load("res/dialogues/frank-receives-booze.csv")
	Inventory.remove("booze")
} else {
	Dialogue.load("res/dialogues/frank-wants-booze.csv")
}
```

### Minigame Core

Used to create fully fledged minigames, micro interactions or effects.

#### Vec2

A simple class that holds both an x and y value.

```wren
var some_position = Vec2.new(12, 32)
some_position.x = some_position.x + 15

System.print("(%(some_position.x), %(some_position.y))")
```

This would print `(27, 32)`.

```wren
var direction = Vec2.new(7534, 7534)
direction.normalize()
System.print("(%(direction.x), %(direction.y))")
```

Vec2 also includes a normalize method; this would print `(1, 1)`.

#### GameObject

A simple class that holds two `Vec2` classes and a rotation `double`, `position`, `scale`, and `rotation`.

```wren
var some_gameobject = GameObject.new(Vec2.new(0, 0), Vec2.new(128, 64))
some_gameobject.position.x = some_gameobject.scale.y/2

System.print("(%(some_gameobject.position.x), %(some_gameobject.position.y)), (%(some_gameobject.scale.x), %(some_gameobject.scale.y))")
```

This would print `(32, 0), (128, 64)`.

#### Window

This class will allow you to interact with the game window by allowing you to render 2D elements to the screen. The rendering method takes the sprite name, x position, y position, x scale, and y scale.

```wren
Window.render("Smiley", 0, 0, 64, 64) // Render Smiley at spesific position and scale
Window.render("Smiley", 0, 0, 64, 64, 45) // Render Smiley at spesific position, scale and rotation (45 deg)
Window.render("Smiley", objectSmiley) // Render Smiley using data stored in objectSmiley
```

The above code would be used to render a sprite named "Smiley" at (0, 0) with size (64, 64) and rotation 0deg or 45deg when specified.

```wren
Window.width // The width of the game's window
Window.height // The height of the game's window
```

The above variables are used for responsive UI and generally making sure the action remains on screen.

#### Input

The input class is most useful for detecting, you guessed it, user input!
You may check key input, mouse input and mouse position. Aside from the mouse position, all other methods will return `true` or `false`. The input class is also filled with all they needed keycodes following the pattern `KEY_<KEY>` and mouse codes `MOUSE_<BUTTON>`!

Here are all the input functions.

```wren
Input.getKey(Input.KEY_A) // Checks if the key 'A' is currently being held.
Input.getKeyDown(Input.KEY_A) // Checks if the key 'A' had just been pressed.
Input.getKeyUp(Input.KEY_A) // Checks if the key 'A' had just been released.

Input.getMouse(Input.MOUSE_LEFT) // Checks if the mouse 'Left Click' is currently being held.
Input.getMouseDown(Input.MOUSE_LEFT) // Checks if the mouse 'Left Click' had just been pressed.
Input.getMouseUp(Input.MOUSE_LEFT) // Checks if the mouse 'Left Click' had just been released.

var mouse_x = Input.getMouseX() // Returns the current mouse X position on the window
var mouse_y = Input.getMouseY() // Returns the current mouse Y position on the window
```

##### Example

Here is a simple example to show how you could manipulate values when a certain key is pressed.

```wren
var speed = 64
if(Input.getKey(Input.KEY_D) || Input.getKey(Input.KEY_RIGHT)) {
	smiley_gameobject.position.x = smiley_gameobject.position.x + speed * elapsedTime
} else if(Input.getKey(Input.KEY_A) || Input.getKey(Input.KEY_LEFT)) {
	smiley_gameobject.position.x = smiley_gameobject.position.x - speed * elapsedTime
}
```

This would move the object `smiley_gameobject` to the left or to the right 64 units per second based on if the user is holding A/D or Left/Right.

#### ResourceManager

The resource manager is very important and powerful, it will allow you to load (PNG) texture and (WAV) audio assets!

```wren
ResourceManager.loadTexture("res/textures/smiley.png", "Smiley") // Load the smiley PNG image and refere to it as Smiley
ResourceManager.loadAudio("res/audio/sfx/pew.png", "Pew") // Load the pew WAV sound effect and refer to it as Pew
```

It is recommended that you follow a pattern when naming your assets as to not interfere with other loaded assets. 
A pattern such as `<MINIGAME_NAME> <ASSET_NAME>`. e.g. `BREAKOUT PADDLE`.
You may also create a folder to hold all the needed assets for your minigame.

#### Math

Simple math functions for lerp, easeinout and AABB.

```wren
Math.lerp(a, b, f) // First value, Second value, percentage (0.0 to 1.0)
Math.easeinout(a, b, f) // First value, Second value, percentage (0.0 to 1.0)

AABB(Ax, Ay, Aw, Ah, Bx, By, Bw, Bh) // A position and scale, B position and scale
AABB(objectA, objectB) // Detect intersection between two GameObjects
```

Mostly used for object intersection.

##### Example

```wren
if(Math.AABB(objectPlayer, objectBadguy)) {
	GameOver()
}
```

## Minigame Structure

The most vital part of minigame creation is understanding it's structure.
There are a set of simple rules that. when followed, will run and render your minigame. 

```wren
import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math, Audio

class MyFirstMinigame {
  construct init() {
  	// asset and global variable initialization
  }

  update(elapsedTime) {
 	// game loop
  }

  render() {
	// rendering
  }
}

var game = MyFirstMinigame.init()
```

It is critical to note that the final variable, outside of the class scope, is named `game`. All minigames require this variable to be properly executed.

Otherwise, your minigame class may have any name and must contain a construction (init), update and render method. Note that the update method has the `elapsedTime` argument, this is the time between two frames.; the delta time. You may name it as you wish.

Rendering is done at the end of each frame, right before it is displayed. Updates are done before that.

It is recommended to make sure your game is playable at a resolution of 800x600.

### Example (Breakout)

```wren
import "minigame-core" for Vec2, GameObject, Window, Input, ResourceManager, Math
import "televoid-core" for Audio

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

    for (y in 0..4) {
    for (x in 0..6) {
      _numBlocks = _numBlocks+1
      _blocks.add(
        GameObject.new(
          Vec2.new(-_gameWidth/2 + x * 64*scalar, _gameHeight/2 - y * 15 * scalar),
          Vec2.new(64 * scalar, 15 * scalar)
        )
      )
    }
    }
  }

  win() {
    _gameState = Breakout.STATE_WIN
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
    var paddleSpeed = 1000

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
      Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.x = _gameWidth/2
      _ballDirection.x = -_ballDirection.x
    }
    if(_objectBall.position.x < -_gameWidth/2) {
      Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.x = -_gameWidth/2
      _ballDirection.x = -_ballDirection.x
    }

    if(_objectBall.position.y > _gameHeight/2) {
      Audio.playSFX("BREAKOUT PONG")
      _objectBall.position.y = _gameHeight/2
      _ballDirection.y = -_ballDirection.y
    }
    if(_objectBall.position.y < -_gameHeight/2) {
      loose()
    }

    if(Math.AABB(_objectPaddle, _objectBall)) {
      Audio.playSFX("BREAKOUT PADDLE HIT")
      bounceBallOffObject(_objectPaddle)
    }
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

    if(_gameState == Breakout.STATE_MENU) {
      Window.render("BREAKOUT GUI PRESS_START", 0, 0, 64 * 4, 64 * 4)
      return
    } else if(_gameState == Breakout.STATE_WIN) {
      Window.render("BREAKOUT GUI YOU_WIN", 0, 0, 64 * 4, 64 * 4)
      return
    }

    if(_lives > 0) {
      for(i in 0.._lives-1) {
        Window.render("BREAKOUT BALL", -_gameWidth/2 + i * (_objectBall.scale.x + 5), _gameHeight/2 - 5, _objectBall.scale.x, _objectBall.scale.y)
      }
    }
    for (block in _blocks) {
      Window.render("BREAKOUT BLOCK", block)
    }
    Window.render("BREAKOUT BALL", _objectBall)
    Window.render("BREAKOUT PADDLE", _objectPaddle)

  }
}

var game = Breakout.init()
```

