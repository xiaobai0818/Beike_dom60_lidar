/**
 * @file input_file_isf.hpp
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

class InputFileISF : public Input
{
public:
  InputFileISF()
  {
  };
  InputFileISF(InputPara& inputparam, Buffer& buffer) : Input(inputparam, buffer)
  {
  };
  ~InputFileISF()
  {
  }

  int32_t init();
  int32_t start(){return 0;};
  int32_t openFile(){return 0;};
  int32_t closeFile(){return 0;};
  int32_t receiveFastSocket(){return 0;};
};

int32_t InputFileISF::init()
{
  return 0;
}
