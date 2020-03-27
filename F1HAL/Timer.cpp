#include "Timer.h"

CBaseTimer::CBaseTimer()// 要写完全!!!
{	
	_baseTimer = 0;	
	SystemCoreClockUpdate();
}

void CBaseTimer::initialize()
{	
	SysTick_Config(SystemCoreClock/8/1000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //21MHz. Different from F1.
	start();
}

void CBaseTimer::doUpdate()
{
	++_baseTimer;
	_baseTimer &= 0x7FFFFFFF;
}

long local_temp;
void CBaseTimer::delay_ms(uint16_t value)
{
	int32_t inTime = _baseTimer;
	while(_baseTimer - inTime < value);
}

Timer::Timer(int32_t delay,int32_t period)
{
	_timer = BaseTimer::Instance()->getTime();
	_counter = delay;
	_period = period;
}

void Timer::reset()
{
	_timer = BaseTimer::Instance()->getTime();
	_counter = _period;
}

bool Timer::isTimeUp()
{
	return isAbsoluteTimeUp();
}

bool Timer::isAbsoluteTimeUp()
{
	int32_t tempBaseTimer = BaseTimer::Instance()->getTime();
	int32_t diff = tempBaseTimer - _timer;
	if ( diff < 0 )
	{
		diff &= 0x7FFFFFFF;
	}
	if (diff >= _period)
	{
		_timer = tempBaseTimer;
		return true;
	}
	return false;
}

void CPUTIMER0_ISR()
{
	BaseTimer::Instance()->doUpdate();
	SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
}
