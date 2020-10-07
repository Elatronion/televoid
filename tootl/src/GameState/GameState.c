#include "GameState.h"
#include <HGE/HGE_Log.h>

game_state GAME_STATE = GAME_PLAY;
bool debug_mode = false;

game_state GetGameState(){ return GAME_STATE; }
void SetGameState(game_state state){ GAME_STATE = state; };

void SetDebugMode(bool debug) {
  if(debug) { HGE_WARNING("DEBUG MODE ENABLED!"); }
  else { HGE_WARNING("DEBUG MODE DISABLED!"); }
  debug_mode = debug;
}
bool IsDebugMode(){ return debug_mode; }
