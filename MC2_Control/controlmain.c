#include<avr/io.h>
#include"eeprom.h"
#include"uart.h"
#include"i2c.h"
#include<util/delay.h>
#include "communication_codes.h"
#include"timer1.h"
#include"dcmotor.h"
#include"buzzer.h"
#include"lcd.h"

uint8 g_ticks=0;
uint8 g_sendback=0;
uint8 g_password[PASSWORD_SIZE];
uint8 g_passwordConfirm[PASSWORD_SIZE];
uint8 g_passwordConfirmationStatus;
uint8 enter=13;
uint8 g_optionPicked;
uint8 g_wrongPasswordCounter=0;
uint32 address=0x0311;
uint8 test[PASSWORD_SIZE];
uart_config uart_config_ptr={9600,DISABLE_PARITY,STOP_1_BIT,DATABITS_8};

void APP_receivePassword(void);
void APP_savePasswordInEEPROM(void);
uint8 APP_confirmPassword2(void);
void APP_setTimer1CallBackFunction(void);
void APP_Buzzer(void);

int main(void){

	// /SREG|=(1<<7) ;

	Timer1_Config timer1_config_ptr={0,0,FCPU_256,NORMAL};
	SREG|=(1<<7); // THIS WAASS TRANSALATED FROM THE FIRST MAIN LINE
	I2C_Config i2c_config_ptr={1,400000}; /*slave address of MC2 is 1*/

	UART_init(&uart_config_ptr);
	Timer1_init(&timer1_config_ptr);
	Timer1_setCallBack(APP_setTimer1CallBackFunction);
	I2C_init(&i2c_config_ptr);
	Buzzer_init();
	DCMotor_init();
	LCD_init();

	while(1){
		while(g_sendback==0){
			APP_receivePassword();
			APP_savePasswordInEEPROM();
		}
		while(g_sendback==1){
			APP_receivePassword();
			g_passwordConfirmationStatus=APP_confirmPassword2();
		}
		UART_init(&uart_config_ptr);

		if(g_passwordConfirmationStatus!=0){
			DCMotor_rotate(100,CW);
			_delay_ms(10);
			UART_sendByte(PASSWORDS_DONOT_MATCH);
			_delay_ms(100);
			g_sendback=0;
		}
		else{
			_delay_ms(100);
			UART_sendByte(PASSWORDS_MATCH);
			_delay_ms(100);

			while(1){
				// UART_init(&uart_config_ptr);
				g_optionPicked = UART_receiveByte();
			//	_delay_ms(100);
			//	g_optionPicked = '+' ;
				if(g_optionPicked=='+'){
					while(1){
						// UART_init(&uart_config_ptr);
						APP_receivePassword();
						//g_passwordConfirmationStatus=APP_confirmPassword();

						if(APP_confirmPassword2()==0){
							UART_sendByte(CORRECT_PASSWORD);
							_delay_ms(100);

							g_ticks=0;
							DCMotor_rotate(100,CW);
							while(g_ticks<8);

							g_ticks=0;
							DCMotor_rotate(0,OFF);
							while(g_ticks<2);

							g_ticks=0;
							DCMotor_rotate(100,ACW);
							while(g_ticks<8);

							DCMotor_rotate(0,OFF);

							g_sendback=0;
							break;
						}

						else{
							g_wrongPasswordCounter++;
							UART_sendByte(WRONG_PASSWORD);
                            _delay_ms(100) ;
							if(g_wrongPasswordCounter==3){
								APP_Buzzer();
								g_sendback=0;
								break;
							}
						}
					}
				}
				else if(g_optionPicked=='-'){
					while(1){
						UART_init(&uart_config_ptr);
						APP_receivePassword();
						g_passwordConfirmationStatus=APP_confirmPassword2();

						if(g_passwordConfirmationStatus==0){
							UART_sendByte(CORRECT_PASSWORD);
							_delay_ms(10);
							g_sendback=0;
							break;
						}
						else{
							g_wrongPasswordCounter++;
							UART_sendByte(WRONG_PASSWORD);

							if(g_wrongPasswordCounter==3){
								APP_Buzzer();
								g_sendback=0;
								break;
							}
						}
						break;
					}
				}
			}
		}
	}
}

void APP_receivePassword(void){
	uint8 i;
	for(i=0;i<PASSWORD_SIZE;i++){
		g_password[i]=UART_receiveByte();
		_delay_ms(10);
	}
	g_sendback++;
}

void APP_savePasswordInEEPROM(void){
	uint8 i;
	address=1;
	for(i=0;i<PASSWORD_SIZE;i++){
		g_passwordConfirm[i]=g_password[i];
	}
}
/*void APP_savePasswordInEEPROM(void){
	uint8 i;
	address=0x0311;

	//for(i=0;i<PASSWORD_SIZE;i++){
	EEPROM_writeByte(address,g_password[0]);
	address++;
	_delay_ms(10);

	EEPROM_writeByte(address,g_password[1]);
	address++;
	_delay_ms(10);

	EEPROM_writeByte(address,g_password[2]);
	_delay_ms(10);

}
*/
uint8 APP_confirmPassword2(void){
	uint8 statuss=0;
	uint8 i;
	address=0x0311;


	//EEPROM_readByte(address,&g_passwordConfirm[0]);

	if(g_password[0] != g_passwordConfirm[0]){
		statuss++;
	}
	address++;
	//_delay_ms(10);

	//EEPROM_readByte(address,&g_passwordConfirm[1]);

	if(g_password[1] != g_passwordConfirm[1]){
		statuss++;
	}
	address++;
	//_delay_ms(10);

	//EEPROM_readByte(address,&g_passwordConfirm[2]);

	if(g_password[2] != g_passwordConfirm[2]){
		statuss++;
	}
	address++;

	//_delay_ms(10);

	//LCD_clearScreen();
	//LCD_integerToString(statuss);
	return statuss;

}

void APP_Buzzer(void){
	g_ticks=0;
	Buzzer_on();
	while(g_ticks<30);
	Buzzer_off();
}

void APP_setTimer1CallBackFunction(void){
	g_ticks++;
}
