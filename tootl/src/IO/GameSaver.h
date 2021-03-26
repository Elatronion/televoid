#ifndef GAME_SAVER_H
#define GAME_SAVER_H

/*
save.csv
items,"scene"
0,0,0,0,0,0,0,0,0,0,"res/scenes/some_scene.tmx"
*/

void televoidSave(const char* scene);
void televoidSaveOptions();
void televoidLoad();
void televoidLoadOptions();


#endif
