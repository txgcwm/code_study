#include "Server.h"


int main(int argc, char** argv)
{
    std::string ssid;
    std::string password;
    int mode;

    GetRouteInfo(9999, ssid, password, mode);

    printf("ssid(%s), password(%s), mode(%d)\n",
            ssid.c_str(), password.c_str(), mode);

    return 0;
}