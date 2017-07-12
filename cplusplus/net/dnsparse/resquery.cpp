#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <string>



bool checkDNS(std::string domain)
{
	int ret = res_init();
	u_char answer[128] = {0};

	ret = res_query(domain.c_str(), C_IN, T_TXT, answer, sizeof(answer));
	if (ret < 12) {
		return false;
	}

	printf("(%s) ret:(%d)\n", domain.c_str(), ret);

	int point = 2;

	if (answer[point] == 0x81 && answer[point + 1] == 0x80) {
		point += 2;
		point += 8;
		point += 5;
		point += 8;
		point += 4;
		point += 1;
		point += 14;
		point += 2;
		if (answer[point -1] == 0x0d && answer[point] == 0x0c) {
			return true;
		}
	}

	return false;
}

int main(int argc, char** argv)
{
	bool res = checkDNS(argv[1]);
	if(res) {
		printf("check success!\n");
	}

	return 0;
}

// g++ -o test resquery.cpp -lresolv