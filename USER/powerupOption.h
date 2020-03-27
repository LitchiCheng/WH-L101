#ifndef POWERUP_OPTION_H
#define POWERUP_OPTION_H

#include <string.h>
#include <stdint.h>
namespace pvf //powerup variables field 
{
	enum powerupVarEnum
	{
		VAR_BOOT_OPTI = 0,
		VAR_NUM_OF_VAR,
		VAR_SECU_KEY,
		VarNum
	};
	
	bool verifyPowerupField();
	
	uint32_t read(powerupVarEnum idx);
	
	void write(powerupVarEnum idx, uint32_t val);
	
	void initInApp();
	
	void printField();
	
	uint32_t getSuitVarNum();
}



#endif
//end of file
