#ifndef _SQUNICODE_H_
#define _SQUNICODE_H_

#ifdef _WIN32

#include <windows.h>
#include <string>

inline std::string narrow(const std::wstring_view wstr) {
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

inline std::wstring widen(const std::string_view str) {
    if (str.empty()) {
        return {};
    }

    int len = ::MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0);
    std::wstring out(len, 0);
    ::MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &out[0], len);

    return out;
}

#endif

#endif /*_SQUIRREL_H_*/
