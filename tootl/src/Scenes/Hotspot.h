#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <HGE/HGE.h>

typedef enum {
	HOTSPOT_INTERACTABLE_SCRIPT,
	HOTSPOT_INTERACTABLE_SNIPPET,
	HOTSPOT_ITEM
} hotspot_type;

typedef struct {
	hotspot_type type;
	hge_entity* parent;
	hge_vec3 interaction_location;
	char data[255];
	bool wireless;
} hotspot_component;

void system_hotspot_renderer(hge_entity* e, hge_transform* transform, hotspot_component* hotspot);
void system_item_renderer(hge_entity* entity, hge_transform* transform, hotspot_component* hotspot, hge_material* material);

#endif
