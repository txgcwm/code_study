#include "Client.h"


#define IFNAME "enp2s0"

int main(int argc, char** argv)
{
    SendBroadcastData((char*)IFNAME, 9999);

    return 0;
}