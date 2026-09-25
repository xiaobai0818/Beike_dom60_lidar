	/********************************************************************
 * $I
 * @Technic Support: <sdk@isurestar.com>
 * All right reserved, Sure-Star Coop.
 ********************************************************************/
#ifndef ICD_COMMON_H
#define ICD_COMMON_H

#include "ICD_LiDAR_API.h"

//#define TRUE 1
//#define FALSE 0
//#include <string.h>
//#include <stdlib.h>
//#include "publicfile.h"

#pragma pack(2)
typedef unsigned char	BYTE;
typedef unsigned char	uchar;
typedef unsigned char	uint8_t ;
typedef short			int_16;
typedef short			int16;
typedef unsigned short	uint_16;
typedef unsigned short	uint16_t ;
//typedef long           int_32;
//typedef long           int32;
typedef unsigned int	uint_32;
typedef unsigned int	uint32_t ;
typedef int             int_32;
typedef int				int32_t ;
//typedef unsigned long  uint_32;
typedef long  long		int_64;
typedef unsigned long long	uint_64;
//typedef long           time_ld;

const int ONEDAY_SECOND_COUND = 86400;  //!<
const int ONEHOUR_SECOND_COUND = 3600;  //!<
const int TWOHOUR_SECOND_COUND = 7200;  //!<
const int TRDHOUR_SECOND_COUND = 10800;  //!<
const int ONEMINUTE_SECOND_COUND = 60;  //!<
const int WEEK_SECOND_MAX = 604800;

const int IMP_HEADER_SIZE = 0x14000;    //IMP文件头大小 80KB
const int ISF_HEADER_SIZE = 0x40000;    //ISF文件头大小 256KB

const int TURNDATA_SIZE = 0xA00000;  // 10M Bytes
//const int SLOW_BUFF_SIZE  = 0x800000;	//慢数据buff大小 8M
//const int END_LEFT_SIZE   = 0xA00000;   //IMP文件头大小 131KB

//const int CHNL_OPEN_ID = 7 ;        //socket
//const int IMPFILE_CLOSE_COUNT = 50 ;

const int DATA_FILE_SIZE = 0x8000000; // 128 * 1024 * 1024 =128M Bytes
//const int USBDATA_FILE_SIZE = 0x40000000; // 128 * 1024 * 1024 =1GM Bytes
//const int SLOWDATA_SIZE = 0x100000;  // 1M Bytes
//const int SLOW_FILE_SIZE = 0x20000000;  // 512M Bytes

const unsigned int IMPHEAD_SYN_WORD = 0xA7A7A7A7;
const unsigned int IMG_SYN_WORD = 0xF9F9F9F9 ;
const unsigned int JPG_SYN_WORD = 0xFAFAFAFA ;
const unsigned int CMPIMG_SYN_WORD = 0xF8F8F8F8 ;

const int  LiDAR_RANGE_MAX = 600 ;     //!< 最大测距默认值：600m
const int  LiDAR_RANGE_MIN = 1 ;        //!< 最小测距默认值：1 m

const int FILE_PATH_LEN = 128; // Compatible with the POSIX standard
//const int FILE_NAME_LEN = 128; // file name exclude file path

typedef struct {
    int msgid;
    int hour, minute, second;
    int day_num, month, year;
    int julian;
    int seconds;   //zks(13/01/11): change time_t to int for standard
    int numBeats;  // count of heartbeat msgs
} DateMsg_S;


#define PPS_STAMP_DEFAULT (0x80000000) //max T0,2^31, 2147483648
#define PPS_DIFF_DEFAULT  (5000000)
#define UTC_STAMP_DEFAULT (0.0)

#define IMUSTAMP_DIFF_DEFAULT  (5000)
#define T0_ImuStamp_Check  (10)
typedef struct tag_PosPPS_S {
	unsigned int ppsStamp;
	int diffPpsStamp;
	double utcStamp;
	tag_PosPPS_S()
	{
		ppsStamp = PPS_STAMP_DEFAULT;
		diffPpsStamp = PPS_DIFF_DEFAULT;
		utcStamp = UTC_STAMP_DEFAULT;
	}
} PosPPS_S ;


//// below will be move to lidarAPI.h

const int MESSAGE_ARRAY_NUM = 64 ; 
const int COMMON_BUFFER_SIZE = 10000;

typedef struct {
  int wrHead, rdTail ;
  char msgArray[MESSAGE_ARRAY_NUM][COMMON_BUFFER_SIZE] ; //64 > sizeof(SCADA_CMDRPT_U)
} MESSG_RINGBUF_S ;

typedef struct {
  int wrHead, rdTail ;
  struct {
    char gpsMsg[COMMON_BUFFER_SIZE] ;
    unsigned short msgSize ;
  } msgArray [MESSAGE_ARRAY_NUM];
} GPSMSG_RINGBUF_S ;

#pragma pack()

/*******************zks 2022.03.13
static int getRevNbr(const char* svnRev) {
  char strNbr[10] = "0\0";
  unsigned int count = 0;
  unsigned int nbrFlag = 2;

  for(unsigned int i = 0; i < strlen(svnRev); i++) {
    if ( nbrFlag == 0 ) {
      if ( svnRev[i] == ' ') {
    strNbr[count] = '\0';
    break;
      }
      else  {
    strNbr[count++] = svnRev[i];
      }
    }
    else {
      if ( svnRev[i] == ' ')  {
    nbrFlag--;
      }
    }
  }
  return atoi(strNbr);
}
zks ************************************/

#endif
