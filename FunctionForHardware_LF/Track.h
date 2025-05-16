#ifndef _Track_H_
#define _Track_H_

#define Line_Error	0	//δ�������
#define Line_M		1	//���������м�
#define Line_L		2	//��ͷ΢ƫ��
#define Line_LL		3	//��ͷƫ���
#define Line_R		4	//��ͷ΢ƫ��
#define Line_RR		5	//��ͷƫ�Ҷ�
#define Line_Cross	6	//ʮ��·�ڴ�
#define Line_T		7	//T��·�ڴ�
#define Line_Black	8	//����
#define Line_White	9	//�׵׺����ڴ������·���������ͷ�ڴ���ɫ��
#define Line_Red	10	//���ߺ����ڴ������·�
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
