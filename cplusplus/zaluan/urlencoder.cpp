#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>

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

void Test()
{
	string encode = "";
	string decode = "";

	UrlEncode("hello world", encode);

	UrlDeCode(encode.c_str(), decode);

	printf("encode: %s, decode: %s\n", encode.c_str(), decode.c_str());
}

int ElementUrlEncode(std::string key, std::string value, std::vector<std::string> &encode)
{
	int ret = -1;
	std::string param;
	CURL *curl = curl_easy_init();

	if(curl == NULL) {
  		return ret;
	}

	do {
		char *output = curl_easy_escape(curl, key.c_str(), key.size());
		if(output == NULL) {
			break;
		}

		param.append(output);
		curl_free(output);
		output = NULL;

		param.append("=");

		output = curl_easy_escape(curl, value.c_str(), value.size());
		if(output == NULL) {
			break;
		}

		param.append(output);
		curl_free(output);
		output = NULL;

		encode.push_back(param);
		ret = 0;
	} while(0);

	curl_easy_cleanup(curl);
	curl = NULL;

	return ret;
}

int RequestParamSignature(std::vector<std::string> &vecItems, std::string secret, std::string &reval)
{
	std::string scrtVal = secret;
	std::vector<std::string>::iterator iter;

	for(iter = vecItems.begin(); iter != vecItems.end(); ++iter) {
		if(iter != vecItems.begin()) {
			reval.append("&");
		}
		
		reval.append(*iter);
    }

	std::sort(vecItems.begin(), vecItems.end());

    for(iter = vecItems.begin(); iter != vecItems.end(); ++iter) {
		scrtVal.append("&");
		scrtVal.append(*iter);
    }

    printf("scrtVal: %s\n", scrtVal.c_str());

	return 0;
}

int main(int argc, char **argv)
{
	std::string value;
	std::vector<std::string> encode;

	ElementUrlEncode("test1", "niu", encode);
	ElementUrlEncode("test2", "bi", encode);
	ElementUrlEncode("aest2", "de", encode);

	RequestParamSignature(encode, "value", value);

	printf("%s\n", value.c_str());

	return 0;
}


// g++ -o test urlencoder.cpp -lcurl
