#include "SaveSystem.h"
#include "Scene.h"

float save_animation = 0.0f;
float desired_save_animation = 0.0f;
float save_animation_wait_timer = 0.0f;

void televoidGlobalSave() {
  SceneSave();
  // play save sound
  // start save animation timer
  desired_save_animation = 1;
  save_animation_wait_timer = 3;
}

void televoidSaveUpdate() {
  //save_animation = (cos(3*hgeRuntime()) + 1) / 2;
  save_animation += (desired_save_animation - save_animation) * 5.f * hgeDeltaTime();
  if(fabs(save_animation - desired_save_animation) < 0.1f) {
    save_animation_wait_timer -= hgeDeltaTime();
    if(save_animation_wait_timer <= 0) {
      save_animation_wait_timer = 0;
      desired_save_animation = -0.5;
    }
  }

  float rotation = cos(16*hgeRuntime()) * 15;
  float float_offset = sin(16*hgeRuntime()) * 2;
  rotation = rotation * (0.25f+save_animation_wait_timer) / 3.f;
  float_offset = float_offset * (0.25f+save_animation_wait_timer) / 3.f;

  float margin = 16.f;

  hge_shader gui_shader = hgeResourcesQueryShader("gui");
  hge_material material = {
    hgeResourcesQueryTexture("GUI FLOPPY"),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
    false,
    hgeVec4(1, 1, 1, 1)
  };

  hge_vec3 scale = hgeVec3(64, 64, 0);
  hge_vec3 position = hgeVec3(hgeWindowWidth()/2 + scale.x/2 - (scale.x + margin)*save_animation,
                        hgeWindowHeight()/2 - scale.y/2 - margin + float_offset,
                        100
                      );
  hge_transform transform = {
    position,
    scale,
    hgeQuaternionInitRotation(hgeVec3(0, 0, 1), hgeRadians(rotation))
  };

  hgeRenderSprite(
    gui_shader,
    material,
    transform
  );
}
