#ifndef _NRF24L01_H_
#define _NRF24L01_H_

void NRF_Pin_Init(void);
void NRF_W_CSN(uint8_t Bitvalue);
void NRF_W_CE(uint8_t Bitvalue);
void NRF_W_SCK(uint8_t Bitvalue);
void NRF_W_MOSI(uint8_t Bitvalue);
uint8_t NRF_R_IRQ(void);
uint8_t NRF_R_MISO(void);
uint8_t NRF_SwapByte(uint8_t ByteSend);
uint8_t NRF_W_Reg(uint8_t Reg, uint8_t value);
uint8_t NRF_R_Reg(uint8_t Reg);
uint8_t NRF_W_Buf(uint8_t Reg, uint8_t *pBuf, uint8_t len);
uint8_t NRF_R_Buf(uint8_t Reg, uint8_t *pBuf, uint8_t len);
uint8_t NRF_SendTxPacket(uint8_t *pTxBuf);
uint8_t NRF_GetRxPacket(uint8_t *pRxBuf);
uint8_t NRF_Cheak(void);
void NRF_RT_Init(void);
void NRF_Init(void);
void NRF_SendBuf(uint8_t *Buf);
uint8_t NRF_GetValueFlag(void);

#endif
