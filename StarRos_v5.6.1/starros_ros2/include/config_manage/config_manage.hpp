/**
 * @file ConfigManage.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <yaml-cpp/yaml.h>
#include "common.h"

class ConfigManage
{
private:
  /// @brief yaml参数节点
  YAML::Node m_yaml;

public:

  /// @brief 配置参数
  InputPara m_input_para;

  /**
   * @brief Construct a new Config Manage object
   * 
   */
  ConfigManage();

  /**
   * @brief Destroy the Config Manage object
   * 
   */
  ~ConfigManage();

  /**
   * @brief yaml文件读取函数
   * 
   * @param config_path 配置文件地址
   * @return int32_t 
   */
  inline int32_t open(const std::string& config_path)
  {
    try
    {
      m_yaml = YAML::LoadFile(config_path);

      return 0;
    }
    catch (...)
    {
      std::cout << "config yaml path is error" << std::endl;
      std::cout << config_path << std::endl;

      return -1;
    }
  }

  /**
   * @brief 单参数读取函数
   * 
   * @tparam T 参数类型
   * @param key 字段
   * @param param 参数
   * @param default_param 默认值
   * @return true 
   * @return false 
   */
  template <typename T>
  inline bool getParam(const std::string& key, T& param, const T& default_param)
  {
    if (!m_yaml[key] || m_yaml[key].Type() == YAML::NodeType::Null)
    {
      param = default_param;
      return false;
    }
    else
    {
      param = m_yaml[key].as<T>();
      return true;
    }
  }

  /**
   * @brief yaml参数打印函数，打印所有变量
   * 
   */
  void printf()
  {
    if(!m_yaml)
    {
      std::cout << "config yaml is null" << std::endl;
      return;
    }

    for(YAML::const_iterator it = m_yaml.begin(); it != m_yaml.end(); it++)
    {
      std::cout << it->first.as<std::string>() << " is " << it->second.as<std::string>() << std::endl;
    }
  };

  /**
   * @brief 配置参数赋值
   * 
   */
  void setupNodeParams();
};

ConfigManage::ConfigManage()
{
}

ConfigManage::~ConfigManage()
{
}


void ConfigManage::setupNodeParams()
{
  getParam<std::string>("model",            m_input_para.device_name,       "CK-128");

  getParam<std::string>("frame_id",         m_input_para.frame_id,          "world");
  getParam<std::string>("pointcloud_topic", m_input_para.pointcloud_topic,  "points_raw");
  getParam<std::string>("imu_topic",        m_input_para.imu_topic,         "imu_raw");

  getParam<std::string>("device_ip",        m_input_para.device_ip,         "192.168.0.10");
  getParam<int>("data_port",                m_input_para.data_port,         0);
  getParam<int>("imu_port",                 m_input_para.imu_port,          0);
  getParam<int>("heart_port",               m_input_para.heart_port,        0);
  getParam<int>("isf_header_port",          m_input_para.isf_header_port,   11000);
  getParam<float>("scan_speed",             m_input_para.scan_speed,        0);
  getParam<float>("zenith_speed",           m_input_para.zenith_speed,      70);
  getParam<bool>("time_sync",               m_input_para.time_sync,         true);

  getParam<int>("freq_khz",                 m_input_para.freq_khz,          350);
  getParam<int>("fire_pwr",                 m_input_para.fire_pwr,          100);

  getParam<std::string>("pcap_path",        m_input_para.pcap_path,         "");
  getParam<std::string>("isf_path",         m_input_para.isf_path,          "");

  getParam<int>("data_proto",               m_input_para.data_proto,        0);
  getParam<std::string>("cfg_path",         m_input_para.cfg_path,          "");

  getParam<bool>("extrinsin_enable",        m_input_para.extrinsin_enable,  false);
  getParam<double>("extrinsic_yaw",         m_input_para.extrinsic_yaw,     0.);
  getParam<double>("extrinsic_pitch",       m_input_para.extrinsic_pitch,   0.);
  getParam<double>("extrinsic_roll",        m_input_para.extrinsic_roll,    0.);
  getParam<double>("extrinsic_tx",          m_input_para.extrinsic_tx,      0.);
  getParam<double>("extrinsic_ty",          m_input_para.extrinsic_ty,      0.);
  getParam<double>("extrinsic_tz",          m_input_para.extrinsic_tz,      0.);

  getParam<bool>("save_xyz",                m_input_para.save_xyz,          false);
  getParam<std::string>("xyz_path",         m_input_para.xyz_path,          "");

  getParam<bool>("save_isf",                m_input_para.save_isf,          false);
  getParam<std::string>("save_isf_path",    m_input_para.save_isf_path,     "");

  getParam<bool>("use_algorithm",           m_input_para.algorithm_flag,    false);

  if(m_input_para.algorithm_flag)
  {
    getParam<bool>("Level0A",               m_input_para.Level0A,           false);
    getParam<bool>("Level0C",               m_input_para.Level0C,           false);
    getParam<bool>("Level0G",               m_input_para.Level0G,           false);
    getParam<bool>("Level0M",               m_input_para.Level0M,           false);
    getParam<bool>("Level0N",               m_input_para.Level0N,           false);
    getParam<bool>("Level0P",               m_input_para.Level0P,           false);
    getParam<bool>("Level0Q",               m_input_para.Level0Q,           false);
    getParam<bool>("Level0S",               m_input_para.Level0S,           false);

    getParam<bool>("Level1A",               m_input_para.Level1A,           false);
    getParam<bool>("Level1B",               m_input_para.Level1B,           false);
    getParam<bool>("Level1F",               m_input_para.Level1F,           false);
    getParam<bool>("Level1G",               m_input_para.Level1G,           false);
    getParam<bool>("Level1L",               m_input_para.Level1L,           false);
    getParam<bool>("Level1M",               m_input_para.Level1M,           false);
    getParam<bool>("Level1N",               m_input_para.Level1N,           false);
    getParam<bool>("Level1Q",               m_input_para.Level1Q,           false);
    getParam<bool>("Level2A",               m_input_para.Level2A,           false);

    getParam<bool>("LevelAB",               m_input_para.LevelAB,           false);
    getParam<bool>("LevelAC",               m_input_para.LevelAC,           false);
    getParam<bool>("LevelAH",               m_input_para.LevelAH,           false);
    getParam<bool>("LevelAI",               m_input_para.LevelAI,           false);
    getParam<bool>("LevelAJ",               m_input_para.LevelAJ,           false);
    getParam<bool>("LevelAM",               m_input_para.LevelAM,           false);
    getParam<bool>("LevelAN",               m_input_para.LevelAN,           false);
  }

  getParam<int32_t>("mode_type",            m_input_para.dome_mode_type,    2);

}           
