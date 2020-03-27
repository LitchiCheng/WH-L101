#ifndef __COMMONCONFIG_H
#define __COMMONCONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"

void CommonConfig(void);
void InitWatchDog(uint16_t);
	 
inline void ServiceDog()
{IWDG_ReloadCounter();}
	 
#ifdef __cplusplus
 }
#endif 
#endif
