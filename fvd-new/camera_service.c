/*
 * author ryder jia
 * date 2016年10月11日
 * File introduction: 相机控制接口的实现
 * service init为业务初始化
 * 相机的open，close，control，他们为保证非阻塞的特性，均实现为尽快返回。
 * 主要业务处理函数为process fun
 * 看门狗用于定期监控和重连
 */
#include <stdio.h>
#include <unistd.h>
#include "camera_service.h"
#include "common.h"
#include "cam_net.h"
#include "cam_alg.h"

#include "cam_net.h"

#include "cam_codec.h"
#include "glplayer.h"
#include "h264_stream_file.h"
#include "client_net.h"
/*

 * 代表一个相机的全部资源或者信息
 * */
typedef struct camera_info {
	int index;
//	int stop_run;//TODO: lock this
	pthread_mutex_t run_lock;
	int open_flg;
	int cam_running_state;
	int cmd;
	unsigned char ip[16];
	unsigned char name[16];
	unsigned char passwd[16];
	int port;
	pthread_mutex_t cmd_lock;
	m_cam_context * p_cam;
#ifdef USE_FILE
	m_h264_file_common h264_file_common;
#endif
#ifdef PLAY_BACK
	m_gl_common gl_common;
#endif
	m_timed_func_data *p_data;
	unsigned char *oubuf;
	unsigned char *oubufu;
	unsigned char *oubufv;
	int watchdog_value;
} m_camera_info;
enum{
	CAM_RUNNING,
	CAM_STOPED
};
enum{
	DOG_HUNGRY_STATE,
	DOG_FULL_STATE
};
//#define CAMERA_NUMBER 4
#define CHECK_DURATION 1000//US
#define WATCHDOG_CHECK_DURATION 1000*1000*10//10s
#define FRAME_DURATION 100000//US
m_camera_info cam_info[ACTIVE_CAM_NUM];
/* Function Description
 * name:
 * return:
 * args:
 * comment:增加一个看门狗，十秒没有回调就重连。
 * todo:
 */
void *process_fun(int index, char *data, int size)
{


	int ret=0;
	//prt(info,"running %d",index);
	m_camera_info *p_info = &cam_info[index];
//	prt(info, "call back %d", p_info->index);
	p_info->watchdog_value=DOG_FULL_STATE;
	pthread_mutex_lock(&p_info->cmd_lock);

#ifdef PLAY_BACK
	if (p_info->gl_common.window_id < 0)
	{
		//	prt(info,"start  gl window");
		start_gl_window(&p_info->gl_common.window_id);
	}
#endif
	//	prt(info, "call back %d", p_info->index);
#ifdef USE_FILE
	//	prt(info,"gettiing file");
	get_file_264buf1(&p_info->h264_file_common);
	//	prt(info,"gettiing file done");
	data =(char *) p_info->h264_file_common.h264_pkt.data;
    size =p_info->h264_file_common.h264_pkt.size;
    prt(info,"get %d %d",index,size);
#else
	//	p_info->codec_common.av_pkt.data =(uint8_t *) data;
	//	p_info->codec_common.av_pkt.size = size;
#endif
	h264_decode(index, data, size, &p_info->oubuf, &p_info->oubufu,
			&p_info->oubufv);
#ifdef PLAY_BACK
	copy_frame(p_info->oubuf, p_info->oubufu,
			p_info->oubufv, WIDTH, HEIGHT,
			(unsigned char *) p_info->gl_common.bf_src);
	//	memset(p_info->gl_common.bf_src,0,WIDTH*HEIGHT*3/2);

	diaplay_frames(p_info->gl_common.bf_src, WIDTH, HEIGHT,
			p_info->gl_common.bf_dst, p_info->gl_common.window_id);
#endif
	 if((ret=run_alg(p_info->index, p_info->oubuf, p_info->oubufu, p_info->oubufv))>0 ){
		 cam_set_shutter(index, get_mCamDetectParam(index)->detectparam[ret -1].shutterMax);
	     reset_alg(index);
	 }

	 client_output(p_info->index);
  //	 submit_sig_data(&p_channel_rst[index],index);
	 	//send_sig_traffic_data(index);
		//	send_client_data();
		//send_sig_machine_data();
	 pthread_mutex_unlock(&p_info->cmd_lock);
}

int get_cmd(int index)
{
	return cam_info[index].cmd;
}
void set_cmd(int cmd, int index)
{
//	prt(info,"cam %d  set cmd to %d",index,cmd);
	cam_info[index].cmd = cmd;
}
void *file_process_fun(int *index, char *data, int size)
{
	m_camera_info *p_info = (m_camera_info *) data;
	process_fun(*index, data, size);
//	start_detached_func((void *)real_process_fun,data);
}
#include "cam_alg.h"
/* Function Description
 * name:
 * return:
 * args:
 * 		index是相机编号，重新打开一个已经打开的相机的行为会被视为ctrl中 的reset。
 * 		ip port username passwd
 * comment:
 * 		非阻塞的打开相机码流，如果打开失败了，状态依旧设置为打开，会定期重连
 * todo:
 */
int camera_open(int index, char ip[], int port, char username[], char passwd[])
{
	int ret=-1;
	prt(camera_msg,"open camera %d",index);
	//如果处于打开状态，就直接reset一下好了
	if (cam_info[index].open_flg) {
		camera_ctrl(CAMERA_CONTROL_RESET, index, 0, NULL);
		return 0;
	}
	//准备算法库
	open_alg(index);
	//准备解码库
	open_h264_decoder(index);
#ifdef USE_FILE
	char filename[]="/mediafiles/test.2640";
	//start_gl_window(&cam_info[index].gl_common.window_id);
	memcpy(cam_info[index].h264_file_common.file_name,filename,sizeof(filename));
//	cam_info[index].h264_file_common.h264_pkt=&cam_info[index].codec_common.av_pkt;
	open_h264_file(&cam_info[index].h264_file_common);
	cam_info[index].p_data=regist_timed_func(FRAME_DURATION,(void *)file_process_fun,(void *)&index);
	start_timed_func(cam_info[index].p_data);

#else
	//prt(info,"ip %s ",ip);
	memset(cam_info[index].ip,0,16);
	memset(cam_info[index].name,0,16);
	memset(cam_info[index].passwd,0,16);
	memcpy(cam_info[index].ip, ip, strlen(ip));
//	prt(info,"ip %s ",cam_info[index].ip);
	memcpy(cam_info[index].name, username, strlen(username));
	memcpy(cam_info[index].passwd, passwd, strlen(passwd));
	cam_info[index].port = port;

//	cam_info[index].p_cam = prepare_camera((char *) cam_info[index].ip,
//			cam_info[index].port, (char *) cam_info[index].name,
//			(char *) cam_info[index].passwd, (void *) frame_process_fun,
//			&cam_info[index]);
	ret=net_open_camera(index, (char *) cam_info[index].ip, cam_info[index].port,
			(char *) cam_info[index].name, (char *) cam_info[index].passwd,
			(void *) process_fun, &cam_info[index]);
//	open_camera(cam_info[index].p_cam);
	if(!ret){
		cam_info[index].cam_running_state = CAM_RUNNING;
	}else{
		cam_info[index].cam_running_state = CAM_STOPED;
	}
#endif
	cam_info[index].open_flg = 1;
	return 0;
}
int camera_close(int index)
{
	prt(info,"close camera %d",index);
	m_camera_info *p_info = (m_camera_info *) &cam_info[index];
#ifdef PLAY_BACK
	stop_gl_window(&p_info->gl_common.window_id);
	p_info->gl_common.window_id=-1;
#endif
//		close_h264_file(&h264_file_common);
//		close_h264_decoder1(&codec_common);

#ifdef USE_FILE
	stop_timed_func(cam_info[index].p_data);
	close_h264_file(&cam_info[index].h264_file_common);
#else
	net_close_camera(index);
//	close_camera(cam_info[index].p_cam);
	if (cam_info[index].p_cam != NULL) {
		free(cam_info[index].p_cam);
	}
#endif
	close_h264_decoder(index);
	release_alg(index);
	cam_info[index].open_flg = 0;
	cam_info[index].cam_running_state = CAM_STOPED;
	return 0;
}
int camera_ctrl(int cmd, int index, int blocked, void *data)
{
	pthread_mutex_lock(&cam_info[index].cmd_lock);
	set_cmd(cmd, index);
	pthread_mutex_unlock(&cam_info[index].cmd_lock);
	return 0;
}

void *watchdog_func(void *data)
{
	m_camera_info *info = (m_camera_info *) data;
	if(info->watchdog_value==DOG_HUNGRY_STATE&&info->open_flg){
		prt(camera_msg,"watch dog for cam %d:camera loop stopped,resetting...",info->index);
		camera_ctrl(CAMERA_CONTROL_RESET,info->index,1,NULL);
		info->cam_running_state = CAM_STOPED;
	}else{
		info->cam_running_state = CAM_RUNNING;
	//	prt(camera_msg,"watch dog for cam %d:camera running normally",info->index);
	}
	info->watchdog_value = DOG_HUNGRY_STATE;
}
int get_cam_running_state(int index)
{
	return cam_info[index].cam_running_state ;
}
/* Function Description
 * name:
 * return:
 * args:data指的是每个相机的资源结构体。
 * comment:循环检测命令的到来，执行完成后立即重置命令。
 * todo:
 */
void *camera_main(void *data)
{
	int ret;
	m_camera_info *info = (m_camera_info *) data;
	int index = info->index;
	pthread_mutex_lock(&info->cmd_lock);
	switch (get_cmd(info->index)) {
	case CAMERA_CONTROL_NULL:
		break;
	case CAMERA_CONTROL_RESET:
//		break;
#ifndef USE_FILE
		prt(info, "reseting............ camera %d",index);
	//	pthread_mutex_lock(&info->cmd_lock);
		//	close_camera(info->p_cam);
		//open_camera(info->p_cam);
		prt(info, "closing camera %d",index);
		net_close_camera(index);
		prt(info, "  camera %d closed",index);
		ret=net_open_camera(index, (char *) cam_info[index].ip,
				cam_info[index].port, (char *) cam_info[index].name,
				(char *) cam_info[index].passwd, (void *) process_fun,
				&cam_info[index]);
		prt(info, "  camera %d opened",index);
		if(ret<0){
			prt(debug_long,"login %s (port %d) fail",cam_info[index].ip,cam_info[index].port);
		}
		reset_alg(info->index);
		prt(info, "  camera %d reset done",index);
	//	pthread_mutex_unlock(&info->cmd_lock);
#endif
		break;
	case CAMERA_CONTROL_SET_NTP:
		prt(info, "set cam")
		;
		break;
	case CAMERA_CONTROL_RESET_ALG:
	//	pthread_mutex_lock(&info->cmd_lock);
		reset_alg(info->index);
	//	pthread_mutex_unlock(&info->cmd_lock);
		prt(info, "reset alg");
		break;
	default:
		break;
	}
	if (get_cmd(info->index) != CAMERA_CONTROL_NULL)
		set_cmd(CAMERA_CONTROL_NULL, info->index);
	pthread_mutex_unlock(&info->cmd_lock);
}
/* Function Description
 * name:
 * return:
 * args:
 * comment:初始化相机资源，开始等待命令的到来。这里的等待方式是每秒循环检查一千次命令是否到来，对cpu效率的影响有待进一步实验。
 * 这种循环检测命令到来时为了保证程序的非阻塞性。
 * todo:
 */
void camera_service_init()
{
	int i;
	//准备网络摄像机sdk
	if (open_sdk()) {
		prt(info, "err in open sdk");
	} else {
		prt(info, "ok to open net camera  sdk");
	}
#ifdef PLAY_BACK
	init_gl();	//初始化opengl
#endif
	//每个相机结构体初始化
#ifdef USE_FILE
	for(i=0;i<1;i++){
#else
	for (i = 0; i < ACTIVE_CAM_NUM; i++) {
#endif
		init_alg(i);
#ifdef PLAY_BACK
		cam_info[i].gl_common.window_id=-1;
#endif
		cam_info[i].open_flg = 0;
		cam_info[i].index = i;
		cam_info[i].cam_running_state = CAM_STOPED;
			set_cmd(CAMERA_CONTROL_NULL, i);	//初始化命令状态，相机准备接受各种命令，相机执行完之后，再设置为默认值
		pthread_mutex_init(&cam_info[i].cmd_lock, NULL);
		pthread_mutex_init(&cam_info[i].run_lock, NULL);
			//prt(info,"init addr %p,i %d",&cam_info[i].cmd_lock,i);
		m_timed_func_data *p_data = regist_timed_func(CHECK_DURATION,
				(void *) camera_main, (void *) &cam_info[i]);//每1/1000秒检查下是否有命令到来
		start_timed_func(p_data);

		cam_info[i].watchdog_value=DOG_HUNGRY_STATE;
		m_timed_func_data *p_data_watchdog = regist_timed_func(WATCHDOG_CHECK_DURATION,
				(void *) watchdog_func, (void *) &cam_info[i]);//每10秒检查下循环是否还在正常运行
		start_timed_func(p_data_watchdog);
	}
}
