#ifndef SCRIPTING_H
#define SCRIPTING_H
#include <HGE/HGE_Core.h>
#include "wren.h"

WrenForeignMethodFn bindForeignMethod(
    WrenVM* vm,
    const char* module,
    const char* className,
    bool isStatic,
    const char* signature);

void writeFn(WrenVM* vm, const char* text);

void errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg);

WrenLoadModuleResult loadModule(WrenVM* vm, const char* name);

bool televoidWrenExecute(const char* script_path);
bool televoidWrenExecuteSnippet(const char* snippet);

#endif
