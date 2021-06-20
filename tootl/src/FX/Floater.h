#ifndef FLOATER_H
#define FLOATER_H
#include <HGE/HGE_Core.h>

typedef struct {
  float amplitude;
  float frequency;
  float angle_of_rotation;
  hge_transform original_transform;
} component_floater;

void system_floater(hge_entity* entity, hge_transform* transform, component_floater* floater);

#endif
