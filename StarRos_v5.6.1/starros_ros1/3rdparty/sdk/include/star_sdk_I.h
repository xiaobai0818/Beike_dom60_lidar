#ifndef star_sdk_I_h__
#define star_sdk_I_h__
#include "lidar_comm.h"
#include "Ilidar_info.h"
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
#include <vector>
#include <functional>

class __dllexport StarSDK_I
{
public:
	StarSDK_I() {};
	virtual ~StarSDK_I() {};

   /**
   * @brief 初始化函数，用结构体传参方式对解码解算初始化
   * 
   * @param config 解码解算用的配置参数
   * @param dataframeCallback 回调注册函数 解码到一帧数据后调用
   * @param is_async 异步/同步回调函数标志
   * @param deviceId 设备id 获取到id号后才可以init操作
   * @param isfHead 文件头信息
   * @return bool 初始化是否成功
   */
	virtual bool init(ConfigParam_S* config, const FrameCallback & dataframeCallback, 
		bool is_async = false, int16_t deviceId = -1, char* isfHead = nullptr) = 0;

   /**
   * @brief 初始化函数，通过读取配置文件方式对解码解算初始化
   *
   * @param path 配置文件的绝对路径
   * @param dataframeCallback 回调注册函数 解码到一帧数据后调用
   * @param is_async 异步/同步回调函数标志
   * @param deviceId 设备id 获取到id号后才可以init操作
   * @return bool 初始化是否成功
   */
   virtual bool init(std::string path, const FrameCallback& dataframeCallback,
	   bool is_async = false, int16_t deviceId = -1, char* isfHead = nullptr) = 0;

   /**
  * @brief 初始化函数，用结构体传参方式对解码解算初始化
  *
  * @param config 解码解算用的配置参数
  * @param dataframeCallback 回调注册函数 解码到一帧数据后调用
  * @param deviceId 设备id 获取到id号后才可以init操作
  * @param isfHead 文件头信息
  * @return bool 初始化是否成功
  */
   virtual bool init(ConfigParam_S* config, const std::function<int32_t(L1DataFrame_S&)>& dataframeCallback,
	   int16_t deviceId = -1, char* isfHead = nullptr) = 0;

   /**
   * @brief 初始化函数，通过读取配置文件方式对解码解算初始化
   *
   * @param path 配置文件的绝对路径
   * @param dataframeCallback 回调注册函数 解码到一帧数据后调用
   * @param deviceId 设备id 获取到id号后才可以init操作
   * @return bool 初始化是否成功
   */
   virtual bool init(std::string path, const std::function<int32_t(L1DataFrame_S&)>& dataframeCallback,
	  int16_t deviceId = -1, char* isfHead = nullptr) = 0;

   /**
	* @brief 静态解码运行接口
	*
	* @param param   解算方式 默认按配置文件中参数
	*/
   virtual void runDec(DecParam param = DecParam::eDefault) = 0;

	/**
	* @brief 动态解码运行接口
	*
	* @param fast_data   快数据地址指针
	* @param data_size   快数据长度
	* @param temperature 设备温度
	* @return int32_t >0 表示已经解码快数据长度 -1表示停止运行
	*/
	virtual int32_t runDec(char* fast_data, int32_t data_size, float temperature) = 0;



   /*
   * @brief 重置解码
   */
	//virtual void reset() = 0;

   /**
    * @brief 回收点云内存块，仅在异步回调时使用，同步回调时内部处理
    * 
    * @param data_frame 点云数据帧
    * @return int32_t 
    */
   virtual int32_t freeDataFrame(L1DataFrame_S* data_frame) = 0;


   ///////////静态处理//////////////
   /**
	* @brief 设置imu获取的buf
	*
	* @param vec  总imu数据的存储buf
	*/
   virtual void setSlowDateBuf_imu(std::vector<L1IMU_S>* vec) = 0;

   /**
   * @brief 设置转台获取的buf
   *
   * @param vec  总转台数据的存储buf
   */
   virtual void setSlowDateBuf_turn(std::vector<TurnData_S>* vec) = 0;

   virtual int32_t runDecSlow(char * slow_data, int32_t data_size) = 0;
   
   /**
   * @brief 获取imu数据
   *
   * @return std::vector<L1IMU_S>&
   */
   virtual std::vector<L1IMU_S>& getImuArray() = 0;
   
   /**
   * @brief 获取回放数据的进度
   *
   * @param curFileNum   回放的数据当前下标
   * @param totalFileNum 回放数据的总个数
   */
   virtual void getDecProgress(int32_t& curFileIndex, int32_t& totalFileNum) = 0;

   /**
	 * @brief 停止解算文件
   */
   virtual void stopCalc() = 0;
   
   /**
	* @brief 设置原始数据路径
   * @param path   原始数据路径
   */
   virtual void setRawDataPath(std::string path) = 0;

   /**
   * @brief 设置输出文件路径
   * @param path   输出文件路径
   */
   virtual void setOutDataPath(std::string path) = 0;

   /**
   * @brief 开启输出文件开关
   * @param name   输出文件名
   */
   virtual void openOutFile(std::string name = "") = 0;

   /**
   * @brief 关闭输出文件开关
   */
   virtual void closeOutFile() = 0;
};

__dllexport StarSDK_I *getStarSDK(DecType type = DecType::eDynamicType);

#endif // star_sdk_I_h__
