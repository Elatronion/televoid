#include "Dialogue.h"
#include "GameState.h"
#include "SafeTrash.h"

#include <HGE/HGE.h>

void print_event_type(dialogue_event_type type) {
	switch (type) {
	case DIALOGUE_MESSAGE:
		printf("TYPE: DIALOGUE_MESSAGE\n");
		break;
	case DIALOGUE_SCRIPT:
		printf("TYPE: DIALOGUE_SCRIPT\n");
		break;
	case DIALOGUE_SNIPPET:
		printf("TYPE: DIALOGUE_SNIPPET\n");
		break;
	case DIALOGUE_LEFT_SPRITE:
		printf("TYPE: DIALOGUE_LEFT_SPRITE\n");
		break;
	case DIALOGUE_RIGHT_SPRITE:
		printf("TYPE: DIALOGUE_RIGHT_SPRITE\n");
		break;
	}
}

static dialogue_event_type get_event_type_from_string(const char* type_str) {
	if (!strcmp(type_str, "MESSAGE")) {
		return DIALOGUE_MESSAGE;
	}
	else if (!strcmp(type_str, "SCRIPT")) {
		return DIALOGUE_SCRIPT;
	}
	else if (!strcmp(type_str, "SNIPPET")) {
		return DIALOGUE_SNIPPET;
	}
	else if (!strcmp(type_str, "LEFT")) {
		return DIALOGUE_LEFT_SPRITE;
	}
	else if (!strcmp(type_str, "RIGHT")) {
		return DIALOGUE_RIGHT_SPRITE;
	}
	else if (!strcmp(type_str, "TYPE")) {
		return DIALOGUE_HEADER;
	}

	printf("Type was: \"%s\"\n", type_str);

	HGE_CRASH("No type was determined", 0);
}

static dialogue_event_type get_event_type(char* source, char* start, char* end) {

	size_t size = (end - start) + 1;

	char* stringType = malloc(size);
	memcpy(stringType, start, size);
	stringType[size - 1] = '\0'; // Explicit null termination

	dialogue_event_type type = get_event_type_from_string(stringType);
	free(stringType);

	return type;
}

static const char* dialogue_create(char* source, char* start, char* end) {

	size_t size = (end - start) + 1;

	char* stringData = malloc(size);
	memcpy(stringData, start, size);
	stringData[size - 1] = '\0'; // Explicit null termination

	return stringData;
}

dialogue_event_node* dialogue_event_create(dialogue_event_type type, const char* data) {
	dialogue_event_node* event_node = malloc(sizeof(dialogue_event_node));
	event_node->event.type = type;
	event_node->event.data = data;
	//event_node->event.data = malloc(strlen(data) + 1);
	//strcpy(event_node->event.data, data);
	event_node->next = NULL;

	st_track(event_node->event.data);
	st_track(event_node);

	return event_node;
}

void dialogue_event_destroy(dialogue_event_node* event_node) {
	st_untrack(event_node->event.data);
	st_untrack(event_node);

	free(event_node->event.data);
	free(event_node);
}

void dialogue_event_destroy_list(dialogue_event_node* head) {
	dialogue_event_node* current = head;
	dialogue_event_node* next = head->next;
	while (current) {
		next = current->next;
		dialogue_event_destroy(current);
		current = next;
	}
}

void dialogue_event_push_list(dialogue_event_node* head, dialogue_event_node* node) {
	dialogue_event_node* current = head;
	while (current->next) {
		current = current->next;
	}
	current->next = node;
}

typedef enum {
	STATE_TYPE,
	STATE_VALUE
} reading_state;

dialogue_event_node* dialogue_load(const char* file_path) {

	dialogue_event_node* head = dialogue_event_create(DIALOGUE_MESSAGE, NULL);
	reading_state state = STATE_TYPE;
	dialogue_event_type type = DIALOGUE_HEADER;

	char* source = hgeLoadFileAsString(file_path);

	char* c = &source[0];
	char* begin = c;

	while (*c != '\0')
	{
		switch (state)
		{
		case STATE_TYPE:
			////////////////////////
			if (*c == ',')
			{
				// Take characters between begin and c
				type = get_event_type(source, begin, c);
				begin = c + 1; // ++ skips the separator
				state = STATE_VALUE;
			}
			////////////////////////
			break;
		case STATE_VALUE:
			switch (type)
			{
			case DIALOGUE_MESSAGE:
				if (*c == '"' && begin != c)
				{
					const char* data = dialogue_create(source, begin + 1, c);
					dialogue_event* dialogueEvent = dialogue_event_create(type, data);
					dialogue_event_push_list(head, dialogueEvent);
					state = STATE_TYPE;

					begin = c + 2; // ++ skips the separator and the '"'
				}
			break;
			case DIALOGUE_HEADER:
				if (*c == '\n')
				{
					state = STATE_TYPE;
					begin = c + 1; // ++ skips the separator
				}
				break;
			default:
				if (*c == '\n')
				{
					const char* data = dialogue_create(source, begin, c);
					dialogue_event* dialogueEvent = dialogue_event_create(type, data);
					dialogue_event_push_list(head, dialogueEvent);
					state = STATE_TYPE;
					begin = c + 1; // ++ skips the separator
				}
				break;
			}
			break;
		}
		
		c++;
	}

	free(source);

	dialogue_event_node* oldHead = head;
	head = head->next;
	dialogue_event_destroy(oldHead);

	return head;
}

void draw_dialogue_box(hge_texture left_sprite, hge_texture right_sprite, bool right_is_active, const char* string) {
	float y_offset = 0.0f;
	float y_offset_character_left = 0.0f;
	float y_offset_character_right = 0.0f;

	float half_screen_height = hgeWindowHeight() / 2;

	hge_shader gui_shader = hgeResourcesQueryShader("gui");

	hge_vec3 bg_scale = { fmin(800 - 50, hgeWindowWidth() - 50), 150, 0 };
	//hge_vec3 bg_position = { 0, y_offset + -hgeWindowHeight()/2 + bg_scale.y/2 + 25, 99 };
	float box_y = -(hgeWindowHeight() / 2.f) + bg_scale.y / 2 + 25;
	hge_vec3 bg_position = { 0, box_y , 99 };



	hge_material character_left_material = { left_sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
	hge_vec3 character_left_scale = { 475, 475, 0 };
	hge_vec3 character_left_position = {
		bg_position.x - bg_scale.x / 2 + character_left_scale.x / 4,
		y_offset + y_offset_character_left + -half_screen_height + character_left_scale.y / 2,
		98
	};

	hge_material character_right_material = { right_sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
	hge_vec3 character_right_scale = { 475, 475, 0 };
	hge_vec3 character_right_position = {
		bg_position.x + bg_scale.x / 2 - character_right_scale.x / 4,
		y_offset + y_offset_character_right + -half_screen_height + character_right_scale.y / 2,
		98
	};

	if (!right_is_active) {
		character_right_position.y -= 50;
		character_right_material.color_multiplier.w = 0.25f;
	}
	else {
		character_left_position.y -= 50;
		character_left_material.color_multiplier.w = 0.25f;
	}

	hge_transform character_left_transform = { character_left_position, character_left_scale, hgeQuaternion(0, 0, 0, 1) };
	hgeRenderSprite(
		gui_shader,
		character_left_material,
		character_left_transform
	);

	hge_transform character_right_transform = { character_right_position, character_right_scale, hgeQuaternion(0, 0, 0, 1) };
	hgeRenderSprite(
		gui_shader,
		character_right_material,
		character_right_transform
	);


	hge_material gui_background_material = { hgeResourcesQueryTexture("dialogue background"), hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 0, 1, 0.75f) };
	hge_transform bg_transform = { bg_position, bg_scale, hgeQuaternion(0, 0, 0, 1) };
	hgeRenderSprite(
		gui_shader,
		gui_background_material,
		bg_transform
	);

	float text_size = 50.f; // 100.f
	float text_height = text_size / 2;
	hge_vec3 text_position = { bg_position.x - bg_scale.x / 2 + 5, bg_position.y + bg_scale.y / 2 - text_height, 100 };

	hge_material text_material;
	text_material.color_multiplier = hgeVec4(1, 1, 1, 1);

	hge_transform text_transform = {
	  text_position,
	  hgeVec3(text_size / 100.f, text_size / 100.f, 0),
	  hgeQuaternion(0, 0, 0, 1)
	};

	hgeRenderText(
		hgeResourcesQueryShader("gui text"),
		text_material,
		text_transform,
		hgeResourcesQueryFont("VCR"),
		HGE_TEXT_ALIGNED_LEFT,
		string
	);
}

void dialogue_execute(dialogue_component* dialogue, dialogue_event event) {
	switch (event.type) {
	case DIALOGUE_SCRIPT:
		televoidWrenExecute(event.data);
		break;
	case DIALOGUE_SNIPPET:
		televoidWrenExecuteSnippet(event.data);
		break;
	case DIALOGUE_LEFT_SPRITE:
		dialogue->active_is_right = false;
		dialogue->left_character_sprite = hgeResourcesQueryTexture(event.data);
		break;
	case DIALOGUE_RIGHT_SPRITE:
		dialogue->active_is_right = true;
		dialogue->right_character_sprite = hgeResourcesQueryTexture(event.data);
		break;
	}
}

void system_dialogue(hge_entity* entity, dialogue_component* dialogue) {
	televoidSetGameState(GAME_DIALOGUE);

	if (hgeInputGetMouseDown(HGE_MOUSE_LEFT)) {
		dialogue->current = dialogue->current->next;
	}

	while (dialogue->current && dialogue->current->event.type != DIALOGUE_MESSAGE) {
		dialogue_execute(dialogue, dialogue->current->event);
		dialogue->current = dialogue->current->next;
	}

	if (dialogue->current == NULL) {
		dialogue_event_destroy_list(dialogue->head);
		televoidSetGameState(GAME_PLAY);
		hgeDestroyEntity(entity);
		return;
	}

	draw_dialogue_box(
		dialogue->left_character_sprite,
		dialogue->right_character_sprite,
		dialogue->active_is_right,
		dialogue->current->event.data
	);
}
