#include "stm32f10x.h"                  // Device header
#include "CheckObj.h"

/**
  * @breuf  	��ʼ����Ӧ���ţ���Ӧ���ż�Ϊ���ⷢ��ģ��Ķ�Ӧ����
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
  * @breuf  	���ú��ⷢ��ģ���������Ƿ���ڣ�ģ��Բ���Ϊ��ɫ
  * @param  	void
  * @retval 	�����Ƿ���ڣ�1Ϊ���ڣ�0Ϊ������
  */
uint8_t CheckObj_Exist(void)
{
	return (GPIO_ReadInputDataBit (Infrared_Port ,Infrared_Pin ));
}









