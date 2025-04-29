#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

uint8_t Serial_1_RxData;
uint8_t Serial_1_RxFlag;

uint8_t Serial_2_RxData;
uint8_t Serial_2_RxFlag;

uint8_t Serial_Select = 1;			//串口选择

uint8_t Package_Flag = 1;			//数据格式，1：单个HEX数据，2：HEX数据包，3：文本数据包

//串口转蓝牙通信的数据包
uint8_t Serial_1H_TxPacket[4];				//定义发送HEX数据包数组，数据包格式：FF 01 02 03 04 FE
uint8_t Serial_1H_RxPacket[4];				//定义接收HEX数据包数组

char Serial_1T_RxPacket[100];				//定义接收文本数据包数组，数据包格式"@MSG\r\n"

//串口转蓝牙所用的串口，USART1
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);	//USART2挂载在APB1总线上
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
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//开启串口，接受中断
	
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
  * @param  	Byte:要发送的字节; select：选择要发送数据的串口，1、2
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
  * @breuf  	判断数据标志位
  * @param  	void 
  * @retval 	0：未接收到有效数据，1：接收到单个HEX数据
  *				2：接收到HEX数据帧	3：接收到为文本数据帧
  */
uint8_t Serial_1_GetRxFlag(void)
{
	if (Serial_1_RxFlag == 1)
	{
		Serial_1_RxFlag = 0;
		if(Package_Flag == 1)			//接收到的为单个数据
			return 1;
		else if(Package_Flag ==2)		//接收到的为HEX数据包					
			return 2;
		else if(Package_Flag == 3)		//接收到的为文本数据包
			return 3;
	}
	return 0;
}

/**
  * @breuf  	读取接收的数据
  *				接收到的数据类型为单个HEX数据则调用该函数读取数据，
  *				若为数据包，则直接操作对应的数据缓存数组
  * @param  	void
  * @retval 	单个HEX数据
  */
uint8_t Serial_1_GetRxData(void)
{
//	return Serial_1_RxData;
	uint8_t temp = Serial_1_RxData;
	Serial_1_RxData = 0x00;
	return temp;
}


/**
  * 函    数：串口发送HEX数据包
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，Serial_TxPacket数组的内容将加上包头（FF）包尾（FE）后，作为数据包发送出去
  */
void Serial_1_SendPacket(void)
{
	Serial_SendByte(0xFF,1);
	Serial_SendArray(Serial_1H_TxPacket, 4,1);
	Serial_SendByte(0xFE,1);
}


/**
  * @breuf  	USART1中断函数
  *				该中断函数将判断接收到的数据类型，有单个HEX数据、HEX数据帧(有效数据为4个，可设置)、文本数据帧
  * @param  	void
  * @retval 	void
  */
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t HpRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	static uint8_t TpRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{
		Serial_1_RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			if (Serial_1_RxData == 0xFF)			//如果数据确实是HEX数据包头
			{
				RxState = 1;			//置下一个状态
				HpRxPacket = 0;			//数据包的位置归零
				Package_Flag = 10;		//表示正在接收HEX数据包
			}
			else if (Serial_1_RxData == '@' && Serial_1_RxFlag == 0)		//如果数据确实是文本数据包头，并且上一个数据包已处理完毕
			{
				RxState = 1;			//置下一个状态
				memset (Serial_1T_RxPacket ,'\0',sizeof (Serial_1T_RxPacket ));	//将上一次接收的数据包清空
				TpRxPacket = 0;			//数据包的位置归零
				Package_Flag = 11;		//表示正在接收文本数据包
			}
			else{						//数据不是包头，则为单个发送的数据
					Serial_1_RxData = USART_ReceiveData(USART1);
					Serial_1_RxFlag = 1;
					Package_Flag = 1;
					RxState = 0;
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState == 1)
		{
			if(Package_Flag == 10){
				Serial_1H_RxPacket[HpRxPacket] = Serial_1_RxData;	//将数据存入数据包数组的指定位置
				HpRxPacket ++;				//数据包的位置自增
				if (HpRxPacket >= 4)			//如果收够4个数据
				{
					RxState = 2;			//置下一个状态
				}
			}
			else if(Package_Flag == 11){
				if (Serial_1_RxData == '\r')			//如果收到第一个包尾
				{
					RxState = 2;			//置下一个状态
				}
				else						//接收到了正常的数据
				{
					Serial_1T_RxPacket[TpRxPacket] = Serial_1_RxData;		//将数据存入数据包数组的指定位置
					TpRxPacket ++;			//数据包的位置自增
				}
			}
		}
		/*当前状态为2，接收数据包包尾*/
		else if (RxState == 2)
		{
			if(Package_Flag == 10){
				if (Serial_1_RxData == 0xFE)			//如果数据确实是包尾部
				{
					RxState = 0;			//状态归0
					Serial_1_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
					Package_Flag = 2;
				}
			}
			else if (Package_Flag == 11){
				if (Serial_1_RxData == '\n')			//如果收到第二个包尾
				{
					RxState = 0;			//状态归0
					Serial_1T_RxPacket[TpRxPacket] = '\0';			//将收到的字符数据包添加一个字符串结束标志
					Serial_1_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
					Package_Flag = 3;
				}
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位
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




/************************************	示例	************************************/
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
			//接收到单个HEX数据
			if(RX_Flag == 1){
				Serial_B_RxData = Serial_1_GetRxData ();
				switch (Serial_B_RxData){
					case 0x00:
						break ;
					default :
						break ;
				}
				
			}
			//接收到HEX数据包
			else if(RX_Flag == 2){
				;
			}
			//接收到文本数据包	//发送的文本数据包应以"@ptarget:0,vp:0.8,vi:0.2,vd:0\r\n(换行符)"等格式发送//
			//接收到文本数据帧后，根据文本数据的定义进行对应的操作
			//例：若规定文本格式为"@<变量名>:<赋值结果>,<变量名>:<赋值结果>\r\n(换行符)"要将存文本数据解析为所定义
					的指令，则按如下操作
			//包括字符数组操作，按特定标识定位分割数组成多个子串，在子串中定位特定标识符，在标识符前后进行操作
			else if(RX_Flag == 3){
				memset (str,'\0',sizeof(str));		//清空原str字符数组
				strcpy (str ,Serial_1T_RxPacket);	//复制字符数组
				char* token = strtok (str,",");		//将字符数组str按 ,(英文输入法) 为界切割，返回第一个子串指针
				while(token != NULL){
					char* colon = strchr (token, ':');	//定位token子串中的 : 
					if(colon != NULL){
						char* endptr;
						float value = strtof (colon+1,&endptr );	//将字符串转换为浮点数
						
						 // 验证转换有效性：endptr需指向字符串末尾（即无残留字符）
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
						token = strtok(NULL, ",");	// 继续分割剩余字符串（传入NULL表示继续处理原字符串）
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















