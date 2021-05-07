#include "Character.h"
//#include "GameState.h"
//#include "Scene.h"
#include <HGE/HGE_FileUtility.h>
#include <HGE/HGE_AudioSample.h>
#include <HGE/HGE_AudioSource.h>
#include <HGE/HGE_Random.h>
#include "Boombox.h"
#include "MousePicker.h"
#include "GameState.h"

void CharacterGroundClick(character_component* character, hge_vec3 position) {
  if(character->state == CHARACTER_INTERACTING) return;

  character->current_hotspot = NULL;
  character->destination = position;
}

void CharacterHotspotClick(character_component* character, hotspot_component* hotspot) {
  if(character->state == CHARACTER_INTERACTING) return;

  character->current_hotspot = hotspot;
  character->destination = hotspot->interaction_location;
}

// Character States
void CharacterIdle(character_component* character, hge_vec3* position, spritesheet_component* spritesheet) {
  float offset = character->speed/8;
  // Set Spritesheet to Idle Animation
  spritesheet->frame.y = 0;

  if(position->x >= character->destination.x + offset || position->x <= character->destination.x - offset) {
    character->state = CHARACTER_WALKING;
  } else if(character->current_hotspot) {
    character->state = CHARACTER_INTERACTING;
    spritesheet->frame.x = 0;
  }
}

bool just_steped = false;
void CharacterWalking(character_component* character, hge_vec3* position, spritesheet_component* spritesheet) {
  float offset = character->speed/8;
  // Set Spritesheet to Walking Animation
  spritesheet->frame.y = 1;

	// Feet contact frames
	if(!just_steped && (spritesheet->frame.x == 1 || spritesheet->frame.x == 5)) {
		int step_n = hgeRandomInt(1, 4);
		char step_resource_name[6];
		sprintf(step_resource_name, "step%d", step_n);

		televoidBoomboxPlaySFX(step_resource_name);
		just_steped = true;
	} else {
		if(just_steped && spritesheet->frame.x != 1 && spritesheet->frame.x != 5) {
			just_steped = false;
		}
	}

  if(position->x >= character->destination.x + offset) {
    position->x -= character->speed * hgeDeltaTime();
    spritesheet->flipped = true;
  } else if(position->x <= character->destination.x - offset) {
    position->x += character->speed * hgeDeltaTime();
    spritesheet->flipped = false;
  } else {
    if(character->current_hotspot) {
      character->state = CHARACTER_INTERACTING;
      spritesheet->frame.x = 0;
    } else character->state = CHARACTER_IDLE;
  }
}

void CharacterInteracting(character_component* character, spritesheet_component* spritesheet) {
  // Set Spritesheet to Interacting Animation
  spritesheet->frame.y = 2;

  if (character->current_hotspot && spritesheet->frame.x == 2) {

    televoidHotspotExecute(character->current_hotspot);

    character->current_hotspot = NULL;
  } else if(spritesheet->frame.x == 7) {
    character->state = CHARACTER_IDLE;
  }
}

void CharacterSystem(hge_entity* e, character_component* character, hge_transform* transform, spritesheet_component* spritesheet) {
  if(televoidGameState() == GAME_PAUSE) return;
	switch(character->state) {
		case CHARACTER_IDLE:
			CharacterIdle(character, &transform->position, spritesheet);
			break;
		case CHARACTER_WALKING:
      CharacterWalking(character, &transform->position, spritesheet);
			break;
    case CHARACTER_INTERACTING:
      CharacterInteracting(character, spritesheet);
      break;
	}
}


void PlayerCharacterControlSystem(hge_entity* e, tag_component* playable, character_component* character) {
  if(hgeInputGetKeyDown(HGE_KEY_ESCAPE)) {
    if(televoidGameState() == GAME_PLAY) {
      televoidBoomboxPlaySFX("Pause");
      televoidSetGameState(GAME_PAUSE);
    }
    else if(televoidGameState() == GAME_PAUSE) {
      televoidBoomboxPlaySFX("Resume");
      televoidSetGameState(GAME_PLAY);
    }
    return;
  }

  if(televoidGameState() != GAME_PLAY) return;

  if(hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
    hge_vec3 mousepicker_raycast = mousePickerRaycast();

    hotspot_component* intersected_hotspot = NULL;
    // Find All Hotspots
		hge_ecs_request found_hotspots = hgeECSRequest(1, "hotspot");
		if(found_hotspots.NUM_ENTITIES > 0)
			for(int i = 0; i < found_hotspots.NUM_ENTITIES; i++) {
				hge_transform mouse_transform;
				mouse_transform.position = mousepicker_raycast;
				mouse_transform.scale.x = 1;
				mouse_transform.scale.y = 1;
				hge_component transform_hge_component = found_hotspots.entities[i]->components[hgeQuery(found_hotspots.entities[i], "transform")];
				hge_transform* transform = transform_hge_component.data;
				// Check If Our Cursor Is Intersecting Any Hotspots
				if(AABB(mouse_transform, *transform)) {
					hge_component hotspot_hge_component = found_hotspots.entities[i]->components[hgeQuery(found_hotspots.entities[i], "hotspot")];
					intersected_hotspot = hotspot_hge_component.data;
				}
			}

		if(intersected_hotspot) {
			CharacterHotspotClick(character, intersected_hotspot);
		} else {
			CharacterGroundClick(character, mousepicker_raycast);
		}
  }

}
