#include <sys/time.h>
#include <errno.h>

#ifdef  __linux__
	#include <sys/prctl.h>
#endif

#include "Log.h"
#include "ThreadLoop.h"



namespace Base {


CThreadLoop::CThreadLoop(std::string name)
: m_name(name)
, m_start(false)
{
}

CThreadLoop::~CThreadLoop()
{
	StopThread();
}
	
bool CThreadLoop::StartThread()
{
	int res = false;

	if(m_start) {
		INFO("thread(%s) has started!\n", m_name.c_str());
		return true;
	}

	pthread_mutex_init(&m_lock, NULL);
	pthread_cond_init(&m_condition, NULL);

	pthread_attr_t attr;

	pthread_attr_init(&attr);  
     
    pthread_attr_setstacksize(&attr, 1024 * 1024);

    if(pthread_create(&m_tid, &attr, ThreadProc, this) == -1) {  
 		ERROR("can not create thread\n");
 		res = false;
    } else {
    	m_start = true;
    	INFO("start thread(%s) success!\n", m_name.c_str());
    }

    pthread_attr_destroy(&attr);

    return res;
}

bool CThreadLoop::StopThread()
{
	if(!m_start) {
		INFO("thread(%s) has stopped!\n", m_name.c_str());
		return true;
	}

	pthread_cond_broadcast(&m_condition);

	pthread_join(m_tid, NULL);

    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&m_condition);

    m_start = false;

    INFO("stop thread(%s) success!\n", m_name.c_str());

	return true;
}

/*****************************************************************************
int pthread_cond_timedwait(pthread_cond_t *restrict cond,
pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);

abstime是一个绝对时间，struct timespce的原型为：

struct timespec {
	__time_t tv_sec;        // Seconds
	long int tv_nsec;       // Nanoseconds
};

其中tv_sec是秒，tv_nsec是纳秒（即1000,000,000分之一秒）

struct timespec abstime;

abstime.tv_nsec = (timeout_ms % 1000) * 1000000;
abstime.tv_sec = time(NULL) + timeout_ms / 1000;

pthread_cond_timedwait(&_read_cond, &_read_mutex, &abstime);

以上代码有问题，主要是因为time(NULL)的返回结果的精度是秒级的，那么如果当前时间是m秒＋n毫秒，
那么实际等待的时间只是timeout_ms – n，且还有可能发生n > timeout_ms的情况，这种情形下，
如果这段代码处在一处while循环内，则会造成大量的pthread_cond_timedwait系统调用，并造成
大量的context switch，系统CPU会占用很高。
*****************************************************************************/
int CThreadLoop::WaitForSleep(int timeout_ms)
{
	int ret = -1;
	struct timespec abstime;
	struct timeval now;

	gettimeofday(&now, NULL);

	int nsec = now.tv_usec * 1000 + (timeout_ms % 1000) * 1000000;
	abstime.tv_nsec = nsec % 1000000000;
	abstime.tv_sec = now.tv_sec + nsec / 1000000000 + timeout_ms / 1000;

	int err = pthread_cond_timedwait(&m_condition, &m_lock, &abstime);
	if(err == ETIMEDOUT) {
		ret = 0;
	}
	
	return ret;
}

void CThreadLoop::EventHandleLoop()
{
	while(1) {
		WaitForSleep(0);
	}
	
	return;
}

void *CThreadLoop::ThreadProc(void *argv)
{
	CThreadLoop *thiz = (CThreadLoop *)argv;

	if(thiz != NULL) {
#ifdef __linux__
		prctl(PR_SET_NAME, thiz->m_name.c_str());
#endif
		thiz->EventHandleLoop();
	}

	return NULL;
}

} // end namespace


