#pragma once
#include <string>

void* kb_loadlib(const std::string& name);
void* kb_getfunc(void* handle, const char* funcName);
