/*
******************************************************************************
* @file           : mcp4728.h
* @brief          : Header for mcp4728.c file.
*                   This file contains the defines for the mcp4728 driver
******************************************************************************
*/
#ifndef __MCP4728_H
#define __MCP4728_H

#include "stm32f4xx_hal.h"

typedef struct
{
	/* 4 bits, 0 = VDD, 1 = interal 2.048V, 0000ABCD*/
	uint8_t		channelVref;
	/* 4 bits, 0 = unity gain, 1 = x2 gain, 0000ABCD*/
	uint8_t		channel_Gain;
	/* 4 12bit numbers specifying the desired initial output values */
	uint16_t	channel_Val[4];
}dacChannelConfig;

/*
*	Function Prototypes
*/
void MCP4728_Write_GeneralCall(I2C_HandleTypeDef *I2CHandler, uint8_t command);
void MCP4728_Write_AllChannels_Same(I2C_HandleTypeDef *I2CHandler, uint16_t output);
void MCP4728_Write_AllChannels_Diff(I2C_HandleTypeDef *I2CHandler, dacChannelConfig output);
void MCP4728_Write_SingleChannel(I2C_HandleTypeDef *I2CHandler, uint8_t channel, uint16_t output, dacChannelConfig channels);
void MCP4728_Init(I2C_HandleTypeDef *I2CHandler, dacChannelConfig output);

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

#define MCP4728_CMD_FASTWRITE		0x00
#define MCP4728_CMD_DACWRITE_MULTI	0x40
#define MCP4728_CMD_DACWRITE_SEQ	0x50
#define MCP4728_CMD_DACWRITE_SINGLE	0x58
#define MCP4728_CMD_ADDRWRITE		0x60
#define	MCP4728_CMD_VREFWRITE		0x80
#define MCP4728_CMD_GAINWRITE		0xC0
#define MCP4728_CMD_PWRDWNWRITE		0xA0

#define MCP4728_BASEADDR			0x60<<1

#define MCP4728_VREF_VDD				0x0
#define MCP4728_VREF_INTERNAL		0x1

#define MCP4728_GAIN_1				0x0
#define MCP4728_GAIN_2				0x1

#define MCP4728_CHANNEL_A			0x0
#define MCP4728_CHANNEL_B			0x1
#define MCP4728_CHANNEL_C			0x2
#define MCP4728_CHANNEL_D			0x3

#define MCP4728_PWRDWN_NORMAL		0x0
#define MCP4728_PWRDWN_1			0x1
#define MCP4728_PWRDWN_2			0x2
#define MCP4728_PWRDWN_3			0x3

#define MCP4728_UDAC_UPLOAD			0x1
#define MCP4728_UDAC_NOLOAD			0x0

#define MCP4728_GENERAL_RESET		0x06
#define MCP4728_GENERAL_WAKEUP		0x09
#define MCP4728_GENERAL_SWUPDATE	0x08
#define MCP4728_GENERAL_READADDR	0x0C

#endif
