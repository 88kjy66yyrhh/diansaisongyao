#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

uint8_t Serial_1_RxData;
uint8_t Serial_1_RxFlag;

uint8_t Serial_2_RxData;
uint8_t Serial_2_RxFlag;

uint8_t Serial_Select = 1;			//����ѡ��

uint8_t Package_Flag = 1;			//���ݸ�ʽ��1������HEX���ݣ�2��HEX���ݰ���3���ı����ݰ�

//����ת����ͨ�ŵ����ݰ�
uint8_t Serial_1H_TxPacket[4];				//���巢��HEX���ݰ����飬���ݰ���ʽ��FF 01 02 03 04 FE
uint8_t Serial_1H_RxPacket[4];				//�������HEX���ݰ�����

char Serial_1T_RxPacket[100];				//��������ı����ݰ����飬���ݰ���ʽ"@MSG\r\n"

//����ת�������õĴ��ڣ�USART1
void Serial_B_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

/*USART2*/
void Serial_2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);	//USART2������APB1������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//�������ڣ������ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}


/**
  * @breuf  	
  * @param  	Byte:Ҫ���͵��ֽ�; select��ѡ��Ҫ�������ݵĴ��ڣ�1��2
  * @retval 	
  */
void Serial_SendByte(uint8_t Byte, uint8_t select)	
{
	if(select == 1){
		USART_SendData(USART1, Byte);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	else if(select == 2){
		USART_SendData(USART2, Byte);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}

}

void Serial_SendArray(uint8_t *Array, uint16_t Length, uint8_t select)
{
	
	uint16_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i],select );
	}
}

void Serial_SendString(char *String, uint8_t select)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i], select );
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length, uint8_t select)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0',select );
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch, Serial_Select  ); 
	return ch;
}

void Serial_Printf( uint8_t select,char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String,select);
}

/**
  * @breuf  	�ж����ݱ�־λ
  * @param  	void 
  * @retval 	0��δ���յ���Ч���ݣ�1�����յ�����HEX����
  *				2�����յ�HEX����֡	3�����յ�Ϊ�ı�����֡
  */
uint8_t Serial_1_GetRxFlag(void)
{
	if (Serial_1_RxFlag == 1)
	{
		Serial_1_RxFlag = 0;
		if(Package_Flag == 1)			//���յ���Ϊ��������
			return 1;
		else if(Package_Flag ==2)		//���յ���ΪHEX���ݰ�					
			return 2;
		else if(Package_Flag == 3)		//���յ���Ϊ�ı����ݰ�
			return 3;
	}
	return 0;
}

/**
  * @breuf  	��ȡ���յ�����
  *				���յ�����������Ϊ����HEX��������øú�����ȡ���ݣ�
  *				��Ϊ���ݰ�����ֱ�Ӳ�����Ӧ�����ݻ�������
  * @param  	void
  * @retval 	����HEX����
  */
uint8_t Serial_1_GetRxData(void)
{
//	return Serial_1_RxData;
	uint8_t temp = Serial_1_RxData;
	Serial_1_RxData = 0x00;
	return temp;
}


/**
  * ��    �������ڷ���HEX���ݰ�
  * ��    ������
  * �� �� ֵ����
  * ˵    �������ô˺�����Serial_TxPacket��������ݽ����ϰ�ͷ��FF����β��FE������Ϊ���ݰ����ͳ�ȥ
  */
void Serial_1_SendPacket(void)
{
	Serial_SendByte(0xFF,1);
	Serial_SendArray(Serial_1H_TxPacket, 4,1);
	Serial_SendByte(0xFE,1);
}


/**
  * @breuf  	USART1�жϺ���
  *				���жϺ������жϽ��յ����������ͣ��е���HEX���ݡ�HEX����֡(��Ч����Ϊ4����������)���ı�����֡
  * @param  	void
  * @retval 	void
  */
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
	static uint8_t HpRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	static uint8_t TpRxPacket = 0;	//�����ʾ��ǰ��������λ�õľ�̬����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		Serial_1_RxData = USART_ReceiveData(USART1);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		/*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
		
		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)
		{
			if (Serial_1_RxData == 0xFF)			//�������ȷʵ��HEX���ݰ�ͷ
			{
				RxState = 1;			//����һ��״̬
				HpRxPacket = 0;			//���ݰ���λ�ù���
				Package_Flag = 10;		//��ʾ���ڽ���HEX���ݰ�
			}
			else if (Serial_1_RxData == '@' && Serial_1_RxFlag == 0)		//�������ȷʵ���ı����ݰ�ͷ��������һ�����ݰ��Ѵ������
			{
				RxState = 1;			//����һ��״̬
				memset (Serial_1T_RxPacket ,'\0',sizeof (Serial_1T_RxPacket ));	//����һ�ν��յ����ݰ����
				TpRxPacket = 0;			//���ݰ���λ�ù���
				Package_Flag = 11;		//��ʾ���ڽ����ı����ݰ�
			}
			else{						//���ݲ��ǰ�ͷ����Ϊ�������͵�����
					Serial_1_RxData = USART_ReceiveData(USART1);
					Serial_1_RxFlag = 1;
					Package_Flag = 1;
					RxState = 0;
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ�����*/
		else if (RxState == 1)
		{
			if(Package_Flag == 10){
				Serial_1H_RxPacket[HpRxPacket] = Serial_1_RxData;	//�����ݴ������ݰ������ָ��λ��
				HpRxPacket ++;				//���ݰ���λ������
				if (HpRxPacket >= 4)			//����չ�4������
				{
					RxState = 2;			//����һ��״̬
				}
			}
			else if(Package_Flag == 11){
				if (Serial_1_RxData == '\r')			//����յ���һ����β
				{
					RxState = 2;			//����һ��״̬
				}
				else						//���յ�������������
				{
					Serial_1T_RxPacket[TpRxPacket] = Serial_1_RxData;		//�����ݴ������ݰ������ָ��λ��
					TpRxPacket ++;			//���ݰ���λ������
				}
			}
		}
		/*��ǰ״̬Ϊ2���������ݰ���β*/
		else if (RxState == 2)
		{
			if(Package_Flag == 10){
				if (Serial_1_RxData == 0xFE)			//�������ȷʵ�ǰ�β��
				{
					RxState = 0;			//״̬��0
					Serial_1_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
					Package_Flag = 2;
				}
			}
			else if (Package_Flag == 11){
				if (Serial_1_RxData == '\n')			//����յ��ڶ�����β
				{
					RxState = 0;			//״̬��0
					Serial_1T_RxPacket[TpRxPacket] = '\0';			//���յ����ַ����ݰ����һ���ַ���������־
					Serial_1_RxFlag = 1;		//�������ݰ���־λ��1���ɹ�����һ�����ݰ�
					Package_Flag = 3;
				}
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ
	}
}


//void USART1_IRQHandler(void)
//{
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		Serial_1_RxData = USART_ReceiveData(USART1);
//		Serial_1_RxFlag = 1;
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}



uint8_t Serial_2_GetRxFlag(void)
{              
	if (Serial_2_RxFlag == 1)
	{          
		Serial_2_RxFlag = 0;
		return 1;
	}          
	return 0;  
}              

uint8_t Serial_2_GetRxData(void)
{
	return Serial_2_RxData;
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		Serial_2_RxData = USART_ReceiveData(USART2);
		Serial_2_RxFlag = 1;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}




/************************************	ʾ��	************************************/
/*

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"
#include <stdlib.h>
#include <string.h>

uint8_t TargetS,Ki,Kp,Kd;
uint8_t TargetPitch,BKi,BKp,BKd;

int main(void)
{
	uint8_t  Init_Flag = 0;
	

	
	OLED_Init ();
	Serial_B_Init ();
	Serial_2_Init ();
		
	uint8_t Serial_B_RxData;
	
	char str[100];
	
	while(1)
	{
		uint8_t RX_Flag = Serial_1_GetRxFlag ();
		if(RX_Flag){
			//���յ�����HEX����
			if(RX_Flag == 1){
				Serial_B_RxData = Serial_1_GetRxData ();
				switch (Serial_B_RxData){
					case 0x00:
						break ;
					default :
						break ;
				}
				
			}
			//���յ�HEX���ݰ�
			else if(RX_Flag == 2){
				;
			}
			//���յ��ı����ݰ�	//���͵��ı����ݰ�Ӧ��"@ptarget:0,vp:0.8,vi:0.2,vd:0\r\n(���з�)"�ȸ�ʽ����//
			//���յ��ı�����֡�󣬸����ı����ݵĶ�����ж�Ӧ�Ĳ���
			//�������涨�ı���ʽΪ"@<������>:<��ֵ���>,<������>:<��ֵ���>\r\n(���з�)"Ҫ�����ı����ݽ���Ϊ������
					��ָ������²���
			//�����ַ�������������ض���ʶ��λ�ָ�����ɶ���Ӵ������Ӵ��ж�λ�ض���ʶ�����ڱ�ʶ��ǰ����в���
			else if(RX_Flag == 3){
				memset (str,'\0',sizeof(str));		//���ԭstr�ַ�����
				strcpy (str ,Serial_1T_RxPacket);	//�����ַ�����
				char* token = strtok (str,",");		//���ַ�����str�� ,(Ӣ�����뷨) Ϊ���и���ص�һ���Ӵ�ָ��
				while(token != NULL){
					char* colon = strchr (token, ':');	//��λtoken�Ӵ��е� : 
					if(colon != NULL){
						char* endptr;
						float value = strtof (colon+1,&endptr );	//���ַ���ת��Ϊ������
						
						 // ��֤ת����Ч�ԣ�endptr��ָ���ַ���ĩβ�����޲����ַ���
						if (endptr != colon+1 && *endptr == '\0'){
							if(strstr (token,"vtarget") != NULL )
								TargetS = value ;
							else if(strstr(token, "vp") != NULL )
								Kp = value ;
							else if(strstr(token, "vi") != NULL )
								Ki = value ;
							else if(strstr(token, "vd") != NULL )
								Kd = value ;
							else if(strstr (token,"ptarget") != NULL )
								TargetPitch = value ;
							else if(strstr(token, "bp") != NULL )
								BKp = value ;
							else if(strstr(token, "bi") != NULL )
								BKi = value ;
							else if(strstr(token, "bd") != NULL )
								BKd = value ;
						}
						token = strtok(NULL, ",");	// �����ָ�ʣ���ַ���������NULL��ʾ��������ԭ�ַ�����
					}
				}
			}
		}

		
		Serial_Printf(2,"%f\r\n", Ki);
		
//		Serial_Printf(2,"%f,%f,%f,%f,%f,%f\r\n", TargetLeft, ActualLeft, Out_Left,TargetRight, ActualRight, Out_Right);
//		Serial_Printf(2,"p:%f,i:%f,d:%f\r\n",Kp,Ki,Kd);
//		Serial_Printf(2,Serial_1T_RxPacket );
//		Serial_Printf(2,"\r\n");
	}
}

*/
/***********************************************************************************/















