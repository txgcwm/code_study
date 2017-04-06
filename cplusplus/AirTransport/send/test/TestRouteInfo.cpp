#include "RouteInfo.h"



int main(int argc, char **argv)
{
	CRouteInfo info;

	info.SetSsidPassword("12345678", "test");

	info.SendData();

	return 0;
}