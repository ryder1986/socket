
#ifdef __cplusplus
extern "C" {
#endif
//#include "DSPARMProto.h"
#ifdef __cplusplus
}
#endif
#include <unistd.h>
#include "cam_alg.h"
//#include "m_arith.h"
#include "common.h"
#include "g_define.h"
//#include "param.h"
#include "client_net.h"
#include "sig_service.h"

mAlgParam algparam[ACTIVE_CAM_NUM];
int time_section_changed(mCamDetectParam *p_camdetectparam, mAlgParam *algparam)
{// qiang
    int newstatus = 0;
    unsigned int value;
    struct tm *pltime;
    int ret = 0;

    time_t now;
    time(&now);
    unsigned int *pTime = p_camdetectparam->timep;
    pltime=localtime(&now);
    value = pltime->tm_hour * 60 + pltime->tm_min;
    prt(debug_time_shift, "look at time %d , ling chen(%d->%d),baitian(%d->%d), huanghun(%d->%d),", value,
        pTime[0], pTime[1],
            pTime[1], pTime[2],
            pTime[2], pTime[3]);
    if(value >= pTime[0] && value < pTime[1])
        newstatus = 3;//ling cheng
    else if(value >= pTime[1] && value < pTime[2])
        newstatus = 4;// bai tian
    else if(value >= pTime[2] && value < pTime[3])
        newstatus = 1;//huang hun
    else
        newstatus = 2;//wan shang
    //0,1,2,3,4
    if(newstatus != algparam->time_section){
        prt(info, "changing time section   from %d  to %d",   algparam->time_section,newstatus);
        algparam->time_section=newstatus ;
        algparam->LaneIn.uEnvironmentStatus= algparam->time_section;
        return 1;
    }
    return ret;
}

int lane_param_init(int curstatus, mDetectParam *tmpDetect,mCamDemarcateParam * tmpCamdem , mAlgParam *algparam)
{
    print_alg("alg ctrl switch");

    algparam->LaneIn.uTransFactor=tmpDetect[curstatus].uTransFactor;
    algparam->LaneIn.uGraySubThreshold=tmpDetect[curstatus].uGraySubThreshold;
    algparam->LaneIn.uSpeedCounterChangedThreshold=tmpDetect[curstatus].uSpeedCounterChangedThreshold;
    algparam->LaneIn.uSpeedCounterChangedThreshold1=tmpDetect[curstatus].uSpeedCounterChangedThreshold1;
    algparam->LaneIn.uSpeedCounterChangedThreshold2=tmpDetect[curstatus].uSpeedCounterChangedThreshold2;
    algparam->LaneIn.uDayNightJudgeMinContiuFrame=tmpDetect[curstatus].uDayNightJudgeMinContiuFrame;
    algparam->LaneIn.uComprehensiveSens=tmpDetect[curstatus].uComprehensiveSens;
    algparam->LaneIn.uDetectSens1=tmpDetect[curstatus].uDetectSens1;
    algparam->LaneIn.uDetectSens2=tmpDetect[curstatus].uDetectSens2;
    algparam->LaneIn.uStatisticsSens1=tmpDetect[curstatus].uStatisticsSens1;
    algparam->LaneIn.uStatisticsSens2=tmpDetect[curstatus].uStatisticsSens2;
    algparam->LaneIn.uSobelThreshold=tmpDetect[curstatus].uSobelThreshold;

    //	memcpy(&algparam->LaneIn,\
    //			&tmpDetect[curstatus],\
    //			&algparam->LaneIn.uEnvironmentStatus-  \
    //			&algparam->LaneIn.uTransFactor);

    algparam->LaneIn.uEnvironment= algparam->alg_index;
    algparam->LaneIn.uEnvironmentStatus= algparam->time_section;
    algparam->LaneIn.base_length = (float)tmpCamdem->baselinelen;
    algparam->LaneIn.near_point_length = (float)tmpCamdem->recent2stop;
    return 0;
}
int lane_pos_init(mChannelCoil *tmpcoil, mLine *tmpline,CPoint *m_ptEnd,int lanenum)
{
    int i=0;
    for(i=0; i<lanenum; i++)
    {
        m_ptEnd[0+i*12].x = tmpline[i*2].startx;
        m_ptEnd[0+i*12].y = tmpline[i*2].starty;
        m_ptEnd[2+i*12].x = tmpline[i*2+1].startx;
        m_ptEnd[2+i*12].y = tmpline[i*2+1].starty;
        m_ptEnd[4+i*12].x = tmpcoil[i].FrontCoil[1].x;
        m_ptEnd[4+i*12].y = tmpcoil[i].FrontCoil[1].y;

        m_ptEnd[5+i*12].x = tmpcoil[i].FrontCoil[2].x;
        m_ptEnd[5+i*12].y = tmpcoil[i].FrontCoil[2].y;

        m_ptEnd[6+i*12].x = tmpcoil[i].FrontCoil[0].x;
        m_ptEnd[6+i*12].y = tmpcoil[i].FrontCoil[0].y;
        m_ptEnd[7+i*12].x = tmpcoil[i].FrontCoil[3].x;
        m_ptEnd[7+i*12].y = tmpcoil[i].FrontCoil[3].y;
        m_ptEnd[8+i*12].x = tmpcoil[i].RearCoil[1].x;
        m_ptEnd[8+i*12].y = tmpcoil[i].RearCoil[1].y;
        m_ptEnd[9+i*12].x = tmpcoil[i].RearCoil[2].x;
        m_ptEnd[9+i*12].y = tmpcoil[i].RearCoil[2].y;
        m_ptEnd[10+i*12].x = tmpcoil[i].RearCoil[0].x;
        m_ptEnd[10+i*12].y = tmpcoil[i].RearCoil[0].y;
        m_ptEnd[11+i*12].x = tmpcoil[i].RearCoil[3].x;
        m_ptEnd[11+i*12].y = tmpcoil[i].RearCoil[3].y;

        m_ptEnd[1+i*12].x = tmpline[i*2].endx;
        m_ptEnd[1+i*12].y = tmpline[i*2].endy;
        m_ptEnd[3+i*12].x = tmpline[i*2+1].endx;
        m_ptEnd[3+i*12].y = tmpline[i*2+1].endy;
    }
    return 0;
}
int get_alg_index(mAlgParam *algparam)
{
    if (DAYTIME == algparam->time_section
            || DUSK == algparam->time_section)
        return ALG_DAYTIME;
    else
        return ALG_NIGHT;
}
#define USE_INI_CONFIG 1
#define TICK_CHECK_POINT 250
#define TICK_ORI_POINT 0

void init_alg_dsp(mAlgParam *algparam,mCamDetectParam *p_camdetectparam,mCamParam *p_cammer)
{
    int i;
    if(algparam->alg_index==ALG_NULL)
        algparam->alg_index=ALG_DAYTIME;
    if(TIME_SECTION_NULL== algparam->time_section)
        algparam->time_section=DUSK;
    algparam->tick=TICK_ORI_POINT;
    memset(algparam->alg_arg.m_ptEnd, 0, sizeof(algparam->alg_arg.m_ptEnd));
    memset(algparam->alg_arg.ptimage, 0, sizeof(algparam->alg_arg.ptimage));
    memset(&algparam->LaneIn,0,sizeof(algparam->LaneIn));

    lane_param_init(algparam->alg_index-1,p_camdetectparam->detectparam,&p_cammer->camdem,algparam);
    lane_pos_init(p_camdetectparam->detectlane.virtuallane,
                  p_camdetectparam->laneline.laneline, algparam->alg_arg.m_ptEnd,p_camdetectparam->detectlane.lanenum);
    mDemDetectArea *tmpArea = &p_camdetectparam->area;
    for(i=0; i<6; i++){
        algparam->alg_arg.ptimage[i].x = tmpArea->vircoordinate[i].x;
        algparam->alg_arg.ptimage[i].y = tmpArea->vircoordinate[i].y;
    }
    algparam->algNum = p_camdetectparam->detectlane.lanenum;
    transform_init_DSP_VC(USE_INI_CONFIG, algparam->algNum,algparam->LaneIn,&algparam->outbuf,&algparam->alg_arg);
    usleep(100000);
}
frame_info info;
void process_nanjing(int index ,OUTBUF *p_outbuf)
{
    int j;

    for(j=0;j<NANJING_LANE_MAX;j++){
        info.cams[index].lanes[j].no=33;
        info.cams[index].lanes[j].ms=33;

        info.cams[index].lanes[j].exist_flag= p_outbuf->calarflag[j];//1 means exist,0 means empty
        if(info.cams[index].lanes[j].last_exist_flag==1&&info.cams[index].lanes[j].exist_flag==0){
            info.cams[index].lanes[j].in_car=0;
            info.cams[index].lanes[j].out_car=1;
        }
        if(info.cams[index].lanes[j].last_exist_flag==0&&info.cams[index].lanes[j].exist_flag==1){
            info.cams[index].lanes[j].in_car=1;
            info.cams[index].lanes[j].out_car=0;
        }

        info.cams[index].lanes[j].last_exist_flag=info.cams[index].lanes[j].exist_flag;
        info.cams[index].lanes[j].queue_len=33;
        info.cams[index].lanes[j].speed=33;
        info.cams[index].lanes[j].start_pos=33;
        info.cams[index].lanes[j].veh_no=33;
        info.cams[index].lanes[j].veh_type=33;
    }

    //lock
    if(!d60[index].data_valid){
        for(int i=0;i<4;i++)
        {
           d60[index].lane_data[i].incar_time

        }
    }
    //unlock


}
int AlgProcessFrameData(mAlgParam *algparam,unsigned char *inbuf,unsigned char *inubuf,unsigned char *invbuf)
{
    int i;
    int ret = 0;
    algparam->framecount++;
    transform_Proc_DSP_VC(inbuf,
                          inubuf,invbuf,FULL_COLS,FULL_ROWS,0,&algparam->outbuf,&algparam->alg_arg);
    return ret;
}

static int alloc_alg(mAlgParam *algparam,mCamDetectParam *p_camdetectparam,mCamParam *p_cammer)
{
    init_alg_dsp(algparam,p_camdetectparam,p_cammer);
    return 0;
}

void pack_sig_data(int index)
{

    m_sig_data *p_channel_rst=get_locked_sig_data(index);
    mAlgParam * p_algparam= &algparam[index];
    m_sig_data *FVDChannel = p_channel_rst;
    //	p_channel_rst->lane_num=p_algparam->algNum;
    int i;
    //	pthread_mutex_lock(&p->channel_rst_lock);
    for(i=0; i<p_algparam->algNum; i++){
        //printf("p_algparam->alg_arg.p_outbuf->calarflag[%d]:[%d]\n", i, p_algparam->alg_arg.p_outbuf->calarflag[i]);
        if(p_algparam->alg_arg.p_outbuf->calarflag[i] == 0x2){
            if(0 == p_channel_rst->car_info.g_50frame1[i])
                p_channel_rst->car_info.g_50frame1[i] = p_algparam->framecount;
            else{
                p_channel_rst->car_info.g_50frame2[i] = p_channel_rst->car_info.g_50frame1[i];
                p_channel_rst->car_info.g_50frame1[i] = p_algparam->framecount;
            }
        }else if(p_algparam->alg_arg.p_outbuf->calarflag[i] == 0x1)
            p_channel_rst->car_info.g_occupancyframe[i]++;

        if(p_algparam->alg_arg.p_outbuf->IsCarInTail[i]){
            if(0 == p_channel_rst->car_info.g_50frame1[i])
                p_channel_rst->car_info.g_50frametail1[i] = p_algparam->framecount;
            else{
                p_channel_rst->car_info.g_50frametail2[i] = p_channel_rst->car_info.g_50frametail1[i];
                p_channel_rst->car_info.g_50frametail1[i] = p_algparam->framecount;
            }
            p_channel_rst->car_info.g_occupancyframetail[i]++;
        }
    }
    char bBackgroundRefreshed = 0x1;
    mCamParam *p=get_mCamParam(index);
    for (i = 0; i < p_algparam->algNum; i++) {
        if (p_algparam->framecount % 12 == 0) {



            FVDChannel->Eachchannel[i].mDetectChannelIndex =p->channelcoil[i].number;
            //printf("sub->cammer.channelcoil[%d].number: [%d]\n",i, FVDChannel->Eachchannel[i].mDetectChannelIndex);

            if (FVDChannel->Eachchannel[i].mDetectChannelIndex < 41
                    && FVDChannel->Eachchannel[i].mDetectChannelIndex > 96)
                FVDChannel->Eachchannel[i].mDetectChannelIndex = 255;  //闁岸鏁撻弬銈嗗闁跨喐鏋婚幏锟�
            FVDChannel->Eachchannel[i].mQueueLength =
                    p_algparam->alg_arg.p_outbuf->uVehicleQueueLength[i];  //闁跨喕鍓奸梼鐔侯劜閹凤拷
            FVDChannel->Eachchannel[i].mRealTimeSingleSpeed =
                    p_algparam->alg_arg.p_outbuf->uVehicleSpeed[i]; //闁跨喎濮拋瑙勫
            if (0x1 == p_algparam->alg_arg.p_outbuf->calarflag[i])
                FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bOccupyStatus1 =
                        0x1; // 0.5s闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏椋庡Ц閹拷
            else
                FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bOccupyStatus1 =
                        0x0; // 0.5s闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏椋庡Ц閹拷

            FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bFix0 =
                    0x0; //闁跨喐鏆�鐠佽瀚归崐锟�

            if (!p_channel_rst->car_info.g_flow[i])
                p_channel_rst->car_info.g_flow[i] = p_algparam->alg_arg.p_outbuf->DetectOutSum[i];
            else {
                FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.flow =
                        p_algparam->alg_arg.p_outbuf->DetectOutSum[i]
                        - p_channel_rst->car_info.g_flow[i]; //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�
                p_channel_rst->car_info.g_flow[i] = p_algparam->alg_arg.p_outbuf->DetectOutSum[i];

                if (FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.flow
                        > 3)
                    FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.flow =
                            0x3;
            }
            if (p_algparam->alg_arg.p_outbuf->calarflag[i]) {
                if (p_algparam->alg_arg.p_outbuf->uVehicleLength[i] > 7)
                    FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bVehicleType =
                            0x3;
                else if (p_algparam->alg_arg.p_outbuf->uVehicleLength[i] > 5)
                    FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bVehicleType =
                            0x2;
                else
                    FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bVehicleType =
                            0x1;
            } else
                FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bVehicleType =
                        0x0; //闁跨喐鏋婚幏鐑芥晸闁扮數灏ㄩ幏锟�

            if (p_channel_rst->car_info.g_50frame1[i] && p_channel_rst->car_info.g_50frame2[i]) {
                FVDChannel->Eachchannel[i].mHeadwayTimeOfHeaderVirtualCoil =
                        (p_channel_rst->car_info.g_50frame1[i] - p_channel_rst->car_info.g_50frame2[i])
                        * (0.4);
                if (FVDChannel->Eachchannel[i].mHeadwayTimeOfHeaderVirtualCoil
                        > 254)
                    FVDChannel->Eachchannel[i].mHeadwayTimeOfHeaderVirtualCoil =
                            255;
            } else
                FVDChannel->Eachchannel[i].mHeadwayTimeOfHeaderVirtualCoil =
                        0x0; //婢舵挳鏁撻弬銈嗗閺冨爼鏁撻弬銈嗗

            if (p_channel_rst->car_info.g_occupancyframe[i]) {
                FVDChannel->Eachchannel[i].mOccupancyOfHeaderVirtualCoil =
                        ((float) (((float) p_channel_rst->car_info.g_occupancyframe[i]) / 12))
                        * 100;
                p_channel_rst->car_info.g_occupancyframe[i] = 0;
            } else
                FVDChannel->Eachchannel[i].mOccupancyOfHeaderVirtualCoil = 0x0; //婢舵挳鏁撻弬銈嗗閸楃娀鏁撻弬銈嗗闁跨喐鏋婚幏锟�

            /*****鐏忛箖鏁撻弬銈嗗**/
            FVDChannel->Eachchannel[i].mDetectDataOfTailVirtualCoil.bOccupyStatus1 =
                    p_algparam->alg_arg.p_outbuf->IsCarInTail[i]; // 0.5s闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏椋庡Ц閹拷
            FVDChannel->Eachchannel[i].mDetectDataOfTailVirtualCoil.bFix0 = 0x0; //闁跨喐鏆�鐠佽瀚归崐锟�

            //printf("g_50frametail1[%d]:[%d] g_50frametail2[%d]:[%d]\n", i, g_50frametail1[i], i, g_50frametail2[i]);
            if (p_channel_rst->car_info.g_50frametail1[i]
                    && p_channel_rst->car_info.g_50frametail2[i]) {
                FVDChannel->Eachchannel[i].mHeadwayTimeOfTailVirtualCoil =
                        (p_channel_rst->car_info.g_50frametail1[i]
                         - p_channel_rst->car_info.g_50frametail2[i]) * (0.4);
                if (FVDChannel->Eachchannel[i].mHeadwayTimeOfTailVirtualCoil
                        > 254)
                    FVDChannel->Eachchannel[i].mHeadwayTimeOfTailVirtualCoil =
                            255;
            } else
                FVDChannel->Eachchannel[i].mHeadwayTimeOfTailVirtualCoil = 0x0; //婢舵挳鏁撻弬銈嗗閺冨爼鏁撻弬銈嗗 鐏忛箖鏁撻弬銈嗗闁跨喓娼鹃崙銈嗗缂佺喖鏁撻弬銈嗗

            //printf("g_occupancyframetail[%d]:[%d]\n", i, g_occupancyframetail[i]);
            if (p_channel_rst->car_info.g_occupancyframetail[i]) {
                FVDChannel->Eachchannel[i].mOccupancyOfTailVirtualCoil =
                        ((float) (((float) p_channel_rst->car_info.g_occupancyframetail[i])
                                  / 12)) * 100;
                //printf("g_occupancyframetail[%d]:[%f]\n", i, (float)(((float)g_occupancyframetail[i])/12));
                p_channel_rst->car_info.g_occupancyframetail[i] = 0;
            } else
                FVDChannel->Eachchannel[i].mOccupancyOfTailVirtualCoil = 0x0; //婢舵挳鏁撻弬銈嗗閸楃娀鏁撻弬銈嗗闁跨喐鏋婚幏锟�

            if (p_algparam->alg_arg.p_outbuf->visibility) {
                print_alg("==error p_algparam visibility: 0x1");
                FVDChannel->Eachchannel[i].mWorkStatusOfDetectChannle.bDataIsValid =
                        0x1; //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔峰建閿燂拷
            } else {
                FVDChannel->Eachchannel[i].mWorkStatusOfDetectChannle.bDataIsValid =
                        0x0; //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔峰建閿燂拷
            }
            /*缂佺喖鏁撻弬銈嗗闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰ú妤呮晸閿燂拷 End*/

            bBackgroundRefreshed &= p_algparam->alg_arg.p_outbuf->getQueback_flag[i];
            if (FVDChannel->EachStatus.mCameralStatus.bBackgroundRefreshed
                    != bBackgroundRefreshed) {
                p_channel_rst->camera_state_change = 1;
            }
            FVDChannel->EachStatus.mCameralStatus.bBackgroundRefreshed =
                    bBackgroundRefreshed;
            //Log0("getQueback_flag[%d]:[%d] bBackgroundRefreshed:[%d]\n",i,p_algparam->alg_arg.p_outbuf->getQueback_flag[i], bBackgroundRefreshed);

            int ori=FVDChannel->EachStatus.mCameralStatus.bWorkMode ;
            if (p_algparam->alg_index == 0x1) //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�
            {
                //	printf("000000000000000");
                //	prt(info,"index %d,day time",index);
                FVDChannel->EachStatus.mCameralStatus.bWorkMode = 0x0;
            }
            else if (p_algparam->alg_index == 0x2)
            {
                //	printf("22222222222222222");
                //		prt(info,"index %d,night time",index);
                FVDChannel->EachStatus.mCameralStatus.bWorkMode = 0x2;
            }

            //0x1闁跨喓瀚涙导娆愬 闁跨喐鏋婚幏鐑芥晸鐟欐帒搴滈幏鐑芥晸閺傘倖瀚归柨鐔告灮閹风兘鏁撻敓锟�   0x3闁跨喎锟界喐娅� 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹风兘鏁撴潏鍐嚋閹风兘鏁撻弬銈嗗
            if ((p_algparam->LaneIn.uEnvironmentStatus == 0x1
                 && p_algparam->alg_index == 0x1)
                    || (p_algparam->LaneIn.uEnvironmentStatus == 0x3
                        && p_algparam->alg_index == 0x2)) {
                FVDChannel->EachStatus.mCameralStatus.bWorkMode = 0x1;
                //	printf("111111111111111111111");

                //prt(info,"index %d,shift time==> %d",index,p_algparam->LaneIn.uEnvironmentStatus);
                //		FVDChannel->Eachchannel[i].mWorkStatusOfDetectChannle.bDataIsValid=0x1; //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔峰建閿燂拷
            }
            if(ori!=FVDChannel->EachStatus.mCameralStatus.bWorkMode ){
                p_channel_rst->camera_state_change = 1;
            }
            FVDChannel->EachStatus.mCameralStatus.bH264DecodeStatus = 0x1;
            FVDChannel->EachStatus.mCameralStatus.bCameralOnLine = 0x1;
            if (FVDChannel->EachStatus.mCameralStatus.bPictureStable ==(p_algparam->alg_arg.p_outbuf->fuzzyflag?1:0)  ) {
                p_channel_rst->camera_state_change = 1;
            }
            if (p_algparam->alg_arg.p_outbuf->fuzzyflag) {
                FVDChannel->EachStatus.mCameralStatus.bPictureStable = 0x0; //闁跨喐鏋婚幏鐑芥晸妤楅缚顔愰幏椋庡Ц閹拷
                FVDChannel->Eachchannel[i].mWorkStatusOfDetectChannle.bDataIsValid =
                        0x1; //闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔峰建閿燂拷
            } else {
                FVDChannel->EachStatus.mCameralStatus.bPictureStable = 0x1; //闁跨喖銈虹拋瑙勫閻樿埖锟斤拷
            }
            /*閻樿埖锟戒線鏁撻弬銈嗗闁跨喐鏋婚幏锟� End*/



            FVDChannel->lane_num= p_algparam->algNum;
            FVDChannel->status=0x1;
            FVDChannel->EachStatus.mCameralStatus.bBackgroundRefreshed = bBackgroundRefreshed;
        } else {
            if (0x1 == p_algparam->alg_arg.p_outbuf->calarflag[i])
                FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bOccupyStatus0 =
                        0x1;  // 0.25s闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏椋庡Ц閹拷
            else
                FVDChannel->Eachchannel[i].mDetectDataOfHeaderVirtualCoil.bOccupyStatus0 =
                        0x0;  // 0.25s闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏椋庡Ц閹拷
            FVDChannel->Eachchannel[i].mDetectDataOfTailVirtualCoil.bOccupyStatus0 =
                    p_algparam->alg_arg.p_outbuf->IsCarInTail[i];  // 0.25s闁跨喐鏋婚幏宄板窗闁跨喐鏋婚幏椋庡Ц閹拷
        }
    }
    submit_unlock_sig_data(index);
    process_nanjing(index,p_algparam->alg_arg.p_outbuf);
    //	pthread_mutex_unlock(&p->channel_rst_lock);
}

void pack_realtime_data(mRealStaticInfo *p_real_time_data,	mAlgParam *p_algparam)
{
    int i;
    mAlgParam *algparam = p_algparam;
    p_real_time_data->flag = 0xFF;
    p_real_time_data->laneNum = algparam->algNum;
    p_real_time_data->curstatus = algparam->alg_index;
    p_real_time_data->fuzzyflag = algparam->alg_arg.p_outbuf->fuzzyflag;
    p_real_time_data->visibility = algparam->alg_arg.p_outbuf->visibility;

    memcpy((char *) p_real_time_data->uDegreePoint,
           (char *) algparam->alg_arg.p_outbuf->uDegreePoint,
           sizeof(algparam->alg_arg.p_outbuf->uDegreePoint));
    for (i = 0; i < algparam->algNum; i++) {
        p_real_time_data->lane[i].state = algparam->alg_arg.p_outbuf->calarflag[i];
        p_real_time_data->lane[i].vehnum = algparam->alg_arg.p_outbuf->DetectOutSum[i];
        p_real_time_data->lane[i].isCarInTail =
                algparam->alg_arg.p_outbuf->IsCarInTail[i];
        p_real_time_data->lane[i].queueLength =
                algparam->alg_arg.p_outbuf->uVehicleQueueLength[i];
        p_real_time_data->lane[i].LineUp[0].x =
                algparam->alg_arg.p_outbuf->LineUp[i][1].x;
        p_real_time_data->lane[i].LineUp[0].y =
                algparam->alg_arg.p_outbuf->LineUp[i][1].y;
        p_real_time_data->lane[i].uActualDetectLength =
                algparam->alg_arg.p_outbuf->uActualDetectLength[i];
        p_real_time_data->lane[i].uActualTailLength =
                algparam->alg_arg.p_outbuf->uActualTailLength[i];


    }
    for (i = 0; i < algparam->algNum; i++) {
//        p_real_time_data->queue_len;
//        p_real_time_data->area_car_num;
//        p_real_time_data->queue_line;

    }
}

int extern open_alg(int index)
{
    alloc_alg(&algparam[index],get_mCamDetectParam(index),get_mCamParam(index));
    memset(client_get_info(0),0,ACTIVE_CAM_NUM*sizeof(mRealStaticInfo));
    return 0;
}
int extern run_alg(int index,unsigned char *y,unsigned char *u,unsigned char *v)
{
    mAlgParam *p_algparam= &algparam[index];

    if (p_algparam->tick++ == TICK_CHECK_POINT) {
        if (time_section_changed(get_mCamDetectParam(index), p_algparam) > 0) {

            transform_arg_ctrl_DSP_VC(&p_algparam->alg_arg);
        }
        p_algparam->tick = TICK_ORI_POINT;
    }
    AlgProcessFrameData(p_algparam,y,u,v);

    pack_realtime_data(client_get_info(index),&algparam[index]);
    pack_sig_data(index);

    // guo du duan
    if((p_algparam->time_section==DUSK)||(p_algparam->time_section==MORNING)){
        if(p_algparam->alg_arg.p_outbuf->thresholdValue !=p_algparam->alg_index){
            p_algparam->alg_index=p_algparam->alg_arg.p_outbuf->thresholdValue;
            prt(info,"(cam %d)alg tell me to change alg index to %d",index,p_algparam->alg_index);
            return p_algparam->alg_index;
        }
    }

    // bai tian hei ye qiangzhi qie
    if(p_algparam->time_section==NIGHT&&p_algparam->alg_index==ALG_DAYTIME){
        p_algparam->alg_index=ALG_NIGHT;
        prt(info,"(cam %d)force change alg index to %d",index,p_algparam->alg_index);
        return p_algparam->alg_index;
    }

    // bai tian hei ye qiangzhi qie
    if(p_algparam->time_section==DAYTIME&&p_algparam->alg_index==ALG_NIGHT){
        p_algparam->alg_index=ALG_DAYTIME;
        prt(info,"(cam %d)force change alg index to",index,p_algparam->alg_index);
        return p_algparam->alg_index;
    }


    //    if(p_algparam->alg_arg.p_outbuf->thresholdValue !=p_algparam->alg_index||
    //    		(p_algparam->time_section==NIGHT&&p_algparam->alg_arg.p_outbuf->thresholdValue==ALG_DAYTIME)||
    //			(p_algparam->time_section==DAYTIME&&p_algparam->alg_arg.p_outbuf->thresholdValue==ALG_NIGHT)){
    //    	prt(info,"cam %d shift alg to %s",index,!(p_algparam->alg_arg.p_outbuf->thresholdValue-1)?"day":"night");
    //    	p_algparam->alg_index = p_algparam->alg_arg.p_outbuf->thresholdValue;
    //    	return p_algparam->alg_index;
    //	}
    return 0;
}

int extern reset_alg(int index)
{
    release_alg(index);
    open_alg(index);
    return 0;
}

void extern release_alg(int index)
{
    transform_release_DSP_VC(&algparam[index].alg_arg);
}
void extern init_alg(int index)
{
    algparam[index].time_section=TIME_SECTION_NULL;
    algparam[index].alg_index=ALG_NULL;
}
