#ifndef SCENE_H
#define SCENE_H
#include <HGE/HGE_Core.h>
#include "Hotspot.h"
#include "Inventory.h"

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

typedef struct {
  hge_material material;
  bool lit;
} background;

// Systems
void TriggerSystem(hge_entity* entity, hge_transform* transform, trigger* c_trigger);

void system_scenelogic(hge_entity* entity, scene_logic* scene_l);

// Actions
void televoidSceneLoad(const char* scene_path);

// Prefabs
void televoidAddBackground(hge_vec3 position, hge_vec3 scale, bool lit, const char* sprite);

void televoidAddItem(hge_vec3 position, hge_vec3 scale, const char* item_name);

void televoidAddDirLight(hge_vec3 direction, hge_vec3 ambient, hge_vec3 diffuse);
void televoidAddPointLight(hge_vec3 position, hge_vec3 ambient, hge_vec3 diffuse);

hotspot_component* televoidAddHotspot();
void televoidSceneAddTrigger(hge_transform transform);
void televoidAddIanPlayer(hge_vec3 position, bool face_left);

#endif
