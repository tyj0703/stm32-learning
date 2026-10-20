#ifndef __LED_H
#define __LED_H

#define OnBoard_LED_Pin GPIO_Pin_13				//GPIOC



void LED_Init(void);
void LED_OnBoard_ON(void);
void LED_OnBoard_OFF(void);
void LED_OnBoard_Turn(void);
void LED_OnBoard_Shine(void);


#endif
