#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <HGE/HGE_Core.h>

/*
  IMPORTANT NOTE:
  ONLY ONE DIALOGUE COMPONENT SHOULD EXIST AT ANY GIVING TIME
  CAUSES MEMORY LEAK, AMONG OTHER ISSUES
*/

typedef struct {
  char str[255];
  char left_sprite[255];
  char right_sprite[255];
  void* next;
} dialogue_message;

typedef struct {
  dialogue_message* root;
  dialogue_message* cur_message;
  float gui_y;
  float character_left_y;
  float character_right_y;
  bool closing;
} dialogue_component;

void LoadDialogue(const char* dialogue_path);

dialogue_component CreateDialogue(const char* str, const char* left_sprite, const char* right_sprite);
void AppendMessage(dialogue_message* root, const char* str, const char* left_sprite, const char* right_sprite);
void DeleteDialogueTree(dialogue_message* root);

void DialogueSystem(hge_entity* entity, dialogue_component* dialogue);

void CleanUpForgottenDialoguePointers();

// Prefabs

void televoidCreateDialogue();

#endif
