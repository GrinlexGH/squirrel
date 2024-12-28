#pragma once
#include <squirrel.h>

#include "kalibri/kbmodule.hpp"
#include "kalibri/kbtable.hpp"
#include "kalibri/kbclass.hpp"
#include "kalibri/kbfunction.hpp"
#include "kalibri/kbconst.hpp"
#include "kalibri/kbutil.hpp"
#include "kalibri/kbscript.hpp"
#include "kalibri/kbarray.hpp"

extern "C" KALIBRI_API SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api, kb::Table& RetTable);
extern "C" KALIBRI_API void sqmodule_destruct(HSQUIRRELVM vm); // Unnecessary destructor
