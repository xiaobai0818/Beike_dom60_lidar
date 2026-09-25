#ifndef _ISF_HEADER_H_
#define _ISF_HEADER_H_

#include "ICD_Common.h"

#define NAV_PAPA_NUM 0xFFFF
#define CTRL_PAPA_NUM 0x6000
#define CALC_PAPA_NUM 0xA000

typedef union _IsfHeader 
{ //IsfHeader
	struct 
	{
		//0x0000 - 0x5FFF
		union 
		{
			struct 
			{
				unsigned int fpgaVer;
                /// isf-6.0  PS
				unsigned int dataVer;    
                /// 数据级别 0/1/2/3
				unsigned int dataGrade;  
                /// 设备类型
				unsigned short deviceID;   
                /// 设备协议
				unsigned short deviceProto; 
                /// lidar激光器点频 khz
				unsigned int laserFreqKHz;  
                /// 扫描电机转速 hz
                float scanspeed; 
                /// 配置参数是否在isf文件头中 0 - 参数不在头需要额外读revise文件, 1 - 参数在文件头中
                unsigned int paramInHead; 
			};
			float fpgaPara[CTRL_PAPA_NUM];
		};
		//0x6000 - 0xFFFF
		float lidarPara[CALC_PAPA_NUM];
	};
	float header[NAV_PAPA_NUM];
}  Isf_Header_S;


#endif
