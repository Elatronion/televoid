#include "GameState.h"

game_state global_game_state = GAME_PLAY;

game_state televoidGameState() { return global_game_state; }

void televoidSetGameState(game_state state) {
  switch(global_game_state) {
      case GAME_PLAY:
        global_game_state = state;
      break;
      default:
        if(state == GAME_PLAY)
          global_game_state = state;
      break;
  }
}
