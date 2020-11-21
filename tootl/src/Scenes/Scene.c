#include "Scene.h"
#include "Character.h"
#include "tmx.h"
#include <stdint.h>
#include <unistd.h>

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

void ParseTMXProp(tmx_object* object) {
  float depth = 0.0f;
  tmx_property* property_depth = tmx_get_property(object->properties, "depth");
  if(property_depth) depth = property_depth->value.decimal;
  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, depth };
  hge_vec3 scl = { object->width, object->height, 0 };
  hge_transform transform;
  transform.position = pos;
  transform.scale = scl;


  char img_path[255];
  hge_material material;
  sprintf(img_path, "res/textures/sprites/props/%s_%s.png", object->name, "diffuse");
  if( access( img_path, F_OK ) == -1 ) {
    HGE_ERROR("File \"%s\" doesn't exists.", img_path);
    return;
  }
  material.diffuse = hgeLoadTexture(img_path);
  sprintf(img_path, "res/textures/sprites/props/%s_%s.png", object->name, "normal");
  if( access( img_path, F_OK ) == -1 ) {
    const char* default_normal = "res/textures/HGE/DEFAULT NORMAL.png";
    HGE_WARNING("File \"%s\" doesn't exists. Using default \"%s\".", img_path, default_normal);
    material.normal = hgeLoadTexture(default_normal);
  } else {
    material.normal = hgeLoadTexture(img_path);
  }
  televoidAddProp(transform, material);
}

void ParseTMXLight(tmx_object* object) {
  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 2 };

  tmx_property* property_diffuse = tmx_get_property(object->properties, "diffuse");
  tmx_property* property_ambient = tmx_get_property(object->properties, "ambient");
  tmx_property* property_dir_x = tmx_get_property(object->properties, "dir_x");
  tmx_property* property_dir_y = tmx_get_property(object->properties, "dir_y");
  tmx_property* property_dir_z = tmx_get_property(object->properties, "dir_z");
  if(!property_diffuse) {
    if(object->name) {
      HGE_ERROR("Light \"%s\" is missing light color property 'diffuse'.", object->name);
    } else {
      HGE_ERROR("Light is missing light color property 'diffuse'.");
    }
  }

  if(!property_ambient) {
    if(object->name) {
      HGE_ERROR("Light \"%s\" is missing light color property 'ambient'.", object->name);
    } else {
      HGE_ERROR("Light is missing light color property 'ambient'.");
    }
  }

  if(!property_diffuse || !property_ambient) return;

  tmx_col_floats diffuse_argb = tmx_col_to_floats(property_diffuse->value.color);
  tmx_col_floats ambient_argb = tmx_col_to_floats(property_ambient->value.color);
  hge_vec3 diffuse_vec = { diffuse_argb.r, diffuse_argb.g, diffuse_argb.b };
  hge_vec3 ambient_vec = { ambient_argb.r, ambient_argb.g, ambient_argb.b };

  //printf("AMBIENT: (%f, %f, %f)\n", ambient_vec.x, ambient_vec.y, ambient_vec.z);

  if(property_dir_x && property_dir_y && property_dir_z) {
    televoidAddDirLight(hgeVec3(property_dir_x->value.decimal, property_dir_y->value.decimal, property_dir_z->value.decimal), diffuse_vec, ambient_vec);
  } else {
    televoidAddPointLight(pos, diffuse_vec, ambient_vec);
  }
}

void ParseTMXItem(tmx_object* object) {
  if(!object->name) {
    HGE_ERROR("Item has no name.");
    return;
  }

  char item_path[255] = "";
  sprintf(&item_path, "res/textures/inventory/items/%s.png", object->name);
  if( access( item_path, F_OK ) == -1 ) {
    HGE_ERROR("File \"%s\" doesn't exists.", item_path);
    return;
  }

  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 0.5f };
  hge_vec3 scl = { object->width, object->height, 90 };
  televoidAddItem(pos, scl, object->name);
}

void ParseTMXHotspot(tmx_object* object) {
  trigger trigger_component;
  tmx_properties* properties = object->properties;
  tmx_property* property_action_script = tmx_get_property(object->properties, "action_script");
  if(!property_action_script) {
    if(object->name) {
      HGE_ERROR("Hotspot \"%s\" has no action script.", object->name);
    } else {
      HGE_ERROR("Hotspot has no action script.", object->name);
    }
    return;
  }
  char script_path[255];
  strcpy(script_path, property_action_script->value.string);
  if( access( script_path, F_OK ) == -1 ) {
    HGE_ERROR("File \"%s\" doesn't exists.", script_path);
    return;
  }
  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 90 };
  hge_vec3 scl = { object->width, object->height, 90 };
  televoidAddHotspot(pos, scl, script_path);
}

void ParseTMXTrigger(tmx_object* object) {
  trigger trigger_component;
  tmx_properties* properties = object->properties;
  tmx_property* property_action_script = tmx_get_property(object->properties, "action_script");
  if(!property_action_script) {
    if(object->name) {
      HGE_ERROR("Hotspot \"%s\" has no action script.", object->name);
    } else {
      HGE_ERROR("Hotspot has no action script.", object->name);
    }
    return;
  }
  strcpy(&trigger_component.action_script, property_action_script->value.string);
  if( access( trigger_component.action_script, F_OK ) == -1 ) {
    HGE_ERROR("File \"%s\" doesn't exists.", trigger_component.action_script);
    return;
  }
  //printf("trigger:\n\taction_script: '%s'\n", trigger_component.action_script);
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
    HGE_WARNING("No previous scene property.");
    strcpy(&start.previous_scene_that_activates, "");
  } else {
    strcpy(&start.previous_scene_that_activates, property_previous_scene->value.string);
  }

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
  if(!object->type) {
    if(object->name) {
      HGE_ERROR("TMX Object With Name \"%s\" Has No Type!", object->name);
    } else {
      HGE_ERROR("TMX Object Has No Type!");
    }
    return;
  }

  if(object->name && object->type) {
    HGE_LOG("Parsing TMX Object \"%s\" type \"%s\"...", object->name, object->type);
  } else if(object->name && !object->type) {
    HGE_LOG("Parsing TMX Object \"%s\"...", object->name);
  } else if(!object->name && object->type) {
    HGE_LOG("Parsing TMX Object type \"%s\"...", object->type);
  } else {
    HGE_LOG("Parsing TMX Object ...", object->type);
  }

  if(strcmp(object->type, "player_start") == 0) ParseTMXPlayerStart(object);
  else if(strcmp(object->type, "trigger") == 0) ParseTMXTrigger(object);
  else if(strcmp(object->type, "hotspot") == 0) ParseTMXHotspot(object);
  else if(strcmp(object->type, "item") == 0)    ParseTMXItem(object);
  else if(strcmp(object->type, "light") == 0)   ParseTMXLight(object);
  else if(strcmp(object->type, "prop") == 0)    ParseTMXProp(object);
}

void strip_filename(char *fname)
{
    char *end = fname + strlen(fname);

    while (end > fname && *end != '/') {
        --end;
    }

    if (end > fname) {
        *end = '\0';
    }
}

void ParseTMXData(tmx_map* map, const char* scene_path) {
  printf("Map Size: %dx%d\n", map->width, map->height);

  tmx_layer* layer = map->ly_head;
  while(layer) {
    //printf("Layer Name: '%s'\n", layer->name);
    HGE_LOG("Parsing TMX Layer \"%s\".", layer->name);

    if(strcmp(layer->name, "background") == 0) {
      hge_vec3 scl = { layer->content.image->width, layer->content.image->height, 0 };
      hge_vec3 pos = { scl.x/2, -scl.y/2, -200 };
      char path[255] = "";
      strcat(&path, scene_path);
      strip_filename(&path);
      strcat(&path, "/");
      strcat(&path, layer->content.image->source);
      printf("image path: '%s'\n", path);
      tmx_property* property_lit = tmx_get_property(layer->properties, "lit");
      bool lit = false;
      if(property_lit)
        lit = property_lit->value.boolean;
      televoidAddBackground(pos, scl, lit, path);
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
  hge_material material = { hgeResourcesQueryTexture(debug_trigger_texture[index]), hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  hgeRenderSprite(
    hgeResourcesQueryShader("sprite_shader"),
    material,
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
  tmx_map *map = tmx_load(scene_path);
  if (!map) {
    tmx_perror("Cannot load map");
    const char* default_scene = "res/scenes/demo/Outside.tmx";
    HGE_ERROR("Cannot load map \"%s\"!", scene_path);
    HGE_WARNING("Loading default scene \"%s\".", default_scene);
    load_scene(default_scene);
		return;
	}
  HGE_LOG("Loading TMX \"%s\"", scene_path);
  UnloadScene();
  ParseTMXData(map, scene_path);
  tmx_map_free(map);

  if(!hgeQueryEntity(1, "Player Ian")) {
    hge_vec3 ian_pos = { 0, 0, 0 };
    televoidAddIanPlayer(ian_pos, false);
  }

  strcpy(&last_loaded_scene, scene_path);
  HGE_LOG("Last Loaded Scene: \"%s\"", last_loaded_scene);
}

void televoidSceneLoad(const char* scene_path) {
  //load_scene(scene_path);
  is_loading = true;
  strcpy(level_that_is_loading, scene_path);
}

// Prefabs
void televoidAddProp(hge_transform prop_transform, hge_material prop_material) {
  hge_entity* prop_entity = hgeCreateEntity();
  hgeAddComponent(prop_entity, hgeCreateComponent("transform", &prop_transform, sizeof(prop_transform)));
  hgeAddComponent(prop_entity, hgeCreateComponent("material", &prop_material, sizeof(prop_material)));
  scene_entities[num_scene_entities] = prop_entity;
  num_scene_entities++;
}

void televoidAddBackground(hge_vec3 position, hge_vec3 scale, bool lit, const char* path) {
  hge_entity* decoration_entity = hgeCreateEntity();
  hge_transform transform;
  transform.position = position;
  transform.scale = scale;
  hgeAddComponent(decoration_entity, hgeCreateComponent("transform", &transform, sizeof(transform)));

  hge_texture sprite = hgeLoadTexture(path); //hgeResourcesQueryTexture(path);

  hge_material material = { sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL") };
  background bg = { material, lit };
  hgeAddComponent(decoration_entity, hgeCreateComponent("background", &bg, sizeof(bg)));

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

void televoidAddDirLight(hge_vec3 direction, hge_vec3 ambient, hge_vec3 diffuse) {
  hge_entity* light_entity = hgeCreateEntity();
  hge_dirlight light = hgeDirLight(
    ambient,
    diffuse,
    hgeVec3(0, 0, 0),
    direction
  );
  hgeAddComponent(light_entity,
  hgeCreateComponent("dirlight", &light, sizeof(light)));

  scene_entities[num_scene_entities] = light_entity;
  num_scene_entities++;
}

void televoidAddPointLight(hge_vec3 position, hge_vec3 ambient, hge_vec3 diffuse) {
  hge_entity* light_entity = hgeCreateEntity();
  hge_vec3 light_position = position;
  hgeAddComponent(light_entity,
  hgeCreateComponent("position", &light_position, sizeof(light_position)));
  hge_pointlight light = hgePointLight(
    ambient,
    diffuse,
    hgeVec3(0, 0, 0), // Specular
    1.0f,
    0.007f/3,  //0.007f/2 0.0014f,
    0.0002f/3  //0.0002f/2 0.000007f
  );
  /*light.ambient = hgeVec3(color.x * 0.1f, color.y * 0.1f, color.z * 0.1f);
  light.diffuse = color;
  light.specular = light.diffuse;

  light.constant = 1.0f;
  light.linear =    0.0014f;
  light.quadratic = 0.000007f;*/
  hgeAddComponent(light_entity,
  hgeCreateComponent("pointlight", &light, sizeof(light)));

  scene_entities[num_scene_entities] = light_entity;
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
  ian_character_spritesheet.spritesheet_material.diffuse = hgeResourcesQueryTexture("moose");
  ian_character_spritesheet.spritesheet_material.normal = hgeResourcesQueryTexture("moose normal");
  hgeAddComponent(ian_character_entity, hgeCreateComponent("SpriteSheet", &ian_character_spritesheet, sizeof(ian_character_spritesheet)));
  tag_component playable;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Playable", &playable, sizeof(playable)));
  character_component ian_character_component;
  ian_character_component.state = CHARACTER_IDLE;
  ian_character_component.destination = position;
  ian_character_component.speed = 25.f; //20.0f;
  ian_character_component.current_hotspot = NULL;
  ian_character_component.current_item = NULL;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Character", &ian_character_component, sizeof(ian_character_component)));
  tag_component inventory;
  hgeAddComponent(ian_character_entity, hgeCreateComponent("Inventory", &inventory, sizeof(inventory)));

  scene_entities[num_scene_entities] = ian_character_entity;
  num_scene_entities++;

  // Make Camera Follow Player Position
  hge_entity* camera_entity = hgeQueryEntity(2, "ActiveCamera", "Follow");
  if(camera_entity) {
  follow_component* camera_follower = camera_entity->components[hgeQuery(camera_entity, "Follow")].data;
    hge_transform* ian_transform_pointer = ian_character_entity->components[hgeQuery(ian_character_entity, "Transform")].data;
  	camera_follower->target_pos = &ian_transform_pointer->position;
    hge_vec3* camera_position = camera_entity->components[hgeQuery(camera_entity, "Position")].data;
    camera_position->x = position.x;
    camera_position->y = position.y;
  }
}
