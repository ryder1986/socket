#ifndef TEST_H
#define TEST_H
#include <functional>
#include "tool.h"
   using namespace std;

class Test
{
public:
    Test();


//    void start(callable&& f, arguments&&... args)
//    {

//        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

///*        function<typename std::result_of<callable(arguments...)>::type()> task
//            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

//           task()*/;
//        prt(info," start ok");
//    }
    template<typename callable,class...arguments>
    void _start(callable&& f, arguments&&... args)
    {

        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        task();
    }
    virtual void start()
    {

    }

private:

};

#endif // TEST_H
