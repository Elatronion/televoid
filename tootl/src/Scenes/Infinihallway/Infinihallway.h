#ifndef INFINIHALLWAY_H
#define INFINIHALLWAY_H
#include <HGE/HGE_Core.h>

typedef struct {
  int dead_end;
} infinihallway_component;

void infinihallway_create_door_hotspot(int index, const char* script);
void system_infinihallway(hge_entity* e, infinihallway_component* infinihallway);

#endif
