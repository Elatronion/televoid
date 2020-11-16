#ifndef HGE_TEXTURE_H
#define HGE_TEXTURE_H

#include <stdint.h>

typedef struct {
  unsigned int id;
  int width, height;
} hge_texture;

void hgeBindTexture(hge_texture texture);
hge_texture hgeGenerateTexture(int width, int height, unsigned char* data);
hge_texture hgeSetTexture(hge_texture texture, unsigned int gltexture, int width, int height);

#endif
