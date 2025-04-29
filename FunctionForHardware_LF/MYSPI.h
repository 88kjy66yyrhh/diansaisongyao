#ifndef _MYSPI_H_
#define _MYSPI_H_

void MYSPI_W_CSN(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue);
void MYSPI_W_CE(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue);
void MYSPI_W_SCK(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue);
void MYSPI_W_MOSI(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t Bitvalue);
uint8_t MYSPI_R_IRQ(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin);
uint8_t MYSPI_R_MISO(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin);
uint8_t MYSPI_SwapByte(GPIO_TypeDef *GPIOx, uint16_t GPIO_pin, uint8_t ByteSend);

#endif
