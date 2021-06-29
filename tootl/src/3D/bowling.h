#ifndef BOWLING_H
#define BOWLING_H
#include <HGE/HGE.h>

typedef struct {
	float yaw;
	float pitch;

	hge_vec3 velocity;
} component_fps_controller;

typedef struct {
	hge_vec3 velocity;
  bool bounce;
	bool is_pin;
	bool is_hit;
	float lifetime;
} component_bowling_pond;

void system_fps_controller(hge_entity* entity, hge_transform* transform, component_fps_controller* fps_controller);
void system_bowling_mesh_renderer(hge_entity* entity, hge_transform* transform, hge_mesh* mesh);

void init_bowling_resources();
void create_bowling_scene();

#endif
