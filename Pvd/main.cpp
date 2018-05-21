#include <iostream>
/*
A Demo of a simple tcp server which parse 1 json string a time.
*/

//#include "server.h"
#include "tool.h"
#include "app.h"
using namespace std;
void handle_cmd()
{
    string str;
    while(1){
        cin>>str;
        cout<<str<<endl;
        system(str.data());
    }
}

int main()
{
    //Tcpserver server(50001,process_data);
 //   server.set_ret_func(bind(process_data,placeholders::_1,placeholders::_2));
    //cout << "Hello server!" << endl;

    thread(bind(handle_cmd)).detach();
    App app;
    app.start();
    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}

