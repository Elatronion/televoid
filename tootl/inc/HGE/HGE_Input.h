#ifndef HGE_INPUT_H
#define HGE_INPUT_H

#include <stdbool.h>
#include "HGE_Math3D.h"

/* The unknown key */
#define HGE_KEY_UNKNOWN            -1

/* Printable keys */
#define HGE_KEY_SPACE              32
#define HGE_KEY_APOSTROPHE         39  /* ' */
#define HGE_KEY_COMMA              44  /* , */
#define HGE_KEY_MINUS              45  /* - */
#define HGE_KEY_PERIOD             46  /* . */
#define HGE_KEY_SLASH              47  /* / */
#define HGE_KEY_0                  48
#define HGE_KEY_1                  49
#define HGE_KEY_2                  50
#define HGE_KEY_3                  51
#define HGE_KEY_4                  52
#define HGE_KEY_5                  53
#define HGE_KEY_6                  54
#define HGE_KEY_7                  55
#define HGE_KEY_8                  56
#define HGE_KEY_9                  57
#define HGE_KEY_SEMICOLON          59  /* ; */
#define HGE_KEY_EQUAL              61  /* = */
#define HGE_KEY_A                  65
#define HGE_KEY_B                  66
#define HGE_KEY_C                  67
#define HGE_KEY_D                  68
#define HGE_KEY_E                  69
#define HGE_KEY_F                  70
#define HGE_KEY_G                  71
#define HGE_KEY_H                  72
#define HGE_KEY_I                  73
#define HGE_KEY_J                  74
#define HGE_KEY_K                  75
#define HGE_KEY_L                  76
#define HGE_KEY_M                  77
#define HGE_KEY_N                  78
#define HGE_KEY_O                  79
#define HGE_KEY_P                  80
#define HGE_KEY_Q                  81
#define HGE_KEY_R                  82
#define HGE_KEY_S                  83
#define HGE_KEY_T                  84
#define HGE_KEY_U                  85
#define HGE_KEY_V                  86
#define HGE_KEY_W                  87
#define HGE_KEY_X                  88
#define HGE_KEY_Y                  89
#define HGE_KEY_Z                  90
#define HGE_KEY_LEFT_BRACKET       91  /* [ */
#define HGE_KEY_BACKSLASH          92  /* \ */
#define HGE_KEY_RIGHT_BRACKET      93  /* ] */
#define HGE_KEY_GRAVE_ACCENT       96  /* ` */
#define HGE_KEY_WORLD_1            161 /* non-US #1 */
#define HGE_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define HGE_KEY_ESCAPE             256
#define HGE_KEY_ENTER              257
#define HGE_KEY_TAB                258
#define HGE_KEY_BACKSPACE          259
#define HGE_KEY_INSERT             260
#define HGE_KEY_DELETE             261
#define HGE_KEY_RIGHT              262
#define HGE_KEY_LEFT               263
#define HGE_KEY_DOWN               264
#define HGE_KEY_UP                 265
#define HGE_KEY_PAGE_UP            266
#define HGE_KEY_PAGE_DOWN          267
#define HGE_KEY_HOME               268
#define HGE_KEY_END                269
#define HGE_KEY_CAPS_LOCK          280
#define HGE_KEY_SCROLL_LOCK        281
#define HGE_KEY_NUM_LOCK           282
#define HGE_KEY_PRINT_SCREEN       283
#define HGE_KEY_PAUSE              284
#define HGE_KEY_F1                 290
#define HGE_KEY_F2                 291
#define HGE_KEY_F3                 292
#define HGE_KEY_F4                 293
#define HGE_KEY_F5                 294
#define HGE_KEY_F6                 295
#define HGE_KEY_F7                 296
#define HGE_KEY_F8                 297
#define HGE_KEY_F9                 298
#define HGE_KEY_F10                299
#define HGE_KEY_F11                300
#define HGE_KEY_F12                301
#define HGE_KEY_F13                302
#define HGE_KEY_F14                303
#define HGE_KEY_F15                304
#define HGE_KEY_F16                305
#define HGE_KEY_F17                306
#define HGE_KEY_F18                307
#define HGE_KEY_F19                308
#define HGE_KEY_F20                309
#define HGE_KEY_F21                310
#define HGE_KEY_F22                311
#define HGE_KEY_F23                312
#define HGE_KEY_F24                313
#define HGE_KEY_F25                314
#define HGE_KEY_KP_0               320
#define HGE_KEY_KP_1               321
#define HGE_KEY_KP_2               322
#define HGE_KEY_KP_3               323
#define HGE_KEY_KP_4               324
#define HGE_KEY_KP_5               325
#define HGE_KEY_KP_6               326
#define HGE_KEY_KP_7               327
#define HGE_KEY_KP_8               328
#define HGE_KEY_KP_9               329
#define HGE_KEY_KP_DECIMAL         330
#define HGE_KEY_KP_DIVIDE          331
#define HGE_KEY_KP_MULTIPLY        332
#define HGE_KEY_KP_SUBTRACT        333
#define HGE_KEY_KP_ADD             334
#define HGE_KEY_KP_ENTER           335
#define HGE_KEY_KP_EQUAL           336
#define HGE_KEY_LEFT_SHIFT         340
#define HGE_KEY_LEFT_CONTROL       341
#define HGE_KEY_LEFT_ALT           342
#define HGE_KEY_LEFT_SUPER         343
#define HGE_KEY_RIGHT_SHIFT        344
#define HGE_KEY_RIGHT_CONTROL      345
#define HGE_KEY_RIGHT_ALT          346
#define HGE_KEY_RIGHT_SUPER        347
#define HGE_KEY_MENU               348
#define HGE_KEY_LAST               HGE_KEY_MENU

#define HGE_MOUSE_LEFT             0
#define HGE_MOUSE_RIGHT            1
#define HGE_MOUSE_LAST             HGE_MOUSE_RIGHT

inline bool hgeInputGetKey(int keyCode);
inline bool hgeInputGetKeyDown(int keyCode);
inline bool hgeInputGetKeyUp(int keyCode);
inline bool hgeInputGetMouse(int keyCode);
inline bool hgeInputGetMouseDown(int keyCode);
inline bool hgeInputGetMouseUp(int keyCode);
inline hge_vec3 hgeInputGetMousePosition();

inline void hgeInputSetKey(int keyCode, bool value);
inline void hgeInputSetKeyDown(int keyCode, bool value);
inline void hgeInputSetKeyUp(int keyCode, bool value);
inline void hgeInputSetMouse(int keyCode, bool value);
inline void hgeInputSetMouseDown(int keyCode, bool value);
inline void hgeInputSetMouseUp(int keyCode, bool value);
inline void hgeInputSetMouseX(int value);
inline void hgeInputSetMouseY(int value);

#endif
