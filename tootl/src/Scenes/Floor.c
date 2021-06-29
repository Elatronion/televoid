#include "Floor.h"
#include "Character.h"

void floor_push(floor_node* root, floor_node* node) {
	floor_node* current = root;
	while (current->next) {
		current = current->next;
	}
	current->next = node;
}

void floor_order(floor_node* root) {
	// sort nodes by X in ascending order
}

void floor_push_position(floor_t* floor, hge_vec2 position) {
	floor_node* node = malloc(sizeof(floor_node));
	node->position = position;
	node->next = NULL;

	if (!floor->root) {
		floor->root = node;
	}
	else {
		floor_push(floor->root, node);
		floor_order(floor->root);
	}
}

float floor_height_at_x(floor_t* floor, float x) {
	float height = 0;

	bool found = false;
	floor_node* node = floor->root;
	while (node->next) {
		if (x >= node->position.x && x <= node->next->position.x) {
			found = true;
			break;
		}
		node = node->next;
	}

	if (found) {
		floor_node* next = node->next;
		/*float ratio =
		(next->position.x - node->position.x) / (x -  node->position.x);*/
		/*float ratio =
		  (next->position.x - node->position.x) / (next->position.x - node->position.x);*/
		float ratio =
			(x - node->position.x) / (next->position.x - node->position.x);
		height = hgeMathFunc_lerp(node->position.y, next->position.y, ratio);
	}

	return height;
}

float floor_left_most_x(floor_t* floor) {
	floor_node* node = floor->root;
	return node->position.x;
}

float floor_right_most_x(floor_t* floor) {
	floor_node* node = floor->root;
	while (node->next) node = node->next;
	return node->position.x;
}

void system_floor(hge_entity* entity, floor_t* floor) {
	hge_entity* ian_entity = hgeQueryEntity(2, "character", "playable");
	hge_transform* ian_transform = (hge_transform*)ian_entity->components[hgeQuery(ian_entity, "transform")].data;
	character_component* ian_character = (character_component*)ian_entity->components[hgeQuery(ian_entity, "character")].data;

	float left_most_x = floor_left_most_x(floor);
	float right_most_x = floor_right_most_x(floor);

	float player_x = ian_transform->position.x;
	if (player_x < left_most_x) {
		ian_transform->position.x = left_most_x;
		CharacterGroundClick(ian_character, ian_transform->position);
	}
	else if (player_x > right_most_x) {
		ian_transform->position.x = right_most_x;
		CharacterGroundClick(ian_character, ian_transform->position);
	}
	player_x = ian_transform->position.x;

	float player_y = floor_height_at_x(floor, player_x) + ian_transform->scale.y / 2;
	ian_transform->position.y = player_y;
}

void system_floor_debug(hge_entity* entity, floor_t* floor) {
	floor_node* node = floor->root;
	while (node) {
		hge_material material = {
		  hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
		  hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
		  false, hgeVec4(1, 1, 1, 1)
		};
		hge_transform transform = { hgeVec3(node->position.x, node->position.y, 0), hgeVec3(2, 2, 0), hgeQuaternion(0, 0, 0, 1) };
		hgeRenderSprite(
			hgeResourcesQueryShader("basic"),
			material,
			transform
		);
		node = node->next;
	}
}
