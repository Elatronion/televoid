#include "fader_sprite.h"

void create_fader_sprite(hge_transform transform, hge_texture texture, float lifetime) {
	fader_sprite_component fader_sprite = {
		lifetime,
		lifetime,
		texture,
		false
	};
	hge_entity* entity = hgeCreateEntity();
	hgeAddComponent(entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
	hgeAddComponent(entity, hgeCreateComponent("fader sprite", &fader_sprite, sizeof(fader_sprite)));
}

void create_fader_spritesheet(hge_transform transform, hge_texture texture, hge_vec2 resolution, hge_vec2 frame, float lifetime) {
	fader_sprite_component fader_sprite = {
		lifetime,
		lifetime,
		texture,
		true,
		resolution,
		frame
	};
	hge_entity* entity = hgeCreateEntity();
	hgeAddComponent(entity, hgeCreateComponent("transform", &transform, sizeof(transform)));
	hgeAddComponent(entity, hgeCreateComponent("fader sprite", &fader_sprite, sizeof(fader_sprite)));
}

void system_fader_sprite(
	hge_entity* entity,
	hge_transform* transform,
	fader_sprite_component* fader_sprite
) {
	fader_sprite->time -= hgeDeltaTime();
	if (fader_sprite->time <= 0) {
		hgeDestroyEntity(entity);
		return;
	}
	hge_shader shader = hgeResourcesQueryShader("basic");
	hge_texture default_normal = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");

	float alpha = fader_sprite->time / fader_sprite->lifetime;

	hge_material material = {
	  fader_sprite->sprite,
	  default_normal,
	  false,
	  hgeVec4(1, 1, 1, alpha)
	};

	if (fader_sprite->is_spritesheet) {
		hgeRenderSpriteSheet(
			shader,
			material,
			*transform,
			fader_sprite->resolution,
			fader_sprite->frame
		);
	}
	else {
		hgeRenderSprite(
			shader,
			material,
			*transform
		);
	}
}
