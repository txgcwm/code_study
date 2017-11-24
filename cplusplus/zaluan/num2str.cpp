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

template<class TO,class TI>
TO convert(const TI& t)
{
    std::stringstream stream;
    TO result;

    stream<<t;
    stream>>result;

    return result;
}

int main(int argc, char** argv)
{
    printf("%s\n", num2str(45).c_str());
    printf("%f\n", convert<double>("12.5"));

    return 0;
}