#ifndef HGE_FRAME_BUFFER_H
#define HGE_FRAME_BUFFER_H
#include <stdio.h>
#include <stdbool.h>
#include "HGE_Texture.h"

typedef unsigned int GLuint;

typedef struct {
  GLuint framebuffer;
  GLuint Width, Height;
  GLuint Internal_Format; // Format of texture object
  GLuint Image_Format; // Format of loaded image
             // Texture configuration
  GLuint Wrap_S; // Wrapping mode on S axis
  GLuint Wrap_T; // Wrapping mode on T axis
  GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
  GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels

  // variables
  hge_texture color;
  bool multisample;
  GLuint colorBuffer;
} hge_framebuffer;

hge_framebuffer hgeGenerateFramebuffer(GLuint width, GLuint height, bool multisampled);

void hgeResolveToFbo(hge_framebuffer framebuffer, hge_framebuffer* outputFbo) ;

void hgeBindFramebuffer(hge_framebuffer framebuffer);
void hgeUnbindFramebuffer();

#endif
