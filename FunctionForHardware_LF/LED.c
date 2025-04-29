#include "stm32f10x.h"                  // Device header
#include "LED.h"

/**
  * @breuf  	LED指示灯初始化
  * @param  	void
  * @retval 	void
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE );
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin = LED_Y_Pin | LED_R_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA,&GPIO_InitStructure );	
}

/**
  * @breuf  	设置选定的指示灯的亮灭状态
  * @param  	指示灯颜色或选定的指示灯，LED.h中定义的指示灯
  *				待设置的指示灯亮灭状态，LED_ON、LED_OFF
  * @retval 	void
  */
void LED_Light(uint8_t LED_Color,BitAction state)
{
	switch(LED_Color ){
		case LED_Y:
			GPIO_WriteBit  (LED_Y_Port ,LED_Y_Pin ,state);
			break;
		case LED_R:
			GPIO_WriteBit  (LED_R_Port ,LED_R_Pin ,state);
			break ;
		default :
			break ;	
	}		
}

/**
  * @breuf  	指示灯亮灭状态翻转
  * @param  	选定的指示灯
  * @retval 	void
  */
void LED_Turn(uint8_t LED_Color)
{
	switch(LED_Color ){
		case LED_Y:
			if(GPIO_ReadOutputDataBit (LED_Y_Port ,LED_Y) == 1){
				GPIO_ResetBits (LED_Y_Port ,LED_Y);
			}else{
				GPIO_SetBits (LED_Y_Port ,LED_Y);
			}
			break;
		case LED_R:
			if(GPIO_ReadOutputDataBit (LED_R_Port ,LED_R) == 1){
				GPIO_ResetBits (LED_R_Port ,LED_R_Pin );
			}else{
				GPIO_SetBits (LED_R_Port ,LED_R_Pin );
			}
			break ;
		default :
			break ;
	}
}












