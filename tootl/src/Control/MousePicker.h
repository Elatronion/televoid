#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H
#include <HGE/HGE_Core.h>

typedef struct {
  hge_vec3 current_ray;
} mouse_picker_component;

hge_vec3 mousePickerRaycast();
hge_transform mouseGUITransform();
void system_mouse_picker(hge_entity* entity, hge_transform* transform, mouse_picker_component* mouse_picker);

#endif
