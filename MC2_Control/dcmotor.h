#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include"std_types.h"

/*Definitions*/
#define MOTOR_MAX_SPEED 256

#define MOTOR_PORT_ID PORTA_ID
#define MOTOR_PIN1_ID PIN0_ID
#define MOTOR_PIN2_ID PIN1_ID

/*Types Declaration*/
typedef enum{
	OFF,ACW,CW
}DCMotor_State;

/*Function Prototypes*/
void DCMotor_init(void);

void DCMotor_rotate(uint8 speed,DCMotor_State state);
#endif /* DCMOTOR_H_ */
