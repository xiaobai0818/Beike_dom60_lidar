/**
 * @file input_file_pcap.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2023-12-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
//#define PCAP
#ifdef PCAP

#include "input.hpp"
#include "decoder_pcap.hpp"

class InputFilePCAP : public Input
{
public:
  InputFilePCAP()
  : m_decoder_pcap(nullptr)
  {
    if(!m_decoder_pcap)
    {
      m_decoder_pcap = new DecoderPcap;
    }
  };
  InputFilePCAP(InputPara& inputparam, Buffer& buffer)
  : Input(inputparam, buffer)
  , m_decoder_pcap(nullptr)
  {
    if(!m_sdk_fastrbuf && buffer.fastr_buf)
    {
      m_sdk_fastrbuf = buffer.fastr_buf;
    }
    if(!m_sdk_slowrbuf && buffer.slowr_buf)
    {
      m_sdk_slowrbuf = buffer.slowr_buf;
    }
    if(!m_sdk_heartrbuf && buffer.heartr_buf)
    {
      m_sdk_heartrbuf = buffer.heartr_buf;
    }
    if(!m_sdk_isf_headerbuf && buffer.isf_header_buf)
    {
      m_sdk_isf_headerbuf = buffer.isf_header_buf;
    }

    if(!m_decoder_pcap)
    {
      m_decoder_pcap = new DecoderPcap;
    }
    DecoderPcapCfg dpc;
    dpc.pcap_path = inputparam.pcap_path;

    dpc.port_fast = inputparam.data_port;
    dpc.port_heart = inputparam.heart_port;
    dpc.port_slow = inputparam.imu_port;
    dpc.port_isf_header = RECV_ISF_HEADER_PORT;

    m_decoder_pcap->init(dpc);
    init();
  };

  ~InputFilePCAP()
  {
  }

  int32_t init();
  int32_t start();
  int32_t openFile(){return 0;};
  int32_t closeFile(){return 0;};
  int32_t receiveFastSocket();

private:
  DecoderPcap* m_decoder_pcap;
};

int32_t InputFilePCAP::init()
{
  if (m_input_param.cfg_path.empty())
  {
    char* buf = new char[IP_PACKET_MAX_LEN];
    int total_length = 0;
    while (true)
    {
      uint32_t len = 0;
      PacketType_E pt;
      int rv = m_decoder_pcap->getPcapData(buf, len, pt);
      //usleep(1000);
      if (rv == 1)
      {
        break;
      }
      if(len > 0)
      {
        switch (pt)
        {
        case PACKET_HEART:
        if (m_sdk_heartrbuf->get_free() >= len)
        {
          m_sdk_heartrbuf->write((uint8_t *)buf, len);
        }
          break;
        case PACKET_ISF_HEADER:
        {
          m_sdk_isf_headerbuf->write((uint8_t *)buf, len);
          total_length += len;
          if (total_length == m_sdk_isf_headerbuf->get_set_size())
          {
            delete []buf;
            return 0;
          }
        }
          break;
        default:
          break;
        }
      }
      else
      {
        usleep(100);
      }
    }
    delete []buf;
  }

  return 0;
}

int32_t InputFilePCAP::start()
{
  if(m_start_flag)
  {
    return 0;
  }

  m_start_flag = true;

  // pcap文件使用同一个线程来解算快数据、慢数据、心跳包数据
  m_fast_data_receive_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&InputFilePCAP::receiveFastSocket, this)));

  return 0;
}

int32_t InputFilePCAP::receiveFastSocket()
{
  char* buf = new char[IP_PACKET_MAX_LEN];

  // uint32_t fast_num = 0, slow_num = 0, heart_num = 0;

  while (true)
  {
    uint32_t len = 0;
    PacketType_E pt;
    int rv = m_decoder_pcap->getPcapData(buf, len, pt);
    //usleep(1000);
    if (rv == 1)
    {
      // std::cout << "fast_num " << fast_num << " slow_num " << slow_num << " heart_num " << heart_num << std::endl;
      break;
    }

    if(len > 0)
    {
      switch (pt)
      {
      case PACKET_FAST:
        while(m_sdk_fastrbuf->get_free() < len)
        {
          usleep(100);
        }
        m_sdk_fastrbuf->write((uint8_t *)buf, len);
        // fast_num ++;
        break;
      case PACKET_SLOW:
        while(m_sdk_slowrbuf->get_free() < len)
        {
          usleep(100);
        }
        m_sdk_slowrbuf->write((uint8_t *)buf, len);
        // slow_num ++;
        break;
      case PACKET_HEART:
        while(m_sdk_heartrbuf->get_free() < len)
        {
          usleep(100);
        }
        m_sdk_heartrbuf->write((uint8_t *)buf, len);
        // heart_num ++;
      default:
        break;
      }
    }
    else
    {
      usleep(100);
    }
  }
  m_decoder_pcap->reset();

  delete []buf;

  return 0;
}

#endif