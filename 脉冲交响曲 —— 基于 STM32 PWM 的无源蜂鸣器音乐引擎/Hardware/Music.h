#ifndef __Music_H
#define __Music_H

#include "stm32f10x.h"                  // Device header

#define MUSIC_TIM_CLK		RCC_APB1Periph_TIM2		//总线时钟
#define MUSIC_GPIO_CLK 		RCC_APB2Periph_GPIOA	//总线时钟
#define MUSIC_TIM			TIM2					//使用TIM2
#define MUSIC_GPIO			GPIOA					//使用GPIOA
#define MUSIC_GPIO_Pin 		GPIO_Pin_0				//Pin_0




//定义结构体，即单个音符，包括音调和音长
typedef struct
{
	uint16_t freq;			//音调
	uint16_t duration;		//音长
}Note_t;


//外部函数声明
void Music_Init(void);
void MusicPlay(const Note_t *score,uint16_t length);
void Silence(void);


#endif
