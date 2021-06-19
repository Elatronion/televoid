#include "bowling.h"
#include "GameState.h"

typedef struct {
	bool can_throw;
	bool ball_is_returned;
	bool return_ball;
	float returning_timer;
	bool second_shot;
	int current_shot;
} component_bowling_game_manager;

typedef struct {
	void (*function)();
} component_hotspot3D;

typedef struct {
	hge_vec2 direction;
} component_stopwall;

component_bowling_game_manager* get_bowling_game_manager() {
	hge_entity* bowling_game_manager_entity = hgeQueryEntity(1, "bowling manager");
	if(!bowling_game_manager_entity) return NULL;
	component_bowling_game_manager* bowling_game_manager = bowling_game_manager_entity->components[hgeQuery(bowling_game_manager_entity, "bowling manager")].data;
	if(!bowling_game_manager) return NULL;
	return bowling_game_manager;
}

hge_entity* createMeshWithTransform(const char* mesh_name, const char* texture_name, hge_transform transform) {
	hge_entity* mesh_entity = hgeCreateEntity();

	hge_mesh mesh = hgeResourcesQueryMesh(mesh_name);
	hge_material material = {
		hgeResourcesQueryTexture(texture_name),
		hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
		true,
		hgeVec4(1, 1, 1, 1)
	};
	mesh.material = material;

	hgeAddComponent(mesh_entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
	hgeAddComponent(mesh_entity, hgeCreateComponent("bowling mesh", &mesh, sizeof(mesh)));

  return mesh_entity;
}

void spawn_pins() {
	for(int z = 0; z < 4; z++) {
		for(int x = 0; x <= z; x++) {
			hge_transform pin_transform = {
				hgeVec3(3 + x - z/2.f, 1, -100 - z),
				hgeVec3(0.5f, 0.5f, 0.5f),
				hgeQuaternion(0, 0, 0, 1)
			};
			hge_entity* entity_bowling_pin = createMeshWithTransform("Bowling Pin", "Bowling Pin", pin_transform);
			component_bowling_pond bowling_pond = {
				hgeVec3(0, 0, 0),
				false,
				true,
				false,
				3
			};
			hgeAddComponent(entity_bowling_pin, hgeCreateComponent("bowling pond", &bowling_pond, sizeof(bowling_pond)));
		}
	}
}

void system_bowling_manager(hge_entity* entity, component_bowling_game_manager* bowling_game_manager) {
	bowling_game_manager->can_throw = false;

	if(bowling_game_manager->returning_timer > 0) {
		bowling_game_manager->returning_timer -= hgeDeltaTime();
		if(bowling_game_manager->returning_timer <= 0.0f) {
			if(!bowling_game_manager->return_ball) {
				bowling_game_manager->return_ball = true;
				bowling_game_manager->returning_timer = 10;

				if(bowling_game_manager->second_shot) {
					bool no_pins = true;
					hge_ecs_request bowling_ponds_request = hgeECSRequest(2, "transform", "bowling pond");
					for(int i = 0; i < bowling_ponds_request.NUM_ENTITIES; i++) {
						hge_entity* bowling_pond_entity = bowling_ponds_request.entities[i];
						component_bowling_pond* bowling_pond = bowling_pond_entity->components[hgeQuery(bowling_pond_entity, "bowling pond")].data;

						if(bowling_pond->is_pin) {
							no_pins = false;
							hgeDestroyEntity(bowling_pond_entity);
						}
					}
					spawn_pins();

					if(no_pins) {
						televoidBoomboxPlaySFX("crowd ooh");
					}

					televoidBoomboxPlaySFX("crowd applause");
				}

				bowling_game_manager->current_shot++;
				printf("Current Shot Is %d\n", bowling_game_manager->current_shot);
				char devtalk_clip[20];
				sprintf(&devtalk_clip, "devtalk bowling %d", bowling_game_manager->current_shot);
				televoidBoomboxPlayDEVTALK(devtalk_clip);

				bowling_game_manager->second_shot = !bowling_game_manager->second_shot;

			} else {
				bowling_game_manager->ball_is_returned = true;
				bowling_game_manager->return_ball = false;
			}
		}
	}

	float percentage_returned = 0.0f;
	if(bowling_game_manager->return_ball) {
		percentage_returned = bowling_game_manager->returning_timer / 10.f;
	}

	if(bowling_game_manager->ball_is_returned || bowling_game_manager->return_ball) {
		hge_material material = {
			hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
			hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
			true,
			hgeVec4(1, 1, 1, 1)
		};
		float rest_z = -20.483383;
		hge_transform transform = {
			hgeVec3(-4.515505, 2, hgeMathFunc_lerp(rest_z, -112.825752, percentage_returned)),
			hgeVec3(1, 1, 1),
			hgeQuaternion(0, 0, 0, 1)
		};
		hgeRenderMesh(hgeResourcesQueryShader("basic shading"), material, hgeResourcesQueryMesh("Bowling Ball"), transform);
	}
}

hge_entity* createMesh(const char* mesh_name, const char* texture_name) {
	hge_transform transform = {
		hgeVec3(0, 0, -10),
		hgeVec3(1, 1, 1),
		hgeQuaternionInitRotation(hgeVec3(0, 1, 0), hgeRadians(-90))
	};

	return createMeshWithTransform(mesh_name, texture_name, transform);
}

hge_entity* createCamera(hge_vec3 position) {
  hge_transform camera_transform;
  camera_transform.position = position;
  camera_transform.scale = hgeVec3(6, 6, 6);
  camera_transform.rotation = hgeQuaternionInitRotation(hgeVec3(0, 1, 0), hgeRadians(180));
  hge_entity* camera_entity = hgeCreateCamera(
      camera_transform,
      hgeVec2(hgeWindowWidth(), hgeWindowHeight()),
      90,
      1, 500.f,
      NULL
  );
  tag_component activecam_tag;
  hgeAddComponent(camera_entity, hgeCreateComponent("active", &activecam_tag, sizeof(activecam_tag)));
  /*
  tag_component freemove_tag;
  hgeAddComponent(camera_entity, hgeCreateComponent("freemove", &activecam_tag, sizeof(activecam_tag)));
  */
  return camera_entity;
}

void system_bowling_mesh_renderer(hge_entity* entity, hge_transform* transform, hge_mesh* mesh) {
	hgeRenderMesh(hgeResourcesQueryShader("basic shading"), mesh->material, *mesh, *transform);
}

#define STEP_SOUND_TIME 0.5
float step_sound_timer = STEP_SOUND_TIME;
int last_mouse_x = 0;
int last_mouse_y = 0;
void system_fps_controller(hge_entity* entity, hge_transform* transform, component_fps_controller* fps_controller) {
	if(televoidGameState() == GAME_DIALOGUE) return;
	float speed = 8.0f;
  float mouse_sensitivity = 2.0f;

  hge_vec3 mouse_position = hgeInputGetMousePosition();
  int mouse_x = mouse_position.x;
  int mouse_y = mouse_position.y;
  int delta_mouse_x = mouse_x - last_mouse_x;
  int delta_mouse_y = -(mouse_y - last_mouse_y);
  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;

  fps_controller->yaw += delta_mouse_x * mouse_sensitivity * hgeDeltaTime();
  fps_controller->pitch += delta_mouse_y * mouse_sensitivity * hgeDeltaTime();
  if(fps_controller->pitch > 90) {
    fps_controller->pitch = 90;
  } else if(fps_controller->pitch < -90) {
    fps_controller->pitch = -90;
  }

  transform->rotation = hgeQuaternionInitRotation(hgeVec3(0, 1, 0), hgeRadians(fps_controller->yaw));
	transform->rotation = hgeQuaternionMul(
		transform->rotation,
		hgeQuaternionInitRotation(
			hgeQuaternionGetRight(transform->rotation),
			hgeRadians(-fps_controller->pitch)
		)
	);

  hge_vec2 direction = hgeVec2(0, 0);
  float move_angle = fps_controller->yaw - 180.0f;

	if(hgeInputGetKey(HGE_KEY_W)) {
    direction.x += sin(hgeRadians(move_angle));
    direction.y += cos(hgeRadians(move_angle));
	}
	if(hgeInputGetKey(HGE_KEY_S)) {
    direction.x += sin(hgeRadians(move_angle+180.0f));
    direction.y += cos(hgeRadians(move_angle+180.0f));
	}
	if(hgeInputGetKey(HGE_KEY_A)) {
    direction.x += sin(hgeRadians(move_angle-90.0f));
    direction.y += cos(hgeRadians(move_angle-90.0f));
	}
	if(hgeInputGetKey(HGE_KEY_D)) {
    direction.x += sin(hgeRadians(move_angle+90.0f));
    direction.y += cos(hgeRadians(move_angle+90.0f));
  }

  direction = hgeVec2Normalize(direction);

  float desired_height = 6;
  if(direction.x != 0 && direction.y != 0) {
    step_sound_timer -= hgeDeltaTime();
    if(step_sound_timer <= 0) {
      televoidBoomboxPlaySFX("step1");
      step_sound_timer = STEP_SOUND_TIME;
    }
    desired_height = 6 + cos(hgeRuntime()*12)/4;
  }
  transform->position.y += (desired_height - transform->position.y) * 5.f * hgeDeltaTime();

  transform->position.x += direction.x * speed * hgeDeltaTime();
  transform->position.z -= direction.y * speed * hgeDeltaTime();

	component_bowling_game_manager* bowling_game_manager = get_bowling_game_manager();

  if(bowling_game_manager->can_throw && InventoryHasItem(televoidGetItemID("bowling ball")) && hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
		InventoryRemoveItem(televoidGetItemID("bowling ball"));
		bowling_game_manager->returning_timer = 10;
    hge_transform ball_transform = {
      transform->position,
      hgeVec3(1, 1, 1),
      hgeQuaternion(0, 0, 0, 1)
    };
    hge_entity* entity_bowling_ball = createMeshWithTransform("Bowling Ball", "HGE DEFAULT NORMAL", ball_transform);
    float throw_speed = 0.3;
    hge_vec3 forward = hgeQuaternionGetFoward(transform->rotation);
    component_bowling_pond bowling_pond = {
      hgeVec3(forward.x * throw_speed, throw_speed + forward.y * throw_speed, forward.z * throw_speed),
      true,
			false,
			false,
			10
    };
    hgeAddComponent(entity_bowling_ball, hgeCreateComponent("bowling pond", &bowling_pond, sizeof(bowling_pond)));
  }

	if(hgeInputGetKeyDown(HGE_KEY_P)) {
		printf("Position (%f, %f, %f)\n", transform->position.x, transform->position.y, transform->position.z);
	}
  if(hgeInputGetKeyDown(HGE_KEY_Q)) {
    hgeWindowCaptureCursor(false);
  }
	if(hgeInputGetKeyDown(HGE_KEY_C)) {
		hgeWindowCaptureCursor(true);
	}
}

bool AABB3D(hge_transform A, hge_transform B) {
	float Ax = A.position.x - A.scale.x/2;
	float Ay = A.position.y + A.scale.y/2;
	float Az = A.position.z - A.scale.z/2;
	float AX = A.position.x + A.scale.x/2;
	float AY = A.position.y - A.scale.y/2;
	float AZ = A.position.z + A.scale.z/2;

	float Bx = B.position.x - B.scale.x/2;
	float By = B.position.y + B.scale.y/2;
	float Bz = B.position.z - B.scale.z/2;
	float BX = B.position.x + B.scale.x/2;
	float BY = B.position.y - B.scale.y/2;
	float BZ = B.position.z + B.scale.z/2;

	return
		!(
			AX < Bx || BX < Ax ||
			AY > By || BY > Ay ||
			AZ < Bz || BZ < Az
		);
}

void system_bowling_pond(hge_entity* entity, hge_transform* transform, component_bowling_pond* bowling_pond) {
  // Update
	float right_side = 8.6f;
	float left_side = -2.6f;

  float gravity = -1.5f;
  bowling_pond->velocity.y += gravity * hgeDeltaTime();

  if(transform->position.y < 1) {
		if(transform->position.x < right_side && transform->position.x > left_side) {
			televoidBoomboxPlaySFX("ground hit");
		}
    bowling_pond->velocity.y = -bowling_pond->velocity.y / 2.f;
		if(bowling_pond->is_pin) {
			bowling_pond->velocity.x = bowling_pond->velocity.x / 1.5f;
			bowling_pond->velocity.z = bowling_pond->velocity.z / 1.5f;
		}

    transform->position.y = 1;
    if(fabs(bowling_pond->velocity.y) < 0.075) {
        bowling_pond->bounce = false;
    }
  }
  if(bowling_pond->bounce)
  transform->position.y += bowling_pond->velocity.y;
  else {
    if(bowling_pond->velocity.y > 0.075) {
      bowling_pond->bounce = true;
    }
  }

	if(bowling_pond->is_pin) {
		if(bowling_pond->is_hit) {
			transform->rotation = hgeQuaternionInitRotation(hgeVec3(1, 0, 0), -hgeRuntime()*10 + transform->position.z + transform->position.x);
			bowling_pond->lifetime -= hgeDeltaTime();
		}
	} else {
		bowling_pond->lifetime -= hgeDeltaTime();
	}

	if(bowling_pond->lifetime <= 0) {
		hgeDestroyEntity(entity);
		return;
	}


  transform->position.x += bowling_pond->velocity.x;
  transform->position.z += bowling_pond->velocity.z;

	// Collision
	hge_ecs_request bowling_ponds_request = hgeECSRequest(2, "transform", "bowling pond");
	for(int i = 0; i < bowling_ponds_request.NUM_ENTITIES; i++) {
		hge_entity* bowling_pond_entity = bowling_ponds_request.entities[i];
		if(entity == bowling_pond_entity) continue;
		hge_transform* bowling_pond_transform = bowling_pond_entity->components[hgeQuery(bowling_pond_entity, "transform")].data;
		component_bowling_pond* bowling_pond2 = bowling_pond_entity->components[hgeQuery(bowling_pond_entity, "bowling pond")].data;

		if (AABB3D(*transform, *bowling_pond_transform)) {

			hge_vec3 velocity_a = bowling_pond->velocity;
			hge_vec3 velocity_b = bowling_pond2->velocity;

			if(bowling_pond->is_pin && !bowling_pond2->is_pin) {
				bowling_pond->velocity.x = velocity_b.x/2.f;
				bowling_pond->velocity.y = 0.5f;
				bowling_pond->velocity.z = velocity_b.z/2.f;
				bowling_pond->is_hit = true;
				televoidBoomboxPlaySFX("step1");
			}
			/*
			bowling_pond->velocity.x = velocity_a.x/2.f + velocity_b.x/2.f;
			//bowling_pond->velocity.y = velocity_a.y/2.f + velocity_b.y/2.f;
			bowling_pond->velocity.z = velocity_a.z/2.f + velocity_b.z/2.f;

			bowling_pond2->velocity.x = velocity_a.x/2.f + velocity_b.x/2.f;
			//bowling_pond2->velocity.y = velocity_a.y/2.f + velocity_b.y/2.f;
			bowling_pond2->velocity.z = velocity_a.z/2.f + velocity_b.z/2.f;
			*/

			/*
			float speed_a = hgeVec3Length(velocity_a);
			float speed_b = hgeVec3Length(velocity_b);
			if(speed_a > speed_b) {
				bowling_pond->velocity.y = 0.25f;

				bowling_pond->velocity.x = velocity_a.x * 1.25f;
				bowling_pond->velocity.z = velocity_a.z * 1.25f;
			}
			else {
				bowling_pond2->velocity.y = 0.25f;

				bowling_pond2->velocity.x = velocity_a.x * 1.25f;
				bowling_pond2->velocity.z = velocity_a.z * 1.25f;
			}
			*/
		}
	}

	if(!bowling_pond->is_pin) {
		float in_floor = 0;
		if(transform->position.x >= right_side) {
			transform->position.x = right_side;
			transform->position.y = in_floor;
		} else if(transform->position.x <= left_side) {
			transform->position.x = left_side;
			transform->position.y = in_floor;
		}
	}

}

void system_hotspot3D(hge_entity* entity, hge_transform* transform, component_hotspot3D* hotspot3D) {
	hge_entity* player_entity = hgeQueryEntity(2, "transform", "fps controller");
	if(!player_entity) return;
	hge_transform* player_transform = player_entity->components[hgeQuery(player_entity, "transform")].data;
	if(!player_transform) return;

	if(AABB3D(*transform, *player_transform)) {
		hotspot3D->function();
	}
}

void system_stopwall(hge_entity* entity, hge_transform* transform, component_stopwall* stopwall) {
	hge_entity* player_entity = hgeQueryEntity(2, "transform", "fps controller");
	if(!player_entity) return;
	hge_transform* player_transform = player_entity->components[hgeQuery(player_entity, "transform")].data;
	if(!player_transform) return;

	if(AABB3D(*transform, *player_transform)) {
		if(stopwall->direction.x != 0)
		player_transform->position.x = transform->position.x + (transform->scale.x/2.f + player_transform->scale.x/2.f)* stopwall->direction.x;
		if(stopwall->direction.y != 0)
		player_transform->position.z = transform->position.z + (transform->scale.z/2.f + player_transform->scale.z/2.f)* stopwall->direction.y;
	}
}

void create_hotspot3D(hge_transform transform, void (*function)()) {
	hge_entity* entity_hotspot3D = hgeCreateEntity();
	hgeAddComponent(entity_hotspot3D, hgeCreateComponent("transform", &transform, sizeof(transform)));
	//hge_entity* entity_hotspot3D = createMeshWithTransform("cube", "debug_hotspot_texture", transform);
	component_hotspot3D hotspot3D = {
		function
	};
	hgeAddComponent(entity_hotspot3D, hgeCreateComponent("hotspot3D", &hotspot3D, sizeof(hotspot3D)));
}

void create_stopwall(hge_transform transform, hge_vec2 direction) {
	//direction = hgeVec2Normalize(direction);
	hge_entity* entity_stopwall = hgeCreateEntity();
	hgeAddComponent(entity_stopwall, hgeCreateComponent("transform", &transform, sizeof(transform)));
	//hge_entity* entity_stopwall = createMeshWithTransform("cube", "debug_hotspot_texture", transform);
	component_stopwall stopwall = {
		direction
	};
	hgeAddComponent(entity_stopwall, hgeCreateComponent("stopwall", &stopwall, sizeof(stopwall)));
}

void init_bowling_resources() {
  // Load Resources
  hgeResourcesLoadShader("res/shader.vs", NULL, "res/shaders/basic-shading.fs", "basic shading");

	hgeResourcesLoadMesh("res/meshes/cube.obj", "cube");

  hgeResourcesLoadTexture("res/HGE/DEFAULT NORMAL.png", "HGE DEFAULT NORMAL");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-bricks.obj", "Bowling Bricks");
  //hgeResourcesLoadTexture("brick-wall-free-textures-01.jpg", "bricks");
  hgeResourcesLoadTexture("res/meshes/bowling/bricks.png", "bricks");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-metal.obj", "Bowling Metal");
  hgeResourcesLoadTexture("res/meshes/bowling/metal.jpg", "metal");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-wood.obj", "Bowling Wood");
  //hgeResourcesLoadTexture("bowling wood.png", "bowling wood");
  hgeResourcesLoadTexture("res/meshes/bowling/bowling wood pixel.png", "bowling wood");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-light.obj", "Bowling Light");
  //hgeResourcesLoadTexture("bowling light.png", "light");
  hgeResourcesLoadTexture("res/meshes/bowling/detectable light.png", "light");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-whitelight.obj", "Bowling White Light");
  //hgeResourcesLoadTexture("bowling whitelight.png", "white light");
  hgeResourcesLoadTexture("res/meshes/bowling/detectable whitelight.png", "white light");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-dark-brown.obj", "Bowling Dark Brown");
  hgeResourcesLoadTexture("res/meshes/bowling/dark-brown.png", "dark brown");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-light-brown.obj", "Bowling Light Brown");
  hgeResourcesLoadTexture("res/meshes/bowling/light-brown.png", "light brown");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-dark-floor.obj", "Bowling Dark Floor");
  hgeResourcesLoadTexture("res/meshes/bowling/wood.png", "wood");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-couches.obj", "Bowling Couches");
  //hgeResourcesLoadTexture("red leather.jpg", "red leather");
  hgeResourcesLoadTexture("res/meshes/bowling/red leather.png", "red leather");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-table-top.obj", "Bowling Table Top");
  hgeResourcesLoadTexture("res/meshes/bowling/table.png", "table top");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-plant.obj", "Bowling Plant");
  hgeResourcesLoadTexture("res/meshes/bowling/plant.png", "plant");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-blue-metal.obj", "Bowling Blue Metal");
  hgeResourcesLoadTexture("res/meshes/bowling/blue metal.jpg", "blue metal");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-red-wood.obj", "Bowling Red Wood");
  hgeResourcesLoadTexture("res/meshes/bowling/red wood.png", "red wood");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling-exit.obj", "Bowling Exit");
  hgeResourcesLoadTexture("res/meshes/bowling/exit.png", "exit");

  hgeResourcesLoadMesh("res/meshes/bowling/bowling ball.obj", "Bowling Ball");
  //hgeResourcesLoadTexture("res/meshes/bowling/exit.png", "exit");
  hgeResourcesLoadMesh("res/meshes/bowling/bowling pin.obj", "Bowling Pin");
  hgeResourcesLoadTexture("res/meshes/bowling/bowling pin.png", "Bowling Pin");

  // Load Systems
  hgeAddSystem(system_bowling_mesh_renderer, 2, "transform", "bowling mesh");
  hgeAddSystem(system_bowling_pond, 2, "transform", "bowling pond");
  hgeAddSystem(system_fps_controller, 2, "transform", "fps controller");
	hgeAddSystem(system_stopwall, 2, "transform", "stopwall");
	hgeAddSystem(system_bowling_manager, 1, "bowling manager");
	hgeAddSystem(system_hotspot3D, 2, "transform", "hotspot3D");
}

void throw_detector() {
	component_bowling_game_manager* bowling_game_manager = get_bowling_game_manager();
	bowling_game_manager->can_throw = true;
}

void ball_pickup_detector() {
	component_bowling_game_manager* bowling_game_manager = get_bowling_game_manager();
	if(bowling_game_manager->ball_is_returned && hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
		bowling_game_manager->ball_is_returned = false;
		InventoryAddItem(televoidGetItemID("bowling ball"));
	}
}

float talk_timer = 0.0f;
void plant_talk () {
	if(televoidGameState() == GAME_DIALOGUE) return;
	talk_timer -= hgeDeltaTime();
	if(hgeInputGetMouseDown(HGE_MOUSE_LEFT) && talk_timer <= 0.0f) {
		talk_timer = 0.5f;
		televoidCreateDialogue("res/dialogue/Bowling - Plant.csv");
	}
}

void create_hotspots() {

	hge_transform bowling_alley_play = {
		hgeVec3(3, 0, -25),
		hgeVec3(6, 10, 5),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_hotspot3D(bowling_alley_play, throw_detector);


	hge_transform bowling_ball_pickup = {
		hgeVec3(-2, 0, -22),
		hgeVec3(1, 10, 3),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_hotspot3D(bowling_ball_pickup, ball_pickup_detector);

	hge_transform plant_hotspot = {
		hgeVec3(-22.341837, 5.999156, -14.331349),
		hgeVec3(1, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_hotspot3D(plant_hotspot, plant_talk);
}

void create_walls() {
	hge_transform bowling_alley_invisible_wall = {
		hgeVec3(-5, 0, -30),
		hgeVec3(21, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(bowling_alley_invisible_wall, hgeVec2(0, 1));

	hge_transform wall_right = {
		hgeVec3(11, 0, -60),
		hgeVec3(1, 10, 100),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(wall_right, hgeVec2(-1, 0));

	hge_transform wall_left = {
		hgeVec3(-25.2f, 0, -60),
		hgeVec3(1, 10, 100),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(wall_left, hgeVec2(1, 0));

	hge_transform wall_hall_right = {
		hgeVec3(-16, 0, -80),
		hgeVec3(1, 10, 100),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(wall_hall_right, hgeVec2(-1, 0));

	hge_transform wall_hall_back = {
		hgeVec3(-16, 0, -111.5f),
		hgeVec3(100, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(wall_hall_back, hgeVec2(0, 1));

	hge_transform ball_return_thing_front = {
		hgeVec3(-4.6f, 0, -18),
		hgeVec3(1, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(ball_return_thing_front, hgeVec2(0, 1));

	hge_transform ball_return_thing_right = {
		hgeVec3(-4.f, 0, -23),
		hgeVec3(1, 10, 7),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(ball_return_thing_right, hgeVec2(1, 0));

	hge_transform ball_return_thing_left = {
		hgeVec3(-5.f, 0, -23),
		hgeVec3(1, 10, 7),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(ball_return_thing_left, hgeVec2(-1, 0));

	hge_transform plant = {
		hgeVec3(-21.799999, 0, -11.275737),
		hgeVec3(1, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(plant, hgeVec2(0, -1));

	hge_transform couch_left_plant_hall = {
		hgeVec3(-18.3, 0, -12),
		hgeVec3(1, 10, 7),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_left_plant_hall, hgeVec2(-1, 0));

	hge_transform couch_left_plant_hall_z = {
		hgeVec3(-18.3, 0, -17),
		hgeVec3(0.8, 10, 0.8),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_left_plant_hall_z, hgeVec2(0, -1));

	hge_transform couch_left_plant_hall_otherside = {
		hgeVec3(-17, 0, -12),
		hgeVec3(1, 10, 7),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_left_plant_hall_otherside, hgeVec2(1, 0));

	hge_transform couch_left_couch_door_z = {
		hgeVec3(-13.500000, 0, -8.235778),
		hgeVec3(5.5f, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_left_couch_door_z, hgeVec2(0, -1));


	hge_transform couch_right_door_z = {
		hgeVec3(4, 0, -7.513974),
		hgeVec3(5, 10, 1),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_right_door_z, hgeVec2(0, -1));

	hge_transform couch_right_door_x = {
		hgeVec3(8.827655, 0, -11.013974),
		hgeVec3(1, 10, 9),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_right_door_x, hgeVec2(-1, 0));

	hge_transform couch_right_door_z_tip = {
		hgeVec3(8.827655, 0, -17),
		hgeVec3(0.8, 10, 0.8),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(couch_right_door_z_tip, hgeVec2(0, -1));

	hge_transform right_pillar = {
		hgeVec3(5.413734, 0, -3),
		hgeVec3(10, 10, 8),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(right_pillar, hgeVec2(-1, 0));

	hge_transform left_pillar = {
		hgeVec3(-15.5f, 0, -2.809897),
		hgeVec3(10, 10, 8),
		hgeQuaternion(0, 0, 0, 1)
	};
	create_stopwall(left_pillar, hgeVec2(1, 0));
}

void create_bowling_scene() {
  // Capture Cursor
  hgeWindowCaptureCursor(true);

  // Create Scene
  hge_entity* camera_entity = createCamera(hgeVec3(-4.835251, 6, -2.632065));
  component_fps_controller fps_controller = {
    180.0f,
    0.0f
  };
  hgeAddComponent(camera_entity, hgeCreateComponent("fps controller", &fps_controller, sizeof(fps_controller)));

  createMesh("Bowling Bricks", "bricks");
  createMesh("Bowling Metal", "metal");
  createMesh("Bowling Wood", "bowling wood");
  createMesh("Bowling Light", "light");
  createMesh("Bowling White Light", "white light");
  createMesh("Bowling Dark Brown", "dark brown");
  createMesh("Bowling Light Brown", "light brown");
  createMesh("Bowling Dark Floor", "wood");
  createMesh("Bowling Couches", "red leather");
  createMesh("Bowling Table Top", "table top");
  createMesh("Bowling Plant", "plant");
  createMesh("Bowling Blue Metal", "blue metal");
  createMesh("Bowling Red Wood", "red wood");
  createMesh("Bowling Exit", "exit");

	create_walls();
	create_hotspots();

	hge_entity* entity_bowling_game_manager = hgeCreateEntity();
	component_bowling_game_manager bowling_game_manager = {
		false, true, false, 0.0f, false, 0
	};
	hgeAddComponent(entity_bowling_game_manager, hgeCreateComponent("bowling manager", &bowling_game_manager, sizeof(bowling_game_manager)));


	spawn_pins();
}
