#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

typedef enum {
  GAME_PLAY,
  GAME_PAUSE,
  GAME_MENU,
  GAME_CUTSCENE,
  GAME_DIALOGUE,
  GAME_MINIGAME
} game_state;

game_state GetGameState();
void SetGameState(game_state state);

void SetDebugMode(bool debug);
bool IsDebugMode();

#endif
