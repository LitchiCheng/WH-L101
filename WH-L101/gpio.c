#include "gpio.h"
#include "Timer.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

void Lora_Reset(void)
{
	GPIO_ResetBits(GPIOB	,LORA_NRESET);
	BaseTimer::Instance()->delay_ms(20);
	GPIO_SetBits(GPIOB,LORA_NRESET);
}

void Lora_Reload(void)
{
	GPIO_ResetBits(GPIOB,LORA_RELOAD);
	BaseTimer::Instance()->delay_ms(4000);
	GPIO_SetBits(GPIOB,LORA_NRESET);
}

void Lora_Wake(void)
{
	GPIO_SetBits(GPIOB,LORA_NRESET);
	BaseTimer::Instance()->delay_ms(20);
	GPIO_ResetBits(GPIOB,LORA_WAKE);
}

void Lora_LED_Control(u8 state)
{
	if(state==1)
		GPIO_SetBits(GPIOA,LORA_LED);
	else
		GPIO_ResetBits(GPIOA,LORA_LED);
}

void Lora_Init(void)
{  
	GPIO_SetBits(GPIOA,LORA_LED);
	GPIO_SetBits(GPIOB,LORA_RELOAD);
}

void Lora_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = LORA_LED ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LORA_NRESET ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LORA_RELOAD ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = LORA_WAKE ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LORA_HOST_WAKE ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	Lora_Reset();
} 
 
void Beep_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = BEEP ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	BaseTimer::Instance()->delay_ms(5);
	Beep_Control(0);
}
 
void Beep_Control(u8 state)
{
	if(state)
		GPIO_SetBits(GPIOA,BEEP); 
	else
		GPIO_ResetBits(GPIOA,BEEP); 
 
}
 
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin =RED|YELLOW|GREEN;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
 
void LED_ON(u16 state)
{
	if(state==RED)
	{ 
		GPIO_SetBits(GPIOA,RED);
		GPIO_ResetBits(GPIOA,YELLOW);
		GPIO_ResetBits(GPIOA,GREEN);
	}
	else if(state==YELLOW)
	{ 
		GPIO_SetBits(GPIOA,YELLOW);
		GPIO_ResetBits(GPIOA,RED);
		GPIO_ResetBits(GPIOA,GREEN);
	}
	else if(state==GREEN)
	{ 
		GPIO_SetBits(GPIOA,GREEN);
		GPIO_ResetBits(GPIOA,RED);
		GPIO_ResetBits(GPIOA,YELLOW);
	}
}
 
void LED_OFF(void)
{
	GPIO_ResetBits(GPIOA,RED);
	GPIO_ResetBits(GPIOA,YELLOW);
	GPIO_ResetBits(GPIOA,GREEN);
}
 