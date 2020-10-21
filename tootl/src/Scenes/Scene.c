#include "Scene.h"
#include "Character.h"
#include "tmx.h"
#define MAX_SCENE_ENTITIES 200

#define SCENE_TRANSITION_TIME 1
float scene_transition_chrono = 0.0f;
bool is_loading = false;
char level_that_is_loading[255];

int num_scene_entities = 0;
hge_entity* scene_entities[MAX_SCENE_ENTITIES];
char last_loaded_scene[255];
void UnloadScene() {
  for(int i = 0; i < num_scene_entities; i++) {
    hgeDestroyEntity(scene_entities[i]);
  }
  num_scene_entities = 0;
}

void ParseTMXItem(tmx_object* object) {
  printf("Add Item '%s'\n", object->name);

  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 90 };
  hge_vec3 scl = { object->width, object->height, 90 };
  televoidAddItem(pos, scl, object->name);
}

void ParseTMXHotspot(tmx_object* object) {
  trigger trigger_component;
  tmx_properties* properties = object->properties;
  tmx_property* property_action_script = tmx_get_property(object->properties, "action_script");
  if(!property_action_script) return;
  char script_path[255];
  strcpy(script_path, property_action_script->value.string);
  // Copy string data
  /*

  printf("trigger:\n\taction_script: '%s'\n", trigger_component.action_script);
  hge_entity* entity = hgeCreateEntity();
  hgeAddComponent(entity, hgeCreateComponent("Trigger", &trigger_component, sizeof(trigger_component)));
  hge_transform ian_character_transform;
  ian_character_transform.position.x = object->x + object->width/2;
  ian_character_transform.position.y = -object->y - object->height/2;
  ian_character_transform.position.z = 90.0f;
  ian_character_transform.scale.x = object->width;
  ian_character_transform.scale.y = object->height;
  ian_character_transform.scale.z = 0.0f;
  hgeAddComponent(entity, hgeCreateComponent("Transform", &ian_character_transform, sizeof(ian_character_transform)));

  scene_entities[num_scene_entities] = entity;
  num_scene_entities++;
  */
  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 90 };
  hge_vec3 scl = { object->width, object->height, 90 };
  televoidAddHotspot(pos, scl, script_path);
}

void ParseTMXTrigger(tmx_object* object) {
  trigger trigger_component;
  tmx_properties* properties = object->properties;
  tmx_property* property_action_script = tmx_get_property(object->properties, "action_script");
  if(!property_action_script) return;
  // Copy string data
  strcpy(&trigger_component.action_script, property_action_script->value.string);

  printf("trigger:\n\taction_script: '%s'\n", trigger_component.action_script);
  hge_entity* entity = hgeCreateEntity();
	hgeAddComponent(entity, hgeCreateComponent("Trigger", &trigger_component, sizeof(trigger_component)));
  hge_transform ian_character_transform;
  ian_character_transform.position.x = object->x + object->width/2;
  ian_character_transform.position.y = -object->y - object->height/2;
  ian_character_transform.position.z = 90.0f;
  ian_character_transform.scale.x = object->width;
  ian_character_transform.scale.y = object->height;
  ian_character_transform.scale.z = 0.0f;
  hgeAddComponent(entity, hgeCreateComponent("Transform", &ian_character_transform, sizeof(ian_character_transform)));

  scene_entities[num_scene_entities] = entity;
  num_scene_entities++;
}

void ParseTMXPlayerStart(tmx_object* object) {
  player_start start;
  tmx_properties* properties = object->properties;
  tmx_property* property_previous_scene = tmx_get_property(object->properties, "previous_scene_that_activates");
  tmx_property* property_face_left = tmx_get_property(object->properties, "face_left");
  tmx_property* property_fade_speed = tmx_get_property(object->properties, "fade_speed");
  tmx_property* property_fadein_on_start = tmx_get_property(object->properties, "fadein_on_start");

  if(!property_previous_scene) {
    // Error: No Previous Scene
    HGE_ERROR("no previous scene");
    return;
  }

  //start.previous_scene_that_activates = property_previous_scene->value.string;
  // Copy string data
  strcpy(&start.previous_scene_that_activates, property_previous_scene->value.string);


  if(!property_face_left)
    start.face_left = false;
  else
    start.face_left = property_face_left->value.boolean;

  if(!property_fadein_on_start)
    start.fadein_on_start = false;
  else
    start.fadein_on_start = property_fadein_on_start->value.boolean;

  if(start.fadein_on_start)
    if(property_fadein_on_start)
      start.fade_speed = property_fade_speed->value.decimal;
    else
      start.fade_speed = 0.0f;

  printf("start = %s\nactual = %s\n", start.previous_scene_that_activates, last_loaded_scene);
  if(strcmp(start.previous_scene_that_activates, &last_loaded_scene) == 0) {
    hge_vec3 ian_pos = { object->x, -object->y, 0 };
    televoidAddIanPlayer(ian_pos, start.face_left);
  }

  printf("player_start:\n\tprevious_scene_that_activates: '%s'\n\tfadein_on_start: %d\n\tfade_speed: %f\n",
  start.previous_scene_that_activates, start.fadein_on_start, start.fade_speed);
}

void ParseTMXObject(tmx_object* object) {
  //printf("(%f, %f) [%f, %f] - Interactable\n", object->x, object->y, object->width, object->height);
  printf("object type: %s\n", object->type);
  if(strcmp(object->type, "player_start") == 0) ParseTMXPlayerStart(object);
  else if(strcmp(object->type, "trigger") == 0) ParseTMXTrigger(object);
  else if(strcmp(object->type, "hotspot") == 0) ParseTMXHotspot(object);
  else if(strcmp(object->type, "item") == 0)    ParseTMXItem(object);
}

void ParseTMXData(tmx_map* map) {
  printf("Map Size: %dx%d\n", map->width, map->height);

  tmx_layer* layer = map->ly_head;
  while(layer != NULL) {
    printf("Layer Name: '%s'\n", layer->name);

    if(strcmp(layer->name, "background") == 0) {
      hge_vec3 scl = { layer->content.image->width, layer->content.image->height, 0 };
      hge_vec3 pos = { scl.x/2, -scl.y/2, -90 };
      char path[255] = "res/scenes/";
      strcat(&path, layer->content.image->source);
      printf("image path: '%s'\n", path);
      televoidAddDecoration(pos, scl, path);
    } else {
      tmx_object_group* object_group = layer->content.objgr;
      tmx_object* object = object_group->head;
      while(object != NULL) {
        ParseTMXObject(object);
        object = object->next;
      }
    }
    layer = layer->next;
  }
}

// Systems
void TriggerSystem(hge_entity* entity, hge_transform* transform, trigger* c_trigger) {
  char debug_trigger_texture[2][25] = {
    "debug_trigger_texture",
    "debug_trigger_on_texture"
  };

  int index = 0;
  hge_entity* player_ian_entity = hgeQueryEntity(1, "Player Ian");
  //printf("player hex: %x\n");
  if(player_ian_entity) {
    hge_transform* player_ian_transform = player_ian_entity->components[hgeQuery(player_ian_entity, "Transform")].data;

    if(AABB(*transform, *player_ian_transform)) {
      index = 1;
      if(!c_trigger->just_entered) {
        televoidRunScript(c_trigger->action_script);
        c_trigger->just_entered = true;
      }
    } else {
      c_trigger->just_entered = false;
    }
  }

  hgeUseShader(hgeResourcesQueryShader("sprite_shader"));
  hgeShaderSetBool(hgeResourcesQueryShader("sprite_shader"), "transparent", true);
  hgeRenderSprite(
    hgeResourcesQueryShader("sprite_shader"),
    hgeResourcesQueryTexture(debug_trigger_texture[index]),
    transform->position, transform->scale, 0.0f);
  hgeShaderSetBool(hgeResourcesQueryShader("sprite_shader"), "transparent", false);
}

void system_scenelogic(hge_entity* entity, scene_logic* scene_l) {
  if(is_loading) {
    scene_transition_chrono += hgeDeltaTime();
    if(scene_transition_chrono >= SCENE_TRANSITION_TIME) {
      load_scene(level_that_is_loading);
      scene_transition_chrono = SCENE_TRANSITION_TIME;
      is_loading = false;
    }
  } else {
    if(scene_transition_chrono > 0) {
      scene_transition_chrono -= hgeDeltaTime();
    } else {
      scene_transition_chrono = 0;
    }
  }
  hgeUseShader(hgeResourcesQueryShader("framebuffer_shader"));
  hgeShaderSetFloat(hgeResourcesQueryShader("framebuffer_shader"), "screen_alpha", 1.0 - (scene_transition_chrono / SCENE_TRANSITION_TIME));
}

// Actions
void load_scene(const char* scene_path) {
  printf("Loading TMX '%s'\n", scene_path);
  tmx_map *map = tmx_load(scene_path);
  if (!map) {
		tmx_perror("Cannot load map");
		return;
	}
  UnloadScene();
  ParseTMXData(map);
  tmx_map_free(map);

  if(!hgeQueryEntity(1, "Player Ian")) {
    hge_vec3 ian_pos = { 0, 0, 0 };
    televoidAddIanPlayer(ian_pos, false);
  }

  strcpy(&last_loaded_scene, scene_path);
  printf("Last Loaded Scene: %s\n", last_loaded_scene);
}

void televoidSceneLoad(const char* scene_path) {
  //load_scene(scene_path);
  is_loading = true;
  strcpy(level_that_is_loading, scene_path);
}

// Prefabs
void televoidAddDecoration(hge_vec3 position, hge_vec3 scale, const char* path) {
  hge_entity* decoration_entity = hgeCreateEntity();
  hge_transform transform;
  transform.position = position;
  transform.scale = scale;
  hgeAddComponent(decoration_entity, hgeCreateComponent("Transform", &transform, sizeof(transform)));
  hge_texture sprite = hgeResourcesQueryTexture(path);
  printf("Sprite ID: %d\n", sprite.id);
  if(sprite.id == -1) {
    printf("SPRITE MUST BE LOADED!\n");
    hgeResourcesLoadTexture(path, path);
    sprite = hgeResourcesQueryTexture(path);
  }

  hgeAddComponent(decoration_entity, hgeCreateComponent("Sprite", &sprite, sizeof(sprite)));

  scene_entities[num_scene_entities] = decoration_entity;
  num_scene_entities++;
}

void televoidAddItem(hge_vec3 position, hge_vec3 scale, const char* item_name) {
  if(InventoryHasItem(televoidGetItemID(item_name))) return;

  hge_entity* item_entity = hgeCreateEntity();
  hge_transform item_transform;
  item_transform.position = position;
  item_transform.scale = scale;
  hgeAddComponent(item_entity, hgeCreateComponent("Transform", &item_transform, sizeof(item_transform)));
  item_component item_c;
  //item_c.name = malloc(strlen(item_name));
  strcpy(item_c.name, item_name);
  item_c.take = false;
  hgeAddComponent(item_entity, hgeCreateComponent("Item", &item_c, sizeof(item_c)));

  scene_entities[num_scene_entities] = item_entity;
  num_scene_entities++;
}

void hotspot_default_event(){ HGE_WARNING("hotspot has no event"); }
hotspot_component* televoidAddHotspot(hge_vec3 position, hge_vec3 scale, const char* script) {
  hge_entity* hotspot_entity = hgeCreateEntity();
  hge_transform hotspot_transform;
  hotspot_transform.position = position;
  hotspot_transform.scale = scale;
  hgeAddComponent(hotspot_entity, hgeCreateComponent("Transform", &hotspot_transform, sizeof(hotspot_transform)));
  hotspot_component hotspot;
  hotspot.interaction_location.x = position.x;
  hotspot.interaction_location.y = position.y;
  hotspot.interaction_location.z = 0;
  //hotspot.event = hotspot_default_event;
  strcpy(hotspot.script, script);
  hge_component c_hotspot = hgeCreateComponent("Hotspot", &hotspot, sizeof(hotspot));
  hgeAddComponent(hotspot_entity, c_hotspot);

  scene_entities[num_scene_entities] = hotspot_entity;
  num_scene_entities++;

  return c_hotspot.data;
}

void televoidAddIanPlayer(hge_vec3 position, bool face_left) {
  // Ian Playable Character
  hge_entity* ian_character_entity = hgeCreateEntity();
  tag_component ian_tag;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Player Ian", &ian_tag, sizeof(ian_tag)));
  hge_transform ian_character_transform;
  position.y += 28.0f/2;
  ian_character_transform.position = position;
  ian_character_transform.scale.x = 28.0f;
  ian_character_transform.scale.y = 28.0f;
  ian_character_transform.scale.z = 0.0f;
  hge_component ian_character_transform_component = hgeCreateComponent("Transform", &ian_character_transform, sizeof(ian_character_transform));
  hgeAddComponent(ian_character_entity, ian_character_transform_component);
  spritesheet_component ian_character_spritesheet;
  ian_character_spritesheet.time = 0;
  ian_character_spritesheet.FPS = 6;
  ian_character_spritesheet.resolution.x = 28;
  ian_character_spritesheet.resolution.y = 28;
  ian_character_spritesheet.frame.x = 0;
  ian_character_spritesheet.frame.y = 1;
  ian_character_spritesheet.num_frames = 7;
  ian_character_spritesheet.flipped = face_left;
  ian_character_spritesheet.playing = true;
  ian_character_spritesheet.spritesheet_material.diffuse = hgeLoadTexture("res/textures/sprites/moose.png");
  ian_character_spritesheet.spritesheet_material.normal = hgeLoadTexture("res/textures/sprites/moose_normal.png");
  hgeAddComponent(ian_character_entity, hgeCreateComponent("SpriteSheet", &ian_character_spritesheet, sizeof(ian_character_spritesheet)));
  tag_component playable;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Playable", &playable, sizeof(playable)));
  character_component ian_character_component;
  ian_character_component.state = CHARACTER_IDLE;
  ian_character_component.destination = position;
  ian_character_component.speed = 20.0f;
  ian_character_component.current_hotspot = NULL;
  ian_character_component.current_item = NULL;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Character", &ian_character_component, sizeof(ian_character_component)));
  tag_component inventory;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Inventory", &inventory, sizeof(inventory)));

  scene_entities[num_scene_entities] = ian_character_entity;
  num_scene_entities++;

  // Make Camera Follow Player Position
  hge_entity* camera_entity = hgeQueryEntity(1, "ActiveCamera");
  follow_component* camera_follower = camera_entity->components[hgeQuery(camera_entity, "Follow")].data;
  hge_transform* ian_transform_pointer = ian_character_entity->components[hgeQuery(ian_character_entity, "Transform")].data;
	camera_follower->target_pos = &ian_transform_pointer->position;
  hge_vec3* camera_position = camera_entity->components[hgeQuery(camera_entity, "Position")].data;
  camera_position->x = position.x;
  camera_position->y = position.y;
}
