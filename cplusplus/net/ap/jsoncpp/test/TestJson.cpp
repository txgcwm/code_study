#include <json/json.h>
#include <string.h>
#include <stdio.h>
#include <string>




/*
{
    "ssid": "xxxxx",        // aes¼ÓÃÜ
    "password": "xxxxx",    // aes¼ÓÃÜ
    "mode": 1
}
*/
int FormateBroadcastData(std::string& data, std::string ssid,
                            std::string password, int mode)
{
    Json::Value root;

    root["ssid"] = ssid;
    root["password"] = password;
    root["mode"] = mode;

    data = root.toStyledString();

    return 0;
}

int ParseBroadcastData(std::string data, std::string& ssid,
                            std::string& password, int& mode)
{
    Json::Reader reader;
    Json::Value value;

    if (!reader.parse(data, value)) {
        return -1;
    }

    ssid = value["ssid"].asString();
    password = value["password"].asString();
    mode = value["mode"].asInt();

    return 0;
}

/*
{
    "result": "0"
}
*/
int FormateResultData(std::string& data)
{
    Json::Value root;

    root["result"] = 0;

    data = root.toStyledString();

    return 0;
}

int ParseResultData(std::string data, int& result)
{
    Json::Reader reader;
    Json::Value value;

    if (!reader.parse(data, value)) {
        return -1;
    }

    result = value["result"].asInt();

    return 0;
}

int main(int argc, char** argv)
{
    std::string data;

    FormateBroadcastData(data, "haqiu", "chaosir", 1);

    printf("data(%s)\n", data.c_str());

    std::string ssid;
    std::string password;
    int mode;

    ParseBroadcastData(data, ssid, password, mode);
    
    printf("ssid(%s), password(%s), mode(%d)\n",
            ssid.c_str(), password.c_str(), mode);

    FormateResultData(data);

    printf("data(%s)\n", data.c_str());

    int result = 0;

    ParseResultData(data, result);

    printf("result(%d)\n", result);

    return 0;
}

