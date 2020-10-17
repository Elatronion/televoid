#ifndef CHARACTER_H
#define CHARACTER_H

#include <HGE/HGE_Core.h>
#include "Hotspot.h"
#include "Inventory.h"

typedef enum {
	CHARACTER_IDLE, CHARACTER_WALKING, CHARACTER_INTERACTING
} character_state;

typedef struct {
	character_state state;
	hge_vec3 destination;
	float speed;
  hotspot_component* current_hotspot;
	item_component* current_item;
} character_component;

void CharacterGroundClick(character_component* character, hge_vec3 position);
void CharacterHotspotClick(character_component* character, hotspot_component* hotspot);

void CharacterSystem(hge_entity* e, character_component* character, hge_transform* transform, spritesheet_component* spritesheet);
void PlayerCharacterControlSystem(hge_entity* e, tag_component* playable, character_component* character);

#endif
