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
	hge_vec3 rendering_scale = transform->scale;
	if(spritesheet->flipped)
		rendering_scale.x = -rendering_scale.x;

	hgeRenderSpriteSheet(hgeResourcesQueryShader("sprite_shader"), spritesheet->spritesheet, transform->position, rendering_scale, 0.0f, spritesheet->resolution, spritesheet->frame);
}

void DebugCommandsSystem(hge_entity* entity, tag_component debug_commands) {
  if(hgeInputGetKeyDown(HGE_KEY_F2)) {
    televoidSceneLoad("res/scenes/Outside.tmx");
  }
  if(hgeInputGetKeyDown(HGE_KEY_F1)) {
    televoidSceneLoad("res/scenes/Inside.tmx");
  }
}

void TextSystem(hge_entity* entity, tag_component* ActiveCamera) {
  hge_mat4 projection_matrix;
  hgeUseShader(hgeResourcesQueryShader("text"));
  projection_matrix = hgeMathMat4OrthographicProjection(hgeWindowWidth(), hgeWindowHeight(), -100, 100);
  hgeShaderSetMatrix4(hgeResourcesQueryShader("text"), "projection", projection_matrix);
  hgeUseShader(hgeResourcesQueryShader("sprite_shader"));
}

int main() {
  hge_window window = { "T:OoTL", 800, 600 };
  hgeInit(60, window, HGE_INIT_ECS | HGE_INIT_RENDERING);

  SetDebugMode(true);

  hgeResourcesLoadShader("res/shaders/text.vs", NULL, "res/shaders/text.fs", "text");
  hgeResourcesLoadShader("res/gui.vs", NULL, "res/gui.fs", "gui_shader");
  hgeResourcesLoadTexture("res/textures/debug/loc.png", "debug_pointer_texture");
  hgeResourcesLoadTexture("res/textures/debug/trigger.png", "debug_trigger_texture");
  hgeResourcesLoadTexture("res/textures/debug/trigger (activated).png", "debug_trigger_on_texture");
  hgeResourcesLoadTexture("res/textures/debug/hotspot.png", "debug_hotspot_texture");
  hgeResourcesLoadTexture("res/textures/inventory/inventory.png", "GUI Inventory");
  hgeResourcesLoadTexture("res/textures/inventory/item_slot.png", "GUI Inventory Slot");

  // Systems //
  hgeAddSystem(system_light, 2, "position", "light");
  hgeAddSystem(system_scenelogic, 1, "scene logic");
  hgeAddSystem(TextSystem, 1, "ActiveCamera");
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

  hgeAddSystem(DialogueSystem, 1, "Dialogue");
	//hgeAddSystem(InteractionSystem, 2, "Transform", "Interactable");
  hgeAddSystem(InventorySystem, 1, "Inventory");
  hgeAddSystem(TriggerSystem, 2, "Transform", "Trigger");
  hgeAddSystem(HotspotSystem, 2, "Transform", "Hotspot");

  // Camera
  hge_entity* camera_entity = hgeCreateEntity();
	hge_camera cam = {true, true, 1.f/6.f, hgeWindowWidth(), hgeWindowHeight(), (float)hgeWindowWidth() / (float)hgeWindowHeight(), -100.0f, 100.0f };
  //hge_camera cam = {true, false, 90, hgeWindowWidth(), hgeWindowHeight(), (float)hgeWindowWidth() / (float)hgeWindowHeight(), 0.0001f, 100.0f };
	//hge_vec3 camera_position = { 0, 0, 50 };
  hge_vec3 camera_position = { 0, 0, 0 };
	orientation_component camera_orientation = {0.0f, -90.0f, 0.0f};
	hgeAddComponent(camera_entity, hgeCreateComponent("Camera", &cam, sizeof(cam)));
	hgeAddComponent(camera_entity, hgeCreateComponent("Position", &camera_position, sizeof(camera_position)));
	hgeAddComponent(camera_entity, hgeCreateComponent("Orientation", &camera_orientation, sizeof(camera_orientation)));
	tag_component activecam_tag;
	hgeAddComponent(camera_entity, hgeCreateComponent("ActiveCamera", &activecam_tag, sizeof(activecam_tag)));
	follow_component camera_follow_component = {NULL, 10.0f, false, false, true};
	hgeAddComponent(camera_entity, hgeCreateComponent("Follow", &camera_follow_component, sizeof(camera_follow_component)));

  freemove_component freemove = { 150.f };
  hgeAddComponent(camera_entity, hgeCreateComponent("FreeMove", &freemove, sizeof(freemove)));
  tag_component debug_commands;
  hgeAddComponent(camera_entity, hgeCreateComponent("DebugCommands", &debug_commands, sizeof(debug_commands)));

  scene_logic scene_l;
  hgeAddComponent(camera_entity, hgeCreateComponent("scene logic", &scene_l, sizeof(scene_l)));

  televoidSceneLoad("res/scenes/Outside.tmx");
  //televoidRunScript("res/scripts/test.wren");

  /*hge_vec3 hotspot_pos = { 50, 0, 0 };
  hge_vec3 hotspot_scl = { 30, 30, 0 };
  hotspot_component* c_h = televoidAddHotspot(hotspot_pos, hotspot_scl);
  c_h->event = televoidSceneLoad;*/

  /*hge_vec3 ian_pos = { 0, 0, 0 };
  televoidAddIanPlayer(ian_pos);

  // Make Camera Follow Player Position
  hge_entity* player_entity = hgeQueryEntity(1, "Player Ian");
  hge_transform* ian_transform_pointer = player_entity->components[hgeQuery(player_entity, "Transform")].data;
	follow_component camera_follow_component = {&ian_transform_pointer->position, 10.0f, false, false, true};
	hgeAddComponent(camera_entity, hgeCreateComponent("Follow", &camera_follow_component, sizeof(camera_follow_component)));*/

  // Working Dialogue System
  /*hge_entity* dialogue_entity = hgeCreateEntity();
  dialogue_component dialogue;
  dialogue.cur_message = &dialogue.root;
  strcpy(dialogue.root.str, "Hello, World: Component!");
  dialogue.root.next = NULL;
  AppendMessage(&dialogue.root, "This is the second message!");
  AppendMessage(&dialogue.root, "This is the last message.\nSo, why not test some crazy stuff!");
  hgeAddComponent(dialogue_entity, hgeCreateComponent("Dialogue", &dialogue, sizeof(dialogue)));

  televoidTextCreate("res/fonts/VCR.ttf");*/

  hge_entity* light_entity = hgeCreateEntity();
  hge_vec3 light_position = { 0, 16, 200 };
  hgeAddComponent(light_entity,
    hgeCreateComponent("position", &light_position, sizeof(light_position)));
  hge_light light;
  light.ambient = hgeCreateVec3(0.1f, 0.1f, 0.1f);
  light.diffuse = hgeCreateVec3(1, 0, 0);
  light.specular = light.diffuse;

  light.constant = 1.0f;
  light.linear = 0.0014f;
  light.quadratic = 0.000007;
  hgeAddComponent(light_entity,
    hgeCreateComponent("light", &light, sizeof(light)));

  hgeStart();
  CleanUpForgottenDialoguePointers();
  return 0;
}
