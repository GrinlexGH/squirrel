#include "sqimport.h"

#include <kalibri/kbmodule.hpp>
#include <filesystem>
#include <cassert>

#include "sqstring.h"
#include "sqvm.h"

namespace
{

HSQAPI sqapi = nullptr;

// Create and populate the HSQAPI structure with function pointers
// If new functions are added to the Squirrel API, they should be added here too
HSQAPI kb_newapi() {
    const auto sq = static_cast<HSQAPI>(sq_malloc(sizeof(sq_api)));

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

}

#ifdef _WIN32
#include <windows.h>

#include <string>

namespace
{

std::wstring widen(const std::string_view str) {
    if (str.empty()) {
        return {};
    }

    const int len = MultiByteToWideChar(
        CP_UTF8,
        0,
        &str[0],
        static_cast<int>(str.size()),
        nullptr,
        0
    );

    std::wstring out(len, 0);
    MultiByteToWideChar(
        CP_UTF8,
        0,
        &str[0],
        static_cast<int>(str.size()),
        &out[0],
        len
    );

    return out;
}

void* LoadLib(const std::string& name) {
    if (void* ret = LoadLibraryExW(
        widen(name + ".dll").c_str(),
        nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH
    ); ret) {
        return ret;
    }

    std::filesystem::path libPath = name;
    const std::string libName = "lib" + libPath.filename().string() + ".dll";
    libPath.remove_filename();
    libPath.append(libName);
    return LoadLibraryExW(
        widen(libPath.string()).c_str(),
        nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH
    );
}

void* GetFunc(void* handle, const char* funcName) {
    return static_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle), funcName));
}

}

#else

#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <linux/limits.h>

namespace
{

void* LoadLib(const std::string& name) {
    std::filesystem::path libPath = name;
    std::string libName = "./lib" + libPath.filename().string() + ".so";
    libPath.remove_filename();
    libPath.append(libName);

    return dlopen(libPath.c_str(), RTLD_NOW);
}

void* GetFunc(void* handle, const char* funcName) {
    return dlsym(handle, funcName);
}

}
#endif

namespace
{

#ifdef SQUNICODE
#define scfopen(x, y) _wfopen(widen(x).c_str(), widen(y).c_str())
#else
#define scfopen fopen
#endif

#define IO_BUFFER_SIZE 2048
struct IOBuffer {
    unsigned char buffer[IO_BUFFER_SIZE];
    SQInteger size;
    SQInteger ptr;
    FILE* file;
};

SQInteger _read_byte(IOBuffer* iobuffer) {
    if (iobuffer->ptr < iobuffer->size) {
        const SQInteger ret = iobuffer->buffer[iobuffer->ptr];
        iobuffer->ptr++;
        return ret;
    } else if ((iobuffer->size = static_cast<SQInteger>(fread(iobuffer->buffer, 1, IO_BUFFER_SIZE, iobuffer->file))) > 0) {
        const SQInteger ret = iobuffer->buffer[0];
        iobuffer->ptr = 1;
        return ret;
    }

    return 0;
}

SQInteger _read_two_bytes(IOBuffer* iobuffer) {
    if (iobuffer->ptr < iobuffer->size) {
        if (iobuffer->size < 2) {
            return 0;
        }
        const SQInteger ret = *reinterpret_cast<const uint16_t*>(&iobuffer->buffer[iobuffer->ptr]);
        iobuffer->ptr += 2;
        return ret;
    } else {
        if ((iobuffer->size = static_cast<SQInteger>(fread(iobuffer->buffer, 1, IO_BUFFER_SIZE, iobuffer->file))) > 0) {
            if (iobuffer->size < 2) {
                return 0;
            }
            const SQInteger ret = *reinterpret_cast<const uint16_t*>(&iobuffer->buffer[0]);
            iobuffer->ptr = 2;
            return ret;
        }
    }

    return 0;
}

SQInteger file_read(SQUserPointer file, SQUserPointer buf, SQInteger size) {
    if (const SQInteger ret = static_cast<SQInteger>(fread(buf, 1, size, static_cast<FILE*>(file))); ret != 0) {
        return ret;
    }

    return -1;
}

SQInteger _io_file_lexfeed_UTF8(SQUserPointer iobuf) {
    auto iobuffer = static_cast<IOBuffer*>(iobuf);

#define READ(iobuf) \
    if((inchar = (unsigned char)_read_byte(iobuf)) == 0) {\
        return 0; \
    }

    static const SQInteger utf8_lengths[16] =
    {
        1,1,1,1,1,1,1,1,        /* 0000 to 0111 : 1 byte (plain ASCII) */
        0,0,0,0,                /* 1000 to 1011 : not valid */
        2,2,                    /* 1100, 1101 : 2 bytes */
        3,                      /* 1110 : 3 bytes */
        4                       /* 1111 :4 bytes */
    };
    static const unsigned char byte_masks[5] = { 0,0,0x1f,0x0f,0x07 };
    unsigned char inchar;
    READ(iobuffer);
    SQInteger c = inchar;

    if (c >= 0x80) {
        SQInteger codelen = utf8_lengths[c >> 4];
        if (codelen == 0) {
            return 0;
        }

        SQInteger tmp = c & byte_masks[codelen];
        for (SQInteger n = 0; n < codelen - 1; n++) {
            tmp <<= 6;
            READ(iobuffer);
            tmp |= inchar & 0x3F;
        }
        c = tmp;
    }
    return c;
}

SQInteger _io_file_lexfeed_UCS2_LE(SQUserPointer iobuf) {
    auto iobuffer = static_cast<IOBuffer*>(iobuf);
    const SQInteger low = _read_two_bytes(iobuffer);
    if (low == 0) {
        return 0;
    }
    if (low < 0xD800 || low > 0xDFFF) {
        return low;
    }

    if (const SQInteger high = _read_two_bytes(iobuffer); high >= 0xDC00 && high <= 0xDFFF) {
        return ((low - 0xD800) << 10) + (high - 0xDC00) + 0x10000;
    }

    return 0;
}

SQInteger _io_file_lexfeed_UCS2_BE(SQUserPointer iobuf) {
    auto iobuffer = static_cast<IOBuffer*>(iobuf);
    SQInteger raw = _read_two_bytes(iobuffer);
    if (raw == 0) {
        return 0;
    }

    // BE to LE
    const SQInteger value = ((raw >> 8) & 0xFF) | ((raw & 0xFF) << 8);

    if (value < 0xD800 || value > 0xDFFF) {
        return value;
    }

    raw = _read_two_bytes(iobuffer);
    // BE to LE
    raw = ((raw >> 8) & 0xFF) | ((raw & 0xFF) << 8);
    if (raw >= 0xDC00 && raw <= 0xDFFF) {
        return ((value - 0xD800) << 10) + (raw - 0xDC00) + 0x10000;
    }

    return 0;
}

SQRESULT loadfile(HSQUIRRELVM v, const SQChar* filename, SQBool printerror) {
    FILE* file = scfopen(filename, "rb");

    unsigned short us;
    unsigned char uc;
    SQLEXREADFUNC func = _io_file_lexfeed_UTF8; // assume file encoding in UTF-8
    if (file) {
        if (const SQInteger ret = static_cast<SQInteger>(fread(&us, 1, 2, file)); ret != 2) {
            // probably an empty file
            us = 0;
        }
        if (us == SQ_BYTECODE_STREAM_TAG) { // BYTECODE
            fseek(file, 0, SEEK_SET);
            if (SQ_SUCCEEDED(sq_readclosure(v, file_read, file))) {
                fclose(file);
                return SQ_OK;
            }
        }
        else { // SCRIPT
            switch (us) {
                // gotta swap the next 2 lines on BIG endian machines
                case 0xFFFE: func = _io_file_lexfeed_UCS2_BE; break; // UTF-16 little endian;
                case 0xFEFF: func = _io_file_lexfeed_UCS2_LE; break; // UTF-16 big endian;
                case 0xBBEF: // UTF-8
                    if (fread(&uc, 1, sizeof(uc), file) == 0) {
                        fclose(file);
                        return sq_throwerror(v, _SC("io error"));
                    }
                    if (uc != 0xBF) {
                        fclose(file);
                        return sq_throwerror(v, _SC("Unrecognized encoding"));
                    }
                    func = _io_file_lexfeed_UTF8;
                    break;
                default: fseek(file, 0, SEEK_SET); break; // ascii or utf8
            }
            IOBuffer buffer;
            buffer.ptr = 0;
            buffer.size = 0;
            buffer.file = file;
            if (SQ_SUCCEEDED(sq_compile(v, func, &buffer, filename, printerror))) {
                fclose(file);
                return SQ_OK;
            }
        }
        fclose(file);
        return SQ_ERROR;
    }
    return sq_throwerror(v, _SC("cannot open the file"));
}

SQRESULT ImportScript(const HSQUIRRELVM v, const SQChar* moduleName) {
    std::string filename(moduleName);
    filename += _SC(".nut");
    if (SQ_FAILED(loadfile(v, moduleName, true))) {
        if (SQ_FAILED(loadfile(v, filename.c_str(), true))) {
            return SQ_ERROR;
        }
    }
    sq_push(v, -2);
    sq_call(v, 1, false, true);
    return SQ_OK;
}

SQRESULT ImportLib(const HSQUIRRELVM v, const SQChar* moduleName, kb::Table& retTable) {
    void* externalLibrary = LoadLib(moduleName);
    if (!externalLibrary) {
        return SQ_ERROR;
    }

    const auto moduleLoader = reinterpret_cast<SQModuleLoad_t>(GetFunc(externalLibrary, "sqmodule_load"));
    if (!moduleLoader) {
        return SQ_ERROR;
    }

    if (const auto moduleDestructor = reinterpret_cast<SQModuleDestruct_t>(GetFunc(externalLibrary, "sqmodule_destruct")); moduleDestructor) {
        v->_module_destructors.push_back(moduleDestructor);
    }

    if (sqapi == nullptr) {
        sqapi = kb_newapi(); // Caching this for multiple imports is probably a very good idea
    }

    if (SQ_FAILED(moduleLoader(v, sqapi, retTable))) {
        return SQ_ERROR;
    }

    return SQ_OK;
}

}

void SQVM::ImportModule(const SQObjectPtr& modulePath, const SQObjectPtr& retTable) {
    kb::Table table(retTable, this);

    if (SQ_FAILED(ImportScript(this, modulePath._unVal.pString->_val))) {
        if (SQ_FAILED(ImportLib(this, modulePath._unVal.pString->_val, table))) {
            Raise_Error("Cannot import library!");
        }
    }

    Pop();
}
