#ifndef HGE_WINDOW_H
#define HGE_WINDOW_H

#include <stdbool.h>

typedef struct {
  const char* title;
  int width, height;
} hge_window;

int hgeCreateWindow(const char* title, int x, int y, int width, int height, int FLAGS);
void hgeDestroyWindow();

void hgeProcessInput();
void hgeSwapBuffers();
void hgePollEvents();

bool hgeWindowIsClosedRequested();

#endif
