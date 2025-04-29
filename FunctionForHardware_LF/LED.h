#ifndef _LED_H_
#define _LED_H_

#define LED_Y_Pin		GPIO_Pin_1
#define LED_Y_Port		GPIOA
#define LED_R_Pin 		GPIO_Pin_2
#define LED_R_Port		GPIOA

#define LED_Y			1
#define LED_R			2
#define LED_ON			1
#define	LED_OFF			0

void LED_Init(void);
void LED_Light(uint8_t LED_Color,BitAction state);
void LED_Turn(uint8_t LED_Color);

#endif
