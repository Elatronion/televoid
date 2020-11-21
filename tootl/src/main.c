//#define HGE_LOGGING
#include <HGE/HGE_Core.h>
#include "Character.h"
#include "Inventory.h"
#include "GameState.h"
#include "Scene.h"
#include "Script.h"
#include "text.h"
#include "Dialogue.h"


void Televoid_FollowTarget(hge_entity* entity, hge_vec3* position, follow_component* follow) {
  if(!follow->target_pos) return;
  if(!follow->lock_x) position->x += (follow->target_pos->x - position->x) * follow->speed * hgeDeltaTime();
	if(!follow->lock_y) position->y += (follow->target_pos->y - position->y) * follow->speed * hgeDeltaTime();
	if(!follow->lock_z) position->z += (follow->target_pos->z - position->z) * follow->speed * hgeDeltaTime();
}

void TelevoidSpriteSheetSystem(hge_entity* entity, hge_transform* transform, spritesheet_component* spritesheet) {
  hge_shader sprite_shader = hgeResourcesQueryShader("sprite_shader");
  hgeUseShader(sprite_shader);

  hgeShaderSetBool(sprite_shader, "lit", true);

	// Update Frame X Relative To FPS
  if(GetGameState() != GAME_PAUSE)
	spritesheet->time += hgeDeltaTime();
	if (spritesheet->time >= 1.0f/spritesheet->FPS) {
			spritesheet->frame.x++;
		if(spritesheet->frame.x > spritesheet->num_frames)
			spritesheet->frame.x = 0;
		spritesheet->time = 0;
	}

	// Flip By Reversing X Scale
	/*hge_vec3 rendering_scale = transform->scale;
	if(spritesheet->flipped)
		rendering_scale.x = -rendering_scale.x;*/

  hgeShaderSetBool(sprite_shader, "flipped", spritesheet->flipped);
	hgeRenderSpriteSheet(hgeResourcesQueryShader("sprite_shader"), spritesheet->spritesheet_material, transform->position, transform->scale, 0.0f, spritesheet->resolution, spritesheet->frame);

  hgeShaderSetBool(sprite_shader, "flipped", false);
  hgeShaderSetBool(sprite_shader, "lit", false);
}

void DebugCommandsSystem(hge_entity* entity, tag_component debug_commands) {
  /*if(hgeInputGetKeyDown(HGE_KEY_F2)) {
    televoidSceneLoad("res/scenes/Outside.tmx");
  }
  if(hgeInputGetKeyDown(HGE_KEY_F1)) {
    televoidSceneLoad("res/scenes/Inside.tmx");
  }*/
}

void TextSystem(hge_entity* entity, tag_component* ActiveCamera) {
  hge_mat4 projection_matrix;
  projection_matrix = hgeMathMat4OrthographicProjection(hgeWindowWidth(), hgeWindowHeight(), -100, 100);
  hgeUseShader(hgeResourcesQueryShader("text"));
  hgeShaderSetMatrix4(hgeResourcesQueryShader("text"), "projection", projection_matrix);
  hgeUseShader(hgeResourcesQueryShader("gui"));
  hgeShaderSetMatrix4(hgeResourcesQueryShader("gui"), "projection", projection_matrix);
  hgeUseShader(hgeResourcesQueryShader("sprite_shader"));
}

void light_sprite_system(hge_entity* entity, hge_vec3* position, hge_pointlight* pointlight) {
  float scale = 15;
  hge_material material = { hgeResourcesQueryTexture("pointlight blub"), hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  hgeRenderSprite(
    hgeResourcesQueryShader("sprite_shader"),
    material,
    *position, hgeVec3(scale, scale, 0), 0.f
  );
  /*hge_mesh mesh = hgeResourcesQueryMesh("HGE MESH LIGHT");
	mesh.material.diffuse = hgeResourcesQueryTexture("HGE DEFAULT WHITE");
	mesh.material.normal = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");
  hgeRenderMesh(
    hgeResourcesQueryShader("sprite_shader"),
    mesh,
    *position, hgeVec3(scale, scale, scale)
  );*/
}

void sprite_rendering_system(hge_entity* entity, hge_transform* transform, hge_material* material) {
  hge_shader sprite_shader = hgeResourcesQueryShader("sprite_shader");
  hgeShaderSetBool(sprite_shader, "lit", true);
  hgeRenderSprite(
    sprite_shader,
    *material,
    transform->position, transform->scale, 0.0f
  );
  hgeShaderSetBool(sprite_shader, "lit", false);
}

void spin_dirlight_system(hge_entity* entity, hge_dirlight* dirlight) {
  if(!hgeInputGetKey(HGE_KEY_R)) return;
  float angle = hgeRuntime() * 100;
  dirlight->direction.x = cos(hgeMathRadians(angle));
  dirlight->direction.y = sin(hgeMathRadians(angle));


  //position->x = cos(hgeMathRadians(hgeRuntime()*10)) * 10 + 8*TILESIZE;
	//position->z = sin(hgeMathRadians(hgeRuntime()*10)) * 10 + 8*TILESIZE;
	//orientation->yaw = hgeRuntime() * 10 - 180;

  hge_shader gui_shader = hgeResourcesQueryShader("gui");
  hgeUseShader(gui_shader);
  hgeShaderSetVec4(gui_shader, "gui_color", hgeVec4(1, 1, 1, 1));
  hge_material dirlight_mat = { hgeResourcesQueryTexture("dirlight arrow"), hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  hge_vec3 scale = hgeVec3(15*4, 22*4, 0);
  hge_vec3 position = hgeVec3(
    cos(hgeMathRadians(angle+180)) * (hgeWindowWidth()/2.5f), //-hgeWindowWidth()/2 + 15*4,
    sin(hgeMathRadians(angle+180)) * (hgeWindowHeight()/2.5f), //hgeWindowHeight()/2 - 22*4,
    0
  );

  hgeRenderSprite(
    gui_shader,
    dirlight_mat,
    position, scale, -angle + 90
  );
}

typedef struct {
	hge_vec3 target_position;
	float speed;
} c_camera_controller;

void camera_controller(hge_entity* entity, c_camera_controller* controller, hge_camera* camera, hge_vec3* position, orientation_component* orientation) {
	//hgeShaderSetVec3(hgeResourcesQueryShader("sprite_shader"), "viewPos", *camera_position);

	float speed = controller->speed;
	//float frictionCoeff = 4.0f;

	float yaw_speed = 100.0f;
	if(hgeInputGetKey(HGE_KEY_SPACE)) {
		controller->target_position.y += speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_LEFT_CONTROL)) {
		controller->target_position.y -= speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_A)) {
		controller->target_position.x -= cos(hgeMathRadians(orientation->yaw+90)) * speed * hgeDeltaTime();
		controller->target_position.z -= sin(hgeMathRadians(orientation->yaw+90)) * speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_D)) {
		controller->target_position.x += cos(hgeMathRadians(orientation->yaw+90)) * speed * hgeDeltaTime();
		controller->target_position.z += sin(hgeMathRadians(orientation->yaw+90)) * speed * hgeDeltaTime();
	}

	if(hgeInputGetKey(HGE_KEY_W)) {
		controller->target_position.x += cos(hgeMathRadians(orientation->yaw)) * speed * hgeDeltaTime();
		controller->target_position.z += sin(hgeMathRadians(orientation->yaw)) * speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_S)) {
		controller->target_position.x -= cos(hgeMathRadians(orientation->yaw)) * speed * hgeDeltaTime();
		controller->target_position.z -= sin(hgeMathRadians(orientation->yaw)) * speed * hgeDeltaTime();
	}

	if(hgeInputGetKey(HGE_KEY_LEFT)) {
		orientation->yaw -= yaw_speed * hgeDeltaTime();
	}
	if(hgeInputGetKey(HGE_KEY_RIGHT)) {
		orientation->yaw += yaw_speed * hgeDeltaTime();
	}

	position->x += (controller->target_position.x - position->x) * controller->speed * hgeDeltaTime();
	position->y += (controller->target_position.y - position->y) * controller->speed * hgeDeltaTime();
	position->z += (controller->target_position.z - position->z) * controller->speed * hgeDeltaTime();
}

void BackgroundSystem(hge_entity* entity, hge_transform* transform, background* bg) {
  hge_shader sprite_shader = hgeResourcesQueryShader("sprite_shader");
  hgeUseShader(sprite_shader);
  hgeShaderSetBool(sprite_shader, "lit", bg->lit);
	hgeRenderSprite(hgeResourcesQueryShader("sprite_shader"), bg->material, transform->position, transform->scale, 0.0f);
  hgeShaderSetBool(sprite_shader, "lit", false);
}

int main(int argc, char **argv) {
  hge_window window = { "T:OoTL", 800, 600 };
  hgeInit(120, window, HGE_INIT_ECS | HGE_INIT_RENDERING);

  SetDebugMode(true);

  hgeResourcesLoadShader("res/shaders/gui.vs", NULL, "res/shaders/gui.fs", "gui");
  hgeResourcesLoadShader("res/shaders/text.vs", NULL, "res/shaders/text.fs", "text");
  hgeResourcesLoadShader("res/gui.vs", NULL, "res/gui.fs", "gui_shader");
  hgeResourcesLoadTexture("res/textures/debug/loc.png", "debug_pointer_texture");
  hgeResourcesLoadTexture("res/textures/debug/trigger.png", "debug_trigger_texture");
  hgeResourcesLoadTexture("res/textures/debug/trigger (activated).png", "debug_trigger_on_texture");
  hgeResourcesLoadTexture("res/textures/debug/hotspot.png", "debug_hotspot_texture");
  hgeResourcesLoadTexture("res/textures/inventory/inventory.png", "GUI Inventory");
  hgeResourcesLoadTexture("res/textures/inventory/item_slot.png", "GUI Inventory Slot");

  //hgeResourcesLoadTexture("res/textures/sprites/moose.png", "moose");
  hgeResourcesLoadTexture("res/textures/sprites/moose.png", "moose");
  hgeResourcesLoadTexture("res/textures/sprites/moose_normal.png", "moose normal");

  // Props
  hgeResourcesLoadTexture("res/textures/sprites/props/chair_diffuse.png", "chair diffuse");
  hgeResourcesLoadTexture("res/textures/sprites/props/chair_normal.png", "chair normal");
  hgeResourcesLoadTexture("res/textures/HGE/dirlight_arrow.png", "dirlight arrow");
  hgeResourcesLoadTexture("res/textures/HGE/pointlight_bulb.png", "pointlight blub");

  // Dialogue
  hgeResourcesLoadTexture("res/textures/dialogue/background.png", "dialogue background");
  hgeResourcesLoadTexture("res/textures/dialogue/Frank/Frank_Idle.png", "Frank_Idle");
  hgeResourcesLoadTexture("res/textures/dialogue/Ian/Ian_Idle.png", "Ian_Idle");
  hgeResourcesLoadTexture("res/textures/dialogue/Sam/Sam_Idle.png", "Sam_Idle");

  hgeResourcesLoadTexture("res/textures/HGE/DEFAULT WHITE.png", "HGE DEFAULT WHITE");
  hgeResourcesLoadTexture("res/textures/HGE/DEFAULT NORMAL.png", "HGE DEFAULT NORMAL");
  hgeResourcesLoadTexture("res/textures/HGE/LIGHT.png", "HGE GUI LIGHT");
  hgeResourcesLoadMesh("res/textures/HGE/LIGHT.obj", "HGE MESH LIGHT");
  //hgeResourcesLoadTexture("res/textures/sprites/moose_normal.png", "HGE DEFAULT NORMAL");

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Systems //
  hgeAddSystem(camera_controller, 4, "camera controller", "Camera", "Position", "Orientation");


  hgeAddSystem(spin_dirlight_system, 1, "dirlight");
  hgeAddSystem(hge_system_dirlight, 1, "dirlight");
  hgeAddSystem(hge_system_pointlight, 2, "position", "pointlight");
  //hgeAddSystem(hge_system_pointlight, 2, "Position", "pointlight");
  hgeAddSystem(light_sprite_system, 2, "position", "pointlight");

  hgeAddSystem(system_scenelogic, 1, "scene logic");
  hgeAddSystem(TextSystem, 1, "ActiveCamera");
  hgeAddSystem(BackgroundSystem, 2, "transform", "background");
	hgeAddSystem(CharacterSystem, 3, "Character", "Transform", "SpriteSheet");
	hgeAddSystem(PlayerCharacterControlSystem, 2, "Playable", "Character");
	//hgeAddSystem(PuzzleRoomSystem, 1, "PuzzleRoom");
  //hgeAddBaseSystems();
  hgeAddSystem(DebugCommandsSystem, 1, "DebugCommands");
  hgeAddSystem(FreeCam, 3, "Camera", "FreeMove", "Position");
	hgeAddSystem(CameraSystem, 3, "Camera", "Position", "Orientation");
	hgeAddSystem(Televoid_FollowTarget, 2, "Position", "Follow");
	hgeAddSystem(SpriteRenderingSystem, 2, "Transform", "Sprite");
	//hgeAddSystem(SpriteSheetSystem, 2, "Transform", "SpriteSheet");
  hgeAddSystem(TelevoidSpriteSheetSystem, 2, "Transform", "SpriteSheet");
  hgeAddSystem(sprite_rendering_system, 2, "transform", "material");

	//hgeAddSystem(InteractionSystem, 2, "Transform", "Interactable");
  hgeAddSystem(InventorySystem, 1, "Inventory");
  hgeAddSystem(TriggerSystem, 2, "Transform", "Trigger");
  hgeAddSystem(HotspotSystem, 2, "Transform", "Hotspot");
  hgeAddSystem(ItemSystem, 2, "Transform", "Item");

  hgeAddSystem(DialogueSystem, 1, "Dialogue");

  // Loading
  televoidGenerateAllItems();

  // Camera
  hge_entity* camera_entity = hgeCreateEntity();
	hge_camera cam = {true, true, 1.f/6.f, hgeWindowWidth(), hgeWindowHeight(), (float)hgeWindowWidth() / (float)hgeWindowHeight(), -500.0f, 500.0f };
  hge_vec3 camera_position = { 0, 0, 0 };
  //hge_camera cam = {true, false, 90, hgeWindowWidth(), hgeWindowHeight(), (float)hgeWindowWidth() / (float)hgeWindowHeight(), 0.5f, 1000.0f };
	//hge_vec3 camera_position = { 0, 0, 50 };
	orientation_component camera_orientation = {0.0f, -90.0f, 0.0f};
	hgeAddComponent(camera_entity, hgeCreateComponent("Camera", &cam, sizeof(cam)));
	hgeAddComponent(camera_entity, hgeCreateComponent("Position", &camera_position, sizeof(camera_position)));
	hgeAddComponent(camera_entity, hgeCreateComponent("Orientation", &camera_orientation, sizeof(camera_orientation)));
	tag_component activecam_tag;
	hgeAddComponent(camera_entity, hgeCreateComponent("ActiveCamera", &activecam_tag, sizeof(activecam_tag)));

  follow_component camera_follow_component = {NULL, 10.0f, false, false, true};
	hgeAddComponent(camera_entity, hgeCreateComponent("Follow", &camera_follow_component, sizeof(camera_follow_component)));


  /*
  c_camera_controller camera_controller = { hgeVec3(0, 0, 0), 100.f };
  hgeAddComponent(camera_entity, hgeCreateComponent("camera controller", &camera_controller, sizeof(camera_controller)));
  */

  /*
  hge_pointlight light = hgePointLight(
    hgeVec3(0, 0, 1),
    hgeVec3(0, 0, 0.1),
    hgeVec3(0, 0, 0), // Specular
    1.0f,
    0.007, 	0.0002
  );
  hgeAddComponent(camera_entity, hgeCreateComponent("pointlight", &light, sizeof(light)));
  */

  /*
  freemove_component freemove = { 150.f };
  hgeAddComponent(camera_entity, hgeCreateComponent("FreeMove", &freemove, sizeof(freemove)));
  */
  tag_component debug_commands;
  hgeAddComponent(camera_entity, hgeCreateComponent("DebugCommands", &debug_commands, sizeof(debug_commands)));

  scene_logic scene_l;
  hgeAddComponent(camera_entity, hgeCreateComponent("scene logic", &scene_l, sizeof(scene_l)));

  if(argc > 1) {
    televoidSceneLoad(argv[1]);
  } else {
    const char* default_scene = "res/scenes/demo/Outside.tmx";
    HGE_WARNING("No Televoid Scene Provided, Loading default \"%s\".", default_scene);
    televoidSceneLoad(default_scene);
  }

  //televoidSceneLoad("res/scenes/Outside.tmx");
  //televoidRunScript("res/scripts/test.wren");

  // Working Dialogue System

  /*hge_entity* dialogue_entity = hgeCreateEntity();
  dialogue_component dialogue = CreateDialogue("This is the first message!", "Frank_Idle", "Sam_Idle");
  //printf("gui_y : %s\n", dialogue.root.str);
  dialogue.cur_message = &dialogue.root;
  AppendMessage(&dialogue.root, "This is the second message!", "Ian_Idle", "Frank_Idle");
  AppendMessage(&dialogue.root, "This is the last message.\nSo, why not test some crazy stuff!", "Frank_Idle", "Sam_Idle");
  hgeAddComponent(dialogue_entity, hgeCreateComponent("Dialogue", &dialogue, sizeof(dialogue)));*/

  televoidCreateDialogue();

  televoidTextCreate("res/fonts/VCR.ttf");

  hgeStart();
  CleanUpForgottenDialoguePointers();
  //televoidCleanAllItems();
  return 0;
}
