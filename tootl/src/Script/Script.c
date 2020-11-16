#include "Script.h"
#include <stdio.h>
#include <string.h>
#include "wren.h"

#include "Scene.h"
#include "Inventory.h"

void wrenSceneLoad(WrenVM* vm) {
  const char* str = wrenGetSlotString(vm, 1);
  televoidSceneLoad(str);
}

void wrenInventoryAdd(WrenVM* vm) {
  const char* item_name = wrenGetSlotString(vm, 1);
  InventoryAddItem(televoidGetItemID(item_name));
}

void wrenInventoryRemove(WrenVM* vm) {
  const char* item_name = wrenGetSlotString(vm, 1);
  InventoryRemoveItem(televoidGetItemID(item_name));
}

void wrenInventoryHas(WrenVM* vm) {
  const char* item_name = wrenGetSlotString(vm, 1);
  wrenSetSlotBool(vm, 0, InventoryHasItem(televoidGetItemID(item_name)));
}

WrenForeignMethodFn bindForeignMethod(
    WrenVM* vm,
    const char* module,
    const char* className,
    bool isStatic,
    const char* signature)
{
  if (strcmp(module, "televoid/core") == 0)
  {
    if (strcmp(className, "Scene") == 0)
    {
      if (isStatic && strcmp(signature, "load(_)") == 0)
      {
        return wrenSceneLoad; // C function for Scene.load(_).
      }
      // Other foreign methods on WrenC...
    } else if(strcmp(className, "Inventory") == 0) {
      if (isStatic && strcmp(signature, "add(_)") == 0) {
        return wrenInventoryAdd; // C function for Inventory.add(_).
      } else if(isStatic && strcmp(signature, "remove(_)") == 0) {
        return wrenInventoryRemove; // C function for Inventory.remove(_).
      } else if(isStatic && strcmp(signature, "has(_)") == 0) {
        return wrenInventoryHas;
      }
    }
    // Other classes in televoid/core...
  }
  // Other modules...
}

static void writeFn(WrenVM* vm, const char* text)
{
  printf("%s", text);
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

char* loadModule(WrenVM* vm, const char* name) {
  printf("Load Module: '%s'\n", name);
  char path[255];
  strcpy(path, "res/scripts/");
  strcat(path, name);
  strcat(path, ".wren");
  return hgeLoadFileAsString(path);
}

void televoidRunScript(const char* path) {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &writeFn;
  config.errorFn = &errorFn;
  config.bindForeignMethodFn = &bindForeignMethod;
  config.loadModuleFn = loadModule;
  WrenVM* vm = wrenNewVM(&config);

  const char* script = hgeLoadFileAsString(path);
  WrenInterpretResult result = wrenInterpret(vm, "main", script);

  // Load the class into slot 0.
  /*wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, "main", "Fibonacci", 0);
  WrenHandle* fibonacciClass = wrenGetSlotHandle(vm, 0);
  wrenSetSlotHandle(vm, 0, fibonacciClass);
  WrenHandle* calculate = wrenMakeCallHandle(vm, "calculate()");
  WrenInterpretResult result2 = wrenCall(vm, calculate);*/

  switch (result) {
    case WREN_RESULT_COMPILE_ERROR:
      { printf("Compile Error!\n"); } break;
    case WREN_RESULT_RUNTIME_ERROR:
      { printf("Runtime Error!\n"); } break;
    case WREN_RESULT_SUCCESS:
      { printf("Success!\n"); } break;
  }

  wrenFreeVM(vm);
}
