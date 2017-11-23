#include <stdio.h>

#include <sstream>
#include <string>

template <typename T>
std::string num2str(T num)
{
    std::stringstream ss;

    ss<<num;

    return ss.str();
}

int main(int argc, char** argv)
{
    printf("%s\n", num2str(45).c_str());

    return 0;
}