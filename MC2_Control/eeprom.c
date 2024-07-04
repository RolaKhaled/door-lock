#include"eeprom.h"
#include"i2c.h"

uint8 EEPROM_writeByte(uint16 address,uint8 data){
	I2C_start();
	if (I2C_getStatus() != TWI_START)
		return ERROR;

	I2C_writeByte((uint8)(0xA0 | ((address & 0x0700)>>7)));
	if (I2C_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	I2C_writeByte((uint8)(address));
	if (I2C_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;


	I2C_writeByte(data);
	if (I2C_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	I2C_stop();

	return SUCCESS;
}
uint8 EEPROM_readByte(uint16 address,uint8 *data){
	I2C_start();
	if (I2C_getStatus() != TWI_START)
		return ERROR;

	I2C_writeByte((uint8)((0xA0) | ((address & 0x0700)>>7)));
	if (I2C_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	I2C_writeByte((uint8)(address));
	if (I2C_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	I2C_start();
	if (I2C_getStatus() != TWI_REP_START)
		return ERROR;

	I2C_writeByte((uint8)((0xA0) | ((address & 0x0700)>>7) | 1));
	if (I2C_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	*data = I2C_readByteWithNACK();
	if (I2C_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;
	I2C_stop();

	return SUCCESS;
}
