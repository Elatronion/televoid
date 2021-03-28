#ifndef SCENE_H
#define SCENE_H
#include <HGE/HGE_Core.h>
#include <HGE/HGE_ResourceManager.h>

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

hge_entity* televoidSceneGetEntity(const char* name);

void televoidSceneAddEntity(hge_entity* entity, const char* name);
void televoidSceneDestroy();

void televoidLoadScene(const char* scene_path);
void televoidSceneUpdate();

hge_transform* televoid_player_transform();

hge_entity* televoidCreateIanPlayer(hge_vec3 position, bool face_left);
hge_entity* televoidCreatePlayerCamera(hge_vec3 position);
hge_entity* televoidCreateHotspot(hge_transform transform, hge_vec3 interaction_location, hotspot_type type, const char* data);
hge_entity* televoidCreateTrigger(hge_transform transform, unsigned int flags, const char* data);
void televoidAddMesh(hge_transform transform, const char* meshPath, hge_material material);
void televoidCreateProp(const char* name, hge_transform prop_transform, hge_material prop_material);
hge_entity* televoidCreateDialogue(const char* file);
hge_entity* televoidIMVCreate(const char* path);

#endif
