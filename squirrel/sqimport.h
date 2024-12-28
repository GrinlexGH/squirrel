#pragma once

#include <kalibri/kbmodule.hpp>
#include <kalibri/kbtable.hpp>

using SQModuleLoad_t = SQRESULT(*)(HSQUIRRELVM, HSQAPI, kb::Table&);
using SQModuleDestruct_t = void(*)(HSQUIRRELVM);
