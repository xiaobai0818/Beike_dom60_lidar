/**
 * @file decoder_pcap.h
 * @author yukai
 * @brief 解析离线pcap包
 * @version 0.1
 * @date 2024-4-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
//#define PCAP
#ifdef PCAP

#include <iostream>
#include <string.h>
#include <pcap.h>
#include <thread>
#include <atomic>
#include <fstream>
#include <list>
#include <condition_variable>
#include <mutex>

static const uint32_t IP_PACKET_MAX_LEN = 65536;
const uint16_t PORT_FAST = 2014;
const uint16_t PORT_SLOW = 2025;
const uint16_t PORT_HEART = 2030;
const uint16_t PORT_ISF_HEADER = 2043;

typedef struct DecoderPcapCfg
{
  std::string pcap_path;		    // pcap文件绝对路径
  std::string filter_rule;  	  // 过滤规则,默认为空
  uint16_t port_fast;           // 快数据端口
  uint16_t port_slow;           // 慢数据端口
  uint16_t port_heart;          // 心跳包端口
  uint16_t port_isf_header;     // isf_header端口
  DecoderPcapCfg()
  {
    pcap_path = "";
    filter_rule = "";
    port_fast = 0;
    port_slow = 0;
    port_heart = 0;
    port_isf_header = 0;
  }
}DecoderPcapCfg_S;

typedef enum PacketType
{
  PACKET_INVALID,
  PACKET_HEART = 5,
  PACKET_FAST,
  PACKET_SLOW,
  PACKET_ISF_HEADER,
}PacketType_E;


#pragma pack(1)
typedef struct EtherHeader
{
  uint8_t dhost[6];
  uint8_t shost[6];
  uint16_t type;
};

typedef struct IPHeader
{
  uint8_t headersize_version;     ///< bit0-3 IP版本，bit4-7数据报头长度
  uint8_t service_type;
  uint16_t total_length;          ///< 头部和数据之和长度,单位：字节
  uint16_t identification;        ///< 标识，辅助判断该IP分片属于哪个数据报文，便于重组
  uint16_t flag_frag_offset;      ///< bit0-12 片偏移 bit13-15 标志：是否分片
  uint8_t time_to_live;           ///< 生存时间
  uint8_t protocol;               ///< 协议
  uint16_t header_checksum;       ///< 首部校验和
  uint32_t source_address;        ///< 来源IP地址
  uint32_t destination_address;   ///< 目标IP地址
}IPHeader_S;

struct udphdr
{
  uint16_t source;
  uint16_t dest;
  uint16_t len;
  uint16_t check;
};
#pragma pack()

typedef struct PcapPacket
{
  char* pcap_data;
  uint32_t pcap_len;
  PacketType_E packet_type;
  PcapPacket()
  {
    pcap_data = nullptr;
    pcap_len = 0;
    packet_type = PACKET_INVALID;
  }
}PcapPacket_S;

const uint16_t ETHERNET_HEADER_LEN = 14;
const uint16_t UDP_HEADER_LEN = 8;

#define PCAP_IP_MF		0x2000	// More Fragments flag 
#define PCAP_IP_OFFMASK 0x1FFF	// Fragment Offset mask 

//多线程
typedef std::list<PcapPacket_S>PCAPPACKETLIST;
PCAPPACKETLIST g_free_packets_list;
PCAPPACKETLIST g_busy_packets_list;
const uint16_t SET_PACKETS_LIST_SIZE = 10;
std::mutex g_mtx;
std::condition_variable g_cv_free;
std::condition_variable g_cv_busy;
int64_t g_last_packet_time = 0;
int32_t g_last_identification = -1;

class DecoderPcap
{
public:
  DecoderPcap();
  virtual ~DecoderPcap();

  /************解码pcap包************/
  /**
  * @brief 初始化,配置文件中任意一个参数改变，均需要调用该接口
  *
  * @param config 初始化配置参数
  * @return =0表示成功，否则错误程序必须退出
  */
  int32_t init(DecoderPcapCfg_S& config);

  /**
  * @brief 获取一个包的裸数据,内部会阻塞直到获取一个包
  *
  * @param buf 需要调用者提前分配好IP_PACKET_MAX_LEN大小的内存，内部会将每个包的数据拷贝给buf
  * @param len 返回实际长度
  * @param type 返回包类型
  * @return =0表示成功；=1表示文件读取结束，无数据获取,退出循环
  */
  int32_t getPcapData(char* buf, uint32_t& len, PacketType_E& type);

  /**
  * @brief 运行过程中强制退出循环解析pcap文件
  *
  * @return
  */
  void exit();

  /**
  * @brief 重置接口，当pcap包解析结束或中间退出调用，与init对应
  *
  * @return
  */
  void reset();

protected:
  static void packetHandler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet);

private:
  DecoderPcapCfg_S m_decoder_pcap_cfg;
  pcap_t* m_pcap_handle;
  std::atomic_bool m_decoder_pcap_finish;
  std::atomic_bool m_break_loop;
  struct bpf_program m_bpf;
  std::thread m_pcap_loop_th;
  static uint32_t m_packet_len;
  static char* m_packet_data;
};

//静态成员变量初始化
uint32_t DecoderPcap::m_packet_len = 0;
char* DecoderPcap::m_packet_data = nullptr;

inline uint16_t my_ntohs(uint16_t net_data)
{
	return (net_data >> 8) | (net_data << 8);
}

DecoderPcap::DecoderPcap()
  : m_pcap_handle(nullptr)
  , m_decoder_pcap_finish(false)
  , m_break_loop(false)
{
  for (uint16_t i = 0; i < SET_PACKETS_LIST_SIZE; ++i)
  {
    PcapPacket_S pp;
    pp.pcap_data = new char[IP_PACKET_MAX_LEN];
    g_free_packets_list.emplace_back(pp);
  }
  m_packet_data = new char[IP_PACKET_MAX_LEN];
}


DecoderPcap::~DecoderPcap()
{
  if (m_pcap_handle)
  {
    pcap_close(m_pcap_handle);
    m_pcap_handle = nullptr;
  }
  if (!g_free_packets_list.empty())
  {
    for (auto& it : g_free_packets_list)
    {
      delete[] it.pcap_data;
      it.pcap_data = nullptr;
    }
    g_free_packets_list.clear();
  }
  if (!g_busy_packets_list.empty())
  {
    for (auto& it : g_busy_packets_list)
    {
      delete[] it.pcap_data;
      it.pcap_data = nullptr;
    }
    g_busy_packets_list.clear();
  }
  if (m_packet_data)
  {
    delete[] m_packet_data;
    m_packet_data = nullptr;
  }

}

int32_t DecoderPcap::init(DecoderPcapCfg_S& config)
{
  m_decoder_pcap_cfg = config;
  char errbuf[PCAP_ERRBUF_SIZE] = { 0 };
  // Open the pcap file for reading 
  m_pcap_handle = pcap_open_offline(m_decoder_pcap_cfg.pcap_path.c_str(), errbuf);
  if (m_pcap_handle == nullptr) {
    std::cerr << "Error opening pcap file: " << errbuf << std::endl;
    return -1;
  }
  // Compile and set the filter 
  bpf_u_int32 net_mask = 0xffffffff;
  if (pcap_compile(m_pcap_handle, &m_bpf, m_decoder_pcap_cfg.filter_rule.c_str(), 1, net_mask) == -1) {
    std::cout << "Error compiling filter: " << pcap_geterr(m_pcap_handle) << std::endl;
    return -1;
  }
  if (pcap_setfilter(m_pcap_handle, &m_bpf) == -1) {
    std::cout << "Error setting filter: " << pcap_geterr(m_pcap_handle) << std::endl;
    return -1;
  }
  m_pcap_loop_th = std::thread([this]() {
    int rv = pcap_loop(m_pcap_handle, -1, &DecoderPcap::packetHandler, reinterpret_cast<u_char*>(this));
    if (rv < 0)
    {
      std::cout << "pcap_loop error: " << pcap_geterr(m_pcap_handle) << std::endl;
    }
    else if (rv == 0)
    {
      std::cout << "no packets were captured " << std::endl;
    }
    if (!g_busy_packets_list.empty())
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    m_decoder_pcap_finish = true;
    g_cv_busy.notify_one();
  });

  return 0;
}

void DecoderPcap::packetHandler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
  DecoderPcap* myself_ptr = reinterpret_cast<DecoderPcap*>(user);
  if (myself_ptr->m_break_loop)
  {
    pcap_breakloop(myself_ptr->m_pcap_handle);
    return;
  }
  int64_t packrt_time = static_cast<int64_t>(pkthdr->ts.tv_sec) * 1000000 + pkthdr->ts.tv_usec;
  if (g_last_packet_time == 0)
  {
    g_last_packet_time = packrt_time;
  }
  int64_t time_diff = packrt_time - g_last_packet_time;
  if (time_diff > 10000 && time_diff < 1000000)
  {
    //TODO 暂时先不在这里sleep，避免造成运行时间过长
    //std::this_thread::sleep_for(std::chrono::microseconds(time_diff));
  }
  g_last_packet_time = packrt_time;

  EtherHeader *ether_header = (EtherHeader*)(packet);
  if (my_ntohs(ether_header->type) != 0x0800) //ipv4
    return;

  IPHeader *ip_header = (IPHeader*)(packet + ETHERNET_HEADER_LEN);
  if (ip_header->protocol != 0x11) //默认只解udp包
    return;

  // ip data
  uint16_t ip_hdr_size = (ip_header->headersize_version & 0xf) * 4;
  uint16_t ip_len = my_ntohs(ip_header->total_length);
  uint16_t frag_offset = my_ntohs(ip_header->flag_frag_offset);
  const uint8_t* udp_data = packet + ETHERNET_HEADER_LEN + ip_hdr_size;
  uint16_t udp_data_len = ip_len - ip_hdr_size;
  bool packet_flag = false;
  if (g_last_identification == ip_header->identification)
  {
    // Check if this is the last fragment 
    if (!(frag_offset & PCAP_IP_MF))
    {
      packet_flag = true;
    }

    uint16_t frag_off = (frag_offset & PCAP_IP_OFFMASK) * 8;
    if (frag_off == m_packet_len)
    {
      memcpy(m_packet_data + m_packet_len, udp_data, udp_data_len);
      m_packet_len += udp_data_len;
      if (packet_flag)
      {
        udp_data = (const uint8_t*)(m_packet_data);
        udp_data_len = m_packet_len - UDP_HEADER_LEN;
        m_packet_len = 0;
      }
    }
    else
    {
      //当满足这个条件意味着丢包了
      m_packet_len = 0;
      packet_flag = false;
    }
  }
  else
  {
    if (frag_offset & PCAP_IP_MF)
    {
      //第一个切片包
      m_packet_len = 0;
      memcpy(m_packet_data + m_packet_len, udp_data, udp_data_len);
      m_packet_len += udp_data_len;
      g_last_identification = ip_header->identification;
    }
    else
    {
      //非切片包
      udp_data_len -= UDP_HEADER_LEN;
      packet_flag = true;
    }
  }

  if (packet_flag)
  {
    udphdr* udp_hdr = (udphdr*)udp_data;
    uint16_t port_dest = my_ntohs(udp_hdr->dest);
		uint16_t port_src = my_ntohs(udp_hdr->source);
		PacketType_E type = PACKET_INVALID;
		if (port_dest == PORT_FAST || port_src == PORT_FAST)
		{
			type = PACKET_FAST;
		}
		else if (port_dest == PORT_SLOW || port_src == PORT_SLOW)
		{
			type = PACKET_SLOW;
		}
		else if (port_dest == PORT_HEART)
		{
			type = PACKET_HEART;
		}
		else if (port_src == PORT_ISF_HEADER)
		{
			type = PACKET_ISF_HEADER;
		}
    if (type != PACKET_INVALID)
    {
      std::unique_lock<std::mutex>lock(g_mtx);
      g_cv_free.wait(lock, []() {return !g_free_packets_list.empty(); });
      PcapPacket_S tmp = g_free_packets_list.back();
      g_free_packets_list.pop_back();
      memcpy(tmp.pcap_data, udp_data + UDP_HEADER_LEN, udp_data_len);
      tmp.packet_type = type;
      tmp.pcap_len = udp_data_len;
      g_busy_packets_list.emplace_back(tmp);
      g_cv_busy.notify_one();
    }
  }
}

int32_t DecoderPcap::getPcapData(char* buf, uint32_t& len, PacketType_E& type)
{
  if (m_decoder_pcap_finish)
    return 1;

  std::unique_lock<std::mutex>lock(g_mtx);
  g_cv_busy.wait(lock, [this]() {return (!g_busy_packets_list.empty() || m_decoder_pcap_finish); });
  if (!g_busy_packets_list.empty())
  {
    PcapPacket_S tmp = g_busy_packets_list.front();
    g_busy_packets_list.pop_front();
    memcpy(buf, tmp.pcap_data, tmp.pcap_len);
    len = tmp.pcap_len;
    type = tmp.packet_type;

    g_free_packets_list.emplace_back(tmp);
    g_cv_free.notify_one();
  }

  return 0;
}

void DecoderPcap::exit()
{
  m_break_loop = true;
  std::lock_guard<std::mutex>lock(g_mtx);
  if (!g_busy_packets_list.empty())
  {
    for (auto it : g_busy_packets_list)
    {
      g_free_packets_list.emplace_back(it);
    }
    g_busy_packets_list.clear();
  }
  g_cv_free.notify_one();
}

void DecoderPcap::reset()
{
  m_pcap_loop_th.join();
  pcap_freecode(&m_bpf);
  if (m_pcap_handle)
  {
    pcap_close(m_pcap_handle);
    m_pcap_handle = nullptr;
  }
  m_decoder_pcap_finish = false;
  m_break_loop = false;
  g_last_identification = -1;
  g_last_packet_time = 0;
  if (!g_busy_packets_list.empty())
  {
    for (auto it : g_busy_packets_list)
    {
      g_free_packets_list.emplace_back(it);
    }
    g_busy_packets_list.clear();
  }
}

#endif