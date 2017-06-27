#include <string.h>
#include <stdio.h>



template <class OutIt>
inline int IntToStr(int n, OutIt out)
{
    if (n == 0)
    {
        *out = '0';
        return 1;
    }

    if (n < 0)
    {
        *out++ = '-';
        n = -n;
    }

    char t[24] = "";
    int len = 0;
    while (n)
    {
        t[len++] = (n % 10) + '0';
        n /= 10;
    }

    t[len] = 0;

    for (int k = len - 1; k >= 0; --k)
    {
        *out++ = t[k];
    }

    return len;
}

inline char* int2str(int n, char* buf)
{
    IntToStr(n, buf);

    return buf;
}

int main(int argc, char** argv)
{
	int type = 2;
	char intbuf[24] = {};

	int2str(type, intbuf);

	printf("type(%d)(%s)\n", type, intbuf);

	return 0;
}

