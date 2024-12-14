#include <kalibri.hpp>
#include <sqvm.h>

#include <filesystem>

using SQModuleLoad_t = SQRESULT(*)(HSQUIRRELVM, HSQAPI);

static HSQAPI sqapi = NULL;

// Create and populate the HSQAPI structure with function pointers
// If new functions are added to the Squirrel API, they should be added here too
static HSQAPI kb_newapi()
{
    HSQAPI sq = (HSQAPI)sq_malloc(sizeof(sq_api));

    /*vm*/
    sq->open = sq_open;
    sq->newthread = sq_newthread;
    sq->seterrorhandler = sq_seterrorhandler;
    sq->close = sq_close;
    sq->setforeignptr = sq_setforeignptr;
    sq->getforeignptr = sq_getforeignptr;
    sq->setsharedforeignptr = sq_setsharedforeignptr;
    sq->getsharedforeignptr = sq_getsharedforeignptr;
    sq->setvmreleasehook = sq_setvmreleasehook;
    sq->getvmreleasehook = sq_getvmreleasehook;
    sq->setsharedreleasehook = sq_setsharedreleasehook;
    sq->getsharedreleasehook = sq_getsharedreleasehook;
    sq->setprintfunc = sq_setprintfunc;
    sq->getprintfunc = sq_getprintfunc;
    sq->geterrorfunc = sq_geterrorfunc;
    sq->suspendvm = sq_suspendvm;
    sq->wakeupvm = sq_wakeupvm;
    sq->getvmstate = sq_getvmstate;
    sq->getversion = sq_getversion;

    /*compiler*/
    sq->compile = sq_compile;
    sq->compilebuffer = sq_compilebuffer;
    sq->enabledebuginfo = sq_enabledebuginfo;
    sq->notifyallexceptions = sq_notifyallexceptions;
    sq->setcompilererrorhandler = sq_setcompilererrorhandler;

    /*stack operations*/
    sq->push = sq_push;
    sq->pop = sq_pop;
    sq->poptop = sq_poptop;
    sq->remove = sq_remove;
    sq->gettop = sq_gettop;
    sq->settop = sq_settop;
    sq->reservestack = sq_reservestack;
    sq->cmp = sq_cmp;
    sq->move = sq_move;

    /*object creation handling*/
    sq->newuserdata = sq_newuserdata;
    sq->newtable = sq_newtable;
    sq->newtableex = sq_newtableex;
    sq->newarray = sq_newarray;
    sq->newclosure = sq_newclosure;
    sq->setparamscheck = sq_setparamscheck;
    sq->bindenv = sq_bindenv;
    sq->setclosureroot = sq_setclosureroot;
    sq->getclosureroot = sq_getclosureroot;
    sq->pushstring = sq_pushstring;
    sq->pushfloat = sq_pushfloat;
    sq->pushinteger = sq_pushinteger;
    sq->pushbool = sq_pushbool;
    sq->pushuserpointer = sq_pushuserpointer;
    sq->pushnull = sq_pushnull;
    sq->pushthread = sq_pushthread;
    sq->gettype = sq_gettype;
    sq->type_of = sq_typeof;
    sq->getsize = sq_getsize;
    sq->gethash = sq_gethash;
    sq->getbase = sq_getbase;
    sq->instanceof = sq_instanceof;
    sq->tostring = sq_tostring;
    sq->tobool = sq_tobool;
    sq->getstring = sq_getstring;
    sq->getinteger = sq_getinteger;
    sq->getfloat = sq_getfloat;
    sq->getbool = sq_getbool;
    sq->getthread = sq_getthread;
    sq->getuserpointer = sq_getuserpointer;
    sq->getuserdata = sq_getuserdata;
    sq->settypetag = sq_settypetag;
    sq->gettypetag = sq_gettypetag;
    sq->setreleasehook = sq_setreleasehook;
    sq->getreleasehook = sq_getreleasehook;
    sq->getscratchpad = sq_getscratchpad;
    sq->getfunctioninfo = sq_getfunctioninfo;
    sq->getclosureinfo = sq_getclosureinfo;
    sq->getclosurename = sq_getclosurename;
    sq->setnativeclosurename = sq_setnativeclosurename;
    sq->setinstanceup = sq_setinstanceup;
    sq->getinstanceup = sq_getinstanceup;
    sq->setclassudsize = sq_setclassudsize;
    sq->newclass = sq_newclass;
    sq->createinstance = sq_createinstance;
    sq->setattributes = sq_setattributes;
    sq->getattributes = sq_getattributes;
    sq->getclass = sq_getclass;
    sq->weakref = sq_weakref;
    sq->getdefaultdelegate = sq_getdefaultdelegate;
    sq->getmemberhandle = sq_getmemberhandle;
    sq->getbyhandle = sq_getbyhandle;
    sq->setbyhandle = sq_setbyhandle;

    /*object manipulation*/
    sq->pushroottable = sq_pushroottable;
    sq->pushregistrytable = sq_pushregistrytable;
    sq->pushconsttable = sq_pushconsttable;
    sq->setroottable = sq_setroottable;
    sq->setconsttable = sq_setconsttable;
    sq->newslot = sq_newslot;
    sq->deleteslot = sq_deleteslot;
    sq->set = sq_set;
    sq->get = sq_get;
    sq->rawset = sq_rawset;
    sq->rawget = sq_rawget;
    sq->rawdeleteslot = sq_rawdeleteslot;
    sq->newmember = sq_newmember;
    sq->rawnewmember = sq_rawnewmember;
    sq->arrayappend = sq_arrayappend;
    sq->arraypop = sq_arraypop;
    sq->arrayresize = sq_arrayresize;
    sq->arrayreverse = sq_arrayreverse;
    sq->arrayremove = sq_arrayremove;
    sq->arrayinsert = sq_arrayinsert;
    sq->setdelegate = sq_setdelegate;
    sq->getdelegate = sq_getdelegate;
    sq->clone = sq_clone;
    sq->setfreevariable = sq_setfreevariable;
    sq->next = sq_next;
    sq->getweakrefval = sq_getweakrefval;
    sq->clear = sq_clear;

    /*calls*/
    sq->call = sq_call;
    sq->resume = sq_resume;
    sq->getlocal = sq_getlocal;
    sq->getcallee = sq_getcallee;
    sq->getfreevariable = sq_getfreevariable;
    sq->throwerror = sq_throwerror;
    sq->throwobject = sq_throwobject;
    sq->reseterror = sq_reseterror;
    sq->getlasterror = sq_getlasterror;

    /*raw object handling*/
    sq->getstackobj = sq_getstackobj;
    sq->pushobject = sq_pushobject;
    sq->addref = sq_addref;
    sq->release = sq_release;
    sq->getrefcount = sq_getrefcount;
    sq->resetobject = sq_resetobject;
    sq->objtostring = sq_objtostring;
    sq->objtobool = sq_objtobool;
    sq->objtointeger = sq_objtointeger;
    sq->objtofloat = sq_objtofloat;
    sq->objtouserpointer = sq_objtouserpointer;
    sq->getobjtypetag = sq_getobjtypetag;
    sq->getvmrefcount = sq_getvmrefcount;

    /*GC*/
    sq->collectgarbage = sq_collectgarbage;
    sq->resurrectunreachable = sq_resurrectunreachable;

    /*serialization*/
    sq->writeclosure = sq_writeclosure;
    sq->readclosure = sq_readclosure;

    /*mem allocation*/
    sq->malloc = sq_malloc;
    sq->realloc = sq_realloc;
    sq->free = sq_free;

    /*debug*/
    sq->stackinfos = sq_stackinfos;
    sq->setdebughook = sq_setdebughook;
    sq->setnativedebughook = sq_setnativedebughook;

    return sq;
}

#ifdef _WIN32
#include <windows.h>

#include <string>

static std::string narrow(const std::wstring_view wstr)
{
    if (wstr.empty()) {
        return {};
    }

    int len = ::WideCharToMultiByte(
        CP_UTF8, 0, &wstr[0], (int)wstr.size(),
        nullptr, 0, nullptr, nullptr
    );
    std::string out(len, 0);
    ::WideCharToMultiByte(
        CP_UTF8, 0, &wstr[0], (int)wstr.size(),
        &out[0], len, nullptr, nullptr
    );

    return out;
}

static std::wstring widen(const std::string_view str)
{
    if (str.empty()) {
        return {};
    }

    int len = ::MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0);
    std::wstring out(len, 0);
    ::MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &out[0], len);

    return out;
}

static void* LoadLib(const std::string& name)
{
    void* ret = LoadLibraryExW(
        widen(name + ".dll").c_str(),
        nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH
    );
    if (ret)
        return ret;

    std::filesystem::path libPath = name;
    std::string libName = "lib" + libPath.filename().string() + ".dll";
    libPath.remove_filename();
    libPath.append(libName);
    return LoadLibraryExW(
        widen(libPath.string()).c_str(),
        nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH
    );
}

static void* GetFunc(void* handle, const char* funcName)
{
    return (void*)GetProcAddress((HMODULE)handle, funcName);
}

#else

#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <linux/limits.h>

static void* LoadLib(const std::string& name)
{
    std::filesystem::path libPath = name;
    std::string libName = "./lib" + libPath.filename().string() + ".so";
    libPath.remove_filename();
    libPath.append(libName);

    return dlopen(libPath.c_str(), RTLD_NOW);
}

static void* GetFunc(void* handle, const char* funcName)
{
    return dlsym(handle, funcName);
}

#endif

static SQRESULT kb_importscript(HSQUIRRELVM v, const SQChar* moduleName)
{
    std::string filename(moduleName);
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

static SQRESULT kb_importlib(HSQUIRRELVM v, const SQChar* moduleName)
{
    void* externalLibrary = LoadLib(moduleName);
    if (!externalLibrary) {
        return SQ_ERROR;
    }

    SQModuleLoad_t moduleLoader = (SQModuleLoad_t)GetFunc(externalLibrary, "sqmodule_load");
    if (!moduleLoader) {
        return SQ_ERROR;
    }

    SQModuleDestructor_t moduleDestructor = (SQModuleDestructor_t)GetFunc(externalLibrary, "sqmodule_destruct");
    if (moduleDestructor) {
        v->_kalibriModuleDestructors.push_back(moduleDestructor);
    }

    if (sqapi == nullptr) {
        sqapi = kb_newapi(); // Caching this for multiple imports is probably a very good idea
    }

    if (SQ_FAILED(moduleLoader(v, sqapi)))
        return SQ_ERROR;

    return SQ_OK;
}

SQInteger kb_import(HSQUIRRELVM v)
{
    SQRESULT ret = SQ_OK;

    switch (sq_gettop(v)) {
    case 2:
        sq_pushroottable(v);
        break;
    case 3:
        // Table passed into second parameter
        break;
    default:
        break;
    }

    const SQChar* moduleName;
    sq_getstring(v, -2, &moduleName);

    HSQOBJECT table;
    sq_getstackobj(v, -1, &table);
    sq_addref(v, &table);

    sq_settop(v, 0);            // Clear Stack
    sq_pushobject(v, table);    // Push the target table onto the stack

    if (SQ_FAILED(kb_importscript(v, moduleName))) {
        ret = kb_importlib(v, moduleName);
    }

    if (SQ_FAILED(ret)) {
        return sq_throwerror(v, "Cannot import library!");
    }

    sq_settop(v, 0);            // Clean up the stack (just in case the module load leaves it messy)
    sq_pushobject(v, table);    // return the target table
    sq_release(v, &table);

    return 1;
}

SQRESULT kb_register_import(HSQUIRRELVM v)
{
    sq_pushroottable(v);

    sq_pushstring(v, "import", -1);
    sq_newclosure(v, &kb_import, 0);
    sq_setparamscheck(v, -2, ".s");
    sq_newslot(v, -3, SQFalse);

    sq_pop(v, 1);

    return SQ_OK;
}
