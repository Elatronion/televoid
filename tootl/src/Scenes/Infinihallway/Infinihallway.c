#include "Infinihallway.h"
#include "Scene.h"
#include "fader_sprite.h"

#define SECTION_WIDTH   54
#define SECTION_HEIGHT  58

#define RENDER_DISTANCE 5

void infinihallway_create_door_hotspot(int index, const char* script) {
	hge_vec3 hallway_scale = { SECTION_WIDTH, SECTION_HEIGHT, 0 };
	hge_vec3 hallway_position = { hallway_scale.x / 2 + index * hallway_scale.x, 0, -1.f };
	hge_vec3 hotspot_scale = { 15, 29, 0 };
	hge_vec3 hotspot_position = { hallway_position.x + hotspot_scale.x / 2 - 25, -hotspot_scale.y / 2 + 13, 0.f };
	hge_transform hotspot_transform = {
		hotspot_position,
		hotspot_scale,
		hgeQuaternion(0, 0, 0, 1)
	};

	televoidCreateHotspot(
		hotspot_transform,
		hotspot_transform.position,
		HOTSPOT_INTERACTABLE_SCRIPT,
		false,
		script);
}

void render_section(infinihallway_component* infinihallway, int index, hge_texture section_texture) {
	hge_shader shader = hgeResourcesQueryShader("basic");
	hge_texture default_normal = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");

	hge_material hallway_section_material = {
		section_texture,
		default_normal,
		false,
		hgeVec4(1, 1, 1, 1)
	};

	hge_vec3 hallway_scale = { SECTION_WIDTH, SECTION_HEIGHT, 0 };
	hge_vec3 hallway_position = { hallway_scale.x / 2 + index * hallway_scale.x, 0, -1.f };

	hge_transform hallway_transform = {
		hallway_position,
		hallway_scale,
		hgeQuaternion(0, 0, 0, 1)
	};

	if (index < infinihallway->dead_end - 1) return;

	hgeRenderSprite(
		shader,
		hallway_section_material,
		hallway_transform
	);

	if (index < infinihallway->dead_end) return;

	char door_number_str[4] = "##";
	sprintf(door_number_str, "%d", index);
	float font_size = 0.15f;
	float text_height = font_size / 2;
	hge_vec3 text_position = { hallway_position.x - 17.5, 3, hallway_position.z + 0.1f };
	hge_material text_material;
	text_material.color_multiplier = hgeVec4(1, 1, 1, 1);
	hge_transform text_transform = {
		text_position,
		hgeVec3(font_size, font_size, 0),
		hgeQuaternion(0, 0, 0, 1)
	};
	hgeRenderText(
		hgeResourcesQueryShader("text"),
		text_material,
		text_transform,
		hgeResourcesQueryFont("pixel"),
		HGE_TEXT_ALIGNED_CENTERED,
		door_number_str
	);
}

int section_at_x(float x) {
	int section = 0;
	int section_size = SECTION_WIDTH;
	section = (int)(x / section_size);
	return section;
}

void destroy_hotspots_out_of_range(infinihallway_component* infinihallway) {
	hge_ecs_request hotspot_request = hgeECSRequest(1, "hotspot");
	for (int i = 1; i < hotspot_request.NUM_ENTITIES; i++) {
		hge_entity* hotspot_entity = hotspot_request.entities[i];
		hge_transform* hotspot_transform = (hge_transform*)(hotspot_entity->components[hgeQuery(hotspot_entity, "transform")].data);
		int hotspot_section = section_at_x(hotspot_transform->position.x);

		if (hotspot_section < infinihallway->dead_end) {
			hgeDestroyEntity(hotspot_entity);
		}
	}
}

bool bpm_wait = false;
void bpm_ian() {
	int loop_timer_i = (int)(hgeRuntime() * 10) % 4;
	if (loop_timer_i == 0 && !bpm_wait) {
		bpm_wait = true;
		//printf("BOOP\n");
		hge_transform* player_transform = televoid_player_transform();
		spritesheet_component* player_spritesheet = televoid_player_spritesheet();
		hge_transform transform = *player_transform;
		if (player_spritesheet->flipped) transform.scale.x = -transform.scale.x;
		//create_fader_sprite(*player_transform, hgeResourcesQueryTexture("moose"), 3.f);
		create_fader_spritesheet(
			transform,
			hgeResourcesQueryTexture("moose"),
			hgeVec2(28, 28),
			player_spritesheet->frame,
			3.f
		);

	}
	else if (loop_timer_i != 0) {
		bpm_wait = false;
	}
}

void system_infinihallway(hge_entity* e, infinihallway_component* infinihallway) {
	hge_transform* player_transform = televoid_player_transform();
	if (!player_transform) return;
	float player_x = player_transform->position.x;
	int player_section = section_at_x(player_x);

	if (player_section - RENDER_DISTANCE > infinihallway->dead_end) {
		infinihallway->dead_end = player_section - RENDER_DISTANCE;
	}

	hge_texture hallway_section = hgeResourcesQueryTexture("hallway section");
	hge_texture hallway_section_destroyed = hgeResourcesQueryTexture("hallway section destroyed");

	for (int i = player_section - RENDER_DISTANCE; i < player_section + RENDER_DISTANCE; i++) {
		hge_texture section_texture = hallway_section;
		if (i < infinihallway->dead_end) {
			section_texture = hallway_section_destroyed;
		}
		render_section(infinihallway, i, section_texture);
	}
	bpm_ian();

	destroy_hotspots_out_of_range(infinihallway);

	if (player_section < infinihallway->dead_end - 1) {
		televoidLoadScene("res/scenes/HUB/HUB.tmx");
	}
}
