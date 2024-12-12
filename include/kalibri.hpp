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

SQRESULT kb_register_import(HSQUIRRELVM vm);

extern "C"
#if _WIN32
__declspec(
#ifdef KALIBRI_IMPORT
    dllimport
#else
    dllexport
#endif
)
#else
__attribute__((visibility("default")))
#endif
SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api);
