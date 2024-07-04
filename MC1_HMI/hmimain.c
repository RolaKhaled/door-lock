#include"lcd.h"
#include"keypad.h"
#include<avr/io.h>
#include<util/delay.h>
#include"timer1.h"
#include "communication_codes.h"
#include "uart.h"

void APP_readPassword(void);
void APP_sendPassword(void);
void APP_pickOptions(void);
void APP_setTimer1CallBackFunction(void);
void APP_openDoor(void);
void APP_changePassword(void);
void APP_errorMessage(void);

uint8 g_status;
uint8 g_sendback=0;
uint8 g_password[PASSWORD_SIZE];
const uint8 enter=13;
uint8 g_optionPicked;
uint8 g_passwordStatus;
uint8 g_wrongPasswordCounter=0;
uint8 g_ticks=0;
uart_config uart_config_ptr={9600,DISABLE_PARITY,STOP_1_BIT,DATABITS_8};
int main (void){
	SREG|=(1<<7);

	/*configurations*/

	Timer1_Config timer1_config_ptr={0,0,FCPU_256,NORMAL};

	LCD_init();
	UART_init(&uart_config_ptr);
	Timer1_init(&timer1_config_ptr);
	/*every time an interrupt occurs(timer overflows) this function is called to increase the ticks by one*/
	Timer1_setCallBack(APP_setTimer1CallBackFunction);


	while(1){
		if(g_sendback==0){
			_delay_ms(10);
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"plz enter pass:");
			LCD_moveCursor(1,0);
			APP_readPassword();
			APP_sendPassword();
		}
		if(g_sendback==1){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"confirm pass");
			LCD_moveCursor(1,0);
			APP_readPassword();
			APP_sendPassword();
		}
		UART_init(&uart_config_ptr);
		do{
			g_status = UART_receiveByte();
			_delay_ms(10);
			LCD_clearScreen();
		}while((g_status!=PASSWORDS_MATCH) && (g_status!=PASSWORDS_DONOT_MATCH));
		LCD_clearScreen();
		/*g_status=UART_receiveByte();
		_delay_ms(10);
		LCD_clearScreen();*/

		if(g_status==PASSWORDS_MATCH){
			LCD_clearScreen();
			APP_pickOptions();
			g_sendback=0;
		}
		else if(g_status ==PASSWORDS_DONOT_MATCH){
			g_sendback=0;
		}
	}
}

void APP_readPassword(void){
	uint8 i;
	LCD_moveCursor(1,0);
	for(i=0;i<PASSWORD_SIZE;i++){
		g_password[i]=KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	//while(KEYPAD_getPressedKey() != enter);
}

void APP_sendPassword(void){
	uint8 i;
	for(i=0;i<PASSWORD_SIZE;i++){
		UART_sendByte(g_password[i]);
		//_delay_ms(10);
	}
	g_sendback++;
}

void APP_pickOptions(void){
	while(1){
		UART_init(&uart_config_ptr);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"+:open door");
		LCD_displayStringRowColumn(1,0,"-:change password");
		g_optionPicked=KEYPAD_getPressedKey();
		_delay_ms(500);
		UART_sendByte(g_optionPicked);
		//_delay_ms(10);

		if(g_optionPicked == '-'){
			APP_changePassword();
			break;
		}
		else{
			APP_openDoor();
		}
	}
}

void APP_openDoor(void){
	while(1){
		_delay_ms(100) ;
		// UART_init(&uart_config_ptr);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"enter password:");
		APP_readPassword();
		APP_sendPassword();
		// APP_errorMessage();
	//	_delay_ms(1000) ;

		g_passwordStatus= UART_receiveByte(); // the problem is in uart communication
		//g_passwordStatus = '&' ;
          _delay_ms(100) ;
      //	APP_errorMessage();
		if(g_passwordStatus==WRONG_PASSWORD){
			g_wrongPasswordCounter++;
		//	APP_errorMessage();
			if(g_wrongPasswordCounter==3){
				APP_errorMessage();
				break;
			}
		}
		else if(g_passwordStatus==CORRECT_PASSWORD){
			LCD_clearScreen();

			g_ticks=0;
			LCD_displayStringRowColumn(0,0,"Door is opening");
			while(g_ticks<8);

			LCD_clearScreen();

			g_ticks=0;
			LCD_displayStringRowColumn(0,0,"Door is open");
			while(g_ticks<2);

			g_ticks=0;
			LCD_displayStringRowColumn(0,0,"Door is locking");
			while(g_ticks<8);

			break;
		}
	}
}

void APP_changePassword(void){
	while(1){
		UART_init(&uart_config_ptr);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"enter password:");
		APP_readPassword();
		APP_sendPassword();
		g_passwordStatus=UART_receiveByte();
		     _delay_ms(10);

		if(g_passwordStatus==WRONG_PASSWORD){
			g_wrongPasswordCounter++;
			if(g_wrongPasswordCounter==3){
				APP_errorMessage();
				break;
			}
		}
		else if(g_passwordStatus==CORRECT_PASSWORD){
			break;
		}
	}
}

void APP_errorMessage(void){
	g_ticks=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"tryAgain in 1min");
	while(g_ticks<30);
}

void APP_setTimer1CallBackFunction(void){
	g_ticks++;
}
