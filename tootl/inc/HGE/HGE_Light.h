#ifndef HGE_LIGHT_H
#define HGE_LIGHT_H

#include "HGE_ECS.h"
#include "HGE_Math3D.h"

#define HGE_MAX_POINT_LIGHTS 10

typedef struct {
    hge_vec3 ambient;
    hge_vec3 diffuse;
    hge_vec3 specular;

    hge_vec3 direction;
} hge_dirlight;

typedef struct {
    int id;
    hge_vec3 ambient;
    hge_vec3 diffuse;
    hge_vec3 specular;

    float constant;
    float linear;
    float quadratic;
} hge_pointlight;

hge_dirlight hgeDirLight(
    hge_vec3 ambient,
    hge_vec3 diffuse,
    hge_vec3 specular,
    hge_vec3 direction
);

hge_pointlight hgePointLight(
    hge_vec3 ambient,
    hge_vec3 diffuse,
    hge_vec3 specular,
    float constant,
    float linear,
    float quadratic
);

void hgeLightDataReset();

void hge_system_dirlight(hge_entity* entity, hge_dirlight* light);
void hge_system_pointlight(hge_entity* entity, hge_vec3* position, hge_pointlight* light);

#endif
