/*********************************************************************************************************
* ģ�����ƣ�Main.c
* ժ    Ҫ�����ļ���������Ӳ����ʼ��������main����
* ��ǰ�汾��1.0.0
* ��    �ߣ�SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* ������ڣ�2020��01��01��
* ��    �ݣ�
* ע    �⣺ע�⹴ѡOptions for Target 'Target1'->Code Generation->Use MicroLIB������printf�޷�ʹ��                                                                  
**********************************************************************************************************
* ȡ���汾��
* ��    �ߣ�
* ������ڣ�
* �޸����ݣ�
* �޸��ļ���
*********************************************************************************************************/

/*********************************************************************************************************
*                                              ����ͷ�ļ�
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
*                                              �궨��
*********************************************************************************************************/
#define MAX_INPUT_SIZE 32


static char inputExpr[MAX_INPUT_SIZE];
static char UARToutputBuffer[16];
static char OLEDoutputBuffer[16];
static u8 inputIndex = 0;
static float result = 0;
/*********************************************************************************************************
*                                              �ڲ�����
*********************************************************************************************************/

/*********************************************************************************************************
*                                              ö�ٽṹ�嶨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                              �ڲ���������
*********************************************************************************************************/
static  void  InitSoftware(void);   //��ʼ�������ص�ģ��
static  void  InitHardware(void);   //��ʼ��Ӳ����ص�ģ��
static  void  Proc2msTask(void);    //2ms��������
static  void  Proc1SecTask(void);   //1s��������

extern size_t strlen(const char *);

/*********************************************************************************************************
*                                              �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* �������ƣ�InitSoftware
* �������ܣ����е������ص�ģ���ʼ�����������ڴ˺�����
* ���������void
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
static  void  InitSoftware(void)
{
  InitRunClock();   //��ʼ��RunClockģ��
}

/*********************************************************************************************************
* �������ƣ�InitHardware
* �������ܣ����е�Ӳ����ص�ģ���ʼ�����������ڴ˺�����
* ���������void
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
static  void  InitHardware(void)
{  
  SystemInit();       //ϵͳ��ʼ��
  InitRCC();          //��ʼ��RCCģ��
  InitNVIC();         //��ʼ��NVICģ��
  InitUART1(115200);  //��ʼ��UARTģ��
  InitTimer();        //��ʼ��Timerģ��
  InitLED();          //��ʼ��LEDģ��
  InitSysTick();      //��ʼ��SysTickģ��
  InitOLED();         //��ʼ��OLEDģ��
  InitEXTI();         //��ʼ��EXTIģ��
}

/*********************************************************************************************************
* �������ƣ�Proc2msTask
* �������ܣ�2ms�������� 
* ���������void
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
void EXTI1_IRQHandler(void)// KEY1 ������
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

void EXTI2_IRQHandler(void) // KEY2 ������
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

void EXTI0_IRQHandler(void) // KEY3 ����
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
  if (Get2msFlag()) //�ж�2ms��־״̬
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
    Clr2msFlag();  //���2ms��־
  }
}


/*********************************************************************************************************
* �������ƣ�Proc1SecTask
* �������ܣ�1s�������� 
* ���������void
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
static  void  Proc1SecTask(void)
{ 
  i16 hour;
  i16 min;
  i16 sec;
  char str[10];
   
  if(Get1SecFlag()) //�ж�1s��־״̬
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
    Clr1SecFlag();  //���1s��־
  }    
}

/*********************************************************************************************************
* �������ƣ�main
* �������ܣ������� 
* ���������void
* ���������void
* �� �� ֵ��int
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
int main(void)
{ 
  InitSoftware();   //��ʼ�������غ���
  InitHardware();   //��ʼ��Ӳ����غ���
  
  printf("Init System has been finished.\r\n" );  //��ӡϵͳ״̬
  printf("KEY1Ϊ���㵱ǰ��ʽ\r\n");
  printf("KEY2Ϊ��ʾ������\r\n");
  printf("KEY3Ϊ����\r\n");
  printf("��Ӵ���������ʽ��\r\n");
  
  PauseClock(FALSE);
  
  SetTimeVal(TIME_VAL_HOUR,0);
  SetTimeVal(TIME_VAL_MIN,0);
  SetTimeVal(TIME_VAL_SEC,0);
  
  OLEDShowString(0, 16, "please input");
  OLEDShowString(36, 48, "By Lzy"); 
  
  while(1)
  {
    Proc2msTask();  //2ms��������
    Proc1SecTask(); //1s��������   
  }
}
