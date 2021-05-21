#include "GameSaver.h"
#include "Inventory.h"
#include "Scene.h"
#include "Boombox.h"

#include "ioutils.h"

typedef struct {
  hge_vec3 position;
  int item0, item1, item2, item3, item4, item5, item6, item7, item8, item9;
  char scene[1000];
} save_t;

typedef struct {
  float volumeMaster, volumeVoice, volumeSFX, volumeBGM;
} options_t;


const char* save_file_path = "./save.sav";
const char* options_file_path = "./options.sav";

void televoidSave(const char* scene, hge_vec3 position) {
  save_t save_data;
  char scene_to_load[1000];

  save_data.position = position;

  save_data.item0 = InventoryGetItemAtIndex(0);
  save_data.item1 = InventoryGetItemAtIndex(1);
  save_data.item2 = InventoryGetItemAtIndex(2);
  save_data.item3 = InventoryGetItemAtIndex(3);
  save_data.item4 = InventoryGetItemAtIndex(4);
  save_data.item5 = InventoryGetItemAtIndex(5);
  save_data.item6 = InventoryGetItemAtIndex(6);
  save_data.item7 = InventoryGetItemAtIndex(7);
  save_data.item8 = InventoryGetItemAtIndex(8);
  save_data.item9 = InventoryGetItemAtIndex(9);
  strcpy(&save_data.scene, scene);

  binwrite(save_file_path, &save_data, sizeof(save_data));
}

void televoidSaveOptions() {
  options_t options_data;
  options_data.volumeMaster = televoidBoomboxSettingsVolumeGetMaster();
  options_data.volumeVoice = televoidBoomboxSettingsVolumeGetVoice();
  options_data.volumeSFX = televoidBoomboxSettingsVolumeGetSFX();
  options_data.volumeBGM = televoidBoomboxSettingsVolumeGetBGM();
  binwrite(options_file_path, &options_data, sizeof(options_data));
}

void load_game_save() {
  FILE* file = fopen(save_file_path, "r");
  if (file == NULL) {
      televoidLoadScene("res/scenes/intro.tmx");
      return;
  }
  fclose(file);

  save_t save_data;
  binread(save_file_path, &save_data, sizeof(save_data));
  InventorySetItemAtIndex(0, save_data.item0);
  InventorySetItemAtIndex(1, save_data.item1);
  InventorySetItemAtIndex(2, save_data.item2);
  InventorySetItemAtIndex(3, save_data.item3);
  InventorySetItemAtIndex(4, save_data.item4);
  InventorySetItemAtIndex(5, save_data.item5);
  InventorySetItemAtIndex(6, save_data.item6);
  InventorySetItemAtIndex(7, save_data.item7);
  InventorySetItemAtIndex(8, save_data.item8);
  InventorySetItemAtIndex(9, save_data.item9);
  televoidLoadSceneForcePlayer(save_data.scene, save_data.position);
}

void load_game_options() {
  FILE* file = fopen(options_file_path, "r");
  if (file == NULL) {
    return;
  }
  fclose(file);

  options_t options_data;
  binread(options_file_path, &options_data, sizeof(options_data));
  //printf("volume_master: %f\n", options_data.volumeMaster);
  //printf("volume_voice: %f\n",  options_data.volumeVoice);
  //printf("volume_sfx: %f\n",    options_data.volumeSFX);
  //printf("volume_bgm: %f\n",    options_data.volumeBGM);
  televoidBoomboxSettingsVolumeSetMaster(options_data.volumeMaster);
  televoidBoomboxSettingsVolumeSetVoice(options_data.volumeVoice);
  televoidBoomboxSettingsVolumeSetSFX(options_data.volumeSFX);
  televoidBoomboxSettingsVolumeSetBGM(options_data.volumeBGM);
}

void televoidLoad() {
  load_game_options();
  load_game_save();
}

void televoidLoadOptions() {
  load_game_options();
}
