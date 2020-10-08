#ifndef SCENE_H
#define SCENE_H
#include <HGE/HGE_Core.h>
#include "Hotspot.h"

typedef struct {
  char previous_scene_that_activates[255];
  bool face_left;
  bool fadein_on_start;
  float fade_speed;
} player_start;

typedef struct {
  char action_script[255];
  bool just_entered;
} trigger; // Detects Player (ian)

typedef struct {
  // lol wut?
} scene_logic;

// Systems
void TriggerSystem(hge_entity* entity, hge_transform* transform, trigger* c_trigger);

void system_scenelogic(hge_entity* entity, scene_logic* scene_l);

// Actions
void televoidSceneLoad(const char* scene_path);

// Prefabs
void televoidAddDecoration(hge_vec3 position, hge_vec3 scale, const char* sprite);

hotspot_component* televoidAddHotspot();
void televoidSceneAddTrigger(hge_transform transform);
void televoidAddIanPlayer(hge_vec3 position, bool face_left);

#endif
