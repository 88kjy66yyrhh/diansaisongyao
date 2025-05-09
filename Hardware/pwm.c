#include "pwm.h"

/*******************************************************************************
* Function Name  : PWM_Init
* Description    : PWM初始化 
					PB6 TIM4 CH1 PWMB
					PB7 TIM4 CH2 PWMA
					最大值7200
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimebaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimebaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimebaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimebaseInitStructure.TIM_Period=7199;
	TIM_TimebaseInitStructure.TIM_Prescaler=0;
	TIM_TimebaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimebaseInitStructure);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
		
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE);
		
}


void PWM_SetComparePWMB(int Compare)
{
	if(Compare > 0)
	{
		TIM_SetCompare1(TIM4, Compare);
	}
	else
	{
		TIM_SetCompare1(TIM4, -Compare);
	}
}
void PWM_SetComparePWMA(int Compare)
{
	if(Compare > 0)
	{
		TIM_SetCompare2(TIM4, Compare);
	}
	else
	{
		TIM_SetCompare2(TIM4, -Compare);
	}
}
