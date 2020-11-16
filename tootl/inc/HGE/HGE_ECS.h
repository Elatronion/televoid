#ifndef HGE_ECS_H
#define HGE_ECS_H

#include <stddef.h>

#define HGE_MAX_ECS_ENTITIES 100000
#define HGE_MAX_ECS_COMPONENTS 10						// Can't Change Just Now, Because Of Trash hgeWorldUpdate
#define HGE_MAX_ECS_DEPENDENT_COMPONENTS 10	// Can't Change Just Now, Because Of Trash hgeWorldUpdate
#define HGE_MAX_ECS_SYSTEMS 1000
#define HGE_MAX_ECS_WORLDS 10

typedef struct {
	const char* name;
	void* data;
} hge_component;

typedef struct {
	int numComponents;
	hge_component components[HGE_MAX_ECS_COMPONENTS];
} hge_entity;

typedef struct {
	void (*function)();
	int argc;
	const char * dependentComponents[HGE_MAX_ECS_DEPENDENT_COMPONENTS];
} hge_system;

typedef struct {
	int NUM_ENTITIES;
	int NUM_SYSTEMS;
	hge_system* systems;
	hge_entity** entities;
} hge_world;

typedef struct {
	int NUM_ENTITIES;
	hge_entity* entities[HGE_MAX_ECS_ENTITIES];
} hge_ecs_request;

void hgeDestroyEntity(hge_entity* entity);
hge_entity* hgeCreateEntity();
hge_component hgeCreateComponent(const char* name, void* componentData, size_t size);

int hgeQuery(hge_entity* entity, const char* component_name);
hge_entity* hgeQueryEntity(int argc, ...);

hge_ecs_request hgeECSRequest(int argc, ...);

void hgeAddComponent(hge_entity* entity, hge_component component);
void hgeAddSystem(void (*function)(), int argc, ...);

void hgeWorldUpdate();

void hgePrintECSInfo();

void hgeECSInit();

void hgeECSCleanUp();

#endif
