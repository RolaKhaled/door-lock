#include"dcmotor.h"
#include"gpio.h"
#include<avr/io.h>
#include"timer1.h"

void DCMotor_init(void){
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_PIN1_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_PIN2_ID,PIN_OUTPUT);
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN1_ID, 0);
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN2_ID, 0);/*stop motor at the beginning*/
}

void PWM_Timer0_Start(uint8 duty_cycle){
	TCNT0=0;
	OCR0= duty_cycle;
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

void DCMotor_rotate(uint8 speed,DCMotor_State state){
	PORTB = (PORTB & 0xFC);
	PORTB|=state;
	// PWM_Timer0_Start((MOTOR_MAX_SPEED*speed)/100);
	PWM_Timer0_Start(255);
}

