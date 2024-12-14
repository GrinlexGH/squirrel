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

SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api) {
    kb::DefaultVM::Set(vm);

    int iResult = WSAStartup(MAKEWORD(2, 2), &g_Wsadata);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    kb::ConstTable().Const("AF_UNSPEC", AF_UNSPEC);
    kb::ConstTable().Const("AF_INET", AF_INET);
    kb::ConstTable().Const("AF_INET6", AF_INET6);

    kb::Class<CSocket> socket(vm, "socket");
    socket
    .Ctor()
    .Ctor<int>()
    .Ctor<int, int>()
    ;

    kb::RootTable().Bind("socket", socket);

    return SQ_OK;
}

void sqmodule_destruct(HSQUIRRELVM vm) {
    WSACleanup();
}
