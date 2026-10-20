#include "stm32f10x.h"
#include "SERVO.h"
#include "DELAY.h"




void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 时钟：GPIOA 在 APB2，TIM1 也在 APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // PA8 复用推挽输出
    GPIO_InitStructure.GPIO_Pin   = Servo_Pin;      // GPIO_Pin_8
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM1 时基：1us 计数，20ms 周期（50Hz）
    TIM_TimeBaseStructure.TIM_Period        = 20000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // CH1 PWM1
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse       = 1500;     // 1.5ms对应中位指令对应90°
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);

    //高级定时器开
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    Servo_SetAngle(90);
	
	//等待舵机转到位
    Delay_ms(500);
}


/*

舵机角度		对应脉宽		对应CCR
0			0.5ms		500
90			1.5ms		1500
180			2.5ms		2500
*/
void Servo_SetAngle(uint8_t angle)
{
	uint16_t ccr;
    int16_t real = (int16_t)angle + SERVO_OFFSET;

    if(real < 0)   real = 0;
    if(real > 180) real = 180;

    ccr = 500 + (uint16_t)real * 2000 / 180;
    TIM_SetCompare1(TIM1, ccr);
}

