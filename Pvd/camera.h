#ifndef CAMERA_H
#define CAMERA_H
#include "videosource.h"
#include "c4processor.h"
#include "videoprocessor.h"
#include "hogprocessor.h"
#include <mutex>
using namespace cv;
class Camera
{
    typedef struct alg{
        JsonValue pvd_c4;
        JsonValue pvd_hog;
        string selected_alg;
    }alg_t;

    typedef struct Camera_config{
        string url;
        int direction;
        int camera_id;
        string user_name;
        string password;
        string camera_ip;
        int camera_port;
        DataPacket chs;
        //alg_t alg;
    }Config_t;
    Rect detect_rect;
public:
    enum{
        MOD_TYPE_ALG,
        MOD_TYPE_ATTR
    };

    Camera(DataPacket pkt)
    {
        send_check_tick=0;
        quit=false;
        decode(pkt);
        start_cam();
    }

    ~Camera()
    {
        stop_cam();
    }

    DataPacket config()
    {
        return encode();
    }
    bool modify(DataPacket data)
    {

    }

private:
    void restart_processor()
    {
//        mtx.lock();
//        if(processors.size()){
//            for (VideoProcessor *p:processors) {
//                delete p;
//            }
//            processors.clear();
//        }
//        for (DataPacket pkt: cam_cfg.chs) {

//            string str=pkt.get_string("selected_alg");
//            int channel_id=pkt.get_int("channel_id");
//            if(str=="pvd_c4"){
//                processors.push_back(new PvdC4Processor(pkt.get_value(str),channel_id));
//            }else if(str=="pvd_hog"){
//                processors.push_back(new PvdHogProcessor(pkt.get_value(str),channel_id));
//            }
//        }

//        mtx.unlock();
    }

    void start_cam()
    {
        src=new VideoSource(cam_cfg.url);
        restart_processor();

    }

    void stop_cam()
    {
        quit=true;
        prt(info,"stoping camera..");
        prt(info," camera %s stoped",this->src->get_url().data());
        delete src;
        if(processors.size()){
            for (VideoProcessor *p: processors) {
                delete p;
            }
            processors.clear();
        }
        src=NULL;
        prt(info,"delete done");
    }

    DataPacket encode()
    {
        DataPacket pkt;
        pkt.set_string("url",cam_cfg.url);
        pkt.set_int("direction",cam_cfg.direction);
        pkt.set_int("camera_id",cam_cfg.camera_id);
        pkt.set_string("user_name",cam_cfg.user_name);
        pkt.set_string("password",cam_cfg.password);
        pkt.set_string("camera_ip",cam_cfg.camera_ip);
        pkt.set_int("camera_port",cam_cfg.camera_port);
        pkt.set_pkt("channel",DataPacket(cam_cfg.chs));

        return pkt;
    }

    void decode(DataPacket pkt)
    {
        cam_cfg.url = pkt.get_string("url");
        cam_cfg.direction=pkt.get_int("direction");
        cam_cfg.camera_id=pkt.get_int("camera_id");
        cam_cfg.user_name=pkt.get_string("user_name");
        cam_cfg.password=pkt.get_string("password");
        cam_cfg.camera_ip=pkt.get_string("camera_ip");
        cam_cfg.camera_port=pkt.get_int("camera_port");
        cam_cfg.chs=pkt.get_pkt("channel");
    }

    void send_out(string ba)
    {
        bool send_flag=false;
        if(send_check_tick++>50){
            send_flag=true;
            send_check_tick=0;

        }

    }

protected:
    void run();
private:
    int threadid;
    VideoSource *src;
    vector <VideoProcessor *> processors;
    Config_t cam_cfg;
    bool quit;
    mutex mtx;
    int send_check_tick;

};

#endif // CAMERA_H
