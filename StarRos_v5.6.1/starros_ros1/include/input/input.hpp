/**
 * @file input.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "common.h"
#include <thread>
#include <functional>

class Input
{
public:

  /**
   * @brief 输入初始化
   * 
   * @return int32_t 
   */
  virtual int32_t init(){return 0;};

  /**
   * @brief 输入线程启动函数
   * 
   * @return int32_t 
   */
  virtual int32_t start() = 0;

  /**
   * @brief 输入线程关闭函数
   * 
   */
  virtual void stop();

  /**
   * @brief Construct a new Input object
   * 
   */
  Input()
  : m_init_flag(false)
  , m_start_flag(false)
  {
  };

  /**
   * @brief Construct a new Input object
   * 
   * @param inputparam 
   */
  Input(InputPara& inputparam, Buffer& buffer)
  : m_init_flag(false)
  , m_start_flag(false)
  , m_sdk_fastrbuf(nullptr)
  , m_sdk_slowrbuf(nullptr)
  , m_sdk_heartrbuf(nullptr)
  , m_sdk_isf_headerbuf(nullptr)
  {
    m_input_param = inputparam;
  }

  /**
   * @brief Destroy the Input object
   * 
   */
  virtual ~Input()
  {
    stop();
  }

protected:
  /// @brief 输入参数
  InputPara m_input_param;

  /// @brief 输入线程指针(快数据)
  std::shared_ptr<std::thread> m_fast_data_receive_thread;

  /// @brief 输入线程指针(慢数据)
  std::shared_ptr<std::thread> m_slow_data_receive_thread;

  /// @brief 初始化标志
  bool m_init_flag;

  /// @brief 线程启动标志
  bool m_start_flag;

  /// @brief 快数据buffer
  ringbuffer* m_sdk_fastrbuf;

  /// @brief 慢数据buffer
  ringbuffer* m_sdk_slowrbuf;

  /// @brief 慢数据buffer
  ringbuffer* m_sdk_heartrbuf;

  /// @brief isf_header数据buffer
  ringbuffer* m_sdk_isf_headerbuf;
};

void Input::stop()
{
  if(m_start_flag)
  {
    m_fast_data_receive_thread->join();
    m_slow_data_receive_thread->join();
    m_start_flag = false;
  }
}
