#include<avr/io.h>
#include"timer1.h"
#include"common_macros.h"
#include<avr/interrupt.h>

static volatile void(*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_OVF_vect){
	if(g_callBackPtr != NULL_PTR){
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect){
	if(g_callBackPtr != NULL_PTR){
		(*g_callBackPtr)();
	}
}

void Timer1_init(const Timer1_Config *config_ptr){
	/*similar initialization steps in both CTC and normal modes*/
	SET_BIT(TCCR1A,FOC1A); /*non pwm mode*/
	SET_BIT(TCCR1A,FOC1B); /*non pwm mode*/
	TCCR1B = (TCCR1B&(0xF7)) | (config_ptr->prescaler);
	TCNT1 = config_ptr->initial_value;

	/*different initialization steps*/
	if(config_ptr->mode == NORMAL){
		SET_BIT(TIMSK,TOIE1);
	}
	if(config_ptr->mode == CTC){
		OCR1A =config_ptr->compare_value;
		SET_BIT(TIMSK,OCIE1A);
	}
}

void Timer1_deInit(void){
	TCNT1=0;
	TCCR1A=0;
	TCCR1B=0;
	TIMSK=0;
	OCR1A=0;
}

void Timer1_setCallBack(void(*a_ptr)(void)){
	g_callBackPtr = a_ptr;
}
