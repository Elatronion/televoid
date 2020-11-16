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

dialogue_component CreateDialogue(const char* str, const char* left_sprite, const char* right_sprite) {
  dialogue_component dialogue;
  dialogue.root = malloc(sizeof(dialogue_message));
  strcpy(dialogue.root->str, str);
  strcpy(dialogue.root->left_sprite, left_sprite);
  strcpy(dialogue.root->right_sprite, right_sprite);
  dialogue.closing = false;
  dialogue.gui_y = -500;
  dialogue.character_left_y = 0;
  dialogue.character_right_y = 0;
  //dialogue.cur_message = &dialogue.root;
  dialogue.root->next = NULL;
  return dialogue;
}

void AppendMessage(dialogue_message* root, const char* str, const char* left_sprite, const char* right_sprite) {
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
  strcpy(next->left_sprite, left_sprite);
  strcpy(next->right_sprite, right_sprite);
  next->next = NULL;
}

void DeleteDialogueTree(dialogue_message* root) {
  printf("%s\n", root->str);
  dialogue_message* next = root;
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

  if(dialogue->closing) {
    dialogue->gui_y += (-500 - dialogue->gui_y) * 10.f * hgeDeltaTime();
    if(dialogue->gui_y <= -450) {
      SetGameState(GAME_PLAY);
      DeleteDialogueTree(dialogue->root);
      hgeDestroyEntity(entity);
    }
  } else {
    dialogue->gui_y += (0 - dialogue->gui_y) * 10.f * hgeDeltaTime();
  }

  dialogue->character_left_y += (0 - dialogue->character_left_y) * 10.f * hgeDeltaTime();
  dialogue->character_right_y += (0 - dialogue->character_right_y) * 10.f * hgeDeltaTime();

  hge_vec3 bg_scale = { fmin(800 - 50, hgeWindowWidth() - 50), 150, 0 };
  hge_vec3 bg_position = { 0, dialogue->gui_y + -hgeWindowHeight()/2 + bg_scale.y/2 + 25, 99 };

  float text_size = 50.f; // 100.f
  float text_height = text_size/2;
  hge_vec3 text_position = { bg_position.x - bg_scale.x/2 + 5, bg_position.y + bg_scale.y/2 - text_height, 100 };
  hge_vec4 text_color = { 1, 1, 1, 1 };

  hge_shader gui_shader = hgeResourcesQueryShader("gui");
  hgeUseShader(gui_shader);

  hge_vec3 character_left_scale = { 475, 475, 0 };
  hge_vec3 character_left_position = {
    bg_position.x - bg_scale.x/2 + character_left_scale.x/4,
    dialogue->gui_y + dialogue->character_left_y + -hgeWindowHeight()/2 + character_left_scale.y/2,
    98
  };
  character_left_scale.x = - character_left_scale.x;

  hge_vec3 character_right_scale = { 475, 475, 0 };
  hge_vec3 character_right_position = {
    bg_position.x + bg_scale.x/2 - character_right_scale.x/4,
    dialogue->gui_y + dialogue->character_right_y + -hgeWindowHeight()/2 + character_left_scale.y/2,
    98
  };

  hge_material character_left_material = { hgeResourcesQueryTexture(dialogue->cur_message->left_sprite), hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  hgeShaderSetVec4(gui_shader, "gui_color", hgeVec4(1, 1, 1, 1));
  hgeRenderSprite(
    gui_shader,
    character_left_material,
    character_left_position, character_left_scale, 0.0f
  );

  hge_material character_right_material = { hgeResourcesQueryTexture(dialogue->cur_message->right_sprite), hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  hgeShaderSetVec4(gui_shader, "gui_color", hgeVec4(1, 1, 1, 1));
  hgeRenderSprite(
    gui_shader,
    character_right_material,
    character_right_position, character_right_scale, 0.0f
  );

  hge_material gui_material = { hgeResourcesQueryTexture("dialogue background"), hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  hgeShaderSetVec4(gui_shader, "gui_color", hgeVec4(0, 0, 0, 0.5f));
  hgeRenderSprite(
    gui_shader,
    gui_material,
    bg_position, bg_scale, 0.0f
  );

  televoidTextRenderSimple(dialogue->cur_message->str, false, text_position, text_size/100, text_color);

  if(hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
    if(dialogue->cur_message->next) {
      dialogue->character_left_y = -500;
      dialogue->character_right_y = -500;
      dialogue->cur_message = dialogue->cur_message->next;
    }
    else {
      dialogue->closing = true;
      /*SetGameState(GAME_PLAY);
      DeleteDialogueTree(dialogue->root);
      hgeDestroyEntity(entity);*/
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

void televoidCreateDialogue() {
  hge_entity* dialogue_entity = hgeCreateEntity();
  dialogue_component dialogue = CreateDialogue("This is the first message!", "Frank_Idle", "Sam_Idle");
  //printf("gui_y : %s\n", dialogue.root.str);
  dialogue_pointers[num_of_dialogue_pointers] = dialogue.root;
  num_of_dialogue_pointers++;
  dialogue.cur_message = dialogue.root;

  AppendMessage(dialogue.root, "This is the second message!", "Ian_Idle", "Frank_Idle");
  AppendMessage(dialogue.root, "This is the last message.\nSo, why not test some crazy stuff!", "Frank_Idle", "Sam_Idle");
  hgeAddComponent(dialogue_entity, hgeCreateComponent("Dialogue", &dialogue, sizeof(dialogue)));
}
