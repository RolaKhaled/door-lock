#ifndef EEPROM_H_
#define EEPROM_H_

#include"std_types.h"

/*Definitions*/
#define ERROR 0
#define SUCCESS 1

/*Types Declaration*/


/*Function Prototypes*/
uint8 EEPROM_writeByte(uint16 address,uint8 data);
uint8 EEPROM_readByte(uint16 address,uint8 *data);

#endif /* EEPROM_H_ */
