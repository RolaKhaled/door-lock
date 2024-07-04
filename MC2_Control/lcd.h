
#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"

#define LCD_DATA_BITS 8

#define RS_PORT_ID PORTB_ID
#define RS_PIN_ID PIN0_ID
#define E_PORT_ID PORTB_ID
#define E_PIN_ID PIN1_ID
#define LCD_DATA_PORT_ID PORTA_ID

#if (LCD_DATA_BITS == 4)

#define LCD_DB4_PIN_ID                 PIN3_ID
#define LCD_DB5_PIN_ID                 PIN4_ID
#define LCD_DB6_PIN_ID                 PIN5_ID
#define LCD_DB7_PIN_ID                 PIN6_ID

#endif

#define LCD_CLEAR_COMMAND                    0x01
#define LCD_GO_TO_HOME                       0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE        0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE         0x28
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32
#define LCD_CURSOR_OFF                       0x0C
#define LCD_CURSOR_ON                        0x0E
#define LCD_SET_CURSOR_LOCATION              0x80

void LCD_sendCommand(uint8 Command);
void LCD_displayCharacter(uint8 Data);
void LCD_init(void);
void LCD_displayString(const char * Str);
void LCD_moveCursor(uint8 row,uint8 col);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_integerToString(int data);
void LCD_clearScreen(void);

#endif /* LCD_H_ */
