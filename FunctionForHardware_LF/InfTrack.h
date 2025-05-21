#ifndef _InfTrack_H_
#define _InfTrack_H_

#define Track_L_Pin		GPIO_Pin_0
#define Track_M_Pin		GPIO_Pin_1
#define Track_R_Pin		GPIO_Pin_2

#define Track_L_Port	GPIOA
#define Track_M_Port	GPIOA
#define Track_R_Port	GPIOA

#define Line_W		1		//ȫ��ɫ
#define Line_L		2		//��ͷ΢ƫ��
#define Line_M		3		//������֮��
#define Line_R		4		//��ͷ΢ƫ��
#define Line_Cross	5		//ʮ��·��
#define Line_T		6		//T��·
#define Line_LL		7		//��ͷƫ��
#define Line_RR		8		//��ͷƫ��
#define Line_Error	9		//����δ����������
#define Line_Des	10		//����Ŀ�ĵأ�ֹͣ�㣩

extern uint8_t NextIsDes_Flag;


#endif
