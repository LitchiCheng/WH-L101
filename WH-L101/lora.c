#include "lora.h"
#include "CUsart.h"
#include "Timer.h"
#include "Console.h"

char LORA[] = "lora";
char start[]="+++";                                 //����ATָ��ģʽ��ǰ���� 
char UART_Check[]="AT+UART\r\n";  //ATָ���ѯ��������
char SPD_Check[]="AT+SPD\r\n";       //ATָ���ѯ�������ʵȼ�
char CH_Check[]="AT+CH\r\n";            //ATָ���ѯ�ŵ�
char NID_Check[]="AT+NID\r\n";       //ATָ���ѯ�ڵ�ID
char VER_Check[]="AT+VER\r\n";       //ATָ���ѯ�̼��汾
char AID_Check[]="AT+AID\r\n";       //ATָ���ѯӦ��ID
char BACK_Setting[]= "AT+E=OFF\r\n";//ATָ��˳�ָ��ظ���ʾģʽ
char ENDSETTING[]="AT+ENTM\r\n";  //ATָ��Ƴ�ATָ������ģʽ
int RXcount;
//u8  Pre_Mode_Flag=SETTING;   //��ʼģʽΪSETTING
u8  choice_flag=0;
u8  request_flag=0;
u8  RXover=0;

const uint8_t RX_BUFF_LENGTH = 100;
extern uint8_t rx_buff[RX_BUFF_LENGTH];
extern CUsart uart1;

void SetUp_Mode_IN(void)         //����ATָ��ģʽ������
{
	Comm_Wakeup();       		 //����lora 
	AT_Instruction_Set_Start();  //����ATָ��ģʽ
	Mode_ReplyBack_Exit();       //�˳�����ģʽ
	Comm_Parameter_Inquiry();    //���ڲ�����ѯ
	Speed_Level_Inquiry();       //���ʵȼ���ѯ
	Channel_Inquiry();           //ͨ����ѯ
	Version_Inquiry();           //ģ��̼��汾��ѯ
	Node_ID_Inquiry();           //NODE�ڵ�ID��ѯ
	Application_ID_Inquiry();    //Ӧ��ID��ѯ
	ATMode_Exit();               //ATָ��ģʽ�˳�
}

void Comm_Wakeup(void)  //����lora����Ϊlora�ڵ��ϵ�ʱ����һ�η�����Ϣ�����ڣ��������κ���Ч�����ϴ�����������ֻ��ظ�lora start
{
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)LORA, 4);//�������ݿ���������
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		memset(temp, 0, 100);
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}
}

void AT_Instruction_Set_Start(void)  //����ATָ��ģʽ
{
	uint8_t plus3[] = "+++";
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array(plus3, 3);
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}
	uint8_t a[] = "a";
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array(a, 1);
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}
}

void Mode_ReplyBack_Exit(void)  //�˳�����ģʽ
{
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)BACK_Setting, strlen(BACK_Setting));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}
	
}

void Comm_Parameter_Inquiry(void)  //���ڲ�����ѯ
{
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)UART_Check, strlen(UART_Check));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}  
}

void Speed_Level_Inquiry(void)  //���ʵȼ���ѯ
{
	//�������ʵȼ���ѯ����
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)SPD_Check, strlen(SPD_Check));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	} 
}

void Channel_Inquiry(void)       //�ŵ���ѯ
{  
	//�����ŵ���ѯָ��
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)CH_Check, strlen(CH_Check));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	} 
}

void Version_Inquiry(void)       //lora�̼��汾��ѯ
{
	//���͹̼��汾��ѯָ��
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)VER_Check, strlen(VER_Check));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}  
}

void Node_ID_Inquiry(void)       //Lora�ڲ��ڵ�ID��ѯ
{
	//����Lora�ڲ��ڵ�ID��ѯָ��
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)NID_Check, strlen(NID_Check));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}   
}

void Application_ID_Inquiry(void)//Ӧ��ID��ѯ�����Ӽ�������Ҫ
{
	//����Ӧ��ID��ѯָ��
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)AID_Check, strlen(AID_Check));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	} 
}

void ATMode_Exit(void)           //ATָ��ģʽ�˳�
{
	//�˳�ATָ��ģʽ 
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)ENDSETTING, strlen(ENDSETTING));
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}  
}