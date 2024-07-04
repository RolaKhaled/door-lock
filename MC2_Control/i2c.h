#ifndef I2C_H_
#define I2C_H_

#include"std_types.h"

/*Definitions*/
#define TWI_START         0x08
#define TWI_REP_START     0x10
#define TWI_MT_SLA_W_ACK  0x18
#define TWI_MT_SLA_R_ACK  0x40
#define TWI_MT_DATA_ACK   0x28
#define TWI_MR_DATA_ACK   0x50
#define TWI_MR_DATA_NACK  0x58


/*Types Declaration*/
typedef unsigned short I2C_Address;

typedef unsigned long I2C_BaudRate;

typedef struct{
	I2C_Address address;
	I2C_BaudRate bit_rate;
}I2C_Config;

/*Function Prototypes*/
void I2C_init(const I2C_Config *i2c_config_ptr);
void I2C_start(void);
void I2C_stop(void);
void I2C_writeByte(uint8 data);
uint8 I2C_readByteWithACK(void);
uint8 I2C_readByteWithNACK(void);
uint8 I2C_getStatus(void);

#endif /* I2C_H_ */
