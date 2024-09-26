#include "sqos.h"

#include <squirrel.h>
#include <kalibri.h>
#include <sqstdio.h>
#include <sqstdimport.h>
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>

static SQRESULT kb_importscript(HSQUIRRELVM v, const SQChar* moduleName) {
    std::basic_string<SQChar> filename(moduleName);
    filename += _SC(".nut");
    if (SQ_FAILED(sqstd_loadfile(v, moduleName, true))) {
        if (SQ_FAILED(sqstd_loadfile(v, filename.c_str(), true))) {
            return SQ_ERROR;
        }
    }
    sq_push(v, -2);
    sq_call(v, 1, false, true);
    return SQ_OK;
}

static SQRESULT kb_importlib(HSQUIRRELVM v, const SQChar* moduleName, SQBool displayInfo) {
    void* externalLibrary = kb_loadlib(moduleName);
    if (!externalLibrary)
    {
        sq_getprintfunc(v)(v, "Failed to load library. Something happened to the file? \n");
        return SQ_ERROR;
    }

    GetInterface_t GetInterface = (GetInterface_t)(void*)kb_getfunc(externalLibrary, "GetInterface");
    if (!GetInterface)
    {
        sq_getprintfunc(v)(v, "Failed to load library. Unable to get library interface. \n");
        return SQ_ERROR;
    }

    SQILibInterface* libInt = GetInterface();

    if (displayInfo)
        sq_getprintfunc(v)(v, libInt->GetLibInfo());

    if (SQ_FAILED(libInt->Initialize(v)))
        return SQ_ERROR;

    return SQ_OK;
}

//sowwy for mess in here...
SQInteger sqstd_import(HSQUIRRELVM v)
{
    SQBool displayInfo = false;
    const SQChar* moduleName;
    SQRESULT ret = SQ_OK;

    switch (sq_gettop(v)) {
    case 2:
        sq_pushroottable(v);
        break;
    case 3:
        // table passed into second parameter
        break;
    case 4:
        if (SQ_FAILED(sq_getbool(v, 3, &displayInfo))) {
            displayInfo = false;
        }
        break;
    default:
        break;
    }

    if (SQ_FAILED(sq_getstring(v, 2, &moduleName))) {
        sq_getprintfunc(v)(v, "Thats not a string!\n");
        return SQ_ERROR;
    }

    HSQOBJECT table;
    sq_getstackobj(v, -1, &table);
    sq_addref(v, &table);
    sq_settop(v, 0);
    sq_pushobject(v, table);

    if (SQ_FAILED(kb_importscript(v, moduleName))) {
        ret = kb_importlib(v, moduleName, displayInfo);
    }

    sq_settop(v, 0);
    sq_pushobject(v, table);
    sq_release(v, &table);

    return ret;
}

SQRESULT sqstd_register_import(HSQUIRRELVM v)
{
    sq_pushstring(v, "import", -1);
    sq_newclosure(v, sqstd_import, 0);
    sq_setparamscheck(v, 2, ".s");
    sq_newslot(v, -3, SQFalse);

    return SQ_OK;
}
