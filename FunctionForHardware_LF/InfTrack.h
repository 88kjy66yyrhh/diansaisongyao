#ifndef _InfTrack_H_
#define _InfTrack_H_

#define Track_L_Pin		GPIO_Pin_0
#define Track_M_Pin		GPIO_Pin_1
#define Track_R_Pin		GPIO_Pin_2

#define Track_L_Port	GPIOA
#define Track_M_Port	GPIOA
#define Track_R_Port	GPIOA

#define Line_W		1		//全白色
#define Line_L		2		//车头微偏左
#define Line_M		3		//线在正之间
#define Line_R		4		//车头微偏右
#define Line_Cross	5		//十字路口
#define Line_T		6		//T字路
#define Line_LL		7		//车头偏左
#define Line_RR		8		//车头偏右
#define Line_Error	9		//其他未被定义的情况
#define Line_Des	10		//到达目的地（停止点）

extern uint8_t NextIsDes_Flag;


#endif
