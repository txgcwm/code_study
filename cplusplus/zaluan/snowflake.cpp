/* 
    snowflake 
     
    ID Éú³É²ßÂÔ 
    ºÁÃë¼¶Ê±¼ä41Î»+»úÆ÷ID 10Î»+ºÁÃëÄÚÐòÁÐ12Î»¡£ 
    0 41 51 64 +-----------+------+------+ |time |pc |inc | +-----------+------+------+ 
    Ç°41bitsÊÇÒÔÎ¢ÃëÎªµ¥Î»µÄtimestamp¡£ 
    ½Ó×Å10bitsÊÇÊÂÏÈÅäÖÃºÃµÄ»úÆ÷ID¡£ 
    ×îºó12bitsÊÇÀÛ¼Ó¼ÆÊýÆ÷¡£ 
    macheine id(10bits)±êÃ÷×î¶àÖ»ÄÜÓÐ1024Ì¨»úÆ÷Í¬Ê±²úÉúID£¬sequence number(12bits)Ò²±êÃ÷1Ì¨»úÆ÷1msÖÐ×î¶à²úÉú4096¸öID£¬ * 
      ×¢Òâµã£¬ÒòÎªÊ¹ÓÃµ½Î»ÒÆÔËËã£¬ËùÒÔÐèÒª64Î»²Ù×÷ÏµÍ³£¬²»È»Éú³ÉµÄID»áÓÐ¿ÉÄÜ²»ÕýÈ· 
*/  
  
#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <sched.h>  
#include <linux/unistd.h>  
#include <sys/syscall.h>  
#include <errno.h>  
#include <linux/types.h>  
#include <time.h>  
#include <stdint.h>  
#include <sys/time.h>  
  
struct  globle {  
    int global_int:12;  
    uint64_t last_stamp;  
    int workid;  
    int seqid;  
};  

struct globle g_info;  
#define   sequenceMask  (-1L ^ (-1L << 12L))

void set_workid(int workid)  
{  
    g_info.workid = workid;  
}

pid_t gettid()  
{  
    return syscall( __NR_gettid );  
}

uint64_t get_curr_ms()  
{  
    struct timeval time_now;  
    gettimeofday(&time_now,NULL);  
    uint64_t ms_time = time_now.tv_sec*1000 + time_now.tv_usec/1000;

    return ms_time;  
}  
  
uint64_t wait_next_ms(uint64_t lastStamp)  
{  
    uint64_t cur = 0;

    do {  
        cur = get_curr_ms();  
    } while (cur <= lastStamp);

    return cur;  
}

int atomic_incr(int id)  
{  
    __sync_add_and_fetch( &id, 1);

    return id;  
}

uint64_t get_unique_id()  
{  
    uint64_t  uniqueId=0;  
    uint64_t nowtime = get_curr_ms();  
    uniqueId = nowtime<<22;  
    uniqueId |= (g_info.workid&0x3ff)<<12;  
  
    if (nowtime < g_info.last_stamp) {  
        perror("error");  
        exit(-1);  
    }

    if (nowtime == g_info.last_stamp) {  
        g_info.seqid = atomic_incr(g_info.seqid)& sequenceMask;  
        if (g_info.seqid == 0) {  
            nowtime = wait_next_ms(g_info.last_stamp);  
        }  
    } else {  
        g_info.seqid  = 0;  
    }

    g_info.last_stamp = nowtime;  
    uniqueId |= g_info.seqid;

    return uniqueId;  
}  

int main(int argc, char** argv)  
{  
    set_workid(100);

    for (;;) {  
        uint64_t unquie = get_unique_id();  
        printf("pthread_id:%u, id [%llu]\n", gettid(), unquie);  
    }  

    return 0;   
}  
