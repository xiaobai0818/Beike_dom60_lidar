/**
 * @file sdk_single_instance.h
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "star_sdk_I.h"
#include "device_control_I.h"

StarSDK_I* getSDKInstance(DecType mode)
{
  static StarSDK_I* sdk;
  if(nullptr == sdk)
  {
    sdk = getStarSDK(mode);
  }
  return sdk;
}

DeviceControl_I* getDeviceControlInstance()
{
  static DeviceControl_I* device_control;
  if(nullptr == device_control)
  {
    device_control = getDeviceControl();
  }
  return device_control;
}