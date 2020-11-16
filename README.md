# Televoid: Out of The Loop
A Brutalmoose/Televoid fan game written in C using the Hyper Game Engine.

## Build Dependencies

- Libraries
	- Hyper Game Engine 1.2.1
	- libicu-dev
	- xml2
	- freetype
- Compilers
	- gcc
	- mingw-w64
- Build Automation Tools
	- make

## Debug

The game features a debug mode (on by default), in this mode GUI elements are constantly present showcasing various things as lights and triggers.

This mode also features some debug commands!

- V
	- Toggle noclip, move around with WASD.
- R
	- Rotate directional light when applicable.

## Editor

This game uses the [Tiled](https://www.mapeditor.org/) map editor.
For the most part, **object layers** are used to position and scale various different elements in a scene.

### Layers

As aforementioned, the object layer is the main layer type.
You may create as many as you desire, they are simply used for organization.
The only exception to this rule is the **background** layer.
The **background** layer is an image layer and must be named background.

#### General Guide

- Items
	- Grey
- Triggers
	- Orange
- Hotspots
	- Yellow
- Lights
	- Yellow
- Player Starting Positions
	- Purple
- Props
	- Grey
- **background**
	- Image

### Types & Properties

An object has a unique type and may require multiple custom properties to load properly.

#### light

Lights allow you to dynamically light up any scene. The point object is used as it's shape.
**Point light** by default, **direction light** if *dir_x*, *dir_y* and *dir_z* are present.

- (color) ambient
- (color) diffuse
- (float) [extra] dir_x
- (float) [extra] dir_y
- (float) [extra] dir_z

#### player_start

The player_start type allows the game to know where ian should spawn when loading a new scene based on whatever previous scene the player is arriving from. The point object is used as it's shape.

- (bool) face_left
	- Should the player spawn facing left?
- (float) [unused] fade_speed
- (bool) [unused] fadein_on_start
- (string) previous_scene_that_activates
	- Path to previous scene relative to executable.
	
#### prop

The prop's shape is a rectangle of any desired size, recommended to put in resolution of prop texture as size. Let the prop name be p, then files `p_diffuse.png` and `p_normal.png` must exist within `res/textures/sprites/props/`.

- (default) Name
	- Prop name
- (float) depth
	- Allows you to place prop behind or infront of any other element of the scene, still wip.

#### item

The item's shape is a rectangle of 9x9. Let i be item name, then files `i.png` must exist within `res/textures/inventory/items/`.

- (default) Name
	- Item name

#### hotspot

The hotspot's shape is a rectangle of any desired size. Hotspots are activated when the player clicks on them, and only execute their script once the interaction animation reaches a certain frame.

- (string) action_script
	- Path to wren script relative to executable.

#### trigger

The trigger's shape is a rectangle of any desired size. Triggers activated their script when ian has had a collision with the trigger's shape.

- (string) action_script
	- Path to wren script relative to executable.

## Scripting

The [wren](https://wren.io/) scripting language is used for scripts. The `televoid/core` file contains definitions of all embedded functions. This basically allows you to interact with the game. Whether it be to check if the player has a certain item, or to load a new scene.

Eventually, wren will also be used to script minigames, but this isn't implemented yet.

### Action Scripts

These scripts are activated once by either a **hotspot** or **trigger**.

#### Examples

```wren
// load_lightshow.wren //
import "televoid/core" for Scene
Scene.load("res/scenes/demo/Lightshow.tmx")
```

This script is quite simple, only two lines of code! The very first line will be used within every script, though what it's for may change. Here we only need the scene functionality, so that's what we import. This allows us to perform line 2. We load in the scene that we want using the relative path from the executable file.

```wren
// win_test.wren //
import "televoid/core" for Scene, Inventory
if (Inventory.has("key") && Inventory.has("blue_key")) {
  Inventory.remove("key")
  Inventory.remove("blue_key")
  Scene.load("res/scenes/demo/Win.tmx")
} else {
  System.print("It's Locked, need keys.") // This will only print in terminal
}
```

This script is a little more complex, we import two things: Scene and Inventory. This allows us to do basic inventory checks using `Inventory.has(item_name)`. We may also load the scene `res/scenes/demo/Win.tmx` if we do meet the requirements of possessing both *key* and *blue_key*.