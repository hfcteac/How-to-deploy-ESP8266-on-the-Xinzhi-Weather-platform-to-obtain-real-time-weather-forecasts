/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <oled.h>
#include "dht11.h"
#include <stdio.h>
#include <stdlib.h>
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}

int fgetc(FILE *f)
{
	uint8_t ch = 0;
	HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
	return ch;
}
u32 cal_map(u32 x, u32 in_min, u32 in_max, u32 out_min, u32 out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

void StepMotor_AA(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_RESET);
}
 
void StepMotor_AB(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_RESET);
}
 
void StepMotor_BB(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_RESET);
}
 
void StepMotor_BC(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_RESET);
}
 
void StepMotor_CC(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_RESET);
}
 
void StepMotor_CD(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_SET);
}
 
void StepMotor_DD(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_SET);
}
 
void StepMotor_DA(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_11 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_1 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_0 ,GPIO_PIN_SET);
}
 
void StepMotor_Stop(void)
{
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}

//////////////*                   */
void StepMotor_AA2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}
 
void StepMotor_AB2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}
 
void StepMotor_BB2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}
 
void StepMotor_BC2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}
 
void StepMotor_CC2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}
 
void StepMotor_CD2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_SET);
}
 
void StepMotor_DD2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_SET);
}
 
void StepMotor_DA2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_SET);
}
 
void StepMotor_Stop2(void)
{
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_7 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_5 ,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4 ,GPIO_PIN_RESET);
}
// 定义一个简单的函数
void targetFunctionopen(int callNumber) {
        StepMotor_AA( );
        HAL_Delay(1);
        StepMotor_AB( );
        HAL_Delay(1);
        StepMotor_BB( );
        HAL_Delay(1);
        StepMotor_BC( );
        HAL_Delay(1);
        StepMotor_CC( );
        HAL_Delay(1);
        StepMotor_CD( );
        HAL_Delay(1);
        StepMotor_DD( );
        HAL_Delay(1);
        StepMotor_DA( );
        HAL_Delay(1);
}
void targetFunctionclose(int callNumber) {
        StepMotor_AA( );
        HAL_Delay(1);
        StepMotor_DA( );
        HAL_Delay(1);
        StepMotor_DD( );
        HAL_Delay(1);
        StepMotor_CD( );
        HAL_Delay(1);
        StepMotor_CC( );
        HAL_Delay(1);
        StepMotor_BC( );
        HAL_Delay(1);
        StepMotor_BB( );
        HAL_Delay(1);
        StepMotor_AB( );
        HAL_Delay(1);
}
void targetFunctionopen2(int callNumber) {
        StepMotor_AA2( );
        HAL_Delay(1);
        StepMotor_AB2( );
        HAL_Delay(1);
        StepMotor_BB2( );
        HAL_Delay(1);
        StepMotor_BC2( );
        HAL_Delay(1);
        StepMotor_CC2( );
        HAL_Delay(1);
        StepMotor_CD2( );
        HAL_Delay(1);
        StepMotor_DD2( );
        HAL_Delay(1);
        StepMotor_DA2( );
        HAL_Delay(1);
}
void targetFunctionclose2(int callNumber) {
        StepMotor_AA2( );
        HAL_Delay(1);
        StepMotor_DA2( );
        HAL_Delay(1);
        StepMotor_DD2( );
        HAL_Delay(1);
        StepMotor_CD2( );
        HAL_Delay(1);
        StepMotor_CC2( );
        HAL_Delay(1);
        StepMotor_BC2( );
        HAL_Delay(1);
        StepMotor_BB2( );
        HAL_Delay(1);
        StepMotor_AB2( );
        HAL_Delay(1);
}
void WhetherReport(int x,int y,int wh)//天气预报显示（最大x从48开始，否则数组越界）
{
    switch(wh)
    {
      case 0: OLED_ShowWte(x,y,0); OLED_ShowWte(x+16,y,1); OLED_ShowWte(x+32,y,4); OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//白天晴
      case 1: OLED_ShowWte(x,y,2); OLED_ShowWte(x+16,y,3); OLED_ShowWte(x+32,y,4); OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//夜晚晴
      case 2: OLED_ShowWte(x,y,0); OLED_ShowWte(x+16,y,1); OLED_ShowWte(x+32,y,4); OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//国网白天
      case 3: OLED_ShowWte(x,y,2); OLED_ShowWte(x+16,y,3); OLED_ShowWte(x+32,y,4); OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//国外夜晚
      case 4: OLED_ShowWte(x,y,7); OLED_ShowWte(x+16,y,8); OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//多云
      case 5: OLED_ShowWte(x,y,4); OLED_ShowWte(x+16,y,5); OLED_ShowWte(x+32,y,6); OLED_ShowWte(x+48,y,7); OLED_ShowWte(x+64,y,8); break;//晴有时多云
      case 6: OLED_ShowWte(x,y,4); OLED_ShowWte(x+16,y,5); OLED_ShowWte(x+32,y,6); OLED_ShowWte(x+48,y,7); OLED_ShowWte(x+64,y,8); break;//夜晚晴有时多云
      case 7: OLED_ShowWte(x,y,9); OLED_ShowWte(x+16,y,10);OLED_ShowWte(x+32,y,7); OLED_ShowWte(x+48,y,8); OLED_ShowWte(x+64,y,50);break;//大部多云
      case 8: OLED_ShowWte(x,y,9); OLED_ShowWte(x+16,y,10);OLED_ShowWte(x+32,y,7); OLED_ShowWte(x+48,y,8); OLED_ShowWte(x+64,y,50);break;//夜晚大部多云
      case 9: OLED_ShowWte(x,y,11);OLED_ShowWte(x+16,y,50);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//阴
      case 10:OLED_ShowWte(x,y,12);OLED_ShowWte(x+16,y,13);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//阵雨
      case 11:OLED_ShowWte(x,y,14);OLED_ShowWte(x+16,y,15);OLED_ShowWte(x+32,y,16);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//雷阵雨
      case 12:OLED_ShowWte(x,y,14);OLED_ShowWte(x+16,y,15);OLED_ShowWte(x+32,y,16);OLED_ShowWte(x+48,y,17);OLED_ShowWte(x+64,y,18);break;//雷阵雨冰雹
      case 13:OLED_ShowWte(x,y,19);OLED_ShowWte(x+16,y,20);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//小雨
      case 14:OLED_ShowWte(x,y,21);OLED_ShowWte(x+16,y,20);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//中雨
      case 15:OLED_ShowWte(x,y,22);OLED_ShowWte(x+16,y,20);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//大雨
      case 16:OLED_ShowWte(x,y,23);OLED_ShowWte(x+16,y,20);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//暴雨
      case 17:OLED_ShowWte(x,y,22);OLED_ShowWte(x+16,y,23);OLED_ShowWte(x+32,y,20);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//大暴雨
      case 18:OLED_ShowWte(x,y,24);OLED_ShowWte(x+16,y,22);OLED_ShowWte(x+32,y,23);OLED_ShowWte(x+48,y,20);OLED_ShowWte(x+64,y,50);break;//特大暴雨
      case 19:OLED_ShowWte(x,y,25);OLED_ShowWte(x+16,y,20);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//冻雨
      case 20:OLED_ShowWte(x,y,20);OLED_ShowWte(x+16,y,26);OLED_ShowWte(x+32,y,27);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//雨夹雪
      case 21:OLED_ShowWte(x,y,12);OLED_ShowWte(x+16,y,27);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//阵雪
      case 22:OLED_ShowWte(x,y,19);OLED_ShowWte(x+16,y,27);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//小雪
      case 23:OLED_ShowWte(x,y,21);OLED_ShowWte(x+16,y,27);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//中雪
      case 24:OLED_ShowWte(x,y,22);OLED_ShowWte(x+16,y,27);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//大雪
      case 25:OLED_ShowWte(x,y,23);OLED_ShowWte(x+16,y,27);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//暴雪
      case 26:OLED_ShowWte(x,y,28);OLED_ShowWte(x+16,y,29);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//浮尘
      case 27:OLED_ShowWte(x,y,30);OLED_ShowWte(x+16,y,29);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//扬尘
      case 28:OLED_ShowWte(x,y,32);OLED_ShowWte(x+16,y,33);OLED_ShowWte(x+32,y,34);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//沙尘暴
      case 29:OLED_ShowWte(x,y,31);OLED_ShowWte(x+16,y,32);OLED_ShowWte(x+32,y,33);OLED_ShowWte(x+48,y,34);OLED_ShowWte(x+64,y,50);break;//强沙尘暴
      case 30:OLED_ShowWte(x,y,35);OLED_ShowWte(x+16,y,50);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//雾
      case 31:OLED_ShowWte(x,y,36);OLED_ShowWte(x+16,y,50);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//霾
      case 32:OLED_ShowWte(x,y,38);OLED_ShowWte(x+16,y,50);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//风
      case 33:OLED_ShowWte(x,y,22);OLED_ShowWte(x+16,y,38);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//大风
      case 34:OLED_ShowWte(x,y,37);OLED_ShowWte(x+16,y,38);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//飓风
      case 35:OLED_ShowWte(x,y,39);OLED_ShowWte(x+16,y,40);OLED_ShowWte(x+32,y,41);OLED_ShowWte(x+48,y,42);OLED_ShowWte(x+64,y,50);break;//热带风暴
      case 36:OLED_ShowWte(x,y,43);OLED_ShowWte(x+16,y,44);OLED_ShowWte(x+32,y,45);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//龙卷风
      case 37:OLED_ShowWte(x,y,46);OLED_ShowWte(x+16,y,50);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//冷
      case 38:OLED_ShowWte(x,y,47);OLED_ShowWte(x+16,y,50);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//热
      case 99:OLED_ShowWte(x,y,48);OLED_ShowWte(x+16,y,49);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//未知
      default:OLED_ShowWte(x,y,48);OLED_ShowWte(x+16,y,49);OLED_ShowWte(x+32,y,50);OLED_ShowWte(x+48,y,50);OLED_ShowWte(x+64,y,50);break;//未知
    }
}

void Winddirection(int x,int y,int wind)//风向显示(下一位要从48开始)
{
  if(wind==0||wind==360)//北
  {
    OLED_Showwind(x,y,3);OLED_Showwind(x+16,y,4);OLED_Showwind(x+32,y,5);
  }
  else if(wind>0&&wind<90)//东北
  {
    OLED_Showwind(x,y,0);OLED_Showwind(x+16,y,3);OLED_Showwind(x+32,y,4); 
  }
  else if(wind==90)//东
  {
    OLED_Showwind(x,y,0);OLED_Showwind(x+16,y,4);OLED_Showwind(x+32,y,5);  
  }
  else if(wind>90&&wind<180)//东南
  {
    OLED_Showwind(x,y,0);OLED_Showwind(x+16,y,1);OLED_Showwind(x+32,y,4);    
  }
  else if(wind==180)//南
  {  
    OLED_Showwind(x,y,1);OLED_Showwind(x+16,y,4);OLED_Showwind(x+32,y,5);    
  }
  else if(wind>180&&wind<270)//西南
  {
    OLED_Showwind(x,y,2);OLED_Showwind(x+16,y,1);OLED_Showwind(x+32,y,4);    
  }
  else if(wind==270)//西
  {  
    OLED_Showwind(x,y,2);OLED_Showwind(x+16,y,4);OLED_Showwind(x+32,y,5);    
  }
  else if(wind>270&&wind<360)//西北
  {
    OLED_Showwind(x,y,2);OLED_Showwind(x+16,y,3);OLED_Showwind(x+32,y,4);    
  }
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
  const int EXECUTION_COUNT = 512;
  int counter = 0;
  int numc,numt,numtqsd,numfjddj,numwd;//串口接收的信息分解
  int nump;//风实际速度带份秒
  char *lastttqsdfjddjwdptm,*lasttqsdfjddjwdptm,*lastfjddjwdptm,*lastwdptm,*lastptm,*tm;
  int task=0,menu1=0,menu2=0,ops=6,cls=18;//控制变量
  int middle1=0,middle2=0;//执行一次变量
  int pollution;//污染
  int turn;//切屏计数
  int clear;//清屏执行一次计数
  int rain=0;//下雨或大风指示
  int clsd;//手动
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define BUFFERSIZE 70//串口1可以接收的最大字符个数
uint8_t ReceiveBuff[BUFFERSIZE]; //串口1接收缓冲区
uint8_t Rx_len;//串口1接收完成中断标志，接收到字符长度
extern DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();
  HAL_GPIO_WritePin(GPIOC ,GPIO_PIN_13 ,GPIO_PIN_RESET ); 
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//使能串口1 IDLE中断   
	HAL_UART_Receive_DMA(&huart1,ReceiveBuff,BUFFERSIZE);//使能接收
  HAL_ADC_Start(&hadc1);//启动ADC装换
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    //串口接收的数据：021310 16 0933 21103 23 1000 25/04/26-16:09
    //                  控制  时间 天气湿度   风角度等级  温度  压力  时间
    HAL_GPIO_TogglePin(GPIOC ,GPIO_PIN_13);
    numc = strtoul((const char*)ReceiveBuff,    &lastttqsdfjddjwdptm, 10);//控制
    numt = strtoul(lastttqsdfjddjwdptm,    &lasttqsdfjddjwdptm, 10);//时间 
    numtqsd = strtoul(lasttqsdfjddjwdptm, &lastfjddjwdptm, 10);//天气湿度    
    numfjddj = strtoul(lastfjddjwdptm,&lastwdptm, 10); //风角度等级
    numwd = strtoul(lastwdptm,   &lastptm, 10); //温度
    nump = strtoul(lastptm,    &tm, 10); //风实际速度
    ///////////////////////////////////////////////////////////////////////////////////////////
    if(numtqsd/100>9&&numfjddj%100>3)
    {
       rain=1;
      if(middle1==1||middle1==0)
      {
        menu1=2;//执行窗关闭
        middle1 =33;//只让这个执行一次 
      }
    }
    else
    {
      rain=0;
      if(middle1==33)
      {
      middle1 =0;
      }
    }
    if(numc%10==1&&rain==0)//窗控制开启
    { 
      if(middle1==0)
      {
        menu1=1;//执行窗开启
        middle1 =1;//只让这个执行一次 
      }
    }
     if(numc%10==0&&rain==0)//窗控制关闭
    { 
      if(middle1==1)
      {
        menu1=2;//执行窗关闭
        middle1 =0;//只让这个执行一次 
      }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    if(numc/10%10==1)//窗帘控制开启
    { 
      clsd=1;
      if(middle2==0)
      {
        menu2=1;//执行窗帘开启
        middle2 =1;//只让这个执行一次 
      }
    }
     if(numc/10%10==0)//窗帘控制关闭
    { 
      clsd=0;
      if(middle2==1)
      {
        menu2=2;//执行窗帘关闭
        middle2 =0;//只让这个执行一次 
      }
    }
//////////////////////////////////////////////////////////////////////////////////////////    
    if(menu1+menu2==0)
    {
    HAL_ADC_PollForConversion(&hadc1, 50);//等待转换完成，第二个参数表示超时时间，单位ms.
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)){
    pollution = cal_map(HAL_ADC_GetValue(&hadc1),0,4095,0,300);//读取ADC转换数据，数据为12位
    }
      DH11_Task();
      if(turn>=0&&turn<10)
      {
        if(clear==0)
        {
          OLED_Clear();
          clear=1;
        }
        if(numc/1000000==3)//显示已连接
        {
          OLED_ShowCHinese(0,0,27);
          OLED_ShowCHinese(16,0,29);
          OLED_ShowCHinese(32,0,30);      
        }
        else
        {
          OLED_ShowCHinese(0,0,28);
          OLED_ShowCHinese(16,0,29);
          OLED_ShowCHinese(32,0,30);      
        }
        OLED_ShowCHinese(0,2,0);
        OLED_ShowCHinese(16,2,2);
        OLED_ShowCHinese(32,2,3);
        OLED_ShowCHinese(48,2,4);
        OLED_ShowCHinese(64,2,5);
        if(numc%10==1)
        {
          OLED_ShowCHinese(80,2,6);
          OLED_ShowCHinese(96,2,7);
        }
        if(numc%10==0)
        {
          OLED_ShowCHinese(80,2,8);
          OLED_ShowCHinese(96,2,9);
        }
        OLED_ShowCHinese(0,4,0);
        OLED_ShowCHinese(16,4,1);
        OLED_ShowCHinese(32,4,3);
        OLED_ShowCHinese(48,4,4);
        OLED_ShowCHinese(64,4,5);
        if(numc/10%10==1)
        {
          OLED_ShowCHinese(80,4,10);
          OLED_ShowCHinese(96,4,11);
        }
        if(numc/10%10==0)
        {
          OLED_ShowCHinese(80,4,16);
          OLED_ShowCHinese(96,4,17);
        }
        OLED_ShowCHinese(0,6,31);
        OLED_ShowCHinese(16,6,32);
        OLED_ShowCHinese(32,6,33);
        OLED_ShowCHinese(48,6,34);
        OLED_ShowCHinese(64,6,5);
        OLED_ShowNum(80,6,pollution,3,16);
      } 
      if(turn>=10&&turn<20)
      {
        if(clear==1)
        {
          OLED_Clear();
          clear=0;
        }
        OLED_ShowCHinese(0,0,18);
        OLED_ShowCHinese(16,0,19);
        OLED_ShowCHinese(32,0,5);        
        WhetherReport(48,0,numtqsd/100);//天气
        OLED_ShowCHinese(0,2,19);
        OLED_ShowCHinese(16,2,20);
        OLED_ShownNum(32,2,numwd,2,16);//温度
        OLED_ShowCHinese(64,2,21);
        OLED_ShowCHinese(80,2,22);
        OLED_ShowNum(96,2,numtqsd%100,2,16);//湿度   
        OLED_ShowNum(0,4,nump,3,16);//压力
        Winddirection(0,6,numfjddj/100);
        OLED_ShowNum(48,6,numfjddj%100,2,16);//风速 
        OLED_ShowCHinese(64,6,36);
      }
      if(turn>=20&&turn<30)
      {
        if(clear==0)
        {
          OLED_Clear();
          clear=1;
        }
        OLED_ShowCHinese(0,0,37);
        OLED_ShowCHinese(16,0,38);
        OLED_ShowCHinese(32,0,20);
        OLED_ShowCHinese(48,0,22);
        OLED_ShowNum(64,0,DH11_data.temp,2,16);
        OLED_ShowCHinese(0,2,37);
        OLED_ShowCHinese(16,2,38);
        OLED_ShowCHinese(32,2,21);
        OLED_ShowCHinese(48,2,22);
        OLED_ShowNum(64,2,DH11_data.humidity,2,16);
        OLED_ShowString(0,4,tm,16);        
      }
      if(turn==30)
      {
        clear=0;
        turn=0;
      }
        turn++;
    }
    ///////////////////////////////////////////////////////////////////////////////////
    if(menu1==1)//开窗
    {
      if(counter < 512) {
      targetFunctionopen(counter + 1);
      counter++;
       // 当函数执行完512次后
      if(counter == 512) {
         StepMotor_Stop();
         counter = 0;
         menu1=0;
         task+=1;//用于发送的控制指令
            }
      }
    }
    if(menu1==2)//关窗
    {
      if(counter < 512) {
      targetFunctionclose(counter + 1);
      counter++;
       // 当函数执行完512次后
      if(counter == 512) {
         StepMotor_Stop();
         counter = 0;
         menu1=0;
        task-=1;//用于发送的控制指令
            }
      }
    }
///////////////////////////////////////////////////////////////////////
    if(menu2==1)//开窗帘
    {
      if(counter < 512) {
      targetFunctionopen2(counter + 1);
      counter++;
       // 当函数执行完512次后
      if(counter == 512) {
         StepMotor_Stop2();
         counter = 0;
         menu2=0;
        task+=10;//用于发送的控制指令
            }
      }
    }
    if(menu2==2)//关窗帘
    {
      if(counter < 512) {
      targetFunctionclose2(counter + 1);
      counter++;
       // 当函数执行完512次后
      if(counter == 512) {
         StepMotor_Stop2();
         counter = 0;
         menu2=0;
        task-=10;//用于发送的控制指令
            }
      }
    }    
    
    
    printf("%02d\r\n",task);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
