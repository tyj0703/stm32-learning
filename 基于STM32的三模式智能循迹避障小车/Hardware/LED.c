#include "stm32f10x.h"                  // Device header
#include "DELAY.h"
#include "LED.h"

/*LED初始化*/
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);		
	
	GPIO_InitTypeDef GPIO_InitStructure;						
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;				
	GPIO_InitStructure.GPIO_Pin=OnBoard_LED_Pin;			
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;				
	GPIO_Init(GPIOC,&GPIO_InitStructure);						
	
	GPIO_SetBits(GPIOC,OnBoard_LED_Pin);					
}
void LED_OnBoard_ON(void)
{	
	GPIO_ResetBits(GPIOC,OnBoard_LED_Pin);
}

void LED_OnBoard_OFF(void)
{
	GPIO_SetBits(GPIOC,OnBoard_LED_Pin);
}

void LED_OnBoard_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOC,OnBoard_LED_Pin)==0)
	{
		GPIO_SetBits(GPIOC,OnBoard_LED_Pin);
	}
	else
	{
		GPIO_ResetBits(GPIOC,OnBoard_LED_Pin);
	}
}
void LED_OnBoard_Shine(void)
{
	LED_OnBoard_ON();
	Delay_ms(500);
	LED_OnBoard_OFF();
	Delay_ms(500);
}
