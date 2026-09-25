#ifndef __LIDAR_ERROR_CODE_H__
#define __LIDAR_ERROR_CODE_H__

#include <iostream>
#include <map>

/// 错误码组成：LIDAR_ +（模块名/类名）+ _ + 错误说明
/// 成功
const int32_t LIDAR_SUCCESS = 0;
const int32_t LIDAR_POINTER_IS_NULL = 0x0001;

/// file 模块
const int32_t LIDAR_FILE_INIT_LOG_FAIL = 0x1000;
const int32_t LIDAR_FILE_READ_DETECT_XML_CFG = 0x1100;

/// decoder 模块
const int32_t LIDAR_DECODER_NOT_FIND_FAST_PACKET = 0x2000;
const int32_t LIDAR_DECODER_FIND_PART_FAST_PACKET = 0x2001;
const int32_t LIDAR_DECODER_PPS_UTC_ERROR = 0x2002;
const int32_t LIDAR_DECODER_PPS_INVALID = 0x2003;
const int32_t LIDAR_DECODER_ECHO_NUM_OVER = 0x2004;
const int32_t LIDAR_DECODER_IMP_HEADER_ERR = 0x2005;
const int32_t LIDAR_DECODER_UT_TURN_ANGLE_ERR = 0x2006;
const int32_t LIDAR_DECODER_POINT_ANGLE_OVER_360 = 0x2007;
const int32_t LIDAR_DECODER_LASER_NUM_OVER = 0x2008;//激光器数超过最大值

/// framemanage 模块
const int32_t LIDAR_FRAME_INVALID_POINT = 0x3000;

/// detect 模块
const int32_t LIDAR_DETECT_POINT_NUM_NOT_ENOUGH = 0x4000;


const int32_t SDK_ERROR_CODE_NODEVICEID  = 0x5001;//获取设备id失败
const int32_t SDK_ERROR_CODE_CREATEDECFAILED   = 0x5002;//创建解码类失败
const int32_t SDK_ERROR_CODE_CREATEFRAMEMANAGE = 0x5003;//创建帧管理类失败
const int32_t SDK_ERROR_CODE_PROTONOTMATCH = 0x5004;//输入协议与实际协议不一致
const int32_t SDK_ERROR_CODE_LOSTPACKET = 0x5005;//丢包
const int32_t SDK_ERROR_CODE_AZIMUTHANGLEBEYOND = 0x5007;//方位角越界
const int32_t SDK_ERROR_CODE_ZENITHANGLEBEYOND = 0x5008;//天顶角越界
const int32_t SDK_ERROR_CODE_ECHONUMBEYOND = 0x5009;//回波数越界
const int32_t SDK_ERROR_CODE_ANGLE_ERROR = 0x5010;//扫描角度越界
const int32_t SDK_ERROR_CODE_TIMESTAMPERROR = 0x5011;//时间戳错误
const int32_t SDK_ERROR_CODE_DATALENERROR = 0x5012;//数据长度错误
const int32_t SDK_ERROR_CODE_CHECKFAILED = 0x5013;//校验失败
const int32_t SDK_ERROR_CODE_DETECTPOINTNOTENOUGH = 0x5014;//检测点数不够
const int32_t SDK_ERROR_CODE_DETECTDATAEMPTY = 0x5015;//检测数据为空
const int32_t SDK_ERROR_CODE_IMUERROR = 0x5016;//imu错误
const int32_t SDK_ERROR_CODE_IMUDATAEMPTY = 0x5017;//imu数据为空
const int32_t SDK_ERROR_CODE_RANGEERROR = 0x5018;//距离错误
const int32_t SDK_ERROR_CODE_PPS_ERROR = 0x5019;//pps错误
const int32_t SDK_ERROR_CODE_TURNDATA_EMPTY = 0x5020;//转台数据为空
const int32_t SDK_ERROR_CODE_TRACKFILE_NOTFOUND = 0x5021;//没用找到轨迹文件
const int32_t SDK_ERROR_CODE_ONLY_HAS_ECHO_0 = 0x5022;//都是0回波数据
const int32_t SDK_ERROR_CODE_CURRENT_POINT_ERROR = 0x5023;//当前点超出帧数据范围
const int32_t SDK_ERROR_CODE_POINT_COUNT_ERROR = 0x5024;//当前输出文件与对比文件点个数不一致
const int32_t SDK_ERROR_CODE_OUTFILE_COL_COUNT_ERROR = 0x5025;//当前输出文件与对比文件属性字段个数不一致
const int32_t SDK_ERROR_CODE_OUTFILE_RESULT_ERROR = 0x5026;//当前输出文件与对比文件结果不一致
const int32_t SDK_ERROR_CODE_OUTFILE_INTENSITY_ERROR = 0x5027;//当前输出文件中灰度值越界

const std::map<int32_t, std::string> g_error_map = {
{LIDAR_SUCCESS, "no error"},
{SDK_ERROR_CODE_NODEVICEID, "no device id"},
{SDK_ERROR_CODE_CREATEDECFAILED, "create dec failed"},
{SDK_ERROR_CODE_CREATEFRAMEMANAGE, "create framemanage failed"},
{SDK_ERROR_CODE_PROTONOTMATCH, "proto not match"},
{SDK_ERROR_CODE_LOSTPACKET, "lost packet"},
{LIDAR_DECODER_LASER_NUM_OVER, "laser number over"},
{SDK_ERROR_CODE_AZIMUTHANGLEBEYOND, "azimuth angle beyond"},
{SDK_ERROR_CODE_ZENITHANGLEBEYOND, "zenith angle beyond"},
{SDK_ERROR_CODE_ECHONUMBEYOND, "echo number beyond"},
{SDK_ERROR_CODE_ANGLE_ERROR, "angle error"},
{SDK_ERROR_CODE_TIMESTAMPERROR, "time stamp error"},
{SDK_ERROR_CODE_DATALENERROR, "data len is error"},
{SDK_ERROR_CODE_CHECKFAILED, "check failed"},
{SDK_ERROR_CODE_DETECTPOINTNOTENOUGH, "detect point not enough"},
{SDK_ERROR_CODE_DETECTDATAEMPTY, "detect data is empty"},
{SDK_ERROR_CODE_IMUERROR, "imu error"},
{SDK_ERROR_CODE_IMUDATAEMPTY, "imu data is empty"},
{SDK_ERROR_CODE_RANGEERROR, "range error"},
{SDK_ERROR_CODE_PPS_ERROR, "pps error"},
{SDK_ERROR_CODE_TURNDATA_EMPTY, "turn data is empty"},
{SDK_ERROR_CODE_TRACKFILE_NOTFOUND, "track file not found"},
{SDK_ERROR_CODE_ONLY_HAS_ECHO_0, "only has 0 echo"},
{SDK_ERROR_CODE_CURRENT_POINT_ERROR, "current point beyond frame count"},
{LIDAR_DECODER_IMP_HEADER_ERR, "set imp header failed"},
{SDK_ERROR_CODE_POINT_COUNT_ERROR, "point count different"},
{SDK_ERROR_CODE_OUTFILE_COL_COUNT_ERROR, "out file col count different"},
{SDK_ERROR_CODE_OUTFILE_RESULT_ERROR, "out file result different to reference file"},
{SDK_ERROR_CODE_OUTFILE_INTENSITY_ERROR, "out file intensity error"}
};
#endif // !__LIDAR_ERROR_CODE_H__

