/*********************************************************************************************************
* 模块名称：Main.c
* 摘    要：主文件，包含软硬件初始化函数和main函数
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日
* 内    容：
* 注    意：注意勾选Options for Target 'Target1'->Code Generation->Use MicroLIB，否则printf无法使用                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "NVIC.h"
#include "SysTick.h"
#include "RCC.h"
#include "Timer.h"
#include "UART1.h"
#include "LED.h"
#include "OLED.h"
#include "RunClock.h"
#include "CalculatorCore.h"
#include "EXTI.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define MAX_INPUT_SIZE 32


static char inputExpr[MAX_INPUT_SIZE];
static char UARToutputBuffer[16];
static char OLEDoutputBuffer[16];
static u8 inputIndex = 0;
static float result = 0;
/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  InitSoftware(void);   //初始化软件相关的模块
static  void  InitHardware(void);   //初始化硬件相关的模块
static  void  Proc2msTask(void);    //2ms处理任务
static  void  Proc1SecTask(void);   //1s处理任务

extern size_t strlen(const char *);

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitSoftware
* 函数功能：所有的软件相关的模块初始化函数都放在此函数中
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  InitSoftware(void)
{
  InitRunClock();   //初始化RunClock模块
}

/*********************************************************************************************************
* 函数名称：InitHardware
* 函数功能：所有的硬件相关的模块初始化函数都放在此函数中
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  InitHardware(void)
{  
  SystemInit();       //系统初始化
  InitRCC();          //初始化RCC模块
  InitNVIC();         //初始化NVIC模块
  InitUART1(115200);  //初始化UART模块
  InitTimer();        //初始化Timer模块
  InitLED();          //初始化LED模块
  InitSysTick();      //初始化SysTick模块
  InitOLED();         //初始化OLED模块
  InitEXTI();         //初始化EXTI模块
}

/*********************************************************************************************************
* 函数名称：Proc2msTask
* 函数功能：2ms处理任务 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void EXTI1_IRQHandler(void)// KEY1 计算结果
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    inputExpr[inputIndex] = '\0';
    float result = EvaluateExpression(inputExpr);
    snprintf(UARToutputBuffer, sizeof(UARToutputBuffer), "Result:%f", result);
    snprintf(OLEDoutputBuffer, sizeof(OLEDoutputBuffer), "=%f", result);
    WriteUART1((u8*)UARToutputBuffer, strlen(UARToutputBuffer));
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

void EXTI2_IRQHandler(void) // KEY2 输出结果
{
  if (EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    if (inputIndex == 0 || inputExpr[0] == '\0')
    {
      snprintf(OLEDoutputBuffer, sizeof(OLEDoutputBuffer), "no input");
      WriteUART1((u8*)OLEDoutputBuffer, strlen(OLEDoutputBuffer));
      OLEDClear();
      OLEDShowString(0, 16, (u8*)OLEDoutputBuffer);
    }
    else
    {
      //snprintf(OLEDoutputBuffer, sizeof(OLEDoutputBuffer), "=%f", result);
      OLEDClear();
      inputExpr[inputIndex-1] = ' ';
      OLEDShowString(0, 16, (u8*)inputExpr);
      OLEDShowString(0, 48, (u8*)OLEDoutputBuffer);
    }
    OLEDRefreshGRAM();
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

void EXTI0_IRQHandler(void) // KEY3 清零
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    char clearBuffer[]="Clear!";
    inputIndex = 0;
    result = 0;
    WriteUART1((u8*)clearBuffer, strlen(clearBuffer));
    OLEDClear();
    OLEDShowString(0, 16, "please input");
    OLEDShowString(36, 48, "By Lzy"); 
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


static void Proc2msTask(void)
{
  u8 recData;
  if (Get2msFlag()) //判断2ms标志状态
  {
    RunClockPer2Ms();
    LEDFlicker(250);
    while (ReadUART1(&recData, 1))
    {
      if (recData == '\r' || inputIndex >= MAX_INPUT_SIZE - 1)
      {
        inputExpr[inputIndex] = '\0';
      }
      else
      {
        inputExpr[inputIndex++] = recData;
      }
    }
    Clr2msFlag();  //清除2ms标志
  }
}


/*********************************************************************************************************
* 函数名称：Proc1SecTask
* 函数功能：1s处理任务 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  Proc1SecTask(void)
{ 
  i16 hour;
  i16 min;
  i16 sec;
  char str[10];
   
  if(Get1SecFlag()) //判断1s标志状态
  {
    hour = GetTimeVal(TIME_VAL_HOUR);
    min  = GetTimeVal(TIME_VAL_MIN);
    sec  = GetTimeVal(TIME_VAL_SEC);
    
    OLEDDispTime(hour, min, sec, str);
    
    //OLEDShowString(16, 0, "2024-10-19"); 
    OLEDShowString(16, 0, (u8*)str);
    //OLEDShowString(24, 48, "By Lzy"); 
    
    //DispTime(hour, min, sec);
    
    OLEDRefreshGRAM();
    Clr1SecFlag();  //清除1s标志
  }    
}

/*********************************************************************************************************
* 函数名称：main
* 函数功能：主函数 
* 输入参数：void
* 输出参数：void
* 返 回 值：int
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
int main(void)
{ 
  InitSoftware();   //初始化软件相关函数
  InitHardware();   //初始化硬件相关函数
  
  printf("Init System has been finished.\r\n" );  //打印系统状态
  printf("KEY1为计算当前算式\r\n");
  printf("KEY2为显示计算结果\r\n");
  printf("KEY3为清零\r\n");
  printf("请从串口输入算式：\r\n");
  
  PauseClock(FALSE);
  
  SetTimeVal(TIME_VAL_HOUR,0);
  SetTimeVal(TIME_VAL_MIN,0);
  SetTimeVal(TIME_VAL_SEC,0);
  
  OLEDShowString(0, 16, "please input");
  OLEDShowString(36, 48, "By Lzy"); 
  
  while(1)
  {
    Proc2msTask();  //2ms处理任务
    Proc1SecTask(); //1s处理任务   
  }
}
