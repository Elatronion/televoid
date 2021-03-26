#ifndef MINIGAME_H
#define MINIGAME_H
#include <HGE/HGE_Core.h>
#include "Scripting.h"

typedef struct {
  WrenVM* vm;
  bool canExit;
} minigame_t;

void televoidMinigameCreate(const char* script_path, bool canExit);
void televoidMinigameUpdate();
void televoidMinigameClean();

#endif
