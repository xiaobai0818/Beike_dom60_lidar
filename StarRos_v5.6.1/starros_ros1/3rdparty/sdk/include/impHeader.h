#ifndef _IMP_HEADER_H_
#define _IMP_HEADER_H_

#include "ICD_Common.h"
#include "ICD_LiDAR_API.h"
#include "ICD_Scada_API.h"
#include "incScadaCfg.h"

/**********************************/
const int IMP_HEADER_VER = 0x06 ;
/**********************************/

#define INTENSITY_TABLE_SIZE 2048
#define ROI_NUMBER (2)
#define  BUFFER_SIZE 9
#define  STDEV 0.01
//#define CALIBPARA_All_SIZE 14688      //60*40+2048*6

const int IMP_SYNCWORD = 0xA7;
const int IMP_SYNCWORD_COUNT = 8 ;

//zks 2022.01.03 const int ISF_HEADER_SIZE = 0x40000 ;	//ISF文件头 256KB
const int IMP_FILE_SIZE = 0x8000000 ;	//IMP-3.x 快数据文件 128 MB

const double OpticalSpeed = 0.14985277;	///<测距数据解算系数

#define T0_STEP (0x80000000) // 2^31, 2147483648
#define T0_Clock_Prcs (40.0e-6)		// 40PPM
#define T0_Clock_Check  (1000)   //bydengxw  //(150)  //by zhangwei 将pps的间隔判断值增加到5000150
//#define T0_Clock_Check (100)

#define UTC_LOSEUPDATA_VALUE (30)  //30s
#define SHOT_RINGBUF_NUM (2)
#define SHOT_RINGBUF_SIZE (0x8000) //32K
#define SHOT_RINGBUF_FULL (0x10)
#define Inclt_RINGBUF_SIZE  (0x10000)
#define Inclt_RINGBUF_FULL (0x10)

#pragma pack(1)

//Mirror No: AP CFans
enum MirrorNum_E {
	oneMirror = 0,
	twoMirror = 1,
	threeMirror = 2,
	fourMirror = 3,
	zeroAngSignal = 4,
	updataFrame = 5,
};

//#define PROJ_STR_SIZE 32
typedef union {
struct {
	char			syncword[IMP_SYNCWORD_COUNT]; //0xA7A7A7A7
	//Device and Data format
	char			licenseID[32] ;   //for security reason
	DEVICE_TYPE_E	deviceType;
	unsigned int	deviceRev ;		//!< 
	unsigned int	fpgaVer;		// FPGA 
	char 			scadaVer[32];  		// SCADA Software Version Num
	unsigned int 	impVers;		// Imp Header
	unsigned int 	formatVer ;		// imp file format 
	//Customer
	char 			deviceID[32];				//
	char 			machAddr[32];          	// mach address
	char 			eth_addr[32];          	// IP address
	char 			customID[32];
	unsigned int	productDate ; 	// from SCDCFG_VERSION_S
	//statistic
	uint16_t 		lifeTime ;		// hours
	uint16_t 		laserTime ;		// hours
	//Project info - obsolete
//	char 			projectID[32];
//	unsigned int 	timeStamp;      //
	unsigned int 	stationNbr;		//obsolete
//	CSYSTEMTIME 	sysTime;		//should use standard format from system call - time() ;
//	unsigned int 	systemState ;	//
//	unsigned int 	m_cycle;		//
} ;
char reserved[1024];				//zks 2022.04.22: should be 1024
} SCADA_META_S;		//total 1024Byte

typedef union {
struct {
  //zks    SCDCFG_VERSION_S version;   
  SCADA_TYPE_E     		m_escada ;
  SCDCFG_LASER_S   		lsrConf ;      	//!< 64 bytes
  SCDCFG_MOTOR_S   		scnConf ;      	//!<
  SCDCFG_STAGE_S   		stgConf ;      	//!<
  SCDCFG_CAMERA_S  		cmrConf ;      	//!<
  SCDCFG_RULER_S   		rlrConf ;      	//!< 
  SCDCFG_ENVIR_S   		envConf ;     	//!<
  SCDCFG_RANGE_S   		lrsRangCfg_ ;  	//!<
  SCDCFG_CHECK_S   		bdChkConf_ ;   	//!<
  SCDCFG_LSRSEL_S		lsrSelConf;  	//!<
  SCDCFG_FREQS_S		freqsConf;     	//!<
  SCDCFG_ANGELS_S		anglesConf;   	//!
  SCDCFG_RLSR_CNL_S		rfansLsr ;
  SCDCFG_RLSR_RANGE_S	rRangeConf;
  IMU_UART_S			imuUart;
  SCAN_POSITION_S		scnPosi;
  TIMESIMU_S			timeSimu;		//!< 64 bytes
  GNSS_CONFIG_S			gnssConfig;		//!<GNSS_INTERFACE_CONFIGURATION锟斤拷gnss锟接匡拷锟斤拷锟矫诧拷锟斤拷
} ;
char reserved[2048] ;           //!< total 2KB
} SCADA_CONFIG_S;

typedef union {
  struct {
  SCDCMD_TIMSYN_S		envCtrl ; 	// SCDCMD_TIMSYN_S     synStat ; 
  SCDCMD_DATACTRL_S		dataCtrl ;
  SCDCMD_LASER_S		lsrCtrl ; 
  SCDCMD_SCANER_S		scnCtrl ; 
  SCDCMD_TURRET_S		stgCtrl ; 
  SCDCMD_CAMERA_S		cmrCtrl ; 
  SCDCMD_PROGRM_S		prgCtrl ; 
  SCDCMD_REGIST_S		regRptr ;
  SCDCMD_DMI_S			dmiData ; 
  SCDCMD_MTA_S			mtaCtrl;
//obsolete  USBCMR_TRIG_S       usbCmrData;
//obsolete  SCDCMD_CAMERA_S     pmCmrData ; 
//obsolete  SCDCMD_PALM_S       pmDate ;    
//obsolete  SCDCMD_PALMSTG_S    pmStgData ;  
  } ; 
  char reserve[1024] ; 	//! total 1KB
} SCADA_CONTROL_S ;

typedef union {
  struct { // for short, medium, long range default parameters 
	SCDCMD_TIMSYN_S	  scdEnvDefs ;
	SCDCMD_LASER_S	  lsrCtrl[3] ;	
	SCDCMD_SCANER_S 	scnCtrl[3] ;  
	SCDCMD_TURRET_S 	stgCtrl[3] ;  
	SCDCMD_PROGRM_S 	prgCtrl[3] ;  
	//	SCADA_CONFIGCTR_S	ctlPara[3] ; //obsolete
  } ; 
  char reservChar[1024] ; 				//! ??? 720 Bytes
} SCADA_CONTROL_DEFS , SCADA_CTRL_DFTS ;  // before : SCADA_DEFAULTCTR_S , SCADA_CONFIGCTR_S

typedef union {
  struct {
  SCDRPT_ENVIR_S      envRprt ; 
  SCDRPT_ENVIR_EXT_S	envExtRprt ;
  SCDRPT_HWREG_S	  regRprt ;
  SCDRPT_PNTCLD_S	  pntRprt ;    //Data Summary
  SCDRPT_LASER_S      lsrRprt ; 
  SCDRPT_SCANER_S     scnRprt ; 
  SCDRPT_TURRET_S     stgRprt ; 
  SCDRPT_CAMERA_S     cmrRprt ; 
//  SCDCMD_PROGRM_S     prgRprt ; 
//  SCDCMD_REGIST_S	  regRprt ;
//  SCDCMD_DMI_S        dmiRprt ; 
//obsolete  USBCMR_TRIG_S       usbCmrData;
//obsolete  SCDCMD_CAMERA_S     pmCmrData ;     //! palm相机命令/状态
//obsolete  SCDCMD_PALM_S       pmDate ;        //! palm 数据状态
//obsolete  SCDCMD_PALMSTG_S    pmStgData ;     //! palm 转台数据
  } ; 
  char reserve[1024] ; 	//! total 1KB
} SCADA_REPORTS_S ;


typedef union _ImpHead_V6 { //ImpHeader_V6
struct  {
    SCADA_META_S DataHead;  			//(1KB)
    SCADA_CONFIG_S scada_config;  		//(2KB)  //should be modified only in lidarAPI, CScada and its children class read only
    union {		
	LiDAR_STATMACH_E machine_stat ;	
    char reservA[2048] ;				//(2KB)
    } ;
    SCADA_CTRL_DFTS scada_defCtrl ; 	//(1KB)  //Only modified in lidarAPI, CScada and its children class read only
    SCADA_CONTROL_S scada_control ;  	//(1KB)  //Only modified in lidarAPI, CScada and its children class read only
    SCADA_REPORTS_S scada_report ;		//(1KB)  //Only modified in CScada and its children class, lidarAPI or other read only
	union { //CALC_ARRAY_S calc_array ; 		//(70KB)
		struct {
			RANGE_CALIB_S	range_calib;	             //64KB			
			LIDAR_MODEL_S	lidar_para;					 //128B
			ANGLE_CALIB_S	angle_calib[MIRROR_NUM];     //256B	
			LIDAR_POS_S		pos_para;					 //512B
			GEO_PROJ_S		geo_proj;                    //256B
			PRE_FILTER_S	pre_filter;                 //512B
            INTENTSITY_CALIB_S intensity_calib;			 //512B
			TEMPER_CALIB_S  temper_calib;                 //512B        
		} ;
		float calib_para[0x4600] ;
	} ;
	char reservB[2048] ;				//(2KB)
} ; 
	char reservC[IMP_HEADER_SIZE] ;   //zks 2022.04.22: because of SCADA_META_S is 1020 for now
}  Imp_Header_S ;


#pragma pack()

/************* zks : duplicated with SCADA_CONTROL_DEFS
const int SCDSCAN_TYPE_COUNT = 3 ;
typedef struct {
  SCADA_CONFIGCTR_S ctlPara[SCDSCAN_TYPE_COUNT] ; //720Byte
  char reserve[304] ;
} SCADA_DEFAULTCTR_S ; //total 1024 Byte

typedef struct { //obsolete
  SCDCMD_SCANER_S	  scnCtrl ; 	 //! 扫描电机命令/状态
  SCDCMD_TURRET_S	  stgCtrl ; 	 //! 转台命令/状态
  SCDCMD_LASER_S	  lsrCtrl ; 	 //! 激光器命令/状态
  SCDCMD_PROGRM_S	  prgCtrl ; 	 //! 扫描配置命令/状态
} SCADA_CONFIGCTR_S ;  //total 240 Byte

typedef union _CALC_A { // 70 KB
struct {
	SCDCAL_ARRAY_S scada_calib; 		//(2KB)妯″瀷鏍￠獙鍙傛暟
	ALGSC_ARRAY_S scada_algscA; 		//(4KB)棰勫?勭悊妯″潡閰嶇疆淇℃??
	RANGE_CALIB_S scada_rangeCal ;		//(64KB)璺濈?荤伆搴︽敼姝ｈ??
	} ;

	float calibOutPara[0x4600]; 		//float : 4 X 16k Bytes in total // before : CALIBPARA_All_SIZE 14688/0x3960
} CALC_ARRAY_S ; 

zks ***********************/


/*********** zks 2022.05.01 : legacy one
struct ImpHeader_V1 {
	SCADA_META_S DataHead;  			//设备信息(1KB)  
	SCADA_CONFIG_S scada_config;  		//系统配置信息(2KB)
	SCDCMD_ENVIR_S scada_env;    		//系统状态信息(2KB)  
	char revEnv[1988];
	SCADA_CONTROL_DEFS scada_defCtrl; 	//设备默认控制参数信息(1KB)
	SCADA_CONTROL_S scada_control;  	//设备控制参数(1KB)
	char reservedA[1024]; //(1KB)
	SCDCAL_ARRAY_S scada_calib; 		//模型校验参数(2KB)    2048
	ALGSC_ARRAY_S scada_algscA; 		//预处理模块配置信息(4KB)  4096
	RANGE_CALIB_S scada_rangeCal; 		//距离灰度改正表(64KB)   65536
	char reservB[2048]; //(2KB)
} ;

struct ImpHeader_V2 { 					// v2相比较v1隐藏了检校参数
       SCADA_META_S DataHead;			//设备信息(1KB)
       SCADA_CONFIG_S scada_config;		//系统配置信息(2KB)
       SCDCMD_ENVIR_S scada_env;		//系统状态信息(2KB)
       char revEnv[1988];
       SCADA_CONTROL_DEFS scada_defCtrl;		//设备默认控制参数信息(1KB)
       SCADA_CONTROL_S scada_control;	//设备控制参数(1KB)
       char reservedA[1024];			//(1KB)
       float calibOutPara[0x4600];		//模型校验和输出参数
       char reservB[2048] ; 			//
};//total 81920 Byte

struct ImpHeader_V3 { 					// V3相比较v2增加版本标识
    SCADA_META_S DataHead;  			//设备信息(1KB)
    SCADA_CONFIG_S scada_config; 	 	//系统配置信息(2KB)
    SCDCMD_ENVIR_S scada_env;    		//系统状态信息(2KB)
    char revEnv[1988];
    SCADA_CONTROL_DEFS scada_defCtrl; 	//设备默认控制参数信息(1KB)
    SCADA_CONTROL_S scada_control;  	//设备控制参数(1KB)
    char reservedA[1024]; //(1KB)
    float calibOutPara[0x4600]; 		//模型校验和输出参数
    char reservB[2044];					//(2KB)  (2048+4096+65536+2052)-14688*4
    int dataflag;						//快慢数据标识；0xFFFF0000:慢数据；0xFFFFFFFF:快数据
} ;

typedef struct _ImpHead_V5 { 			// IMP文件格式_v3.1.0.txt
    SCADA_META_S DataHead;  			//设备信息(1KB)
    SCADA_CONFIG_S scada_config;		//系统配置信息(2KB)
    SCDCMD_ENVIR_S scada_env ;			//ZKS: duplicated with scada_control.envCtrl
    char revEnv[1988] ;					//
    SCADA_CONTROL_DEFS scada_defCtrl;	//设备默认控制参数信息(1KB)
    SCADA_CONTROL_S scada_control;		//设备控制参数(1KB)
    char reservedA[1024] ;				//(1KB)
    SCDCAL_ARRAY_S scada_calib;			//模型校验参数(2KB)
    ALGSC_ARRAY_S scada_algscA;			//预处理模块配置信息(4KB)
    RANGE_CALIB_S scada_rangeCal ;		//距离灰度改正表(64KB)
    char reservB[2044] ; 				//(2KB)
	int dataflag;						//快慢数据标识;0xFFFF0000:慢数据;0xFFFFFFFF:快数据
} Imp_Header_S5 ;
zks ************************/

#endif
