#ifndef _SQUNICODE_H_
#define _SQUNICODE_H_

#ifdef _WIN32

#include <windows.h>
#include <string>

inline std::string narrow(const std::wstring_view wstr) {
    if (wstr.empty()) {
        return {};
    }

    const int len = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(), static_cast<int>(wstr.size()),
        nullptr, 0,
        nullptr, nullptr
    );

    std::string out(len, 0);
    WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(), static_cast<int>(wstr.size()),
        out.data(), len,
        nullptr, nullptr
    );

    return out;
}

inline std::wstring widen(const std::string_view str) {
    if (str.empty()) {
        return {};
    }

    const int len = MultiByteToWideChar(
        CP_UTF8, 0,
        str.data(), static_cast<int>(str.size()),
        nullptr, 0
    );

    std::wstring out(len, 0);
    MultiByteToWideChar(
        CP_UTF8, 0,
        str.data(), static_cast<int>(str.size()),
        out.data(), len
    );

    return out;
}

#endif

#endif /*_SQUIRREL_H_*/
