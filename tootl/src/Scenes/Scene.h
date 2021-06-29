#ifndef SCENE_H
#define SCENE_H

#include <HGE/HGE.h>

#include "Dialogue.h"
#include "Hotspot.h"
#include "Trigger.h"
#include "Prop.h"

#include "Character.h"
#include "MousePicker.h"
#include "Hotspot.h"
#include "GameState.h"
#include "Dialogue.h"
#include "SafeTrash.h"
#include "Inventory.h"
#include "Minigame.h"
#include "IMVPlayer.h"

#include "IMVPlayer.h"
#include "Floor.h"
#include "Floater.h"

hge_entity* televoidSceneGetEntity(const char* name);

void televoidSceneAddEntity(hge_entity* entity, const char* name);
void televoidSceneDestroyEntity(const char* name);
void televoidSceneDestroy();

void televoidLoadSceneForcePlayer(const char* scene_path, hge_vec3 position);
void televoidLoadScene(const char* scene_path);
void televoidSceneUpdate(bool skip);

void televoidAddBackground(hge_vec3 position, hge_vec3 scale, bool lit, const char* path);
void televoidAddDirLight(hge_vec3 direction, hge_vec3 ambient, hge_vec3 diffuse);
void televoidAddPointLight(hge_vec3 position, hge_vec3 ambient, hge_vec3 diffuse);

hge_transform* televoid_player_transform();
spritesheet_component* televoid_player_spritesheet();

hge_entity* televoidCreateIanPlayer(hge_vec3 position, bool face_left);
hge_entity* televoidCreatePlayerCamera(hge_vec3 position);
hge_entity* televoidCreateHotspot(hge_transform transform, hge_vec3 interaction_location, hotspot_type type, bool wireless, const char* data);
hge_entity* televoidCreateTrigger(hge_transform transform, unsigned int flags, const char* data);
void televoidAddMesh(hge_transform transform, const char* meshPath, hge_material material);
hge_entity* televoidCreateProp(const char* name, hge_transform prop_transform, hge_material prop_material);
hge_entity* televoidCreateDialogue(const char* file);
hge_entity* televoidIMVCreate(const char* path);

void SceneSave();

#endif
