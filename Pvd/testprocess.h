#ifndef TESTPROCESS_H
#define TESTPROCESS_H
#include "tool.h"
#include "test.h"
class TestProcess:public Test
{
public:
    TestProcess();
    void start()
    {
        prt(info,"start here");
    }

};

#endif // TESTPROCESS_H
