#include "Scene.h"
#include "Character.h"
#include "tmx.h"
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef struct scene_entity_t {
  const char* name;
  hge_entity* entity;
  struct scene_entity_t* next;
} scene_entity;

//int num_scene_entities = 0;
scene_entity* root_scene_entity = NULL;
//hge_entity* scene_entities[SCENE_MAX_ENTITIES];

hge_entity* televoidSceneGetEntity(const char* name) {
  scene_entity* current = root_scene_entity;
  while(current) {
    if(!strcmp(current->name, name)) {
      return current->entity;
    }
    current = current->next;
  }
  HGE_ERROR("NO SCENE ENTITY W/ NAME \"%s\" FOUND!", name);
}

void televoidSceneAddEntity(hge_entity* entity, const char* name) {
  printf("Adding Scene Entity \"%s\"\n", name);
  if(!root_scene_entity) {
    root_scene_entity = (scene_entity*) malloc(sizeof(scene_entity));
    root_scene_entity->name = malloc(strlen(name)+1);
    strcpy(root_scene_entity->name, name);
    root_scene_entity->entity = entity;
    root_scene_entity->next = NULL;
    return;
  }
  scene_entity* current = root_scene_entity;
  while(current->next) {
    current = current->next;
  }

  current->next = (scene_entity*) malloc(sizeof(scene_entity));
  current = current->next;
  current->name = malloc(strlen(name)+1);
  strcpy(current->name, name);
  current->entity = entity;
  current->next = NULL;

  //scene_entities[num_scene_entities] = entity;
  //num_scene_entities++;
}

void televoidSceneDestroy() {
  /*
  for(int i = 0; i < num_scene_entities; i++) {
    hgeDestroyEntity(scene_entities[i]);
  }
  */

  scene_entity* current = root_scene_entity;
  while(current) {
    scene_entity* freeme = current;
    current = current->next;
    free(freeme->name);
    hgeDestroyEntity(freeme->entity);
    free(freeme);
  }
  root_scene_entity = NULL;
  //num_scene_entities = 0;
  imvCleanAll();
  televoidSetGameState(GAME_PLAY);
}

typedef struct {
  char previous_scene_that_activates[255];
  bool face_left;
  bool fadein_on_start;
  float fade_speed;
} player_start;

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

  //if(!strcmp(start.previous_scene_that_activates, &last_loaded_scene)) {
    hge_vec3 ian_pos = { object->x, -object->y, 0 };
    televoidCreateIanPlayer(ian_pos, start.face_left);
  //}

  printf("player_start:\n\tprevious_scene_that_activates: '%s'\n\tfadein_on_start: %d\n\tfade_speed: %f\n",
  start.previous_scene_that_activates, start.fadein_on_start, start.fade_speed);
}

void ParseTMXTrigger(tmx_object* object) {
  tmx_properties* properties = object->properties;
  tmx_property* property_script = tmx_get_property(object->properties, "script");
  tmx_property* property_snippet = tmx_get_property(object->properties, "snippet");
  tmx_property* property_exec_on_enter = tmx_get_property(object->properties, "exec_on_enter");
  tmx_property* property_exec_on_exit = tmx_get_property(object->properties, "exec_on_exit");

  if(!property_script && !property_snippet) {
    if(object->name) {
      HGE_ERROR("Trigger \"%s\" has no script or snippet.", object->name);
    } else {
      HGE_ERROR("Trigger has no script or snippet.", object->name);
    }
    return;
  }

  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 90 };
  hge_vec3 scl = { object->width, object->height, 90 };
  hge_transform transform = {
    hgeVec3(object->x + object->width/2, -object->y - object->height/2, 0),
    hgeVec3(object->width, object->height, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  hge_entity* trigger_entity = NULL;

  unsigned int trigger_flags = 0;

  if(property_exec_on_enter)
    trigger_flags |= (property_exec_on_enter->value.boolean ? TRIGGER_EXEC_ON_ENTER : 0);
  if(property_exec_on_exit)
    trigger_flags |= (property_exec_on_exit->value.boolean ? TRIGGER_EXEC_ON_EXIT : 0);

  if(!property_exec_on_enter && !property_exec_on_exit) {
    trigger_flags = TRIGGER_EXEC_ON_ENTER;
  }

  if(property_script) {
    if( access( property_script->value.string, F_OK ) == -1 ) {
      HGE_ERROR("File \"%s\" doesn't exists.", property_script->value.string);
      return;
    }
    trigger_entity = televoidCreateTrigger(transform, trigger_flags, property_script->value.string);
  } else if(property_snippet) {
    trigger_entity = televoidCreateTrigger(transform, trigger_flags, property_snippet->value.string);
  }
}

void ParseTMXHotspot(tmx_object* object) {
  tmx_properties* properties = object->properties;
  tmx_property* property_auto_exec = tmx_get_property(object->properties, "auto_exec");
  tmx_property* property_script = tmx_get_property(object->properties, "script");
  tmx_property* property_snippet = tmx_get_property(object->properties, "snippet");

  if(!property_script && !property_snippet) {
    if(object->name) {
      HGE_ERROR("Hotspot \"%s\" has no script or snippet.", object->name);
    } else {
      HGE_ERROR("Hotspot has no script or snippet.", object->name);
    }
    return;
  }

  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 90 };
  hge_vec3 scl = { object->width, object->height, 90 };
  hge_transform transform = {
    hgeVec3(object->x + object->width/2, -object->y - object->height/2, 0),
    hgeVec3(object->width, object->height, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  hge_entity* hotspot_entity = NULL;

  if(property_script) {
    if( access( property_script->value.string, F_OK ) == -1 ) {
      HGE_ERROR("File \"%s\" doesn't exists.", property_script->value.string);
      return;
    }
    hotspot_entity = televoidCreateHotspot(transform, transform.position, HOTSPOT_INTERACTABLE_SCRIPT, property_script->value.string);
  } else if(property_snippet) {
    hotspot_entity = televoidCreateHotspot(transform, transform.position, HOTSPOT_INTERACTABLE_SNIPPET, property_snippet->value.string);
  }

  if(property_auto_exec)
  if(property_auto_exec->value.boolean) {
    hotspot_component* hotspot = (hotspot_component*) hotspot_entity->components[hgeQuery(hotspot_entity, "hotspot")].data;
    televoidHotspotExecute(hotspot);
  }

}

void ParseTMXItem(tmx_object* object) {
  if(!object->name) {
    HGE_ERROR("Item at (%d, %d) has no name!", object->x, object->y);
    return;
  }

  if(televoidGetItemID(object->name) == 0) {
    HGE_ERROR("Item \"%s\" has no sprite...", object->name);
    return;
  }

  if(InventoryHasItem(televoidGetItemID(object->name))) return;

  tmx_properties* properties = object->properties;

  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 90 };
  hge_vec3 scl = { object->width, object->height, 90 };
  hge_transform transform = {
    hgeVec3(object->x + object->width/2, -object->y - object->height/2, 0),
    hgeVec3(object->width, object->height, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  hge_material material = {
    hgeResourcesQueryTexture(object->name),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
  };

  hge_entity* item_hotspot_entity = televoidCreateHotspot(transform, transform.position, HOTSPOT_ITEM, object->name);
  hgeAddComponent(item_hotspot_entity, hgeCreateComponent("material", &material, sizeof(material)));
}

void ParseTMXProp(tmx_object* object) {
  float depth = 0.0f;
  tmx_property* property_depth = tmx_get_property(object->properties, "depth");
  if(property_depth) depth = property_depth->value.decimal;
  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, -depth };
  hge_vec3 scl = { object->width, object->height, 0 };
  hge_transform transform;
  transform.position = pos;
  transform.scale = scl;
  transform.rotation = hgeQuaternion(0, 0, 0, 1);

  char img_path[255];
  hge_material material;
  sprintf(img_path, "res/textures/sprites/props/%s_%s.png", object->name, "diffuse");
  if( access( img_path, F_OK ) == -1 ) {
    HGE_ERROR("File \"%s\" doesn't exists.", img_path);
    return;
  }
  hge_texture texture = hgeResourcesQueryTexture(object->name);
  if(texture.id == -1) {
    hgeResourcesLoadTexture(img_path, object->name);
    texture = hgeResourcesQueryTexture(object->name);
  }
  material.diffuse = texture;
  sprintf(img_path, "res/textures/sprites/props/%s_%s.png", object->name, "normal");
  if( access( img_path, F_OK ) == -1 ) {
    HGE_WARNING("File \"%s\" doesn't exists. Using default.", img_path);
    material.normal = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");
  } else {

    char normal_name[255] = "";
    strcat(normal_name, object->name);
    strcat(normal_name, "_normal");
    printf("Can I get \"%s\"? (%s)\n", normal_name, img_path);
    hge_texture normal_texture = hgeResourcesQueryTexture(normal_name);
    if(normal_texture.id == -1) {
      hgeResourcesLoadTexture(img_path, normal_name);
      normal_texture = hgeResourcesQueryTexture(normal_name);
    }

    material.normal = normal_texture;
  }

  material.lit = false;
  material.color_multiplier = hgeVec4(1, 1, 1, 1);

  televoidCreateProp(object->name, transform, material);
}

void ParseTMXLight(tmx_object* object) {
  hge_vec3 pos = { object->x + object->width/2, -object->y - object->height/2, 15 };

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

void ParseTMXFloor(tmx_object* object) {
  hge_vec2 origin = { object->x, object->y };

  floor_t floor;
  floor.root = NULL;

  for(int i = 0; i < object->content.shape->points_len; i++) {
    hge_vec2 point = hgeVec2(object->content.shape->points[i][0], object->content.shape->points[i][1]);

    hge_vec2 position = hgeVec2(origin.x + point.x, origin.y + point.y);
    position.y = -position.y;
    HGE_LOG("[TMX FLOOR] add point (%f, %f)\n", position.x, position.y);
    floor_push_position(&floor, position);
  }

  hge_entity* floor_entity = hgeCreateEntity();
  hgeAddComponent(floor_entity, hgeCreateComponent("floor", &floor, sizeof(floor)));
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

  if(!strcmp(object->type, "player_start")) ParseTMXPlayerStart(object);
  else if(!strcmp(object->type, "hotspot")) ParseTMXHotspot(object);
  else if(!strcmp(object->type, "trigger")) ParseTMXTrigger(object);
  else if(strcmp(object->type, "item") == 0) ParseTMXItem(object);
  else if(!strcmp(object->type, "prop")) ParseTMXProp(object);
  else if(strcmp(object->type, "light") == 0) ParseTMXLight(object);
  else if(strcmp(object->type, "floor") == 0) ParseTMXFloor(object);
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
  tmx_layer* layer = map->ly_head;
  while(layer) {
    if(!layer->visible) {
      layer = layer->next;
      continue;
    }
    HGE_LOG("Parsing TMX Layer \"%s\".", layer->name);

    switch(layer->type) {
      case L_OBJGR:
        HGE_LOG("\tOBJECT LAYER");
        tmx_object_group* object_group = layer->content.objgr;
        tmx_object* object = object_group->head;
        while(object != NULL) {
          ParseTMXObject(object);
          object = object->next;
        }
      break;
      case L_IMAGE:
        HGE_LOG("\tIMAGE LAYER");
        float offset_x = layer->offsetx;
        float offset_y = layer->offsety;
        float depth = 50.f;
        tmx_property* property_depth = tmx_get_property(layer->properties, "depth");
        if(property_depth) depth = property_depth->value.decimal;

        hge_vec3 scl = { layer->content.image->width, layer->content.image->height, 0 };
        hge_vec3 pos = { scl.x/2 + offset_x, -scl.y/2 - offset_y, -depth };
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

        tmx_property* property_mesh = tmx_get_property(layer->properties, "mesh");
        if(property_mesh) {

          hge_transform transform = {
            hgeVec3(offset_x, -offset_y, depth),
            hgeVec3(8, 8, 8),
            hgeQuaternion(0, 0, 0, 1)
          };
          hge_texture sprite = hgeLoadTexture(path);
          hge_material material = { sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), lit, hgeVec4(1, 1, 1, 1) };
          televoidAddMesh(transform, property_mesh->value.string, material);

        } else {
          televoidAddBackground(pos, scl, lit, path);
        }
      break;
    }

    layer = layer->next;
  }
}

void LoadScene(const char* scene_path) {
  televoidMinigameClean();
  televoidSceneDestroy();

  televoidCreatePlayerCamera(hgeVec3(0, 0, 100));

  tmx_map *map = tmx_load(scene_path);
  if (!map) {
    tmx_perror("Cannot load map");
    const char* default_scene = "res/scenes/demo/Outside.tmx";
    HGE_ERROR("Cannot load map \"%s\"!", scene_path);
    HGE_WARNING("Loading default scene \"%s\".", default_scene);
    televoidLoadScene(default_scene);
		return;
	}

  HGE_LOG("Loading TMX \"%s\"", scene_path);
  //televoidUnloadScene();
  ParseTMXData(map, scene_path);
  tmx_map_free(map);

  //strcpy(&last_loaded_scene, scene_path);
  //HGE_LOG("Last Loaded Scene: \"%s\"", last_loaded_scene);

  if(strcmp(scene_path, "res/scenes/main_menu.tmx") != 0 &&
     strcmp(scene_path, "res/scenes/settings_menu.tmx") != 0 &&
     strcmp(scene_path, "res/scenes/splash.tmx") != 0) {
    televoidSave(scene_path);
  }
}

char* level_to_load = NULL;
void televoidLoadScene(const char* scene_path) {
  level_to_load = malloc(strlen(scene_path)+1);
  strcpy(level_to_load, scene_path);
}

void televoidSceneUpdate() {
  if(level_to_load) {
    LoadScene(level_to_load);
    free(level_to_load);
    level_to_load = NULL;
  }
}

hge_transform* televoid_player_transform() {
	hge_entity* player_entity = hgeQueryEntity(2, "playable", "character");
	hge_transform* transform = (hge_transform*)player_entity->components[hgeQuery(player_entity, "transform")].data;
	return transform;
}

hge_entity* televoidCreateIanPlayer(hge_vec3 position, bool face_left) {
  if(hgeQueryEntity(2, "character", "playable")) return;
  hge_entity* entity = hgeCreateEntity();
  character_component character = {
    CHARACTER_IDLE,
    position,
    32,
    NULL
  };
  hge_component character_component_struct = hgeCreateComponent("character", &character, sizeof(character));
  hgeAddComponent(entity, character_component_struct);
  position.y += 28/2.f;
  hge_transform transform = {
    position,
    hgeVec3(28, 28, 0),
    hgeQuaternion(0, 0, 0, 1)
  };
  hgeAddComponent(entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
  hge_material material = {
    hgeResourcesQueryTexture("moose"),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
  };
  spritesheet_component spritesheet = {
    0.0f,
    8,
    hgeVec2(28, 28),
    hgeVec2(0, 0),
    7,
    face_left,
    true,
    material
  };
  hgeAddComponent(entity, hgeCreateComponent("spritesheet", &spritesheet, sizeof(spritesheet)));
  tag_component playable;
  hgeAddComponent(entity, hgeCreateComponent("playable", &playable, sizeof(playable)));

  // Make Camera Follow Player Position
  hge_entity* camera_entity = hgeQueryEntity(2, "active", "camera");
  if(camera_entity) {
    follow_component* camera_follower = camera_entity->components[hgeQuery(camera_entity, "follower")].data;
    hge_transform* player_transform = televoid_player_transform();
    camera_follower->target_pos = &player_transform->position;
    hge_transform* camera_transform = camera_entity->components[hgeQuery(camera_entity, "transform")].data;
    camera_transform->position.x = position.x;
    camera_transform->position.y = position.y;
  }

  televoidSceneAddEntity(entity, "ian");
}

hge_entity* televoidCreatePlayerCamera(hge_vec3 position) {
  hge_transform camera_transform;
  camera_transform.position = position;
  camera_transform.scale = hgeVec3(0, 0, 0);
  camera_transform.rotation = hgeQuaternionInitRotation(hgeVec3(0, 1, 0), hgeRadians(180));
  /*hge_entity* camera_entity = hgeCreateCamera(
      camera_transform,
      hgeVec2(hgeWindowWidth(), hgeWindowHeight()),
      1,
      -100.f, 100.f,
      HGE_CAMERA_ORTHOGRAPHIC
  );*/
  hge_entity* camera_entity = hgeCreateCamera(
      camera_transform,
      hgeVec2(hgeWindowWidth(), hgeWindowHeight()),
      90,
      1, 500.f,
      NULL
  );
  tag_component activecam_tag;
  hgeAddComponent(camera_entity, hgeCreateComponent("active", &activecam_tag, sizeof(activecam_tag)));
  tag_component freemove_tag;
  hgeAddComponent(camera_entity, hgeCreateComponent("freemove", &activecam_tag, sizeof(activecam_tag)));
  mouse_picker_component mouse_picker = {
    hgeVec3(0, 0, 0)
  };
  hgeAddComponent(camera_entity, hgeCreateComponent("mouse picker", &mouse_picker, sizeof(mouse_picker)));
  follow_component follower = {
    NULL,
    2.0f,
    false, false, true
  };
  hgeAddComponent(camera_entity, hgeCreateComponent("follower", &follower, sizeof(follower)));
  televoidSceneAddEntity(camera_entity, "camera");
  return camera_entity;
}

hge_entity* televoidCreateHotspot(hge_transform transform, hge_vec3 interaction_location, hotspot_type type, const char* data) {
  hge_entity* entity = hgeCreateEntity();
  hgeAddComponent(entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
  hotspot_component hotspot;
  hotspot.type = type;
  hotspot.parent = entity;
  hotspot.interaction_location = interaction_location;
  strcpy(&hotspot.data, data);
  hgeAddComponent(entity, hgeCreateComponent("hotspot", &hotspot, sizeof(hotspot)));
  televoidSceneAddEntity(entity, "hotspot");
  return entity;
}

hge_entity* televoidCreateTrigger(hge_transform transform, unsigned int flags, const char* data) {
  hge_entity* entity = hgeCreateEntity();
  hgeAddComponent(entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
  trigger_component trigger;
  trigger.exec_flags = flags;
  trigger.intersected = false;
  strcpy(&trigger.data, data);
  hgeAddComponent(entity, hgeCreateComponent("trigger", &trigger, sizeof(trigger)));
  televoidSceneAddEntity(entity, "trigger");
  return entity;
}

void televoidAddMesh(hge_transform transform, const char* meshPath, hge_material material) {
  hge_entity* mesh_entity = hgeCreateEntity();

  hgeResourcesLoadMesh(meshPath, meshPath);
  hge_mesh mesh = hgeResourcesQueryMesh(meshPath);
  mesh.material = material;

  hgeAddComponent(mesh_entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
  hgeAddComponent(mesh_entity, hgeCreateComponent("mesh", &mesh, sizeof(mesh)));
  televoidSceneAddEntity(mesh_entity, "mesh");
}

void televoidAddBackground(hge_vec3 position, hge_vec3 scale, bool lit, const char* path) {
  hge_texture sprite = hgeLoadTexture(path);
  hge_transform transform = {
    position,
    scale,
    hgeQuaternion(0, 0, 0, 1)
  };
  hge_material material = { sprite, hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), lit, hgeVec4(1, 1, 1, 1) };
  televoidCreateProp("background", transform, material);
}

void televoidCreateProp(const char* name, hge_transform prop_transform, hge_material prop_material) {
  hge_entity* prop_entity = hgeCreateEntity();
  hgeAddComponent(prop_entity, hgeCreateComponent("transform", &prop_transform, sizeof(prop_transform)));
  hgeAddComponent(prop_entity, hgeCreateComponent("material", &prop_material, sizeof(prop_material)));

  prop_component prop;
  hgeAddComponent(prop_entity, hgeCreateComponent("prop", &prop, sizeof(prop)));
  televoidSceneAddEntity(prop_entity, name);
}

hge_entity* televoidCreateDialogue(const char* file) {
  hge_entity* dialogue_entity = hgeCreateEntity();
  dialogue_event_node* head = dialogue_load(file);
  dialogue_component dialogue = {
    head, head,
    hgeResourcesQueryTexture("FRANK IDLE"),
    hgeResourcesQueryTexture("FRANK IDLE")
  };
  hgeAddComponent(dialogue_entity, hgeCreateComponent("dialogue", &dialogue, sizeof(dialogue)));
  televoidSceneAddEntity(dialogue_entity, "dialogue");
}

hge_entity* televoidIMVCreate(const char* path) {
  hge_entity* imv_entity = imvLoad(path);
  televoidSceneAddEntity(imv_entity, "IMV");
  return imv_entity;
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
  televoidSceneAddEntity(light_entity, "dirlight");
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
  televoidSceneAddEntity(light_entity, "pointlight");
}
