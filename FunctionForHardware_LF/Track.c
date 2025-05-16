#include "stm32f10x.h"                  // Device header
#include "Track.h"
#include "AD.h"
#include <math.h>
#include "Delay.h"
//#include "OLED.h"

#define Init_LoopTimes		8000	//初始化测定白色对应值时的循环次数
#define Track_Filter_Times	20		//平均值滤波次数
#define Window_Width		50-1	//滑动平均值滤波次数（滑动窗口大小）

unsigned short Is_White[5] = {0};						//白色初始值
unsigned short Is_Red[5] = {0};							//红色初始值
unsigned short Is_Black[5] = {500,500,500,500,500};		//黑色初始值

unsigned short sensor_state[5] = {0};	//传感器反馈值

uint16_t white_min[5] = {2000,2000,2000,2000,2000};	//白色值下限

/**
  * @breuf  	循迹初始化
  * @param  	void
  * @retval 	void
  */
void Track_Init(void)
{
//	uint32_t white_sum[5] = {0};
//	uint32_t red_sum[5] = {0};
//	uint32_t black_sum[5] = {0};
	AD_Init();
	
	Delay_ms (500);
	
//	for(uint32_t i = 0; i < Init_LoopTimes; i++){
//		white_sum[0] += AD_Value [0];
//		white_sum[1] += AD_Value [1];
//		white_sum[2] += AD_Value [2];
//		white_sum[3] += AD_Value [3];
//		white_sum[4] += AD_Value [4];

////		red_sum[0] += AD_Value [0];
////		red_sum[1] += AD_Value [1];
////		red_sum[2] += AD_Value [2];
////		red_sum[3] += AD_Value [3];
////		red_sum[4] += AD_Value [4];
////		
////		black_sum[0] += AD_Value [0];
////		black_sum[1] += AD_Value [1];
////		black_sum[2] += AD_Value [2];
////		black_sum[3] += AD_Value [3];
////		black_sum[4] += AD_Value [4];		
//		
//	}
	
//	Is_White [0] = white_sum [0] / Init_LoopTimes;
//	Is_White [1] = white_sum [1] / Init_LoopTimes;
//	Is_White [2] = white_sum [2] / Init_LoopTimes;
//	Is_White [3] = white_sum [3] / Init_LoopTimes;
//	Is_White [4] = white_sum [4] / Init_LoopTimes;

	

	for (uint32_t i = 0; i < Init_LoopTimes; i++){
		for(uint8_t j = 0; j < 5; j++){
			if(white_min[j] > AD_Value [j] && AD_Value [j] != 0){
				white_min[j] = AD_Value[j];
			}				
		}
	}
	
	Is_White [0] = white_min [0];
	Is_White [1] = white_min [1];
	Is_White [2] = white_min [2];
	Is_White [3] = white_min [3];
	Is_White [4] = white_min [4];
	
	for(uint8_t i = 0; i < 5; i++){
		Is_Red [i] = Is_White [i] - White_to_Red  ;
	}
}

/**
  * @breuf  	AD读取值滤波
  * @param  	AD通道
  * @retval 	滤波后的值
  */
uint16_t Track_Filter(uint8_t AD_channel)
{
	uint16_t AD_filter;
	
	uint32_t temp_sum = 0;
	
	static uint16_t slide_window[Window_Width];
	
//	for(uint8_t j = 0; j < Track_Filter_Times; j++){
//		temp_sum += AD_Value [AD_channel];
//	}
//	
//	AD_filter = temp_sum / Track_Filter_Times ;
	
	static uint8_t flag = 0;
	static uint8_t location = 0;
	if(flag == 0){
		flag = 1;
		for(uint8_t i=0; i<Window_Width; i++){
			slide_window [i] = AD_Value [AD_channel];
			temp_sum += AD_Value [AD_channel];
		}
		AD_filter = temp_sum / Window_Width ;
	}else {
		if(location >= Window_Width ){
			slide_window [location] = AD_Value [AD_channel];
			location = 0;
		}else{
			slide_window [location] = AD_Value [AD_channel];
			location ++;
		}
		for(uint8_t j=0; j<Window_Width; j++){
			temp_sum += slide_window [j];
		}
		AD_filter = temp_sum / Window_Width ;
	}
	
	return AD_filter ;
}

/**
  * @breuf  	单个传感器反馈的状态
  * @param  	
  * @retval 	
  */
void Track_State(void)
{
	uint16_t temp = 0;
	
	for(uint8_t i = 0; i < 5; i++){		
//		temp = AD_Value [i];		
		temp = Track_Filter(i);
		if(temp > Is_White[i] - Bios ){
			sensor_state[i] = 0;
		}else if(temp <= Is_Red[i] + Bios && AD_Value [i] > Is_Black_Max ){
			sensor_state [i] = 1;
		}else if(temp <= Is_Black_Max ){
			sensor_state [i] = 2;
		}else{
			sensor_state[i] = 3;
		}
	}
}

/**
  * @breuf  	巡线反馈
  * @param  	void
  * @retval 	巡线状态，具体返回值对应情况见Track.h中的宏定义
  */
uint8_t Track_Line(void)
{
	uint8_t Line_state = 0;
	uint16_t sensor_Line = 0;
	for(uint8_t i = 0; i < 5; i++){
		sensor_Line += sensor_state [4-i] * pow (10, i);
	}
	switch (sensor_Line){
		case 00000:
			Line_state = Line_White ;
			break;
		case 100:	//00100
		case 1110:	
			Line_state = Line_M ;
			break;
		case 1000:
		case 1100:	
			Line_state = Line_L ;
			break ;
		case 10:
		case 110:	
			Line_state = Line_R ;
			break ;
		case 10000:
		case 11000:
			Line_state = Line_LL ;
			break ;
		case 1:
		case 11:	
			Line_state = Line_RR ;
			break ;
		case 11111:
			Line_state = Line_Red ;	//
			break ;
		case 11011:
			Line_state = Line_T ;
			break ;
		
		case 11100:
			break ;
		
		
		case 00111:
			break ;
		case 11110:
		case 1111:
		case 10111:
		case 11101:
			Line_state = Line_Red;
			break ;
		case 22022:
		case 22222:
		case 2222:
		case 22220:
//		case 20222:
			Line_state = Line_Black ;
			break ;
		default :
			Line_state = Line_Error;
			break ;
	}
	
//	OLED_ShowNum (48,8,sensor_Line ,5,OLED_6X8 );
	
	return Line_state ;
}









