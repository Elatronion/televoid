#include "Minigame.h"
#include "GameState.h"
#define MINIGAME_MAX_MINIGAMES 10

int num_minigames = 0;
minigame_t minigames[MINIGAME_MAX_MINIGAMES];

void televoidMinigameCreate(const char* script_path, bool canExit) {
  minigame_t minigame;
  minigame.canExit = canExit;
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;
  config.bindForeignMethodFn = &bindForeignMethod;
  config.loadModuleFn = loadModule;
  minigame.vm = wrenNewVM(&config);

  const char* script = hgeLoadFileAsString(script_path);
  WrenInterpretResult result = wrenInterpret(minigame.vm, "main", script);

  switch (result) {
    case WREN_RESULT_COMPILE_ERROR:
    case WREN_RESULT_RUNTIME_ERROR:
      wrenFreeVM(minigame.vm);
    break;
    case WREN_RESULT_SUCCESS:
      minigames[num_minigames] = minigame;
      num_minigames++;
    break;
  }
}

bool processMinigameExitButton(int index) {
  hge_texture minigame_exit_texture = hgeResourcesQueryTexture("GUI Minigame Exit");
  hge_shader gui_shader = hgeResourcesQueryShader("gui");
  hge_material material = { minigame_exit_texture, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  float size = 64 * 2;
  float margin = 15.f;
  hge_transform transform = { hgeVec3(-(hgeWindowWidth()/2.f) + size/2.f + margin, (hgeWindowHeight()/2.f) - size/2.f - margin, 0), hgeVec3(size, size, 0), hgeQuaternion(0, 0, 0, 1) };
  hgeRenderSprite(
    gui_shader,
    material,
    transform
  );

  hge_vec3 position = hgeInputGetMousePosition();
  position.y = -position.y;
  position.x -= hgeWindowWidth()/2.f;
  position.y += hgeWindowHeight()/2.f;

  hge_transform mouse_transform = {
    position,
    hgeVec3(0, 0, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  if(AABB(mouse_transform, transform) && hgeInputGetMouse(HGE_MOUSE_LEFT)) {
    return true;
  }

  return false;
}

void televoidMinigameUpdate() {
  if(num_minigames > 0)
    televoidSetGameState(GAME_MINIGAME);
  if(televoidGameState() == GAME_MINIGAME && num_minigames == 0)
    televoidSetGameState(GAME_PLAY);

  for(int i = 0; i < num_minigames; i++) {
    if(minigames[i].canExit) {
      if(processMinigameExitButton(i)) {
        televoidMinigameClean();
        break;
      }
    }
    // Load the class into slot 0.
    wrenEnsureSlots(minigames[i].vm, 1);
    wrenGetVariable(minigames[i].vm, "main", "game", 0);
    WrenHandle* game_variable = wrenGetSlotHandle(minigames[i].vm, 0);
    wrenSetSlotHandle(minigames[i].vm, 0, game_variable);
    wrenSetSlotDouble(minigames[i].vm, 1, hgeDeltaTime());

    WrenHandle* game_update_method = wrenMakeCallHandle(minigames[i].vm, "update(_)");

    wrenCall(minigames[i].vm, game_update_method);

    wrenSetSlotHandle(minigames[i].vm, 0, game_variable);
    WrenHandle* game_rendering_method = wrenMakeCallHandle(minigames[i].vm, "render()");
    wrenCall(minigames[i].vm, game_rendering_method);
  }
}

void televoidMinigameClean() {
  for(int i = 0; i < num_minigames; i++) {
    wrenFreeVM(minigames[i].vm);
    minigames[i].vm = NULL;
  }
  num_minigames = 0;
}
