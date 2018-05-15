#include <iostream>
/*
A Demo of a simple tcp server which parse 1 json string a time.
*/

#include "server.h"
#include "tool.h"
using namespace std;
void process_data(Session *clt,char *data,int len)
{
    prt(info,"get  %s",data);
}

int main()
{
    Tcpserver server(50001,process_data);
 //   server.set_ret_func(bind(process_data,placeholders::_1,placeholders::_2));
    cout << "Hello server!" << endl;

    while(1)
        ;
    return 0;
}

