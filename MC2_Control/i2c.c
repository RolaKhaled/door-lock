#include"i2c.h"
#include<avr/io.h>
#include"common_macros.h"

void I2C_init(const I2C_Config *i2c_config_ptr){
	uint16 TWBR_value = (uint16)(((F_CPU/i2c_config_ptr->bit_rate)-16)/2);
	TWBR = TWBR_value;
	TWSR = 0x00;
	TWAR = i2c_config_ptr->address;
	SET_BIT(TWCR,2);
}
void I2C_start(void){
	TWCR = (1<<7) | (1<<5) | (1<<2);
	while(BIT_IS_CLEAR(TWCR,TWINT)){
		/*wait till start bit is sent*/
	}
}
void I2C_stop(void){
	TWCR = (1<<7) | (1<<4) | (1<<2);
}
void I2C_writeByte(uint8 data){
	TWDR = data;
	TWCR = (1<<7) | (1<<2);
	while(BIT_IS_CLEAR(TWCR,TWINT)){
		/*wait till data is sent successfully*/
	}
}
uint8 I2C_readByteWithACK(void){
	TWCR = (1<<7) | (1<<2) | (1<<6);
	while(BIT_IS_CLEAR(TWCR,TWINT)){
		/*wait till data is received successfully*/
	}
	return TWDR;
}
uint8 I2C_readByteWithNACK(void){
	TWCR = (1<<7) | (1<<2);
	while(BIT_IS_CLEAR(TWCR,TWINT)){
		/*wait till data is received successfully*/
	}
	return TWDR;
}
uint8 I2C_getStatus(void){
	uint8 status;
	status = TWSR & (0xF8);
	return status;
}
