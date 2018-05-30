#ifndef TESTPROCESS_H
#define TESTPROCESS_H
#include "tool.h"
#include "test.h"
#include "videosource.h"
#include "c4processor.h"
#include "videoprocessor.h"
#include "hogprocessor.h"
class TestProcess:public Test
{
public:
   //   TestProcess():src("rtsp://192.168.1.95:554/av0_1")
    TestProcess():src("rtsp://192.168.1.216:8554/test1")
      {
        DataPacket pkt;
        pkt.set_string("ratio","0.7");
        pkt.set_int("step",2);
      //   pro=new PvdC4Processor(pkt);
       pro=new PvdHogProcessor(pkt);
    }


    ~TestProcess()
    {
        delete pro;
        prt(info," quit test process ");
    }
    void fun()
    {
        prt(info,"test %s ",typeid(TestProcess).name());
    }
    void run_process(int t)
    {
        Mat frame;
        while(1){
            this_thread::sleep_for(chrono::milliseconds(10));
            if(src.get_frame(frame)&&frame.cols>0&&frame.rows>0){
                prt(info,"get a frame ");
                vector <Rect> rcts;
                Rect area(0,0,640,480);

                pro->process(frame,rcts,area);
                prt(info,"result %d ",rcts.size());

                if(rcts.size()>0){
                    cv::Rect rc=rcts.front();
                //    prt(info,"%d %d %d %d  ",rc.x,rc.y,rc.width,rc.height);
                    rectangle(frame,rc, cv::Scalar(0,255,255), 1);
                }
                imshow("123",frame);
                waitKey(1);
            }
        }
    }

    void start()
    {
        _start(bind(&TestProcess::run_process,this,placeholders::_1),99);
    }

private:
    VideoSource src;
    VideoProcessor *pro;
    //  Timer1 *tm;
};

#endif // TESTPROCESS_H
