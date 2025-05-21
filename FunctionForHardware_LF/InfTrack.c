#include "stm32f10x.h"                  // Device header
#include "InfTrack.h"

//uint8_t sensor_state = 0;

uint8_t NextIsDes_Flag = 0;	//�յ㣨ֹͣ�㣩��־λ����С�������������һ��λ��Ϊ�յ㣨ֹͣ�㣩����1

/**
  * @breuf  	����ѭ����ʼ��
  * @param  	void
  * @retval 	void
  */
void InfTrack_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin = Track_L_Pin | Track_M_Pin | Track_R_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	
	GPIO_Init (Track_M_Port,&GPIO_InitStructure);	
}

/**
  * @breuf  	���⴫����̽��ֵ
  * @param  	void
  * @retval 	��·��������̽��ֵ
  */
uint8_t Inf_State(void)
{
	uint8_t inf_state = 0;
	
	//�����Ʒ�ʮ����
	inf_state |= GPIO_ReadInputDataBit (Track_L_Port ,Track_L_Pin) << 2;
	inf_state |= GPIO_ReadInputDataBit (Track_L_Port ,Track_L_Pin) << 1;
	inf_state |= GPIO_ReadInputDataBit (Track_L_Port ,Track_L_Pin) ;
	
	return inf_state ;
}

/**
  * @breuf  	����Ѳ��״̬
  * @param  	void
  * @retval 	���ص�ǰ����ʲô״̬
  */
uint8_t InfTrack_Line(void)
{
	uint8_t Line_State = 0;
	
	uint8_t inf_state = Inf_State ();
	
	//0Ϊ��ɫ��1Ϊ��/��
	switch (inf_state ){
		case 0:		//000
			Line_State = Line_W;
			break;
		case 1:		//001
			Line_State = Line_RR;
			break ;
		case 2:		//010
			Line_State = Line_M;
			break ;
		case 3:		//011
			Line_State = Line_R;
			break ;
		case 4:		//100
			Line_State = Line_LL;
			break ;
		case 5:		//101
			if(NextIsDes_Flag == 0){
				Line_State = Line_T;
			}else{
				Line_State = Line_Des ;
			}
			break ;
		case 6:		//110
			Line_State = Line_L;
			break ;
		case 7:		//111
			Line_State = Line_Cross ;
			break ;
		default :
			Line_State = Line_Error;
			break ;
	}
	
	return Line_State ;
}











