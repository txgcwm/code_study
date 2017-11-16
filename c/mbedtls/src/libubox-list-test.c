#include <stdlib.h>
#include <stdio.h>

#include "libubox/list.h"


typedef struct _Data {
    char data;
    struct list_head list;
} Data;

void decimal_to_binary()
{
    long number, temp;
    Data mylist, *tmp = NULL;
    struct list_head *pos = NULL, *p = NULL;

    INIT_LIST_HEAD(&mylist.list);
    
    printf("please input a number you want to convert:");
    scanf("%ld", &number);
    printf("Decemal number %ld's binary is:", number);
    
    if(number == 0) {
        printf("%ld\n", number);
        return;
    }

    while(number != 0) {
        tmp = (Data *)malloc(sizeof(Data));
        temp = number % 2;
        tmp->data = temp;
        list_add(&(tmp->list), &(mylist.list));
        number = number / 2;
    }

    list_for_each(pos, &mylist.list) {
        tmp = list_entry(pos, Data, list);
        printf("%ld", (long)tmp->data);
    }
    printf("\n");

    list_for_each_safe(pos, p, &mylist.list) {
        tmp = list_entry(pos, Data, list);
        list_del(pos);
        free(tmp);
    }

    if(list_empty(&mylist.list)) {
        printf("The list now is empty!\n");
    }

    return;
}

void binary_to_decimal()
{
    Data mylist, *tmp = NULL;
    struct list_head *pos = NULL, *p = NULL;
    char ch = '0';
    long dec = 1;
    long dec_number = 0;

    INIT_LIST_HEAD(&mylist.list);

    printf("Please input the binary number you want to convert:");
    ch = getchar();

    while((ch == '0') || (ch == '1')) {
        tmp = (Data *)malloc(sizeof(Data));
        tmp->data = ch;
        list_add(&(tmp->list), &(mylist.list));
        ch = getchar();
    }

    list_for_each(pos, &mylist.list) {
        tmp = list_entry(pos, Data,list);
        dec_number += (int)(tmp->data - '0') * dec;
        dec *= 2;
    }

    printf("Decimal number is %ld\n", dec_number);

    list_for_each_safe(pos, p, &mylist.list) {
        tmp = list_entry(pos, Data, list);
        list_del(pos);
        free(tmp);
    }

    if(list_empty(&mylist.list)){
        printf("The list now is empty!\n");
    }

    return;
}

int main(int argc, char **argv)
{    
    int select = 0;

    printf("**********************************************\n");
    printf("** 1.Decimal to binary **\n");
    printf("** 2.Binary to decimal **\n");
    printf("** 0.Exit **\n");
    printf("**********************************************\n");
    printf("Please select which you want to convert:");
    scanf("%d", &select);
    getchar();

    switch(select) {
    case 0:
    	printf("Welcome to use next time!\n");
    	break;

    case 1:
    	decimal_to_binary();
    	break ;

    case 2:
    	binary_to_decimal();
    	break;

    default:
    	printf("Your select is not right!");
    	break;
    }

    return 0;
}

