#ifndef _LORA_H_
#define _LORA_H_

#include "stm32f10x.h"
#include <stdio.h>

void key_scan(void);
void Clear_RXbuf(void);//���RXbuf���ݻ����������ݣ�������50�����ݵ����
void Clear_BUF2(void); //������ݻ�������������10�����ݵ����
void Comm_Wakeup(void);//����lora
void AT_Instruction_Set_Start(void);//����ATָ���ǰ��ʱ��
void Mode_ReplyBack_Exit(void);//�˳�����ģʽ
void Comm_Parameter_Inquiry(void);//���ڲ�����ѯ
void Speed_Level_Inquiry(void);//���ʵȼ���ѯ
void Channel_Inquiry(void); //�ŵ���ѯ
void Version_Inquiry(void); //lora�̼��汾��ѯ
void Node_ID_Inquiry(void);//Lora�ڲ��ڵ�ID��ѯ
void Application_ID_Inquiry(void);//Ӧ��ID��ѯ�����Ӽ�������Ҫ
void ATMode_Exit(void);     //ATָ��ģʽ�˳�
void SetUp_Mode_IN(void);   //����ATָ��ģʽ�Ŀ�ʼ

#endif //_LORA_H_
