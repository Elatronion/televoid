#ifndef INVENTORY_H
#define INVENTORY_H

#include <HGE/HGE.h>

#include "Items.h"

typedef struct {
	int item_id;
} inventory_item;

void televoid_inventory_update();
//void InventorySystem(hge_entity* entity, tag_component* inventory);
void InventoryPrint();
void InventoryAddItem(int item_id);
void InventoryRemoveItem(int item_id);
bool InventoryHasItem(int item_id);

int InventoryGetItemAtIndex(int index);
void InventorySetItemAtIndex(int index, int value);

#endif
