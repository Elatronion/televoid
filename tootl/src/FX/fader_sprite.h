#ifndef FADER_SPRITE_H
#define FADER_SPRITE_H
#include <HGE/HGE_Core.h>

typedef struct {
  float time;
  float lifetime;
  hge_texture sprite;
  bool is_spritesheet;
  hge_vec2 resolution;
  hge_vec2 frame;
} fader_sprite_component;

void create_fader_sprite(hge_transform transform, hge_texture texture, float lifetime);
void create_fader_spritesheet(hge_transform transform, hge_texture texture, hge_vec2 resolution, hge_vec2 frame, float lifetime);

void system_fader_sprite(
                          hge_entity* entity,
                          hge_transform* transform,
                          fader_sprite_component* fader_sprite
                        );

#endif
