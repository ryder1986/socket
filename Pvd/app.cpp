#include "app.h"

App::App():str_stream("")
{
    static Tcpserver server1(clients1,12345,bind(&App::process_data,
                                 this,placeholders::_1,
                                 placeholders::_2,
                                 placeholders::_3));
    static Tcpserver server2(clients2,10000,bind(&App::process_data,
                                 this,placeholders::_1,
                                 placeholders::_2,
                                 placeholders::_3));
}

