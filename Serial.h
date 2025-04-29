#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_1H_TxPacket[];
extern uint8_t Serial_1H_RxPacket[];

extern char Serial_1T_RxPacket[100];

extern uint8_t Serial_Select;

void Serial_B_Init(void);
void Serial_2_Init(void);
void Serial_SendByte(uint8_t Byte, uint8_t select);
void Serial_SendArray(uint8_t *Array, uint16_t Length, uint8_t select);
void Serial_SendString(char *String, uint8_t select);
void Serial_SendNumber(uint32_t Number, uint8_t Length, uint8_t select);
void Serial_Printf(uint8_t select, char *format, ...);

void Serial_1_SendPacket(void);

uint8_t Serial_1_GetRxFlag(void);
uint8_t Serial_1_GetRxData(void);

uint8_t Serial_2_GetRxFlag(void);
uint8_t Serial_2_GetRxData(void);

#endif
