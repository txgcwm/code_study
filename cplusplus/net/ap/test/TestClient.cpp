#include "Client.h"


#define IFNAME "enp2s0"

int main(int argc, char** argv)
{
    char* ssid = (char*)"test";
    char* password = (char*)"12345678";
    int mode = 1;

    SendBroadcastData((char*)IFNAME, 9999, ssid, password, mode);

    return 0;
}