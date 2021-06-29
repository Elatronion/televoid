#ifndef TELEVOID_ITEMS_H
#define TELEVOID_ITEMS_H

#include <HGE/HGE.h>

typedef struct {
	char name[255];
} item;

typedef struct {
	char name[255];
	bool take;
} item_component;

void televoidItemsClean();
void televoidGenerateAllItems();

item televoidGetItemName(int id);
int televoidGetItemID(const char* name);

// Systems
//void ItemSystem(hge_entity* entity, hge_transform* transform, item_component* item_c);

#endif
