#ifndef TELEVOID_PROP_H
#define TELEVOID_PROP_H
#include <HGE/HGE_Core.h>

typedef struct {
  hge_material material;
} prop_component;

void system_prop(hge_entity* entity, hge_transform* transform, hge_material* material, prop_component* prop);

#endif
