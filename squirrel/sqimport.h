#pragma once

#include <kalibri/kbtable.hpp>

using SQModuleLoader_t = SQRESULT(*)(HSQUIRRELVM, kb::Table&);
using SQModuleDestructor_t = void(*)(HSQUIRRELVM);
