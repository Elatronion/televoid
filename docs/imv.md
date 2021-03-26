# IMVs

The Ian Music Video file format, used to define realtime animations and cutscenes for the tootl game.

## The IMV file format

The IMV format is simillar to the [Wavefront OBJ file format](https://en.wikipedia.org/wiki/Wavefront_.obj_file).

Anything following a hash character (#) is a comment.

```
# this is a comment
```

An IMV file may contain 2D animation elements, text elements, Scene Object references, keyframes, and scripts/snippets.

```
# let's create an element with the "GUI Inventory Slot" texture
# at the position (0,0,0) with a scale of (64, 64, 0) and rotation of 0
e 0 0 0 64 64 0 0 "GUI Inventory Slot"
# this element will have the following keyframes for it's x position property
k easeinout x 0 0
k easeinout x 3 100
k easeinout x 6 -100
k easeinout x 9 0
k easeinout x 10 0
# and these keyframes for it's y position property
k easeinout y 0 0
k easeinout y 3 100
k easeinout y 6 -100
k easeinout y 9 0

# scene objects may also be references for animation
o easeinout "camera"
k easeinout x 5 463
k easeinout y 5 -179
k easeinout z 5 50
k easeinout z 7 50

# Here is a script and a snippet, both will be executed at timestamp 3
s 3 "res/scripts/test.wren"
s 3 "System.print("Hello, Snippet!")"
```

The Z value acts as depth or layer, it is used to place elements in front or behind others. The higher the value, the closer it is to the camera.

### Elements

The most vital part of an IMV is it's elements. They properties and a bit of metadata. All you need to know is that they hold what you want to animate and can represent various different things from scene objects to scripts.

### Properties

Every element contains the following basic properties.

Property | Definition 
-- | --
x | The X position property
y | The Y position property
z | The Z position property
sx | The X scale property
sy | The Y scale property
sz | The Z scale property
r | The rotation property, degrees clockwise 
colorR | The Red Channel Multiplier
colorG | The Green Channel Multiplier
colorB | The Blue Channel Multiplier
colorA | The Alpha Channel Multiplier

Every property is independent and may be keyframed.

### IMV_ELEMENT_SPRITE

The sprite element is defined using the 'e' character followed by it's position, scale, (Z) rotation and texture string.

```
e 12 32 7 64 64 0 45 "Smile Face"
```

This would create a sprite element at position (12, 32, 7), scale (64, 64, 0) and rotation 45deg.

### IMV_ELEMENT_TEXT

The text element is defined using the 't' character followed by it's (X, Y, Z) position, font size, and the text string.

```
t 42 -12 10 2 "Hello, World!"
```

This would create a text element at position (42, -12, 10), font size 2 and the text string "Hello, World".

### IMV_ELEMENT_ENTITY

The entity element is defined using the 'o' character followed by the first keyframe type (using the inital position as the first keyframe at timestamp 0) and the string name of the scene object (game entity).

```
o easeinout "camera"
```

This would reference the game camera: scene object "camera".

### IMV Keyframes

Keyframes will be used to manipulate various element properties over the course of time.
Keyframes are applied to the last defined element.

```
k linear x 3 0
```

This is the baisc pattern of a keyframe. This defines a keyframe of type linear for the property x at timestamp 3 will a value 0. It says that x will be 0 at timestamp 3.

But, for what element? As aforementioned, it will apply to the last defined element. As such, at least one element must be defined before a keyframe can be created.

```
e 0 0 0 64 64 0 0 "Smiley"
k easeinout x 0 0
k easeinout x 3 100

e 0 0 0 64 64 0 0 "Birdie"
k linear y 0 0
k linear y 3 100
```

This would ease in out the x position property of element "Smiley" from 0 to 100 in the timespan between 0 and 3 seconds. It would also linearly interpolate the y position property of element "Birdie" from 0 to 100 in the same timespan.

tl;dr "Smiley" would go right smoothly and "Birdie" would go up robotically.

#### IMV Data Keyframes

Used to change the data of an sprite or text element.

```imv
d 0 "data"
```

Explanation...

#### IMV Keyframe interpolation

Keyframes can have different interpolation types.

Keyframe Type | Definition 
-- | --
linear | linearly interpolate between two values
easeinout | slow at the beginning and at the end, speeding up in between
hold | unchanged value until new keyframe

### IMV_ELEMENT_SCRIPT / IMV_ELEMENT_SNIPPET

The script/snippet element is defined using the 's' character followed by the execution timestamp and either the script or snippet that shall be executed.

```
s 3 "res/scripts/test.wren"
```

This would create a script element that would be execute the script "res/scripts/test.wren" at timestamp 3.

```
s 3 "System.print("Hello, World!")"
```

This would create a snippet element that would execute the snippet "System.print("Hello, World!")" at timestamp 3, therefore printing "Hello, World!" to the terminal.