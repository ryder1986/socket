///////////////////////////////////////////////////////
//		DSPARMProto.h
//
//		dsp与arm通讯协议
//  	BY DAVID 
//      20130512
//		VER: 1.01.00.00
///////////////////////////////////////////////////////

#ifndef __DSP_PROTO_H__
#define __DSP_PROTO_H__

#define MAX_DETECTOR_TYPES		2			//最大支持两种检测器
#define MAX_DETECTOR_ONETYPE	8			//每种检测器最大支持8个
////20131222
#define MAX_LANE		8	//	8			//每个检测器最大支持8个车道

//#define FULL_COLS  					(720)
#define FULL_COLS  					(640)
#define FULL_ROWS  					(480)
#define  CALIBRATION_POINT_NUM   8  //标定点数2015102
//////////////////////////////////////////////////
//		结构定义: 	检测器配置信息结构
/////////////////////////////////////////////////

#ifndef __ALG_POINT__
#define __ALG_POINT__

typedef  unsigned int 	Uint32;
typedef  int 			Int32;
typedef  unsigned short Uint16;
typedef  short 			Int16;
typedef  int             BOOL;
typedef  unsigned char	Uint8;

typedef struct 
{
	Uint16 x;
	Uint16 y;
}CPoint;
#endif

typedef struct tagSPEEDLANE
{
	Uint16				uLaneID; //车道号
	//detect region
	CPoint				ptFourCorner[6];//四个点的坐标
	CPoint				ptCornerQ[2];//排队前置线
	CPoint				ptCornerQA[2];//警戒线
	CPoint				ptCornerLB[2];//20150918
	CPoint				ptCornerRB[2];//20150918
	Uint16				uDetectDerection;//方向
	Uint16				ptFrontLine;//钱直线
	Uint16				ptBackLine;//后直线
	Uint16				uReserved0[30];//保留

	//vehicle length and speed transformation params
	Uint16				uTransFactor;//转换系数
	
	//extended params by david 20130904
	Uint32 			uSpeedCounterChangedThreshold;
	Uint32 			uSpeedCounterChangedThreshold1;
	Uint32 			uSpeedCounterChangedThreshold2;
	Uint32				uGraySubThreshold;//灰度差分值域
	
	
	Uint16				uTrackParams[20];		//预留
	Uint16				uReserved1[20];			//预留
}SPEEDLANE;

/*车辆存在检测器定义,start*/
typedef struct tagZENITH_SPEEDDETECTOR {
	
	Uint16				uLaneTotalNum;//车道总数
	SPEEDLANE 				SpeedEachLane[MAX_LANE];
	Uint16				uEnvironmentStatus;			//环境状态, 1－白天  2－白天阴天(傍晚) 3－晚上路灯 4－晚上无路灯 0－其他  //20130930 by david
	//alg params
	Uint16				uDayNightJudgeMinContiuFrame;	//环境转换灵敏度 15
	Uint16				uComprehensiveSens;		//综合灵敏度 60
	Uint16				uDetectSens1;			//检测灵敏度1	 20	
	Uint16				uDetectSens2;			//检测灵敏度2    10
	Uint16				uStatisticsSens1;		//统计灵敏度1   15
	Uint16				uStatisticsSens2;		//统计灵敏度2   3
	Uint16				uSobelThreshold;		//阀值灵敏度    3
	Uint16             uEnvironment;           //20140320,白天 晚上不抽点与抽点的转换
	CPoint					ptactual[8];//标定点
	CPoint					ptimage[8];//标定点	
        //CPoint calibration_point[4];//标定区域点
	//CPoint base_line[2];//标定基准线点
	float base_length;//基准线长
	float near_point_length;//最近点距离
	Uint16				uReserved1[10];			//预留   
}ZENITH_SPEEDDETECTOR;
/*车辆存在检测器定义,end*/


/*车队长度检测器,start*/
typedef struct tagPRESENCELANE {
	Uint16 uReserved[256];
}PRESENCELANE;

typedef struct tagZENITH_PRESENCEDETECTOR {
	
	Uint16			uLaneTotalNum;
	PRESENCELANE		PresenseEachLane[MAX_LANE];
}ZENITH_PRESENCEDETECTOR;
/*车队长度检测器,end*/

//////////////////////////////////////////////////
//		结构定义: 	消息传输 arm ---> dsp
/////////////////////////////////////////////////

typedef struct tagSpeedCfgSeg {
	Uint16				uType;		//检测器类型:1－车辆存在检测器,2－车队长度检测器,3－256 保留,0－无检测器
	Uint16				uNum; 		//检测器个数
	ZENITH_SPEEDDETECTOR	uSegData[1];//同类型所有检测器配置数据区
} SPEEDCFGSEG;

typedef struct tagPresenceCfgSeg {
	Uint16				uType;		//检测器类型:1－车辆存在检测器,2－车队长度检测器,3－256 保留,0－无检测器
	Uint16				uNum; 		//检测器个数
	ZENITH_PRESENCEDETECTOR	uSegData[1];//同类型所有检测器配置数据区
} PRESENCECFGSEG;

//union NormalCfgSeg {
//	SPEEDCFGSEG  		cSpeedCfgSeg;
//	PRESENCECFGSEG		cPresenceCfgSeg;
//};

typedef struct tagMsgHeader {
    Uint16   	uFlag;
    Uint16  	uCmd;
    Uint16  	uMsgLength;		
} MSGHEADER;

typedef struct tagConfigInfoHeader {
    Uint16		uDetectPosition;
    Uint16		uDetectFuncs[2];	//
} CFGINFOHEADER;

typedef struct tagCfgMsg {
	MSGHEADER		uMsgHeader;
	CFGINFOHEADER	uCfgHeader;			
} CFGMSG;
//////////////////////////////////////////////////
//		结构定义: 	消息传输  dsp ---> arm
//////////////////////////////////////////////////
typedef struct tagSpeedDetectInfo {
	BOOL		bInfoValid;				//检测器结果有效
	Uint16	bVehicleSta;			//车入车出状态
	CPoint		ptVehicleCoordinate;	//车辆位置
	Uint16	uVehicleSpeed;			//车速
	Uint16	uVehicleLength;			//车长
	Uint16	uVehicleHeight;			//车高
	Uint16	uVehicleHeadtime;		//车头时距
	Uint16 uVehicleQueueLength;    //排队长度
	unsigned int calarflag;
	unsigned int car_in;
	unsigned int car_out;
	CPoint	LineUp[2];
	int AlarmLineflag;
	bool     IsCarInTailFlag;    //尾部区域占有标志
	bool     getQueback_flag;	//txl,20160104
	Uint16 uDetectRegionVehiSum; //区域车辆数
	Uint16 uVehicleQueueLength1; //排队长度
	CPoint QueLine[2]; //排队长度线
	Uint16	uReserved[20];			//预留
}SpeedDetectInfo_t;

typedef struct tagPresenceDetectInfo {
	BOOL		bInfoValid;				//检测器结果有效
	Uint16	uMotorCadeLength;		//车队长度
	CPoint		ptLisnceCoordinate[4];	//车牌坐标
	Uint16	uLisnceID[10];			//车牌号码
	Uint16	uLisnceColor;			//车牌颜色
	Uint16	uVehicleColor;			//车身颜色
	Uint16	uVehicleBrand;			//车标类型(车辆品牌)
	Uint16	uSignalLightSta;		//信号灯状态			
	Uint16	uEnvirenmentSta;		//环境状态			
	Uint16	uReserved[100];			//预留
}PresenceDetectInfo_t;

typedef struct tagResultData {
	Uint16			uLaneID;				//车道ID
	Uint16			uReserved0;				//
	SpeedDetectInfo_t		SpeedDetectInfo1;
	PresenceDetectInfo_t PresenceDetectInfo;
	Uint16			uReserved[95];			//预留 
}RESULTDATA;

typedef struct tagResultInfo {
	Uint16 		LaneSum;						//
	Uint16			uEnvironmentStatus;		//环境状态, 1－白天  2－白天阴天(傍晚) 3－晚上路灯 4－晚上无路灯 0－其他  //20130930 by david
    RESULTDATA			uEachLaneData[8];				//包含8个车道所有的检测信息
} RESULTINFO;

typedef struct tagResultMsg {
    MSGHEADER		uMsgHeader;
    RESULTINFO 		uResultInfo;
} RESULTMSG;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#endif
