#ifndef UART2_H_
#define UART2_H_

#include"std_types.h"

/*Definitions*/


/*Types Declaration*/
typedef enum{
	DISABLE_PARITY,EVEN_PARITY=2,ODD_PARITY=3
}uart_paritymode;

typedef enum{
	STOP_1_BIT,STOP_2_BITS
}uart_stopbitselect;

typedef enum{
	DATABITS_5,DATABITS_6,DATABITS_7,DATABITS_8,DATABITS_9=7
}uart_charactersize;

typedef struct{
uint32 baudrate;
uart_paritymode paritybit;
uart_stopbitselect stopbit;
uart_charactersize databits;
}uart_config;

/*Function Prototypes*/
void UART_init(const uart_config *UART_Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_receiveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str);

#endif /* UART2_H_ */
