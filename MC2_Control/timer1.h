#ifndef TIMER1_H_
#define TIMER1_H_

#include"std_types.h"

/*Definitions*/

/*Types Declaration*/
typedef enum{
NO_CLOCK,FCPU,FCPU_8,FCPU_64,FCPU_256,FCPU_1024
}Timer1_prescaler;

typedef enum{
	NORMAL,CTC=4
}Timer1_Mode;

typedef struct{
	uint16 initial_value;
	uint16 compare_value;
	Timer1_prescaler prescaler;
	Timer1_Mode mode;

}Timer1_Config;

/*Function Prototypes*/
void Timer1_init(const Timer1_Config *config_ptr);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
