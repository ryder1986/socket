#include "camera.h"
//Camera::Camera(DataPacket pkt)
//{
//    send_check_tick=0;
//    quit=false;
//    decode(pkt);
//    start_cam();
//}

//bool Camera::modify_alg(JsonValue jv)
//{

//    DataPacket pkt(jv);

//    cam_cfg.chs=pkt;

//    restart_processor();
//    return true;
//}

//void Camera::modify_attr(JsonValue v)
//{
//    DataPacket pkt(v);
//    int di= pkt.get_int("direction");
//    int no= pkt.get_int("camera_id");
//    cam_cfg.direction=di;
//    cam_cfg.camera_id=no;
//}

void Camera::run()
{
    int i=0;
    int sz=0;
    Mat frame;
    vector<JsonValue> channel_data;
    VideoProcessor *pro;
    while(!quit){
        mtx.lock();
        if(src->get_frame(frame)&&frame.cols>0&&frame.rows>0){
          //  frame_rate++;
            // bool ret=process(frame,rst);
#if 0
            foreach (VideoProcessor *processor, processors) {
                bool ret=processor->process(frame);
                send_out(processor->get_rst());
            }
#else
            channel_data.clear();

            sz=processors.size();
            for(i=0;i<sz;i++){
                pro= processors[i];
               // resize(frame,frame,Size(frame.cols / 2, frame.rows / 2),CV_INTER_LINEAR);
                pro->process(frame);
                JsonValue v=DataPacket((pro->get_rst())).value();
                DataPacket pkt;
                pkt.set_value("result",v);
                pkt.set_int("channel_id",pro->get_id());

                channel_data.push_back(pkt.value());
            }
          //  send_out(DataPacket(channel_data).data());
            DataPacket pkt1;
            pkt1.set_value("rt_data",DataPacket(channel_data).value());
            send_out(pkt1.data());
#endif
        }else{
            //prt(info,"get no frame");
        }
        mtx.unlock();
        //QThread::msleep(1);
    }
    // QThread::msleep(10);
}
