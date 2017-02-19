
/**
*@file USART.h
*@author 2015-20-28 Neutree
*@version v1.1
*@brief stm32f10x串口驱动文件，使用时引入 USART.h USART.cpp FIFOBuffer.h Interrupt.h Interrupt.cpp 五个文件，
*       然后根据需要配置USART.h中开头的配置部分（缓冲区大小配置）
*       中断均由Interrupt.cpp中管理，若需要自定义，请自行编辑中断函数
*       然后定义对象初始化，既可以使用
*@copyright CQUTIOTLIB all right reserved
*
*/
#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"
#include "FIFOBuffer.h"
#include "Configuration.h"
#include <string.h>


/**
  *@addtogroup USART_CONFIGURATION
  *@{
  */
/******************************************************************************************************/
/****configuration，使用前请自行配置****/
#define USART_TX_BUFFER_SIZE     600              //USART BUFFER FIFO SIZE
#define USART_RX_BUFFER_SIZE     200              //USART BUFFER FIFO SIZE
#define USART_DMA_TX_BUFFER_SIZE 200             //USART DMA BUFFER SIZE

/*******************************************************************************************************/

class USART
{
private:
	bool isBusySend;

	u32            mBaudrate; //baudrate of usart
	u16            mParity;   //parity of usart
	u16            mWordLen;  //world length of usart
	u16            mStopBits; //stop bits of usart
	u16            mTxPin;    //Tx gpio pin
	u16            mRxPin;    //Rx gpio pin
	u16            mTxPinSource;    //Tx gpio pinSource
	u16            mRxPinSource;    //Rx gpio pinSource
	u8             mIRQn;     //USART IRQn
	u8             mGPIOAF;   //GPIO AF
	GPIO_TypeDef*  mTxPort;     //GPIO port
	GPIO_TypeDef*  mRxPort;     //GPIO port
	USART_TypeDef* mUSARTx;   //USARTx
	u32            mGPIORcc;  //GPIO Clock
	u32            mUSARTRcc; //USART Clock
	bool           mRemap;    //gpio remap flag
	u8             mPrePri;   //preemption priority
	u8             mSubPri;   //sub priority
	u8             mPriGroup; //priority group
	FIFOBuffer<u8, USART_TX_BUFFER_SIZE>  mTxBuf;  //USART Tx Buffer
	FIFOBuffer<u8, USART_RX_BUFFER_SIZE>  mRxBuf;  //USART Rx Buffer

	u8 mPrecision;   //when show precision after dot "."  when use "<<" to show float value

	u16 mTxOverflow; //Tx overflow byte count
	u16 mRxOverflow; //Rx overflow byte count

private:
	void InitGPIO();
	void InitUSART();
	void InitNVIC();
public:
	USART(USART_TypeDef* USARTx, u32 baud, u8 priGroup = 3, u8 prePri = 7, u8 subPri = 1, bool remap = false, u16 parity = USART_Parity_No, u16 wordLen = USART_WordLength_8b, u16 stopBits = USART_StopBits_1);
	void Initialize();

	virtual bool SendBytes(u8 txData[], u16 size);
	virtual bool SendByte(u8 data);
	virtual bool GetBytes(u8 data[], u16 num);
	virtual bool GetByte(u8 &data);

	virtual u16 TxSize();
	virtual u16 RxSize();

	virtual u16 TxOverflowSize();
	virtual u16 RxOverflowSize();

	bool CheckFrame(DataFrame &df);

	void IRQ();

	USART& operator<<(int val);
	USART& operator<<(double val);
	USART& operator<<(const char* pStr);
	USART& operator<<(u8* pStr);

#ifdef USE_USART_DMA
private:
	u8                   mDMAIRQn;
	u32									 mDMATxCh;
	DMA_Stream_TypeDef*  mDMA_Streamx;//DMA data stream
	u32                  mDMATCFlag;
	u32                  mDMAGLFlag;
	u8                   mDMATxBuf[USART_DMA_TX_BUFFER_SIZE];
	void InitDMA();
public:
	void DMAIRQ();
#endif
};

/**
	  ===========================================================================
	  |			remap value				|
	  usartName	|	0x00	|	0x01	|	0x11	|
	  usart1Tx	|	PA9		|	PB6		|			|
	  usart1Rx	|	PA10	|	PB7		|			|
	  usart2Tx	|	PA2		|	PD5		|			|
	  usart2Rx	|	PA3		|	PD6		|			|
	  usart3Tx	|	PB10	|	PC10	|	PD8		|
	  usart3Rx	|	PB11	|	PC11	|	PD9		|
	  ===========================================================================
	  */

#endif
