#include "Inventory.h"
#include "GameState.h"

item items[10] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};

void InventoryPrint() {
  printf("Inventory:\n\t");
  for (int i = 0; i < 10; i++)
   printf("%d ", items[i].item_id);
  printf("\n");
}

void InventoryAddItem(int item_id) {
  for (int i = 0; i < 10; i++)
    if (items[i].item_id == 0) {
      items[i].item_id = item_id;
      break;
    }
}

void InventoryRemoveItem(int item_id) {
  for (int i = 0; i < 10; i++)
    if (items[i].item_id == item_id) {
      items[i].item_id = 0;
      break;
    }
}

bool InventoryHasItem(int item_id) {
  for (int i = 0; i < 10; i++)
    if(items[i].item_id == item_id)
      return true;
  return false;
}

float gui_y = 0.0f;
void InventorySystem(hge_entity* entity, tag_component* inventory) {

  float aspect = (float)hgeWindowWidth() / (float)hgeWindowHeight();
  float x_percent = 100.f/100.f;
  float y_percent = (0.140625 * hgeWindowWidth()) / (hgeWindowHeight());

  hge_vec3 scale = { x_percent*2, y_percent*2, 0 };

  float desired_y = 0.0f;
  switch(GetGameState()) {
    case GAME_PLAY:
      desired_y = -1 + scale.y/2;
      break;
    default:
      desired_y = -1 - scale.y/2;
      break;
  }
  gui_y += (desired_y - gui_y) * 10.f * hgeDeltaTime();
  hge_vec3 position = { 0, gui_y, -0.9f };

  hgeRenderSprite(
    hgeResourcesQueryShader("gui_shader"),
    hgeResourcesQueryTexture("GUI Inventory"),
    position, scale, 0.0f);

  float slot_width = 0.0703f;
  hge_vec3 slot_scale = { 2 * slot_width, 2 * (slot_width*hgeWindowWidth())/hgeWindowHeight(), 1 };
  float slot_y = position.y - scale.y/2 + slot_scale.y/2
  + (2 * (0.03125*hgeWindowWidth())/hgeWindowHeight());
  // Left Half
  for(int i = 0; i < 5; i++) {
    // 0.046875
    float offset = i * (slot_width*2 + 0.0234375);
    hge_vec3 slot_position = {
      -1 + slot_width + 0.09375 + offset,
      slot_y,
      -1 };
    hgeRenderSprite(
      hgeResourcesQueryShader("gui_shader"),
      hgeResourcesQueryTexture("GUI Inventory Slot"),
      slot_position, slot_scale, 0.0f);
  }
  // Right Half
  for(int i = 0; i < 5; i++) {
    // 0.046875
    float offset = i * (slot_width*2 + 0.0234375);
    hge_vec3 slot_position = {
      -1 + slot_width + 1.109375 + offset,
      slot_y,
      -1 };
    hgeRenderSprite(
      hgeResourcesQueryShader("gui_shader"),
      hgeResourcesQueryTexture("GUI Inventory Slot"),
      slot_position, slot_scale, 0.0f);
  }
}
