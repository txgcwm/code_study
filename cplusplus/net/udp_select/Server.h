#ifndef __SERVER_H__
#define __SERVER_H__

#include "ThreadLoop.h"


class CServer : public CThreadLoop
{
public:
    CServer();
    ~CServer();

    bool Start(int port);
    bool Stop();
    
    virtual void EventHandleLoop();

private:
	int m_port;
};

#endif