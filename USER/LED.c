/* KEY.C file
STM32-SDK 开发板相关例程
编写者：lisn3188
网址：www.chiplab7.com
作者E-mail：lisn3188@163.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2012-02-28
测试： 本程序已在第七实验室的STM32-SDK上完成测试
功能：实现	STM32-SDK 开发板上的两个 LED 操作接口

---------硬件上的引脚连接:----------
LED -->  PA8  	(输出低电平,灯亮;输出高电平灯灭)
------------------------------------
 */

#include "LED.h"
#include "Timer.h"
#include "printf.h"

static uint8_t isLedInitialized = 0;
static Timer ledBlink(50, 50);

/**************************实现函数********************************************
*函数原型:		void Initial_LED_GPIO(void)
*功　　能:		配置 LED 对应的端口为输出
*******************************************************************************/
void Initial_LED_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //使能GPIOA 的时钟,
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
  //配置PA8 为推挽输出  刷新频率为2Mhz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //应用配置到GPIOA 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  	/*
	配置 PA0  PA1 为 输入且使能上拉电阻
	*/
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//应用配置　到GPIOA　
	GPIO_Init(GPIOA, &GPIO_InitStructure);


  //设置LED 端口输出高电平, 关灯.
  GPIO_SetBits(GPIOA, GPIO_Pin_8);
  GPIO_SetBits(GPIOA, GPIO_Pin_1);	 
}

/**************************实现函数********************************************
*函数原型:		void Initial_PWMLED(void)
*功　　能:		配置 PWM  使得PB1输出PWM信号.
*******************************************************************************/
void Initial_PWMLED(void)
{
             
}

u8 lightc=0;
/**************************实现函数********************************************
*函数原型:		void LED_Change(void)
*功　　能:		改变LED的亮度，从	LightLevel 数据
*******************************************************************************/
void LED_Change(void)
{

}

/**************************实现函数********************************************
*函数原型:		void LED_Reverse(void)
*功　　能:		LED 灯取反, 即,当亮时设置端口使之转成灭状态,
								当灭时设置端口使之转成亮状态.
*******************************************************************************/
void LED_Reverse(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))	
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		else
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

void LED_Run()
{
	if(!isLedInitialized)
	{
		Initial_LED_GPIO();
		isLedInitialized = true;
	}
	if(ledBlink.isAbsoluteTimeUp())
	{
		LED_Reverse();
	}
}
//------------------End of File----------------------------
