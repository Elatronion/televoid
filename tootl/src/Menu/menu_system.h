#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <HGE/HGE.h>

#include <stdbool.h>

#define MENU_STR_LEN 64
#define MAX_OPTIONS 8

typedef struct {
	char str[MENU_STR_LEN];
	void (*function)();
} menu_option;

typedef struct {
	char title[MENU_STR_LEN];
	menu_option options[MAX_OPTIONS];
	int length;
	int* current_option;
} gui_menu;

gui_menu menu_create(const char* title, int* current_option);
void menu_add_option(gui_menu* menu,
	const char* str,
	void (*function)());

int process_menu(gui_menu menu);

#endif
