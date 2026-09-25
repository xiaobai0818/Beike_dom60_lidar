/**********************************************************
 * $Id$
 * @author Darsi Zhang <darsiz@isurestar.com>
 * All right reserved, Sure-Star Coop.
 ***********************************************************/
#ifndef _ICD_SCADA_CFG_SH_
#define _ICD_SCADA_CFG_SH_

/****************************************************
*    Data Structure for SCADA Hardware Configuration
*****************************************************/
#include "ICD_Scada_API.h"

//!< Decoder pulse num @ 360° = encoder number * freq. multiply * FPGA subdivision 
//<< Defined in lidar.cfg，applied by FPGA registers 
const int SCAN_ENCODE_MAX = (4*64) ;

// Pulse Num = encoder num  * subdivision num * FPGA multiply num * FPGA subdivision
const int STAG_ENCODE_MAX = (8192 * 10 * 4 * 4);//!< 
const int SL_ENCODE_MAX = 16383 ;


#pragma pack(push, 2)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * Laser Configuration
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
typedef enum { //laser types
    eVGEN = 0,
    eKPS,
    eML1550,
    eVGEN5000,
    eVGEN10000,
    eKPSUM82,
	eGUANGXUN,
} LASER_TYPE_E;

const LASER_TYPE_E  SCDLSR_TYPE_DFLT = eKPS;      	//!< ：KPS
const int  SCDLSR_CURR_MIN  = 0 ;                  	//!< min current (mA)：0
const int  SCDLSR_CURR_MAX  = 204 ;                	//!< max current (mA): 204
const int  SCDLSR_POW_MIN   = 10 ;                 	//!< min Laser Power ：10%
const int  SCDLSR_POW_MAX   = 100 ;                	//!< max Laser Power：100%
const int  SCDLSR_RANGE_MIN = 0 ;                  	//!< min Laser Ranging ：0m
const int  SCDLSR_RANGE_MAX = 15000 ;               //!< Max Laser Ranging: 1500m

typedef struct {			   	//<0x2001>
	LASER_TYPE_E lsrType;   //<0x2001>
	int freqMin;           	//<0x2001>--unit KHz
	int freqMax;           	//<0x2001>--unit KHz
	int elecMin;           	//<0x2001>--unit mw
	int elecMax;           	//<0x2001>--unit mw
	int powMin;            	//<0x2001>--min Power user defined
	int powMax;            	//<0x2001>--
	int rangeMin;          	//<0x2001>--
	int rangeMax;          	//<0x2001>--
	int workPowMin;        	//<0x2001>--min Working Power
	int workPowMax;        	//<0x2001>--max Working Power
	int reserved[5];
} SCDCFG_LASER_S;					//total 64byte

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * motor defaults: it should be 3rd party visible !
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
const int SCDSCN_TYPE_DFLT  = 0;               //!< obsolete
const int SCDSCN_CODE_DFLT  = 4000;            //!< Encoder default number ：4000
const int SCDSCN_ZERO_DFLT  = 0;               //!< Zero offset ：0
const int SCDSTG_TYPE_DFLT  = 0;
const int SCDSCN_SPDCHECK_DFLT   = 1;

//???
const int SCD_VLBMIN_DFLT = 0;               //!< 视场最小角度默认值： 0
const int SCD_VLBMAX_DFLT = 360;             //!< 视场最大角度默认值： 360

const int SCD_STAGE_FREDB = 4 ;               //Freq. Multiply for stage motor

typedef enum {
  eBCDStage = 0,
  eACJStage = 1,
  eRegisterStage,
}SCDSTAGE_TYPE_E;

const int FOUR_MULTIPLIER = 4;
//! Scanner Configuration
typedef struct {			//<0x2002>
	int motType;				//<0x2002> 
	float spdMin;				//<0x2002>  
	float spdMax;				//<0x2002>  
	int coder;					//<0x2002> encoder
	float zeroAng;			//<0x2002> zero offset angle or angle resolution for the stage
	// FOV start and stop angle 
	unsigned int angstr[4], angstp[4] ;
	unsigned short scnSpdCheck;	//<0x2002>--the threshold for the speed stablity checking
	unsigned short stgType;			//<0x2002>--stage motor Controlling type  0: bcd ; 1: acj
	unsigned int periodOrPichHigh ;		//  or PITCH_all
	char reserved[4];
} SCDCFG_MOTOR_S, SCDCFG_STAGE_S;    //total 64byte

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * Camera Configuration - obsolete
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
typedef enum {
  eExpRise = 0, //default
  eExpFall = 1,
} EXP_TYPE_E;

typedef enum {
  eFlash = 0, //default
  eShutter,
} CMR_MODE_E;

const int SCDCMR_PLSWTH_DFLT = 1000000;      //!< The width of the camera trigger pulse：10000000*10ns = 100毫秒
const int SCDCMR_COUNT_DFLT = 2;             //!< Camera number
const float SCDDMI_CIRCUM_DFLT   = 2.0f ;    //!< the wheel perimeter
const float SCDDMI_ENCODER = 512 ;           //!< the encoder number
const float SCDCMR_SHUTOFFSET_DFLT = 0 ;
//! 相机高级配置
typedef struct {
    int cmrType;                //<相机配置 0x2009 >!< 相机曝光类型
    int cmrMode;                //<相机配置 0x2009 >!< 相机记录模式
    int plsWidth;               //<相机配置 0x2009 >!< 相机电平脉宽
    int cmrCount;               //<相机配置 0x2009 >!< 相机个数
    float circum;               //<相机配置 0x2009 >!< 车轮周长
    int dmiEncoder ;            //<相机配置 0x2009 >!< DMI 编码器线数
    unsigned int shutOffset[CMR_NUMBER];   //<相机配置 0x2009>CMR_NUMBER个相机曝光延时寄存器
    int cmrFlashSign ;          //<相机配置 0x2009 >1：USB相机外触发 0：USB相机内触发
	int cmrTriFactor;			//<相机配置 0x2009 >!<相机定时触发系数
    int reserved[2];            //<相机配置 0x2009 >!< 保留  //total 64
} SCDCFG_CAMERA_S ;

const int SCDlSR_NEAR_POW_MIN = 10 ;    //!< 近距离最小功率默认值
const int SCDlSR_NEAR_POW_MAX = 50 ;    //!< 近距离最大功率默认值
const int SCDlSR_FAR_POW_MIN  = 50 ;    //!< 远距离最小功率默认值
const int SCDlSR_FAR_POW_MAX  = 100 ;   //!< 远距离最大功率默认值
const int SCDRANGE_NEAR_MIN   = 0 ;     //!< 近距离最小测距默认值
const int SCDRANGE_NEAR_MAX   = 50 ;    //!< 近距离最大测距默认值
const int SCDRANGE_FAR_MIN   = 30 ;     //!< 远距离最小测距默认值
const int SCDRANGE_FAR_MAX  = 1500 ;   //!< 远距离最大测距默认值


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * Sensor Parameters * * * * * * * * * * * * * * 
typedef enum {
  eAMCW = 0xAB,
  eTOF  = 0xCB
} RANGE_MODE_E;

const int             SCD_TMMSRPRCSNA_DFLT = 7872446;	//!< MPIA, defaults
const int             SCD_TMMSRPRCSNB_DFLT = 7872446;	//!< MPIB锛宒efaults
const TZERO_MODE_E    SCD_TMMSREDGE_DFLT  = eRise; 

typedef enum {
  eFall = 1 ,         //!< Falling Edge Active
  eRise = 0 ,         //!< Raising Edge Active
} TZERO_MODE_E;

typedef enum { // zks...... obsolete
  eOnlyA = 0,         //!< 浠匨PIA_A
  eOnlyB = 2,         //!< 浠匨PIA_B
  eT0Same = 4,        //!< MPIA_A&B锛屼笖浣跨敤鍚屼竴T0
  eT0Diff = 8,         //!< MPIA_A&B锛岄棿闅斾竴涓猅0
  eMFmode = 9
} MPIA_MODE_E ;
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Defaults ....
const unsigned short	SCDCFG_PEAKDLY_DFLT		= 300 ;	//!< defaults for Peak Sync.
const unsigned short	SCDCFG_PULSEWIDTH_DFLT	= 0 ;	//!< defaults for pulse width correction , unit : mm
const unsigned short	SCDCFG_MSET_DFLT  		= 16 ;	//mset for GPX

typedef union {
	struct {
	//GPX
	unsigned short		mpiaMode ;			//!< //obsolete
	unsigned short		mSet;				//!< Timing <0x2007> , before : int
	int					tdcaFact ;			//!< TDC for GPX-A, before : int
	int					tdcbFact ;			//!< TDC for GPX-B, before : int
	//Ranging 	---- 
	unsigned short		rangeMode ;			//< obsolete : eAMCW = 0xAB, eTOF  = 0xCB
	unsigned short		tzeroMode ;			//<0x2006> 鏃堕棿绐楀彛瀵瑰簲T0淇″彿娌? 0-rise 1-fall        ; before : TZERO_MODE_E
	unsigned short 		selecPeak ;			//<0x2006> threshold for Amplifier Selection(the saturation for higher Amplifiyer)
	unsigned short  	timeDelay ;			//<0x2006> Timing Window Delay
	unsigned short   	pulseCtrl ;			//<0x2006> pulse width control 
	unsigned short  	pulseDelay;			//<0x2006> pulse width delay
	unsigned short 		selecPeakL;			
	unsigned short 		tdc_time_delay ;	//before : Delay_tdc_time
	//Pulse Width
	//double			pulseWdth ;			//!< units ??? why doouble ?
	float				pulseWdthA_;		//<0x2007> unit: mm, Timing, min pulse-width in saturatipm
	float				pulseWdthB_;		//<0x2007> unit: mm, GPXB pulse-width correction value
	//Peak ..
	int					peakSync;			//<2006> Peak Delay in higher amplifying times
	int					peakSyncL;			//<2006> Peak Delay in lower amplifying times
	//Simulation ...
	int					simuRatio;			//<0x2008> Simulation Ratio
	//others ...
	unsigned short 		scdVersion;			//obsolete : <0x2008> --SCD versions
	unsigned int 		packEnable;			//<0x2008> Data Package Enable
	unsigned short 		dynamic_lsr_freq_enable ;	//<0x2007> before : laser_freq_enable
	unsigned short  	bin_res_coeff;		//before : Bin_Resolution_coeff
	float				compass_offset ;	//<0x2006> compass offset, before : af; unit: 1/60 degress 
	} ;
	char reserved[64] ;
} SCDCFG_RULER_S;		// SCDCFG_RULER_EXT_S ;	

typedef union {
	struct {
	int nearPowMin_;    //<0x200A> near range with min laser power
	int nearPowMax_;    //<0x200A> near range with max laser power
	int farPowMin_;	    //
	int farPowMax_;     //
	int nearRangeMin_;  //<0x200A> the min range allowed for near range mode
	int nearRangeMax_;  //<0x200A> the max range allowed for near range mode
	int farRangeMin_;   //<0x200A> the min range allowed for far range mode
	int farRangeMax_;   //<0x200A> the max range allowed for far range mode
	} ;
	char reserved[64] ;
} SCDCFG_RANGE_S ;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * Temperature Sensor Configurations
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
const int SCDCFG_TEMPTYPE_DFLT = 0;        //!< default: none
const int SCDCFG_TEMPMIN_DFLT  = 0;        //!< low temperature , 0 degrees by default
const int SCDCFG_TEMpMAX_DFLT  = 60;       //!< low temperature , 60 degrees by default

typedef enum {
  eIMU_LCI100,
  eIMU_uIMUIC,
  eIMU_HG4930CA51,   
  eIMU_RAWDATA,     //raw data for any imu types, before : SRC_DATA
} SCDCFG_IMUTYPE_E;

typedef union {
	struct {
	int tempType;           //<0x2005> sensor type
	int tempMin;            //<0x2005> the min temperature to start the heater
	int tempMax;            //<0x2005> the threshold to high temperature warning
	GNSS_TYPE_E gpsType;
	unsigned short  imuTypeSelect; //<0x2008> 0:uIMU-IC; 1:LCI-100; 2:HG4930CA51
	} ;
	char reserved[64];       //total 64 Byte
} SCDCFG_ENVIR_S ;

//int nRangScopeMin,	nRangScopeMax;		  //!< scan distances （m 米）
//int nLaserFreqMin,	  nLaserFreqMax;	  //!< e.g (50 - 300))Frequence (KHz)
//int nLaserPowerMin,   nLaserPowerMax;	  //!< e.g Min(51%) (功率单位 %）
//int fScannerSpeedMin, fScannerSpeedMax;   //<! rpm
//float fStageSpeedMin, fStageSpeedMax;	  //!< 转台速度边界

typedef union {
struct{
	unsigned short areaStartAngMin, areaStartAngMax;	//<0x200C> degree; angular area
	unsigned short areaStopAngMin, areaStopAngMax;		//<0x200C> degree; 
	unsigned short areaItvAngMin, areaItvAngMax;		//<0x200C> degree; angular area, camera triging intervals
	unsigned short angTrgStartMin, angTrgStartMax;		//<0x200D> degree
	unsigned short angTrgStopMin, angTrgStopMax;		//<0x200D> degree
	unsigned short angTrgItvMin, angTrgItvMax;			//<0x200D> degree
	unsigned short gridHmin, gridHmax;					//<0x200E> ?cm, griding resolution in horizontal 
	unsigned short gridVmin, gridVmax;					//<0x200E> ?cm, griding in vertical
	unsigned short gridrefRangeMin, gridrefRangeMax;	//<0x200E> meter, griding reference range (min - max) 
	unsigned short radianHmin, radianHmax;				//<0x200F> radian, in Horizontal
	unsigned short radianVmin, radianVmax;				//<0x200F> 
	unsigned short roiHstartAngMin, roiHstartAngMax;	//<0x2010> ROI
	unsigned short roiHstopAngMin, roiHstopAngMax;		//<0x2010>
	unsigned short roiVstartAngMin, roiVstartAngMax;	//<0x2010>
	unsigned short roiVstopAngMin, roiVstopAngMax;		//<0x2010>
	unsigned short cmrTimeItvMin, cmrTimeItvMax;		//<0x200D>
	unsigned short cmrRangeItvMin, cmrRangeItvMax;		//<0x200D>
} ;
	char reserved[256] ;
}SCDCFG_CHECK_S;	//total 256 Byte


const unsigned short LSR_SEL_NUM=16;
typedef struct{
  unsigned short laserSel[LSR_SEL_NUM];
} SCDCFG_LSRSEL_S;

const unsigned short FREQ_NUM = 7;
typedef struct {
  unsigned short freq[FREQ_NUM];
  unsigned short nflight;//n倍光程
}SCDCFG_FREQS_S ; //total 16 Byte

const unsigned short ANGEL_NUM = 16;
typedef struct{
  unsigned short angle[ANGEL_NUM];
}SCDCFG_ANGELS_S;	//total 32Byte

typedef struct {
  unsigned int lsrRevSel_0_7 ;
  unsigned int lsrRevSel_8_15 ;
  unsigned int lsrRevTiem_0_3 ;
  unsigned int lsrRevTiem_4_7 ;
  unsigned int lsrRevTiem_8_11 ;
  unsigned int lsrRevTiem_12_15 ;

	int reserved[10];
} SCDCFG_RLSR_CNL_S; //total 64 Byte

typedef struct {
  float range[16];
} SCDCFG_RLSR_RANGE_S ; //total 64 Byte

typedef struct _ScanPosition {
	int scan_position_encode1;  //对地位置编号1的编码器值
	int scan_position_encode2;  //对地位置编号2的编码器值
	int scan_position_encode3;  //对地位置编号3的编码器值
	int scan_position_encode4;  //对地位置编号4的编码器值
	int scan_frame_rate1;		//对地位置编号1的帧频抽稀配置
	int scan_frame_rate2;		//对地位置编号2的帧频抽稀配置
	int scan_frame_rate3;		//对地位置编号3的帧频抽稀配置
	int scan_frame_rate4;		//对地位置编号4的帧频抽稀配置
	int reflection_ang_pos1;	//位置编号1的角度，取决于设备安装
	int reflection_ang_pos2;	//位置编号2的角度，取决于设备安装
	int reflection_ang_pos3;	//位置编号3的角度，取决于设备安装
	int reflection_ang_pos4;	//位置编号4的角度，取决于设备安装
} SCAN_POSITION_S;


//! 系统配置参数
const int INTENSE_MAX = 2048 ;
const int INTENSE_MAX1 = 6144 ;
const int RANGE_MODE_NUM = 4;
const int MIRROR_NUM = 4;
//Can only read from config file, not from the cmd line or GUI

typedef union {
  struct {//zks:为什么用字符串？
	char srCalibs[256] ;	//0x3001
	char calibs[256] ;		//AP:0x3002 ,RA:0x3003 ;UA:0x3004 ;AK:0x3008
	char urInclt[256] ;		//0x3005
	char RIfilt[256] ;		//0x3006
  } ;
  char reserved[2048] ;		//total 2KB
} SCDCAL_ARRAY_S ;



typedef union{
	struct {
		double origin_offset;
		double mirror_angle;
		double ray_offset_X;
		double ray_offset_Z;
		double zero_pos_angle;
		double angle_rotate_axis;
		double view_angle_min;
		double view_angle_max;
		double mirror_num;
	};
	double reserved[16] ;  //128B
}LIDAR_MODEL_S;          

typedef union {
	struct {
		double ray_horiz_angle;
		double ray_verti_angle;
		double range_coeff;
		double scan_angle_coeff[3];
		double turn_angle_coeff[2];
		double mirror_angle_coeff;
		double angle_axisY;
		double angle_axisZ;
		double angle_rotate_axis[3];
	};
	double reserved[32] ;   //256B
}ANGLE_CALIB_S;


typedef union {
	struct {
		float multip_coeff_gpxA[RANGE_MODE_NUM];
		float add_coeff_gpxA[RANGE_MODE_NUM];
		float multip_coeff_gpxB[RANGE_MODE_NUM];
		float add_coeff_gpxB[RANGE_MODE_NUM];
		float range_table_gpxA[RANGE_MODE_NUM][INTENSE_MAX];	//0xA B(50k)  0xC D(300k) 0xE 9(500k)
		float range_table_gpxB[RANGE_MODE_NUM][INTENSE_MAX];	//0xB(50k)	
	};
	char reserved[65632] ; 		
} RANGE_CALIB_S ;

typedef union INTENTSITY_CALIB {
	struct {
		bool calib_enable;
		int angleIntVal[3];
		double intCoff[5];
		bool stretch_enable;
		int transInt[8];
	};
	char reserved[512];
}INTENTSITY_CALIB_S;

typedef union TEMPER_CALIB {
	struct {
	  float temper_table[100];
	};
	char reserved[512];
}TEMPER_CALIB_S;


typedef struct {
  unsigned char comNum   ;         //!< 端口号
  unsigned int  baudRate ;         //!< 波特率
  unsigned char dataBits ;         //!< 数据位
  unsigned char stopBits ;         //!< 停止位
  unsigned char parity   ;         //!< 校验位
  unsigned char timeOut  ;         //!< 超时判断数据停止传输
  char reserved[24]      ;          //!< total 32Byte
} COM_SETUP ;

typedef struct {
  COM_SETUP gpsSetup ; //GPS接受
  COM_SETUP imuSetup ; //IMU接受
  COM_SETUP cmrSetup ; //相机接受
  COM_SETUP cmdSetup ; //命令接受发送
  COM_SETUP stgSetup ; //转台
//obsolete  COM_SETUP airSpdSetup ; //空速管
  COM_SETUP scanSetup ;
  COM_SETUP bluetoothSetup ;
  COM_SETUP eckSetup;
  COM_SETUP compassAndIncSetup;
  char reserved[288]  ;           //!< total 512Byte
}LIDAR_COM_GROUP , LIDAR_COM_SETUP ;

//鎶曞奖鍙傛暟
typedef enum {
	EastLongitude = 0,
	WestLongitude = 1,
	NorthLatitude = 2,
	SouthLatitude = 3,
} LongLatType_E, GEO_INTERVAL_E/*obsolete*/ ;

typedef enum {
	WGS84 = 0,
	BeiJing54 = 1,
	XiAn80 = 2,
	GJ2000 = 3,
} EllipsoidType_E, ELLIP_TYPE_E /*obsolete*/;

typedef enum {
	UTM = 0,
	GUSS = 1,
	TM = 2,
} ProjectionType_E, PROJECT_TYPE_E/*obsolete*/ ;


typedef enum {
	PROJ_ZONE_6 = 0,
	PROJ_ZONE_3 = 1,
	PROJ_CONFIG = 2,
} PROJ_ZONE_E ;


typedef union {
	struct  {
		//0x4000
		bool	calc_traj_enable;
		int		traj_calcMode;
		int		imu_clock[3];
		int		frd_directZ;
		int	imu_frd_angle[3];
		double	lidar_frd_angle[3];

		//0x4001
		bool	interp_traj_enable;  
		bool	calc_btd_enable;
		double	utc_gps;
		double	delt_angle[3];
		double	delt_xyz[3];

		//0x4002
		int     anlign_type;
		int		lidar_freq;
		double	freq_delt_angle[3];
		double	freq_delt_xyz[3];
	};
	char reserved[512];
}LIDAR_POS_S;


typedef union {
	struct  {
		//0x4010
		double longtitude;
		double latitude;
		double altitude;
		double central_longtitude;
		double base_station_height;
		double gps_delt_xyz[3];

		//0x4011
		bool			proj_enable;
		bool			switch_longitude;              
		PROJECT_TYPE_E	proj_type; 
		ELLIP_TYPE_E	ellip_type;  
		GEO_INTERVAL_E	lati_interval,longi_interval;   
		PROJ_ZONE_E		proj_zone;
	};
	char reserved[256];
}GEO_PROJ_S;



typedef enum {
	NONE = 0,
	IN_FILTER = 1,
	OUT_FILTER = 2,
}ANGLE_FILTER_E;



typedef union {
	struct {
		//0x5000
		bool	range_enable;	
		double	range_min, range_max;   //meters
		//0x5001
		bool	range_intensity_enable;
		double	ri_range_min, ri_range_max;   //meters
		double	ri_intens_min, ri_intens_max;
		//0x5002
		ANGLE_FILTER_E	angle_enable;       
		double			angle_min, angle_max;     //degree
	};
	char reserved[512];
}PRE_FILTER_S;


#pragma pack(pop)

#endif
