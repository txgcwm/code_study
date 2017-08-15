#include <string.h>
#include <stdio.h>

#include "Client.h"


#define IFNAME "enp2s0"

int main(int argc, char** argv)
{
    char* ssid = (char*)"qipashuo";
    char* password = (char*)"gansiren";
    int mode = 1;
    int result = -1;

    SendBroadcastData(result, (char*)IFNAME, 9999, ssid, password, mode);

    printf("result(%d)\n", result);

    return 0;
}