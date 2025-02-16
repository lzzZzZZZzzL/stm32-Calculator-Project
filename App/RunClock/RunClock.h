/*********************************************************************************************************
* 模块名称：RunClock.h
* 摘    要：RunClock模块
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日 
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/
#ifndef _RUN_CLOCK_H_
#define _RUN_CLOCK_H_

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "DataType.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/
//定义枚举
typedef enum  
{
  TIME_VAL_HOUR = 0,
  TIME_VAL_MIN,
  TIME_VAL_SEC,
  TIME_VAL_MAX
}EnumTimeVal;

/*********************************************************************************************************
*                                              API函数声明
*********************************************************************************************************/
void  InitRunClock(void);                   //初始化RunClock模块

void  RunClockPer2Ms(void);                 //每2ms调用一次
void  PauseClock(u8 flag);                  //flag，TRUE-暂停，FALSE-正常运行

i16   GetTimeVal(u8 type);                  //获取时间值
void  SetTimeVal(u8 type, i16 timeVal);     //设置时间值

void  DispTime(i16 hour, i16 min, i16 sec); //显示当前的时间
void  OLEDDispTime(i16 hour, i16 min, i16 sec,char *str);

#endif


