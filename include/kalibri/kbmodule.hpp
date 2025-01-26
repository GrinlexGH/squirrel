#pragma once
#include <squirrel.h>

#include "kbtable.hpp"

extern "C" KALIBRI_API SQRESULT sqmodule_load(HSQUIRRELVM vm, kb::Table& BindingTable);
extern "C" KALIBRI_API void sqmodule_destruct(HSQUIRRELVM vm); // Unnecessary destructor
