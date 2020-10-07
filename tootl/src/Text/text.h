#ifndef TEXT_H
#define TEXT_H

#pragma once

#include <stdio.h>
#include <HGE/HGE_Core.h>
#include <glad/glad.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
  int x, y;
} v2i;

typedef struct {
  GLuint TextureID;   // ID handle of the glyph texture
  v2i Size;    // Size of glyph
  v2i Bearing;  // Offset from baseline to left/top of glyph
  GLuint Advance;    // Horizontal offset to advance to next glyph
} Character;

void televoidTextCreate(const char* font_path);
void televoidTextRenderSimple(const char* text, bool centered, hge_vec3 position, float scale, hge_vec4 color);
void televoidTextRender(hge_shader shader, bool centered, const char* text, hge_vec3 position, GLfloat x, GLfloat y, GLfloat scale, hge_vec4 color);

#endif
