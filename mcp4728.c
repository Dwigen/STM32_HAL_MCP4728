#include "mcp4728.h"

HAL_StatusTypeDef MCP4728_Write_VRef_Select(I2C_HandleTypeDef *I2CHandler, dacChannelConfig config){
	uint8_t data = config.channelVref | (MCP4728_CMD_VREFWRITE);
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Master_Transmit(I2CHandler, MCP4728_BASEADDR, data, sizeof(data), HAL_MAX_DELAY);
	return ret;
}
HAL_StatusTypeDef MCP4728_Write_PWRDWN_Select(I2C_HandleTypeDef *I2CHandler, uint8_t command){
	uint8_t data[2];
	data[0] = MCP4728_CMD_PWRDWNWRITE | command<<2 | command;
	data[1] = (command << 6 |command << 4) & 0xF0 ;
	HAL_StatusTypeDef ret;
	ret = HAL_I2C_Master_Transmit(I2CHandler, MCP4728_BASEADDR, data, sizeof(data), HAL_MAX_DELAY);
	return ret;
}
void MCP4728_Write_GeneralCall(I2C_HandleTypeDef *I2CHandler, uint8_t command)
{
	HAL_I2C_Master_Transmit(I2CHandler, 0x00, command, 1, HAL_MAX_DELAY);
}

void MCP4728_Write_AllChannels_Same(I2C_HandleTypeDef *I2CHandler, uint16_t output)
{
	uint8_t buf[8];
	uint8_t lowByte = output & 0xff;
	uint8_t highByte = (output >> 8);
	for(int i = 0; i<8; i = i+2){
		buf[i] = 0x0f&highByte;
		buf[i+1] = lowByte;
	}
	HAL_I2C_Master_Transmit(I2CHandler, MCP4728_BASEADDR, buf, sizeof(buf), HAL_MAX_DELAY);
	MCP4728_Write_GeneralCall(I2CHandler,MCP4728_GENERAL_SWUPDATE);
}

void MCP4728_Write_AllChannels_Diff(I2C_HandleTypeDef *I2CHandler, dacChannelConfig output)
{
	uint8_t buf[8];
	for(uint8_t i = 0; i < 4; i++){
		uint8_t lowByte = output.channel_Val[i] & 0xff;
		uint8_t highByte = (output.channel_Val[i] >> 8);
		buf[i*2] =  0x0f&highByte;
		buf[(i*2)+1] = lowByte;
	}
	HAL_I2C_Master_Transmit(I2CHandler, MCP4728_BASEADDR, buf, sizeof(buf), HAL_MAX_DELAY);
	MCP4728_Write_GeneralCall(I2CHandler,MCP4728_GENERAL_SWUPDATE);
}

void MCP4728_Write_SingleChannel(I2C_HandleTypeDef *I2CHandler, uint8_t channel, uint16_t output, dacChannelConfig channels)
{
	uint8_t buf[3];
	uint8_t lowByte = output & 0xff;
	uint8_t highByte = (output >> 8);
	buf[0] = MCP4728_CMD_DACWRITE_SINGLE | (channel<<1);
	buf[1] = (0<<7) | (channel<<5) | highByte;
	buf[2] = lowByte;
	HAL_I2C_Master_Transmit(I2CHandler, MCP4728_BASEADDR, buf, sizeof(buf), HAL_MAX_DELAY);
	MCP4728_Write_GeneralCall(I2CHandler,MCP4728_GENERAL_SWUPDATE);
}

void MCP4728_Init(I2C_HandleTypeDef *I2CHandler, dacChannelConfig output){
	MCP4728_Write_GeneralCall(I2CHandler,MCP4728_GENERAL_RESET);
	MCP4728_Write_GeneralCall(I2CHandler,MCP4728_GENERAL_WAKEUP);
	uint8_t buf[9];
	buf[0] = MCP4728_CMD_DACWRITE_SEQ;
	for(uint8_t i = 1; i <= 4; i++){
		buf[(i*2)+1] = 0x00;
		buf[(i*2)] = (0 << 7) | ((i-1)<<4) | 0x0;
	}
	HAL_I2C_Master_Transmit(I2CHandler, MCP4728_BASEADDR, buf, sizeof(buf), HAL_MAX_DELAY);
	MCP4728_Write_GeneralCall(I2CHandler,MCP4728_GENERAL_SWUPDATE);
}
