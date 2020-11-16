#ifndef HGE_MATH3D_H
#define HGE_MATH3D_H

#include <stdbool.h>
#include <math.h>

typedef struct {
  float x, y;
} hge_vec2;

typedef struct {
  float x, y, z;
} hge_vec3;

typedef struct {
  float x, y, z, w;
} hge_vec4;

typedef struct {
  float value[4][4];
} hge_mat4;

typedef struct {
  hge_vec3 position;
  hge_vec3 scale;
} hge_transform;

// Initialization Functions
hge_vec2 hgeVec2(float x, float y);
hge_vec3 hgeVec3(float x, float y, float z);
hge_vec4 hgeVec4(float x, float y, float z, float w);
hge_transform hgeTransform(hge_vec3 position, hge_vec3 scale);

// Math Functions
float hgeMathRadians(float degrees);
hge_vec3 hgeNormalizedDeviceCoords(float mouseX, float mouseY);

hge_vec3 hgeMathVec3Add(hge_vec3 vecA, hge_vec3 vecB);
hge_vec3 hgeMathVec3Sub(hge_vec3 vecA, hge_vec3 vecB);
hge_vec3 hgeMathVec3Mul(hge_vec3 vecA, hge_vec3 vecB);
hge_vec3 hgeMathVec3MulAmt(hge_vec3 vec, float amt);

hge_vec2 hgeMathVec2Normalize(hge_vec2 vec2);
hge_vec3 hgeMathVec3Normalize(hge_vec3 vec3);
hge_vec3 hgeMathVec3Cross();

void hgeMathPrintMat4(hge_mat4 mat4);

hge_mat4 hgeMathMat4(float n);
hge_mat4 hgeMathMatMultiply(hge_mat4 mat4A, hge_mat4 mat4B);
hge_mat4 hgeMathMat4Translate(hge_mat4 mat4, hge_vec3 vec3);
hge_mat4 hgeMathMat4Scale(hge_mat4 mat4, hge_vec3 vec);
hge_mat4 hgeMathMat4Rotate(hge_mat4 mat4, float angle);

hge_mat4 hgeMathMat4PerspectiveProjection(float fov, float aspect, float fNear, float fFar);
hge_mat4 hgeMathMat4OrthographicProjection(float width, float height, float fNear, float fFar);

hge_mat4 hgeMath4LookAt(hge_vec3 position, hge_vec3 target, hge_vec3 up);

#endif
