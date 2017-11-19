#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <ucontext.h>


#define FRAME_MAX_NUM 100

void print_backtrace()
{
  void* frame_ptrs[FRAME_MAX_NUM];
  int f_num;
  char **frame_symbols;
  int i;

  f_num = backtrace(frame_ptrs, FRAME_MAX_NUM);
  frame_symbols = backtrace_symbols(frame_ptrs, f_num);
  for (i = 0; i < f_num; i++) {
    fprintf(stderr, "%s\n", frame_symbols[i]);
  }

  free(frame_symbols);

  return ;
}

void print_context_backtrace(void *context)
{
    void **ctx_bp = NULL;
    void *ctx_ip = NULL;

#ifdef __GLIBC__
    Dl_info dlinfo;
    ucontext_t *uctx = (ucontext_t*)context;

#if defined(__i386__)
    ctx_bp = (void**)uctx->uc_mcontext.gregs[REG_EBP];
    ctx_ip = (void*)uctx->uc_mcontext.gregs[REG_EIP];
#elif defined(__x86_64__)
    ctx_bp = (void**)uctx->uc_mcontext.gregs[REG_RBP];
    ctx_ip = (void*)uctx->uc_mcontext.gregs[REG_RIP];
#elif defined(__arm__)
    ctx_bp = (void**)uctx->uc_mcontext.arm_fp;
    ctx_ip = (void*)uctx->uc_mcontext.arm_pc;
#endif

    memset(&dlinfo, 0, sizeof(dlinfo));

    while (NULL != ctx_bp && NULL != ctx_ip) {
      if (0 == dladdr(ctx_ip, &dlinfo)) {
        break; 
      }

      fprintf(stderr, "%s(%s+%u) [%p]\n", dlinfo.dli_fname, dlinfo.dli_sname, 
              (unsigned int)((char*)ctx_ip - (char*)dlinfo.dli_saddr), ctx_ip);

      ctx_ip = ctx_bp[1];
      ctx_bp = (void**)ctx_bp[0];
    }
#endif /* __GLIBC__ */

    print_backtrace();

    return ;
}

    // clock_t  si_utime;    /* User time consumed */
    // clock_t  si_stime;    /* System time consumed */
    // sigval_t si_value;    /* Signal value */
    // int      si_int;      /* POSIX.1b signal */
    // void    *si_ptr;      /* POSIX.1b signal */
    // int      si_overrun;   Timer overrun count; POSIX.1b timers 
    // int      si_timerid;  /* Timer ID; POSIX.1b timers */
    // void    *si_addr;     /* Memory location which caused fault */
    // long     si_band;     /* Band event (was int in
    //                          glibc 2.3.2 and earlier) */
    // int      si_fd;       /* File descriptor */
    // short    si_addr_lsb;

static void signal_handle(int sign, siginfo_t *siginfo, void *context)
{
  struct sigaction sigact;
    
  memset(&sigact, 0, sizeof(sigact));
    
  (void)siginfo;

  printf("Fatal signal(%d) catched.\n", sign);
  printf("signo: %d\n", siginfo->si_signo);
  printf("errno: %d\n", siginfo->si_errno);
  printf("code: %d\n", siginfo->si_code);
  // printf("trapno: %d\n", siginfo->si_trapno);
  printf("pid: %d\n", siginfo->si_pid);
  printf("uid: %d\n", siginfo->si_uid);
  printf("status: %d\n", siginfo->si_status);
  printf("addr: %p\n", siginfo->si_addr);

  print_context_backtrace(context);

  sigact.sa_handler = SIG_DFL;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags = 0;
  sigact.sa_restorer = NULL;

  sigaction(sign, &sigact, NULL);

  kill(getpid(), sign);

  return;
}

void signal_set()
{
  struct sigaction sigact;

  memset(&sigact, 0, sizeof(sigact));

  sigemptyset(&sigact.sa_mask);

  sigact.sa_flags = SA_SIGINFO; /* set SA_SIGINFO to get more information */
  sigact.sa_sigaction = signal_handle;
  sigact.sa_restorer = NULL;

#ifdef SIGINT
    sigaction(SIGINT, &sigact, NULL);
#endif /* SIGINT */
#ifdef SIGTERM
    sigaction(SIGTERM, &sigact, NULL);
#endif /* SIGTERM */
#ifdef SIGHUP
    sigaction(SIGHUP, &sigact, NULL);
#endif /* SIGHUP */
#ifdef SIGPIPE
    sigaction(SIGPIPE, &sigact, NULL);
#endif /* SIGHUP */
#ifdef SIGQUIT
    sigaction(SIGQUIT, &sigact, NULL);
#endif /* SIGQUIT */
#ifdef SIGILL
    sigaction(SIGILL, &sigact, NULL);
#endif /* SIGILL */
#ifdef SIGABRT
    sigaction(SIGABRT, &sigact, NULL);
#endif /* SIGABRT */
#ifdef SIGFPE
    sigaction(SIGFPE, &sigact, NULL);
#endif /* SIGFPE */
#ifdef SIGSEGV
    sigaction(SIGSEGV, &sigact, NULL);
#endif /* SIGSEGV */
#ifdef SIGBUS
    sigaction(SIGBUS, &sigact, NULL);
#endif /* SIGBUS */
#ifdef SIGSYS
    sigaction(SIGSYS, &sigact, NULL);
#endif /* SIGSYS */
#ifdef SIGXCPU
    sigaction(SIGXCPU, &sigact, NULL);
#endif /* SIGXCPU */
#ifdef SIGXFSZ
    sigaction(SIGXFSZ, &sigact, NULL);
#endif /* SIGXFSZ */
#ifdef SIGIOT
    sigaction(SIGIOT, &sigact, NULL);
#endif /* SIGIOT */

  return;
}

int main(int argc, char **argv)
{
  signal_set();

  for(int i = 0; i < 50; i++) {
    if((i + 1) % 3 == 0) {
      // char *p = NULL;
      // *p = 78;
      char *p = (char *)malloc(60);
      free(p);

      p[60] = 123456; 
    }

    sleep(1);
  }

  return 0;
}


// g++ -rdynamic signal.cpp -ldl