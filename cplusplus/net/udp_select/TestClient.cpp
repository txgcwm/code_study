#include <unistd.h>

#include "Client.h"



int main(int argc, char** argv)
{
	CClient client;

	client.Start("127.0.0.1", 5888);

	client.SendData("12345", 5);
	client.SendData("afry8x", 6);

	sleep(5);

	client.Stop();

	return 0;
}

// g++ -o TestClient TestClient.cpp Client.cpp