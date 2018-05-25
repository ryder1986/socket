#include "cameramanager.h"
void CameraManager::restart_cameras(DataPacket cfg)
{
    decode(cfg);
    stop_cams();//stop all cameras
    start_cams();//start all cameras with new config
}

//wanted index means ,the new camera want to get the position
bool CameraManager::insert_camera(int wanted_index,DataPacket data)
{
    int ret=false;
    if(index_valid(wanted_index)){
        prt(info,"add cam into pos %d  ",wanted_index);
        Camera *c=new Camera(data);
        vector<Camera *>::iterator it_cams;
        cameras.insert(it_cams+wanted_index-1,c);
        vector <DataPacket>::iterator  it_cfg;
        cam_cfgs.insert(it_cfg+wanted_index-1,c->config());
        ret=true;
    }else{
        ret=false;
    }
    return ret;
}
bool CameraManager::delete_camera(int index)
{
    int ret=false;
    if(index_valid(index)){
        prt(info,"deleting cam  %d  ",index);
        delete cameras[index-1];
        vector<Camera *>::iterator it;
        cameras.erase(it+index-1);
        vector <DataPacket>::iterator it_cfg;
        cam_cfgs.erase(it_cfg+index-1);
        ret=true;
    }else{
        ret=false;
    }
    return ret;
}
bool CameraManager::modify_camera(int index,DataPacket data,int mod_type)
{
    bool ret=false;
    if(index_valid(index)){
       Camera *c= cameras[index-1];
       c->modify(data);
       cam_cfgs[index-1]=c->config();
    }
    return ret;
}
