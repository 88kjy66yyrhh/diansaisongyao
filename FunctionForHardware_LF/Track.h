#ifndef _Track_H_
#define _Track_H_

#define Line_Error	0	//未被定义的
#define Line_M		1	//红线在正中间
#define Line_L		2	//车头微偏左
#define Line_LL		3	//车头偏左端
#define Line_R		4	//车头微偏右
#define Line_RR		5	//车头偏右端
#define Line_Cross	6	//十字路口处
#define Line_T		7	//T字路口处
#define Line_Black	8	//黑线
#define Line_White	9	//白底横置在传感器下方，表明车头在纯白色区
#define Line_Red	10	//红线横置在传感器下方
#define Line_Orther	11
//#define Line_TurnLeft	


//#define Is_White		1800
//#define Is_Red		1500
#define Is_Black_Max	500

//#define Bios_Max	100
//#define Bios_Min	-100

#define White_to_Red	560
#define Bios	White_to_Red/2

extern unsigned short Is_White[5];
extern unsigned short sensor_state[5] ;

void Track_Init(void);
void Track_State(void);
uint16_t Track_Filter(uint8_t AD_channel);
uint8_t Track_Line(void);

#endif
