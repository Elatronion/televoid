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
  void* next;
} dialogue_message;

typedef struct {
  dialogue_message root;
  dialogue_message* cur_message;
} dialogue_component;

void LoadDialogue(const char* dialogue_path);

void AppendMessage(dialogue_message* root, const char* str);
void DeleteDialogueTree(dialogue_message root);

void DialogueSystem(hge_entity* entity, dialogue_component* dialogue);

void CleanUpForgottenDialoguePointers();

#endif
