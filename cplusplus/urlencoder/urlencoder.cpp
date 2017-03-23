#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <string>

using namespace std;



int UrlEncode(const char *ptext, string &encode)
{
	CURL *curl = curl_easy_init();
	if(curl) {
  		char *output = curl_easy_escape(curl, ptext, strlen(ptext));
  		if(output) {
    		printf("Encode: %s\n", output);
			encode.assign(output, output + strlen(output));
    		curl_free(output);
			output = NULL;
  		}
	}

	curl_easy_cleanup(curl);
	curl = NULL;

	return 0;
}

int UrlDeCode(const char *pencode, string &decode)
{
	CURL *curl = curl_easy_init();
	if(curl) {
		int outlen = 0;
  		char *output = curl_easy_unescape(curl, pencode, strlen(pencode), &outlen);
  		if(output) {
    		printf("Decode: %s\n", output);
			decode.assign(output, output + outlen);
    		curl_free(output);
			output = NULL;
  		}
	}

	curl_easy_cleanup(curl);
	curl = NULL;

	return 0;
}

int main(int argc, char **argv)
{
	string encode = "";
	string decode = "";

	UrlEncode("hello world", encode);

	UrlDeCode(encode.c_str(), decode);

	printf("encode: %s, decode: %s\n", encode.c_str(), decode.c_str());

	return 0;
}


// g++ -o test urlencoder.cpp -lcurl
