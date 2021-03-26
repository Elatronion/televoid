#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <HGE/HGE_Core.h>
#include <stdbool.h>

/*
  The Game FSM
*/

typedef enum {
  GAME_PLAY,
  GAME_PAUSE,
  GAME_CUTSCENE,
  GAME_DIALOGUE,
  GAME_MINIGAME
} game_state;

game_state televoidGameState();

void televoidSetGameState(game_state state);

#endif
