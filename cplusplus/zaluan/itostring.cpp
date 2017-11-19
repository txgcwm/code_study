#include <string.h>
#include <stdio.h>
#include <string>


std::string itostring(int value)
{
	std::string vstr = "";
	char temp[10] = {0};

	snprintf(temp, sizeof(temp), "%d", value);
	vstr.append(temp);

	return vstr;
}

int main(int argc, char **argv)
{
	int value = 8;

	printf("value: %s\n", itostring(value).c_str());

	return 0;
}

