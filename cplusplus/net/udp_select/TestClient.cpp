#include <unistd.h>

#include "Client.h"



int main(int argc, char** argv)
{
	CClient::Instance()->Start("127.0.0.1", 5888);

	CClient::Instance()->SendData("12345", 5);
	CClient::Instance()->SendData("afry8x", 6);

	sleep(5);

	CClient::Instance()->Stop();

	return 0;
}

// g++ -o TestClient TestClient.cpp Client.cpp