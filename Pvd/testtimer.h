#ifndef TESTTIMER_H
#define TESTTIMER_H

#include <iostream>
#include "tool.h"
class TestTimer
{
public:
    TestTimer();
    void start()
    {
        prt(info,"start test %s",typeid(TestTimer).name());
        Timer2 t2;
        //   t2.AsyncWait(1000,bind(&TestTimer::fun,this,placeholders::_1),3);
        // t2.SyncWait(1000,bind(&TestTimer::fun_0,this));
        t2.SyncWait(1000,bind(&TestTimer::fun,this,placeholders::_1,placeholders::_2),3,"test");
        prt(info,"end test %s",typeid(TestTimer).name());
    }
private:
    void fun(int t,string abc)
    {
        prt(info,"%d",t);
        prt(info,"%s",abc.data());
    }
    void fun_0()
    {
        prt(info,"aaa");
    }
};

#endif // TESTTIMER_H
