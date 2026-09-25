/********************************************************************
 * $I
 * @Technic Support: <sdk@isurestar.com>
 * All right reserved, Sure-Star Coop.
 ********************************************************************/
#ifndef ICD_SCADA_API_H_
#define ICD_SCADA_API_H_

#include "ICD_Common.h"
#include "ICD_LiDAR_API.h"

#pragma pack(2)

//====================== from icnLidarAPI.h ===============

const int LiDAR_SHOTRBUFF_SIZE = 0x8000000 ; //128 MB
typedef struct {
  int wrHead, rdTail ;
  char buffer[LiDAR_SHOTRBUFF_SIZE] ; 
} SHOTS_RINGBUF_S ;

const int SLOW_RINGBUF_SIZE = 0x1000000 ; // 16 MB 
typedef struct { //SlowRingBuff 
	int wrHead , rdTail;
	char buffer[SLOW_RINGBUF_SIZE] ; 
} SLOW_RINGBUF_S, SLOW_RINGBUFFER_S;

typedef struct
{
  unsigned short pkgFlag;        //转台包标识  0xEFEF
  unsigned int pkgNo;         //记录turret pkg个数的循环累加值，数值范围0~65535
  unsigned long long fpgaClkCount;     //8个字节，同步 fpga 心跳包打包时的时间。单位200ns/LSB.
  unsigned int   reserved;       // 预留字，所有字节全0，暂无意义。
  unsigned short angleData;      //转台角度，数值范围0-35999   ,转台angle=Data/100，单位：0.01度
}TURRET_PS_PACK_S;

const int TURRET_RINGBUF_SIZE = 1000 ; //
typedef struct {
	int wrHead , rdTail;
	TURRET_PS_PACK_S buffer[TURRET_RINGBUF_SIZE] ; 
} TURRET_RINGBUF_S;


//转台角度信息
struct _TURN_COUNT_S {
	uint16_t dataId;
	uint16_t t0stampL;
	uint16_t t0stampH;
	uint16_t TURN_ABS_CONT_H;
	uint16_t TURN_ABS_CONT_L;
	uint16_t TURN_RELATIVE_CONT_H;
	uint16_t TURN_RELATIVE_CONT_L : 6;
	uint16_t RESERVE : 10;
};

typedef struct {
	int wrHead, rdTail;
	_TURN_COUNT_S buffer[TURRET_RINGBUF_SIZE];
} _TURN_RINGBUF_S;

//const int LiDAR_SHOTBUF_SIZE = 0x40000 ;  //256KB

typedef enum {
  ePosReady,
  ePosWork,
  ePosIdle,
  ePosFileNew,
  ePosFileRenew,
  ePosFileCloseAll,
} LiDARPos_Action_E;

typedef enum {
  eGetImuStat,
  eGetGpsStat,
  eGetUtcStat,
  eGetCmrStat,
} LiDARPos_Tran_E ;

typedef enum {//<0x1001> old
  eDev_Cfg_APilot = 0,
  eDev_Cfg_RAngle,
  eDev_Cfg_UArm,
  eDev_Cfg_TPilot,
  eDev_Cfg_AK,
  eDev_Cfg_eRFans,
  eDev_Cfg_SL,
  eDev_Cfg_UT,
} LiDAR_DEVTYPE_CFG_E;

typedef enum {//<0x1001>
  eDev_APilot = 0,
  eDev_RAngle,		//11
  eDev_UArm,		//21
  eDev_TPilot,		//31
  eDev_AK,			//51
  eDev_UT = 8 ,		//61
  eDev_RFans ,   
  eDev_CFans , 
  eRF_016_75 = 101, //version V7-5.0
  eRF_032_75 = 102, //version V7-5.0, before eSL,
  eCF_032_60 = 111, //version V6.0
  eCF_128_60 = 121, //
  eCF_256_60 = 122, //
  eCK_128_21 = 131, //version 2.1
  eCK_256_00 = 151,  
} DEVICE_TYPE_E; //before : LiDAR_DEVTYPE_E
  
const int SCDSCAN_TYPE_COUNT = 3;
const static int FREQ_FAR_MIN = 50;
const static int FREQ_FAR_MAX = 100;
const static int FREQ_MEDIUM_FAR_MIN = 110;
const static int FREQ_MEDIUM_FAR_MAX = 200;
const static int FREQ_MEDIUM_MIN = 210;
const static int FREQ_MEDIUM_MAX = 450;
const static int FREQ_NERAR_MIN = 460;
const static int FREQ_NERAR_MAX = 1000;
typedef enum {		//!<
  eScanNone , 		//!< = 0 , defaults : no defination
  eScanNear ,		//!< 
  eScanMiddle ,		//!< 
  eScanMiddleFar , 	//!< 
  eScanFar ,    	//!< 
} SCDSCAN_TYPE_E ;

/*This enum initially indicates the outside communication path of lidarAPI. 
  if LidarServer it is eLidarNet 
  if WinNav it is eLidarGui
  if XXX it is eLidarUrt
  
  howerver later other info like data process behaviours are also included in this enum.
  e.g. dataSave, data process, data forward...
  
  keep in mind :
  whatever outside communication path or data process behaviour the values are not mutex exclusive.
  so if some stack has differnt requirement please kindly add a new enum value e.g.  eLidarNetAndUrt

  Actaully it is better to have two differen enum types.
 */

typedef enum {//上位机的控制模式
  eLidarTcp ,  // Acting as Server, accept cmd from Ethernet
  eLidarUdp ,  // Acting as Server, accept cmd from UDP
  eLidarGui ,  // controlling the LiDAR through GUI
  eLidarUrt ,  // Acting as Server, accept cmd from Uart
} LiDAR_TYPE_E , LiDAR_COMU_CHNL_E;

typedef enum {//上位机的控制模式
  eDataImpSave ,  // for Data Handle: Save locally
  eDataImpFlow ,  // for Data Handle: Transfer through Net
  eDataIsfSave ,  // for Data Handle: Save locally
  eDataIsfFlow ,  // for Data Handle: Transfer through Net  
  eDataCrsSave ,  // obsolete: Transfer the data into CRS format and upload through Net
  eDataCrsFlow ,  // obsolete: Transfer the data into CRS format and upload through Net
  eDataPetaFlow ,  // obsolete: Transfer the data and upload through udp
  eDataCompressFlow ,  // obsolete: Transfer the data and upload through udp
} LiDAR_DATA_FLOW_E ;


typedef enum {
  eScadaPci ,  		// deprecated 
  eScadaTcp ,  		// e.g. lidarAPI in WinNav
  eScadaUdp ,		// udpRfans, like Genius
  eScadaUsb ,		// e.g. lidarAPI in LidarServer
  eScadaUrt ,  		// silimar to eScadaNet but UART instead of Net
  eScadaSim ,  		// ARM simute data, eScdSimDa, eScdSimIm
  eScadaGrdUdp,  	// v+AP 
  eScadaSN,    		// starnavi
  eScadaAxi,		// petalinux axi dma
} SCADA_TYPE_E , SCADA_DEVTYPE_E;
  
//===========================================================================

typedef struct {
  unsigned int synWord ;
  unsigned short cmrNo ;
  unsigned int imgSize ;
  float currAngle ;
} USBCMR_IMGHEADER_S ;

typedef enum {
  eUsbDiskBreak,
  eUsbDiskInsert,
} LDRUSB_STATE_E;

typedef struct {
  uint16_t vol, amp ;       				//!< mVDC, mA
  //obsolete  unsigned short altitude ;     //!< in SCDRPT_ENVIR_S 
  unsigned int tdcaFact ;         			//!< FPGA GPX
  unsigned int tdcbFact ;         			//!< FPGA GPX
  uint16_t mSet ;           				//!< FPGA GPX
} SCDRPT_HWREG_S ;			//!< before : SCDRPT_HWINFO_S, SCADA_HWINFO_S 

//! motor hardValue - Machine Level
typedef struct{
  unsigned int spdTmpc ;
  unsigned int amcSpdCount ;
  unsigned int amcCrcValue ;
} SCDMACH_SCANER_S;

typedef struct {
  SCDEV_CMD_E cmdstat ;
  int dmiEncoder ;			//!< DMI
  float dmiCircum ;         //!< DMI
  int dmiSelect ;
} SCDCMD_DMI_S ;

typedef struct _ImuUart
{
	int buad_rate;  //
	int check_type; //
} IMU_UART_S;

typedef struct _TimeSimu {
	int win_bound_default ; // (win_upper_limit<<16) | (win_lower_limit <<4)
	int win_upper_limit;  //
	int win_lower_limit;  //
	int delay_t0_win;     //
} TIMESIMU_S;

typedef struct _GNSS_CONTROL_S
{
	int GNSS_data_type;		//GNSS
	bool ppstr_enable;		//ppstr
	bool bestposb_enable;	//bestposb
	bool timeb_enable;		//timeb
	bool gprmc_enable;		//gprmc
	int UART_check;			//uart
	int buad_rate_cnt;		//buad_rate_cnt = clk_freq / (Baud_Rate * 16)
} GNSS_CONFIG_S ;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * <2/5> scanner
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef enum {
  eScanPlusPosA  ,            
  eScanPlusPosR  ,            
  eScanMinusPosA ,           
  eScanMinusPosR ,           
  eScanPlusSpeed ,           
  eScanMinuSpeed ,            
} SCDSCN_MODE_E;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * <4/5> Stage / Turrent
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
const int 	SCDSTG_ANGLE_MIN   	= 0;
const int 	SCDSTG_ANGLE_MAX   	= 360;
const float SCDSTG_SPEED_MIN 	= 0.5 ;   	// degree/sec
const float SCDSTG_SPEED_MAX 	= 3.0 ;   	// degree/sec
const float SCDSTG_SPEED_DFLT 	= 3.0 ;  	// degree/sec

const float TRIGGER_IMAGE_SPEED = 1; 	//DPS

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * <4/5> Camera
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
const int	SCDCMR_TIMER_MIN = 0;
const int	SCDCMR_TIMER_MAX = 6553;
const int   SCDCMR_TIME_DFLT = 4000;     
const int   SCDCMR_ANGLE_MIN = 0;
const int   SCDCMR_ANGLE_MAX      = 360;
const int   SCDCMR_ANGLE_DFLT     = 180; 
const int   SCDCMR_ANGLOFFST_DFLT = 0;  

const int   SCDCMR_DMI_ENCODER = 0 ;    	//!< 
const float SCDCMR_DMI_CIRCUM = 0 ;  		//!< 

const int 	CMR_A_INDEX = 0;
const int 	CMR_B_INDEX = 1;

//! 
typedef struct {
  unsigned int regAddr;          //!< 
  unsigned int regValue;         //!<
  //read from SCADA, belong to SCADA_FPGA_STATE before
  unsigned char hardState ;	 	//!<
  unsigned char packState ; 	//!<
  unsigned char tranState ;  	//!<
  unsigned char simuState ;  	//!<
} SCDCMD_REGIST_S;

typedef enum {  
  eEchoZro = 0,
  eEchoOne = 1,    //!< 
  eEchoTwo = 2,    //!< 
  eEchoFor = 4,    //!< 
  eEchoEit = 8,    //!<
} ECHO_TYPE_E ;

typedef struct cmd_scada_L2 {
	LiDAR_CMDID_E cmdId;               //!<
  int isAsk ;
  union {
	SCDCMD_TIMSYN_S	 envCtrl ;			//! SCDCMD_TIMSYN_S  synData ;   
	SCDRPT_ENVIR_S   envRprt ;			//! SCADA_STATE_S
    SCDRPT_HWREG_S   devRegs ;			//! before : statData, devStat
	SCDCMD_DATACTRL_S	 dataCtrl ;
    SCDRPT_PNTCLD_S	 pntRprt ;      	//! real time data collection, before : infoData
    SCDCMD_LASER_S   lsrCtrl ;       	//!
	SCDRPT_LASER_S	 lsrRprt ;
    SCDCMD_SCANER_S  scnCtrl ;       	//!
    SCDRPT_SCANER_S  scnRprt ;
	SCDCMD_TURRET_S  stgCtrl ;
	SCDRPT_TURRET_S  stgRprt ;
    SCDCMD_CAMERA_S  cmrCtrl ;       	//! before: SCDCMD_CAMERA_S cmrData
    SCDRPT_CAMERA_S  cmrRprt ;       	//! 
    SCDCMD_PROGRM_S  prgCtrl ;       	//!
    SCDCMD_REGIST_S  regCtrl ;      	//! 
    char msgstream[64] ;
  };
  cmd_scada_L2() {
    isAsk = false ;
    cmdId = eLidarReset ; // eMsgSoftCk ;
  }
} SCADA_CMDRPT_U ;

typedef struct comu_scada_L2 {
  const char command_mark[8] = "A7C7A5C" ; //{ LIDAR_COMMAND_MARK } 
  SCADA_CMDRPT_U commander ;
  unsigned int check_sum ;
} SCADA_COMMAND_S ; //before: SCADA_CMDRPTS_S 

int pipeMsgIn(SCADA_CMDRPT_U cmdrpt, MESSG_RINGBUF_S* messgRing_) ;
int pipeMsgIn(char* msg, MESSG_RINGBUF_S* messgRing_) ;

#pragma pack()

#pragma pack(1)
/// 导航设备心跳包信息固定256字节
typedef struct  
{
  unsigned int pkgflag;                 ///< 包标志,固定0xE4E3E2E1
  unsigned int pkgnumber;               ///< 包号,每包+1
  unsigned char gps_time[6];            ///< GPS时间,导航设备暂未用到
  unsigned int maca;                    ///< MAC地址高4字节
  unsigned short macb;                  ///< MAC地址低2字节
  unsigned short dataport;              ///< 快数据端口
  unsigned short msgport;               ///< 命令数据端口
  unsigned char motorspd;               ///< 扫描电机转速
  unsigned int fpgaversion;             ///< FPGA版本
  unsigned short phaseAngle;            ///< 相位角
  unsigned char pack_format;            ///< 回波格式 0x2008寄存器
  unsigned char device_id;              ///< 设备ID
  short temperature;                    ///< 设备温度,跟随APD偏压反馈温度
  unsigned int err_chksum;              ///< 废弃,占位用
  unsigned int point_freq;              ///< 设备当前工作点频
  unsigned int dev_status;              ///< 设备状态
  unsigned int low_sn;                  ///< 废弃,占位用
  unsigned int high_sn;                 ///< 废弃,占位用
  unsigned long long FPGA_5mhz_cnt;     ///< 设备内部5M时钟计数值
  unsigned char time_server;            ///< 固定为2,历史代码不清楚为啥是2
  unsigned char point_pro;              ///< 数据等级选择 0x2010寄存器
  unsigned long long unixTmStamp;       ///< 设备内部unix时间,高4字节s,低4字节us
  unsigned char arrui8devName[16];      ///< 设备名称
  unsigned short lidarsvrversion;       ///< 设备demosvr程序版本
  unsigned short lidarcfgversion;       ///< 设备配置文件版本
  unsigned int lidarip;                 ///< lidarIP地址小端
  float scanspeed;                      ///< 自转电机速度
  float scanspeed1;                     ///< 公转电机速度
  float scanphase;                      ///< 自转电机相位
  float scanphase1;                     ///< 公转电机相位
  float fpgacore_temperature;           ///< FPGA核心温度
  bool  isrevise;                       ///< 设备内部是否保存有revise文件
  unsigned short slowport;              ///< 慢数据端口
  unsigned char padding[136];           ///< 预留,保证结构体256字节
}HEARTBEAT_S;

#pragma pack()

#endif
