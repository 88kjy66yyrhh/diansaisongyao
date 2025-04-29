#include "stm32f10x.h"                  // Device header
#include "NRF24L01_ins.h"
//#include "MYSPI.h"
#include "Delay.h"

#define TX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define RX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define TX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��
#define RX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //���յ�ַ

/**
  * @breuf  	���ų�ʼ��
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
  * @breuf  	����һ���ֽ�
  * @param  	Ҫ���͵��ֽ�
  * @retval 	���յ��ֽ�
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
  * @breuf  	д�Ĵ���
  * @param  	�Ĵ�����ַ��Ҫд���ֵ
  * @retval 	״ֵ̬
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
  * @breuf  	���Ĵ���
  * @param  	�Ĵ�����ַ
  * @retval 	ָ���Ĵ���������
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
  * @breuf  	д���ݣ�һ��д����ֽ�
  * @param  	�Ĵ�����ַ��Ҫд�������������׵�ַ�����ȣ��ֽ�����
  * @retval 	�˴ζ�����״̬�Ĵ���ֵ
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
  * @breuf  	�����ݣ�һ�ζ��������
  * @param  	�Ĵ�����ַ������ȡ����Ҫ��ŵĵ�ַ�����ݳ���
  * @retval 	�˴ζ�ȡ����״̬�Ĵ�����ֵ
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
  * @breuf  	�������ݣ�����
  * @param  	���������ݵ��׵�ַ
  * @retval 	�������״����TX_OK(0x20):���յ�Ӧ�𣬷�����ɣ�MAX_TX(0x10)���ﵽ����Զ��ش����������ݷ���ʧ�ܣ�
  *				NOP������ԭ��ʧ��
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
  * @breuf  	����(��ȡ)����(��)
  * @param  	��ȡ��������Ҫ��ŵ��׵�ַ
  * @retval 	��ȡ���������0�����ճɹ���1������ʧ��
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
  * @breuf  	����Ƿ����NRF24L01+������MCU��Ȿ���Ƿ���ڸ�����
  * @param  	void
  * @retval 	1�������ڣ�0������
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
  * @breuf  	NRF24L01+�շ���ʼ��
  * @param  	void
  * @retval 	void
  */
void NRF_RT_Init(void)
{
	NRF_W_CE (0);
	NRF_W_Reg (NRF24_W_REG + RX_PW_P0 ,RX_PLOAD_WIDTH );						//ѡ��ͨ��0����Ч���ݿ��
	NRF_W_Reg (FLUSH_RX ,NOP );													//���RX FIFO�Ĵ���
	NRF_W_Buf (NRF24_W_REG + TX_ADDR ,(uint8_t *)TX_ADDRESS ,TX_ADR_WIDTH );	//дTX�ڵ��ַ	
	NRF_W_Buf (NRF24_W_REG + RX_ADDR_P0 ,(uint8_t *)RX_ADDRESS ,RX_ADR_WIDTH );	//����RX�ڵ��ַ
	NRF_W_Reg (NRF24_W_REG + EN_AA ,0x01);				//ʹ���Զ�Ӧ��
	NRF_W_Reg (NRF24_W_REG + EN_RXADDR ,0x01);			//ʹ��ͨ��0�Ľ��յ�ַ
	NRF_W_Reg (NRF24_W_REG + SETUP_RETR ,0x1A);			//�����Զ��ط����ʱ�䣺500us+86us;����ط�������10��
	NRF_W_Reg (NRF24_W_REG + RF_CH, 0);					//����RFͨ��Ƶ��
	NRF_W_Reg (NRF24_W_REG + RF_SETUP ,0x0F);			//����TX���Ͳ�����0db���棬2Mbps,���������濪��
	NRF_W_Reg (NRF24_W_REG + CONFIG , 0x0F);			//���û�������ģʽ������PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
	NRF_W_CE (1); 
}

/**
  * @breuf  	NRF24L01+��ʼ��
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
  * @breuf  	����ģʽ�·�������
				�ú��������üĴ���(CONFIG)����λ����ģʽ(PTX)��������NRF_SendTxPacket()�������ݣ�
				���ͺ�����λ����ģʽ(PRX)
  * @param  	�����������׵�ַ
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
  * @breuf  	��ȡ�жϱ�־�����յ����ݺ󴥷��жϣ�IRQ���ͣ���0Ϊ���յ�����
  * @param  	void
  * @retval 	0�����յ����ݣ�1��δ���յ�����
  */
uint8_t NRF_GetValueFlag(void)
{
	return NRF_R_IRQ ();
}

void NRF_SendFrames(uint8_t *Buf)
{
	
}



/************************************ʾ��******************************/

/*
int main()
{
	//��������//
	uint8_t Txbuf[32] = {};	//�ɷ���HEX���ݣ��ı����ݣ��ɷ�װ�����ݰ�����
	uint8_t Rxbuf[32] = {};	//���ڽ������ݵ�����

	NRF_Init();
	NRF_SendBuf(Txbuf);

	///////////

	while(1)
	{
		if(NRF_GetValueFlag () == 0){
			NRF_GetRxPacket (Rxbuf);
			//�������ݰ���ʽͨ�ţ�����н����ݰ����жϰ�ͷ��β�Ƿ����Э�飬��ȡ��Ч����		
		}
		
	}
	
}








*/




