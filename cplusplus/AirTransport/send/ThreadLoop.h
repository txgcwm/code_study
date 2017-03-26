#ifndef __THREADLOOP_H__
#define __THREADLOOP_H__

#include <pthread.h>
#include <string>



class CThreadLoop
{
public:
	CThreadLoop(std::string name);
	~CThreadLoop();

	virtual bool StartThread();
	virtual bool StopThread();

	virtual int WaitForSleep(int timeout_ms);
	virtual void EventHandleLoop() = 0;

private:
	static void *ThreadProc(void *argv);
	
private:
	std::string				m_name;
	pthread_t 				m_tid;

    pthread_mutex_t 		m_lock;
	pthread_cond_t 			m_condition;

    bool					m_start;
};


#endif


