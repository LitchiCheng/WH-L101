#ifndef __include_h
#define __include_h

#include "stm32f10x.h"

#define LORA_LED        GPIO_Pin_5
#define LORA_NRESET     GPIO_Pin_8
#define LORA_RELOAD     GPIO_Pin_5
#define LORA_WAKE       GPIO_Pin_4
#define LORA_HOST_WAKE  GPIO_Pin_3
#define BEEP            GPIO_Pin_3
#define RED             GPIO_Pin_0
#define YELLOW          GPIO_Pin_1
#define GREEN           GPIO_Pin_2
#define SETTING         1
#define STANDBY         2
#define RUNNING         3
#define NET_IN          0x01
#define DATA_CIRCLE     0x02
void Lora_Reset(void);
void USART_SEND(u8 *str);
void USART_SendOneData(u8 str);
void USART_Config(void);
void Lora_Config(void);
void Lora_Reload(void);
void Lora_Wake(void);
void Lora_Init(void);
void Key_Init(void);
void Lora_LED_Control(u8 state);
void Beep_Config(void);
void Beep_Control(u8 state);
void LED_Config(void);
void LED_ON(u16 state);
void LED_OFF(void);
#endif
