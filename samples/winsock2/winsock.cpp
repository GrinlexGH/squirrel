#include <kalibri.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

WSADATA g_Wsadata {};

class CSocket {
public:
    CSocket(int family = AF_INET, int type = SOCK_STREAM) {
        
    }
};

SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api, kb::Table& RetTable) {
    kb::DefaultVM::Set(vm);

    int iResult = WSAStartup(MAKEWORD(2, 2), &g_Wsadata);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    RetTable.SetValue("AF_UNSPEC", (int)AF_UNSPEC);
    RetTable.SetValue("AF_INET", (int)AF_INET);
    RetTable.SetValue("AF_INET6", (int)AF_INET6);

    kb::Class<CSocket> socket(vm, "socket");
    socket
    .Ctor()
    .Ctor<int>()
    .Ctor<int, int>()
    ;

    RetTable.Bind("socket", socket);

    return SQ_OK;
}

void sqmodule_destruct(HSQUIRRELVM vm) {
    WSACleanup();
}
