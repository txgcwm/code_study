#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <string>



static void get_rand_str(std::string& value, int num)
{
	int i = 0;
	char ss[17] = {0};
	char str[64] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	srand((unsigned int)time((time_t *)NULL));
	
	for(i = 0; i < num; i++) {
		sprintf(&ss[i], "%c", str[(rand()%62)]);
	}
	
	value = ss;

	return;
}

static void get_key_iv(std::string& key, std::string& iv)
{
	int i = 0, num = 16;
	char ss[17] = {0};
	char str[64] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	srand((unsigned int)time((time_t *)NULL));
	
	for(i = 0; i < num; i++) {
		sprintf(&ss[i], "%c", str[(rand()%62)]);
	}
	key = ss;

	for(i = 0; i < num; i++) {
		sprintf(&ss[i], "%c", str[(rand()%62)]);
	}
	iv = ss;
	
	return;
}

int main(int argc, char** argv)
{
	std::string key, iv;
	
	// get_rand_str(key, 16);
	// get_rand_str(iv, 16);
	
	get_key_iv(key, iv);
	
	printf("%s, %s\n", key.c_str(), iv.c_str());
	
	return 0;
}

