#include "stm32f10x.h"                  // Device header

void MYSPI_W_CSN(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue)
{
	GPIO_WriteBit(GPIOx,GPIO_pin ,(BitAction)Bitvalue);
}

void MYSPI_W_CE(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue)
{
	GPIO_WriteBit(GPIOx,GPIO_pin ,(BitAction)Bitvalue);
}

void MYSPI_W_SCK(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue)
{
	GPIO_WriteBit(GPIOx,GPIO_pin ,(BitAction)Bitvalue);
}

void MYSPI_W_MOSI(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue)
{
	GPIO_WriteBit(GPIOx,GPIO_pin ,(BitAction)Bitvalue);
}

uint8_t MYSPI_R_IRQ(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin)
{
	return GPIO_ReadInputDataBit(GPIOx ,GPIO_pin );
}

uint8_t MYSPI_R_MISO(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin)
{
	return GPIO_ReadInputDataBit(GPIOx ,GPIO_pin );
}

uint8_t MYSPI_SwapByte(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t ByteSend)  
{
	uint8_t i,ByteReceive = 0x00;
	for(i = 0; i < 8; i ++) 
	{
		MYSPI_W_MOSI (GPIOx,GPIO_pin,ByteSend & (0x80 >> i));
		MYSPI_W_SCK (GPIOx,GPIO_pin,1);
		
		if(MYSPI_R_MISO(GPIOx,GPIO_pin) == 1){ByteReceive |= (0x80 >> i);}			
		
		MYSPI_W_SCK(GPIOx,GPIO_pin,0);					
	}
	return ByteReceive;
}


