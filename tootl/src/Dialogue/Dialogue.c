#include "Dialogue.h"
#include "GameState.h"
#include "SafeTrash.h"
#include <HGE/HGE_FileUtility.h>

dialogue_event_node* dialogue_event_create(dialogue_event_type type, const char* data) {
  dialogue_event_node* event_node = (dialogue_event_node *) malloc(sizeof(dialogue_event_node));
  event_node->event.type = type;
  //event_node->event.data = (char *) malloc(strlen(data) + 1);
  strcpy(event_node->event.data, data);
  event_node->event.data[strlen(data)+1] = '\0';
  event_node->next = NULL;

  //st_track(event_node->event.data);
  st_track(event_node);

  return event_node;
}

void dialogue_event_destroy(dialogue_event_node* event_node) {
  //st_untrack(event_node->event.data);
  st_untrack(event_node);
  //free(event_node->event.data);
  free(event_node);
}

void dialogue_event_destroy_list(dialogue_event_node* head) {
  dialogue_event_node* current = head;
  dialogue_event_node* next = head->next;
  while(current) {
    next = current->next;
    dialogue_event_destroy(current);
    current = next;
  }
}

void dialogue_event_push_list(dialogue_event_node* head, dialogue_event_node* node) {
  dialogue_event_node * current = head;
  while (current->next) {
    current = current->next;
  }
  current->next = node;
}

dialogue_event_node* dialogue_load(const char* file_path) {
  dialogue_event_node* head = dialogue_event_create(DIALOGUE_MESSAGE, "This should be ignored...");
  dialogue_event_push_list(head, dialogue_event_create(DIALOGUE_MESSAGE, "[IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]\nN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]\nS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]\n[IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]\nAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]\nNE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]\n IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE] [IAN IS FINE]"));


  /*
  FILE * file;
  char * line = NULL;
  size_t len = 0;
  size_t read;

  file = fopen(file_path, "r");
  if (file == NULL)
      exit(EXIT_FAILURE);

  char type[50];
  char data[MAX_DATA_LENGTH];

  while ((read = hgeGetLine(&line, &len, file)) != -1) {
    sscanf(line, "%[^,],%[^\n]", &type, &data);

    char *pos;
    if ((pos=strchr(data, '\n')) != NULL) {
      *pos = '\0';
    }

    if(!strcmp(type, "TYPE")) continue;
    else if(!strcmp(type, "LEFT")) {
      dialogue_event_push_list(head, dialogue_event_create(DIALOGUE_LEFT_SPRITE, data));
    } else if(!strcmp(type, "RIGHT")) {
      dialogue_event_push_list(head, dialogue_event_create(DIALOGUE_RIGHT_SPRITE, data));
    } else if(!strcmp(type, "MESSAGE")) {

      strcat(data, "\n");
      if(data[0] == '"') {
        while(data[strlen(data)-2] != '"') {
          read = hgeGetLine(&line, &len, file);
          strcat(data, line);
        }
      }
      char tmp_data[MAX_DATA_LENGTH];
      strcpy(tmp_data, data);
      tmp_data[strlen(data)-2] = '\0';
      strcpy(data, &tmp_data[1]);

      dialogue_event_push_list(head, dialogue_event_create(DIALOGUE_MESSAGE, data));
    } else if(!strcmp(type, "SCRIPT")) {

      if(data[0] == '"') {
        char tmp_data[1000];
        strcpy(tmp_data, &data[1]);
        strcpy(data, tmp_data);
        while((pos=strchr(data, '"')) != NULL) {
          *pos = '\0';
        }
      }

      dialogue_event_push_list(head, dialogue_event_create(DIALOGUE_SCRIPT, data));
    } else if(!strcmp(type, "SNIPPET")) {

      if(data[0] == '"') {
        char tmp_data[1000];
        strcpy(tmp_data, &data[1]);
        strcpy(data, tmp_data);
        while((pos=strchr(data, '"')) != NULL) {
          *pos = '\0';
        }
        while((pos=strchr(data, '“')) != NULL) {
          *(pos-2) = ' ';
          *(pos-1) = ' ';
          *pos = '"';
        }
        while((pos=strchr(data, '”')) != NULL) {
          *(pos-2) = '"';
          *(pos-1) = ' ';
          *pos = ' ';
        }
      }

      dialogue_event_push_list(head, dialogue_event_create(DIALOGUE_SNIPPET, data));
    }
  }
  fclose(file);
  if (line)
      free(line);
  */

  dialogue_event_node* tmp = head;
  head = head->next;
  dialogue_event_destroy(tmp);

  return head;
}


void draw_dialogue_box(hge_texture left_sprite, hge_texture right_sprite, bool right_is_active, const char* string) {
  float y_offset = 0.0f;
  float y_offset_character_left = 0.0f;
  float y_offset_character_right = 0.0f;

  float half_screen_height = hgeWindowHeight()/2;

  hge_shader gui_shader = hgeResourcesQueryShader("gui");

  hge_vec3 bg_scale = { fmin(800 - 50, hgeWindowWidth() - 50), 150, 0 };
  //hge_vec3 bg_position = { 0, y_offset + -hgeWindowHeight()/2 + bg_scale.y/2 + 25, 99 };
  float box_y = -(hgeWindowHeight() / 2.f) + bg_scale.y/2 + 25;
  hge_vec3 bg_position = { 0, box_y , 99 };



  hge_material character_left_material = { left_sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  hge_vec3 character_left_scale = { 475, 475, 0 };
  hge_vec3 character_left_position = {
    bg_position.x - bg_scale.x/2 + character_left_scale.x/4,
    y_offset + y_offset_character_left + -half_screen_height + character_left_scale.y/2,
    98
  };

  hge_material character_right_material = { right_sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  hge_vec3 character_right_scale = { 475, 475, 0 };
  hge_vec3 character_right_position = {
    bg_position.x + bg_scale.x/2 - character_right_scale.x/4,
    y_offset + y_offset_character_right + -half_screen_height + character_right_scale.y/2,
    98
  };

  if(!right_is_active) {
    character_right_position.y -= 50;
    character_right_material.color_multiplier.w = 0.25f;
  } else {
    character_left_position.y -= 50;
    character_left_material.color_multiplier.w = 0.25f;
  }

  hge_transform character_left_transform = { character_left_position, character_left_scale, hgeQuaternion(0, 0, 0, 1) };
  hgeRenderSprite(
    gui_shader,
    character_left_material,
    character_left_transform
  );

  hge_transform character_right_transform = { character_right_position, character_right_scale, hgeQuaternion(0, 0, 0, 1) };
  hgeRenderSprite(
    gui_shader,
    character_right_material,
    character_right_transform
  );


  hge_material gui_background_material = { hgeResourcesQueryTexture("dialogue background"), hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 0, 1, 0.75f) };
  hge_transform bg_transform = { bg_position, bg_scale, hgeQuaternion(0, 0, 0, 1) };
  hgeRenderSprite(
    gui_shader,
    gui_background_material,
    bg_transform
  );

  float text_size = 50.f; // 100.f
  float text_height = text_size/2;
  hge_vec3 text_position = { bg_position.x - bg_scale.x/2 + 5, bg_position.y + bg_scale.y/2 - text_height, 100 };

  hge_material text_material;
  text_material.color_multiplier = hgeVec4(1, 1, 1, 1);

  hge_transform text_transform = {
    text_position,
    hgeVec3(text_size/100.f, text_size/100.f, 0),
    hgeQuaternion(0, 0, 0, 1)
  };


  hgeRenderText(
    hgeResourcesQueryShader("gui text"),
    text_material,
    text_transform,
    hgeResourcesQueryFont("VCR"),
    HGE_TEXT_ALIGNED_LEFT,
    string
  );
}

void dialogue_execute(dialogue_component* dialogue, dialogue_event event) {
  switch(event.type) {
    case DIALOGUE_SCRIPT:
      televoidWrenExecute(event.data);
    break;
    case DIALOGUE_SNIPPET:
      televoidWrenExecuteSnippet(event.data);
    break;
    case DIALOGUE_LEFT_SPRITE:
      dialogue->active_is_right = false;
      dialogue->left_character_sprite = hgeResourcesQueryTexture(event.data);
    break;
    case DIALOGUE_RIGHT_SPRITE:
      dialogue->active_is_right = true;
      dialogue->right_character_sprite = hgeResourcesQueryTexture(event.data);
    break;
  }
}

void system_dialogue(hge_entity* entity, dialogue_component* dialogue) {
  televoidSetGameState(GAME_DIALOGUE);

  if(hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
      dialogue->current = dialogue->current->next;
  }

  while(dialogue->current && dialogue->current->event.type != DIALOGUE_MESSAGE) {
    dialogue_execute(dialogue, dialogue->current->event);
    dialogue->current = dialogue->current->next;
  }

  if(dialogue->current == NULL) {
    dialogue_event_destroy_list(dialogue->head);
    televoidSetGameState(GAME_PLAY);
    hgeDestroyEntity(entity);
    return;
  }

  draw_dialogue_box(
    dialogue->left_character_sprite,
    dialogue->right_character_sprite,
    dialogue->active_is_right,
    dialogue->current->event.data
  );
}
