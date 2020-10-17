#include "Items.h"

item items_db[TELEVOID_MAX_ITEMS];
int item_db_index;

void GenerateItem(const char* item_name) {
  item new_item = { item_name };
  items_db[item_db_index] = new_item;
  item_db_index++;
}

void LoadAllItemSprites() {
  for(int i = 0; i < TELEVOID_MAX_ITEMS; i++) {
    char path[100] = "res/textures/inventory/items/";
    strcat(path, items_db[i].name);
    strcat(path, ".png");
    printf("Loading '%s'\n", path);
    hgeResourcesLoadTexture(path, items_db[i].name);
  }
}

void televoidGenerateAllItems() {
  GenerateItem("key");

  LoadAllItemSprites();
}

item televoidGetItemName(int id) {
  id--;
  item fake_item = { "GUI Inventory Slot" };
  if(id < 0) return fake_item;
  return items_db[id];
}

int televoidGetItemID(const char* name) {
  for(int i = 0; i < TELEVOID_MAX_ITEMS; i++) {
    if(strcmp(items_db[i].name, name) == 0) {
      return i+1;
    }
  }
  return 0;
}


// Systems
void ItemSystem(hge_entity* entity, hge_transform* transform, item_component* item_c) {
  hgeRenderSprite(
    hgeResourcesQueryShader("sprite_shader"),
    hgeResourcesQueryTexture(item_c->name),
    transform->position, transform->scale, 0.0f
  );

  if(item_c->take) {
    InventoryAddItem(televoidGetItemID(item_c->name));
    hgeDestroyEntity(entity);
  }
}
