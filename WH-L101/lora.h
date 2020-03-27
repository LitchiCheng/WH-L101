#ifndef _LORA_H_
#define _LORA_H_

#include "stm32f10x.h"
#include <stdio.h>

void key_scan(void);
void Clear_RXbuf(void);//清空RXbuf数据缓冲区的数据，适用于50个数据的清空
void Clear_BUF2(void); //清空数据缓冲区，适用于10个数据的清空
void Comm_Wakeup(void);//唤醒lora
void AT_Instruction_Set_Start(void);//进入AT指令的前导时序
void Mode_ReplyBack_Exit(void);//退出回显模式
void Comm_Parameter_Inquiry(void);//串口参数查询
void Speed_Level_Inquiry(void);//速率等级查询
void Channel_Inquiry(void); //信道查询
void Version_Inquiry(void); //lora固件版本查询
void Node_ID_Inquiry(void);//Lora内部节点ID查询
void Application_ID_Inquiry(void);//应用ID查询，连接集中器需要
void ATMode_Exit(void);     //AT指令模式退出
void SetUp_Mode_IN(void);   //进入AT指令模式的开始

#endif //_LORA_H_
