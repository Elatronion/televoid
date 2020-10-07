#include "Dialogue.h"
#include "text.h"
#include "GameState.h"

#define MAX_DIALOGUE_POINTERS 100
dialogue_message* dialogue_pointers[MAX_DIALOGUE_POINTERS];
int num_of_dialogue_pointers = 0;

void LoadDialogue(const char* dialogue_path) {
  // Load dialogue data from file
  // Add dialogue entity to game
}

void AppendMessage(dialogue_message* root, const char* str) {
  dialogue_message* next = root;
  while (next) {
    if(!next->next) break;
    next = next->next;
  }
  next->next = malloc(sizeof(dialogue_message));

  if(num_of_dialogue_pointers > MAX_DIALOGUE_POINTERS) {
    free(next->next);
    CleanUpForgottenDialoguePointers();
    printf("Critical Error: Too Many Dialogues!!!\n");
    // Close Game
  } else {
    dialogue_pointers[num_of_dialogue_pointers] = next->next;
    num_of_dialogue_pointers++;
  }

  next = next->next;
  strcpy(next->str, str);
  next->next = NULL;
}

void DeleteDialogueTree(dialogue_message root) {
  printf("%s\n", root.str);
  dialogue_message* next = root.next;
  while (next) {
    printf("%s\n", next->str);
    dialogue_message* tmp = next->next;
    free(next);
    next = tmp;
  }
  num_of_dialogue_pointers = 0;
}

void DialogueSystem(hge_entity* entity, dialogue_component* dialogue) {
  SetGameState(GAME_DIALOGUE);
  float text_size = 50.f; // 100.f
  hge_vec3 text_position = { -hgeWindowWidth()/2, /*-hgeWindowHeight()/2*/ hgeWindowHeight()/2 - text_size/2, 100 };
  hge_vec4 text_color = { 1, 1, 1, 1 };
  televoidTextRenderSimple(dialogue->cur_message->str, false, text_position, text_size/100, text_color);

  if(hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
    if(dialogue->cur_message->next)
      dialogue->cur_message = dialogue->cur_message->next;
    else {
      SetGameState(GAME_PLAY);
      DeleteDialogueTree(dialogue->root);
      hgeDestroyEntity(entity);
    }
  }

}

void CleanUpForgottenDialoguePointers() {
  if(num_of_dialogue_pointers > 0) printf("OUPS, FORGOT SOME DIALOGUE POINTERS!\n");
  for(int i = 0; i < num_of_dialogue_pointers; i++) {
    free(dialogue_pointers[i]);
  }
  num_of_dialogue_pointers = 0;
}
