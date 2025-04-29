#ifndef _NRF24L01_INS_H_
#define _NRF24L01_INS_H_

/**********	NRF24L01引脚定义	**********/
#define CSN_Port	GPIOB
#define	CE_Port		GPIOB
#define	MOSI_Port	GPIOB
#define	SCK_Port	GPIOB
#define	IRQ_Port	GPIOB
#define	MISO_Port	GPIOB

#define	CSN_Pin		GPIO_Pin_1
#define	CE_Pin		GPIO_Pin_0
#define	MOSI_Pin	GPIO_Pin_15
#define	SCK_Pin		GPIO_Pin_13
#define	IRQ_Pin		GPIO_Pin_12
#define	MISO_Pin	GPIO_Pin_14
/*************************************/

/**********	寄存器操作命令	**********/
#define NRF24_R_REG		0x00	//读配置寄存器，低5位为寄存器地址
#define NRF24_W_REG		0x20	//写配置寄存器，低5位为寄存器地址
#define R_RX_PAYLOAD	0x61	//读RX有效数据，1-32字节
#define W_TX_PAYLOAD	0xA0	//写TX有效数据，1-32字节
#define	FLUSH_TX		0xE1	//清空TX FIFO寄存器，发送模式下用
#define	FLUSH_RX		0xE2	//清空RX FIFO寄存器，接收模式下用
#define	REUSE_TX_PL		0xE3	//复用上次发送载荷，TX 载荷被复用直到 W_TX_PAYLOAD 或 FLUSH_TX 被执行，不能在传输期间改变复用状态
								//即重新使用上一包数据，CE置1，数据包不断发送
#define	NOP				0xFF	//空操作，用以读取STATUS(状态)寄存器
/*************************************/

/************	寄存器地址	***********/
#define	CONFIG		0x00	//配置寄存器地址
							//bit0,1、PRX,0、PTX；bit1,电选择；bit2,CRC模式，0、1字节，1、2字节；bit3，CRC使能
							//bit4,中断MAX_RT(达到最大重发次数中断)使能，bit5,中断TX_DS使能，bit6,中断RX_DR使能
#define	EN_AA		0x01	//使能自动应答功能
							//bit0-5,对应通道0-5，哪个bit位置1即选择哪个通道
#define	EN_RXADDR	0x02	//接收地址允许
							//bit0-5,对应通道0-5
#define	SETUP_AW	0x03	//设置地址宽度
							//bit1:0  00，非法；01，3字节；10，4字节；11，5字节
#define	SETUP_RETR	0x04	//建立自动重发
							//bit3:0,自动重发计数器0000-1111 1-15次自动重发；
							//bit7:自动重发延时，0000：250us,……1111：4000us
#define	RF_CH		0x05	//RF通道(射频通道)
							//bit6:0,工作频率通道
#define	RF_SETUP	0x06	//RF寄存器
							//bit3,传输速率(0,1Mbps 1,2Mbps)；bit2:1，发射功率；bit0:低噪声放大器增益
#define	STATUS		0x07	//状态寄存器
							//bit0,TXFIFO满标志(1为满)；
							//bit3:1,接收数据通道号(000-101为通道号，110表示未使用，111表示RXFIFO空)
							//bit4最大TX重传中断数，置1清除标志位，如果MAX_RT断言，须清除以允许继续通信
							//bit5,数据发送完成中断位，bit6，接收数据中断位	//置1清除标志位
#define	OBSERVE_TX	0x08	//发送检测寄存器
							//bit7:4，数据丢失计数器；bit3:0，重发计数器
#define	RPD			0x09	//载波检测寄存器
#define	RX_ADDR_P0	0x0A	//数据通道0接收地址	最大长度5字节，低字节在前
#define	RX_ADDR_P1	0x0B	//数据通道1接收地址	最大长度5字节，低字节在前
#define	RX_ADDR_P2	0x0C	//数据通道2接收地址	最低字节可设置，高字节必须同RX_ADDR_P1[39:8]相等
#define	RX_ADDR_P3	0x0D	//数据通道3接收地址	最低字节可设置，高字节必须同RX_ADDR_P1[39:8]相等
#define	RX_ADDR_P4	0x0E	//数据通道4接收地址	最低字节可设置，高字节必须同RX_ADDR_P1[39:8]相等
#define	RX_ADDR_P5	0x0F	//数据通道5接收地址	最低字节可设置，高字节必须同RX_ADDR_P1[39:8]相等
#define	TX_ADDR		0x10	//发送地址	发送模式下用
							//低字节在前，ShockBurst模式下RX_ADDR_P0与此地址相同
#define	RX_PW_P0	0x11	//接收数据通道0有效数据宽度	1-32字节，0为非法
#define	RX_PW_P1	0x12	//接收数据通道1有效数据宽度	1-32字节，0为非法
#define	RX_PW_P2	0x13	//接收数据通道2有效数据宽度	1-32字节，0为非法
#define	RX_PW_P3	0x14	//接收数据通道3有效数据宽度	1-32字节，0为非法
#define	RX_PW_P4	0x15	//接收数据通道4有效数据宽度	1-32字节，0为非法
#define	RX_PW_P5	0x16	//接收数据通道5有效数据宽度	1-32字节，0为非法
#define FIFO_STATUS	0x17	//FIFO状态寄存器
							//bit0,RX FIFO寄存器空标志(1为空)；bit1,RX FIFO寄存器满标志(1为满)
							//bit3:2,保留位，写0；bit7,保留位，写0
							//bit4,TX FIFO寄存器空标志(1位空)；bit5,TX FIFO寄存器满标志(1为满)
							//bit6,用于发送模式，置1循环发送上以数据包，置0不循环
/**************************************/

/******	STATUS寄存器bit位定义	******/
#define MAX_TX  	0x10  	  //达到最大发送次数中断
#define TX_OK   	0x20  	  //TX发送完成中断
#define RX_OK   	0x40  	  //接收到数据中断
/*************************************/

#endif
