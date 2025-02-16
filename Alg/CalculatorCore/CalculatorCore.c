/*********************************************************************************************************
* 模块名称：CalculatorCore.h
* 摘    要：计算器核心模块
* 当前版本：1.0.0
* 作    者：Lzy
* 完成日期：
* 内    容：
* 注    意：                                                                  
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
#include "CalculatorCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ApplyOperator
* 函数功能：单个运算符运算
* 输入参数：
*   a - 第一个操作数
*   b - 第二个操作数
*   op - 运算符
* 输出参数：无
* 返 回 值：运算结果
* 创建日期：
* 注    意：
*********************************************************************************************************/
static float ApplyOperator(float a, float b, char op)
{
  if (op == '/' && b == 0)// 检查是否除以零
  {
    printf("Error: Division by zero!\r\n");
    return 0;
  }
  switch (op)
  {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    default: return 0;
  }
}

/*********************************************************************************************************
* 函数名称：IsOperator
* 函数功能：判断是否运算符
* 输入参数：
*   c - 待判断的字符
* 输出参数：无
* 返 回 值：
*   1 - 是运算符
*   0 - 不是运算符
* 创建日期：
* 注    意：
*********************************************************************************************************/
static u8 IsOperator(char c)
{
  return (c == '+' || c == '-' || c == '*' || c == '/');
}

/*********************************************************************************************************
* 函数名称：GetPrecedence
* 函数功能：检查运算符优先级
* 输入参数：op - 运算符
* 输出参数：无
* 返 回 值：优先级值（1 或 2）
* 创建日期：
* 注    意：
*********************************************************************************************************/
static u8 GetPrecedence(char op)
{
  if (op == '+' || op == '-')
  {
    return 1;
  }
  if (op == '*' || op == '/')
  {
    return 2;
  }
  return 0;
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：EvaluateExpression
* 函数功能：逆波兰，计算结果
* 输入参数：expr - 表达式字符串
* 输出参数：无
* 返 回 值：计算结果
* 创建日期：
* 注    意：
*********************************************************************************************************/
float EvaluateExpression(const char* expr)
{
  StructStack numStack, opStack; // 数字栈和运算符栈
  STACK_TYPE numBuf[MAX_EXPR_SIZE], opBuf[MAX_EXPR_SIZE];// 栈缓冲区
  InitStack(&numStack, numBuf, MAX_EXPR_SIZE);// 初始化栈
  InitStack(&opStack, opBuf, MAX_EXPR_SIZE);

  i16 i = 0;// 表达式索引
  while (expr[i] != '\0')// 遍历表达式
  {
    if (isspace(expr[i]))// 跳过空格
    {
      i++;
      continue;
    }
    
    if (isdigit(expr[i]) || (expr[i] == '-' && (i == 0 || IsOperator(expr[i - 1]))))// 解析数字
    {
      char numStr[16] = {0};// 临时存储数字字符串
      i16 numIndex = 0;// 数字字符串索引
      u8 hasDecimal = 0;// 是否有小数点

      if (expr[i] == '-')// 处理负数
      {
        numStr[numIndex++] = expr[i++];
      }

      while (isdigit(expr[i]) || (!hasDecimal && expr[i] == '.')) // 解析数字和小数点
      {
        if (expr[i] == '.')
        {
          hasDecimal = 1;// 标记小数点
        }
        numStr[numIndex++] = expr[i++];// 将字符加入数字字符串
      }
      numStr[numIndex] = '\0';// 终止字符串

      PushStack(&numStack, atof(numStr));// 将解析的数字压入数字栈
    } 
    else if (expr[i] == '(')// 遇到左括号
    {
      PushStack(&opStack, expr[i]);// 将左括号压入运算符栈
      i++;
    } 
    else if (expr[i] == ')')// 遇到右括号
    {
      STACK_TYPE op;// 运算符
      while (!StackEmpty(&opStack) && PeekStack(&opStack, &op) && op != '(')// 弹出运算符栈中的运算符并执行运算
      {
        PopStack(&opStack, &op);// 弹出运算符
        STACK_TYPE b, a;
        PopStack(&numStack, &b);// 弹出两个操作数
        PopStack(&numStack, &a);
        PushStack(&numStack, ApplyOperator(a, b, op));// 执行运算并将结果压入数字栈
      }
      PopStack(&opStack, &op);// 弹出左括号
      i++;
    } 
    else if (IsOperator(expr[i]))// 遇到运算符
    {
      STACK_TYPE op;
      while (!StackEmpty(&opStack) && PeekStack(&opStack, &op) && GetPrecedence(op) >= GetPrecedence(expr[i]))// 比较优先级并执行运算
      {
        PopStack(&opStack, &op);// 弹出运算符
        STACK_TYPE b, a;
        PopStack(&numStack, &b);// 弹出两个操作数
        PopStack(&numStack, &a);
        PushStack(&numStack, ApplyOperator(a, b, op));// 执行运算并将结果压入数字栈
      }
      PushStack(&opStack, expr[i]);// 将当前运算符压入运算符栈
      i++;
    }
  }

  while (!StackEmpty(&opStack))// 处理最后的运算
  {
    STACK_TYPE op, b, a;
    PopStack(&opStack, &op);// 弹出运算符
    PopStack(&numStack, &b);// 弹出两个操作数
    PopStack(&numStack, &a);
    PushStack(&numStack, ApplyOperator(a, b, op));// 执行运算并将结果压入数字栈
  }

  STACK_TYPE result;
  PopStack(&numStack, &result);// 弹出最终结果
  return result;
}
