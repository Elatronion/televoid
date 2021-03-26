#include "Hotspot.h"
#include "GameState.h"

void televoidHotspotExecute(hotspot_component* hotspot) {
  switch(hotspot->type) {
    case HOTSPOT_INTERACTABLE_SCRIPT:
      televoidWrenExecute(hotspot->data);
    break;
    case HOTSPOT_INTERACTABLE_SNIPPET:
      televoidWrenExecuteSnippet(hotspot->data);
    break;
    case HOTSPOT_ITEM:
      printf("Pick up item \"%s\"\n", hotspot->data);
      InventoryAddItem(televoidGetItemID(hotspot->data));
      InventoryPrint();
      hgeDestroyEntity(hotspot->parent);
    break;
  }
}

void system_hotspot_renderer(hge_entity* entity, hge_transform* transform, hotspot_component* hotspot) {
  hge_material rendering_material = {
    hgeResourcesQueryTexture("debug_hotspot_texture"),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
  };

  int material_id = hgeQuery(entity, "material");
  if(material_id != -1) {
    hge_material* item_material = entity->components[material_id].data;
    rendering_material = *item_material;
  }
  hgeRenderSprite(
    hgeResourcesQueryShader("basic"),
    rendering_material,
    *transform
  );
}
