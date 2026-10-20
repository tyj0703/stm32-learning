#include "stm32f10x.h"                  // Device header
#include "DELAY.h"
#include "KEY.h"

uint8_t KeyNum;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=Button_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	if(GPIO_ReadInputDataBit(GPIOB, Button_Pin) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, Button_Pin) == 0);
		Delay_ms(20);
		KeyNum=(KeyNum+1)%3;
	}
	return KeyNum;
}
