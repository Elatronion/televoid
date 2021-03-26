#include "Trigger.h"
#include "Scene.h"

void system_trigger_logic(hge_entity* e, hge_transform* transform, trigger_component* trigger) {
  hge_transform* player_transform_ptr = televoid_player_transform();
  if(!player_transform_ptr) return;

  hge_transform player_transform = *player_transform_ptr;
  player_transform.scale.x /= 3.f;

  if(!trigger->intersected && AABB(*transform, player_transform)) {
    printf("trigger->exec_flags : %d\n", trigger->exec_flags);
    if(trigger->exec_flags & TRIGGER_EXEC_ON_ENTER)
    televoidWrenExecuteSnippet(trigger->data);
  } else if(trigger->intersected && !AABB(*transform, player_transform)) {
    if(trigger->exec_flags & TRIGGER_EXEC_ON_EXIT)
    televoidWrenExecuteSnippet(trigger->data);
  }

  trigger->intersected = AABB(*transform, player_transform);
}

void system_trigger_renderer(hge_entity* e, hge_transform* transform, trigger_component* trigger) {
  hge_texture debug_trigger_texture = hgeResourcesQueryTexture("debug_trigger_texture");
  if(trigger->intersected) debug_trigger_texture = hgeResourcesQueryTexture("debug_trigger_texture (active)");
  hge_material rendering_material = {
    debug_trigger_texture,
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
  };

  hgeRenderSprite(
    hgeResourcesQueryShader("basic"),
    rendering_material,
    *transform
  );
}
