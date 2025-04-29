#include "stm32f10x.h"                  // Device header
#include "NRF24L01_ins.h"
//#include "MYSPI.h"
#include "Delay.h"

#define TX_ADR_WIDTH    5     //5字节地址宽度
#define RX_ADR_WIDTH    5     //5字节地址宽度
#define TX_PLOAD_WIDTH  32    //32字节有效数据宽度
#define RX_PLOAD_WIDTH  32    //32字节有效数据宽度

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //接收地址

/**
  * @breuf  	引脚初始化
  * @param  	void
  * @retval 	void
  */
void NRF_Pin_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB ,ENABLE );
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CSN_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(CSN_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CE_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CE_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOSI_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SCK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCK_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = IRQ_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IRQ_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = MISO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MISO_Port, &GPIO_InitStructure);
	
}

void NRF_W_CSN(uint8_t Bitvalue)
{
	GPIO_WriteBit(CSN_Port ,CSN_Pin ,(BitAction)Bitvalue);
}

void NRF_W_CE(uint8_t Bitvalue)
{
	GPIO_WriteBit(CE_Port ,CE_Pin ,(BitAction)Bitvalue);
}

void NRF_W_SCK(uint8_t Bitvalue)
{
	GPIO_WriteBit(SCK_Port ,SCK_Pin,(BitAction)Bitvalue);
}

void NRF_W_MOSI(uint8_t Bitvalue)
{
	GPIO_WriteBit(MOSI_Port ,MOSI_Pin ,(BitAction)Bitvalue);
}

uint8_t NRF_R_IRQ()
{
	return GPIO_ReadInputDataBit(IRQ_Port ,IRQ_Pin);
}

uint8_t NRF_R_MISO()
{
	return GPIO_ReadInputDataBit(MISO_Port ,MISO_Pin);
}

/**
  * @breuf  	交换一个字节
  * @param  	要发送的字节
  * @retval 	接收的字节
  */
uint8_t NRF_SwapByte(uint8_t ByteSend)  
{
	uint8_t i,ByteReceive = 0x00;
	for(i = 0; i < 8; i ++) 
	{
		NRF_W_MOSI (ByteSend & (0x80 >> i));
		NRF_W_SCK (1);
		
		if(NRF_R_MISO() == 1){ByteReceive |= (0x80 >> i);}			
		
		NRF_W_SCK(0);					
	}
	return ByteReceive;
}

/**
  * @breuf  	写寄存器
  * @param  	寄存器地址，要写入的值
  * @retval 	状态值
  */
uint8_t NRF_W_Reg(uint8_t Reg, uint8_t value)
{
	uint8_t status;
	
	NRF_W_CSN (0);
	status = NRF_SwapByte (Reg);
	NRF_SwapByte (value );
	NRF_W_CSN (1);
	
	return status ;
}

/**
  * @breuf  	读寄存器
  * @param  	寄存器地址
  * @retval 	指定寄存器的内容
  */
uint8_t NRF_R_Reg(uint8_t Reg)
{
	uint8_t value;
	
	NRF_W_CSN (0);
	NRF_SwapByte (Reg );
	value = NRF_SwapByte (NOP);
	NRF_W_CSN (1);
	
	return value ;
}

/**
  * @breuf  	写数据，一次写多个字节
  * @param  	寄存器地址，要写入的数据数组的首地址，长度（字节数）
  * @retval 	此次读到的状态寄存器值
  */
uint8_t NRF_W_Buf(uint8_t Reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status, i;
	NRF_W_CSN (0);
	status = NRF_SwapByte (Reg );
	for(i=0;i<len;i++){
		NRF_SwapByte (*pBuf ++);
	}
	NRF_W_CSN (1);
	
	return status;
}

/**
  * @breuf  	读数据，一次读多个数据
  * @param  	寄存器地址，所读取数据要存放的地址，数据长度
  * @retval 	此次读取到的状态寄存器的值
  */
uint8_t NRF_R_Buf(uint8_t Reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status, i;
	NRF_W_CSN (0);
	status = NRF_SwapByte (Reg );
	for(i=0; i<len; i++){
		pBuf[i] = NRF_SwapByte (NOP);
	}
	NRF_W_CSN (1);
	
	return status ;
}

/**
  * @breuf  	发送数据（包）
  * @param  	待发送数据的首地址
  * @retval 	发送完成状况；TX_OK(0x20):接收到应答，发送完成；MAX_TX(0x10)：达到最大自动重传次数，数据发送失败；
  *				NOP，其他原因失败
  */
uint8_t NRF_SendTxPacket(uint8_t *pTxBuf)
{
	uint8_t state;
	
	NRF_W_CE (0);
	NRF_W_Buf (W_TX_PAYLOAD ,pTxBuf ,TX_PLOAD_WIDTH );
	NRF_W_CE (1);
	while(NRF_R_IRQ () != 0);
	state = NRF_R_Reg (STATUS );
	NRF_W_Reg (NRF24_W_REG + STATUS ,state);
	if(state & MAX_TX){
		NRF_W_Reg (FLUSH_TX ,NOP );
		return MAX_TX ;
	}
	if(state & TX_OK ){
		return TX_OK;
	}
	return NOP;
}

/**
  * @breuf  	接收(读取)数据(包)
  * @param  	读取到的数据要存放的首地址
  * @retval 	读取接收情况，0：接收成功，1：接收失败
  */
uint8_t NRF_GetRxPacket(uint8_t *pRxBuf)
{
	uint8_t state;
	state = NRF_R_Reg (STATUS);
	NRF_W_Reg(NRF24_W_REG + STATUS , state );
	if(state & RX_OK ){
		NRF_W_CE (1);
		NRF_R_Buf (R_RX_PAYLOAD ,pRxBuf ,RX_PLOAD_WIDTH);
		NRF_W_Reg (FLUSH_RX ,NOP);
		NRF_W_CE (1);
		Delay_us (150);
		
		return 0;
	}
	
	return 1;
}

/**
  * @breuf  	检测是否存在NRF24L01+，用于MCU检测本机是否存在该外设
  * @param  	void
  * @retval 	1：不存在，0：存在
  */
uint8_t NRF_Cheak(void)
{
	uint8_t cheak_in[5] = {0xA5,0xA5,0xA5,0xA5,0xA5};
	uint8_t cheak_out[5] = {0x00};
	
	NRF_W_SCK (0);
	NRF_W_CSN (1);
	NRF_W_CE (0);
	
	NRF_W_Buf (NRF24_W_REG + TX_ADDR ,cheak_in ,5);
	NRF_R_Buf (NRF24_R_REG + TX_ADDR ,cheak_out,5);
	
	for(uint8_t i=0; i<5; i++){
		if(cheak_out [i] != 0xA5){
			return 1;
		}
	}
	
	return 0;
}

/**
  * @breuf  	NRF24L01+收发初始化
  * @param  	void
  * @retval 	void
  */
void NRF_RT_Init(void)
{
	NRF_W_CE (0);
	NRF_W_Reg (NRF24_W_REG + RX_PW_P0 ,RX_PLOAD_WIDTH );						//选择通道0的有效数据宽度
	NRF_W_Reg (FLUSH_RX ,NOP );													//清空RX FIFO寄存器
	NRF_W_Buf (NRF24_W_REG + TX_ADDR ,(uint8_t *)TX_ADDRESS ,TX_ADR_WIDTH );	//写TX节点地址	
	NRF_W_Buf (NRF24_W_REG + RX_ADDR_P0 ,(uint8_t *)RX_ADDRESS ,RX_ADR_WIDTH );	//设置RX节点地址
	NRF_W_Reg (NRF24_W_REG + EN_AA ,0x01);				//使能自动应答
	NRF_W_Reg (NRF24_W_REG + EN_RXADDR ,0x01);			//使能通道0的接收地址
	NRF_W_Reg (NRF24_W_REG + SETUP_RETR ,0x1A);			//设置自动重发间隔时间：500us+86us;最大重发次数：10次
	NRF_W_Reg (NRF24_W_REG + RF_CH, 0);					//设置RF通道频率
	NRF_W_Reg (NRF24_W_REG + RF_SETUP ,0x0F);			//设置TX发送参数，0db增益，2Mbps,低噪声增益开启
	NRF_W_Reg (NRF24_W_REG + CONFIG , 0x0F);			//配置基本工作模式参数；PWR_UP,EN_CRC,16BIT_CRC,接收模式
	NRF_W_CE (1); 
}

/**
  * @breuf  	NRF24L01+初始化
  * @param  	void
  * @retval 	void
  */
void NRF_Init(void)
{
	NRF_Pin_Init ();
	while(NRF_Cheak ());
	NRF_RT_Init();
}

/**
  * @breuf  	发送模式下发送数据
				该函数将配置寄存器(CONFIG)设置位发送模式(PTX)，并调用NRF_SendTxPacket()发送数据，
				发送后，配置位接收模式(PRX)
  * @param  	待发送数据首地址
  * @retval 	void
  */
void NRF_SendBuf(uint8_t *Buf)
{
	NRF_W_CE (0);
	NRF_W_Reg (NRF24_W_REG + CONFIG ,0x0E);
	NRF_W_CE(1);
	Delay_us (15);
	NRF_SendTxPacket (Buf);
	NRF_W_CE (0);
	NRF_W_Reg (NRF24_W_REG + CONFIG ,0x0F);
	NRF_W_CE (1);
}

/**
  * @breuf  	获取中断标志，接收到数据后触发中断，IRQ拉低，即0为接收到数据
  * @param  	void
  * @retval 	0：接收到数据，1：未接收到数据
  */
uint8_t NRF_GetValueFlag(void)
{
	return NRF_R_IRQ ();
}

void NRF_SendFrames(uint8_t *Buf)
{
	
}



/************************************示例******************************/

/*
int main()
{
	//发送数据//
	uint8_t Txbuf[32] = {};	//可发送HEX数据，文本数据；可封装成数据包发送
	uint8_t Rxbuf[32] = {};	//用于接收数据的数组

	NRF_Init();
	NRF_SendBuf(Txbuf);

	///////////

	while(1)
	{
		if(NRF_GetValueFlag () == 0){
			NRF_GetRxPacket (Rxbuf);
			//若以数据包形式通信，需进行解数据包，判断包头包尾是否符合协议，提取有效数据		
		}
		
	}
	
}








*/




