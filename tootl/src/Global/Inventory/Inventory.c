#include "Inventory.h"
#include "GameState.h"
#include <HGE/HGE_GameMath.h>
#include "MousePicker.h"
#include <math.h>

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

float gui_y = -10000.0f; // some default value below screen
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
  float max_height = (float)(-(float)(hgeWindowHeight()) / 2.f) - (float)(gui_y);
  if(gui_y > max_height) gui_y = max_height;

  hge_vec3 gui_inventory_position = { 0, gui_y, 0 };

  hge_material gui_inventory_material = { hgeResourcesQueryTexture("GUI Inventory"), hgeResourcesQueryTexture("HGE DEAFULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  hge_transform gui_inventory_transform = { gui_inventory_position, gui_inventory_scale, hgeQuaternion(0, 0, 0, 1) };
  hgeRenderSprite(
    hgeResourcesQueryShader("gui"),
    gui_inventory_material,
    gui_inventory_transform
  );

  int hovered_item_index = -1;
  hge_transform mouse_transform = mouseGUITransform();

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

    if(AABB(mouse_transform, gui_slot_transform)) {
      hovered_item_index = 4-i;
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

    if(items[i+5].item_id != 0) {
      item slot_item = televoidGetItemName(items[i+5].item_id);
      hge_material gui_item_material = { hgeResourcesQueryTexture(slot_item.name), hgeResourcesQueryTexture("HGE DEAFULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
      gui_slot_transform.position.z++;
      hgeRenderSprite(
        hgeResourcesQueryShader("gui"),
        gui_item_material,
        gui_slot_transform
      );

    }

    if(AABB(mouse_transform, gui_slot_transform)) {
      hovered_item_index = i + 5;
    }
  }

  // action/item text
  if (televoidGameState() != GAME_PLAY) return;
  float font_size = 0.45f;
  hge_material text_material;
  text_material.color_multiplier = hgeVec4(1, 1, 1, 1);


  hge_transform text_transform = {
    hgeVec3(0, gui_y + 60, 99),
    hgeVec3(font_size, font_size, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  int hovered_item_id;
  item hovered_item;
  if(hovered_item_index >= 0) {
    hovered_item_id = InventoryGetItemAtIndex(hovered_item_index);
    hovered_item = televoidGetItemName(hovered_item_id);
  }
  if(hovered_item_id != 0 && hovered_item_index >= 0)
  hgeRenderText(
    hgeResourcesQueryShader("gui text"),
    text_material,
    text_transform,
    hgeResourcesQueryFont("VCR"),
    HGE_TEXT_ALIGNED_CENTERED,
    hovered_item.name
  );
}

int InventoryGetItemAtIndex(int index) {
  return items[index].item_id;
}

void InventorySetItemAtIndex(int index, int value) {
  items[index].item_id = value;
}
