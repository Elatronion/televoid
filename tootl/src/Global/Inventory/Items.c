#include "Items.h"
#include "dirent.h"

typedef struct s_item_db {
  item item_element;
  struct s_item_db* next;
} item_db;

item_db* item_db_root = NULL;

void clean_item_db() {
  item_db* cur = item_db_root;
  while(cur) {
    item_db* idb_tofree = cur;
    cur = cur->next;
    free(idb_tofree);
  }
}

void pushback_item_db(item item_element) {
  item_db* cur = item_db_root;
  while(cur->next) cur = cur->next;
  cur->item_element = item_element;
  cur->next = malloc(sizeof(item_db));
  cur->next->next = NULL;
}

item get_item_db(int index) {
  int i = 0;
  item_db* cur = item_db_root;
  while(i < index) {
    i++;
    cur = cur->next;
  }
  return cur->item_element;
}

void televoidItemsClean() { clean_item_db(); }

void GenerateItem(const char* item_name) {
  item new_item;
  strcpy(new_item.name, item_name);
  pushback_item_db(new_item);
}

void LoadAllItemSprites() {
  int i = 0;
  item_db* cur = item_db_root;
  while(cur->next) {
    char path[100] = "res/textures/sprites/inventory/items/";
    strcat(path, cur->item_element.name);
    strcat(path, ".png");
    printf("Loading '%s'\n", path);
    hgeResourcesLoadTexture(path, cur->item_element.name);
    cur = cur->next;
  }
}

void televoidGenerateAllItems() {

  if(item_db_root) free(item_db_root);
  item_db_root = malloc(sizeof(item_db));
  item_db_root->next = NULL;


  DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("res/textures/sprites/inventory/items")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			char *dot = strrchr(ent->d_name, '.');
			if (dot && !strcmp(dot, ".png")) {
        //char item_name[255];
        //strcpy(item_name, ent->d_name);
        ent->d_name[strlen(ent->d_name)-4] = '\0';
        HGE_LOG("generated item %s", ent->d_name);
        GenerateItem(ent->d_name);
      }
		}
		closedir (dir);
	} else {
		return EXIT_FAILURE;
	}



  LoadAllItemSprites();

}


item televoidGetItemName(int id) {
  id--;
  item fake_item = { "GUI Inventory Slot" };
  if(id < 0) return fake_item;
  return get_item_db(id);
}

int televoidGetItemID(const char* name) {
  int i = 0;
  item_db* cur = item_db_root;
  while(cur) {
    if(!strcmp(cur->item_element.name, name)) {
      return i+1;
    }
    i++;
    cur = cur->next;
  }
  return 0;
}


// Systems
/*
void ItemSystem(hge_entity* entity, hge_transform* transform, item_component* item_c) {
  hge_shader sprite_shader = hgeResourcesQueryShader("sprite_shader");
  hgeUseShader(sprite_shader);

  hge_material material = { hgeResourcesQueryTexture(item_c->name), hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), true, hgeVec4(1, 1, 1, 1) };
  hgeRenderSprite(
    hgeResourcesQueryShader("sprite_shader"),
    material,
    *transform
  );

  if(item_c->take) {
    InventoryAddItem(televoidGetItemID(item_c->name));
    hgeDestroyEntity(entity);
  }
}
*/
