#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <HGE/HGE_Core.h>

typedef struct {
  hge_vec3 interaction_location;
  //void (*event)();
  char script[255];
} hotspot_component;

void HotspotSystem(hge_entity* e, hge_transform* transform, hotspot_component* hotspot);

#endif
