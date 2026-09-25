/**
 * @file common.h
 * @author yuansheng
 * @brief
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <iostream>
#include <string>
#include "ringbuffer.hpp"

/**
 * @brief 数据来源
 *
 */
enum InputType
{
  NET_UDP = 0,
  ISF_FILE = 1,
  PCAP_FILE = 2,
};

/**
 * @brief 参数配置
 *
 */
struct InputPara
{
  /// @brief 设备名称
  std::string device_name;
  /// @brief 设备 IP
  std::string device_ip;
  /// @brief 数据端口
  int32_t data_port;
  /// @brief 数据端口
  int32_t imu_port;
  /// @brief 心跳端口
  int32_t heart_port;
  /// @brief isf_header端口
  int32_t isf_header_port;
  /// @brief 扫描电机转速
  float scan_speed;
  /// @brief 天顶电机转速
  float zenith_speed;
  /// @brief 时间同步标志位
  bool time_sync;
  /// @brief 激光器点频
  int32_t freq_khz;
  /// @brief 激光器功率
  int32_t fire_pwr;

  /// @brief 回放文件路径
  std::string isf_path;

  /// @brief PCAP文件路径
  std::string pcap_path;

  /// @brief 配置文件路径
  std::string cfg_path;

  /// @brief 是否使用外参变换
  bool extrinsin_enable;
  /// @brief 绕z轴旋转
  double extrinsic_yaw;
  /// @brief 绕y轴旋转
  double extrinsic_pitch;
  /// @brief 绕x轴旋转
  double extrinsic_roll;
  /// @brief x方向平移
  double extrinsic_tx;
  /// @brief y方向平移
  double extrinsic_ty;
  /// @brief z方向平移
  double extrinsic_tz;

  /// @brief 点云消息帧ID
  std::string frame_id;

  /// @brief 点云话题
  std::string pointcloud_topic;

  /// @brief IMU话题
  std::string imu_topic;

  /// @brief xyz文件保存标记
  bool save_xyz;
  /// @brief xyz文件保存路径
  std::string xyz_path;

  /// @brief ISF文件保存标记
  bool save_isf;
  /// @brief ISF文件保存路径
  std::string save_isf_path;

  /// @brief 数据协议类型
  int32_t data_proto;

  /// @brief 算法使用标记
  bool algorithm_flag;

  /// @brief 距离过滤算法
  bool Level0A;
  /// @brief 角度裁剪算法
  bool Level0C;
  /// @brief 脉冲过滤算法
  bool Level0G;
  /// @brief 角度距离过滤算法
  bool Level0M;
  /// @brief ROI过滤算法
  bool Level0N;
  /// @brief 扫描角度过滤算法
  bool Level0P;
  /// @brief 基于窗口回波过滤
  bool Level0Q;
  /// @brief 时间窗口过滤
  bool Level0S;

  /// @brief 脉宽修正算法（距离）
  bool Level1A;
  /// @brief 距离修正算法（脉宽）
  bool Level1B;
  /// @brief 距离修正算法（温度）
  bool Level1F;
  /// @brief 距离修正算法（中远距离）
  bool Level1G;
  /// @brief 灰度拉伸(12bit)
  bool Level1L;
  /// @brief 脉宽修正算法（距离）
  bool Level1M;
  /// @brief 脉宽修正距离算法 ( 弱模式用最大level-1，强模式用xlevel )
  bool Level1N;
  /// @brief 阳光噪点过滤
  bool Level1Q;
  /// @brief 12bit转8bit：线性拉伸
  bool Level2A;

  /// @brief 零位角修正算法
  bool LevelAB;
  /// @brief 角度修正算法
  bool LevelAC;
  /// @brief 室外锯齿平滑算法
  bool LevelAH;
  /// @brief 拉丝处理算法
  bool LevelAI;
  /// @brief 高反膨胀算法
  bool LevelAJ;
  /// @brief 灰度增强算法
  bool LevelAM;
  /// @brief 地下镜面点过滤算法
  bool LevelAN;

  /// @brief 解算模式
  int32_t dome_mode_type;

  InputPara()
  {
    device_name = "";
    device_ip = "";
    data_port = 0;
    imu_port = 0;
    heart_port = 0;
    isf_header_port = 0;
    scan_speed = 0.;
    zenith_speed = 0.;

    isf_path = "";
    cfg_path = "";

    extrinsin_enable = false;
    extrinsic_yaw = 0.;
    extrinsic_pitch = 0.;
    extrinsic_roll = 0.;
    extrinsic_tx = 0.;
    extrinsic_ty = 0.;
    extrinsic_tz = 0.;

    frame_id = "world";
    pointcloud_topic = "points_raw";
    imu_topic = "imu_raw";

    save_xyz = false;
    xyz_path = "";

    save_isf = false;
    save_isf_path = "";

    pcap_path = "";

    data_proto = 0;

    algorithm_flag = false;
    Level0A = false;
    Level0C = false;
    Level0G = false;
    Level0M = false;
    Level0N = false;
    Level0P = false;
    Level0Q = false;
    Level0S = false;
    
    Level1A = false;
    Level1B = false;
    Level1F = false;
    Level1G = false;
    Level1L = false;
    Level1M = false;
    Level1N = false;
    Level1Q = false;
    Level2A = false;

    LevelAB = false;
    LevelAC = false;
    LevelAH = false;
    LevelAI = false;
    LevelAJ = false;
    LevelAM = false;
    LevelAN = false;
    
    dome_mode_type = 2;
  }
};

struct Buffer
{
  ringbuffer *fastr_buf;
  ringbuffer *slowr_buf;
  ringbuffer *heartr_buf;
  ringbuffer *isf_header_buf;
  Buffer()
  {
    fastr_buf = nullptr;
    slowr_buf = nullptr;
    heartr_buf = nullptr;
    isf_header_buf = nullptr;
  }
};
