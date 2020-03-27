#include "CCan.h"
#include "Console.h"

namespace
{
	const uint8_t TX_QUEBUF_SIZE = 64;
	CanTxMsg router1_txQueBuf[TX_QUEBUF_SIZE];
//	CanTxMsg router2_txQueBuf[TX_QUEBUF_SIZE];
};
CCanRouter CanRouter1(CAN1, router1_txQueBuf, TX_QUEBUF_SIZE, 250000);
//CCanRouter CanRouter2(CAN2, router2_txQueBuf, TX_QUEBUF_SIZE, 250000);

/**
  * @brief  Constructor
	* @param  None
  * @retval None
  */
CCanRxMailbox::CCanRxMailbox(CanRxMsg* rxQueBuf, uint16_t rxQueSize)
	:rxQue_(rxQueBuf, rxQueSize),
	rxOverflowCount_(0),
	isAttached(false)
{
	
}

/**
  * @brief  set mailbox id as std
	* @param  standard id value
  * @retval None
  */
void CCanRxMailbox::setStdId(uint16_t stdId)
{
	stdId_ = stdId;
	IDE_ = CAN_Id_Standard;
}

/**
  * @brief  set mailbox id as ext
	* @param  extended id value
  * @retval None
  */
void CCanRxMailbox::setExtId(uint32_t extId)
{
	extId_ = extId;
	IDE_ = CAN_Id_Extended;
}

/**
  * @brief  judge if mailbox id is equal to message
	* @param  const reference of message
  * @retval ture of false
  */
bool CCanRxMailbox::operator == (const CanRxMsg& rhs_msg)
{
	if(rhs_msg.IDE != IDE_) return false;
	if(((IDE_ == CAN_Id_Standard) && (stdId_ == rhs_msg.StdId))
		||((IDE_ == CAN_Id_Extended) && (extId_ == rhs_msg.ExtId)))
		return true;
	else
		return false;
}

/**
  * @brief  judge if mailbox id is equal to message
	* @param  const reference of message
  * @retval ture of false
  */
bool CCanRxMailbox::isIdEqual(const CanRxMsg& rhs_msg)
{
	if(rhs_msg.IDE != IDE_) return false;
	if(((IDE_ == CAN_Id_Standard) && (stdId_ == rhs_msg.StdId))
		||((IDE_ == CAN_Id_Extended) && (extId_ == rhs_msg.ExtId)))
		return true;
	else
		return false;
}

/**
  * @brief  judge if mailbox id is equal to message
	* @param  const reference of message
  * @retval ture of false
  */
bool CCanRxMailbox::isIdEqual(CCanRxMailbox* pMailbox)
{
	if(pMailbox->IDE_ != IDE_) return false;
	if(((IDE_ == CAN_Id_Standard) && (stdId_ == pMailbox->stdId_))
		||((IDE_ == CAN_Id_Extended) && (extId_ == pMailbox->extId_)))
		return true;
	else
		return false;
}
 
/**
  * @brief  return how many messages in rx queue
	* @param  None
  * @retval quantity of messages
  */
uint16_t CCanRxMailbox::msgsInQue() const
{
	return rxQue_.elemsInQue();
}

/**
  * @brief  get a message from rx queue
	* @Note 	Only copy for one time. We recommand to use this.
	* @param  Pointer for received message
  * @retval None
  */
void CCanRxMailbox::getMsg(CanRxMsg* pCanRxMsg)
{
	if(rxQue_.elemsInQue() == 0) return ;
	
	*pCanRxMsg = rxQue_.front();
	rxQue_.pop();
}


/**
  * @brief  push back a message into queue
	* @param  Pointer for received message
  * @retval None
  */
void CCanRxMailbox::pushMsg(const CanRxMsg& msg)
{
	if(!rxQue_.push(msg)) 
		rxOverflowCount_++;
}

/**
  * @brief  attach the mailbox to a CanRouter
	* @param  reference of target router
  * @retval if attach succeeded
  */
bool CCanRxMailbox::attachToRouter(CCanRouter& refRouter)
{
	isAttached = true;
	return refRouter.attachMailbox(this);
}

/*********************************************************
*********     this part is for CCanRouter       **********
*********************************************************/
/**
  * @brief  constructor
	* @param  
  * @retval None
  */
CCanRouter::CCanRouter(CAN_TypeDef* CANx, 
	CanTxMsg* txQueBuf, 
	uint16_t txQueSize,
	uint32_t BaudRate)
:CANx_(CANx),
	baudRate_(BaudRate),
	txOverflowCount_(0),
	isGpioInitialized_(false),
	isCanInitialized_(false),
	txQue_(txQueBuf, txQueSize),
	mailboxNum_(0)
{
	if(CANx_ == CAN1) 
		CAN_Filter_FIFO_  = CAN_Filter_FIFO0;
	else if(CANx_ == CAN2) 
		CAN_Filter_FIFO_  = CAN_Filter_FIFO1;
	else
	{
		Console::Instance()->postErr("CANx_ fault.");
		seer_assert(false);
	}
	
}

/**
  * @brief  Initialize CAN periperial
	* @param  
  * @retval None
  */
void CCanRouter::InitCan()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//CAN1 must be open when CAN2 used
	if(CANx_ == CAN2) 
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	}else if(CANx_ == CAN1)
	{
	}
	
	//CAN
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_TTCM = DISABLE;	
	CAN_InitStructure.CAN_ABOM = DISABLE;	
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;	
	
	if(baudRate_ == 250000)
	{
		CAN_InitStructure.CAN_Prescaler = 24;
	}
	else if(baudRate_ == 500000)
	{
		CAN_InitStructure.CAN_Prescaler = 12;
		Console::Instance()->postErr("500K baudrate not tested");
	}

	CAN_Init(CANx_, &CAN_InitStructure);
	
	//config the filter
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; //mask mode
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//32bits
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	
	if(CAN1 == CANx_)
	{
		CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //must < 14
	}
	else if(CAN2 == CANx_)
	{
		CAN_FilterInitStructure.CAN_FilterNumber = 15;	  //must >= 14
	}
	CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)0x0000<<21)&0xFFFF0000)>>16;								
	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)0x0000<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xFFFF;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO_;
	CAN_FilterInit(&CAN_FilterInitStructure);
	for(int i = 0; i < 3; i++)
		CAN_FIFORelease(CANx_, CAN_Filter_FIFO_);
	isCanInitialized_ = true;
}

/**
  * @brief  Initialize CAN IO port
	* @param  
  * @retval None
  */
void CCanRouter::InitCanGpio(int IOGroup)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t RCC_APB2Periph_GPIOx;
	uint8_t GPIO_PinSource_BASE;
	uint8_t GPIO_AF_CANx;
	GPIO_TypeDef *GPIOx;
	
	if(IOGroup == GROUP_B12)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
		GPIOx = GPIOB;
		GPIO_PinSource_BASE = GPIO_PinSource12;
	}	
	else if(IOGroup == GROUP_A11)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
		GPIOx = GPIOA;
		GPIO_PinSource_BASE = GPIO_PinSource11;
	}
	else if(IOGroup == GROUP_B8)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
		GPIOx = GPIOB;
		GPIO_PinSource_BASE = GPIO_PinSource8;
		GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	}
	else seer_assert(false); //undefined!
	
	/* open clock of MOSI MISO SCK nCS */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx,  ENABLE);	
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 << GPIO_PinSource_BASE; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOx, &GPIO_InitStructure);	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 << GPIO_PinSource_BASE; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ; 
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	isGpioInitialized_ = true;
}

/**
  * @brief  set baudrate of CAN
	* @param  baudrate value
  * @retval None
  */
void CCanRouter::setBaudrate(uint32_t value)
{
	baudRate_ = value;
	InitCan();
}

/**
  * @brief  run CAN transmitter for one time.
	*					Send until txMailbox full or txQue_ is empty
	* @param  None
  * @retval None
  */
void CCanRouter::runTransmitter()
{
	if(txQue_.elemsInQue() == 0) return;
	
	uint8_t temp_mbox = 0xFF;
	temp_mbox = CAN_Transmit(CANx_, &txQue_.front());
	while(CAN_TxStatus_NoMailBox != temp_mbox)
	{
		txQue_.pop();
		if(txQue_.elemsInQue() == 0) break;
		temp_mbox = CAN_Transmit(CANx_, &txQue_.front());
	}
}

/**
  * @brief  Run CAN receiver for one time. Until FIFO is empty
	* @param  None
  * @retval None
  */
void CCanRouter::runReceiver()
{
	if(CAN_MessagePending(CANx_, CAN_Filter_FIFO_) == 0) return;
	CanRxMsg RxMessage;
	while(CAN_MessagePending(CANx_, CAN_Filter_FIFO_) != 0)
	{
		CAN_Receive(CANx_, CAN_Filter_FIFO_, &RxMessage);
		for(int i = 0; i < mailboxNum_; i++)
		{
			if(mailboxTab[i]->isIdEqual(RxMessage))
			{
				mailboxTab[i]->pushMsg(RxMessage);
				return;
			}
		}
		//if program goes here, means the router can't find the mailbox for this id.
#if 0	//no mailbox message report
		Console::Instance()->printf("CAN%d cannot find the rx mailbox ", CANx_ == CAN1 ? 1 : 2);
		if(RxMessage.IDE == CAN_Id_Standard)
		{
			Console::Instance()->printf("STD 0x%X\r\n", RxMessage.StdId);
		}else
		{
			Console::Instance()->printf("EXT 0x%X\r\n", RxMessage.ExtId);
		}
#endif
	}
}

/**
  * @brief  push message to txQue_
	* @param  None
  * @retval None
  */
void CCanRouter::putMsg(CanTxMsg& refMsg)
{
	if(!txQue_.push(refMsg))
		txOverflowCount_++;
}

/**
  * @brief  attach mailbox into mailbox table
	* @param  None
  * @retval None
  */
bool CCanRouter::attachMailbox(CCanRxMailbox* pMailbox)
{
		Console::Instance()->printf("CAN%d attaching mailbox: ", CANx_ == CAN1 ? 1 : 2);
	if(pMailbox->IDE() == CAN_Id_Standard)
	{
		Console::Instance()->printf("STD 0x%X...\r\n", pMailbox->stdId());
	}else
	{
		Console::Instance()->printf("EXT 0x%X...\r\n", pMailbox->extId());
	}

	if(MAX_MAILBOX_NUM <= mailboxNum_) 
	{
		Console::Instance()->postErr("Mailbox table overflow");
		return false;
	}
	for(int i = 0; i < mailboxNum_; i++)
	{
		if(mailboxTab[i]->isIdEqual(pMailbox)) 
		{
			if(mailboxTab[i] == pMailbox)
			{
				Console::Instance()->printf("Mailbox already attached\r\n");
				return true;
			}
			else
				Console::Instance()->postErr("Mailbox id conflict");
			return false;
		}
	}
		
	Console::Instance()->printf("Attach finished.\r\n");
	mailboxTab[mailboxNum_] = pMailbox;
	mailboxNum_++;
	
	return true;
}

bool CCanRouter::isTransmitterIdel()
{
	runTransmitter();
	static uint16_t noAckCounter = 0;
	if(CAN_ErrorCode_ACKErr == CAN_GetLastErrorCode(CANx_) 
		|| CAN_ErrorCode_BitRecessiveErr == CAN_GetLastErrorCode(CANx_))
	{
		//CAN bus empty, no node reply
		if(noAckCounter++ > 3000)
			return true;
	}
	else
	{
		noAckCounter = 0;
	}
	
	return (0 == getMsgsInTxQue()
		&& CAN_TxStatus_Ok == CAN_TransmitStatus(CANx_, 0)
		&& CAN_TxStatus_Ok == CAN_TransmitStatus(CANx_, 1)
		&& CAN_TxStatus_Ok == CAN_TransmitStatus(CANx_, 2));
}
//end of file
