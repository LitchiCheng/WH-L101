#include "lora.h"
#include "CUsart.h"
#include "Timer.h"
#include "Console.h"

char LORA[] = "lora";
char start[]="+++";                                 //进入AT指令模式的前导码 
char UART_Check[]="AT+UART\r\n";  //AT指令，查询串口数据
char SPD_Check[]="AT+SPD\r\n";       //AT指令，查询空中速率等级
char CH_Check[]="AT+CH\r\n";            //AT指令，查询信道
char NID_Check[]="AT+NID\r\n";       //AT指令，查询节点ID
char VER_Check[]="AT+VER\r\n";       //AT指令，查询固件版本
char AID_Check[]="AT+AID\r\n";       //AT指令，查询应用ID
char BACK_Setting[]= "AT+E=OFF\r\n";//AT指令，退出指令回复显示模式
char ENDSETTING[]="AT+ENTM\r\n";  //AT指令，推出AT指令设置模式
int RXcount;
//u8  Pre_Mode_Flag=SETTING;   //初始模式为SETTING
u8  choice_flag=0;
u8  request_flag=0;
u8  RXover=0;

const uint8_t RX_BUFF_LENGTH = 100;
extern uint8_t rx_buff[RX_BUFF_LENGTH];
extern CUsart uart1;

void SetUp_Mode_IN(void)         //进入AT指令模式的设置
{
	Comm_Wakeup();       		 //唤醒lora 
	AT_Instruction_Set_Start();  //进入AT指令模式
	Mode_ReplyBack_Exit();       //退出回显模式
	Comm_Parameter_Inquiry();    //串口参数查询
	Speed_Level_Inquiry();       //速率等级查询
	Channel_Inquiry();           //通道查询
	Version_Inquiry();           //模块固件版本查询
	Node_ID_Inquiry();           //NODE节点ID查询
	Application_ID_Inquiry();    //应用ID查询
	ATMode_Exit();               //AT指令模式退出
}

void Comm_Wakeup(void)  //唤醒lora，因为lora节点上电时，第一次发送消息给串口，不会有任何有效数据上传到集中器，只会回复lora start
{
	while(0 != uart1.get_BytesInTxFifo()){}
	uart1.send_Array((uint8_t*)LORA, 4);//唤醒数据可任意设置
	BaseTimer::Instance()->delay_ms(100);
	if(uart1.get_BytesInRxFifo() > 0){
		u8 temp[100];
		memset(temp, 0, 100);
		uart1.read_RxFifo(temp);
		Console::Instance()->printf("%s\r\n",temp);
		memset(temp, 0, 100);
	}
}

void AT_Instruction_Set_Start(void)  //进入AT指令模式
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

void Mode_ReplyBack_Exit(void)  //退出回显模式
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

void Comm_Parameter_Inquiry(void)  //串口参数查询
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

void Speed_Level_Inquiry(void)  //速率等级查询
{
	//发送速率等级查询命令
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

void Channel_Inquiry(void)       //信道查询
{  
	//发送信道查询指令
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

void Version_Inquiry(void)       //lora固件版本查询
{
	//发送固件版本查询指令
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

void Node_ID_Inquiry(void)       //Lora内部节点ID查询
{
	//发送Lora内部节点ID查询指令
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

void Application_ID_Inquiry(void)//应用ID查询，连接集中器需要
{
	//发送应用ID查询指令
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

void ATMode_Exit(void)           //AT指令模式退出
{
	//退出AT指令模式 
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