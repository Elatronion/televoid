#include "Hotspot.h"
#include "GameState.h"

void HotspotSystem(hge_entity* e, hge_transform* transform, hotspot_component* hotspot) {
  // DEBUG RENDER
  if(IsDebugMode()) {
    hgeUseShader(hgeResourcesQueryShader("sprite_shader"));
    hgeShaderSetBool(hgeResourcesQueryShader("sprite_shader"), "transparent", true);
    hge_vec3 max_z_overlay_position = transform->position;
    max_z_overlay_position.z = 100.0f;
    hgeRenderSprite(hgeResourcesQueryShader("sprite_shader"), hgeResourcesQueryTexture("debug_hotspot_texture"), max_z_overlay_position, transform->scale, 0.0f);
    hgeShaderSetBool(hgeResourcesQueryShader("sprite_shader"), "transparent", false);
  }
}
