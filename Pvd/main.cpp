#include <iostream>
#include "videosource.h"
#include "testtimer.h"
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
    //    while(1){
    //        cout<<"abc\n";fflush(stdout);
    //        cin>>str;
    //        cout<<str+"111"<<endl;fflush(stdout);
    //        str="";
    //    //    system(str.data());
    //    }
}
void test_fun()
{
    prt(info,"call %s",__FUNCTION__);

}
void test_fun1(int t)
{
    prt(info,"call %s : %d",__FUNCTION__,t);

}
int main()
{

    App app;
    app.start();

    //    TestTimer().start();
    //    while (1) {
    //            this_thread::sleep_for(chrono::milliseconds(100));
    //    }

    //    MediaSource s("rtsp://192.168.1.95:554/av0_1");
    //    Mat frame;
    //    while(1){
    //        this_thread::sleep_for(chrono::milliseconds(100));
    //        bool rst=s.get_frame(frame);
    //        if(rst){
    //            prt(info,"ok");
    //        }else{
    //            prt(info,"err");
    //        }
    //    }
    //    while(1)
    //        ;
    //    thread(bind(handle_cmd)).detach();
    //    App app;
    //    app.start();

    //    //    Timer1 t1(bind(test_fun));
    //    //  t1.start(1000);
    //    //   prt(info,"looop begin");
    //    VideoSource src("rtsp://192.168.1.95:554/av0_1");

    //    Timer2 t2(bind(test_fun));
    //    t2.AsyncWait(10,bind(test_fun1,1),1);

    //    Mat f1;
    //    while(1){
    //        this_thread::sleep_for(chrono::milliseconds(1000));
    //        bool rst=src.get_frame(f1);
    //        if(rst){
    //            prt(info,"ok");
    //        }else{
    //            prt(info,"err");
    //        }
    //    }
    //   t2.SyncWait(3000,bind(test_fun),NULL);

    //   prt(info,"looop begin");
    //Mat frame;
    //Timer1 t2(bind(src.get_frame));
    //t2.start(1000);

    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}

