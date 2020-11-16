#ifndef HGE_RESOURCE_MANAGER_H
#define HGE_RESOURCE_MANAGER_H

#define HGE_MAX_TEXTURES 1000
#define HGE_MAX_MESHES 1000
#define HGE_MAX_AUDIOSAMPLES 1000
#define HGE_MAX_SHADERS 1000

#include "HGE_Shader.h"
#include "HGE_Texture.h"
#include "HGE_Mesh.h"

void hgeResourcesLoadShader(const char* vertex_path, const char* geometry_path, const char* fragment_path, const char* name);
void hgeResourcesLoadTexture(const char* path, const char* name);
void hgeResourcesLoadMesh(const char* path, const char* name);
void hgeResourcesLoadAudio(const char* path, const char* name);

hge_shader hgeResourcesQueryShader(const char* name);
hge_texture hgeResourcesQueryTexture(const char* name);
hge_mesh hgeResourcesQueryMesh(const char* name);

#endif
