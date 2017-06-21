#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>


#define BASE_PORT 	18888

int getrandom(int min, int max, int seed)
{
    int nu = 0;

    srand((unsigned int)time(NULL) + seed);

    nu = min + (int)((float)max * rand()/(RAND_MAX + 1.0));

    if(nu < min || nu > max) {
        nu = min;
    }

    return nu;
}

void getRandStr(std::string& randstr, int num)
{
    std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    srand((unsigned int)time((time_t *)NULL));

    for(int i = 0; i < num; i++) {
        randstr += str[(rand()%str.size())];
    }

    return;
}

void GenerateRandPort()
{
	int count = 0;
	int seed = 0;
	std::vector<int> vports;

	while(count < 100) {
		seed = getrandom(0, 5000, seed);

		int value = BASE_PORT + seed;

		std::vector<int>::iterator iter;

	    iter = std::find(vports.begin(), vports.end(), value);
	    if(iter != vports.end()) {
	      	continue;
	    }

		vports.push_back(value);
		count++;
	}

	std::sort(vports.begin(), vports.end());

	for(int i = 0; i < vports.size(); i++) {
		printf("%d ", vports[i]);
	}

	return;
}

int main(int argc, char **argv)
{
	std::string rand;

	getRandStr(rand, 4);

	printf("%s\n", rand.c_str());

	return 0;
}


