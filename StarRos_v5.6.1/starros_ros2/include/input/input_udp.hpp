/**
 * @file input_udp.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string>
#include <unistd.h>
#include "input.hpp"

#include "sdk_single_instance.h"
#include "publish.hpp"

class InputUDP : public Input
{
public:

  /**
   * @brief Construct a new Input UDP object
   * 
   */
  InputUDP()
  {
  };

  /**
   * @brief Construct a new Input UDP object
   * 
   * @param inputparam 
   */
  InputUDP(InputPara& inputparam, Buffer& buffer) : Input(inputparam, buffer)
  {
    init(inputparam);
  };

  /**
   * @brief Destroy the Input UDP object
   * 
   */
  ~InputUDP()
  {
    if(m_device_control)
    {
      m_device_control->stopWork();
    }
  }

  /**
   * @brief UDP输入初始化函数
   * 
   * @param inputparam 
   * @return int32_t 
   */
  int32_t init(InputPara& inputparam);

  /**
   * @brief 输入线程启动函数
   * 
   * @return int32_t 
   */
  int32_t start();

  /**
   * @brief 输入线程实际运行函数(快数据)
   * 
   * @return int32_t 
   */
  int32_t receiveFastSocket();

  /**
   * @brief 输入线程实际运行函数(慢数据)
   * 
   * @return int32_t 
   */
  int32_t receiveSlowSocket();

private:

  /// @brief 设备控制类指针
  DeviceControl_I* m_device_control;
};

int32_t InputUDP::init(InputPara& inputparam)
{
  if(m_init_flag)
  {
    return 0;
  }
  m_init_flag = true;
  
  int size = inputparam.device_ip.find_last_of(".");
  std::string template_ip = inputparam.device_ip.substr(0, size + 1);
  m_device_control = getDeviceControlInstance();
  //todo 获取本机IP传入init
  std::string local_ip;
  struct ifaddrs* ifAddr = nullptr;
  int rv = getifaddrs(&ifAddr);
  for (struct ifaddrs* ifa = ifAddr; ifa != nullptr; ifa = ifa->ifa_next)
  {
      if (ifa->ifa_flags & (IFF_LOOPBACK))
      {
        continue;
      }
      if (ifa->ifa_addr->sa_family == AF_INET)
      {
        char ip[INET_ADDRSTRLEN] = {0};
        struct sockaddr_in *in = (struct sockaddr_in *)ifa->ifa_addr;
        inet_ntop(AF_INET,&in->sin_addr, ip, INET_ADDRSTRLEN);
        std::string str_ip = ip;
        if (str_ip.find(template_ip) != std::string::npos)
        {
          local_ip = str_ip;
          std::cout << "starros log : get local ip : " << local_ip << std::endl;
        }
      }
  }
  if (local_ip.empty())
  {
    std::cout<< "starros log : connect device failed,please check whether device and host are in the same network segment !!!" << std::endl;
    return -1;
  }

  m_device_control->init(inputparam.device_ip.c_str(), local_ip.c_str(), inputparam.data_port, inputparam.imu_port, inputparam.heart_port, g_msgPort, inputparam.isf_header_port);
  
  return 0;
}

int32_t InputUDP::start()
{
  if(m_start_flag)
  {
    return 0;
  }

  m_start_flag = true;

  if(m_device_control)
  {
    while (true)
    {
      unsigned char deviceId = m_device_control->getDeviceId();
      if (0 != deviceId)
      {
        printf("Device ID is %d\n", deviceId);
        break;
      }
      usleep(1000);
    }

    int set_cfg_moudle = 1; //默认值是1. 1 代表使用设备内部xml配置， 0 代表使用手动配置参数
    //获取设备设置的转速
    if ("Dome" == m_input_param.device_name)
    {
      float speed = 0;
      int rv = m_device_control->QueryScanSpeed(speed);
      if (rv != 0 || speed <= 0)
      {
        std::cout << "starros log: query scan_speed failed from device,maybe need update device, rv = " << rv << " scan_speed = " << speed <<std::endl;
        return -1;
      }
      else
      {
        std::cout << "starros log: query scan_speed success from device, scan_speed = " << speed <<std::endl;
      }
      Publish::getInstance()->setScanSpeed(speed);
      if (set_cfg_moudle == 0)
      {
        m_input_param.scan_speed = 61.7;
        m_input_param.zenith_speed = 70;
        m_input_param.freq_khz = 350;
        m_input_param.fire_pwr = 100;
      }
    }
    
    SCDCMD_PROGRM_S cmd;
    cmd.cmdID = eDevCmdWork;
    cmd.echo_etype = 0x8;
    cmd.laserFreqKHz = m_input_param.freq_khz;
    cmd.laserFirePwr = m_input_param.fire_pwr;
    cmd.scanAngleSpeed = m_input_param.scan_speed;
    cmd.scanAngleSpeed1 = m_input_param.zenith_speed;
    cmd.scanAnglePhase = 0.;
    cmd.scanAnglePhase1 = 0.;
    cmd.prog_emode = set_cfg_moudle;

    m_device_control->startWork(cmd);

    m_fast_data_receive_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&InputUDP::receiveFastSocket, this)));

    m_slow_data_receive_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&InputUDP::receiveSlowSocket, this)));
  }

  return 0;
}

int32_t InputUDP::receiveFastSocket()
{
  if(m_device_control)
  {
    m_device_control->recvFastData();
  }
  return 0;
}

int32_t InputUDP::receiveSlowSocket()
{
  if(m_device_control)
  {
    m_device_control->recvSlowData();
  }
  return 0;
}