#ifndef HGE_CORE_H
#define HGE_CORE_H

#include <stdlib.h>
#include <stdbool.h>
#include "HGE_Log.h"
#include "HGE_Input.h"
#include "HGE_RenderingEngine.h"
#include "HGE_Window.h"
#include "HGE_FileUtility.h"
#include "HGE_ResourceManager.h"
#include "HGE_ECS.h"
#include "HGE_NetworkingEngine.h"

#include "hge_components.h"

#define HGE_INIT_ALL 0xFF
#define HGE_INIT_NONE 0x00
#define HGE_INIT_RENDERING 0x01
#define HGE_INIT_NETWORKING 0x02
#define HGE_INIT_ECS 0x04

void hgeInit(int frame_rate, hge_window window, uint8_t flags);
void hgeCleanUp();

float hgeDeltaTime();
float hgeRuntime();
int hgeTicks();

void hgeStart();
void hgeStop();

#endif
