#ifndef HGE_RENDERING_ENGINE_H
#define HGE_RENDERING_ENGINE_H

#include "HGE_Texture.h"
#include "HGE_Shader.h"
#include "HGE_Window.h"
#include "HGE_FrameBuffer.h"
#include "HGE_Light.h"
#include "HGE_Mesh.h"

typedef struct {
  bool isMainCamera;
  bool isOrthographic;

  float fov;
  float width;
  float height;
  float aspect;
  float zNear;
  float zFar;
} hge_camera;

void hgeClearColor(float r, float g, float b, float a);
void hgeClear(long int mask);

void hgeRenderInit(hge_window window);

void hgeSetMainFramebuffer(hge_framebuffer framebuffer);
void hgeBindMainFrameBuffer();
void hgeUnbindMainFrameBuffer();
void hgeRenderFramebufferToScreen();

void hgeRenderMesh(hge_shader shader, hge_mesh mesh, hge_vec3 position, hge_vec3 scale);

void hgeRenderSprite(hge_shader shader, hge_material material, hge_vec3 position, hge_vec3 scale, float rotation);
void hgeRenderSpriteSheet(hge_shader shader, hge_material material, hge_vec3 position, hge_vec3 scale, float rotation, hge_vec2 frame_resolution, hge_vec2 frame);
void hgeRenderQuad();

void hgeRenderFrame();

#endif
