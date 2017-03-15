#include <unistd.h>

#include "AirKiss.h"



int main(int argc, char **argv)
{
	Slink::CAirKiss airKiss;

	airKiss.SetRouteInfo("21d12", "12345678");

	while(1) {
		sleep(10);
	}

	airKiss.Reset();

	return 0;
}
