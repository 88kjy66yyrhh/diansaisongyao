#include "Motor.h"

/*******************************************************************************
* Function Name  : Motor_Init
* Description    : 电机端口初始化 PB8 PB9 PC14 PC15
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	PWM_Init();
}


/*******************************************************************************
* Function Name  : Set_Pwm
* Description    : 将计算出的PWM数值赋予电机
* Input          : moto1为右轮，moto2为左轮
						CN2:R PWMA PA8 TIM1_CH1
						CN3:L PWMB PA11 TIM1_CH4
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Pwm(int motoA,int motoB)
{
	if(motoB<0)	
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8);	
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	}	
	else 	      
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_9);	
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	}  
		PWM_SetComparePWMB(motoB);	
	if(motoA<0)		
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_14);	
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	}	
	else 	      
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_15);	
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	}
	PWM_SetComparePWMA(motoA);
}

/*******************************************************************************
* Function Name  : myabs
* Description    : 取绝对值
* Input          : a: int型数据
* Output         : None
* Return         : int型数据的绝对值
*******************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}




	
