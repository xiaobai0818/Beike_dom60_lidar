#ifndef __I_LIDAR_INFO_H__
#define __I_LIDAR_INFO_H__
#include <vector>
#if defined __linux__
#include <stdint.h>
#include <iostream>
#include <string.h>
#endif
const uint8_t ECHO_NUM_CALC_ = 4;
#pragma pack(1)

/// @brief L1数据 回波结构体（笛卡尔坐标）
typedef struct L1CellXYZI {
    double X, Y, Z;             ///< 直角坐标 unit: m
    float range;                ///< 距离 unit: m
    float pulsewide_sum;        ///< 脉宽和
    uint32_t intensity;         ///< 灰度
    uint8_t cur_level : 3;      ///< 当前level
    uint8_t max_level : 3;      ///< 最大level
    uint8_t flag : 1;           ///< 激光点有效标志位
    uint8_t ground_flag : 1;    ///< 地面点标识 0:非地面点 1:地面点
	uint8_t echo_match : 3;		///< 置信度
	uint8_t reserved : 5;		///< 保留bit
    L1CellXYZI() {
        X = Y = Z = 0.;
        range = 0.;
        pulsewide_sum = 0.0f;
        intensity = 0;
        cur_level = 0;
        max_level = 0;
        flag = 0;
        ground_flag = 0;
		echo_match = 0;
		reserved = 0;
    }
} L1CellXYZI_S;

/// @brief L1数据 点结构体（笛卡尔坐标）
typedef struct L1XYZShots {
    uint64_t utc_ntp_time;		///< utc、unix 100MHz time 单位ns
    float angle;				///< 水平角度
    float zenith_angle;			///< 天顶角度
    uint8_t laser_num;          ///< 通道号0~31
    uint8_t mirror_num : 2;     ///< 镜面编号
    uint8_t echo_num : 4;       ///< 回波数量
    uint8_t time_type : 2;      ///< 时间类型：utc：0、unix:1 100MHz:2
    L1CellXYZI_S xyz_cell[ECHO_NUM_CALC_];
    L1XYZShots() {
        angle = 0.;
        zenith_angle = 0.;
        utc_ntp_time = 0;
        laser_num = 0;
        mirror_num = 0;
        echo_num = 0;
        time_type = 0;
    }
}L1XYZShots_S;

/// @brief L1数据 扫描帧结构体
typedef struct L1FrameEcho {
    uint16_t mirror_num;        ///< 镜面编号，用于区分扫描帧
    uint16_t mat_row;           ///< 二维矩阵的行数
    uint32_t mat_column;        ///< 二维矩阵的列数
    L1XYZShots_S** point_matrix;///< 二维矩阵指针
    L1FrameEcho() {
        mirror_num = 0;
        mat_row = 0;
        mat_column = 0;
        point_matrix = nullptr;
    }
}L1FrameEcho_S;

/// @brief L1 数据帧结构体
typedef struct L1DataFrame {              
    uint8_t save_num;               ///< 已存储扫描帧数量
    L1FrameEcho_S* scan_frame[4];    ///< 扫描帧指针存储数组
    L1DataFrame()
    {
        save_num = 0;
    }
}L1DataFrame_S;

typedef struct L1IMU {
	uint64_t clock_cnt;             ///< 时间戳，单位ns
	double  second;
	float  accel_x;
	float  accel_y;
	float  accel_z;
	float  gyro_x;
	float  gyro_y;
	float  gyro_z;
	float  roll;
	float  pitch;
	float  yaw;
	float mag_x;
	float mag_y;
	float mag_z;
	L1IMU()
	{
		clock_cnt = 0;
		second = 0;
		accel_x = 0;
		accel_y = 0;
		accel_z = 0;
		gyro_x = 0;
		gyro_y = 0;
		gyro_z = 0;
		roll = 0;
		pitch = 0;
		yaw = 0;
		mag_x = 0;
		mag_y = 0;
		mag_z = 0;
	}
}L1IMU_S;

typedef struct TurnData
{
	uint64_t clock_cnt;
	double utc_time;
	float angle;
	TurnData()
	{
		clock_cnt = 0;
		utc_time = 0;
		angle = 0;
	}
}TurnData_S;
#pragma pack()


// @brief 帧处理回调函数类型
typedef int32_t(*FrameCallback)(L1DataFrame_S&);
typedef std::vector<L1IMU_S> L1IMUVEC;
typedef std::vector<TurnData_S> L1TURNVEC;

#endif // !__I_LIDAR_INFO_H__