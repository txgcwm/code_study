#include <unistd.h>

#include "AirTransport.h"



int main(int argc, char **argv)
{
	CAirTransport airKiss;

	airKiss.Start("", "12345678");

	while(1) {
		sleep(10);
	}

	airKiss.Stop();

	return 0;
}


// g++ AirTransport.cpp ThreadLoop.cpp TestAirTransport.cpp -o send -lpthread