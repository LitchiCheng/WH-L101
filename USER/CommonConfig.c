#include "stm32f10x.h"
#include "CommonConfig.h"

void CommonConfig(void)
{
		__set_PRIMASK(0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

void InitWatchDog(uint16_t time_ms)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//Input clock 32KHz
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(time_ms/8);
  IWDG_Enable();
	IWDG_ReloadCounter();
}

