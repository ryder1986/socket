#include "common.h"
#include "g_define.h"
#include "client_obj.h"
#include "file_op.h"
char cfg_dir[]="./cfg/";
char *cfg_file_name(char *filename,int class_type)
{
	switch(class_type){
	case CLASS_mCamParam:
		sprintf(filename,"./cfg/%s","cam_param");
		break;
	case CLASS_mDetectDeviceConfig:
		sprintf(filename,"./cfg/%s","dev_config");
		break;
	case CLASS_mCamDetectParam:
		sprintf(filename,"./cfg/%s","det_param");
		break;

	default :
		prt(info,"unsupported save class %d",class_type);
		break;
	}
	return filename;
}
void save_obj(unsigned char * p_obj,int class_type,int index)
{
	int pos=0;
	char filename[FILE_NAME_LENGTH];
	int len=get_obj_len(class_type);
	if(class_type==CLASS_mDetectDeviceConfig){
		pos=0;
	}else{
		pos=index*len;
	}
	char *p_dst=NULL;
	switch(class_type){
	case CLASS_mCamParam:
		//memcpy(&g_cam_cfg[index],p_obj,len);
	//	p_dst=(char *)&g_cam_cfg[index].cam_param;
		sprintf(filename,"./cfg/%s","cam_param");
		break;
	case CLASS_mDetectDeviceConfig:
	//	p_dst=(char *)&g_dev_cfg;
		sprintf(filename,"./cfg/%s","dev_config");
		break;
	case CLASS_mCamDetectParam:
	//	p_dst=(char *)&g_cam_cfg[index].det_param;
		sprintf(filename,"./cfg/%s","det_param");
		//prt(info,"===11111>%d====>>> %d",g_cam_cfg[index].det_param.area.vircoordinate[2].x,((mCamDetectParam*)p_obj)->area.vircoordinate[2].x);
		break;

	default :
		prt(info,"unsupported save class %d",class_type);
		break;
	}
//	memcpy(p_dst,p_obj,len);
	save_buf(filename,(char *)p_obj,pos,len);
}
void load_obj(unsigned char * p_obj,int class_type,int index)
{
	int pos=0;
	char filename[FILE_NAME_LENGTH];
	int len=get_obj_len(class_type);
	if(class_type==CLASS_mDetectDeviceConfig){
		pos=0;
	}else{
		pos=index*len;
	}
	char *p_dst=NULL;
	switch(class_type){
	case CLASS_mCamParam:
		//memcpy(&g_cam_cfg[index],p_obj,len);
	//	p_dst=(char *)&g_cam_cfg[index].cam_param;
		sprintf(filename,"./cfg/%s","cam_param");
		break;
	case CLASS_mDetectDeviceConfig:
	//	p_dst=(char *)&g_dev_cfg;
		sprintf(filename,"./cfg/%s","dev_config");
		break;
	case CLASS_mCamDetectParam:
	//	p_dst=(char *)&g_cam_cfg[index].det_param;
		sprintf(filename,"./cfg/%s","det_param");
		//prt(info,"===11111>%d====>>> %d",g_cam_cfg[index].det_param.area.vircoordinate[2].x,((mCamDetectParam*)p_obj)->area.vircoordinate[2].x);
		break;

	default :
		prt(info,"unsupported save class %d",class_type);
		break;
	}
//	memcpy(p_dst,p_obj,len);
	load_buf(filename,(char *)p_obj,pos,len);
}
