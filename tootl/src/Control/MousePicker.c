#include "MousePicker.h"
#include "Scene.h"

hge_vec3 mousePickerRaycast() {
  hge_entity* main_camera_entity = hgeQueryEntity(2, "active", "camera");
  mouse_picker_component* mouse_picker = main_camera_entity->components[hgeQuery(main_camera_entity, "mouse picker")].data;
  return mouse_picker->current_ray;
}

hge_transform mouseGUITransform() {
  hge_vec3 mouse_position = hgeInputGetMousePosition();
  mouse_position.x -= hgeWindowWidth()/2;
  mouse_position.y = -mouse_position.y;
  mouse_position.y += hgeWindowHeight()/2;
  hge_transform mouse_transform = {
    mouse_position,
    hgeVec3(0, 0, 0),
    hgeQuaternion(0, 0, 0, 1)
  };
  return mouse_transform;
}

hge_vec3 calculateMouseRay(hge_transform* transform, mouse_picker_component* mouse_picker) {
  hge_vec3 mouse_normalized = hgeNormalizedDeviceCoords(hgeInputGetMousePosition().x, hgeInputGetMousePosition().y);
  hge_vec2 normalized_coords = hgeVec2(mouse_normalized.x, mouse_normalized.y);

  float aspect_ratio = (float)hgeWindowWidth()/(float)hgeWindowHeight();
  hge_vec3 screen_world_position;
  screen_world_position.x = normalized_coords.x * aspect_ratio;
  screen_world_position.y = normalized_coords.y;
  screen_world_position.z = -1.f;
  screen_world_position = hgeVec3Mulf(screen_world_position, transform->position.z);
  screen_world_position.x += transform->position.x;
  screen_world_position.y += transform->position.y;
  screen_world_position.z += transform->position.z;

  /*
  hge_material rendering_material = {
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL"),
    hgeResourcesQueryTexture("HGE DEFAULT NORMAL")
  };

  hge_transform rendering_transform = {
    screen_world_position,
    hgeVec3(10, 10, 10),
    hgeQuaternion(0, 0, 0, 1)
  };

  hgeRenderSprite(
    hgeResourcesQueryShader("basic"),
    rendering_material,
    rendering_transform
  );
  */

  return screen_world_position;
}

void system_mouse_picker(hge_entity* entity, hge_transform* transform, mouse_picker_component* mouse_picker) {
  //mouse_picker->view_matrix = view matrix from camera
  mouse_picker->current_ray = calculateMouseRay(transform, mouse_picker);
}
