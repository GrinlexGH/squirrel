#include <kalibri.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

WSADATA g_WSAData {};

class CSocket {
public:
    CSocket(int family = AF_INET, int type = SOCK_STREAM) {
        
    }
};

SQRESULT sqmodule_load(HSQUIRRELVM vm, kb::Table& BindingTable) {
    kb::DefaultVM::Set(vm);

    int iResult = WSAStartup(MAKEWORD(2, 2), &g_WSAData);
    if (iResult != 0) {
        std::ostringstream err;
        err << "WSAStartup failed: " << iResult << '.';
        return sq_throwerror(vm, err.str().c_str());
    }

    kb::ConstTable().Const("AF_UNSPEC", static_cast<int>(AF_UNSPEC));
    kb::ConstTable().Const("AF_INET", AF_INET);
    kb::ConstTable().Const("AF_INET6", AF_INET6);

    kb::ConstTable().Const("SOCK_STREAM", SOCK_STREAM);
    kb::ConstTable().Const("SOCK_DGRAM", SOCK_DGRAM);
    kb::ConstTable().Const("SOCK_RAW", SOCK_RAW);
    kb::ConstTable().Const("SOCK_RDM", SOCK_RDM);
    kb::ConstTable().Const("SOCK_SEQPACKET", SOCK_SEQPACKET);

    kb::Class<CSocket> socket(vm, "socket");
    socket
    .Ctor()
    .Ctor<int>()
    .Ctor<int, int>()
    ;

    BindingTable.Bind("socket", socket);

    return SQ_OK;
}

void sqmodule_destruct(HSQUIRRELVM vm) {
    WSACleanup();
}
