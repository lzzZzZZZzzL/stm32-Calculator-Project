/*********************************************************************************************************
* ģ�����ƣ�EXTI.c
* ժ    Ҫ��EXTIģ��
* ��ǰ�汾��1.0.0
* ��    �ߣ�SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* ������ڣ�2020��01��01��
* ��    �ݣ�
* ע    �⣺                                                                  
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
#include "EXTI.h"
#include "stm32f10x_conf.h"

/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                              ö�ٽṹ�嶨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                              �ڲ�����
*********************************************************************************************************/

/*********************************************************************************************************
*                                              �ڲ���������
*********************************************************************************************************/
static void ConfigEXTIGPIO(void);   //����EXTI��GPIO 
static void ConfigEXTI(void);       //����EXTI

/*********************************************************************************************************
*                                              �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* �������ƣ�ConfigEXTIGPIO
* �������ܣ�����EXTI��GPIO 
* ���������void 
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
static void ConfigEXTIGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  //GPIO_InitStructure���ڴ��GPIO�Ĳ���
                                                                     
  //ʹ��RCC���ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��GPIOC��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOA��ʱ��
  
  //����PC1
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;           //��������
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //������������
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //���ݲ�����ʼ��GPIO
                                                                                          
  //����PC2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;           //��������
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //������������
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //���ݲ�����ʼ��GPIO
                                                                                       
  //����PA0
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;           //��������
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //������������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //���ݲ�����ʼ��GPIO
}

/*********************************************************************************************************
* �������ƣ�ConfigEXTI
* �������ܣ�����EXTI
* ���������void 
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
static void ConfigEXTI(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;  //EXTI_InitStructure���ڴ��EXTI�Ĳ���
  NVIC_InitTypeDef NVIC_InitStructure;  //NVIC_InitStructure���ڴ��NVIC�Ĳ���
  
  //ʹ��RCC���ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //ʹ��AFIO��ʱ��
  
  //����PC1��EXTI��NVIC 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);   //ѡ��������Ϊ�ж���
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;                    //ѡ���ж���
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�����ж�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //����Ϊ�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ���ж��� 
  EXTI_Init(&EXTI_InitStructure);                               //���ݲ�����ʼ��EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;              //�ж�ͨ����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //������ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //���������ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�
  NVIC_Init(&NVIC_InitStructure);                               //���ݲ�����ʼ��NVIC
  
  //����PC2��EXTI��NVIC  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);   //ѡ��������Ϊ�ж���
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;                    //ѡ���ж���
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�����ж�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //����Ϊ�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ���ж��� 
  EXTI_Init(&EXTI_InitStructure);                               //���ݲ�����ʼ��EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //�ж�ͨ����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //������ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //���������ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�
  NVIC_Init(&NVIC_InitStructure);                               //���ݲ�����ʼ��NVIC
  
  //����PA0��EXTI��NVIC
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);   //ѡ��������Ϊ�ж���
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                    //ѡ���ж���
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�����ж�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //����Ϊ�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ���ж��� 
  EXTI_Init(&EXTI_InitStructure);                               //���ݲ�����ʼ��EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //�ж�ͨ����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //������ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //���������ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�
  NVIC_Init(&NVIC_InitStructure);                               //���ݲ�����ʼ��NVIC
}

/*********************************************************************************************************
* �������ƣ�EXTI0_IRQHandler
* �������ܣ�EXTI2���жϷ���������ӦKEY3
* ���������void 
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
//void EXTI0_IRQHandler(void) // KEY3 ����
//{
//  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
//  {
//    char clearBuffer[]="Clear!";
//    inputIndex = 0;
//    result = 0;
//    WriteUART1((u8*)clearBuffer, strlen(clearBuffer));
//    OLEDClear();
//    OLEDShowString(0, 16, "please input");
//    OLEDShowString(36, 48, "By Lzy"); 
//    EXTI_ClearITPendingBit(EXTI_Line0);
//  }
//}

/*********************************************************************************************************
* �������ƣ�EXTI1_IRQHandler
* �������ܣ�EXTI2���жϷ���������ӦKEY1
* ���������void 
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
//void EXTI1_IRQHandler(void)// KEY1 ������
//{
//  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
//  {
//    inputExpr[inputIndex] = '\0';
//    float result = EvaluateExpression(inputExpr);
//    snprintf(UARToutputBuffer, sizeof(UARToutputBuffer), "Result:%f", result);
//    snprintf(OLEDoutputBuffer, sizeof(OLEDoutputBuffer), "=%f", result);
//    WriteUART1((u8*)UARToutputBuffer, strlen(UARToutputBuffer));
//    EXTI_ClearITPendingBit(EXTI_Line1);
//  }
//}

/*********************************************************************************************************
* �������ƣ�EXTI2_IRQHandler
* �������ܣ�EXTI2���жϷ���������ӦKEY2
* ���������void 
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��01��01��
* ע    �⣺
*********************************************************************************************************/
//void EXTI2_IRQHandler(void) // KEY2 ������
//{
//  if (EXTI_GetITStatus(EXTI_Line2) != RESET)
//  {
//    if (inputIndex == 0 || inputExpr[0] == '\0')
//    {
//      snprintf(OLEDoutputBuffer, sizeof(OLEDoutputBuffer), "no input");
//      WriteUART1((u8*)OLEDoutputBuffer, strlen(OLEDoutputBuffer));
//      OLEDClear();
//      OLEDShowString(0, 16, (u8*)OLEDoutputBuffer);
//    }
//    else
//    {
//      //snprintf(OLEDoutputBuffer, sizeof(OLEDoutputBuffer), "=%f", result);
//      OLEDClear();
//      inputExpr[inputIndex-1] = ' ';
//      OLEDShowString(0, 16, (u8*)inputExpr);
//      OLEDShowString(0, 48, (u8*)OLEDoutputBuffer);
//    }
//    OLEDRefreshGRAM();
//    EXTI_ClearITPendingBit(EXTI_Line2);
//  }
//}

/*********************************************************************************************************
*                                              API����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* �������ƣ�InitEXTI
* �������ܣ���ʼ��EXTIģ��
* ���������void 
* ���������void
* �� �� ֵ��void
* �������ڣ�2018��1��1��
* ע    �⣺
*********************************************************************************************************/
void  InitEXTI(void)
{
  ConfigEXTIGPIO(); //����EXTI��GPIO
  ConfigEXTI();     //����EXTI
}
