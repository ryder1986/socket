//#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h> /// div_t 碌脛脡霉脙梅
#include <string.h>
#include "m_arith.h"
#include "DSPARMProto.h"
#include <math.h>
#include<iostream>
#include<fstream>
//
//SPEEDCFGSEG    pDetectCfgSeg;
//CFGINFOHEADER  pCfgHeader;
//RESULTMSG outbuf;




using namespace std;


CIVD_SDK_API bool transform_init_DSP_VC(bool iniflag, Uint16 lanecount,
		LANEINISTRUCT LaneIn,RESULTMSG *p_outbuf,m_args *p_arg)
{  
	SPEEDCFGSEG    *p_pDetectCfgSeg=&p_arg->pDetectCfgSeg;
	CFGINFOHEADER  *p_pCfgHeader=&p_arg->pCfgHeader;

	CPoint *ptimage=p_arg->ptimage;
	CPoint *m_ptend=p_arg->m_ptEnd;

	CPoint	ptactual[8];
	// RESULTMSG *p_outbuf=( RESULTMSG *)p_arg->p_outbuf;
	alg_mem_malloc(p_arg);
	ALGPARAMS *pParams=p_arg->pParams;
	ALGCFGS *pCfgs=p_arg->pCfgs;
	Uint16 ChNum=0;
	bool   flag_ini=false;
	p_pCfgHeader->uDetectPosition=2;//txl,20150709,2->1
	p_pCfgHeader->uDetectFuncs[0]=0x00000001;
	p_pCfgHeader->uDetectFuncs[1]=0;
	/*************************************************************************************************************/
	p_pDetectCfgSeg->uNum=1;
	p_pDetectCfgSeg->uType=1;
	p_pDetectCfgSeg->uSegData[0].uLaneTotalNum=lanecount;//锟斤拷锟矫筹拷锟斤拷锟斤拷锟斤拷
	for(int i=0;i<8;i++)
	{
		p_pDetectCfgSeg->uSegData[0].ptactual[i].x=ptactual[i].x;
		p_pDetectCfgSeg->uSegData[0].ptactual[i].y=ptactual[i].y;
		p_pDetectCfgSeg->uSegData[0].ptimage[i].x=ptimage[i].x;
		p_pDetectCfgSeg->uSegData[0].ptimage[i].y=ptimage[i].y;

	}	
	//memset((void *)&pCfgs,0,sizeof(pCfgs));
	for(unsigned int i=0;i<p_pDetectCfgSeg->uSegData[0].uLaneTotalNum;i++)//每锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	{
		if(iniflag)
		{
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[0].x = m_ptend[8+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[0].y = m_ptend[8+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[1].x = m_ptend[9+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[1].y = m_ptend[9+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[2].x = m_ptend[11+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[2].y = m_ptend[11+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[3].x = m_ptend[10+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[3].y = m_ptend[10+i*12].y;//锟斤拷锟斤拷锟斤拷锟斤拷

			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQ[0].x = m_ptend[4+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQ[0].y = m_ptend[4+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQ[1].x = m_ptend[5+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQ[1].y = m_ptend[5+i*12].y;

			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQA[0].x = m_ptend[6+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQA[0].y = m_ptend[6+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQA[1].x = m_ptend[7+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerQA[1].y = m_ptend[7+i*12].y;//锟斤拷锟斤拷锟斤拷值
 			
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerLB[0].x = m_ptend[0+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerLB[0].y = m_ptend[0+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerLB[1].x = m_ptend[1+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerLB[1].y = m_ptend[1+i*12].y;//20150918

			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerRB[0].x = m_ptend[2+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerRB[0].y = m_ptend[2+i*12].y;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerRB[1].x = m_ptend[3+i*12].x;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptCornerRB[1].y = m_ptend[3+i*12].y;//20150918
		}
			
		else{
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[0].x =410;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[0].y =200;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[1].x = 580;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[1].y = 200;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[2].x = 580;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[2].y = 390;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[3].x = 410;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[3].y = 390;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[4].x = 410;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[4].y = 300;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[5].x = 580;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[5].y = 300;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[6].x = 410;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[6].y = 300;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[7].x = 580;
			p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[7].y = 300;
			for (int j=0;j<10;j++)
			{
				p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[6+j].x = 0;
				p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFourCorner[6+j].y = 0;

			}
		}
			//printf("ptCornerQ[0] is:%d,%d\n",m_ptend[0].x,m_ptend[0].y);
			//printf("ptCornerQ[1]is:%d,%d\n",m_ptend[1].x,m_ptend[1].y);
			//printf("ptCornerQA[0]is:%d,%d\n",m_ptend[6].x,m_ptend[6].y);
			//printf("ptCornerQA[1]is:%d,%d\n",m_ptend[7].x,m_ptend[7].y);
			//printf("ptFourCorner[0] is:%d,%d\n",m_ptend[4].x,m_ptend[4].y);
			//printf("ptFourCorner[1]is:%d,%d\n",m_ptend[5].x,m_ptend[5].y);
			//printf("ptFourCorner[2]is:%d,%d\n",m_ptend[2].x,m_ptend[2].y);
			//printf("ptFourCorner[3]is:%d,%d\n",m_ptend[3].x,m_ptend[3].y);
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uDetectDerection=2;////1锟角筹拷尾 2锟角筹拷头
		
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptFrontLine=0;
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].ptBackLine=0;
		//p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uTransFactor=2.0;//锟斤拷圈实锟绞碉拷锟斤拷锟�?锟斤拷
		//p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uGraySubThreshold=40;//200
		//p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uSpeedCounterChangedThreshold=20;
		//p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uSpeedCounterChangedThreshold1=20;
		//p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uSpeedCounterChangedThreshold2=20;

		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uTransFactor=LaneIn.uTransFactor;//锟斤拷圈实锟绞碉拷锟斤拷锟�?锟斤拷
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uGraySubThreshold=LaneIn.uGraySubThreshold;//200
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uSpeedCounterChangedThreshold=LaneIn.uSpeedCounterChangedThreshold;
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uSpeedCounterChangedThreshold1=LaneIn.uSpeedCounterChangedThreshold1;
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uSpeedCounterChangedThreshold2=LaneIn.uSpeedCounterChangedThreshold2;
     
		p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uLaneID=0;
		memset((void *)&p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uTrackParams,0,sizeof(Uint16)*20);
		memset((void *)&p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uReserved0,0,sizeof(Uint16)*30);
		memset((void *)&p_pDetectCfgSeg->uSegData[0].SpeedEachLane[i].uReserved1,0,sizeof(Uint16)*20);
		pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.car_in=0;
		pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.calarflag=0;
		pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.car_out=0;
		pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleHeight=0;
		pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleLength=0;
		pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleSpeed=0;
		p_outbuf->uResultInfo.uEachLaneData[i].SpeedDetectInfo1.calarflag=0;
		p_outbuf->uResultInfo.uEachLaneData[i].SpeedDetectInfo1.car_in=0;
		p_outbuf->uResultInfo.uEachLaneData[i].SpeedDetectInfo1.car_out=0;
		p_outbuf->uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleHeight=0;
		p_outbuf->uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleLength=0;
		p_outbuf->uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleSpeed=0;
	}
	//p_pDetectCfgSeg->uSegData[0].uDayNightJudgeMinContiuFrame=15;
	//p_pDetectCfgSeg->uSegData[0].uComprehensiveSens=60;
	//p_pDetectCfgSeg->uSegData[0].uDetectSens1=20;
	//p_pDetectCfgSeg->uSegData[0].uDetectSens2=20;
	//p_pDetectCfgSeg->uSegData[0].uStatisticsSens1=30;//30
	//p_pDetectCfgSeg->uSegData[0].uStatisticsSens2=3;//3
	//p_pDetectCfgSeg->uSegData[0].uSobelThreshold=3;

	p_pDetectCfgSeg->uSegData[0].uDayNightJudgeMinContiuFrame=LaneIn.uDayNightJudgeMinContiuFrame;
	p_pDetectCfgSeg->uSegData[0].uComprehensiveSens=LaneIn.uComprehensiveSens;
	p_pDetectCfgSeg->uSegData[0].uDetectSens1=LaneIn.uDetectSens1;
	p_pDetectCfgSeg->uSegData[0].uDetectSens2=LaneIn.uDetectSens2;
	p_pDetectCfgSeg->uSegData[0].uStatisticsSens1=LaneIn.uStatisticsSens1;//30
	p_pDetectCfgSeg->uSegData[0].uStatisticsSens2=LaneIn.uStatisticsSens2;//3
	p_pDetectCfgSeg->uSegData[0].uSobelThreshold=LaneIn.uSobelThreshold;
	//printf("uEnvironment0 is %d/t",LaneIn.uEnvironment);
	//printf("uEnvironmentStatus0 is %d/n",LaneIn.uEnvironmentStatus);
	p_pDetectCfgSeg->uSegData[0].uEnvironment=LaneIn.uEnvironment;//锟斤拷锟斤拷锟斤拷锟�
	p_pDetectCfgSeg->uSegData[0].uEnvironmentStatus=LaneIn.uEnvironmentStatus;//锟狡昏开始
        p_pDetectCfgSeg->uSegData[0].base_length=LaneIn.base_length;
        p_pDetectCfgSeg->uSegData[0].near_point_length=LaneIn.near_point_length;
	/*************************************************************************************************************/
	//memset((void *)&pParams,0,sizeof(pParams));

	
	/*************************************************************************************************************/



	/*************************************************************************************************************/
	ChNum=0;
	flag_ini=ArithInit(ChNum, p_pCfgHeader, p_pDetectCfgSeg, pCfgs, pParams);//,m_ptend,LineUp//ARM->DSP锟斤拷锟斤拷
	//printf("190 is ok\t");
	if(flag_ini) return TRUE;
	else return FALSE;

}

CIVD_SDK_API unsigned int transform_Proc_DSP_VC(unsigned char  *pInFrameBuf,
		unsigned char *pInuBuf,unsigned char *pInvBuf,int nWidth,\
		int nHeight, int hWrite,RESULTMSG *p_outbuf,m_args *p_arg)//,CPoint LineUp[],CPoint m_ptend[]
{    
	ALGPARAMS *pParams=p_arg->pParams;
	ALGCFGS *pCfgs=p_arg->pCfgs;
//	 RESULTMSG *p_outbuf=p_arg->p_outbuf;
	OUTBUF *outBuf=p_arg->p_outbuf;
	Uint16 ChNum=0;
	Int32 outSize;
	SPEEDCFGSEG    *p_pDetectCfgSeg=&p_arg->pDetectCfgSeg;
	CFGINFOHEADER  *p_pCfgHeader=&p_arg->pCfgHeader;
	ChNum=0;
	IVIDEO1_BufDescIn sample_buf;
	sample_buf.framePitch=0;
	sample_buf.bufDesc[0].accessMask=0;
	outSize=sizeof(RESULTMSG);
	sample_buf.frameHeight=nHeight;
	sample_buf.frameWidth=nWidth;
	sample_buf.bufDesc[0].bufSize=sample_buf.frameHeight*sample_buf.frameWidth;
	sample_buf.bufDesc[0].buf=pInFrameBuf;
	sample_buf.numBufs=1;
	//txl,20150714
	IVIDEO1_BufDescIn u_buf;
	u_buf.framePitch=0;
	u_buf.bufDesc[0].accessMask=0;
	u_buf.frameHeight=nHeight/4;
	u_buf.frameWidth=nWidth/4;
	u_buf.bufDesc[0].bufSize=u_buf.frameHeight*u_buf.frameWidth;
	u_buf.bufDesc[0].buf=pInuBuf;
	u_buf.numBufs=1;
	//txl,20151016
	IVIDEO1_BufDescIn v_buf;
	v_buf.framePitch=0;
	v_buf.bufDesc[0].accessMask=0;
	v_buf.frameHeight=nHeight/4;
	v_buf.frameWidth=nWidth/4;
	v_buf.bufDesc[0].bufSize=v_buf.frameHeight*v_buf.frameWidth;
	v_buf.bufDesc[0].buf=pInvBuf;
	v_buf.numBufs=1;

	//printf("start to ArithProc");
	ArithProc(ChNum,&sample_buf,p_outbuf,outSize, pCfgs, pParams);//,LineUp,m_ptend锟斤拷锟街拷锟揭惨拷锟揭伙拷锟阶拷锟�.....
	//printf("232 is ok\t");
	//printf("ArithProc is ok");
	if((pCfgs->CameraCfg.uEnvironmentStatus!=2)&&(pCfgs->gThisFrameTime%100==1))
	{
	    if(Color_deviate(pInuBuf,pInvBuf,u_buf.frameWidth,u_buf.frameHeight))
		pCfgs->abnormal_time++;
	    else
		pCfgs->abnormal_time=0;	   
	    pCfgs->fuzzyflag=pCfgs->abnormal_time>5?true:false;//txl,20150714锟斤拷锟斤拷色锟届常锟斤拷锟�
	}
	else if(pCfgs->CameraCfg.uEnvironmentStatus==2)//night
	{
		pCfgs->fuzzyflag=false;
	}
	for(Uint8 i=0;i<pCfgs->ResultMsg.uResultInfo.LaneSum;i++)
	{  
		outBuf->DetectInSum[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.car_in;
		outBuf->DetectOutSum[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.car_out;
		outBuf->calarflag[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.calarflag;
		outBuf->uVehicleHeight[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleHeight;
		outBuf->uVehicleLength[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleLength;
		outBuf->uVehicleSpeed[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleSpeed;
		//for (int j=0;j<5;j++)
		//{
		//	outBuf->AlarmLineflag[i][j]=pCfgs->sptimes[j];//pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo.AlarmLineflag;
		//}
		outBuf->LineUp[i][0]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.LineUp[0];
		outBuf->LineUp[i][1]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.LineUp[1];
		outBuf->uVehicleQueueLength[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.uVehicleQueueLength;
		outBuf->IsCarInTail[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.IsCarInTailFlag;
		outBuf->getQueback_flag[i]=pCfgs->ResultMsg.uResultInfo.uEachLaneData[i].SpeedDetectInfo1.getQueback_flag;//txl,20160104
                
	}
	outBuf->fuzzyflag=pCfgs->fuzzyflag;
	outBuf->visibility=pCfgs->visibility;    
        for(Uint8 i=0;i<20;i++)
        {
          outBuf->uDegreePoint[i][0]=pCfgs->degreepoint[i][0];
          outBuf->uDegreePoint[i][1]=pCfgs->degreepoint[i][1];
        }
        for(Uint8 i=0;i<pCfgs->ResultMsg.uResultInfo.LaneSum;i++)
        {
           outBuf->uActualDetectLength[i]=pCfgs->uActualDetectLength[i];
           outBuf->uActualTailLength[i]=pCfgs->uActualTailLength[i];
        }
	//printf("visibility is :%d,",outBuf->visibility);
	//printf("fuzzyflag is :%d,\n",outBuf->fuzzyflag);
	outBuf->uVehicleHeight[0]=pCfgs->gdWrap;
	outBuf->DetectInSum[1]=pCfgs->gdSum;
	outBuf->thresholdValue=pCfgs->bAuto;
	//printf("pCfgs->bAuto is %d\n",outBuf->thresholdValue);
	outBuf->uEnvironmentStatus=pCfgs->ResultMsg.uResultInfo.uEnvironmentStatus;
	//printf("uEnvironmentStatus is %d\n",outBuf->uEnvironmentStatus);
	outBuf->frame++;

	return 1;
}

int transform_release_DSP_VC(m_args *args)
{
    alg_mem_free(args);

	return 0;
}
int transform_arg_ctrl_DSP_VC(m_args *args)
{

	ZENITH_SPEEDDETECTOR *pDownSpeedDetect =
			(ZENITH_SPEEDDETECTOR*) args->pDetectCfgSeg.uSegData;
	ALGCFGS *pCfgs=args->pCfgs;
	pCfgs->bAuto = pDownSpeedDetect->uEnvironment; //TRUE;
	pCfgs->CameraCfg.uEnvironmentStatus = pDownSpeedDetect->uEnvironmentStatus;
	if (pCfgs->CameraCfg.uEnvironmentStatus == 1
			|| pCfgs->CameraCfg.uEnvironmentStatus == 3)
		pCfgs->CameraCfg.uSquareThreshold =
				(pCfgs->CameraCfg.uSquareThreshold < 20) ?
						pCfgs->CameraCfg.uSquareThreshold : 20; //dawn dusk

	return 0;
}
