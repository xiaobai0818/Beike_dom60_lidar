/**
 * @file driver.cpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "common.h"
#include "config_manage.hpp"
#include "input_create.hpp"
#include "calc.hpp"
#include "publish.hpp"

int main(int argc, char **argv)
{
#ifdef ROS1
  ros::init(argc, argv, "surestar_driver_node");
  Publish* publish = Publish::getInstance();
  ros::NodeHandle config_read_nh("~");
  std::string config_path;
  config_read_nh.param("config_path", config_path, std::string(""));
#elif defined ROS2
  rclcpp::init(argc, argv);
  Publish* publish = Publish::getInstance();
  std::string config_path;
  publish->m_nh->get_parameter("config_path", config_path);
#endif

  ConfigManage config_manage;
  std::string project_path = (std::string)PROJECT_PATH;

  // config_path = "/launch/config/Dome.yaml";
  config_manage.open(project_path+config_path);
  config_manage.setupNodeParams();
  // config_manage.printf();

  publish->setupParam(config_manage.m_input_para);
  publish->init();
  publish->start();

  Buffer buffer;
  buffer.fastr_buf = new ringbuffer(16*1024*1024);
  buffer.slowr_buf = new ringbuffer(1 *1024*1024);
  buffer.heartr_buf = new ringbuffer(128*1024);
  buffer.isf_header_buf = new ringbuffer(ISF_HEADER_SIZE);//256*1024

  Input* input = nullptr;
  input = createInput(config_manage.m_input_para, buffer);
  if(nullptr == input)
  {
    std::cout << "INPUT IS NULLPTR" << std::endl;
    return 0;
  }

  Calc* calc = Calc::getInstance();
  int rv = input->start();
  if (rv < 0)
  {
      #ifdef ROS1
    ros::spin();
  #elif defined ROS2
    rclcpp::spin(publish->m_nh);
    rclcpp::shutdown();
  #endif

    delete input;
    // delete calc;

    delete buffer.fastr_buf;
    delete buffer.slowr_buf;
    delete buffer.heartr_buf;
    return -1;
  }
  calc->setParam(config_manage.m_input_para, buffer);
  rv = calc->init(buffer);
  if (rv < 0)
  {
      #ifdef ROS1
    ros::spin();
  #elif defined ROS2
    rclcpp::spin(publish->m_nh);
    rclcpp::shutdown();
  #endif

    delete input;
    // delete calc;

    delete buffer.fastr_buf;
    delete buffer.slowr_buf;
    delete buffer.heartr_buf;
    return -1;
  }
  
  calc->start();

#ifdef ROS1
  ros::spin();
#elif defined ROS2
  rclcpp::spin(publish->m_nh);
  rclcpp::shutdown();
#endif

  delete input;
  // delete calc;

  delete buffer.fastr_buf;
  delete buffer.slowr_buf;
  delete buffer.heartr_buf;

  return 0;
}