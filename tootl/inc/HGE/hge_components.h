#ifndef HGE_COMPONENTS_H
#define HGE_COMPONENTS_H

#include "HGE_Core.h"
#include "HGE_RenderingEngine.h"

typedef struct {} tag_component;

typedef struct {
	float pitch;
	float yaw;
	float roll;
} orientation_component;

typedef struct {
	hge_vec3* target_pos;
	float speed;
	bool lock_x, lock_y, lock_z;
} follow_component;

typedef struct {
	float speed;
} freemove_component;

// For Sprite sheets
typedef struct {
	float time;
	int FPS;
	hge_vec2 resolution;
	hge_vec2 frame;
	int num_frames;
	bool flipped;
	bool playing;
	hge_material spritesheet_material;
} spritesheet_component;

void CameraSystem(hge_entity* entity, hge_camera* camera, hge_vec3* position, orientation_component* orientation);
void FollowTarget(hge_entity* entity, hge_vec3* position, follow_component* follow);
void FreeCam(hge_entity* entity, hge_camera* camera, freemove_component* freemove, hge_vec3* position);

void SpriteRenderingSystem(hge_entity* entity, hge_transform* transform, hge_texture* sprite);

void SpriteSheetSystem(hge_entity* entity, hge_transform* transform, spritesheet_component* spritesheet);

void hgeAddBaseSystems();


#endif
