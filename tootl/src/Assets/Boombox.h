#ifndef TELEVOID_BOOMBOX_H
#define TELEVOID_BOOMBOX_H
#include <HGE/HGE_Core.h>
#include <HGE/HGE_FileUtility.h>
#include <HGE/HGE_AudioSample.h>
#include <HGE/HGE_AudioSource.h>

/*
TODO:
  Support the cross-fading of different BGM tracks.
*/

float televoidBoomboxSettingsVolumeGetMaster();
float televoidBoomboxSettingsVolumeGetVoice();
float televoidBoomboxSettingsVolumeGetSFX();
float televoidBoomboxSettingsVolumeGetBGM();

void televoidBoomboxSettingsVolumeSetMaster(float volume);
void televoidBoomboxSettingsVolumeSetVoice(float volume);
void televoidBoomboxSettingsVolumeSetSFX(float volume);
void televoidBoomboxSettingsVolumeSetBGM(float volume);

void televoidBoomboxPlaySFX(const char* name);
void televoidBoomboxStopSFX(const char* name);

void televoidBoomboxPlayBGM(const char* name, bool looping);
void televoidBoomboxStopBGM(const char* name);

void televoidBoomboxPlayVoice(const char* name);
void televoidBoomboxStopVoice(const char* name);

void televoidBoomboxPlayDEVTALK(const char* name);

void televoidBoomboxUpdate();

#endif
