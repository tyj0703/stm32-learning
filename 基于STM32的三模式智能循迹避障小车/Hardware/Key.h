#ifndef __KEY_H
#define __KEY_H

#define Button_Pin GPIO_Pin_10					//GPIOB

extern uint8_t Key_Num;


void Key_Init(void);
uint8_t Key_GetNum(void);



#endif
