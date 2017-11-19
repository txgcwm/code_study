#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>


static void generateRandom(char *buf, int len)
{
	bzero(buf, len);

	srand((int)time(NULL));

    for (int i = 0; i < len - 1; i++) {
        buf[i] = 'a' + (0 + (int)(26.0 * rand()/(RAND_MAX + 1.0)));
    }

    buf[len - 1] = '\0';

	return;
}

//如果是在一个程序的循环中不断调用这个函数，那么是没有效果的
//虽然也是使用的系统的时间函数来初始化随机数发生器，但程序的
//执行速度太快了，可能执行1000次循环返回的秒数都是一样的
//time返回时间戳
void get_rand_str(char s[],int num)
{
    //定义随机生成字符串表
    char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int i, lstr = strlen(str);
    char ss[2] = {0};

    srand((unsigned int)time((time_t *)NULL));//使用系统时间来初始化随机数发生器

    for(i = 1; i <= num; i++){//按指定大小返回相应的字符串
        sprintf(ss, "%c", str[(rand()%lstr)]);//rand()%lstr 可随机返回0-71之间的整数, str[0-71]可随机得到其中的字符
        strcat(s, ss);//将随机生成的字符串连接到指定数组后面
    }

    return;
}

int main(int argc, char *argv[])
{
    // int len = 20;
    // char buf[20];

    // generateRandom(buf, len);
    
    // printf("The randm String is [ %s ]\n", buf);

    char s[5] = {0};//定义一个可接收1400个字符的字符数组
    get_rand_str(s, 4);
    printf("%s\n", s);

    return 0;
}

 
