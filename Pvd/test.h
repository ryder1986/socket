#ifndef TEST_H
#define TEST_H
#include <functional>
#include "tool.h"
   using namespace std;

class Test
{
public:
    Test();
//    template<typename callable,class...arguments>
//    void _start(callable&& f, arguments&&... args)
//    {
//        //prt(info,"test start>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

//        //prt(info,"%s",  typeid(f).name());
//        std::function<typename std::result_of<callable(arguments...)>::type()> task
//            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
//        task();

//        //prt(info,"test end<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
//    }
    virtual void start()
    {

    }

private:

};

#endif // TEST_H
