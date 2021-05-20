#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <HGE/HGE_Core.h>

typedef enum {
  DIALOGUE_MESSAGE,
  DIALOGUE_SCRIPT,
  DIALOGUE_SNIPPET,
  DIALOGUE_LEFT_SPRITE,
  DIALOGUE_RIGHT_SPRITE
} dialogue_event_type;

typedef struct {
  dialogue_event_type type;
  const char* data;
} dialogue_event;

typedef struct dialogue_event_node_t {
  dialogue_event event;
  struct dialogue_event_node_t* next;
} dialogue_event_node;

typedef struct {
  dialogue_event_node* head;
  dialogue_event_node* current;
  hge_texture left_character_sprite;
  hge_texture right_character_sprite;
  bool active_is_right;
} dialogue_component;

dialogue_event_node* dialogue_event_create(dialogue_event_type type, const char* data);
void dialogue_event_destroy(dialogue_event_node* event_node);
void dialogue_event_destroy_list(dialogue_event_node* head);
void dialogue_event_push_list(dialogue_event_node* head, dialogue_event_node* node);

dialogue_event_node* dialogue_load(const char* file_path);

void system_dialogue(hge_entity* entity, dialogue_component* dialogue);

#endif
