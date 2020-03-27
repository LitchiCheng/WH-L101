#ifndef _FW_TIMER_H_
#define _FW_TIMER_H_
#include "stm32f10x.h"
#include <stdint.h>
#include "Singleton.h"
//base timer
class CBaseTimer
{
public:
	CBaseTimer();// need EALLOW
	~CBaseTimer(){}

	void initialize();
	void doUpdate();
	int32_t getTime(){return _baseTimer;}
	void delay_ms(uint16_t value);
	
	inline void start()
	{
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
	inline void stop()
	{
		SysTick->CTRL &= ~(uint16_t)SysTick_CTRL_ENABLE_Msk;
	}

private:
	volatile int32_t _baseTimer;
};

typedef NormalSingleton<CBaseTimer>	BaseTimer;

class Timer
{
public:
	Timer():_timer(1),_period(1),_counter(1){}
	Timer(int32_t delay,int32_t period);
	bool isTimeUp();	//相对时钟。只有持续不断的访问才有意义！！其中的判断语句为_timer != tempBaseTimer
	bool isAbsoluteTimeUp();//绝对时钟
	void reset();
private:
	int32_t _timer;
	int32_t _period;
	int32_t _counter;
};

void CPUTIMER0_ISR();

#endif	//_FW_TIMER_H_
