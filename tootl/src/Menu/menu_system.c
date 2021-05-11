#include "menu_system.h"

gui_menu menu_create(const char* title, int* current_option) {
  gui_menu menu;
  strcpy(menu.title, title);
  menu.length = 0;
  menu.current_option = current_option;
  return menu;
}

void menu_add_option( gui_menu* menu,
                      const char* str,
                      void (*function)()) {
    menu_option option;
    strcpy(option.str, str);
    option.function = function;
    menu->options[menu->length] = option;
    menu->length++;
}

void draw_menu_title(gui_menu menu) {
  hge_material text_material;
  text_material.color_multiplier = hgeVec4(1, 1, 1, 1);

  float height_offset = (50 * (menu.length+1))/2.f;

  float font_size = 100.f;
  hge_shader gui_shader = hgeResourcesQueryShader("gui text");
  float text_height = font_size/2;
  hge_vec3 text_position = { 0, height_offset, 100 };

  hge_transform text_transform = {
    text_position,
    hgeVec3(font_size/100.f, font_size/100.f, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  hgeRenderText(
    gui_shader,
    text_material,
    text_transform,
    hgeResourcesQueryFont("VCR"),
    HGE_TEXT_ALIGNED_CENTERED,
    menu.title
  );
}

void draw_menu_option(gui_menu menu, int option_index, int currently_selected) {
  menu_option option = menu.options[option_index];

  hge_material text_material;
  if(option_index == currently_selected) {
    text_material.color_multiplier = hgeVec4(1, 1, 1, 1);
  } else {
    text_material.color_multiplier = hgeVec4(0.5, 0.5, 0.5, 1);
  }

  float height_offset = (50 * menu.length)/2.f;

  float font_size = 100.f;
  hge_shader gui_shader = hgeResourcesQueryShader("gui text");
  float text_height = font_size/2;
  hge_vec3 text_position = { 0, -25 - 50 * option_index + height_offset, 100 };

  hge_transform text_transform = {
    text_position,
    hgeVec3(font_size/100.f, font_size/100.f, 0),
    hgeQuaternion(0, 0, 0, 1)
  };

  hgeRenderText(
    gui_shader,
    text_material,
    text_transform,
    hgeResourcesQueryFont("VCR"),
    HGE_TEXT_ALIGNED_CENTERED,
    option.str
  );
}

int process_menu(gui_menu menu) {
  draw_menu_title(menu);
  for(int i = 0; i < menu.length; i++) {
    draw_menu_option(menu, i, *menu.current_option);
  }

  bool pressed_key_up = hgeInputGetKeyDown(HGE_KEY_W) | hgeInputGetKeyDown(HGE_KEY_UP);
  bool pressed_key_down = hgeInputGetKeyDown(HGE_KEY_S) | hgeInputGetKeyDown(HGE_KEY_DOWN);
  bool pressed_key_accept = hgeInputGetKeyDown(HGE_KEY_ENTER);
  menu_option selected_option = menu.options[*menu.current_option];

  if(pressed_key_up) {
    *menu.current_option = *menu.current_option - 1;
  }
  if(pressed_key_down) {
    *menu.current_option = *menu.current_option + 1;
  }
  if(pressed_key_accept) {
    selected_option.function();
  }

  if(*menu.current_option > menu.length-1) {
    *menu.current_option = menu.length-1;
  }
  if(*menu.current_option < 0) {
    *menu.current_option = 0;
  }
}
