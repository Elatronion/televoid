#include <HGE/HGE_Core.h>
#include <HGE/hge_components.h>
#include <HGE/HGE_FileUtility.h>
#include <HGE/HGE_AudioSample.h>
#include <HGE/HGE_AudioSource.h>

#include "Scene.h"
#include "dirent.h"

#include "Floor.h"

void televoid_system_spritesheet(hge_entity* entity, hge_transform* transform, spritesheet_component* spritesheet) {
	// Flip By Reversing X Scale
	hge_vec3 rendering_scale = transform->scale;
	if(spritesheet->flipped)
		rendering_scale.x = -rendering_scale.x;

	hge_transform rendered_transform = *transform;
	rendered_transform.scale = rendering_scale;
	hgeRenderSpriteSheet(hgeResourcesQueryShader("basic"), spritesheet->spritesheet_material, rendered_transform, spritesheet->resolution, spritesheet->frame);

	if(televoidGameState() == GAME_PAUSE) return;

	// Update Frame X Relative To FPS
	spritesheet->time += hgeDeltaTime();
	if (spritesheet->time >= 1.0f/spritesheet->FPS) {
			spritesheet->frame.x++;
		if(spritesheet->frame.x > spritesheet->num_frames)
			spritesheet->frame.x = 0;
		spritesheet->time = 0;
	}

}

/*
typedef struct {
	hge_mesh mesh;
} background_component;

void system_background_rendering(hge_entity* entity, background_component* background, hge_transform* transform) {

	hge_material material = {
		hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
		hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
	};

	hgeRenderMesh(hgeResourcesQueryShader("basic"), material, background->mesh, *transform);
}
*/

void system_mesh_renderer(hge_entity* entity, hge_transform* transform, hge_mesh* mesh) {
	hge_material material = {
		hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
		hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
	};

	hgeRenderMesh(hgeResourcesQueryShader("basic"), mesh->material, *mesh, *transform);
}

void system_freemove(hge_entity* entity, tag_component* freemove, hge_transform* transform) {
	float speed = 100;
	if(hgeInputGetKey(HGE_KEY_A)) {
		transform->position.x -= speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_D)) {
		transform->position.x += speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_S)) {
		transform->position.y -= speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_W)) {
		transform->position.y += speed * hgeDeltaTime();
	}

	if(hgeInputGetKey(HGE_KEY_LEFT_SHIFT)) {
		transform->position.z -= speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_LEFT_CONTROL)) {
		transform->position.z += speed * hgeDeltaTime();
	}
}


void televoid_system_follower(hge_entity* entity, hge_transform* transform, follow_component* follow) {
	if(televoidGameState() == GAME_CUTSCENE) return;
	if(!follow->target_pos) return;
	if(!follow->lock_x) transform->position.x += (follow->target_pos->x - transform->position.x) * follow->speed * hgeDeltaTime();
	if(!follow->lock_y) transform->position.y += (follow->target_pos->y - transform->position.y) * follow->speed * hgeDeltaTime();
	//if(!follow->lock_z) transform->position.z += (follow->target_pos->z - transform->position.z) * follow->speed * hgeDeltaTime();
	transform->position.z += (100 - transform->position.z) * follow->speed * hgeDeltaTime();
}

void gui_shader_math() {
  float screen_width = hgeWindowWidth();
  float screen_height = hgeWindowHeight();
  hge_mat4 projection_matrix = hgeMat4OrthographicProjection(screen_width, screen_height, -500, 500);
  for(int i = 0; i < hgeResourcesNumShaders(); i++) {
    hge_resource_shader* shaders = hgeResourcesGetShaderArray();
    hge_shader shader = shaders[i].shader;
    hgeUseShader(shader);
    hgeShaderSetMatrix4(shader, "pixel_perfect_projection", projection_matrix);
  }
}

void televoid_system_global_update(hge_entity* entity, tag_component* global_updater) {
	gui_shader_math();
	televoid_inventory_update();
	televoidMinigameUpdate();
	televoidSceneUpdate();
}

int main(int argc, char **argv) {
	hge_window window = { "Voidjam", 1280, 720 };
	hgeInit(60, window, HGE_INIT_ECS | HGE_INIT_RENDERING | HGE_INIT_AUDIO);
	st_init();

	televoidGenerateAllItems();

	hgeResourcesLoadTexture("res/textures/sprites/minigame-exit-button.png", "GUI Minigame Exit");

	hgeResourcesLoadTexture("res/textures/sprites/inventory/inventory.png", "GUI Inventory");
	hgeResourcesLoadTexture("res/textures/sprites/inventory/item_slot.png", "GUI Inventory Slot");

	// Shaders
	hgeResourcesLoadShader("res/shaders/gui.vs", NULL, "res/shaders/gui.fs", "gui");
  //hgeResourcesLoadShader("res/shaders/text.vs", NULL, "res/shaders/text.fs", "text");
	hgeResourcesLoadShader("res/shaders/gui text.vs", NULL, "res/shaders/text.fs", "gui text");

	// Fonts
	hgeResourcesLoadFont("res/fonts/VCR OSD Mono.ttf", "VCR");

	// Sprites
	hgeResourcesLoadTexture("res/textures/debug/trigger.png", "debug_trigger_texture");
	hgeResourcesLoadTexture("res/textures/debug/trigger (active).png", "debug_trigger_texture (active)");
	hgeResourcesLoadTexture("res/textures/debug/hotspot.png", "debug_hotspot_texture");
	hgeResourcesLoadTexture("res/ROOM END.png", "TEST");
	hgeResourcesLoadTexture("res/HGE/DEFAULT NORMAL.png", "HGE DEFAULT NORMAL");

	hgeResourcesLoadTexture("res/textures/sprites/moose.png", "moose");

	// Dialogue
	const char* dialogue_character_portraits_path = "res/textures/dialogue/";
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (dialogue_character_portraits_path)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			char *dot = strrchr(ent->d_name, '.');
			if (dot && !strcmp(dot, ".png")) {
        ent->d_name[strlen(ent->d_name)-4] = '\0';
        HGE_LOG("Character Portrait %s", ent->d_name);
				const char* path = malloc(strlen(dialogue_character_portraits_path) + strlen(ent->d_name) + strlen(".png") + 1);
				sprintf(path, "%s%s.png", dialogue_character_portraits_path, ent->d_name);
				HGE_LOG("Character Portrait Path: \"%s\"", path);
				hgeResourcesLoadTexture(path, ent->d_name);
				free(path);
      }
		}
		closedir (dir);
	} else {
		return EXIT_FAILURE;
	}


	// Meshes
	//hgeResourcesLoadMesh("res/meshes/test_background.obj", "background mesh");

	// Audio
	hgeResourcesLoadAudio("res/audio/sfx/ian/steps/step1.wav", "step1");
	hgeResourcesLoadAudio("res/audio/sfx/ian/steps/step2.wav", "step2");
	hgeResourcesLoadAudio("res/audio/sfx/ian/steps/step3.wav", "step3");

	//hgeAddBaseSystems();
	hgeAddSystem(televoid_system_global_update, 1, "global updater");
	hgeAddSystem(televoid_system_follower, 2, "transform", "follower");
	hgeAddSystem(hge_system_sprite, 2, "transform", "sprite");
	hgeAddSystem(system_prop, 3, "transform", "material", "prop");
	hgeAddSystem(televoid_system_spritesheet, 2, "transform", "spritesheet");
	//hgeAddSystem(system_test, 1, "audiosource");
	hgeAddSystem(PlayerCharacterControlSystem, 2, "playable", "character");
	hgeAddSystem(CharacterSystem, 3, "character", "transform", "spritesheet");
	//hgeAddSystem(system_background_rendering, 2, "background", "transform");
	if(false) hgeAddSystem(system_freemove, 2, "freemove", "transform");
	hgeAddSystem(system_mouse_picker, 2, "transform", "mouse picker");

	hgeAddSystem(system_dialogue, 1, "dialogue");
	hgeAddSystem(system_imv, 1, "imv");

	hgeAddSystem(system_mesh_renderer, 2, "transform", "mesh");

	hgeAddSystem(system_trigger_logic, 2, "transform", "trigger");

	hgeAddSystem(system_floor, 1, "floor");
	// debug systems
	hgeAddSystem(system_trigger_renderer, 2, "transform", "trigger");
	hgeAddSystem(system_hotspot_renderer, 2, "transform", "hotspot");
	hgeAddSystem(system_floor_debug, 1, "floor");

	// Global Updater
	hge_entity* global_updater_entity = hgeCreateEntity();
	tag_component global_updater;
	hgeAddComponent(global_updater_entity, hgeCreateComponent("global updater", &global_updater, sizeof(global_updater)));

	// Camera
	//televoidCreatePlayerCamera(hgeVec3(0, 0, 100));

	switch(argc) {
		case 2:
			televoidLoadScene(argv[1]);
			break;
		default:
			televoidLoadScene("res/scenes/splash.tmx");
	}

	televoid_system_global_update(NULL, NULL);
	//televoidIMVCreate("res/imv/test.imv");


	hgeStart();
	//televoidSceneDestroy();
	imvCleanAll();
	televoidItemsClean();
	televoidMinigameClean();
	st_clean();

	return 0;
}
