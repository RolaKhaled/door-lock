#include"gpio.h"
#include"lcd.h"
#include<util/delay.h>
#include"common_macros.h"

void LCD_init(void){
	GPIO_setupPinDirection(RS_PORT_ID, RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(E_PORT_ID, E_PIN_ID, PIN_OUTPUT);
	_delay_ms(20);
#if(LCD_DATA_BITS == 4)
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,PIN_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

#elif(LCD_DATA_BITS == 8)
	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);
#endif
	LCD_sendCommand(LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}

void LCD_sendCommand(uint8 Command)
{
	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);

#if(LCD_DATA_BITS == 4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(Command,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(Command,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(Command,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(Command,7));

	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);

	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(Command,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(Command,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(Command,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(Command,3));

	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_LOW);
	_delay_ms(1);

#elif(LCD_DATA_BITS == 8)
	GPIO_writePort(LCD_DATA_PORT_ID, Command);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
#endif
}

void LCD_displayCharacter(uint8 Data)
{
	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_DATA_BITS == 8)
	GPIO_writePort(LCD_DATA_PORT_ID, Data);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
#elif(LCD_DATA_BITS == 4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(Data,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(Data,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(Data,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(Data,7));

	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);

	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(Data,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(Data,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(Data,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(Data,3));

	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
#endif
}



void LCD_displayString(const char * Str){
	uint8 i=0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}
void LCD_moveCursor(uint8 row,uint8 col){
	uint8 lcd_memory_address;
	switch(row)
	{
	case 0:
		lcd_memory_address=col;
		break;
	case 1:
		lcd_memory_address=col+0x40;
		break;
	case 2:
		lcd_memory_address=col+0x10;
		break;
	case 3:
		lcd_memory_address=col+0x50;
		break;
	}
	LCD_sendCommand(lcd_memory_address | 0x80);
}
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str){
	LCD_moveCursor(row,col);
	LCD_displayString(Str);
}
void LCD_integerToString(int data){
	char buffer[16];
	itoa(data,buffer,10);
	LCD_displayString(buffer);
}
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_COMMAND);
}
