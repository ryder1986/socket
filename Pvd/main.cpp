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
    string str="123";
    while(1){
        cout<<"abc\n";fflush(stdout);
        cin>>str;
        cout<<str+"111"<<endl;fflush(stdout);
        str="";
    //    system(str.data());
    }
}

int main()
{
    thread(bind(handle_cmd)).detach();
    App app;
//    app.start();
    fflush(NULL);
    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}

