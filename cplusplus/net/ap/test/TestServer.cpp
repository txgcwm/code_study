#include "Server.h"


int main(int argc, char** argv)
{
    std::string ssid;
    std::string password;
    int mode;

    GetRouteInfo(9999, ssid, password, mode);

    return 0;
}