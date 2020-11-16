#ifndef HGE_SHADER_H
#define HGE_SHADER_H

#include <stdbool.h>
#include "HGE_Math3D.h"

typedef struct {
  int id;
} hge_shader;

hge_shader hgeLoadShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
void hgeUseShader(hge_shader shader);
void hgeShaderSetMatrix4(hge_shader shader, const char *name, hge_mat4 matrix);
void hgeShaderSetBool(hge_shader shader, const char* name, bool value);
void hgeShaderSetFloat(hge_shader shader, const char* name, float value);
void hgeShaderSetVec3(hge_shader shader, const char *name, hge_vec3 vec3);
void hgeShaderSetVec4(hge_shader shader, const char *name, hge_vec4 vec4);
void hgeShaderSetInt(hge_shader shader, const char* name, int value);

#endif
