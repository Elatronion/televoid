#ifndef IMV_PLAYER_H
#define IMV_PLAYER_H
#include <HGE/HGE_Core.h>
#include "GameState.h"

#define DATA_STR_SIZE 100

typedef enum {
  IMV_KEYFRAME_ERROR,
  IMV_KEYFRAME_LINEAR,
  IMV_KEYFRAME_EASEINOUT,
  IMV_KEYFRAME_HOLD
} imv_keyframe_type;

typedef struct imv_keyframe_t {
  imv_keyframe_type type;
  char data[DATA_STR_SIZE];
  float value;
  float timestamp;
  struct imv_keyframe_t* prev;
  struct imv_keyframe_t* next;
} imv_keyframe;

typedef struct imv_property_t {
  const char* name;
  char data[DATA_STR_SIZE];
  float value;
  imv_keyframe* root_keyframe;
  struct imv_property_t* prev;
  struct imv_property_t* next;
} imv_property;

typedef enum {
  IMV_ELEMENT_SPRITE,
  IMV_ELEMENT_ENTITY,
  IMV_ELEMENT_TEXT,
  IMV_ELEMENT_SCRIPT,
  IMV_ELEMENT_SNIPPET
} imv_element_type;

typedef struct imv_element_t {
  imv_element_type type;
  const char* data;
  imv_property* root_property;
  struct imv_element_t* prev;
  struct imv_element_t* next;
} imv_element;

typedef struct {
  float timestamp;
  imv_element* root_element;
} imv_animation;


typedef struct {
  imv_animation* animation;
} imv_component;

void imvPushProperty(imv_element* element, imv_property* property);
hge_entity* imvLoad(const char* path);

void system_imv(hge_entity* entity, imv_component* imv);

#endif
