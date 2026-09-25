/********************************************************************
 * $I
 * @Technic Support: <sdk@isurestar.com>
 * All right reserved, Sure-Star Coop.
 ********************************************************************/
#ifndef ICD_LIDAR_API_H_
#define ICD_LIDAR_API_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <cstdint>
#else
	#include <unistd.h>
    #include <sys/types.h>
    #include <stdint.h>
	#include <sys/time.h>
#endif
#pragma pack(2)

const char LIDAR_IPADDR_DFLT[] = "192.168.0.188";  	//!< 默认IP：192.168.0.188
const char LIDAR_IPADDR_WIFI[] = "192.168.10.1" ;   //!< 无线控制 IP：192.168.10.1
const char XFANS_IPADDR_DFLT[] = "224.0.0.1" ;  	//!< 默认IP：192.168.0.188

//TCP port
const int LIDAR_MSGPORT = 2008;  	//消息数据端口
const int LIDAR_PNTPORT = 2009;  	//雷达数据端口
const int LIDAR_IMGPORT = 2011;  	//图像数据端口
const int LIDAR_SLWPORT = 2012;  	//导航数据端口
const int LIDAR_GPSPORT = 2020;  	//GNSS传输端口
const int LIDAR_TURPORT = 2021;  	//转台数据传输端口

//UDP port
const int LIDAR_UDPPORT 			= 2013;  	///< openlidar协议默认监听端口
const int LIDAR_UDP_DATA_UDPPORT 	= 2014;		///< 导航设备快数据默认上送端口
const int LIDAR_UDP_CMD_UDPPORT		= 2015;		///< deb-mtdc专用协议固定端口,用于rfans v6k设备
const int LIDAR_GPSMSG_UDPPORT 		= 2016;  	///< GPS消息默认上送端口
const int SCADA_UDP_CPORT 			= 2017;		///< 本地UDP端口号
const int IPCFG_UDP_CPORT 			= 2018;		///< 本地监听固定端口用于IP配置
const int GROUND_DATAPORT 			= 2208;  	///< ATG 消息数据端口

const int LIDAR_UDP_SLW_UDPPORT 	  = 2025;   ///< 慢数据默认上送端口
const int LIDAR_UDP_HEART_DBG_UDPPORT = 2030;   ///< 心跳默认上送端口
const int LIDAR_UPDATEPORT            = 2035;   ///< OTA升级用固定端口
const int LIDAR_UDP_ALARM_UDPPORT     = 2040;   ///< 时间信息udp端口
const int LIDAR_UDP_ISFHEAD_UDPPORT   = 2043;   ///< isfhead结构体默认上送udp端口

const uint16_t LIDAR_BODY_MAX     = 1028;       ///< 新协议支持的最大body长度
const uint16_t LIDAR_BODY_MAX_OLD = 64;         ///< 旧协议支持的固定数据长度

const int SCDLSR_POWR_DFLT = 100;          //!< 100%
const int SCDLSR_POWR_MIN = 50;            //!< 50%
const int SCDLSR_POWR_MAX = 100;           //!< 100%
const int SCDLSR_FREQ_DFLT = 200;          //!< 200 kHz
const int SCDLSR_FREQ_MIN = 30;
const int SCDLSR_FREQ_MAX = 600;           //!< 600 kHz
const int SCDSCN_SPEED_MIN = 0;
const int SCDSCN_SPEED_MAX = 6000;
const int SCDSCN_SPEED_DFLT = 1800;
// const int COMMON_BUFFER_SIZE = 10000;

typedef enum {
    e_msgCnt = 0x01,
    e_pntCnt = 0x02,
    e_imgCnt = 0x04,
    e_slwCnt = 0x08,
    e_gpsCnt = 0x10,
    e_turCnt = 0x12,       ///< 转台数据上传选项
    e_allCnt = 0x1F,        // e_msgCnt | e_pntCnt | e_imgCnt | e_slwCnt | e_gpsCnt
    e_allTcp = 0x1F1F,
    e_udpMsg = 0x010000,    //0x01 << 16
    e_udpGps = 0x020000,
    e_udpAtg = 0x040000,
    e_udppnt = 0x080000, // udp快数据
    e_udpslw = 0x100000, // udp慢数据
    e_udphet = 0x200000, // upd心跳包
    e_udpPet = 0x400000,
    e_udpCmdV40 = 0x800000,
    e_udpAlarm  = 0x1000000,
    e_udpIsfHead  = 0x2000000,
    e_allUdp = 0x7f0000,
	e_allEth = 0x5FFFFF,
} ICNT_CNTSTATE_E;
//**********************************************************************
// CMDID字段命令码定义
//**********************************************************************
/// CMDID字段命令码定义
typedef enum LiDAR_CMDID
{
    eMsgSoftCk = 0x0,               ///< 0x0
    eMsgEnvir = 0x1,                ///< 0x1
    eMsgCamera = 0x2,               ///< 0x2
    eMsgScanner = 0x3,              ///< 0x3
    eMsgTurret = 0x4,               ///< 0x4
    eMsgLaser = 0x5,                ///< 0x5
    eMsgProgam = 0x6,               ///< 0x6
    eMsgAll = 0x7,                  ///< 0x7
    eMsgStat = 0x8,                 ///< 0x8
    eMsgRegister = 0x9,             ///< 0x9
    eMsgRuler = 0xA,                ///< 0xA
    eMsgUsbCmr = 0xB,               ///< 0xB
    eMsgUmrTrig = 0xC,              ///< 0xC
    eMsgHeader = 0xD,               ///< 0xD
    eMsgString = 0xE,               ///< 0xE
    eMsgStepMto = 0xF,              ///< 0xF
    eMsgDataInfo = 0x10,            ///< 0x10
    eMsgCameraRprt = 0x11,          ///< 0x11
    eMsgSensorRprt = 0x12,          ///< 0x12
    eMsgCompassRead = 0x13,         ///< 0x13
    eMsgFpgaStat = 0x14,            ///< 0x14
    eMsgDataSave = 0x15,            ///< 0x15
    eMsgTaskStop = 0x16,            ///< 0x16
    eMsgDMI = 0x17,                 ///< 0x17
    eMsgPCM = 0x18,                 ///< 0x18
    eMsgPDT = 0x19,                 ///< 0x19
    eMsgPST = 0x1A,                 ///< 0x1A
    eMsgEnvirAsk = 0x1B,            ///< 0x1B
    eMsgSyncTime = 0x1C,            ///< 0x1C
    eMsgGpsTrack = 0x1D,            ///< 0x1D
    eLidarReset = 0x1E,             ///< 0x1E
    eMsgStorgeDir = 0x1F,           ///< 0x1F
    eMsgAppDeviceMeta = 0x20,       ///< 0x20  设备信息,升级同样适用
    eMsgAPPCountPro = 0x21,         ///< 0x21  统计工程数量
    eMsgAPPGetPro = 0x22,           ///< 0x22  获取工程信息
    eMsgAPPGetProg = 0x23,          ///< 0x23  获取工程作业信息
    eMsgAPPGetCommPro = 0x24,       ///< 0x24  获取工程备注信息
    eMsgAPPUpdatePro = 0x25,        ///< 0x25  更新工程信息
    eMsgAPPDeletePro = 0x26,        ///< 0x26  删除工程
    eMsgAPPAddCommPro = 0x27,       ///< 0x27  增加工程备注
    eMsgAPPCreatePro = 0x28,        ///< 0x28  新建工程
    eMsgBatchReadRequest = 0x29,    ///< 0x29
    eMsgBatchReadRprt = 0x2A,       ///< 0x2A
    eMsgBatchWriteRequest = 0x2B,   ///< 0x2B
    eMsgBatchWriteRprt = 0x2C,      ///< 0x2C
	eMsgMovProgam = 0x2D,           ///< 0x2D
	eMsgECKGetState = 0x2E,         ///< 0x2E
	eMsgECKCtrlAlarm = 0x2F,        ///< 0x2F
	eMsgECKGetBatInfo = 0x30,       ///< 0x30
	eMsgECKCtrlPtz = 0x31,          ///< 0x31
    eMsgfindDeviceReq = 0x32,       ///< 0x32 发现设备请求命令
    eMsgfindDeviceResp = 0x33,      ///< 0x33 发现设备命令应答
    eMsgIPCfgReq = 0x34,            ///< 0x34 IP配置请求命令
    eMsgIPCfgResp = 0x35,           ///< 0x35 IP配置命令应答
    eMsgheartBeatReq = 0x36,        ///< 0x36 设备心跳信息请求命令
    eMsgheartBeatResp = 0x37,       ///< 0x37 设备心跳信心命令应答
    eMsgDataPath = 0x38,            ///< 0x38 修改文件存储路径指令
    eMsgAppSvrLocation = 0x39,      ///< 0x39
    eMsgAppSvrGetcatalog = 0x3a,    ///< 0x3a
    eMsgAppSvrCatalognum = 0x3b,    ///< 0x3b
	eMsgAppSvrUpdatecatalog = 0x3c, ///< 0x3c
	eMsgAppSvrRemovecatalog = 0x3d, ///< 0x3d
	eMsgAppSvrAddcomment = 0x3e,    ///< 0x3e
	eMsgAppSvrGetcomment = 0x3f,    ///< 0x3f
	eMsgAppSvrUpdatecomment = 0x40, ///< 0x40
    eMsgDACSetReq = 0x41,           ///< 0x41 DAC设备电压设置请求命令
    eMsgDACSetResp = 0x42,          ///< 0x42 DAC设备电压设置命令应答
    eMsgTMPGetReq = 0x43,           ///< 0x43 温度读取命令请求
    eMsgTMPGetResp = 0x44,          ///< 0x44 温度读取命令应答
    eMsgEnvirExt = 0x45,            ///< 0x45
    eMsgMta = 0x46,                 ///< 0x46
    eMsgCameraRpt = 0x47,           ///< 0x47
    eMsgADCGetReq = 0x48,           ///< 0x48 ADC数据读取命令
    eMsgADCGetResp = 0x49,          ///< 0x49 ADC数据读取命令应答
    eMsgCfgGtrlReq = 0x4A,          ///< 0x4A 配置文件控制命令
    eMsgCfgGtrlResp = 0x4B,         ///< 0x4B 配置文件控制命令应答
    eMsgWeatherReq = 0x4C,          ///< 0x4C 气象站信息获取
    eMsgWeatherResp = 0x4D,         ///< 0x4D 气象站信息获取应答
    eMsgFileListReq = 0x4E,         ///< 0x4E 文件列表获取
    eMsgFileListResp = 0x4F,        ///< 0x4F 文件列表获取命令应答
    eMsgFileGtrlReq = 0x50,         ///< 0x50 文件控制命令
    eMsgFileGtrlResp = 0x51,        ///< 0x51 文件控制命令应答
    eMsgImg          = 0x52,		///< 0x52
    eMsgUpdateReq     = 0x53,       ///< 0x53 请求更新
    eMsgUpdateResp = 0x54,          ///< 0x54 请求更新应答
    eMsgErrorBackReq = 0x55,        ///< 0x55 更新过程中异常回复
    eMsgErrorBackResp = 0x56,       ///< 0x56 更新过程中异常回复应答
    eMsgEventReq = 0x57,            ///< 0x57 上位机对设备事件使能请求
    eMsgEventResp = 0x58,           ///< 0x58 上位机对设备时间使能请求应答
    eMsgIntReq = 0x59,              ///< 0x59 程序中整型变量信息查询和设置命令
    eMsgIntResp = 0x5a,             ///< 0x5a 程序中整型变量信息查询和设置命令应答
    eMsgFloatReq = 0x5b,            ///< 0x5b 程序中浮点变量信息查询和设置命令
    eMsgFloatResp = 0x5c,           ///< 0x5c 程序中浮点变量信息查询和设置命令应答
    eMsgBoolReq = 0x5d,             ///< 0x5d 程序中布尔变量信息查询和设置命令
    eMsgBoolResp = 0x5e,            ///< 0x5e 程序中布尔变量信息查询和设置命令应答
    eMsgUnicastReq = 0x60,          ///< 0x60 单播设置及查询命令
    eMsgUnicastResp = 0x61,         ///< 0x61 单播设置及查询命令应答 
    eMsgParamReq = 0x62,            ///< 0x62 参数组设置及查询命令
    eMsgParamResp = 0x63,           ///< 0x63 参数组设置及查询命令应答
    eMsgalarmReq = 0x64,            ///< 0x64 报警信息阈值参数设置及查询命令
    eMsgalarmResp = 0x65,           ///< 0x65 报警信息阈值参数设置及查询命令应答
    eMsgIMUReq = 0x66,              ///< 0x66 IMU信息设置及查询命令
    eMsgIMUResp = 0x67,             ///< 0x67 IMU信息设置及查询命令应答

    eEventAlarmResp = 0x8000,       ///<  报警事件上报命令
}LiDAR_MSGID_E, LiDAR_CMDID_E;


//**********************************************************************
// body字段结构定义
//**********************************************************************
typedef enum LiDAR_STATMACH
{
    eLidarProgError      = 0,
    eLidarProgStandBy    = 1,
	eLidarProgDateSet    = 2,
    eLidarProgReady      = 3,
    eLidarProgEnvir      = 4,
    eLidarProgCamera     = 5,
    eLidarProgMotor      = 6,
    eLidarProgStage      = 7,
    eLidarProgLaser      = 8,
    eLidarProgStart      = 9,
    eLidarProgAction     = 10,
    eLidarStopAll        = 11,
    eLidargoldensucc     = 12,  ///< lidar处在golden程序中等待升级
    eLidargoldenerrfpga  = 13,  ///< lidar处在golden程序但是fpga分区损坏
    eLidargoldenerrimage = 14,  ///< lidar处在golden程序但是linux镜像分区损坏
    eLidargoldenerrsvr   = 15,  ///< lidar处在golden程序但是demosvr分区损坏
    eLidargoldenerrcfg   = 16,  ///< lidar处在golden程序但是配置文件分区损坏
    eLidarappsucc        = 17,  ///< lidar处在app程序中可以正常升级
    eLidarapperrsvr      = 18,  ///< lidar处在app程序中但是demosvr分区损坏
    eLidarapperrcfg      = 19,  ///< lidar处在app程序中但是配置文件分区损坏

    eLidarTempOverload   = 20,  ///< lidar温度过载
} LDRPROG_STAT_E,  LiDAR_STATMACH_E;

typedef enum SCDEV_CMD
{
    eDevCmdIdle  = 0,
    eDevCmdWork  = 1,
    eDevCmdSimu  = 2,
    eDevCmdReset = 3,
	eDevCmdAsk   = 4,    ///< 查询指令
    eDevCmdWorkOnce = 5, ///< 单次工作指令
    eDevCmdAutoWorkON = 6, ///< 上电启动工作
    eDevCmdAutoWorkOFF = 7, ///< 上电启动工作
    eDevCmdAskWorkOnce = 8, ///< 查询指令
}SCDEV_CMD_E;

typedef struct GROUND_DATA
{
    uint32_t header;
    float height;
    float staDeviation;
    uint32_t packetIndex;
    uint32_t checksum_crc32;
} GROUND_DATA_S;

typedef struct CSYSTEMTIME
{
    uint16_t tm_sec;    	/* Seconds (0-60) */
    uint16_t tm_min;    	/* Minutes (0-59) */
    uint16_t tm_hour;    	/* Hours (0-23) */
    uint16_t tm_mday;    	/* Day of the month (1-31) */
    uint16_t tm_mon;      /* Month (0-11) */
    uint16_t tm_year;     /* Year - 1900 */
    uint32_t   tm_msec ; 	/* before: wMilliseconds */
} CSYSTEMTIME_S;


typedef enum GNSS_TYPE
{
	eGNSS_Applanix = 0,        ///< eGpsApplanix
  	eGNSS_Trimble = 1,        ///< eGpsTrimble
  	eGNSS_Noval = 2,        ///< eGpsNoval
  	eGNSS_Javat = 3,        ///< eGpsJavat
  	eGNSS_Nmea = 4,        ///< eGpsNmea
  	eGNSS_Gsof = 5,        ///< eGpsGsof
  	eGNSS_Inpps = 6,        ///< eGPSInpps
  	eGNSS_Unicore = 7,		
  	eGNSS_Unic982 = 8,
  	eGNSS_Unic482 = 9,
  	eGNSS_SinoK823 =10,
} GNSS_TYPE_E, GPS_TYPE_E;

typedef struct SCDCMD_TIMSYN 
{
    CSYSTEMTIME_S syncTime;
    int tz_secs;               ///< secs, timezone,
    int diff_gps_utc;          ///< secs, the difference between GPS and UTC
    GNSS_TYPE_E gps_etype;     
    uint32_t ro_ttsecs;    ///<  Epoch time(time_t), auto calc. from syncTime
} SCDCMD_TIMSYN_S, SCDCMD_ENVIR_S;

typedef struct SCDRPT_ENVIR
{
    float temp[2];
    float hemi[2];
    GNSS_TYPE_E gps_etype;              
	volatile uint32_t gnstmiTime;  	///< 0x74 0x75 : YYMMDDHH, totally 4 Bytes ; before: gnsUtcTime or UtcTime
	volatile uint64_t gnsUtcSecs;       ///< ... check FPGA to make sure it is utc format ; before: utcSec
    uint16_t gns_satelites; 		///< 
    uint16_t imu_signals;         ///< ... ? make sure the meaning ; --- IMU
    float fAtg ;
    float fdiv ;					
    float fGPSx;
    float fGPSy; 
    float fGPSz;        	
    float incX;
    float incY;                    ///< inclinometer X and Y
} SCADA_STATE_S, SCDRPT_ENVIR_S;

typedef struct SCDRPT_ENVIR_EXT
{
    float heading;
    float pitch;
    float roll;
	bool pps ;
	float atgHeight;
} SCDRPT_ENVIR_EXT_S;

typedef struct  SCDRPT_PNTCLD
{
	LiDAR_STATMACH_E pntcld_state ;	
    uint32_t diskSize ;
    uint16_t intens_min;
    uint16_t intens_max;
    unsigned shortintens_max;      
    float ranges_min;
    float ranges_max;        		
    float datahandle_rate;
    float datahandle_volumn;   
    bool sdcard_flag;
} SCDRPT_PNTCLD_S, LiDAR_PNTCLD_S;

// laser
typedef struct SCDCMD_LASER
{
    SCDEV_CMD_E cmdID;
    int freqKHz;
    int ampPercent;
	uint16_t laser_freq_enable;
} SCDCMD_LASER_S;

typedef struct SCDRPT_LASER
{
    SCDEV_CMD_E statID;
    int freqKHz;
    int ampPercent;
    int optingTemp;
} SCDRPT_LASER_S;

typedef enum SCDSCN_STATE
{
    standReady = 0x0,
    eScanStand = 0x1,
    eScanError = 0x2,
} SCDSCN_STATE_E;

typedef struct SCDCMD_SCANER
{
    SCDEV_CMD_E cmdID;
    int scan_emode;
    float angSpeed;
    uint16_t roistart[4];
    uint16_t roistop[4];
} SCDCMD_SCANER_S;

typedef struct SCDRPT_SCANER
{
    SCDEV_CMD_E statID;
    uint32_t speedCount;
    SCDSCN_STATE_E speedState;
    int grade;
} SCDRPT_SCANER_S;
///< 转台工作模式
typedef enum SCDTURRET_MODE
{
  eTurretReset     = 0x00, ///< 重置
  eTurretContinue  = 0x01, ///< 连续模式
  eTurretArea      = 0x02, ///< 区域模式
  eTurretPosition  = 0x04, ///< 位置模式
  eTurretNextTrige = 0x08, ///< 下一个曝光点
  eTurretStop      = 0x10, ///< 停止
  eTurretSearch    = 0x20, ///< 查询
} SCDTURRET_MODE_E ;

typedef struct SCDCMD_TURRET
{
  SCDEV_CMD_E cmdID ;
  SCDTURRET_MODE_E stag_emode ;		//ref: SCDTURRET_MODE_E
  float angSpeed ;
  float strAngle;
  float stpAngle;
  uint16_t roistr[4];
  uint16_t roistp[4] ;
} SCDCMD_TURRET_S ;

typedef struct SCDRPT_TURRET
{
  SCDEV_CMD_E statID ;
  float currAng ;                     ///< 当前角度
} SCDRPT_TURRET_S ;

///< 相机曝光模式
typedef enum SCDCMR_MODE
{
  eCmrTimerNero,             ///< 时间曝光, 无零位触发
  eCmrTimerZero,             ///< 时间曝光, 有零位触发
  eCmrAngleNero,             ///< 角度曝光, 无零位触发
  eCmrAngleZero,             ///< 角度曝光, 有零位触发
  eCmrPosnNero,              ///< 位置曝光, 无零位触发
  eCmrPosnZero,              ///< 位置曝光, 有零位触发
  eCmrRangeNero,             ///< 距离曝光, 无零位触发
  eCmrRangeZero,             ///< 距离曝光, 有零位触发
} SCDCMR_MODE_E;

typedef struct SCDCMD_CAMERA
{
  SCDEV_CMD_E cmdID ;        	///< 
  SCDCMR_MODE_E trigger_emode ; ///< ref: SCDCMR_MODE_E
  int interval ;               	///< 
  int angOffset ;              	///< 
  uint32_t msgCount;       	///< 
  float strAngle;
  float stpAngle;   	        ///< 
} SCDCMD_CAMERA_S ;

#define CMR_NUMBER 6 // 1 trige + 6 flash
typedef struct SCDRPT_CAMERA
{
  SCDEV_CMD_E statID ;        	///<
  uint32_t flashCount[CMR_NUMBER] ;
  uint32_t trigCount ;
} SCDRPT_CAMERA_S ;

typedef struct SCDCMD_DATA_DIR
{
    SCDEV_CMD_E cmdID;
	char dataDir[60];
}SCDCMD_DATA_DIR_S, SCDRPT_DATA_DIR_S;

typedef struct SCDCMD_MTA
{
	uint32_t freq12;//freq1[15:0] freq2[31:16]
	uint32_t freq34;//freq3[15:0] freq4[31:16]
	uint32_t freqRef;//base freq
	uint32_t freqRefSPeriod;//freq start Period
	uint32_t scanFreq;
	uint32_t onoff;
} SCDCMD_MTA_S;

typedef enum {
	eImgExport,
	eImgClear,
	eImgAskState,
	eImgSetparam,
} IMG_CMD_S;

typedef struct {
	int speed;
	int iso;
} SCDCMD_IMG_PARAM_S;

typedef struct {
	IMG_CMD_S cmdId;
	union {
		SCDCMD_IMG_PARAM_S param;
		char dataDir[60];
	};
} SCDCMD_IMG_S;

typedef struct {
	IMG_CMD_S cmdId;
	int exportPercent;
} SCDRPT_IMG_S;

#define MPMODE_REVISE_RANGE (120)  ///< MP模式过滤杂点数据 单位m
typedef enum SCDMODE_PROGRAM
{
	eProgrmModeNon = 0,
	eProgrmModeMP = 1,
}SCDMODE_PROGRAM_E;

#define ROI_NUMBER (2)
typedef enum SCDMODE_DATAPLACE
{
	eNetFlow = 0x01,
	eDeviceSave = 0x02,
	ePCSave = 0x04,
	eUSBSave = 0x08,
	eMemorySave = 0x10,
}SCDMODE_DATAPLACE_E;


typedef enum LiDARImp_Action
{
	eFileStay, // no action to file handler
	eFileNew,
	eFileRenew,
	eFileClose,
	eFileCloseAll,
	eFileImpClose,
	eFileIssClose,
} LiDARImp_Action_E;


typedef enum Point_DataFlow{ 
  ePoints_NULL ,
  ePoints_NetFlow , 	//eNetFlow = 0x01,
  ePoints_DevSave , 	//eDeviceSave = 0x02 ,
  ePoints_PcSave  , 	//ePCSave = 0x04 , 
  ePoints_UsbSave , 	//eUSBSave = 0x08,
  ePoints_MemSave , 	//eMemorySave = 0x10,
} Point_DataFlow_E;

typedef enum {
  eDataNULL   ,
  eIMPDATA	  , //= 0x1 ,
  eXYZIDATA   , //= 0x2 ,
  eARIDATA	  , //= 0x4 ,
  eCRSDATA	  , //= 0x8 ,
  eWUDDATA	  , //= 0x10,
  eNARIDATA   , //= 0x12,
  eGEOXYZDAT  , //= 0x14,
  eCPTDATA	  , //= 0x16,
  eSTDATA	  , //= 0x18,
  ePLRDATA	  , //= 0x20,
  eISSDATA	  , //= 0x21,
  ePMISSDATA  , //= 0x22,
} DATA_TYPE_E , Point_DataType_E ;

//
typedef struct SCDCMD_DATACTRL
{
	SCDEV_CMD_E cmdID ;
	Point_DataFlow_E data_flow ;				// storePlace ;
	Point_DataType_E data_type ; 		 		// saving format
} SCDCMD_DCTRL_S, SCDCMD_DATACTRL_S;

typedef struct SCDCMD_PROGRM
{
    SCDEV_CMD_E cmdID;
    int echo_etype;
    int laserFreqKHz;
    int laserFirePwr;
    float scanAngleSpeed;
    float scanAngleSpeed1; ///< 公转电机转速 hz 0.01
    float scanAnglePhase;  ///< 自转电机相位 角度 0.01
    float scanAnglePhase1; ///< 公转电机相位 角度 0.01
	int range_from, range_to;    //the ranging scope
    int prog_emode;
}SCDCMD_PROGRM_S;

typedef struct SCDCMD_MOV_PROGRM
{
	float sHoriAngle;// 0到360
	float sVertAngle;// -25到15
	float ptzTurnSpd;// 0.1到0.5度每秒, 0为不转
	unsigned char scanDataFrame; //frame number
	unsigned char state;
	uint16_t recvTmOutMs;//超时时间ms,0表示不设置
	SCDCMD_PROGRM_S prog;
	unsigned char ptzPosDelayTime;//转到指定位置后延时扫描时间，单位s
} SCDCMD_MOV_PROGRM_S;//后面考虑移出

typedef struct SCDCMD_GET_PROSTATE
{
	int state;//1表示运行中，0表示空闲, -1,表示设备出错
    char SN[28];
} SCDCMD_GET_PROSTATE_S;

typedef struct SCDCMD_CTRL_ALARM
{
	int onoff;//1打开0关闭
	unsigned char vol;//level 1-30
} SCDCMD_CTRL_ALARM_S;

typedef struct SCDCMD_GET_BATINFO
{
	uint32_t power;
	float voltage;
	float current;
} SCDCMD_GET_BATINFO_S;

typedef struct SCDCMD_CTL_PTZ
{
	unsigned char ctrl;//1 reset, 2 move to pos, 3 get pos
	float HoriAngle;  // 水平转动到的角度
	float VertAngle;  // 垂直转动到的角度
    float RollAngle;  // 转台的翻滚角
} SCDCMD_CTL_PTZ_S;


typedef struct SCDCMD_APPDeviceMeta
{
    int request;  // 
} SCDCMD_APPDeviceMeta_S;


typedef struct SCDRPT_APPDeviceMeta
{
    float tempDc ;       // 温度，单位°
    float batPer;        // 电池电量百分比
    float tfCapPer;      // 容量百分比 如：60.1，表示容量已用60.1%
    int deviceID ;       // 序列号
    char softVer[16];    // 固件版本
    char hwVer[8];       // 硬件版本
    char proctorData[8]; // 生产日期,如：20220801
    LiDAR_STATMACH_E execstatus;  // 程序启动状态
} SCDRPT_APPDeviceMeta_S;


typedef struct SCDCMD_APPCountPro
{
    int request;  // 
} SCDCMD_APPCountPro_S;

typedef struct SCDRPT_APPCountPro
{
  uint16_t count;  //如5个工程
} SCDRPT_APPCountPro_S;



typedef struct SCDCMD_APPGetPro
{
    uint16_t num;  // 如：0-4
} SCDCMD_APPGetPro_S;

typedef struct SCDRPT_APPGetPro
{
  char progId[13]; //220810181012
  char proName[22]; //工程名称
} SCDRPT_APPGetPro_S;


typedef struct SCDCMD_APPGetProg
{
    char progId[13];  //工程id
} SCDCMD_APPGetProg_S;


typedef struct SCDRPT_APPGetProg
{
   char progStartTime[16];  //开始时间: 20220803181000
   uint32_t proRunTime; // 作业时长，单位秒
   uint16_t  proRoom; //工程作业所占空间大小,单位MB
   char pos[40];
} SCDRPT_APPGetProg_S;

typedef struct SCDCMD_APPGetCommPro
{
    char progId[13];  //工程id
} SCDCMD_APPGetCommPro_S;

typedef struct SCDRPT_APPGetCommPro
{
  char proComment[53]; //工程备注
} SCDRPT_APPGetCommPro_S;


typedef struct SCDCMD_APPUpdatePro
{
    char progId[13];   //工程id
    char newProName[22];   //新工程名称
} SCDCMD_APPUpdatePro_S;


typedef struct SCDRPT_APPUpdatePro
{
    int updateResult;  //0 表示成功，其他值失败
} SCDRPT_APPUpdatePro_S;


typedef struct SCDCMD_APPDeletePro
{
    char progId[13];   //工程id
} SCDCMD_APPDeletePro_S;


typedef struct SCDRPT_APPDeletePro
{
    int deleteResult;  //0 表示成功，其他值失败
} SCDRPT_APPDeletePro_S;

typedef struct SCDCMD_APPAddCommPro
{
    char progId[13];   //工程id
    char proComment[53];
} SCDCMD_APPAddCommPro_S;


typedef struct SCDRPT_APPAddCommPro
{
    int addResult;  //0 表示成功，其他值失败
} SCDRPT_APPAddCommPro_S;


typedef struct SCDCMD_APPCreatePro
{
  char proName[22]; //工程名称
  char pos[40];//位置信息
} SCDCMD_APPCreatePro_S;

typedef struct SCDRPT_APPCreatePro
{
   int newResult;
} SCDRPT_APPCreatePro_S;

typedef struct SCDRPT_PROGRAM
{
    SCDEV_CMD_E cmdID;
    int result;    //0 表示成功，其他值失败
    char progId[14];  //当前工程id
    int echo_etype;
    int laserFreqKHz;
    int laserFirePwr;
    float scanAngleSpeed;
    float scanAngleSpeed1; ///< 公转电机转速 hz 0.01
    float scanAnglePhase;  ///< 自转电机相位 角度 0.01
    float scanAnglePhase1; ///< 公转电机相位 角度 0.01
	int range_from, range_to;    //the ranging scope
    int prog_emode;
    uint16_t isautostart;
} SCDRPT_PROGRAM_S;

typedef struct SCDCMD_REGISTER
{
	unsigned char flag;//0 表示读，1表示写， 2，标识从xml文件中读  3，表示设置xml配置项且临时生效
	uint32_t reg;
	uint32_t value;
}SCDCMD_REGISTER_S, SCDRPT_REGISTER_S;

//批量读请求
typedef struct SCDCMD_BATCH_READ
{
    //批量地址数组，最大可读取16组地址数据，当只需查询3组地址数据时，后13组数据用0填充
 	uint32_t request[16];
}SCDCMD_BATCH_READ_S;

//批量读反馈
typedef struct SCDRPT_BATCH_READ
{
    //批量地址数组对应的value
	uint32_t response[16];
}SCDRPT_BATCH_READ_S;

//批量写请求
typedef struct SCDCMD_BATCH_WRITE
{
    //批量地址、数值数组，地址、数值交替出现，最多可写8组数据，当只需写3组数据时，后5组数据用0填充
	uint32_t request[16];
}SCDCMD_BATCH_WRITE_S;

//批量写反馈
typedef struct SCDRPT_BATCH_WRITE
{
    //批量地址、数值数组，地址、数值交替出现
	uint32_t response[16];
}SCDRPT_BATCH_WRITE_S;

/// 发现设备命令内容结构
typedef struct SCDCMD_FINDDEV
{
    unsigned char request[64]; ///< 这里的body只是为了占位里面的命令内容将会被忽略
}SCDCMD_FINDDEV_S;

/// 发现设备命令回应结构
typedef struct SCDRPT_FINDDEV
{
    uint8_t arrui8Vender[16];   ///< 设备厂商名称
    uint8_t arrui8devName[16];  ///< 设备名称
    uint32_t ui32devVersion;    ///< 设备程序版本
    uint32_t ui32devStatus;     ///< 设备状态
    uint32_t ui32devMAC[2];     ///< 设备MAC
    uint32_t ui32devIPMode;     ///< 设备当前IP配置模式
    uint32_t ui32devIP;         ///< 设备当前IP
    uint32_t ui32devMask;       ///< 设备当前子网掩码
    uint32_t ui32devGateway;    ///< 设备当前网关
}SCDRPT_FINDDEV_S;

/// IP模式配置设置命令
typedef struct  SCDCMD_IPCFG
{
    uint32_t ui32mode;   ///< IP配置模式
    uint32_t ui32IP;     ///< 当前配置的IP
    uint32_t ui32mask;   ///< 当前要配置的子网掩码
    uint32_t ui32gateway;///< 当前要配置的网关
    uint32_t ui32MAC[2]; ///< MAC地址，根据这个去选择是否接受配置
}SCDCMD_IPCFG_S;
/// IP模式配置命令返回
typedef struct  SCDRPT_IPCFG
{
    uint32_t ui32status; ///< 返回状态可能的定义见LiDARIPCfg.cpp中ipcfg函数返回值
}SCDRPT_IPCFG_S;

/// 心跳包命令
typedef struct  SCDCMD_HEARTBEAT
{
   uint32_t devstatus;   ///< 写的话要写的状态值。0 - 空闲  1 - 只读 2 - 控制
   uint8_t  sn[17];
}SCDCMD_HEARTBEAT_S;
/// 心跳包命令返回
typedef struct  SCDRPT_HEARTBEAT
{
    uint32_t cmdstatus;  ///< 读的话返回设备状态，写的话返回写入后的状态
    uint8_t  sn[17];; ///< 命令占位
}SCDRPT_HEARTBEAT_S;

/// DAC的设备类型
typedef enum DAC_DEV_TYPE
{
    DAC101 = 0,
    DAC104 = 1,
}DAC_DEV_TYPE_E;
/// DAC设备通道表示，目前最多4通道
typedef enum DAC_DEV_CHANNEL
{
    CHANNEL_0 = 0,
    CHANNEL_1 = 1,
    CHANNEL_2 = 2,
    CHANNEL_3 = 3,
    CHANNEL_ALL = 4,
}DAC_DEV_CHANNEL_E;
/// DAC电压输出设置
typedef struct SCDCMD_DAC_S
{
    DAC_DEV_TYPE_E   dactype;
    DAC_DEV_CHANNEL_E dacchannel;
    float  Voltage;
}SCDCMD_DAC_S;
/// DAC电压设置反馈
typedef struct SCDRPT_DAC
{
    uint32_t status;  ///< 设置状态
}SCDRPT_DAC_S;
/// 温度芯片的设备类型
typedef enum TMP_DEV_TYPE
{
    TMP422 = 0,
    FPGA_CODE = 1,
}TMP_DEV_TYPE_E;
/// 温度芯片设备通道表示，目前最多5通道
typedef enum TMP_DEV_CHANNEL
{
    TMP_LOCAL = 0,
    TMP_REMOTE_1 = 1,
    TMP_REMOTE_2 = 2,
    TMP_REMOTE_3 = 3,
    TMP_REMOTE_4 = 4,
}TMP_DEV_CHANNEL_E;
/// 温度读取请求
typedef struct SCDCMD_TMP
{
    TMP_DEV_TYPE_E    tmptype;
    TMP_DEV_CHANNEL_E tmpchannel;
}SCDCMD_TMP_S;
/// 温度读取反馈
typedef struct  SCDRPT_TMP
{
    TMP_DEV_TYPE_E    tmptype;
    TMP_DEV_CHANNEL_E tmpchannel;
    uint32_t status;    ///< 返回读取状态
    float    tmp_value; ///< 返回读取到的值
}SCDRPT_TMP_S;

/// ADC转换的设备类型
typedef enum ADC_DEV_TYPE
{
    XADC = 0,
}ADC_DEV_TYPE_E;
/// ADC读取请求
typedef struct SCDCMD_ADC
{
    ADC_DEV_TYPE_E    adctype;
    uint32_t adcchannel;
}SCDCMD_ADC_S;
/// ADC读取反馈
typedef struct  SCDRPT_ADC
{
    ADC_DEV_TYPE_E    adctype;
    uint32_t  adcchannel;
    uint32_t status;    ///< 返回读取状态
    float    adc_value; ///< 返回读取到的值
}SCDRPT_ADC_S;

/// 配置文件控制请求
typedef struct SCDCMD_CFGCTRL
{
    uint32_t flag;           ///< 0为读，1为写
    uint32_t total_lens;     ///< 如果标志位读，则是已读取到的数量，如果是写，则是要写入的总数
    uint32_t data_offset;    ///< 如果为读，此位无效，如果位写，此为是正在写入数据在整个数据中的偏移
    uint8_t  data_buf[1018]; ///< 文件数据
}SCDCMD_CFGCTRL_S;
/// 配置文件控制反馈
typedef struct  SCDRPT_CFGCTRL
{
    uint32_t  total_lens;     ///< 如果为读命令的回应，要读取文件的整体大小，如果是写命令的应答，则是写入了多少文件
    uint32_t  data_offset;    ///< 如果为读命令的回应，就是当前包在整体的偏移位置，如果写写命令的应答，此位无效
    uint8_t  data_buf[1020];  ///< 文件数据
}SCDRPT_CFGCTRL_S;

/// 气象站信息请求，body任填可以为0.
/// 气象站信息反馈
typedef struct  SCDRPT_WEATHER
{
    float  temperature;     ///< 温度
    float  humidity;        ///< 湿度
    float  light_intensity; ///< 光照强度
    float  atmos;           ///< 大气压
}SCDRPT_WEATHER_S;

typedef enum FILE_TYPE
{
    FILE_LIDAR_CFG = 0,   // 配置文件
    TYPE_DIR  = 1,        // 获取的是具体目录信息
    TYPE_FILE = 2,        // 获取的是具体的文件信息
}FILE_TYPE_E;

typedef struct SCDCMD_FILE_LIST
{
    char dir_name[128];    ///< 目录名称，如果写成文件的话，后续列表将返回-1.
    uint16_t recv_num;     ///< 已接收的文件名数量
}SCDCMD_FILE_LIST_S;

typedef struct SCDRPT_FILE_LIST
{
    FILE_TYPE_E file_type; ///< 文件类型
    uint16_t all_num;      ///< 所有文件计数
    uint16_t this_num;     ///< 当前包文件数量计数
    char file_name[1016];  ///< 文件名以'\0'分割。
}SCDRPT_FILE_LIST_S;

typedef struct SCDCMD_FILE
{   
    char file_name[128];     ///< 读写的文件名
    uint32_t flag;           ///< 0为读，1为写，2为删除
    uint32_t total_lens;     ///< 如果标志位读，则是已读取到的数量，如果是写，则是要写入的总数
    uint32_t data_offset;    ///< 如果为读，此位无效，如果位写，此为是正在写入数据在整个数据中的偏移
    uint8_t  data_buf[888];  ///< 文件数据
}SCDCMD_FILE_S;

typedef struct SCDRPT_FILE
{
    uint32_t  total_lens;     ///< 如果为读命令的回应，要读取文件的整体大小，如果是写命令的应答，则是写入了多少文件
    uint32_t  data_offset;    ///< 如果为读命令的回应，就是当前包在整体的偏移位置，如果写写命令的应答，此位无效
    uint8_t  data_buf[1020];  ///< 文件数据
}SCDRPT_FILE_S;


typedef enum UPDATE_TYPE
{
    UP_IDLE = 0,  ///< 未升级状态
    UP_FPGA = 1,  ///< 升级FPGA分区
    UP_IMAGE  = 2,  ///< 升级image分区
    UP_SVR  = 3,  ///< 升级demosvr程序
    UP_CFG  = 4,  ///< 升级配置文件
    UP_APP  = 5,  ///< 升级除golden之外的所有文件
    UP_REVISE  = 6,  ///< 升级revise文件
    UP_ALL  = 0xFFFFFFFF,  ///< 升级整体BOOT分区
}UPDATE_TYPE_E;


typedef struct SCDCMD_UPDATECHECK
{
    int port ;           ///< 请求更新服务器端口
    char ip[16];         ///< 请求更新服务器ip
    char version[6];     ///< 当前版本号
    char packet_name[26];///< 当前包名
    UPDATE_TYPE_E  type;     ///< 升级类型
    char packet_MD5[33];     ///< 当前包MD5校验字符
    char packet_lens[15];    ///< 当前包长度
    char packet_version[16]; ///< 当前包版本
} SCDCMD_UPDATECHECK_S;


typedef enum DEVICE_UPSTATUS
{
    UP_SUCCESS  = 0, ///< 命令成功，且确认需要更新。确认需要更新的操作在ECK2.0的设备上使用
    UP_NOUP     = 1, ///< 当前无需更新
    UP_TIMEOUT  = 2, ///< 访问更新数据服务器超时（ECK2.0设备使用）
    UP_DISKERR  = 3, ///< 当前磁盘空间不足
    UP_PARAMERR = 4, ///< 升级参数错误
}DEVICE_UPSTATUS_E;

typedef struct SCDRPT_UPDATECHECK
{
    DEVICE_UPSTATUS_E status;  // 0 更新成功，1 无更新，2超时，3磁盘空间不足
    UPDATE_TYPE_E  type;       ///< 升级类型
    char packet_version[16];   ///< 待升级分区中原有的版本
} SCDRPT_UPDATECHECK_S;


typedef enum DEVICE_UPERR
{
    UP_ERR_SUCCESS  = 0, ///< 升级成功
    UP_ERR_PARSE    = 1, ///< 数据解压失败
    UP_ERR_REPACE   = 2, ///< 替换失败
    UP_ERR_REBOOT   = 3, ///< 重启失败
    UP_ERR_DOWNLODA = 4, ///< 数据主动下载失败
    UP_ERR_PARAMERR = 5, ///< 升级参数错误
}DEVICE_UPERR_E;

typedef struct SCDCMD_ERRORBACK
{
    int status;  // 0 成功  1解压失败  2替换失败 3重启失败 4下载失败
} SCDCMD_ERRORBACK_S;

typedef struct SCDRPT_ERRORBACK
{
    DEVICE_UPERR_E status;  // 0 成功  1解压失败  2替换失败 3重启失败 4下载失败
} SCDRPT_ERRORBACK_S;



typedef enum RW_FLAG
{
    READ = 0,   ///< 读标志
    WRITE = 1,  ///< 写标志
}RW_FLAG_E;

typedef enum RW_SELELCT_E
{
    CLK5MTOUNIXTIME = 0x0,          ///< 读标志
    APDTEMPERATURE  = 0x01,         ///< 设备apd温度
    // 预留
    EVENT_UDPCHN_PEERADDR = 0x40,      ///< 事件上报远端地址配置
    EVENT_UDPCHN_PEERPORT = 0x41,      ///< 事件上报远端端口配置
    NTP_SERVER_IP = 0x42,              ///< ntp时间同步服务端IP地址
    NTP_UPDATE_INTERVAL = 0x43,        ///< ntp时间同步间隔时间单位S
}RW_SELELCT_E;

typedef struct SCDCMD_BOOL
{
    RW_FLAG_E flag;           ///< 0为读，1为写;
    RW_SELELCT_E select;
    bool istrue;
} SCDCMD_BOOL_S;

typedef struct SCDRPT_BOOL
{
    RW_FLAG_E flag;           ///< 0为读，1为写;
    RW_SELELCT_E select;
    bool istrue;
} SCDRPT_BOOL_S;

typedef struct SCDCMD_INT
{
    RW_FLAG_E flag;           ///< 0为读，1为写;
    RW_SELELCT_E select;
    int val;
} SCDCMD_INT_S, SCDRPT_INT_S;

/// 上位机可开启的事件类型
typedef enum EVENT_TYPE
{
    EVENT_ALL_OFF     = 0x00000000,   ///< 所有事件都关闭
    EVENT_TEMPERATURE = (0x00000001 << 0),   ///< 温度报警事件
    EVENT_ALL         = 0xFFFFFFFF,   ///< 所有事件都打开
}EVENT_TYPE_E;

typedef struct SCDCMD_EVENT
{
    uint32_t flag;           ///< 0为读，1为写
    uint32_t dstip;
    uint16_t dstport;
    EVENT_TYPE eventtype;
}SCDCMD_EVENT_S;

typedef struct SCDRPT_EVENT
{
    uint32_t status;         ///< 读写状态
    uint32_t dstip;
    uint16_t dstport;
    EVENT_TYPE eventtype;
}SCDRPT_EVENT_S;

// 报警事件ID,设备主动上报用
typedef enum EVENT_ALARM_ID
{
    EVENT_NORMAL = 0x00,           // 正常
    EVENT_TEMP_WARN = 0x01,        // 设备温度偏高
    EVENT_TEMP_OVERLOAD = 0x02     // 设备温度过载
}EVENT_ALARM_ID_E;

// 报警事件
typedef struct SCDRPT_EVENT_ALARM
{
    EVENT_ALARM_ID_E id;
}SCDRPT_EVENT_ALARM_S;

/// 文件头结构体发送结构体, 获取isf文件头有效
typedef struct SCDCMD_HEADER
{
    uint32_t receiverip;   ///< 接收者IP
    uint16_t receiverport; ///< 接收者端口
}SCDCMD_HEADER_S;

/// 文件头状态应答,isf文件头有效
typedef struct SCDRPT_HEADER
{
    uint32_t result;   ///< 文件头获取结果
}SCDRPT_HEADER_S;

/// 数据单播信息设置
typedef struct SCDCMD_UNICAST
{
    uint32_t flag;           ///< 0为读，1为设置
    uint32_t dstip;          ///< 数据发送目的IP设置
    uint16_t dstfastport;    ///< 快数据目的端口设置
    uint16_t dstslowport;    ///< 慢数据目的端口设置
}SCDCMD_UNICAST_S;

/// 数据单播信息应答
typedef struct SCDRPT_UNICAST
{
    uint32_t status;         ///< 读写结果, 如果为读后成员有效
    uint32_t dstip;          ///< 当前数据目的IP
    uint16_t dstfastport;    ///< 当前快数据目的端口
    uint16_t dstslowport;    ///< 当前慢数据目的端口
}SCDRPT_UNICAST_S;

/// 用户设定的参数类型
typedef enum PARAM_TYPE
{
    PARAM_DEFAULT = 0x00,   ///< 默认参数组
    PARAM_USETSET = 0x01,   ///< 用户参数组
}PARAM_TYPE_E;

typedef struct SCDCMD_PARAMCTRL
{
    uint32_t flag;            ///< 0为加载，1为保存, 2为设置
    PARAM_TYPE_E param_type;  ///< 设定的参数类型
}SCDCMD_PARAMCTRL_S;

typedef struct SCDRPT_PARAM
{
    uint32_t status;          ///< 读写状态
    PARAM_TYPE_E param_type;  ///< 设定的参数类型
}SCDRPT_PARAMCTRL_S;

typedef struct SCDCMD_ALARMPARAM
{
    uint32_t flag;            ///< 0为读取，1为设置
    float temp_warn;          ///< 温度报警阈值
    float temp_overload;      ///< 温度过载报警阈值
}SCDCMD_ALARMPARAM_S;

typedef struct SCDRPT_ALARMPARAM
{
    uint32_t status;          ///< 读写状态
    float temp_warn;          ///< 温度报警阈值
    float temp_overload;      ///< 温度过载报警阈值
}SCDRPT_ALARMPARAM_S;

typedef struct SCDCMD_IMU
{
    SCDEV_CMD_E cmdID;
}SCDCMD_IMU_S;

typedef struct SCDRPT_IMU
{
    uint32_t status;
    SCDEV_CMD_E cmdID;
}SCDRPT_IMU_S;
/***********************************************************************
 ***********************************************************************
 **** Communication Interface
 ***********************************************************************
 ***********************************************************************/
typedef struct cmd_lidar_L1 {
    LiDAR_MSGID_E cmdId ;               //!<
    int isAsk ;
    union {
        SCDCMD_ENVIR_S envCtrl;            //! SCDCMD_TIMSYN_S  synData ;
        SCDRPT_ENVIR_S envRprt;
		SCDRPT_ENVIR_EXT_S envExtRprt;
        SCDRPT_PNTCLD_S pntRprt;        //! real time data collection, before : infoData
        SCDCMD_LASER_S lsrCtrl;        //!
        SCDRPT_LASER_S lsrRprt;
        SCDCMD_SCANER_S scnCtrl;        //!
        SCDRPT_SCANER_S scnRprt;
        SCDCMD_PROGRM_S prgCtrl;        //!
        SCDRPT_PROGRAM_S prgRprt;
        SCDCMD_REGISTER_S regCtrl;        //!
        SCDCMD_DATACTRL_S dataCtrl;
		SCDCMD_TURRET_S  stgData ;
		SCDRPT_TURRET_S  stgRprt;
		SCDCMD_CAMERA_S  pmCmrData ;		//!
		SCDRPT_CAMERA_S  cmrRprt;
		SCDCMD_MTA_S 	mtaData;
		SCDCMD_IMG_S	imgCtrl;
		SCDRPT_IMG_S	imgRprt;
		SCDCMD_APPDeviceMeta_S appDevMeatCtrl;
        SCDRPT_APPDeviceMeta_S appDevMeatRprt;
        SCDCMD_APPCountPro_S   appCountCtrl;
        SCDRPT_APPCountPro_S   appCountRprt;
        SCDCMD_APPGetPro_S     appGetProCtrl;
        SCDRPT_APPGetPro_S     appGetProRprt;
        SCDCMD_APPGetProg_S    appGetProgCtrl;
        SCDRPT_APPGetProg_S    appGetProgRprt;
        SCDCMD_APPGetCommPro_S appGetCommProCtrl;
        SCDRPT_APPGetCommPro_S appGetCommProRprt;
        SCDCMD_APPUpdatePro_S  appUpdateProCtrl;
        SCDRPT_APPUpdatePro_S  appUpdateProRprt;
        SCDCMD_APPDeletePro_S  appDeleteProCtrl;
        SCDRPT_APPDeletePro_S  appDeleteProRprt;
        SCDRPT_APPAddCommPro_S appAddCommProRprt;
        SCDCMD_APPCreatePro_S  appCreateProCtrl;
        SCDRPT_APPCreatePro_S  appCreateProRprt;

        SCDCMD_BATCH_READ_S    batchReadCtrl;
        SCDRPT_BATCH_READ_S    batchReadRprt;
        SCDCMD_BATCH_WRITE_S   batchWriteCtrl;
        SCDRPT_BATCH_WRITE_S   batchWriteRprt;

		SCDCMD_DATA_DIR_S	dataDir;

        SCDCMD_MOV_PROGRM_S movPrgCtrl;
		SCDCMD_GET_PROSTATE_S getPrgState;
		SCDCMD_CTRL_ALARM_S ctrlAlarm;
		SCDCMD_GET_BATINFO_S getBatInfo;
		SCDCMD_CTL_PTZ_S ctrlPtz;

        SCDCMD_DAC_S setdacinfo;
        SCDRPT_DAC_S setdacrprt;
        SCDCMD_TMP_S gettempinfo;
        SCDRPT_TMP_S gettemprprt;
        SCDCMD_ADC_S getadcinfo;
        SCDRPT_ADC_S getadcrprt;

        SCDCMD_BOOL_S boolcfgreq;
        SCDRPT_BOOL_S boolcfgresp;
        SCDCMD_INT_S intcfgreq;
        SCDRPT_INT_S intcfgresp;
        
        SCDRPT_EVENT_ALARM_S alarm;

        SCDCMD_HEADER_S headinfo;
        SCDRPT_HEADER_S headresp;

        SCDCMD_UNICAST_S unicastinfo; 
        SCDRPT_UNICAST_S unicastresp;

        SCDCMD_EVENT_S eventinfo;
        SCDRPT_EVENT_S eventresp;

        SCDCMD_PARAMCTRL_S paraminfo;
        SCDRPT_PARAMCTRL_S paramresp;

        SCDCMD_ALARMPARAM_S alarminfo;
        SCDRPT_ALARMPARAM_S alarmresp;

        SCDCMD_IMU_S imuinfo;
        SCDRPT_IMU_S imuresp;

        char msgstream[64];
    };
	
	cmd_lidar_L1() {
		cmdId = eLidarReset ; // eMsgSoftCk ;
	  isAsk = false ;
	}
} LIDAR_CMDRPT_U;

typedef struct comu_lidar_L1 {
    const char command_mark[8] = "A7C7A5C";
    LIDAR_CMDRPT_U commander;
    unsigned int check_sum;
} LIDAR_COMMAND_S ;

//**********************************************************************
// openlidar协议结构
//**********************************************************************
const char LIDAR_COMMAND_MARK[8] = "A7C7A5C"; /// openlidar协议同步字
/// OpenLiDAR协议命令头
typedef struct OPEN_LIDAR_HEAD 
{
    char     strsyncword[8]; ///< 同步字，固定为"A7C7A5C"
    uint16_t ui16cmdid;      ///< 命令id，具体可接受定义见枚举类型LiDAR_MSGID_E定义
    uint16_t ui16lens;       ///< body段的实际长度，可以设置为0，如果设置为0的话则body段为64字节
    uint32_t reqid;          ///< reqid命令包的请求ID，回复包的回复ID也使用此位并且回复包需要和命令包一致
}OPEN_LIDAR_HEAD_S;

typedef struct OPENLIDAR_BODY
{
    uint32_t msgstream[LIDAR_BODY_MAX];
} OPENLIDAR_BODY_S ;

/// OpenLiDAR协议命令尾
typedef struct OPEN_LIDAR_TAIL
{
   uint32_t check_sum;      ///< 协议包除去同步字后整体的crc32校验值
}OPEN_LIDAR_TAIL_S;

/// 新的OpenLiDAR协议结构
// typedef struct LIDAR_COMMAND_S
// {
//     OPEN_LIDAR_HEAD_S sthead;    ///< 协议命令头
//     OPENLIDAR_BODY_S commander; ///< 请求包或应答包的具体数据
//     OPEN_LIDAR_TAIL_S sttril;    ///< 尾包，crc校验值
// } LIDAR_COMMAND_S;
//**********************************************************************
#pragma pack()

#endif

