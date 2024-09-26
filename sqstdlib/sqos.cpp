#include "sqos.h"

#include <string>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <squnicode.h>

static void* TryLoadLib(const std::string& name) {
    // Firstly search in PATH or in exe's directory
    void* ret = nullptr;
    std::wstring libName = widen(name);
    ret = LoadLibraryExW(
        libName.c_str(),
        nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH
    );
    if (ret) {
        return ret;
    }

    // Secondly search in script directory
    static std::filesystem::path sqScriptPath;
    if (sqScriptPath.empty()) {
        int argc;
        wchar_t** sqScriptName = CommandLineToArgvW(GetCommandLineW(), &argc);
        for (int arg = 1; arg < argc; ++arg) {
            if (*sqScriptName[arg] == L'-') {
                continue;
            }
            sqScriptPath = sqScriptName[arg];
            sqScriptPath.remove_filename();
            break;
        }
        LocalFree(sqScriptName);
    }
    ret = LoadLibraryExW(
        (sqScriptPath.wstring() + libName).c_str(),
        nullptr,
        LOAD_WITH_ALTERED_SEARCH_PATH
    );
    return ret;
}

void* kb_loadlib(const std::string& name) {
    void* ret = TryLoadLib(name + ".dll");
    if (ret) {
        return ret;
    }
    std::filesystem::path libPath = name;
    std::string libName = "lib" + libPath.filename().string() + ".dll";
    libPath.remove_filename();
    libPath.append(libName);
    ret = TryLoadLib(libPath.string());
    return ret;
}

void* kb_getfunc(void* handle, const char* funcName) {
    return (void*)GetProcAddress((HMODULE)handle, funcName);
}

#else

#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <squirrel.h>
#include <string>
#include <linux/limits.h>

void kb_initconsole() {

}

static std::vector<std::string> GetArgs() {
    std::ifstream cmdline_file("/proc/self/cmdline", std::ios::binary);
    if (!cmdline_file) {
        return {};
    }

    std::vector<std::string> args;
    std::string arg;
    char c;

    while (cmdline_file.get(c)) {
        if (c == '\0') {
            args.push_back(arg);
            arg.clear();
        } else {
            arg.push_back(c);
        }
    }

    if (!arg.empty()) {
        args.push_back(arg);
    }

    return args;
}

void* kb_loadlib(const std::string& name) {
    std::filesystem::path libPath = name;
    std::string libName = "lib" + libPath.filename().string() + ".so";
    libPath.remove_filename();
    libPath.append(libName);

    // Firstly search in LD_LIBRARY_PATH, .so's directory or ../lib directory
    void* ret = nullptr;
    ret = dlopen(libPath.c_str(), RTLD_NOW);
    if (ret) {
        return ret;
    }

    // Secondly search in script's directory
    static std::vector<std::string> args;
    if(args.empty()) {
        args = GetArgs();
    }
    static std::filesystem::path sqScriptPath;
    if (sqScriptPath.empty()) {
        for (int arg = 1; arg < args.size(); ++arg) {
            if (args[arg][0] == '-') {
                continue;
            }
            sqScriptPath = args[arg];
            sqScriptPath.remove_filename();
            break;
        }
    }
    ret = dlopen((sqScriptPath.string() + libPath.string()).c_str(), RTLD_NOW);
    if (ret) {
        return ret;
    }

    // Thirdly search in exec's directory
    static std::filesystem::path execPath;
    if (execPath.empty()) {
        execPath = args[0];
        execPath.remove_filename();
    }
    ret = dlopen((execPath.string() + libPath.string()).c_str(), RTLD_NOW);
    return ret;
}

void* kb_getfunc(void* handle, const char* funcName) {
    return dlsym(handle, funcName);
}

#endif
