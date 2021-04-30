#include "Boombox.h"

float boombox_animation = 0.0f;
float boombox_animation_countdown = 0.0f;

float volume_master = 1.0f;
float volume_voice = 1.0f;
float volume_sfx = 1.0f;
float volume_bgm = 1.0f;

char last_played_bgm[255] = "";
char artist[255] = "";

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

  strcpy(last_played_bgm, name);
  boombox_animation_countdown = 6.f;
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

void televoidBoomboxRender() {
  hge_shader gui_shader = hgeResourcesQueryShader("gui");

  hge_vec3 scale = { 350, 150, 0 };
  hge_vec3 position = { -(float)(hgeWindowWidth())/2.f - scale.x/2.f + scale.x * boombox_animation, (float)(hgeWindowHeight())/2.f - scale.y/2.f, 100 };
  hge_material material = { hgeResourcesQueryTexture("GUI RADIO"), hgeResourcesQueryTexture("HGE DEFAULT NORMAL"), false, hgeVec4(1, 1, 1, 1) };
  hge_transform transform = { position, scale, hgeQuaternion(0, 0, 0, 1) };

  hge_material text_material;
  text_material.color_multiplier = hgeVec4(132.f/255.f, 190.f/255.f, 58.f/255.f, 1);
  hge_vec3 text_position = { position.x - 135, position.y + 5, 99 };
  hge_vec3 text_scale = { 0, 0.5f, 0 };
  hge_transform text_transform = { text_position, text_scale, hgeQuaternion(0, 0, 0, 1) };
  hgeRenderText(
    hgeResourcesQueryShader("gui text"),
    text_material,
    text_transform,
    hgeResourcesQueryFont("VCR"),
    HGE_TEXT_ALIGNED_LEFT,
    last_played_bgm
  );

  text_transform.position.y = position.y - 15;
  text_transform.scale.y = 0.3f;
  hgeRenderText(
    hgeResourcesQueryShader("gui text"),
    text_material,
    text_transform,
    hgeResourcesQueryFont("VCR"),
    HGE_TEXT_ALIGNED_LEFT,
    artist
  );

  // Background
  transform.position.z = 95;
  material.diffuse = hgeResourcesQueryTexture("HGE DEFAULT NORMAL");
  material.color_multiplier = hgeVec4(0, 0, 0, 1);
  hgeRenderSprite(
    gui_shader,
    material,
    transform
  );

  // Radio
  transform.position.z = 100;
  material.diffuse = hgeResourcesQueryTexture("GUI RADIO");
  material.color_multiplier = hgeVec4(1, 1, 1, 1);
  hgeRenderSprite(
    gui_shader,
    material,
    transform
  );
}

void televoidBoomboxUpdate() {
  if(boombox_animation_countdown <= 0) {
    boombox_animation += (0.0f - boombox_animation) * 10.0f * hgeDeltaTime();
  } else {
    boombox_animation += (1.0f - boombox_animation) * 10.0f * hgeDeltaTime();
    boombox_animation_countdown -= hgeDeltaTime();
  }

  if (boombox_animation > 1.0f) boombox_animation = 1.0f;

  televoidBoomboxRender();
}
