#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H


#include "pvd.h"
#include "camera.h"
/* recive packet: value of cameras[vector of datapacket]
    "cameras": [
        {
        },
        {
        }
     ]
*/
class CameraManager
{
public:
    enum{
        MODIFY_ALG,
        MODIFY_URL,
        MODIFY_DIRECTION,
        MODIFY_ARGS
    };

    CameraManager(DataPacket cfg)
    {
        cameras.clear();
        decode(cfg);
        start_cams();
    }
    ~CameraManager()
    {
        stop_cams();
    }
    DataPacket config()
    {
        return encode();
    }
public:
    void restart_cameras(DataPacket cfg);
    bool insert_camera(int index,DataPacket cfg);
    bool delete_camera(int index);
    bool modify_camera(int index,DataPacket v,int mod_type);
    void modify_attr(int index,DataPacket v);
private:
    void start_cams()
    {
        for (DataPacket v: cam_cfgs)
        {
            cameras.push_back(new Camera(v));
        }
    }

    void stop_cams()
    {
        for (Camera *c: cameras)
        {
            delete c;
        }
        cameras.clear();
    }

    bool index_valid(int index)
    {
        if(index<1||index>cameras.size()){
            prt(fatal,"index %d out of range(1-%d)",index,cameras.size());
            return false;
        }
        return true;
    }

    void decode(DataPacket pkt)
    {
        cam_cfgs.clear();
        for (DataPacket v: pkt.array_packet())
        {
            cam_cfgs.push_back(v);
        }
    }
    DataPacket encode()
    {
        return  DataPacket(cam_cfgs);
    }

    //trans config to json value
    //    inline JsonValue cfg_2_jv()
    //    {
    //        vector<JsonValue> vec;
    //        for (JsonValue v: cam_cfgs) {
    //            vec.push_back(v);
    //        }
    //        DataPacket pkt(vec);
    //        return pkt.value();
    //    }

    //    inline void jv_2_cfg(JsonValue jv)
    //    {
    //        DataPacket pkt(jv);
    //        cam_cfgs.clear();
    //        foreach (JsonValue v, pkt.array_value())
    //        {
    //            cam_cfgs.push_back(v);
    //        }
    //    }

private:
    vector <Camera *>cameras;
    vector <DataPacket> cam_cfgs;
};

#endif // CAMERAMANAGER_H
