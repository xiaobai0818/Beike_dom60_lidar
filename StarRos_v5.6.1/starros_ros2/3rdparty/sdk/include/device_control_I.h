#ifndef device_control_I_h__
#define device_control_I_h__
#ifndef __dllexport
#if defined(_MSC_VER)
#define __dllexport __declspec(dllexport)
#define __dllimport __declspec(dllimport)
#define __dllhidden
#elif defined(__MINGW32__) || defined(__SYGWIN__)
#define __dllexport __attribute__((dllexport))
#define __dllimport __attribute__((dllimport))
#define __dllhidden __attribute__((visibility("hidden")))
#else
#define __dllexport
#define __dllimport
#define __dllhidden
#endif
#endif
#include <string>
#include "ICD_LiDAR_API.h"
#include <vector>
const int g_heartPort = 2030;//心跳端口
const int g_fastPort = 2014; //快数据端口
const int g_msgPort = 2013;  //命令端口
const int g_slowPort = 2025; //慢数据端口
const int g_isfHeaderPort = 11000; //isf文件头接收端口
/// 设备信息
typedef struct DEVICE_MSG
{
	std::string interip; ///< 上位机网口IP
	std::string name;    ///< 设备名称 
	std::string ip;      ///<  设备IP
	std::string mac;     ///< 设备mac
}DEVICE_MSG_S;

class __dllexport DeviceControl_I
{
public:
	DeviceControl_I() {};
	virtual ~DeviceControl_I() {};

	/*
   * @brief 初始化所有参数
   *
   * @return bool 初始化是否成功
   */
	virtual bool init(const char* devIp, const char* localIp,
		int fastPort    = g_fastPort,  int slowPort = g_slowPort,
		int heartPort   = g_heartPort, int msgPort  = g_msgPort, 
		int isfHeadPort = g_isfHeaderPort) = 0;
	/*
  * @brief 开始工作
  *
  * @param cmd 开始工作命令
  * @param ip 设备ip
  */
	virtual void startWork(SCDCMD_PROGRM_S cmd) = 0;

	/*
  * @brief 停止工作
  *
  * @param ip 设备ip
  */
	virtual void stopWork() = 0;

	/**
	* 从设备接收快数据
	* @return uint32_t 0--执行成功  1--执行失败
	*/
	virtual uint32_t recvFastData() = 0;

	/**
	 * 从设备接收慢数据
	 * @return uint32_t 0--执行成功  1--执行失败
	 */
	virtual uint32_t recvSlowData() = 0;

	/*
    * @brief 获取快数据
    * @param pfast_data 快数据指针
    * @param pfast_lens 快数据长度
    * @return uint32_t 0--执行成功  1--执行失败
    */
	virtual uint32_t getFastData(unsigned char *pfast_data, int &pfast_lens) = 0;

	/*
	* @brief 获取慢数据
	* @param pslow_data 慢数据指针
	* @param pslow_len 慢数据长度
	* @return uint32_t 0--执行成功  1--执行失败
	*/
	virtual uint32_t getSlowData(unsigned char *pslow_data, int  &pslow_len) = 0;

	/*
	* @brief 释放快数据buffer
	* @param fast_lens 释放快数据长度
	* @return uint32_t 0--执行成功  1--执行失败
	*/
	virtual uint32_t freeFastBuffter(uint32_t fast_lens) = 0;

	/*
	* @brief 释放慢数据buffer
	* @param slow_lens 释放慢数据长度
	* @return uint32_t 0--执行成功  1--执行失败
	*/
	virtual uint32_t freeSlowBuffter(uint32_t slow_lens) = 0;

	/*
	* @brief 释放慢数据buffer
	* @param enable 是否保存isf文件
	* @param path   保存isf文件路径
	*/
	virtual void setWriteFileEnable(bool enable, std::string path = "") = 0;

	/*
	* @brief 设备是否已经开始工作
	*/
	virtual bool isListener() = 0;
	
	/*
	* @brief 设置接收数据状态
	* @param flg 是否可以接收数据
	*/
	virtual void setRevDataFlg(bool flg) = 0;

	/*
	* @brief 获取设备id
	*/
	virtual unsigned char getDeviceId() = 0;

	/*
	* @brief 获取设备协议
	*/
	virtual unsigned char getDevProto() = 0;

	/*
	* @brief 获取设备温度
	*/
	virtual float getTemperature() = 0;

	/*
	* @brief 获取isf文件头
	* @param ip  设备ip
	* @return head内容（256k or nullptr）
	*/
	virtual char* getIsfHead(char* ip) = 0;

	/*
	* @brief 获取lidarsvr版本号
	* @return 版本号(十六进制）
	*/
	virtual short getLidarsvrVersion() = 0;

	/*
	* @brief 更改接收数据的端口
	* @param fastPort 快数据端口
	* @param slowPort 慢数据端口
	* @return 返回0表示成功
	*/
	virtual int32_t changeDataPort(uint16_t fastPort, uint16_t slowPort) = 0;

	/*
	* @brief 获取设备信息
	* @param localIp 本地ip
	* @param deviceMsg_vec 获取到的设备信息列表
	* @return 返回0表示成功
	*/
	virtual uint32_t FindDeviceMsg(std::string localIp, std::vector<DEVICE_MSG_S>& deviceMsg_vec) = 0;

	/*
	* @brief 通过查询方式获取心跳包
	* @param heart 指向心跳包信息指针，需要提前分配256字节内存，内部做深拷贝
	* @return 返回0表示成功，!=0 表示查询失败属于老设备
	*/
	virtual uint32_t QueryHeartBeat(char* heart) = 0;

	/*
	* @brief 用openlidar查询设备的转速
	* @param scan_speed 设备扫描转速 
	* @return 返回0表示成功，!=0 表示查询失败属于老设备
	*/
	virtual uint32_t QueryScanSpeed(float& scan_speed) = 0;

};
__dllexport DeviceControl_I* getDeviceControl();


#endif // device_control_I_h__
