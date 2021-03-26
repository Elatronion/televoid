# Scenes

Scenes define an explorable area for the player.
It is used to place various elements such as clickable hotspots, items, backgrounds, lights, basic collision shapes and characters.

## The TMX file format

Scenes are defined using tmx files that are generated using [Tiled](https://www.mapeditor.org/).

Scenes rely heavily on object layers. Layers, their name, color or order are not limited or important, they are simply used to help designers organize their levels. 

That said, there are some common guidelines you may follow that could help.

## Layers

Type | Name | Color | Description
--- | --- | --- | ---
Object | [Items](#Items) | (255, 0, 255) | Items that can be picked up by the player, they only appear if the player has not said item.
Object | [Hotspots](#Hotspots) | (255, 255, 0) | Hotspots will activate a script or snippet when the player interacts.
Object | [Triggers](#Triggers) | (255, 99, 71) | Triggers will activate a script or snippet when the player intersects.
Object | [Lights](#Lights) | (255, 255, 255) | Lights, defined by points, are used to decorate the scene with real time lighting effects.
Object | [Player Starting Positions](#Player Starting Positions) | (128, 0, 255) | Starting Positions that the player can appear in the scene at, defined by points.
Object | [Props](#Props) | (0, 255, 0)     | Props are used to decorate the scene with sprite entities.
Image | [Background](#Background) | N/A | Defines a background image.

### Items

Objects that can be picked up by the player. If the item exists within the inventory, the object shall not spawn on scene load.

Property | Type | Description
--- | --- | ---
Name | string | This will be the item's sprite, loaded from `res/textures/sprites/inventory/items/<Name>.png`
Type | string | Must be set to `item` to set object as item. 

Items have no special shape or custom properties.
Items are 9x9.

### Hotspots

An area that can be clicked and interacted with by a character.

Property | Type | Description
--- | --- | ---
Name | string | Used for organization.
Type | string | Must be set to `hotspot` to set object as hotspot. 
snippet | string | A snippet of wren code that will be executed upon interaction.
script | string | Path to wren script that will be executed upon interaction. e.g. `res/scripts/test.wren`
auto_exec | bool | Should the script/snippet be executed once the scene loads.

Hotspots are axis-aligned bounding boxes, rectangles that have no rotation. There are three custom properties that you may add. At minimum, you need a snippet or script.

If a snippet AND a script exist, only the script shall be executed.

### Triggers

Similar to [hotspots](#Hotspots), triggers are areas that, when intersected by Ian, will execute a snippet or script.

Property | Type | Description
--- | --- | ---
Name | string | Used for organization.
Type | string | Must be set to `trigger` to set object as trigger. 
snippet | string | A snippet of wren code that will be executed upon player intersection. 
script | string | Path to wren script that will be executed upon player intersection. e.g. `res/scripts/test.wren` 

Triggers are axis-aligned bounding boxes, rectangles that have no rotation. There are three custom properties that you may add. At minimum, you need a snippet or script.

If a snippet AND a script exist, only the script shall be executed.

### Lights

A point in space that will dynamically light the scene, it's props and characters included.

Property | Type | Description
--- | --- | ---
Name | string | Used for organization.
Type | string | Must be set to `light` to set object as light. 
ambient | color | The color of the light.
diffuse | color | The color of the light's shadow, diffused across an object.

Lights are points in space, they must include both `ambient` and `diffuse` custom properties.

### Player Starting Positions

A point in space that describes a starting location for Ian.

Property | Type | Description
--- | --- | ---
Name | string | Used for organization.
Type | string | Must be set to `player_start` to set object as a player starting position. 
face_left | bool | Will the player be facing left when spawning at this location?
previous_scene_that_activates | string | The scene that, if loaded from, will activate this starting position. e.g. `res/scenes/test.tmx`

If previous_scene_that_activates is empty, it will be used if no other previous_scene_that_activates apply.
If no player starting position is found in a scene, Ian will not spawn.

### Props

A sprite with no logic, used for decoration.

Property | Type | Description
--- | --- | ---
Name | string | This will be the item's name and sprite, loaded from `res/textures/sprites/props/<Name>.png` 
Type | string | Must be set to `prop` to set object as prop. 

Props are axis-aligned bounding boxes, rectangles that have no rotation. The prop will be rendered within the boundaries.

### Background

Background are used to fill the gaps with massive images or meshes

Property | Type | Description
--- | --- | ---
Name | string | Used for organization.
Horizontal Offset | float | Offset horizontal position of background. 
Vertical Offset | float | Offset vertical position of background. 
Image | file | Image that shall be displayed. MUST BE IN SAME DIRECTORY AS SCENE 
depth | float | How far is the background. (default is 50)
lit | bool | Should the element react to lights.
mesh | string | Path to a desired mesh, used for 3d background. Image will be used as the texture. e.g. `res/meshes/test.obj` 

Background or foreground elements are represented by image layers. If you wish to have a 3D mesh, you may provide one using the custom property, if none is set, then the image shall be displayed as normal.

The mesh's origin will be aligned to the top left corner of the representing image.