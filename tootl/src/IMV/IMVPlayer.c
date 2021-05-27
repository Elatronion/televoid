#include "IMVPlayer.h"
#include "SafeTrash.h"
#include <HGE/HGE_MathFunc.h>
#include <HGE/HGE_FileUtility.h>
#include "Scene.h"

typedef struct imvs_t {
  imv_animation* animation;
  struct imvs_t* next;
} imv_imvs;

imv_imvs* imvs = NULL;

void imvIMVS_pushback(imv_animation* animation) {
  imv_imvs* current = imvs;
  if(!current) {
    imvs = (imv_imvs*) malloc(sizeof(imv_imvs));
    imvs->animation = animation;
    imvs->next = NULL;
    return;
  }

  while(current->next) {
    current = current->next;
  }

  current->next = (imv_imvs*) malloc(sizeof(imv_imvs));
  current->next->animation = animation;
  current->next->next = NULL;
}

void imvIMVS_remove(imv_animation* animation) {
  imv_imvs* prev = NULL;
  imv_imvs* current = imvs;
  if(!current) return;

  while(current) {
    if(current->animation == animation) {
      break;
    }
    prev = current;
    current = current->next;
  }

  if(prev) prev->next = current->next;

  if(current == imvs) {
    free(current);
    imvs = NULL;
  } else {
    free(current);
  }
}

imv_keyframe* imvElementFindLastKeyframe(imv_property* property, float timestamp) {
  imv_keyframe* current = property->root_keyframe;
  if(current)
  while(current->next) {
    if(timestamp >= current->timestamp && timestamp < current->next->timestamp) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

float imvPropertyValueAtTime(imv_property* property, float timestamp) {

  imv_keyframe* last = imvElementFindLastKeyframe(property, timestamp);
  if(!last) return property->value;
  imv_keyframe* next = last->next;
  if(!next) return property->value;


  float t = (float)(timestamp - last->timestamp) /
            (float)(next->timestamp - last->timestamp);
  float value = 0.0f;

  switch(next->type) {
    case IMV_KEYFRAME_LINEAR:
      value = hgeMathFunc_lerp(last->value, next->value, t);
    break;
    case IMV_KEYFRAME_EASEINOUT:
      value = hgeMathFunc_easeinout(last->value, next->value, t);
    break;
    case IMV_KEYFRAME_HOLD:
      value = last->value;
    break;
  }

  return value;
}

char* imvPropertyDataAtTime(imv_property* property, float timestamp) {
  char* data = malloc(DATA_STR_SIZE);
  strcpy(data, "NULL");

  imv_keyframe* last = imvElementFindLastKeyframe(property, timestamp);
  if(!last) {
    strcpy(data, property->data);
    return data;
  }

  imv_keyframe* next = last->next;
  if(!next) {
    strcpy(data, last->data);
    return data;
  }

  float t = (float)(timestamp - last->timestamp) /
            (float)(next->timestamp - last->timestamp);

  switch(next->type) {
    case IMV_KEYFRAME_HOLD:
      strcpy(data, last->data);
    break;
  }

  return data;
}

void imvUpdateAllPropertyValues(imv_element* element, float timestamp) {
  imv_property* current = element->root_property;
  while(current) {
    current->value = imvPropertyValueAtTime(current, timestamp);

    if(!strcmp(current->name, "data")) {
      char* data_at_time = imvPropertyDataAtTime(current, timestamp);
      strcpy(current->data, data_at_time);
      free(data_at_time);
    }

    current = current->next;
  }
}

void imvUpdateAllElements(imv_animation* animation) {
  imv_element* current = animation->root_element;
  while(current) {
    imvUpdateAllPropertyValues(current, animation->timestamp);
    current = current->next;
  }
}


imv_keyframe* imvCreateKeyframe(imv_keyframe_type type, float value, float timestamp) {
  imv_keyframe* keyframe = (imv_keyframe*) malloc(sizeof(imv_keyframe));
  keyframe->type = type;
  keyframe->value = value;
  strcpy(keyframe->data, "NULL");
  keyframe->timestamp = timestamp;
  keyframe->prev = NULL;
  keyframe->next = NULL;
  return keyframe;
}

imv_keyframe* imvCreateDataKeyframe(const char* data, float timestamp) {
  imv_keyframe* keyframe = imvCreateKeyframe(IMV_KEYFRAME_HOLD, 0.0f, timestamp);
  strcpy(keyframe->data, data);
  return keyframe;
}

imv_property* imvCreateProperty(const char* name, float value) {
  imv_property* property = (imv_property*) malloc(sizeof(imv_property));
  property->name = malloc(strlen(name)+1);
  strcpy(property->name, name);
  strcpy(property->data, "NULL");
  property->value = value;
  property->root_keyframe = NULL;
  property->prev = NULL;
  property->next = NULL;
  return property;
}

imv_property* imvCreateDataProperty(const char* name, const char* data) {
  imv_property* property = imvCreateProperty(name, 0.0f);
  strcpy(property->data, data);
  return property;
}

imv_element* imvCreateElement(imv_element_type type, const char* data) {
  imv_element* element = (imv_element*) malloc(sizeof(imv_element));
  element->type = type;
  element->data = malloc(strlen(data)+1);
  strcpy(element->data, data);
  element->root_property = NULL;
  element->prev = NULL;
  element->next = NULL;

  imvPushProperty(element, imvCreateProperty("x", 0.0f));
  imvPushProperty(element, imvCreateProperty("y", 0.0f));
  imvPushProperty(element, imvCreateProperty("z", 0.0f));

  imvPushProperty(element, imvCreateProperty("sx", 64.0f));
  imvPushProperty(element, imvCreateProperty("sy", 64.0f));
  imvPushProperty(element, imvCreateProperty("sz", 0.0f));

  imvPushProperty(element, imvCreateProperty("r", 0.0f));

  imvPushProperty(element, imvCreateProperty("colorR", 1.0f));
  imvPushProperty(element, imvCreateProperty("colorG", 1.0f));
  imvPushProperty(element, imvCreateProperty("colorB", 1.0f));
  imvPushProperty(element, imvCreateProperty("colorA", 1.0f));

  imvPushProperty(element, imvCreateDataProperty("data", "NULL"));

  return element;
}

imv_animation* imvCreateAnimation() {
  imv_animation* animation = (imv_animation*) malloc(sizeof(imv_animation));
  animation->timestamp = 0.0f;
  animation->root_element = NULL;
  return animation;
}

void imvCleanKeyframe(imv_keyframe* keyframe) {
  imv_element* prev = keyframe->prev;
  imv_element* next = keyframe->next;
  if(prev) prev->next = next;
  if(next) next->prev = prev;

  free(keyframe);
}

void imvCleanProperty(imv_property* property) {
  imv_keyframe* current = property->root_keyframe;
  while(current) {
    imv_keyframe* free_me = current;
    current = current->next;
    imvCleanKeyframe(free_me);
  }

  imv_element* prev = property->prev;
  imv_element* next = property->next;
  if(prev) prev->next = next;
  if(next) next->prev = prev;

  free(property->name);
  free(property);
}

void imvCleanElement(imv_element* element) {
  imv_property* current = element->root_property;
  while(current) {
    imv_property* free_me = current;
    current = current->next;
    imvCleanProperty(free_me);
  }

  imv_element* prev = element->prev;
  imv_element* next = element->next;
  if(prev) prev->next = next;
  if(next) next->prev = prev;

  free(element->data);
  free(element);
}

void imvClean(imv_animation* animation) {
  imv_element* current = animation->root_element;
  while(current) {
    imv_element* free_me = current;
    current = current->next;
    imvCleanElement(free_me);
  }

  free(animation);
}

void imvCleanAll() {
  imv_imvs* current = imvs;
  while(current) {
    imvClean(current->animation);
    current = current->next;
  }
}

void imvPushKeyframe(imv_property* property, imv_keyframe* keyframe) {
  imv_keyframe* current = property->root_keyframe;
  if(!current) {
    property->root_keyframe = keyframe;
    return;
  }

  while(current->next) {
    current = current->next;
  }

  current->next = keyframe;
  keyframe->prev = current;
}

void imvPushProperty(imv_element* element, imv_property* property) {
  imv_property* current = element->root_property;
  if(!current) {
    element->root_property = property;
    return;
  }

  while(current->next) {
    current = current->next;
  }

  current->next = property;
  property->prev = current;
}

void imvPushElement(imv_animation* animation, imv_element* element) {
  imv_element* current = animation->root_element;
  if(!current) {
    animation->root_element = element;
    return;
  }

  while(current->next) {
    current = current->next;
  }

  current->next = element;
  element->prev = current;
}

imv_property* imvPropertyWithName(imv_element* element, const char* name) {
  imv_property* property = element->root_property;
  while(property) {
    if(!strcmp(property->name, name)) {
      return property;
    }
    property = property->next;
  }

  return NULL;
}

imv_keyframe_type imvKeyframeTypeFromString(const char* string) {
  imv_keyframe_type keyframe_type = IMV_KEYFRAME_ERROR;
  if(strcmp(string, "linear") == 0) {
    keyframe_type = IMV_KEYFRAME_LINEAR;
  } else if(strcmp(string, "easeinout") == 0) {
    keyframe_type = IMV_KEYFRAME_EASEINOUT;
  } else if(strcmp(string, "hold") == 0) {
    keyframe_type = IMV_KEYFRAME_HOLD;
  }
  return keyframe_type;
}

hge_entity* imvLoad(const char* path) {
  imv_animation* animation = imvCreateAnimation();

  printf("LOADING...\n");

  FILE * file;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  file = fopen(path, "r");
  if (file == NULL)
      exit(EXIT_FAILURE);

  imv_element* element = NULL;
  char tmp[255];
  char data[255];
  char property[255];
  float x, y, z, sx, sy, sz, r;
  float timestamp, value;

  while ((read = hgeGetLine(&line, &len, file)) != -1) {
    switch(line[0]) {
      case '#':
        // '#' are comments
      break;
      case 'e':
        sscanf(line, "%*s %f %f %f %f %f %f %f %[^\n]", &x, &y, &z, &sx, &sy, &sz, &r, &data);

        strcpy(&tmp, &data);
        tmp[strlen(data)-1] = '\0';
        tmp[0] = '\0';
        for(int i = 0; i < strlen(data); i++) data[i] = tmp[i+1];

        HGE_LOG("[IMV] Creating Element with Texture \"%s\" at (%f, %f, %f) and scale (%f, %f, %f)\n", data, x, y, z, sx, sy, sz);
        element = imvCreateElement(IMV_ELEMENT_SPRITE, data);
        imvPropertyWithName(element, "x")->value = x;
        imvPropertyWithName(element, "y")->value = y;
        imvPropertyWithName(element, "z")->value = z;
        imvPropertyWithName(element, "sx")->value = sx;
        imvPropertyWithName(element, "sy")->value = sy;
        imvPropertyWithName(element, "sz")->value = sz;
        imvPushElement(animation, element);
      break;
      case 'k':
      {
        sscanf(line, "%*s %s %s %f %f", &data, &property, &timestamp, &value);
        HGE_LOG("[IMV] Creating Keyframe of type \"%s\" for property \"%s\" at timestamp %f with value %f\n", data, property, timestamp, value);

        imv_keyframe_type keyframe_type = imvKeyframeTypeFromString(data);
        imvPushKeyframe(imvPropertyWithName(element, property), imvCreateKeyframe(keyframe_type, value, timestamp));
      }
      break;
      case 'd':
      {
        sscanf(line, "%*s %f %[^\n]", &timestamp, &data);

        strcpy(&tmp, &data);
        tmp[strlen(data)-1] = '\0';
        tmp[0] = '\0';
        for(int i = 0; i < strlen(data); i++) data[i] = tmp[i+1];

        HGE_LOG("[IMV] Creating Data Keyframe at timestamp %f with value \"%s\"\n", timestamp, data);
        imvPushKeyframe(imvPropertyWithName(element, "data"), imvCreateDataKeyframe(data, timestamp));
      }
      break;
      case 'o':
      {
        char keyframe_string[255];
        sscanf(line, "%*s %s %[^\n]", &keyframe_string, &data);
        strcpy(&data[0], &data[1]);
        data[strlen(data)-1] = '\0';

        imv_keyframe_type keyframe_type = imvKeyframeTypeFromString(keyframe_string);
        element = imvCreateElement(IMV_ELEMENT_ENTITY, data);
        hge_entity* element_entity = televoidSceneGetEntity(data);
        hge_transform* entity_transform = (hge_transform*) element_entity->components[hgeQuery(element_entity, "transform")].data;
        imvPushKeyframe(imvPropertyWithName(element, "x"), imvCreateKeyframe(keyframe_type, entity_transform->position.x, 0.0f));
        imvPushKeyframe(imvPropertyWithName(element, "y"), imvCreateKeyframe(keyframe_type, entity_transform->position.y, 0.0f));
        imvPushKeyframe(imvPropertyWithName(element, "z"), imvCreateKeyframe(keyframe_type, entity_transform->position.z, 0.0f));
        imvPushElement(animation, element);
      }
      break;
      case 't':
        sx = 0;
        sz = 0;
        sscanf(line, "%*s %f %f %f %f %[^\n]", &x, &y, &z, &sy, &data);

        strcpy(&tmp, &data);
        tmp[strlen(data)-1] = '\0';
        tmp[0] = '\0';
        for(int i = 0; i < strlen(data); i++) data[i] = tmp[i+1];

        HGE_LOG("[IMV] Creating Element with Text \"%s\" at (%f, %f, %f) and scale (%f, %f, %f)\n", data, x, y, z, sx, sy, sz);
        element = imvCreateElement(IMV_ELEMENT_TEXT, data);
        imvPropertyWithName(element, "x")->value = x;
        imvPropertyWithName(element, "y")->value = y;
        imvPropertyWithName(element, "z")->value = z;
        imvPropertyWithName(element, "sx")->value = sx;
        imvPropertyWithName(element, "sy")->value = sy;
        imvPropertyWithName(element, "sz")->value = sz;
        imvPushElement(animation, element);
      break;
      case 's':
        sscanf(line, "%*s %f %[^\n]", &timestamp, &data);

        strcpy(&tmp, &data);
        tmp[strlen(data)-1] = '\0';
        tmp[0] = '\0';
        for(int i = 0; i < strlen(data); i++) data[i] = tmp[i+1];

        if(hgeFileExists(data)) {
          HGE_LOG("[IMV] Creating Script Element \"%s\"\n", data);
          element = imvCreateElement(IMV_ELEMENT_SCRIPT, data);
        } else {
          HGE_LOG("[IMV] Creating Snippet Element \"%s\"\n", data);
          element = imvCreateElement(IMV_ELEMENT_SNIPPET, data);
        }

        imvPushProperty(element, imvCreateProperty("execution timestamp", timestamp));
        imvPushElement(animation, element);

      break;
      case 'm':
        sscanf(line, "%*s %[^\n]", &data);

        strcpy(&tmp, &data);
        tmp[strlen(data)-1] = '\0';
        tmp[0] = '\0';
        for(int i = 0; i < strlen(data); i++) data[i] = tmp[i+1];

        televoidBoomboxPlayBGM(data, false);
      break;
    }
  }

  printf("LOADED!\n");

  hge_entity* imv_entity = hgeCreateEntity();
  imv_component imv = { animation };
  hgeAddComponent(imv_entity, hgeCreateComponent("imv", &imv, sizeof(imv)));
  return imv_entity;
}

float imvFurthestKeyframeTime(imv_component* imv) {
  float timestamp = 0.0f;
  imv_element* current = imv->animation->root_element;
  if(current)
  while(current) {
    imv_property* current_property = current->root_property;
    if(current_property)
    while(current_property) {
      imv_keyframe* current_keyframe = current_property->root_keyframe;
      if(current_keyframe)
      while(current_keyframe) {
        if(current_keyframe->timestamp > timestamp) {
          timestamp = current_keyframe->timestamp;
        }
        current_keyframe = current_keyframe->next;
      }
      current_property = current_property->next;
    }
    current = current->next;
  }
  return timestamp;
}

void system_imv(hge_entity* entity, imv_component* imv) {
  imv->animation->timestamp += hgeDeltaTime();

  if(imv->animation->timestamp <= imvFurthestKeyframeTime(imv)) {
    televoidSetGameState(GAME_CUTSCENE);
  } else {
    televoidSetGameState(GAME_PLAY);
    imvIMVS_remove(imv->animation);
    imvClean(imv->animation);
    hgeDestroyEntity(entity);
    return;
  }

  imvUpdateAllElements(imv->animation);

  float x, y, z, sx, sy, sz, r, colorR, colorG, colorB, colorA;
  char data[DATA_STR_SIZE];

  imv_element* current = imv->animation->root_element;
  if(current)
  while(current) {
    imv_property* current_property = current->root_property;
    if(current_property)
    while(current_property) {

      strcpy(data, "NULL");

      if(!strcmp(current_property->name, "x")) {
        x = current_property->value;
      } else if(!strcmp(current_property->name, "y")) {
        y = current_property->value;
      } else if(!strcmp(current_property->name, "z")) {
        z = current_property->value;
      } else if(!strcmp(current_property->name, "sx")) {
        sx = current_property->value;
      } else if(!strcmp(current_property->name, "sy")) {
        sy = current_property->value;
      } else if(!strcmp(current_property->name, "sz")) {
        sz = current_property->value;
      } else if(!strcmp(current_property->name, "r")) {
        r = current_property->value;
      } else if(!strcmp(current_property->name, "colorR")) {
        colorR = current_property->value;
      } else if(!strcmp(current_property->name, "colorG")) {
        colorG = current_property->value;
      } else if(!strcmp(current_property->name, "colorB")) {
        colorB = current_property->value;
      } else if(!strcmp(current_property->name, "colorA")) {
        colorA = current_property->value;
      } else if(!strcmp(current_property->name, "data")) {
        if(strcmp(current_property->data, "NULL")) {
          strcpy(data, current_property->data);
        }
      }

      current_property = current_property->next;
    }

    //printf("DATA : %s\n", data);

    hge_material element_material;
    hge_entity* element_entity;
    hge_transform* entity_transform;

    switch(current->type) {
      case IMV_ELEMENT_SPRITE:
        if(strcmp(data, "NULL")) strcpy(current->data, data);
        element_material.diffuse = hgeResourcesQueryTexture(current->data);
        element_material.normal = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");
        element_material.lit = false;
        element_material.color_multiplier = hgeVec4(colorR, colorG, colorB, colorA);
        hgeRenderSprite(
          hgeResourcesQueryShader("gui"),
          element_material,
          hgeTransform(hgeVec3(x, y, z), hgeVec3(sx, sy, sz), hgeQuaternionInitRotation(hgeVec3(0, 0, -1), hgeRadians(r)))
        );
      break;
      case IMV_ELEMENT_ENTITY:
        element_entity = televoidSceneGetEntity(current->data);
        entity_transform = (hge_transform*) element_entity->components[hgeQuery(element_entity, "transform")].data;
        entity_transform->position.x = x;
        entity_transform->position.y = y;
        entity_transform->position.z = z;
      break;
      case IMV_ELEMENT_TEXT:
        if(strcmp(data, "NULL")) strcpy(current->data, data);
        element_material.diffuse = hgeResourcesQueryTexture(current->data);
        element_material.normal = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");
        element_material.lit = false;
        element_material.color_multiplier = hgeVec4(colorR, colorG, colorB, colorA);
        hgeRenderText(
          hgeResourcesQueryShader("gui text"),
          element_material,
          hgeTransform(hgeVec3(x, y, z), hgeVec3(sx, sy, sz), hgeQuaternion(0, 0, 0, 1)),
          hgeResourcesQueryFont("VCR"),
          HGE_TEXT_ALIGNED_CENTERED,
          current->data
        );
      break;
      case IMV_ELEMENT_SCRIPT:
        if(imv->animation->timestamp > imvPropertyWithName(current, "execution timestamp")->value) {
          televoidWrenExecute(current->data);
          imvCleanElement(current);
        }
      break;
      case IMV_ELEMENT_SNIPPET:
        if(imv->animation->timestamp > imvPropertyWithName(current, "execution timestamp")->value) {
          televoidWrenExecuteSnippet(current->data);
          imvCleanElement(current);
        }
      break;
    }


    current = current->next;
  }


}
