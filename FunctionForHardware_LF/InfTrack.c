#include "stm32f10x.h"                  // Device header
#include "InfTrack.h"

//uint8_t sensor_state = 0;

uint8_t NextIsDes_Flag = 0;	//终点（停止点）标志位：若小车即将到达的下一个位置为终点（停止点）则置1

/**
  * @breuf  	红外循迹初始化
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
  * @breuf  	红外传感器探测值
  * @param  	void
  * @retval 	多路传感器的探测值
  */
uint8_t Inf_State(void)
{
	uint8_t inf_state = 0;
	
	//二进制非十进制
	inf_state |= GPIO_ReadInputDataBit (Track_L_Port ,Track_L_Pin) << 2;
	inf_state |= GPIO_ReadInputDataBit (Track_L_Port ,Track_L_Pin) << 1;
	inf_state |= GPIO_ReadInputDataBit (Track_L_Port ,Track_L_Pin) ;
	
	return inf_state ;
}

/**
  * @breuf  	红外巡线状态
  * @param  	void
  * @retval 	返回当前处在什么状态
  */
uint8_t InfTrack_Line(void)
{
	uint8_t Line_State = 0;
	
	uint8_t inf_state = Inf_State ();
	
	//0为白色，1为红/黑
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











