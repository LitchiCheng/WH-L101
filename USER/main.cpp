#include "stm32f10x.h"
#include "Console.h"
#include "CommonConfig.h"
#include "Timer.h"
//#include "LED.h"
#include "CUsart.h"
#include "gpio.h"
#include "lora.h"
uint8_t test_buf[10] = {0,0,0,0,0,0,0,0,0,0};
const uint8_t RX_BUFF_LENGTH = 100;
uint8_t rx_buff[RX_BUFF_LENGTH];
CUsart uart1(USART1, rx_buff, RX_BUFF_LENGTH);

int main()
{
	CommonConfig();
	BaseTimer::Instance()->initialize();
	BaseTimer::Instance()->delay_ms(50);
	
	uart1.InitSciGpio();
	uart1.InitSci();
	uart1.setBaudrate(115200);
	Lora_Config();
	Lora_Init();
	Key_Init();                            //按键初始化
	Beep_Config();                         //蜂鸣器初始化
	LED_Config();
	SetUp_Mode_IN();                       //进入AT指令模式，进行相关参数的查询or设置
	LED_ON(RED);  
	Timer freq(1000,1000);
	
	while(1)
	{
		Console::Instance()->runTransmitter();
		if(freq.isAbsoluteTimeUp()){
			uint8_t a[] = {0x02};
			while(0 != uart1.get_BytesInTxFifo()){}
			uart1.send_Array(a, 1);
			
		}
		if(uart1.get_BytesInRxFifo() > 0){
			u8 temp[100];
			u8 rx_size = uart1.read_RxFifo(temp);
			for(int i=0;i<rx_size;i++){
				Console::Instance()->printf("0x%02x",temp[i]);
			}
			Console::Instance()->printf("\r\n");
		}
	}
}

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	Timer loopTimer(1000,1000);
	
  /* Infinite loop */
  while (1)
  {
		Console::Instance()->runTransmitter();
		if(loopTimer.isAbsoluteTimeUp())
		{
			Console::Instance()->printf("Wrong parameters value: file %s on line %d\r\n", file, line);
		}
  }
}
