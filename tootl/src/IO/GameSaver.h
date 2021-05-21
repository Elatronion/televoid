#ifndef GAME_SAVER_H
#define GAME_SAVER_H

#include <HGE/HGE_Core.h>
/*
save.csv
items,"scene"
0,0,0,0,0,0,0,0,0,0,"res/scenes/some_scene.tmx"
*/

void televoidSave(const char* scene, hge_vec3 position);
void televoidSaveOptions();
void televoidLoad();
void televoidLoadOptions();


#endif
