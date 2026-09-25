/**
 * @file input_create.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "input.hpp"
#include "input_udp.hpp"
#include "input_file_isf.hpp"
#include "input_file_pcap.hpp"

InputType initRunMode(const InputPara& input)
{
  InputType run_mode = InputType::NET_UDP;
  if(!input.isf_path.empty())
  {
    run_mode = InputType::ISF_FILE;
  }
#ifdef PCAP
  if(!input.pcap_path.empty())
  {
    run_mode = InputType::PCAP_FILE;
  }
#endif
  return run_mode;
}

/**
 * @brief 输入类实例化函数
 *
 * @param input_param
 * @param buffer
 * @return Input* 
 */
inline Input* createInput(InputPara& input_param, Buffer& buffer)
{
  Input* input = nullptr;

  switch (initRunMode(input_param))
  {
  case InputType::NET_UDP:
    input =  static_cast<Input*>(new InputUDP(input_param, buffer));
    break;
  case InputType::ISF_FILE:
    input =  static_cast<Input*>(new InputFileISF(input_param, buffer));
    break;
  case InputType::PCAP_FILE:
#ifdef PCAP
    input =  static_cast<Input*>(new InputFilePCAP(input_param, buffer));
#else
    input =  nullptr;
#endif
    break;
  
  default:
    input =  nullptr;
    break;
  }
  return input;
}