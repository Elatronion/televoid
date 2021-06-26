#include "Hotspot.h"
#include "GameState.h"

void televoidHotspotExecute(hotspot_component* hotspot) {
  printf("START\n");
  switch(hotspot->type) {
    case HOTSPOT_INTERACTABLE_SCRIPT:
    printf("2\n");
      televoidWrenExecute(hotspot->data);
    break;
    case HOTSPOT_INTERACTABLE_SNIPPET:
    printf("3\n");
      printf("hotspot data: \"%s\"\n", hotspot->data);
      televoidWrenExecuteSnippet(hotspot->data);
    break;
    case HOTSPOT_ITEM:
    printf("4\n");
      InventoryAddItem(televoidGetItemID(hotspot->data));
      printf("5\n");
      InventoryPrint();
      printf("6\n");
      hgeDestroyEntity(hotspot->parent);
    break;
  }
  printf("END\n");
}

void system_hotspot_renderer(hge_entity* entity, hge_transform* transform, hotspot_component* hotspot) {
  hge_material rendering_material = {
    hgeResourcesQueryTexture("debug_hotspot_texture"),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
    false,
    hgeVec4(1, 1, 1, 0.75f)
  };

  hgeRenderSprite(
    hgeResourcesQueryShader("basic"),
    rendering_material,
    *transform
  );
}

void system_item_renderer(hge_entity* entity, hge_transform* transform, hotspot_component* hotspot, hge_material* material) {
  hge_transform rendered_transform = *transform;
  float animation_offset = transform->position.x;
  float animation_x = hgeRuntime() + animation_offset;
  rendered_transform.position.y = transform->position.y + cosf(animation_x) * 3;
  rendered_transform.rotation = hgeQuaternionInitRotation(hgeVec3(0, 1, 0), 0.5f*sinf(animation_x));

  int precision = 100;
  int repeat_at_second = 10; // 15
  float animation_repeat_x = ((int)(animation_x*precision) % (repeat_at_second*precision))/(float)(precision);

  float sm_x = animation_repeat_x * 10.f;
  float x_scale_multiplier = (cosf(sm_x))*(1/(sm_x+0.5f)) + 1;
  float y_scale_multiplier = (sinf(sm_x))*(1/(sm_x+0.5f)) + 1;
  if(animation_repeat_x > 4.f) {
    x_scale_multiplier = 1.f;
    y_scale_multiplier = 1.f;
  }
  rendered_transform.scale.x = 9 * x_scale_multiplier;
  rendered_transform.scale.y = 9 * y_scale_multiplier;

  hge_material rendering_material = {
    hgeResourcesQueryTexture("debug_hotspot_texture"),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
    true,
    hgeVec4(1, 1, 1, 1)
  };

  rendering_material = *material;
  hgeRenderSprite(
    hgeResourcesQueryShader("basic"),
    rendering_material,
    rendered_transform
  );

}
