#include "Inventory.h"
#include "GameState.h"

inventory_item items[10] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};


void InventoryPrint() {
  printf("Inventory:\n\t");
  for (int i = 0; i < 10; i++)
   printf("%d ", items[i].item_id);
  printf("\n");
}

void InventoryAddItem(int item_id) {
  for (int i = 0; i < 10; i++) {
    if(items[i].item_id == item_id) break;
    if (items[i].item_id == 0) {
      items[i].item_id = item_id;
      break;
    }
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

float gui_y = 10000.0f; // some default value below screen
void televoid_inventory_update() {
  hge_vec2 gui_inventory_resolution = { 128, 18 };
  float gui_width = fminf(hgeWindowWidth(), 800);
  hge_vec3 gui_inventory_scale = { gui_width, gui_width * (gui_inventory_resolution.y / gui_inventory_resolution.x), 0 };

  float desired_y = 0.0f;
  switch(televoidGameState()) {
    case GAME_PLAY:
      desired_y = -(float)hgeWindowHeight()/2.f + gui_inventory_scale.y/2.f;
      break;
    default:
      desired_y = -(float)hgeWindowHeight()/2.f - gui_inventory_scale.y/2.f;
      break;
  }
  gui_y += (desired_y - gui_y) * 10.f * hgeDeltaTime();

  hge_vec3 gui_inventory_position = { 0, gui_y, 0 };

  hge_material gui_inventory_material = { hgeResourcesQueryTexture("GUI Inventory"), hgeResourcesQueryTexture("HGE DEAFULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  hge_transform gui_inventory_transform = { gui_inventory_position, gui_inventory_scale, hgeQuaternion(0, 0, 0, 1) };
  hgeRenderSprite(
    hgeResourcesQueryShader("gui"),
    gui_inventory_material,
    gui_inventory_transform
  );


  hge_material gui_slot_material = { hgeResourcesQueryTexture("GUI Inventory Slot"), hgeResourcesQueryTexture("HGE DEAFULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  // left_slots
  for(int i = 0; i < 5; i++) {
    float slot_scale = (9.f/128.f) * gui_width;
    hge_transform gui_slot_transform = {
      hgeVec3(
        gui_inventory_position.x + slot_scale/2.f - (13 + i*10) * (1.f/gui_inventory_resolution.x) * gui_inventory_scale.x,
        gui_inventory_position.y - slot_scale/2.f + 4 * (1.f/gui_inventory_resolution.y) * gui_inventory_scale.y,
        1
      ),
      hgeVec3(slot_scale, slot_scale, 0),
      hgeQuaternion(0, 0, 0, 1)
    };
    hgeRenderSprite(
      hgeResourcesQueryShader("gui"),
      gui_slot_material,
      gui_slot_transform
    );

    if(items[4-i].item_id != 0) {
      item slot_item = televoidGetItemName(items[4-i].item_id);
      hge_material gui_item_material = { hgeResourcesQueryTexture(slot_item.name), hgeResourcesQueryTexture("HGE DEAFULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
      gui_slot_transform.position.z++;
      hgeRenderSprite(
        hgeResourcesQueryShader("gui"),
        gui_item_material,
        gui_slot_transform
      );

    }
  }

  // right slots
  for(int i = 0; i < 5; i++) {
    float slot_scale = (9.f/128.f) * gui_width;
    hge_transform gui_slot_transform = {
      hgeVec3(
        gui_inventory_position.x + slot_scale/2.f + (4 + i*10) * (1.f/gui_inventory_resolution.x) * gui_inventory_scale.x,
        gui_inventory_position.y - slot_scale/2.f + 4 * (1.f/gui_inventory_resolution.y) * gui_inventory_scale.y,
        1
      ),
      hgeVec3(slot_scale, slot_scale, 0),
      hgeQuaternion(0, 0, 0, 1)
    };
    hgeRenderSprite(
      hgeResourcesQueryShader("gui"),
      gui_slot_material,
      gui_slot_transform
    );
  }
}

int InventoryGetItemAtIndex(int index) {
  return items[index].item_id;
}

void InventorySetItemAtIndex(int index, int value) {
  items[index].item_id = value;
}
