/**
 * @file calc.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <thread>
#include "sdk_single_instance.h"
#include "common.h"
#include "ICD_Scada_API.h"

extern int32_t pointcloudCallback(L1DataFrame_S& data_frame);

extern int32_t imuCallback(std::vector<L1IMU_S>& raw_imu_msg);

extern void updateScanSpeed(int32_t speed);

const uint32_t FAST_CP_BUF_SIZE = 100 * 1024;
const uint32_t SLOW_CP_BUF_SIZE = 16  * 1024;
const float DOM_SPEED = 36.7;
const float DOM_SPEED_2 = 61.7;

class Calc
{

public:
  /**
   * @brief 单例模式获取实例化对象
   * 
   * @return Calc* 
   */
  static Calc* getInstance()
  {
    static Calc* ptr = nullptr;
    if (nullptr == ptr)
    {
      ptr = new Calc();
    }
    return ptr;
  };

  /**
   * @brief Construct a new Calc object
   * 
   */
  Calc();

  /**
   * @brief Destroy the Calc object
   * 
   */
  ~Calc();

  /**
   * @brief 设置sdk解算类初始化参数
   * 
   * @param input_para 初始化参数
   * @return int32_t 
   */
  int32_t setParam(InputPara& input_para, Buffer& buffer);

  /**
   * @brief SDK初始化函数(ISF文件)
   * 
   * @return int32_t 
   */
  int32_t initStatic();

  /**
   * @brief SDK初始化函数(UDP)
   * 
   * @return int32_t 
   */
  int32_t initDynamic();

  /**
   * @brief SDK初始化函数(PCAP)
   * 
   * @return int32_t 
   */
  int32_t initDynamicPcap();

  /**
   * @brief SDK初始化函数
   * 
   * @return int32_t 
   */
  int32_t init(Buffer& buffer);

  /**
   * @brief 线程启动函数
   * 
   * @return int32_t 
   */
  int32_t start();

  /**
   * @brief 线程停止函数
   * 
   * @return int32_t 
   */
  int32_t stop();

  /**
   * @brief 数据解码线程运行函数（实时）
   * 
   */
  void dataHandleThreadDynamic();

  /**
   * @brief 数据解码线程运行函数（实时）
   * 
   */
  void dataSlowHandleThreadDynamic();


  /**
   * @brief 数据解码线程运行函数（离线）
   * 
   */
  void dataHandleThreadStatic();

  /**
   * @brief 快数据解码线程运行函数（离线PCAP）
   *
   */
  void dataSlowHandleThreadPCAP();

  /**
   * @brief 满数据解码线程运行函数（离线PCAP）
   *
   */
  void dataFastHandleThreadPCAP();

  /**
   * @brief 点云扫描帧释放函数，仅在异步回调时使用
   * 
   * @param data_frame 数据帧指针
   */
  void freeDataFrame(L1DataFrame_S* data_frame);

private:

  /// @brief sdk类实例化对象指针
  StarSDK_I* m_sdk;

  /// @brief 设备控制类实例化对象指针
  DeviceControl_I* m_device_control;

  /// @brief 快数据解算线程指针
  std::shared_ptr<std::thread> m_calc_thread;

  /// @brief 慢数据解算线程指针
  std::shared_ptr<std::thread> m_calc_slow_thread;

  /// @brief sdk初始化配置参数
  ConfigParam_S m_config_param;

  /// @brief sdk运行模式标志
  InputType m_input_type;

  /// @brief calc初始化标志
  bool m_init_flag;

  /// @brief calc线程启动标志
  bool m_start_flag;

  /// @brief 设备类型名
  std::string m_device_name;

  /// @brief 快数据buffer
  ringbuffer* m_sdk_fastrbuf;

  /// @brief 慢数据buffer
  ringbuffer* m_sdk_slowrbuf;

  /// @brief 慢数据buffer
  ringbuffer* m_sdk_heartrbuf;

  /// @brief isf_header数据buffer
  ringbuffer* m_sdk_isf_headerbuf;

};

Calc::Calc()
: m_sdk(nullptr)
, m_device_control(nullptr)
, m_input_type(InputType::NET_UDP)
, m_init_flag(false)
, m_start_flag(false)
, m_device_name("")
, m_sdk_fastrbuf(nullptr)
, m_sdk_slowrbuf(nullptr)
, m_sdk_heartrbuf(nullptr)
, m_sdk_isf_headerbuf(nullptr)
{
  m_device_control = getDeviceControlInstance();
}

Calc::~Calc()
{
  stop();

  if(m_init_flag)
  {
    if(m_sdk)
    {
      delete m_sdk;
      m_sdk = nullptr;
    }

    if(m_device_control)
    {
      delete m_device_control;
      m_device_control = nullptr;
    }

    m_init_flag = false;
  }
}

int32_t Calc::setParam(InputPara& input_para, Buffer& buffer)
{
  m_device_name = input_para.device_name;
  m_config_param.calc_config_param.Level0A = input_para.Level0A;
  m_config_param.calc_config_param.Level0C = input_para.Level0C;
  m_config_param.calc_config_param.Level0G = input_para.Level0G;
  m_config_param.calc_config_param.Level0M = input_para.Level0M;
  m_config_param.calc_config_param.Level0N = input_para.Level0N;
  m_config_param.calc_config_param.Level0P = input_para.Level0P;
  m_config_param.calc_config_param.Level0Q = input_para.Level0Q;
  m_config_param.calc_config_param.Level0S = input_para.Level0S;
  
  m_config_param.calc_config_param.Level1A = input_para.Level1A;
  m_config_param.calc_config_param.Level1B = input_para.Level1B;
  m_config_param.calc_config_param.Level1F = input_para.Level1F;
  m_config_param.calc_config_param.Level1G = input_para.Level1G;
  m_config_param.calc_config_param.Level1L = input_para.Level1L;
  m_config_param.calc_config_param.Level1M = input_para.Level1M;
  m_config_param.calc_config_param.Level1N = input_para.Level1N;
  m_config_param.calc_config_param.Level1Q = input_para.Level1Q;
  m_config_param.calc_config_param.Level2A = input_para.Level2A;

  m_config_param.calc_config_param.LevelAB = input_para.LevelAB;
  m_config_param.calc_config_param.LevelAC = input_para.LevelAC;
  m_config_param.calc_config_param.LevelAH = input_para.LevelAH;
  m_config_param.calc_config_param.LevelAI = input_para.LevelAI;
  m_config_param.calc_config_param.LevelAJ = input_para.LevelAJ;
  m_config_param.calc_config_param.LevelAM = input_para.LevelAM;
  m_config_param.calc_config_param.LevelAN = input_para.LevelAN;

  m_config_param.calc_config_param.dome_coor_modle = (DomeCoorModle_S)input_para.dome_mode_type;

  m_config_param.dec_config_param.dev_proto = (DevProto_E)input_para.data_proto;

  if(!input_para.pcap_path.empty())
  {
    m_input_type = InputType::PCAP_FILE;
    m_sdk_fastrbuf = buffer.fastr_buf;
    m_sdk_slowrbuf = buffer.slowr_buf;
    m_sdk_heartrbuf = buffer.heartr_buf;
    m_sdk_isf_headerbuf = buffer.isf_header_buf;

    // 获取心跳包中设备ID
    HEARTBEAT_S heart_beat;
    memset((void *)&heart_beat, 0x00, sizeof(HEARTBEAT_S));
    while(true)
    {
      m_sdk_heartrbuf->read((uint8_t *)&heart_beat, sizeof(HEARTBEAT_S));
      if(0xE4E3E2E1 == heart_beat.pkgflag)
      {
        break;
      }
    }
    m_config_param.dec_config_param.device_id = heart_beat.device_id;
  }
  else if(!input_para.isf_path.empty())
  {
    m_input_type = InputType::ISF_FILE;
    m_config_param.fileConfig.raw_path = input_para.isf_path;
  }
  else
  {
    m_input_type = InputType::NET_UDP;
    m_config_param.dec_config_param.device_id = m_device_control->getDeviceId();
  }
  m_config_param.fileConfig.cfg_path = input_para.cfg_path;
  m_config_param.fileConfig.out_path = input_para.save_isf_path;

  m_config_param.fileConfig.out_file.out_log = false;
  if (input_para.cfg_path.empty())
  {
    m_config_param.fileConfig.method = RAW_FILE;
  }
  else
  {
    m_config_param.fileConfig.method = CONFIG_FILE;
  }
  m_config_param.dynanic_dec_param.devIp = input_para.device_ip;
  m_config_param.dynanic_dec_param.isSaveISF = input_para.save_isf;
  m_config_param.dynanic_dec_param.savePath = input_para.save_isf_path;

  return 0;
}

int32_t Calc::initStatic()
{
  m_sdk = getSDKInstance(DecType::eStaticType);
  m_sdk->init(&m_config_param, pointcloudCallback, true);

  return 0;
}

int32_t Calc::initDynamic()
{
  m_sdk = getSDKInstance(DecType::eDynamicType);
  // while(!m_device_control->getDeviceId())
  // {
  //   usleep(1000);
  // }
  char* isf_header = m_device_control->getIsfHead((char*)m_config_param.dynanic_dec_param.devIp.c_str());
  if (!isf_header)
  {
    std::cout << "starros log : get isf_header is null from device, need check device config xml !!!!" << std::endl;
  }
  
  m_sdk->init(&m_config_param, pointcloudCallback, true, m_config_param.dec_config_param.device_id,isf_header);
  m_device_control->setWriteFileEnable(m_config_param.dynanic_dec_param.isSaveISF, m_config_param.dynanic_dec_param.savePath);

  return 0;
}

int32_t Calc::initDynamicPcap()
{
  m_sdk = getSDKInstance(DecType::eDynamicType);
  char* isf_header = nullptr;
  if (m_config_param.fileConfig.cfg_path.empty())
  {
    isf_header = new char[ISF_HEADER_SIZE];
    uint32_t rtn = m_sdk_isf_headerbuf->read((uint8_t *)isf_header, ISF_HEADER_SIZE);
    if (ISF_HEADER_SIZE != (rtn + 1))
    {
      std::cout<< "pcap use device isf header,but not get one,need use revise.ini !!!" << std::endl;
      delete []isf_header;
      isf_header = nullptr;
    }
  }  
  m_sdk->init(&m_config_param, pointcloudCallback, true, m_config_param.dec_config_param.device_id,isf_header);

  if (isf_header)
  {
    if (m_device_name == "Dome")
    {
      Isf_Header_S* header = (Isf_Header_S*)isf_header;
      //回放pcap时需要根据头文件里的转速设置对应的转速
      if (header->scanspeed == DOM_SPEED || header->scanspeed == 60)
      {
        updateScanSpeed(60);
      }
      else if (header->scanspeed == DOM_SPEED_2 || header->scanspeed == 70)
      {
        updateScanSpeed(70);
      }
    }

    delete []isf_header;
    isf_header = nullptr;
  }
  
  return 0;
}

int32_t Calc::init(Buffer& buffer)
{
  int32_t rv = 0;
  if(m_init_flag)
  {
    return rv;
  }
  m_init_flag = true;

  // 暂时这么写，实时模式和离线PCAP模式相似，后续会统一离线ISF模式解析
  switch (m_input_type)
  {
  case InputType::NET_UDP :
    rv = initDynamic();
    break;
  case InputType::PCAP_FILE :
    rv = initDynamicPcap();
    break;
  case InputType::ISF_FILE :
    rv = initStatic();
    break;
  default:
    break;
  }
  return rv;
}

void Calc::dataHandleThreadDynamic()
{
  uint8_t *m_fast_cp_buffer = new uint8_t[FAST_CP_BUF_SIZE];
  int32_t pfastlens = FAST_CP_BUF_SIZE;
  int32_t rtnfast = 0;
  while (true)
  {
    if (!m_device_control->isListener())
    {
      usleep(100000);
      continue;
    }
    //1、获取快慢数据
    pfastlens = FAST_CP_BUF_SIZE;
    rtnfast = m_device_control->getFastData(m_fast_cp_buffer, pfastlens);

    if (rtnfast != 0 || !pfastlens)
    {
      usleep(100);
      continue;
    }
    try
    {
      //2、解算数据
      if(0 == rtnfast && pfastlens)
      {
        uint32_t dec_fast_size = m_sdk->runDec((char*)m_fast_cp_buffer, pfastlens, m_device_control->getTemperature());
        if(0 == dec_fast_size)
        {
          usleep(100);
          continue;
        }

        m_device_control->freeFastBuffter(dec_fast_size);
      }
    }
    catch (...)
    {
      std::cout << "decode fast error" << std::endl;
    }
  }

  return;
}

void Calc::dataSlowHandleThreadDynamic()
{
  uint8_t *m_slow_cp_buffer = new uint8_t[SLOW_CP_BUF_SIZE];
  int32_t pslowlens = SLOW_CP_BUF_SIZE;
  int32_t rtnslow = 0;

  while (true)
  {
    pslowlens = SLOW_CP_BUF_SIZE;
    rtnslow = m_device_control->getSlowData(m_slow_cp_buffer, pslowlens);

    if (rtnslow != 0 || !pslowlens)
    {
      usleep(100);
      continue;
    }
    try
    {
      //2、解算数据
      if(0 == rtnslow && pslowlens)
      {
        uint32_t dec_slow_size = m_sdk->runDecSlow((char*)m_slow_cp_buffer, pslowlens);
        if(0 == dec_slow_size)
        {
          usleep(100);
          continue;
        }
        imuCallback(m_sdk->getImuArray());
        m_device_control->freeSlowBuffter(dec_slow_size);
      }
    }
    catch (...)
    {
      std::cout << "decode slow error" << std::endl;
    }
  }
  return;
}

void Calc::dataHandleThreadStatic()
{
  m_sdk->runDec();
  return;
}

void Calc::dataSlowHandleThreadPCAP()
{
  uint8_t *m_slow_cp_buffer = new uint8_t[SLOW_CP_BUF_SIZE];
  uint32_t pslowlens = SLOW_CP_BUF_SIZE;
  uint32_t rtnslow = 0;

  uint8_t* temp_buffer = new uint8_t[SLOW_CP_BUF_SIZE];
  uint32_t heartlens = 0;

  while(true)
  {
    pslowlens = SLOW_CP_BUF_SIZE;
    rtnslow = m_sdk_slowrbuf->read_without_free(m_slow_cp_buffer, pslowlens);
    try
    {
      //2、解算数据
      if(0 != rtnslow)
      {
        uint32_t dec_slow_size = m_sdk->runDecSlow((char*)m_slow_cp_buffer, rtnslow);
        if(0 == dec_slow_size)
        {
          usleep(100);
          continue;
        }
        m_sdk_slowrbuf->free(dec_slow_size);
        imuCallback(m_sdk->getImuArray());

      }
      else
      {
        usleep(100);
      }
    }
    catch (...)
    {
      std::cout << "decode slow error" << std::endl;
    }
  }


  return;
}

void Calc::dataFastHandleThreadPCAP()
{
  uint8_t *m_fast_cp_buffer = new uint8_t[FAST_CP_BUF_SIZE];
  uint32_t pfastlens = FAST_CP_BUF_SIZE;
  uint32_t rtnfast = 0;

  float temperature = 0.;
  HEARTBEAT_S heart_beat;
  uint32_t rtnheart = 0;

  while (true)
  {
    //1、获取快慢数据
    pfastlens = FAST_CP_BUF_SIZE;

    // 这样写，如果心跳包端口存在其他数据，会一直读取不到，后续考虑替换为窥读方式
    memset((void *)&heart_beat, 0x00, sizeof(HEARTBEAT_S));
    rtnheart = m_sdk_heartrbuf->read((uint8_t *)&heart_beat, sizeof(HEARTBEAT_S));
    if(0 != rtnheart && 0xE4E3E2E1 == heart_beat.pkgflag)
    {
      temperature = heart_beat.temperature * 0.01;

      // std::cout << "temperature " << temperature << std::endl;
    }

    rtnfast = m_sdk_fastrbuf->read_without_free(m_fast_cp_buffer, pfastlens);;

    try
    {
      //2、解算数据
      if(0 != rtnfast)
      {
        uint32_t dec_fast_size = m_sdk->runDec((char*)m_fast_cp_buffer, rtnfast, temperature);
        if(0 == dec_fast_size)
        {
          usleep(100);
          continue;
        }
        m_sdk_fastrbuf->free(dec_fast_size);
      }
      else
      {
        usleep(100);
      }
    }
    catch (...)
    {
      std::cout << "decode fast error" << std::endl;
    }
  }

  return;
}

int32_t Calc::start()
{
  if(m_start_flag)
  {
    return 0;
  }

  m_start_flag = true;


  switch (m_input_type)
  {
  case InputType::NET_UDP :
    m_calc_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&Calc::dataHandleThreadDynamic, this)));
    // 只有Dome存在慢数据
    if("Dome" == m_device_name)
    {
      m_calc_slow_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&Calc::dataSlowHandleThreadDynamic, this)));
    }
    break;
  case InputType::PCAP_FILE :
    m_calc_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&Calc::dataFastHandleThreadPCAP, this)));
    // 只有Dome存在慢数据
    if("Dome" == m_device_name)
    {
      m_calc_slow_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&Calc::dataSlowHandleThreadPCAP, this)));
    }
    break;
  
  case InputType::ISF_FILE :
      m_calc_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&Calc::dataHandleThreadStatic, this)));
    break;
  default:
    break;
  }
  return 0;
}

int32_t Calc::stop()
{
  if(m_start_flag)
  {
    m_calc_thread->join();
    m_start_flag = false;
  }

  return 0;
}

void Calc::freeDataFrame(L1DataFrame_S* data_frame)
{
  m_sdk->freeDataFrame(data_frame);
}