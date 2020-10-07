#include "Character.h"
#include "GameState.h"
#include "Scene.h"

bool noclip = false;

void CharacterNoclip(hge_transform* transform) {
  const float noclip_speed = 200.0f;
  if(hgeInputGetKey(HGE_KEY_W)) transform->position.y += noclip_speed*hgeDeltaTime();
  if(hgeInputGetKey(HGE_KEY_S)) transform->position.y -= noclip_speed*hgeDeltaTime();
  if(hgeInputGetKey(HGE_KEY_A)) transform->position.x -= noclip_speed*hgeDeltaTime();
  if(hgeInputGetKey(HGE_KEY_D)) transform->position.x += noclip_speed*hgeDeltaTime();
}

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

void CharacterWalking(character_component* character, hge_vec3* position, spritesheet_component* spritesheet) {
  float offset = character->speed/8;
  // Set Spritesheet to Walking Animation
  spritesheet->frame.y = 1;

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
    //character->current_hotspot->event();
    televoidRunScript(character->current_hotspot->script);
    character->current_hotspot = NULL;
  } else if(spritesheet->frame.x == 7) {
    character->state = CHARACTER_IDLE;
  }
}

void CharacterSystem(hge_entity* e, character_component* character, hge_transform* transform, spritesheet_component* spritesheet) {
  if(GetGameState() == GAME_PAUSE || GetGameState() == GAME_MINIGAME) return;

  /*hge_vec3 text_position = { -hgeWindowWidth()/2, -hgeWindowHeight()/2, 100 };
  hge_vec4 text_color = { 1, 1, 1, 1 };
  char my_text[255];
  snprintf( my_text, 255, "pos: (%d, %d)", (int)transform->position.x, (int)transform->position.y);
  televoidTextRenderSimple(my_text, false, text_position, 1, text_color);*/

  if(IsDebugMode()) {
    if(hgeInputGetKeyDown(HGE_KEY_V)) noclip = !noclip;

    if(noclip) {
      CharacterNoclip(transform);
      return;
    }
  }

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

	// DEBUG RENDER
  if(IsDebugMode()) {
    hge_vec3 max_z_overlay_position = character->destination;
    max_z_overlay_position.z = 100.0f;
    hge_vec3 loc_scale = {9, 9, 1};
	 hgeRenderSprite(hgeResourcesQueryShader("sprite_shader"), hgeResourcesQueryTexture("debug_pointer_texture"), max_z_overlay_position, loc_scale, 0.0f);
  }
}

void PlayerCharacterControlSystem(hge_entity* e, tag_component* playable, character_component* character) {
	bool RIGHT = hgeInputGetKey(HGE_KEY_D);
	bool LEFT = hgeInputGetKey(HGE_KEY_A);
	bool UP = hgeInputGetKey(HGE_KEY_W);
	bool DOWN = hgeInputGetKey(HGE_KEY_S);

  if(hgeInputGetKeyDown(HGE_KEY_ESCAPE)) {
    switch(GetGameState()) {
        case GAME_PLAY:
          SetGameState(GAME_PAUSE);
        break;
        case GAME_PAUSE:
          SetGameState(GAME_PLAY);
        break;
    }
  }

  if(GetGameState() != GAME_PLAY) return;

	hge_vec3 mouse_normalized = hgeNormalizedDeviceCoords(hgeInputGetMousePosition().x, hgeInputGetMousePosition().y);
	float mouse_x_normalized = mouse_normalized.x;
	float mouse_y_normalized = mouse_normalized.y;

	hge_entity* cam_entity = hgeQueryEntity(1, "ActiveCamera");
	hge_vec3* cam_pos = cam_entity->components[hgeQuery(cam_entity, "Position")].data;
  hge_camera* cam_camera = cam_entity->components[hgeQuery(cam_entity, "Camera")].data;

	float main_orth_cam_zoom = 1/cam_camera->fov;
	hge_vec3 transformed_mousepos = { cam_pos->x + (hgeInputGetMousePosition().x - hgeWindowWidth()/2)/main_orth_cam_zoom, cam_pos->y + (-hgeInputGetMousePosition().y+hgeWindowHeight()/2)/main_orth_cam_zoom, 0.0f};

	if(hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
		hotspot_component* intersected_hotspot = NULL;
		// Find All Hotspots
		hge_ecs_request found_hotspots = hgeECSRequest(1, "Hotspot");
		if(found_hotspots.NUM_ENTITIES > 0)
			for(int i = 0; i < found_hotspots.NUM_ENTITIES; i++) {
				hge_transform mouse_transform;
				mouse_transform.position = transformed_mousepos;
				mouse_transform.scale.x = 1;
				mouse_transform.scale.y = 1;
				hge_component transform_hge_component = found_hotspots.entities[i]->components[hgeQuery(found_hotspots.entities[i], "Transform")];
				hge_transform* transform = transform_hge_component.data;
				// Check If Our Cursor Is Intersecting Any Hotspots
				if(AABB(mouse_transform, *transform)) {
					hge_component hotspot_hge_component = found_hotspots.entities[i]->components[hgeQuery(found_hotspots.entities[i], "Hotspot")];
					intersected_hotspot = hotspot_hge_component.data;
				}
			}

		if(intersected_hotspot) {
			//printf("Clicked Hotspot!\n");
			CharacterHotspotClick(character, intersected_hotspot);
		} else {
			//printf("Clicked Nothing!\n");
			CharacterGroundClick(character, transformed_mousepos);
		}

	}

	// DEBUG RENDER
	if(IsDebugMode()) {
    hge_vec3 max_z_overlay_position = transformed_mousepos;
    max_z_overlay_position.z = 100.0f;
		hge_vec3 loc_scale = {9, 9, 1};
		hgeRenderSprite(hgeResourcesQueryShader("sprite_shader"), hgeResourcesQueryTexture("debug_pointer_texture"), max_z_overlay_position, loc_scale, 0.0f);
	}
}
