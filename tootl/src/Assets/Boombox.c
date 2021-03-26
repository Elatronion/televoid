#include "Boombox.h"

float volume_master = 1.0f;
float volume_voice = 1.0f;
float volume_sfx = 1.0f;
float volume_bgm = 1.0f;

char last_played_bgm[255] = "";

float televoidBoomboxSettingsVolumeGetMaster() { return volume_master; }
float televoidBoomboxSettingsVolumeGetVoice()  { return volume_voice; }
float televoidBoomboxSettingsVolumeGetSFX()    { return volume_sfx; }
float televoidBoomboxSettingsVolumeGetBGM()    { return volume_bgm; }

void televoidBoomboxSettingsVolumeSetMaster(float volume) { volume_master = volume; }
void televoidBoomboxSettingsVolumeSetVoice(float volume)  { volume_voice = volume; }
void televoidBoomboxSettingsVolumeSetSFX(float volume)    { volume_sfx = volume; }
void televoidBoomboxSettingsVolumeSetBGM(float volume)    { volume_bgm = volume; }

void televoidBoomboxPlaySFX(const char* name) {
  hge_audiosource audiosource = { hgeResourcesQueryAudio(name), volume_sfx * volume_master };
  hgeAudioSourcePlay(audiosource);
}

void televoidBoomboxStopSFX(const char* name) {
  hge_audiosource audiosource = { hgeResourcesQueryAudio(name), volume_sfx * volume_master };
  hgeAudioSourceStop(audiosource);
}

void televoidBoomboxPlayBGM(const char* name) {
  hge_audiosource audiosource = { hgeResourcesQueryAudio(name), volume_bgm * volume_master };
  hgeAudioSourcePlay(audiosource);
}

void televoidBoomboxStopBGM(const char* name) {
  hge_audiosource audiosource = { hgeResourcesQueryAudio(name), volume_bgm * volume_master };
  hgeAudioSourceStop(audiosource);
}

void televoidBoomboxPlayVoice(const char* name) {
  hge_audiosource audiosource = { hgeResourcesQueryAudio(name), volume_voice * volume_master };
  hgeAudioSourcePlay(audiosource);
}

void televoidBoomboxStopVoice(const char* name) {
  hge_audiosource audiosource = { hgeResourcesQueryAudio(name), volume_voice * volume_master };
  hgeAudioSourceStop(audiosource);
}

void televoidBoomboxUpdate() {

}
