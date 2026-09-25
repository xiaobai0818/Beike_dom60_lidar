/**
 * @file lidar_comm.h
 * @author yukai
 * @brief 对外公共头文件
 * @version 0.1
 * @date 2023-11-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __LIDAR_COMM_H__
#define __LIDAR_COMM_H__

#include <iostream>
#include <string.h>
#include <vector>

#include "lidar_error_code.h"
#include "logger.h"
#include "impHeader.h"
#include "isfHeader.h"

/// 解码时回波数量
const uint8_t ECHO_NUM_DEC = 8;
const uint8_t ECHO_MAX_INDEX = ECHO_NUM_DEC - 1;
/// 解算时回波数量
const uint8_t ECHO_NUM_CALC = 4;
const uint8_t ECHO_MAX_CALC = ECHO_NUM_CALC - 1;
/// 一圈是360度，单位0.01
const uint16_t ONE_CIRCLE_ANGLE = 36000;
const float ANGLE_COEFF = 0.01;
/// @brief 4mm/LSB
const float RANGE_WIDE_LSB_4MM = 0.004;
/// @brief LSB/m
const double RANGE_WIDE_LSB_M = 250;
const float MIN_INVALID_VALUE = -999.0;
const float MAX_INVALID_VALUE = 999.0;
const float MAX_ANGLE = 360.0;
const double PI = 3.14159265358979323846;
const uint32_t T0_Clock_Freq = 5000000;

const double S_TO_MICRO = 1000000.0;  ///< 秒转微秒
const double MICRO_TO_S = 0.000001;  ///< 微秒转秒

/// 定义zero，
const float ZERO_F = 1e-7;   ///< #define ZEROF 1e-7

/// 温度校正常量
const int32_t TEMPER_MIN = 0; 
const int32_t TEMPER_MAX = 99;
const float TEMPER_RANGE_COFF = 0.01;
const int32_t TEMPER_NUM = 100;

/// 跨周检测常量，即上一时间-当前时间 > WEEK_SECOND_THRESHOLD
const int WEEK_SECOND_THRESHOLD = 604000;  //zks 2022.05.21: 

//设置最大mta倍数值
const uint16_t MAX_MTA_NUM = 10;

const uint16_t RECV_ISF_HEADER_PORT = 11000;

/// 设备类型区分
typedef enum DevType
{
    DEV_ISF = 0,  ///< 导航设备
    DEV_IMP = 1,  ///< 测绘设备
}DevType_E;

/// 设备协议区分
typedef enum DevProto
{
	PROTO_NULL = 0x0000,
    /// 测绘类
    IMP_V6_COM = 0x0001,	///< 公共测绘设备
    /*IMP_V6_AP = 0x0010,		///< AP3000	
    IMP_V6_UT = 0x0020,		///< UT3.0和3.1*/

    /// 导航类
    ISF_V6_ZERO = 0x0700,  ///< 星探rfansv6k
    /// V4代表版本号
    ISF_V4_DEB1 = 0x0711,
    ISF_V4_DEB3 = 0x0715,
    ISF_V4_LEVELX = 0x0719,
    ISF_V4_SHORT = 0x071b,
    ISF_V4_EXTEND_LEVELX = 0x071c,
    ISF_V4_EXTEND_SHORT = 0x071d,
	ISF_V4_PRECISE_XLEVEL = 0x071e,
	ISF_V4_MULTI = 0x071f,
	ISF_V4_ADAPTIVE_MULTI = 0x0721,
}DevProto_E;

/// 解码数据类型
typedef enum DecDataType
{
    FAST_DATA = 0,
    SLOW_DATA = 1,
    FAST_SLOW_DATA = 2,

}DecDataType_E;

/// 运行模式
typedef enum RunModel
{
    RUN_ONLINE = 0, 
    RUN_OFFLINE = 1,
	RUN_PCAP = 2,
}RunModel_E;

/// 初始化imp/isf头方式
typedef enum InitHeaderMethod
{
    RAW_FILE = 0,
    CONFIG_FILE = 1,
}InitHeaderMethod_E;

/// xyz输出类型
typedef enum XYZType
{
	NO_XYZ = 0,			///< 不输出xyz
	XYZ_COMM = 1,		///< 通用xyz
	XYZ_CALIB = 2,		///< 生产标定xyz
	XYZ_CK128 = 3,		///< ck128检测工具xyz，输出指定帧数xyz文件
	XYZ_DOME = 4,		///< DOME检测工具xyz，输出指定帧数xyz文件
}XYZType_E;

/// level类型
typedef enum LevelType
{
    LEVEL_NULL = 0,
    LEVEL_MAX = 1,
    LEVEL_ALL = 2,
}LevelType_E;

typedef enum HexDataType
{
    NO_OUT = 0,
    BEFORE_DEC = 1,
    AFLTER_DEC = 2,
}HexDataType_E;

typedef enum CameraDataType
{
	NO_CAM = 0,
	CAM_TRIGGRR = 1,
	CAM_FEEDBACK = 2,
	CAM_IMAGE = 3,
}CameraDataType_E;

/// 以下全是配置参数的结构体
///解码快数据模块配置结构体
typedef struct DecConfigParam
{
    DevType_E dev_type;			///< 设备类型
    DevProto_E dev_proto;		///< 设备协议
    int16_t device_id;			///< 设备id,测绘设备不填
	float t0_thresdhold;		///< ap3000设备解码时t0判断阈值,需要设置
    DecConfigParam() {
        dev_type = DevType_E::DEV_ISF;
        dev_proto = DevProto_E::PROTO_NULL;
        device_id = -1;
		t0_thresdhold = 5;
    }
}DecConfigParam_S;

typedef enum ImuType
{
	NO_IMU = 0,
	IMU_UIMU = 1,
	IMU_HG4930 = 2,
	IMU_BKTH = 3,
}ImuType_E;

///解码慢数据模块配置结构体
typedef struct DecSlowConfigParam
{
	int16_t device_id;			///< 设备id,测绘设备不填
	ImuType_E imu_type;
	DecSlowConfigParam() {
		device_id = 161;
		imu_type = NO_IMU;
	}
}DecSlowConfigParam_S;

/// 解码过程输出中间数据
typedef struct OutFile
{
    bool out_raw_data;
	bool out_l1_data;
    bool out_proto_data;
    bool out_las;
    bool out_imu;
    bool out_turn;
	bool out_log;		///< log文件是否输出，默认为true输出。一般实时设为false，不输出
	bool out_csv;       ///< 完整性一致性设置开关，当置为true时开启检测，默认为false
    bool out_pps;		///< 输出pps时间对文本
	bool out_gps;
	bool out_ground;	///< 对地高
	bool out_imu_ie;
	bool out_mta;
	bool out_detect_log;///< 输出完整性检测过程中日志
    HexDataType_E out_hex_data;
	XYZType_E out_xyz;
	CameraDataType_E out_camera;
    OutFile() {
        out_raw_data = false;
		out_l1_data = false;
        out_proto_data = false;
        out_las = false;
        out_imu = false;
        out_turn = false;
		out_log = true;
		out_csv = false;
        out_pps = false;
		out_gps = false;
		out_ground = false;
		out_imu_ie = false;
		out_mta = false;
		out_detect_log = false;
        out_hex_data = HexDataType_E::NO_OUT;
		out_xyz = XYZType_E::NO_XYZ;
		out_camera = CameraDataType_E::NO_CAM;
    }
}OutFile_S;

/// 文件模块配置结构体
typedef struct FileConfigParam
{
    DevProto_E dev_proto;		///< 设备协议
    RunModel_E run_model;
    DecDataType_E data_type;	///< 解码数据类型
    InitHeaderMethod_E method;
    /// log设置
    PRIORITYLEVEL_E priority;	///< 输出日志等级
    uint16_t log_num;			///< 日志数量
    uint32_t log_size;			///< 日志大小，单位byte

    OutFile_S out_file;
    /// 设置路径
    std::string raw_path;		///< 原始文件路径
    std::string cfg_path;		///< 配置文件路径
    std::string out_path;		///< 输出文件路径
    std::string pic_path;		///< 照片文件路径
	std::string report_path;    ///< 报告文件路径
	std::string camera_params_path; ///<相机参数文件路径
    FileConfigParam() {
        dev_proto = DevProto_E::PROTO_NULL;
        run_model = RunModel_E::RUN_ONLINE;
        data_type = DecDataType_E::FAST_DATA;
        method = InitHeaderMethod_E::RAW_FILE;
        priority = PRIORITYLEVEL_E::LOG_ERROR;
        log_num = 10;
        log_size = 20 * 1024 * 1024;
        raw_path = "E:/test/multi/ISF";
        cfg_path = "E:/test/multi/POS";
        out_path = "E:/test/multi/LAS";
        pic_path = "E:/test/multi/PIC";
		report_path = "";
		camera_params_path = "";
    }

}FileConfigParam_S;

/// 以下是可选配置参数，只会在部分情况下使用
/// 16进制向量数据配置参数
typedef struct HexConfig
{
    uint32_t row_num;
    uint32_t min_stamp;
    uint32_t max_stamp;
    float min_angle;	///< 范围内最小值
    float max_angle;	///< 范围内最大值
	float angle_step;	///< 角度步长
    uint32_t cur_num;   ///< 当前记录行数
    HexConfig()
    {
        row_num = 100000; ///< 默认10万条数据
        min_stamp = 0;
        max_stamp = 0;
        min_angle = 0.0;
        max_angle = 0.0;
		angle_step = 0.0;
        cur_num = 0;
    }
}HexConfig_S;

/// 输出mlevel协议（CK和dome设备）中rawdata和proto文件过滤条件，需要在配置文件或字符终端配置
typedef struct RawDataProtoConfig
{
	float s_angle_scope[2];				///< 扫描角度范围，可以是[10 30]或者[350 10]，默认[0 360]不做过滤
	float z_angle_scope[2];				///< 天顶角度范围，可以是[10 30]或者[350 10]，默认[0 360]不做过滤
	std::vector<uint8_t> laser_num_vec;	///< 通道号，默认是空，即不做过滤，只有不为空时做过滤
	RawDataProtoConfig()
	{
		s_angle_scope[0] = 0.0;
		s_angle_scope[1] = 360.0;
		z_angle_scope[0] = 0.0;
		z_angle_scope[1] = 360.0;
	}
}RawDataProtoConfig_S;

/// rfansV6K(星探设备)设置过滤异常数据
typedef struct RfansV6KFilter
{
    bool use_flag;       ///< 是否开启过滤，默认是false不开启
    float theory_val;
    float ratio;
    RfansV6KFilter()
    {
        use_flag = false;
        theory_val = 50.0;
        ratio = 0.5;
    }
}RfansV6KFilter_S;

typedef enum DecRangeType
{
    cNone,
    cRise,
    cFall,
    cAll,
}DecRangeType_E;

///解码快数据模块可选结构体
typedef struct DecOptionParam
{
    // 导航类参数
    LevelType_E level_type;
    HexConfig_S hex_cfg;
	RawDataProtoConfig_S raw_proto_cfg;
    RfansV6KFilter_S filter;
    bool bin_fix;               ///< true新版本码密度修正 false老版本码密度修正
    int8_t select_level_num;   ///< 指定level

    // 测绘类参数
    bool use_pps;			///< 测绘设备是否使用pps，有的设备没有pps时间对，默认使用true
    DecRangeType_E rang_type;
	uint8_t scd_ver;
    DecOptionParam() {
        level_type = LevelType_E::LEVEL_MAX;
        bin_fix = true;
        select_level_num = -1;

        use_pps = true;
        rang_type = cRise;
		scd_ver = 3;
    }
}DecOptionParam_S;

typedef enum DomeCoorMode
{
    MODE_A,
    MODE_B,
    MODE_C
}DomeCoorModle_S;

typedef enum DomeAnlignPos
{
	ANLIGN_0,
	ANLIGN_90,
	ANLIGN_180,
	ANLIGN_270,
}DomeAnlignPos_E;

/// 算法模块配置结构体
typedef struct AlgConfigParam
{
	bool Level0A;	///< 距离过滤算法
    bool Level0C;   ///< 角度裁剪算法
    bool Level0G;   ///< 脉冲过滤算法
	bool Level0M;	///< 角度距离过滤算法
	bool Level0N;	///< ROI过滤算法
	bool Level0P;	///< 扫描角度过滤算法
	bool Level0Q;	///< 基于窗口回波过滤
	bool Level0R;	///< 多帧距离中值加权
	bool Level0S;	///< 时间窗口过滤算法
    bool Level1A;   ///< 脉宽修正距离算法
	bool Level1M;   ///< 脉宽修正距离算法 ( 最大level-1 )
	bool Level1N;   ///< 脉宽修正距离算法 ( 弱模式用最大level-1，强模式用xlevel )
    bool Level1B;   ///< 距离修正脉宽算法
    bool Level1F;   ///< 温度补偿算法
    bool Level1G;   ///< 中远距离修正算法
    bool Level1L;   ///< 灰度拉伸
    bool Level1Q;   ///< 阳光噪点过滤
    bool Level2A;   ///< 12bit转8bit：线性拉伸
    bool LevelAB;   ///< 零位角修正算法
    bool LevelAC;   ///< 角度修正算法
    bool LevelAH;   ///< 室外地面抗锯齿算法
    bool LevelAI;   ///< 阳光噪点过滤算法
    bool LevelAJ;   ///< 高反膨胀算法
    bool LevelAM;   ///< 灰度增强算法
    bool LevelAN;   ///< 地下镜面点过滤算法
    DomeCoorModle_S dome_coor_modle;
	DomeAnlignPos_E anlignAngle;

    /// SURV_ALG_OPT_S 
    bool calib_r;           ///< calibR， 距离标定
    bool calib_a;           ///< calibA, 角度标定
    bool calib_i;           ///< calibI, 反射强度标定
    bool smooth_range;      ///< smoothRange, 距离平滑
    bool strech_gray_SWTX;  ///< strechGray_SWTX, 灰度拉伸
    bool mta_calc;          ///< mtaCalc, AP3000 MTA算法
    bool lidar_calc;        ///< lidarCalc, 坐标计算
    bool interp_pps;        ///< interpPps, PPS插值 // 0x21
    bool calib_inclt;       ///< calibInclt, UT电子罗盘+倾角仪

    /// PRE_ALG_OPT_S 预过滤
    bool range_enable;            ///< 距离range过滤
    bool range_intensity_enable;  ///< 反射强度intensity过滤
    ANGLE_FILTER_E angle_enable;  ///< 水平扫描角度scan_angle过滤

    /// POST_FILTER_S 后过滤
    bool heigh_enable;      ///< 高度H过滤
    bool resample_enable;   ///< 采样抽稀过滤
    bool air_wide_enable;   ///< airWide_enable 蜂鸟、蜂蝶航带宽度裁剪（过滤）
    bool angle_view_enable; ///< angleView_enable 蜂鸟、蜂蝶视场角过滤
    bool echoes_enable;     ///< echo 回波过滤
    bool gps_time_enable;   ///< gpsTime_enable gps时间过滤
    bool nanoflann_enable;  ///< 离散点距离过滤
   
    /// GeoAlgOpt 地理坐标计算
    bool interp_traj_enable;  ///< 轨迹插值计算位姿
    bool calc_btd_enable;     ///< 地理坐标

    /// 赋彩色点云
    bool color_points_enable;

    AlgConfigParam()
    {
        Level0G = Level0C = Level0M = Level0N = Level0P = Level0Q = Level0R = Level0S = Level0A = false;
        Level1A = Level1B = Level1F = Level1G = Level1L = Level1M = Level1Q = Level1N = false;
        Level2A = false;
        LevelAB = LevelAC = LevelAI = LevelAH = LevelAJ = LevelAM = false;
        LevelAN = false;
        dome_coor_modle = DomeCoorModle_S::MODE_C;
		anlignAngle = DomeAnlignPos_E::ANLIGN_90;

        calib_r = false;           ///< calibR， 距离标定
        calib_a = false;           ///< calibA, 角度标定
        calib_i = false;           ///< calibI, 反射强度标定
        smooth_range = false;      ///< smoothRange, 距离平滑
        strech_gray_SWTX = false;  ///< strechGray_SWTX, 灰度拉伸
        mta_calc = false;          ///< mtaCalc, AP3000 MTA算法
        lidar_calc = false;        ///< lidarCalc, 坐标计算
        interp_pps = false;        ///< interpPps, PPS插值 // 0x21
        calib_inclt = false;       ///< calibInclt, UT电子罗盘+倾角仪

        /// PRE_ALG_OPT_S
        range_enable = false;            ///< 距离range过滤
        range_intensity_enable = false;  ///< 反射强度intensity过滤
        angle_enable = NONE;  ///< 水平扫描角度scan_angle过滤

        /// POST_FILTER_S 后过滤
        heigh_enable = false;      ///< 高度H过滤
        resample_enable = false;   ///< 采样抽稀过滤
        air_wide_enable = false;   ///< airWide_enable 蜂鸟、蜂蝶航带宽度裁剪（过滤）
        angle_view_enable = false; ///< angleView_enable 蜂鸟、蜂蝶视场角过滤
        echoes_enable = false;     ///< echo 回波过滤
        gps_time_enable = false;   ///< gpsTime_enable gps时间过滤
        nanoflann_enable = false;  ///< 离散点距离过滤

        /// GeoAlgOpt 地理坐标计算
        interp_traj_enable = false;  ///< 轨迹插值计算位姿
        calc_btd_enable = false;     ///< 地理坐标

        color_points_enable = false;   ///< 彩色点云
    }

}AlgConfigParam_S;


/// gps 时间滤波，具体含义待核实
const int GPSTIME_FILTER_NUM = 40;

/// 点滤波算法参数
typedef struct FilterPara
{
    float filter_range_min;        ///< 距离滤波最小值
    float filter_range_max;        ///< 距离滤波最大值
    uint16_t filter_intensity_min; ///< 灰度滤波最小值
    uint16_t filter_intensity_max; ///< 灰度滤波最大值
    float filter_scan_angle_min;   ///< 角度滤波最小值
    float filter_scan_angle_max;   ///< 角度滤波最大值
    double filter_z_value_min;     ///< 高度滤波最小值
    double filter_z_value_max;     ///< 高度滤波最大值
    float angle_viewL;             ///< 航带过滤时，左侧最大角度值，angle_view_enable为true时生效
    float angle_viewR;             ///< 航带过滤时，右侧最大角度值，angle_view_enable为true时生效
    float airline_width;           ///< 航带宽度，air_wide_enable为true时生效

    float filter_gps_time[GPSTIME_FILTER_NUM];  ///< gps time滤波
    uint32_t filter_resample_ratio;    ///< 抽稀ratio
    bool filter_scan_angle_in;                  ///< 标记转台滤波时是否在范围内
    bool filter_echo_flag[ECHO_NUM_CALC];      ///< 回波滤波设置
} FilterPara_S;


/// frame滤波算法参数
typedef struct FrameFilterParas
{
    // sor滤波
    int32_t sor_mean_k;
    float sor_stddev_thresh;

    // radius滤波
    float nano_radius;
    int32_t nano_near_point_num;

    FrameFilterParas()
        : sor_mean_k(3)
        , sor_stddev_thresh(60)
        , nano_radius(0.5)
    {

    }
} FrameFilterParas_S;

/// 不同旋转方向，SDK3.0中 calculate.h中
typedef enum RotateClock
{
    CLOCK = 0,     //顺时帧
    UNTI_CLOCK = 1,  //逆时针
} RotateClock_E;

/// mta算法配置参数
typedef struct MtaConfig
{
	float correct_value;  //校正值
	float rng_value;
	float filter_ele_line;
	float power_rng_value;
	//电力线翻转
	bool grid_enable;
	uint16_t grid_point_num;
	uint16_t grid_ratio;
	float grid_diff_range;
	bool trans_mta;  //默认false，当做mta翻转时置为true
	uint16_t frame_num;
	MtaConfig()
	{
		correct_value = 10.0;
		rng_value = 30.0;
		filter_ele_line = 60.0;
		power_rng_value = 30;

		grid_enable = true;
		grid_point_num = 10;
		grid_ratio = 3;
		grid_diff_range = 30.0;
		trans_mta = false;
		frame_num = 6;
	}
}MtaConfig_S;

/// Lidar相对惯导系统的安装参数；lidar与惯导集成后，该参数写入配置文件；
typedef struct LidarInsParas
{
    double lever_arm[3];    ///< 杆臂值
    double angle_rph[3];    ///< 角度

    double	freq_delt_angle[3];
    double	freq_delt_xyz[3];

    double	delt_angle[3];
    double	delt_xyz[3];

    LidarInsParas()
    {
        memset(lever_arm, 0, sizeof(double) * 3);
        memset(angle_rph, 0, sizeof(double) * 3);
       
        memset(freq_delt_angle, 0, sizeof(double) * 3);
        memset(freq_delt_xyz, 0, sizeof(double) * 3);

        memset(delt_angle, 0, sizeof(double) * 3);
        memset(delt_xyz, 0, sizeof(double) * 3);
    }
} LidarInsParas_S;

/// 算法模块配置结构体
typedef struct AlgOptionParam
{
	bool production_calibration;    ///< 生产检校标记
    char pos_file_name[255];         ///< 轨迹文件
	uint8_t output_frame_type;		 ///< 设置输出帧类型，4是数据帧，1是扫描帧，默认是4
    FilterPara_S filter_paras;       ///< 点滤波参数
    FrameFilterParas_S frame_filter_paras;  ///< 帧滤波参数
    RotateClock_E rotate_clock;    ///< 标记电机旋转方向
    GEO_PROJ_S geo_proj_paras;     ///< 投影参数
    LidarInsParas lidar_ins_paras;  ///< lidar与惯导之间的安装参数
	MtaConfig_S mta_cfg;

    AlgOptionParam()
	{
		production_calibration = false;
		output_frame_type = 4;
        rotate_clock = UNTI_CLOCK;
        memset(pos_file_name, 0, 255 * sizeof(char));
    }

}AlgOptionParam_S;

/// 检测项
typedef enum DetectItem
{
	NO_DETECT,			 ///< 没有检测
	FAST_INTE_CONSIST,   ///< 快数据完整性一致性
	SLOW_INTE_CONSIST,   ///< 慢数据完整性一致性
	MIN_RANGE,			 ///< 最小测距
	FAST_SLOW_INTE_CONSIST,///< 快慢数据完整性一致性
}DetectItem_E;

/// 检测配置参数
typedef struct DetectConfigParam
{
	int32_t point_num;			///< 用来做检测的点数量
	int32_t set_speed;			///< 设置转速
	int32_t set_zenith_speed;	///< 设置dome天顶角转速
	int16_t device_id;			///< 设备id
	DetectItem_E detect_item;	///< 设置检测项
	std::string cfg_path;		///< 配置文件路径
	DetectConfigParam()
	{
		point_num = 100000;
		set_speed = 10;
		set_zenith_speed = 70;
		device_id = 161;
		detect_item = DetectItem_E::NO_DETECT;
		cfg_path = "E:/test/multi";
	}

}DetectConfigParam_S;

/// 配置每个检测项的参数
typedef struct DetectItemsParam
{
	//快数据检测配置
	int32_t frames_num;		///< 快数据检测设置检测帧数
	//FOV视场连续性检测
	float fov_azimuth_step; ///< 方位角间隔
	float fov_pitch_step;	///< 俯仰角间隔
	double fov_azimuth_min;
	double fov_pitch_min;
	int pitch_angle_left;
	int pitch_angle_right;
	//距离连续性检测
	float max_range;		///< 设置最大距离
	float min_range;		///< 设置最小距离
	float range_step;		///< 距离间隔
	//最远距离检测
	float max_azimuth;
	float min_azimuth;
	float max_pitch;
	float min_pitch;
	float begin_range;		///< 设置最小距离
	float end_range;		///< 设置最大距离
	int32_t max_inten;		///< 设置最大灰度
	int32_t min_inten;		///< 设置最小灰度

	DetectItemsParam()
	{
		frames_num = 5;
		fov_azimuth_step = 60;
		fov_pitch_step = 10;
		max_range = 200;
		min_range = 0;
		range_step = 10;
		max_azimuth = 30;
		min_azimuth = 0;
		max_pitch = 30;
		min_pitch = 0;
		begin_range = 0;
		end_range = 50;
		max_inten = 100;
		min_inten = 0;
		fov_azimuth_min = 1;
		fov_pitch_min = 1;
		pitch_angle_left = -7;
		pitch_angle_right = 53;
	}

}DetectItemsParam_S;

enum DecType
{
	eStaticType,         ///< 静态解算，用于回放isf数据
	eDynamicType,        ///< 动态解算，用于实时解算isf数据
	eDynamic_detect_Type,///< 动态解算，用于实时解算isf数据
};

enum DecParam  ///< 解算方式
{
	eSlow,     ///< 解算慢数据
	eFast,     ///< 解算快数据
	eDefault,  ///< 按配置文件配置的解算方式
};

enum DataSuffixType
{
	eSuffix_ISF,
	eSuffix_PCAP,
	eSuffix_IMP,
};
typedef struct DynamicDecParam
{
	std::string devIp;
	std::string localIp;
	std::string savePath;
	bool isSaveISF;
	int laserFreqKHz;
	int laserFirePwr;
	float scanAngleSpeed;
	DynamicDecParam()
	{
		devIp = "";
		localIp = "";
		savePath = "./";
		isSaveISF = false;
		laserFreqKHz = 1000;
		laserFirePwr = 100;
		scanAngleSpeed = 40.0;
	}
}DynamicDecParam_S;

typedef struct DetectConfig
{
	DetectConfigParam_S detectParam;
	DetectItemsParam_S  detectItemsParam;
	int detectTime;
	DetectConfig()
	{
		detectTime = 2;
	}
}DetectConfig_S;
//sdk初始化函数参数结构体
typedef struct ConfigParam
{
	FileConfigParam_S fileConfig;
	DecConfigParam_S dec_config_param;
	DecOptionParam_S dec_option_param;
	AlgConfigParam_S calc_config_param;
	AlgOptionParam_S calc_option_param;
	DetectConfig_S   detect_config_param;
	DynamicDecParam_S  dynanic_dec_param;
	ImuType imuType;
	DataSuffixType dataType;
	std::string lidar_config_file_name;//配置文件名
	ConfigParam()
	{
		lidar_config_file_name = "revise.ini";
	}
}ConfigParam_S;

#endif // !__LIDAR_COMM_H__