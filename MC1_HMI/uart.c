#include"common_macros.h"
#include<avr/io.h>
#include "uart.h"

void UART_init(const uart_config *UART_Config_Ptr){
	uint16 UBRR_value =0;
	SET_BIT(UCSRA,U2X);
	/*disable interrupts and enable receiver and transmitter*/
	UCSRB |= ((1<<4) | ((1<<3) & (~(1<<7)) & (~(1<<6)) & (~(1<<5))));
	SET_BIT(UCSRC,URSEL);
	/*select asynchronous mode*/
	CLEAR_BIT(UCSRC,UMSEL);
	UCSRC = (UCSRC&(0xCF)) | ((UART_Config_Ptr->paritybit)<<4);
	UCSRC = (UCSRC&(0xF7)) | ((UART_Config_Ptr->stopbit)<<3);
	UCSRB = (UCSRB&(0xFB)) | (((UART_Config_Ptr->databits)<<2)& (0xFB));
	UCSRC = (UCSRC&(0xF9)) | (((UART_Config_Ptr->databits))&(0xF9));
	UBRR_value = (uint16)(((F_CPU / (UART_Config_Ptr->baudrate * 8UL))) - 1);
	UBRRH = UBRR_value>>8;
	UBRRL = UBRR_value;
}

void UART_sendByte(const uint8 data){
	while(BIT_IS_CLEAR(UCSRA,UDRE)){
		/*wait till data register empty flag is set*/
	}
	UDR=data;
}

uint8 UART_receiveByte(void){
	while(BIT_IS_CLEAR(UCSRA,RXC)){
		/*wait till receive complete flag is set*/
	}
	return UDR;
}

void UART_sendString(const uint8 *Str){
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str){
	uint8 i = 0;
	Str[i] = UART_receiveByte();

	/* create new null which is '#'. receive string till '#" */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}

	/* replace the fake null '#' with real null '\0' so the string is in its normal format*/
	Str[i] = '\0';
}

