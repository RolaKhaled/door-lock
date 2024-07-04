#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"

/*Definitions*/
#define BUZZER_PORT_ID  PORTB_ID
#define BUZZER_PIN_ID  PIN7_ID

/*Types Declaration*/


/*Function Prototypes*/
void Buzzer_init();
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* BUZZER_H_ */
