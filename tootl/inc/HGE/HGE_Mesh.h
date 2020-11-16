#ifndef HGE_MESH_H
#define HGE_MESH_H

#include "HGE_Material.h"

typedef struct {
  unsigned int VAO, VBO, EBO;
  unsigned int* indexBuffer;
  unsigned int num_indicies;
  hge_material material;
} hge_mesh;

hge_mesh hgeLoadMesh(const char* meshName);

#endif
