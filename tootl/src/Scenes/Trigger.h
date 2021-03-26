#ifndef TRIGGER_H
#define TRIGGER_H
#include <HGE/HGE_Core.h>

#define TRIGGER_EXEC_NULL 0x0
#define TRIGGER_EXEC_ON_ENTER 0x1
#define TRIGGER_EXEC_ON_EXIT 0x2

typedef struct {
  unsigned int exec_flags;
  char data[255];
  bool intersected;
} trigger_component;

void system_trigger_logic(hge_entity* e, hge_transform* transform, trigger_component* trigger);
void system_trigger_renderer(hge_entity* e, hge_transform* transform, trigger_component* trigger);


#endif
