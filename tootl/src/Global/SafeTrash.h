#ifndef SAFE_TRASH_H
#define SAGE_TRASH_H

#include <HGE/HGE.h>

typedef void* pointer;

void st_track(pointer ptr);
void st_untrack(pointer ptr);

void st_init();
void st_clean();

#endif
