#ifndef INVENTORY_H
#define INVENTORY_H

#include <HGE/HGE_Core.h>

typedef struct {
  int item_id;
} item;

void InventorySystem(hge_entity* entity, tag_component* inventory);

void InventoryPrint();
void InventoryAddItem(int item_id);
void InventoryRemoveItem(int item_id);
bool InventoryHasItem(int item_id);

#endif
