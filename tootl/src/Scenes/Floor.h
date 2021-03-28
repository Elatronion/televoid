#ifndef FLOOR_H
#define FLOOR_H
#include <HGE/HGE_Core.h>

typedef struct floor_node_t {
  hge_vec2 position;
  struct floor_node_t* next;
} floor_node;

typedef struct {
  floor_node* root;
} floor_t;

void floor_push_position(floor_t* floor, hge_vec2 position);

void system_floor(hge_entity* entity, floor_t* floor);
void system_floor_debug(hge_entity* entity, floor_t* floor);

#endif
