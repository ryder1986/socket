#include "videosource.h"
#include <thread>
#include <functional>
VideoSource::VideoSource(string path):t1(bind(&VideoSource::check_point,this)),frame_rate(0)
{
  //  Timer1 t1(bind(&VideoSource::check_point,this));
    t1.start(1000);
    prt(info,"%s",path.data());
    url=path;
    quit_flg=false;
    thread(bind(&VideoSource::run,this)).detach();
}
VideoSource::~VideoSource()
{
    t1.stop();
    quit_flg=true;
    prt(info,"quit video: %s", url.data());
}
void VideoSource::run()
{
    vcap=VideoCapture(url);
  //  this_thread::sleep_for(chrono::milliseconds(1000));
    if(!vcap.isOpened()){
        prt(info,"fail to open %s", url.data());
    }else{
        prt(info,"ok to open %s", url.data());
    }

    if(1){
        frame_wait_time=0;
    }else{
        frame_wait_time=40;
    }

    Mat frame;
    int flag_retry=0;
    while(!quit_flg){
        if( vcap.isOpened()){
            flag_retry=0;
            frame.release();
            bool rt= vcap.read(frame);
            if(!rt){
                cout<<url.data()<<" get frame error!"<<endl;
                prt(info,"get frame fail,restart video capture %s", url.data());
                vcap.release();  vcap=   VideoCapture( url.data());
            }
            if(frame.cols==0){
                vcap.release();
                cout<<"get frame invalid"<<url.data();
                prt(info,"%s get frame error,retrying ... ", url.data());
                continue;
                prt(info,"restarting %s      ", url.data());
            }else{
                frame_rate++;
                lock.lock();
                if(frame_list.size()<3){
                    frame_list.push_back(frame);
                }
                lock.unlock();
                if(frame_wait_time)
                    this_thread::sleep_for(chrono::milliseconds( frame_wait_time));
            }
        }else{
            if(flag_retry++<10){
                this_thread::sleep_for(chrono::milliseconds(100));
            }else{
                this_thread::sleep_for(chrono::seconds(1));
            }
            vcap=VideoCapture( url.data());
            cout<<"open url err:"<<url.data()<<endl;
        }
    }
    prt(info,"thread is ending");
    if( vcap.isOpened())
        vcap.release();
}

