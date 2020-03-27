#include "CUsart.h"
#include "string.h"
#include <assert.h>
//
//constructor
//
CUsart::CUsart(USART_TypeDef * USARTx, 
	uint8_t* rxBuff,
	uint16_t rxBuffSize,
	uint32_t BaudRate):
		USARTx_(USARTx), 
		DmaTxChannel_(TxDMA(USARTx_)), 
		DmaRxChannel_(RxDMA(USARTx_)), 
		baudRate_(BaudRate),
		rxOverflowCount_(0),
		rxBuff_(rxBuff),
		rxBuffSize_(rxBuffSize)
{
}
	
/**
  * @brief  send char array with length.
	* @param  buf: pointer of char buffer array.
	* @param  size: length of array.
	* @retval true:	 send succeeded.
	* @retval false: DMA is working with last send and sending failed.
  */
uint16_t CUsart::send_Array(uint8_t* buf, uint8_t size)
{
	/* judge whether DMA is sending */
	if(DmaTxChannel_->CNDTR != 0) return 0;
	
	DMA_Cmd(DmaTxChannel_, DISABLE);
	
	DmaTxChannel_->CNDTR = size;
	DmaTxChannel_->CMAR = (uint32_t)buf;
	
	while((DmaTxChannel_->CCR & 1) == 0)
	{
		DMA_Cmd(DmaTxChannel_, ENABLE);
	}
	
	return 1;
}

//
//uint8_t CUsart::read_RxFifo(uint8_t* buf)
//
uint16_t CUsart::read_RxFifo(uint8_t* buf)
{
	uint16_t rxDataSize;
	DMA_Cmd(DmaRxChannel_, DISABLE);
	
	rxDataSize = rxBuffSize_ - DmaRxChannel_->CNDTR;
	if(rxDataSize == 0) 
	{
		DMA_Cmd(DmaRxChannel_, ENABLE);
		return 0;
	}
	
	/* check for overflow */
	if(rxDataSize > 10) rxOverflowCount_++;
	
	memcpy(buf, (uint8_t*)(DmaRxChannel_->CMAR), rxDataSize);
	
	DmaRxChannel_->CNDTR = rxBuffSize_;
	DMA_Cmd(DmaRxChannel_, ENABLE);
	
	return rxDataSize;
}

uint16_t CUsart::read_RxFifo(ringque_base<uint8_t>& ref_ringque)
{
	uint16_t rxDataSize;
	DMA_Cmd(DmaRxChannel_, DISABLE);
	
	rxDataSize = rxBuffSize_ - DmaRxChannel_->CNDTR;
	if(rxDataSize == 0) 
	{
		DMA_Cmd(DmaRxChannel_, ENABLE);
		return 0;
	}
	
	rxDataSize = ref_ringque.push_array((uint8_t*)(DmaRxChannel_->CMAR), rxDataSize);
	
	DmaRxChannel_->CNDTR = rxBuffSize_;
	DMA_Cmd(DmaRxChannel_, ENABLE);
	
	return rxDataSize;
}

//
//uint8_t CUsart::get_BytesInRxFifo()
//
uint16_t CUsart::get_BytesInRxFifo()
{
	uint16_t size = rxBuffSize_ - DmaRxChannel_->CNDTR;
	
	return size;
}

//
//uint8_t CUsart::get_BytesInTxFifo()
//
uint16_t CUsart::get_BytesInTxFifo()
{
	uint8_t size = DmaTxChannel_->CNDTR;
	return size;
}

/**
  * @brief  set the Baudrate of UART
	* @param  None
  * @retval None
  */
void CUsart::clear_rxFifo()
{
	uint8_t rxDataSize;
	DMA_Cmd(DmaRxChannel_, DISABLE);
	
	rxDataSize = rxBuffSize_ - DmaRxChannel_->CNDTR;
	if(rxDataSize == 0) 
	{
		DMA_Cmd(DmaRxChannel_, ENABLE);
		return ;
	}
	
	DmaRxChannel_->CNDTR = rxBuffSize_;
	DMA_Cmd(DmaRxChannel_, ENABLE);

}

//
//void CUsart::InitSciGpio(int IOGroup)
//
void CUsart::InitSciGpio()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t RCC_APB2Periph_GPIOx;
	uint8_t GPIO_PinSource_Tx;
	uint8_t GPIO_PinSource_Rx;
//	uint8_t GPIO_AF_USARTx;
	GPIO_TypeDef *GPIOx_Tx;
	GPIO_TypeDef *GPIOx_Rx;
	IOGroup_Type IOGroup = BspIOGroup(USARTx_);
	
	if(IOGroup == GROUP_A2A3)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
		GPIOx_Tx = GPIOx_Rx = GPIOA;
		GPIO_PinSource_Tx = GPIO_PinSource2;
		GPIO_PinSource_Rx = GPIO_PinSource3;
	}	
	else if(IOGroup == GROUP_A9A10)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
		GPIOx_Tx = GPIOx_Rx = GPIOB;
		GPIO_PinSource_Tx = GPIO_PinSource6;
		GPIO_PinSource_Rx = GPIO_PinSource7;
	}
	else if(IOGroup == GROUP_B10B11)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
		GPIOx_Tx = GPIOx_Rx = GPIOB;
		GPIO_PinSource_Tx = GPIO_PinSource10;
		GPIO_PinSource_Rx = GPIO_PinSource11;
	}
	else if(IOGroup == GROUP_C10C11)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
		GPIOx_Tx = GPIOx_Rx = GPIOC;
		GPIO_PinSource_Tx = GPIO_PinSource10;
		GPIO_PinSource_Rx = GPIO_PinSource11;
	}
	else if(IOGroup == GROUP_C12D2)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC;
		GPIOx_Tx = GPIOC;
		GPIOx_Rx = GPIOD;
		GPIO_PinSource_Tx = GPIO_PinSource12;
		GPIO_PinSource_Rx = GPIO_PinSource2;
	}
	else if(IOGroup == GROUP_C6C7)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
		GPIOx_Tx = GPIOx_Rx = GPIOC;
		GPIO_PinSource_Tx = GPIO_PinSource6;
		GPIO_PinSource_Rx = GPIO_PinSource7;
	}
	else seer_assert(false); //undefined!

	/* open clock of GPIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	/* Config Pin: TXD RXD*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0<< GPIO_PinSource_Tx;
	GPIO_Init(GPIOx_Tx, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0<< GPIO_PinSource_Rx;
	GPIO_Init(GPIOx_Rx, &GPIO_InitStructure);
}

/**
  * @brief  Initialize the USART, which is called SCI in DSP
	* @param  rxBuf: rxBuf address
  * @retval None
  */
void CUsart::InitSci()
{
	/* init clock of USART */
	if(USARTx_ == USART1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	else if(USARTx_ == USART2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	else if(USARTx_ == USART3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	else if(USARTx_ == UART4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	else if(USARTx_ == UART5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	else seer_assert(false);
	
	/* Deinitializes the USARTx */
	USART_DeInit(USARTx_);
	
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = baudRate_; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	
	USART_Cmd(USARTx_, DISABLE); 
	USART_Init(USARTx_, &USART_InitStructure); 
	USART_Cmd(USARTx_, ENABLE); 
	
	USART_DMACmd(USARTx_, USART_DMAReq_Tx, DISABLE);
	USART_DMACmd(USARTx_, USART_DMAReq_Rx, DISABLE);
	
	/* DMA Clock Config */
	uint32_t RCC_AHB1Periph;
	DMA_TypeDef * DMAx = ((DMA_TypeDef *)((uint32_t)DmaTxChannel_&0xFFFFFC00));
	
	if(DMAx == DMA1) RCC_AHB1Periph = RCC_AHBPeriph_DMA1;
	else if(DMAx == DMA2) RCC_AHB1Periph = RCC_AHBPeriph_DMA2;
	else seer_assert(false); //error
	RCC_AHBPeriphClockCmd(RCC_AHB1Periph, ENABLE);
	
	DMA_DeInit(DmaTxChannel_);
	DMA_DeInit(DmaRxChannel_);
	
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/* TX DMA Config */	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USARTx_->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)"0";
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;
	
	DMA_Cmd(DmaTxChannel_, DISABLE);
	DMA_Init(DmaTxChannel_, &DMA_InitStructure);
	
	/* RX DMA Config */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USARTx_->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rxBuff_;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = CUsart::rxBuffSize_;
	
	DMA_Cmd(DmaRxChannel_, DISABLE);
	DMA_Init(DmaRxChannel_, &DMA_InitStructure);
	DMA_Cmd(DmaRxChannel_, ENABLE);
	
	USART_DMACmd(USARTx_, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USARTx_, USART_DMAReq_Rx, ENABLE);
}

/**
  * @brief  set the Baudrate of UART
	* @param  None
  * @retval None
  */
void CUsart::setBaudrate(uint32_t BaudRate = 0)
{
	uint32_t tmpreg = 0x00, apbclock = 0x00;
  uint32_t integerdivider = 0x00;
  uint32_t fractionaldivider = 0x00;
	uint16_t CR1_OVER8_Set = (u16)0x8000;
	
  RCC_ClocksTypeDef RCC_ClocksStatus;
	
	/* update baudrate_ if BaudRate is effective*/
	if(BaudRate > 0) baudRate_ = BaudRate;
	
	/* check if the Usart is opened */
	bool isUsartOpen = (USARTx_->CR1 & USART_CR1_UE);
	
	/* close the USART if it is opened */
	if(isUsartOpen) USART_Cmd(USARTx_, DISABLE);
	
	assert_param(IS_USART_BAUDRATE(baudRate_));  
	/* Configure the USART Baud Rate */
  RCC_GetClocksFreq(&RCC_ClocksStatus);

  if (USARTx_ == USART1)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  
   /* Determine the integer part */
  if ((USARTx_->CR1 & CR1_OVER8_Set) != 0)
  {
    /* Integer part computing in case Oversampling mode is 8 Samples */
    integerdivider = ((25 * apbclock) / (2 * (BaudRate)));    
  }
  else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  {
    /* Integer part computing in case Oversampling mode is 16 Samples */
    integerdivider = ((25 * apbclock) / (4 * (BaudRate)));    
  }
  tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
  fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
  if ((USARTx_->CR1 & CR1_OVER8_Set) != 0)
  {
    tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  }
  else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  {
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  }
  
  /* Write to USART BRR register */
  USARTx_->BRR = (uint16_t)tmpreg;
	
	/* recover the state of USART */
	if(isUsartOpen) USART_Cmd(USARTx_, ENABLE);
}

/**
  * @brief  get overflow count of Rx buffer
	* @param  None
  * @retval Number of overflow
  */
uint8_t CUsart::getRxOverflowCount()
{
	return rxOverflowCount_;
}

/**
  * @brief  TxDMA()
	* @param  None
  * @retval Tx Dma Channel of the Usart
  */
DMA_Channel_TypeDef * CUsart::TxDMA(USART_TypeDef * targetUart)
{
	if(USART1 == targetUart)
		return DMA1_Channel4;
	else if(USART2 == targetUart)
		return DMA1_Channel7;
	else if(USART3 == targetUart)
		return DMA1_Channel2;
	else if(UART4 == targetUart)
		return DMA2_Channel5;
	else
		seer_assert(false);
	
	//program should not come here
	return DMA1_Channel4;
}

/**
  * @brief  TxDMA()
	* @param  None
  * @retval Tx Dma Channel of the Usart
  */
DMA_Channel_TypeDef * CUsart::RxDMA(USART_TypeDef * targetUart)
{
	if 	(targetUart == USART1) 
		return	DMA1_Channel5;	
	else if 	(targetUart == USART2)
		return	DMA1_Channel6;	
	else if 	(targetUart == USART3)
		return	DMA1_Channel3;
	else if 	(targetUart == UART4)
		return	DMA2_Channel3;
	else
		seer_assert(false);
	
	//program should not come here
	return DMA1_Channel4;
}

/**
  * @brief  BspIOGroup
	* @param  None
  * @retval Tx Dma Channel of the Usart
  */
CUsart::IOGroup_Type CUsart::BspIOGroup(USART_TypeDef * targetUart)
{
	if(targetUart == USART1) 
		return	GROUP_A9A10;	
	if(targetUart == USART2) 
		return GROUP_A2A3;
	else
		seer_assert(false);
	
	//program should not come here
	return GROUP_A9A10;
}

//
//destructor
//
CUsart::~CUsart()
{
}
//end of file
