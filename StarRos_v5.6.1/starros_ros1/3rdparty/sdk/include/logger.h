/**
* @file Logger.h
* @brief 日志信息类
* @details 主要包括：日志优先级定义、日志形式定义、以及该类的初始化和信息打印等。
* @author zhuqin
* @version 1.0.0
* @date 2023.10.23
* @copyright ISurstar software @2005-2023
*/

//#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

#ifdef _WIN32
#define EXPORTS
#include <cstdint>
#ifdef EXPORTS
#define EXPORT_API __declspec(dllexport)
#else 
#define EXPORT_API __declspec(dllimport)
#endif

#else
#include <stdint.h>
#define EXPORT_API
#endif 


#include <stdio.h>
#include <string>

//优先级，与Log4cpp中的定义一直
//取值越小，优先级越高。
//比如一个log的优先级为101，则全部EMERG FATAL ALERT日志都能够记录下来，其他则不能，即低于该级别的日志不被记录
typedef enum PRIORITYLEVEL
{
	LOG_FATAL = 0,		///<fatal（致命）
	LOG_ERROR = 300,	///<error（错误）
	LOG_WARN = 400,		///<warning（警告）
	LOG_INFO = 600,		///<info（消息）
	LOG_DEBUG = 700,	///<debug（调试）
}PRIORITYLEVEL_E;

//log信息打印的方式：即写到文件、或打印到终端
typedef enum LOGMODE
{
	File_Mode = 0,		///<log信息写入文件
	Console_Mode = 1	///<log信息打印到终端
}LOGMODE_E;

/**
* @file Logger类
* 负责日志信息的定义和打印
*/
class EXPORT_API Logger
{
public:

	/**
	* @brief Logger类的声明函数
	* @return 无
	*/
	Logger();
	~Logger();

	/**
	* @brief Logger类的初始化，如果设置为终端打印，最后三个参数可忽略
	* @param[in] type：logger信息的打印方式 File_Mode = 0，log信息写入文件；Console_Mode = 1，log信息打印到终端；
	* @param[in] level：log优先级级别，低于该级别的日志不被记录
	* @param[in] maxFileSize：单个文件大小，log4cpp隐含定义文件最小尺寸为200k，只有大于200k的设置才能生效，<=200k的设置都被认为200k 
	* @param[in] maxBackupIndex：文件个数，最终会生成maxBackupIndex+1个文件，如果设置为5，会生成6个文件，如test.log, test.log.1-test.log.5,test.log为最新
	* @param[in] log_file：logger信息打印到文件的文件名称，如type=File_mode, strlen(log_file)==0， 函数返回false，初始化失败
	* @retval false：初始化失败
	*		  true：初始化成功
	*/
	bool Init(std::string intance_name,  LOGMODE_E type, PRIORITYLEVEL_E level, const char* log_file, size_t maxFileSize, unsigned int maxBackupIndex);

	/**
	* @brief 日志打印
	* @param[in] log_level：当前日志的类别
	* @param[in] msgInfo：当前日志内容
	*/
	void PrintLog(PRIORITYLEVEL_E log_level, const char* msgInfo, ...);

	/**
	* @brief 日志Fatal打印
	* @param[in] msgInfo：当前日志内容
	*/
	void PrintFatal(const char* msgInfo, ...);

	/**
	* @brief 日志Error打印
	* @param[in] msgInfo：当前日志内容
	*/
	void PrintError(const char* msgInfo, ...);

	/**
	* @brief 日志Warning打印
	* @param[in] msgInfo：当前日志内容
	*/
	void PrintWarning(const char* msgInfo, ...);

	/**
	* @brief 日志info打印
	* @param[in] msgInfo：当前日志内容
	*/
	void PrintInfo(const char* msgInfo, ...);

	/**
	* @brief 日志debug打印
	* @param[in] msgInfo：当前日志内容
	*/
	void PrintDebug(const char* msgInfo, ...);

	/**
	* @brief 关掉所有的log4cpp相关对象 只在最上层软件退出时 调用一次，中间不可用调用 如调用 之前new的logger都不能再使用
	*/
	void Shutdown();

private:
	//含参的cha*转成std::string
	//代码实现参考log4cpp的源码
	std::string VForm(const char* format, va_list args);

private:
	//char*转string时用到
	char* m_buffer;

	void* m_log_category;
};

#endif // !__LOGGER_H__

/*

FATAL = 0,		///<fatal（致命）： fatal级别的日志用于记录严重的错误，通常表示应用程序遇到了无法继续运行的情况。可能会导致应用程序崩溃。
                                    这个级别的日志用于标识需要立即处理的严重问题。

ERROR = 300,	///<error（错误）：  error级别的日志用于记录错误情况，例如操作失败、异常抛出等。这些日志表明应用程序遇到了一些问题，但通常不糊导致应用程序崩溃。
                                     这个级别的日志在生产环境中非常重要，应及时发现和修复错误。

WARN = 400,		///<warning（警告）：warning级别的日志用于记录警告信息，例如潜在的问题或者不正常的情况，但不会导致应用程序终止。
                                     这个级别的日志可以帮助监控和排查问题。

INFO = 600,		///<info（消息）：   info级别的日志用于记录应用程序的一般信息，例如应用程序启动、关键事件、操作完成等。
                                     这个级别的日志对于了解应用程序的运行状态和性能是有用的，通常会在生产环境中启用。

DEBUG = 700,	///<debug（调试）：  debug级别的日志用于记录应用的调试信息，例如变量的值、函数的执行路径等。它有助于开发人员查找和修复问题。
                                     这个级别的日志 在开发和测试阶段启用，但在生产环境中通常禁用，因为它可能会产生大量的输出

*/
