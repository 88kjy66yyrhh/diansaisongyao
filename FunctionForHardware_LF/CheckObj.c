#include "stm32f10x.h"                  // Device header
#include "CheckObj.h"

/**
  * @breuf  	初始化对应引脚，对应引脚即为红外发射模块的对应引脚
  * @param  	void
  * @retval 	void
  */
void ChekObj_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin = Infrared_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	
	GPIO_Init (Infrared_Port,&GPIO_InitStructure);	
}

/**
  * @breuf  	采用红外发射模块检测物体是否存在，模块对侧需为黑色
  * @param  	void
  * @retval 	物体是否存在，1为存在，0为不存在
  */
uint8_t CheckObj_Exist(void)
{
	return (GPIO_ReadInputDataBit (Infrared_Port ,Infrared_Pin ));
}









