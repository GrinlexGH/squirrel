#include <kalibri.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api) {
    kb::DefaultVM::Set(vm);



    return SQ_OK;
}
