#ifndef TELEVOID_ITEM_H
#define TELEVOID_ITEM_H
#include <HGE/HGE_ResourceManager.h>
#include <HGE/HGE_Core.h>

typedef struct {
  char name[255];
} item;

typedef struct {
  char name[255];
  bool take;
} item_component;

//void televoidCleanAllItems()
void televoidGenerateAllItems();

item televoidGetItemName(int id);
int televoidGetItemID(const char* name);


// Systems
void ItemSystem(hge_entity* entity, hge_transform* transform, item_component* item_c);

#endif
