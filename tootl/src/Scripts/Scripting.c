#include "Scripting.h"
#include "wren.h"
#include "Boombox.h"
#include "Character.h"
#include "Scene.h"
#include "GameSaver.h"

// // // // // // // // // // // // // // // //

void wrenSceneLoad(WrenVM* vm) {
  const char* str = wrenGetSlotString(vm, 1);
  televoidLoadScene(str);
}

void wrenInventoryAdd(WrenVM* vm) {
  const char* item_name = wrenGetSlotString(vm, 1);
  InventoryAddItem(televoidGetItemID(item_name));
}

void wrenInventoryRemove(WrenVM* vm) {
  const char* item_name = wrenGetSlotString(vm, 1);
  InventoryRemoveItem(televoidGetItemID(item_name));
}

void wrenInventoryHas(WrenVM* vm) {
  const char* item_name = wrenGetSlotString(vm, 1);
  wrenSetSlotBool(vm, 0, InventoryHasItem(televoidGetItemID(item_name)));
}

void wrenDialogueLoad(WrenVM* vm) {
  const char* path_str = wrenGetSlotString(vm, 1);
  televoidCreateDialogue(path_str);
}

void wrenMinigameLoad(WrenVM* vm) {
  const char* path_str = wrenGetSlotString(vm, 1);
  bool canExit = wrenGetSlotBool(vm, 2);
  televoidMinigameCreate(path_str, canExit);
}


void wrenRender(WrenVM* vm) {
  const char* sprite_name = wrenGetSlotString(vm, 1);
  float x = wrenGetSlotDouble(vm, 2);
  float y = wrenGetSlotDouble(vm, 3);
  float w = wrenGetSlotDouble(vm, 4);
  float h = wrenGetSlotDouble(vm, 5);
  float r = wrenGetSlotDouble(vm, 6);

  hge_shader gui_shader = hgeResourcesQueryShader("gui");
  hge_material material = { hgeResourcesQueryTexture(sprite_name), hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  hge_transform transform = { hgeVec3(x, y, 0), hgeVec3(w, h, 0), hgeQuaternionInitRotation(hgeVec3(0, 0, -1), hgeRadians(r)) };
  hgeRenderSprite(
    gui_shader,
    material,
    transform
  );
}

void wrenWindowWidth(WrenVM* vm) {
  float window_width = hgeWindowWidth();
  wrenSetSlotDouble(vm, 0, window_width);
}

void wrenWindowHeight(WrenVM* vm) {
  float window_height = hgeWindowHeight();
  wrenSetSlotDouble(vm, 0, window_height);
}

void wrenInputGetKey(WrenVM* vm) {
  int key_code = wrenGetSlotDouble(vm, 1);
  wrenSetSlotBool(vm, 0, hgeInputGetKey(key_code));
}

void wrenInputGetKeyDown(WrenVM* vm) {
  int key_code = wrenGetSlotDouble(vm, 1);
  wrenSetSlotBool(vm, 0, hgeInputGetKeyDown(key_code));
}

void wrenInputGetKeyUp(WrenVM* vm) {
  int key_code = wrenGetSlotDouble(vm, 1);
  wrenSetSlotBool(vm, 0, hgeInputGetKeyUp(key_code));
}

void wrenInputGetMouse(WrenVM* vm) {
  int mouse_code = wrenGetSlotDouble(vm, 1);
  wrenSetSlotBool(vm, 0, hgeInputGetMouse(mouse_code));
}

void wrenInputGetMouseDown(WrenVM* vm) {
  int mouse_code = wrenGetSlotDouble(vm, 1);
  wrenSetSlotBool(vm, 0, hgeInputGetMouseDown(mouse_code));
}

void wrenInputGetMouseUp(WrenVM* vm) {
  int mouse_code = wrenGetSlotDouble(vm, 1);
  wrenSetSlotBool(vm, 0, hgeInputGetMouseUp(mouse_code));
}


void wrenInputGetMouseX(WrenVM* vm) {
  hge_vec3 mouse_position = hgeInputGetMousePosition();
  wrenSetSlotDouble(vm, 0, mouse_position.x - hgeWindowWidth()/2.f);
}

void wrenInputGetMouseY(WrenVM* vm) {
  hge_vec3 mouse_position = hgeInputGetMousePosition();
  wrenSetSlotDouble(vm, 0, -mouse_position.y + hgeWindowHeight()/2.f);
}

void wrenLoadTexture(WrenVM* vm) {
  const char* path_str = wrenGetSlotString(vm, 1);
  const char* name_str = wrenGetSlotString(vm, 2);
  hgeResourcesLoadTexture(path_str, name_str);
}

void wrenLoadAudio(WrenVM* vm) {
  const char* path_str = wrenGetSlotString(vm, 1);
  const char* name_str = wrenGetSlotString(vm, 2);
  hgeResourcesLoadAudio(path_str, name_str);
}

void wrenPlayVoice(WrenVM* vm) {
  const char* name_str = wrenGetSlotString(vm, 1);
  televoidBoomboxPlayVoice(name_str);
}

void wrenPlayBGM(WrenVM* vm) {
  const char* name_str = wrenGetSlotString(vm, 1);
  televoidBoomboxPlayBGM(name_str);
}

void wrenPlaySFX(WrenVM* vm) {
  const char* name_str = wrenGetSlotString(vm, 1);
  televoidBoomboxPlaySFX(name_str);
}

void wrenLoadAnimation(WrenVM* vm) {
  const char* path_str = wrenGetSlotString(vm, 1);
  televoidIMVCreate(path_str);
}

void wrenCharacterWalk(WrenVM* vm) {
  float x = wrenGetSlotDouble(vm, 1);
  float y = wrenGetSlotDouble(vm, 2);
  const char* character_name = wrenGetSlotString(vm, 3);

  hge_entity* ian_entity = televoidSceneGetEntity(character_name);
  if(!ian_entity) {
    HGE_WARNING("Could not find any scene entity with name \"%s\"", character_name);
    return;
  }

  int character_component_id = hgeQuery(ian_entity, "character");
  printf("Character Component ID: %d\n", character_component_id);
  character_component* character = (character_component*) ian_entity->components[hgeQuery(ian_entity, "character")].data;
  hge_vec3 position = {x, y, 0};
  CharacterGroundClick(character, position);
}

void wrenGameSave(WrenVM* vm) {
  //televoidSave();
}

void wrenGameLoad(WrenVM* vm) {
  televoidLoad();
}

void wrenLerp(WrenVM* vm) {
  float a = wrenGetSlotDouble(vm, 1);
  float b = wrenGetSlotDouble(vm, 2);
  float f = wrenGetSlotDouble(vm, 3);
  wrenSetSlotDouble(vm, 0, hgeMathFunc_lerp(a, b, f));
}

void wrenEaseInOut(WrenVM* vm) {
  float a = wrenGetSlotDouble(vm, 1);
  float b = wrenGetSlotDouble(vm, 2);
  float f = wrenGetSlotDouble(vm, 3);
  wrenSetSlotDouble(vm, 0, hgeMathFunc_easeinout(a, b, f));
}

void wrenAABB(WrenVM* vm) {
  float Ax = wrenGetSlotDouble(vm, 1);
  float Ay = wrenGetSlotDouble(vm, 2);
  float Aw = wrenGetSlotDouble(vm, 3);
  float Ah = wrenGetSlotDouble(vm, 4);

  float Bx = wrenGetSlotDouble(vm, 5);
  float By = wrenGetSlotDouble(vm, 6);
  float Bw = wrenGetSlotDouble(vm, 7);
  float Bh = wrenGetSlotDouble(vm, 8);

  hge_transform A = {
    hgeVec3(Ax, Ay, 0),
    hgeVec3(Aw, Ah, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  hge_transform B = {
    hgeVec3(Bx, By, 0),
    hgeVec3(Bw, Bh, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  wrenSetSlotBool(vm, 0, AABB(A, B));
}

void wrenRand(WrenVM* vm) {
  int from = (int) (wrenGetSlotDouble(vm, 1));
  int to = (int) (wrenGetSlotDouble(vm, 2));

  int rand_value = hgeRandomInt(from, to);
  wrenSetSlotDouble(vm, 0, rand_value);
}

void wrenOptionsSet(WrenVM* vm) {
  float volume_master = wrenGetSlotDouble(vm, 1);
  float volume_voice = wrenGetSlotDouble(vm, 2);
  float volume_SFX = wrenGetSlotDouble(vm, 3);
  float volume_BGM = wrenGetSlotDouble(vm, 4);

  televoidBoomboxSettingsVolumeSetMaster(volume_master);
  televoidBoomboxSettingsVolumeSetVoice(volume_voice);
  televoidBoomboxSettingsVolumeSetSFX(volume_SFX);
  televoidBoomboxSettingsVolumeSetBGM(volume_BGM);
  televoidSaveOptions();
}

void wrenOptionsGet(WrenVM* vm) {
  const char* option_name = wrenGetSlotString(vm, 1);
  float value = 0.0f;

  if(strcmp(option_name, "volume master") == 0) {
    value = televoidBoomboxSettingsVolumeGetMaster();
  } else if(strcmp(option_name, "volume voice") == 0) {
    value = televoidBoomboxSettingsVolumeGetVoice();
  } else if(strcmp(option_name, "volume sfx") == 0) {
    value = televoidBoomboxSettingsVolumeGetSFX();
  } else if(strcmp(option_name, "volume bgm") == 0) {
    value = televoidBoomboxSettingsVolumeGetBGM();
  } else {
    HGE_ERROR("Option \"%s\" doesn't exist", option_name);
  }

  wrenSetSlotDouble(vm, 0, value);
}

void wrenOptionsLoad(WrenVM* vm) {
  televoidLoad();
}

// // // // // // // // // // // // // // // //

WrenForeignMethodFn bindForeignMethod(
    WrenVM* vm,
    const char* module,
    const char* className,
    bool isStatic,
    const char* signature)
{
  if (strcmp(module, "televoid-core") == 0)
  {
    if (strcmp(className, "Scene") == 0)
    {
      if (isStatic && strcmp(signature, "load(_)") == 0)
      {
        return wrenSceneLoad; // C function for Scene.load(_).
      }
      // Other foreign methods on WrenC...
    } else if(strcmp(className, "Inventory") == 0) {
      if (isStatic && strcmp(signature, "add(_)") == 0) {
        return wrenInventoryAdd; // C function for Inventory.add(_).
      } else if(isStatic && strcmp(signature, "remove(_)") == 0) {
        return wrenInventoryRemove; // C function for Inventory.remove(_).
      } else if(isStatic && strcmp(signature, "has(_)") == 0) {
        return wrenInventoryHas;
      }
    } else if(strcmp(className, "Dialogue") == 0) {
      if (isStatic && strcmp(signature, "load(_)") == 0)
      {
        return wrenDialogueLoad; // C function for Dialogue.load(_).
      }
    } else if (strcmp(className, "Minigame") == 0) {
      if (isStatic && strcmp(signature, "load(_,_)") == 0)
      {
        return wrenMinigameLoad;
      }
    } else if (strcmp(className, "Audio") == 0) {
      if (isStatic && strcmp(signature, "playSFX(_)") == 0) {
        return wrenPlaySFX;
      } else if(isStatic && strcmp(signature, "playBGM(_)") == 0) {
        return wrenPlayBGM;
      } else if(isStatic && strcmp(signature, "playVoice(_)") == 0) {
        return wrenPlayVoice;
      }
    } else if (strcmp(className, "Animation") == 0) {
      if (isStatic && strcmp(signature, "load(_)") == 0) {
        return wrenLoadAnimation;
      }
    } else if (strcmp(className, "Character") == 0) {
      if (isStatic && strcmp(signature, "walk(_,_,_)") == 0) {
        return wrenCharacterWalk;
      }
    } else if (strcmp(className, "GameSaver") == 0) {
      if (isStatic && strcmp(signature, "save()") == 0) {
        return wrenGameSave;
      } else if (isStatic && strcmp(signature, "load()") == 0) {
        return wrenGameLoad;
      }
    }
    // Other classes in televoid-core...
  } else if (strcmp(module, "minigame-core") == 0) {
    if (strcmp(className, "Window") == 0) {
      if (isStatic && strcmp(signature, "render(_,_,_,_,_,_)") == 0) {
        return wrenRender;
      } else if(isStatic && strcmp(signature, "getWidth()") == 0) {
        return wrenWindowWidth;
      } else if(isStatic && strcmp(signature, "getHeight()") == 0) {
        return wrenWindowHeight;
      }
    } else if (strcmp(className, "Input") == 0) {
      if (isStatic && strcmp(signature, "getKey(_)") == 0) {
        return wrenInputGetKey;
      } else if (isStatic && strcmp(signature, "getKeyDown(_)") == 0) {
        return wrenInputGetKeyDown;
      } else if (isStatic && strcmp(signature, "getKeyUp(_)") == 0) {
        return wrenInputGetKeyUp;
      } else if (isStatic && strcmp(signature, "getMouse(_)") == 0) {
        return wrenInputGetMouse;
      } else if (isStatic && strcmp(signature, "getMouseDown(_)") == 0) {
        return wrenInputGetMouseDown;
      } else if (isStatic && strcmp(signature, "getMouseUp(_)") == 0) {
        return wrenInputGetMouseUp;
      } else if (isStatic && strcmp(signature, "getMouseX()") == 0) {
        return wrenInputGetMouseX;
      } else if (isStatic && strcmp(signature, "getMouseY()") == 0) {
        return wrenInputGetMouseY;
      }
    } else if (strcmp(className, "ResourceManager") == 0) {
      if (isStatic && strcmp(signature, "loadTexture(_,_)") == 0) {
        return wrenLoadTexture;
      } else if (isStatic && strcmp(signature, "loadAudio(_,_)") == 0) {
        return wrenLoadAudio;
      }
    } else if (strcmp(className, "Math") == 0) {
      if (isStatic && strcmp(signature, "lerp(_,_,_)") == 0) {
        return wrenLerp;
      } else if (isStatic && strcmp(signature, "easeinout(_,_,_)") == 0) {
        return wrenEaseInOut;
      } else if (isStatic && strcmp(signature, "AABB(_,_,_,_,_,_,_,_)") == 0) {
        return wrenAABB;
      } else if (isStatic && strcmp(signature, "rand(_,_)") == 0) {
        return wrenRand;
      }
    } else if (strcmp(className, "Options") == 0) {
      if (isStatic && strcmp(signature, "set(_,_,_,_)") == 0) {
        return wrenOptionsSet;
      } else if (isStatic && strcmp(signature, "get(_)") == 0) {
        return wrenOptionsGet;
      } else if (isStatic && strcmp(signature, "load()") == 0) {
        return wrenOptionsLoad;
      }
    }
  }
  // Other modules...

}

void writeFn(WrenVM* vm, const char* text)
{
  printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

char* loadModule(WrenVM* vm, const char* name) {
  printf("Load Module: '%s'\n", name);
  char path[255];
  strcpy(path, "res/scripts/");
  strcat(path, name);
  strcat(path, ".wren");
  return hgeLoadFileAsString(path);
}

bool televoidWrenExecute(const char* script_path) {
  printf("Executing Script \"%s\"!\n", script_path);

  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;
  config.bindForeignMethodFn = &bindForeignMethod;
  config.loadModuleFn = loadModule;
  WrenVM* vm = wrenNewVM(&config);

  const char* script = hgeLoadFileAsString(script_path);
  WrenInterpretResult result = wrenInterpret(vm, "main", script);

  switch (result) {
    case WREN_RESULT_COMPILE_ERROR:
      { printf("Compile Error!\n"); } break;
    case WREN_RESULT_RUNTIME_ERROR:
      { printf("Runtime Error!\n"); } break;
    /*case WREN_RESULT_SUCCESS:
      { printf("Success!\n"); } break;*/
  }

  wrenFreeVM(vm);

  return result;
}

bool televoidWrenExecuteSnippet(const char* snippet) {
  printf("Executing Snippet \"%s\"!\n", snippet);

  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;
  config.bindForeignMethodFn = &bindForeignMethod;
  config.loadModuleFn = loadModule;
  WrenVM* vm = wrenNewVM(&config);

  char script[500] = "import \"televoid-core\" for Scene, Inventory, Dialogue, Minigame, Audio, Animation, Character, GameSaver\n";
  strcat(script, snippet);

  WrenInterpretResult result = wrenInterpret(vm, "main", script);

  switch (result) {
    case WREN_RESULT_COMPILE_ERROR:
      { printf("Compile Error!\n"); } break;
    case WREN_RESULT_RUNTIME_ERROR:
      { printf("Runtime Error!\n"); } break;
    /*case WREN_RESULT_SUCCESS:
      { printf("Success!\n"); } break;*/
  }

  wrenFreeVM(vm);

  return result;
}
