#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Music.h"


//内部函数声明
static void SetFreq(uint16_t freq);



void Music_Init(void)
{
	//使能时钟
	RCC_APB1PeriphClockCmd(MUSIC_TIM_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(MUSIC_GPIO_CLK,ENABLE);
	
	//配置时钟源
	TIM_InternalClockConfig(MUSIC_TIM);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;		//ARR,自动重装载寄存器，决定PWM周期（频率）
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;		//PSC，预分频器，决定计数快慢，决定整体频率
	TIM_TimeBaseInit(MUSIC_TIM,&TIM_TimeBaseInitStruct);
	
	//配置输出比较通道
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=700;				//CCR，捕获/比较寄存器，决定PWM高电平持续时间（占空比），占空比越接近中间值，响度越大
	TIM_OC1Init(MUSIC_TIM,&TIM_OCInitStruct);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=MUSIC_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(MUSIC_GPIO,&GPIO_InitStruct);
	
	
	
	//默认静音
	TIM_Cmd(MUSIC_TIM,DISABLE);
}


void SetFreq(uint16_t freq)
{
	if(freq==0)
	{
		Silence();
		return;
	}
	uint32_t arr=1000000/freq-1;
	if(arr>65535)
	{
		arr=65535;
	}
	if(arr<1)
	{
		arr=1;
	}
	TIM_SetAutoreload(MUSIC_TIM,arr);				//修改ARR（频率/音调）
	TIM_SetCompare1(MUSIC_TIM,arr/2);				//修改CCR（占空比/响度）
	TIM_Cmd(MUSIC_TIM,ENABLE);
}

void MusicPlay(const Note_t *song,uint16_t length)
{
	for(uint16_t i=0;i<length;i++)
	{
		if(song[i].freq==0)
		{
			Silence();
		}
		else
		{
			SetFreq(song[i].freq);
		}
		Delay_ms(song[i].duration);
	}
	
	Silence();	
}

//静音
void Silence(void)
{
	TIM_SetCompare1(MUSIC_TIM, MUSIC_TIM->ARR+1);

}

