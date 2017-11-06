#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "sigslot.h"



using namespace std;

class CSender {
public:
    sigslot::signal2<string, int> m_pfnsigDanger;

    void Panic() {
        static int nVal = 0;
        char szVal[20] = {0};

        sprintf(szVal, "help--%d", nVal);

        m_pfnsigDanger(szVal, nVal++);
    }
};

class CReceiver: public sigslot::has_slots<> {
public:
    void OnDanger(string strMsg, int nVal) {
        cout << strMsg.c_str() << " ==> " << nVal << endl;
    }
};

int main(int argc, char** argv)
{
    int count = 5;
    CSender sender;
    CReceiver recever;

    sender.m_pfnsigDanger.connect(&recever, &CReceiver::OnDanger);

    cout << "connect succ!" << endl;

    while(count-- > 0) {
        cout << "in while..." << endl;

        sender.Panic();
        sleep(2);

        cout << "end of sleep" << endl;
    }

    sender.m_pfnsigDanger.disconnect(&recever);

    return 0;
}

