#ifndef _CRTTCONSOLE_H_
#define _CRTTCONSOLE_H_
#include "Console.h"
#include "SEGGER_RTT.h"
class CRttConsole
	:public CConsole::COstreamDev
{
public:
	CRttConsole(){}
	virtual ~CRttConsole(){close();}

	virtual uint16_t write(uint8_t* srcBuf, uint16_t srcLen)
	{
		return SEGGER_RTT_Write(0, srcBuf, srcLen);
	}
	
	virtual bool open()
	{
		SEGGER_RTT_Init();
		return true;
	}
	
	virtual uint16_t getFreeSize(){return 64;}
	
	virtual bool isIdel(){return true;}

private:

};
#endif
//end  of file
