#include <unistd.h>

#include "AirKiss.h"



int main(int argc, char **argv)
{
	Slink::CAirKiss airKiss;

	airKiss.SetRouteInfo("", "");

	while(1) {
		sleep(10);
	}

	airKiss.Reset();

	return 0;
}
