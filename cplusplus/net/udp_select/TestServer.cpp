#include <unistd.h>

#include "Server.h"



int main(int argc, char **argv)
{  
	CServer server;

	server.Start(5888);

    while(true) {  
        sleep(5);
    }

    server.Stop();

	return 0;
}

// g++ -o TestServer TestServer.cpp Server.cpp ThreadLoop.cpp -lpthread