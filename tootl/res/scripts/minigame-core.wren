class Vec2 {
  construct new(x, y) {
    _x = x
    _y = y
  }
  x { _x }
  y { _y }
  x=(value) { _x = value }
  y=(value) { _y = value }
  normalize() {
    var length = (_x*_x + _y*_y).sqrt
    _x = _x / length
    _y = _y / length
  }
}

class GameObject {
  construct new(position, scale) {
    _position = position
    _scale = scale
    _rotation = 0.0
  }
  construct new(position, scale, rotation) {
    _position = position
    _scale = scale
    _rotation = rotation
  }

  position { _position }
  scale { _scale }
  rotation { _rotation }
  position=(value) { _position = value }
  scale=(value) { _scale = value }
  rotation=(value) { _rotation = value }
}

class Window {
  foreign static getWidth()
  foreign static getHeight()
  static width { getWidth() }
  static height { getHeight() }

  foreign static render(sprite_name, x, y, w, h, r)
  static render(sprite_name, x, y, w, h) {
    render(sprite_name, x, y, w, h, 0.0)
   }
  static render(sprite_name, object) {
    render(sprite_name, object.position.x, object.position.y, object.scale.x, object.scale.y, object.rotation)
  }
}

class Input {
  foreign static getKey(key_code)
  foreign static getKeyDown(key_code)
  foreign static getKeyUp(key_code)
  foreign static getMouse(mouse_code)
  foreign static getMouseDown(mouse_code)
  foreign static getMouseUp(mouse_code)
  foreign static getMouseX()
  foreign static getMouseY()

  /* Printable keys */
  static KEY_SPACE              { 32 }
  static KEY_APOSTROPHE         { 39 }  /* ' */
  static KEY_COMMA              { 44 }  /* , */
  static KEY_MINUS              { 45 }  /* - */
  static KEY_PERIOD             { 46 }  /* . */
  static KEY_SLASH              { 47 }  /* / */
  static KEY_0                  { 48 }
  static KEY_1                  { 49 }
  static KEY_2                  { 50 }
  static KEY_3                  { 51 }
  static KEY_4                  { 52 }
  static KEY_5                  { 53 }
  static KEY_6                  { 54 }
  static KEY_7                  { 55 }
  static KEY_8                  { 56 }
  static KEY_9                  { 57 }
  static KEY_SEMICOLON          { 59 }  /* ; */
  static KEY_EQUAL              { 61 }  /* = */
  static KEY_A                  { 65 }
  static KEY_B                  { 66 }
  static KEY_C                  { 67 }
  static KEY_D                  { 68 }
  static KEY_E                  { 69 }
  static KEY_F                  { 70 }
  static KEY_G                  { 71 }
  static KEY_H                  { 72 }
  static KEY_I                  { 73 }
  static KEY_J                  { 74 }
  static KEY_K                  { 75 }
  static KEY_L                  { 76 }
  static KEY_M                  { 77 }
  static KEY_N                  { 78 }
  static KEY_O                  { 79 }
  static KEY_P                  { 80 }
  static KEY_Q                  { 81 }
  static KEY_R                  { 82 }
  static KEY_S                  { 83 }
  static KEY_T                  { 84 }
  static KEY_U                  { 85 }
  static KEY_V                  { 86 }
  static KEY_W                  { 87 }
  static KEY_X                  { 88 }
  static KEY_Y                  { 89 }
  static KEY_Z                  { 90 }
  static KEY_LEFT_BRACKET       { 91 }  /* [ */
  static KEY_BACKSLASH          { 92 }  /* \ */
  static KEY_RIGHT_BRACKET      { 93 }  /* ] */
  static KEY_GRAVE_ACCENT       { 96 }  /* ` */
  static KEY_WORLD_1            { 161 } /* non-US #1 */
  static KEY_WORLD_2            { 162 } /* non-US #2 */

  /* Function keys */
  static KEY_ESCAPE             { 256 }
  static KEY_ENTER              { 257 }
  static KEY_TAB                { 258 }
  static KEY_BACKSPACE          { 259 }
  static KEY_INSERT             { 260 }
  static KEY_DELETE             { 261 }
  static KEY_RIGHT              { 262 }
  static KEY_LEFT               { 263 }
  static KEY_DOWN               { 264 }
  static KEY_UP                 { 265 }
  static KEY_PAGE_UP            { 266 }
  static KEY_PAGE_DOWN          { 267 }
  static KEY_HOME               { 268 }
  static KEY_END                { 269 }
  static KEY_CAPS_LOCK          { 280 }
  static KEY_SCROLL_LOCK        { 281 }
  static KEY_NUM_LOCK           { 282 }
  static KEY_PRINT_SCREEN       { 283 }
  static KEY_PAUSE              { 284 }
  static KEY_F1                 { 290 }
  static KEY_F2                 { 291 }
  static KEY_F3                 { 292 }
  static KEY_F4                 { 293 }
  static KEY_F5                 { 294 }
  static KEY_F6                 { 295 }
  static KEY_F7                 { 296 }
  static KEY_F8                 { 297 }
  static KEY_F9                 { 298 }
  static KEY_F10                { 299 }
  static KEY_F11                { 300 }
  static KEY_F12                { 301 }
  static KEY_F13                { 302 }
  static KEY_F14                { 303 }
  static KEY_F15                { 304 }
  static KEY_F16                { 305 }
  static KEY_F17                { 306 }
  static KEY_F18                { 307 }
  static KEY_F19                { 308 }
  static KEY_F20                { 309 }
  static KEY_F21                { 310 }
  static KEY_F22                { 311 }
  static KEY_F23                { 312 }
  static KEY_F24                { 313 }
  static KEY_F25                { 314 }
  static KEY_KP_0               { 320 }
  static KEY_KP_1               { 321 }
  static KEY_KP_2               { 322 }
  static KEY_KP_3               { 323 }
  static KEY_KP_4               { 324 }
  static KEY_KP_5               { 325 }
  static KEY_KP_6               { 326 }
  static KEY_KP_7               { 327 }
  static KEY_KP_8               { 328 }
  static KEY_KP_9               { 329 }
  static KEY_KP_DECIMAL         { 330 }
  static KEY_KP_DIVIDE          { 331 }
  static KEY_KP_MULTIPLY        { 332 }
  static KEY_KP_SUBTRACT        { 333 }
  static KEY_KP_ADD             { 334 }
  static KEY_KP_ENTER           { 335 }
  static KEY_KP_EQUAL           { 336 }
  static KEY_LEFT_SHIFT         { 340 }
  static KEY_LEFT_CONTROL       { 341 }
  static KEY_LEFT_ALT           { 342 }
  static KEY_LEFT_SUPER         { 343 }
  static KEY_RIGHT_SHIFT        { 344 }
  static KEY_RIGHT_CONTROL      { 345 }
  static KEY_RIGHT_ALT          { 346 }
  static KEY_RIGHT_SUPER        { 347 }
  static KEY_MENU               { 348 }

  /* mouse buttons */
  static MOUSE_LEFT             { 0 }
  static MOUSE_RIGHT            { 1 }
}

class ResourceManager {
  foreign static loadTexture(path, name)
  foreign static loadAudio(path, name)
}

class Math {
  foreign static lerp(a, b, f)
  foreign static easeinout(a, b, f)

  foreign static AABB(Ax, Ay, Aw, Ah, Bx, By, Bw, Bh)
  static AABB(objectA, objectB) { Math.AABB(objectA.position.x, objectA.position.y, objectA.scale.x, objectA.scale.y, objectB.position.x, objectB.position.y, objectB.scale.x, objectB.scale.y) }

  foreign static rand(from, to)
}

class Options {
  foreign static set(volume_master, volume_voice, volume_BGM, volume_SFX)
  foreign static get(option_name)
  foreign static load()
}
