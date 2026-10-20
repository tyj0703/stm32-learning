#ifndef __HC05_H
#define __HC05_H

#define HC05_TX GPIO_Pin_9						//GPIOA
#define HC05_RX GPIO_Pin_10						//GPIOA


void HC05_Init(void);
uint8_t HC05_GetRxFlag(void);
uint16_t HC05_GetRxData(void);


#endif
