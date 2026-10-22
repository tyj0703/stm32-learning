#include "stm32f10x.h"
#include "OBSTACLE.h"
#include "MOTOR.h"
#include "SERVO.h"
#include "DELAY.h"


void Obstacle_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // HC-SR04 Trig：输出
    GPIO_InitStructure.GPIO_Pin   = ultrasonic_Trig;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // HC-SR04 Echo：输入
    GPIO_InitStructure.GPIO_Pin  = ultrasonic_Echo;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Prescaler     = 72 - 1;      // 72MHz / 72 = 1MHz，1us 一个数
    TIM_TimeBaseStructure.TIM_Period        = 65535;       // 最大计数
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM4,ENABLE);
	
    // 舵机初始化 + 探头回正
    Servo_Init();
    Servo_SetAngle(ANGLE_MID);
    Delay_ms(500);
}

uint16_t Obstacle_GetDistance(void)
{
    uint16_t start, end, width;
    uint16_t t0;
	
	/*触发超声波模块发波，至少要保持10us的高电平才会触发*/
    GPIO_SetBits(GPIOB, ultrasonic_Trig);
    Delay_us(10);
    GPIO_ResetBits(GPIOB, ultrasonic_Trig);

    /*等待模块确实发波*/
    t0 = TIM4->CNT;                                       
    while(GPIO_ReadInputDataBit(GPIOB, ultrasonic_Echo) == 0)
    {
        if((uint16_t)(TIM4->CNT - t0) > 30000) return 0;
    }

    start = TIM4->CNT;                                   

    while(GPIO_ReadInputDataBit(GPIOB, ultrasonic_Echo) == 1)
    {
        if((uint16_t)(TIM4->CNT - start) > 30000) return 0;
    }

    end   = TIM4->CNT;                                   
    width = end - start;                                

    return width / 58;			//距离=（width/2）*0.034cm/us
}

uint16_t MeasureAt(uint8_t angle)
{
    uint16_t d;

    Servo_SetAngle(angle);
    Delay_ms(300);                 // 等舵机到位
    d = Obstacle_GetDistance();
    Delay_ms(60);                  // 等回波余波消散

    return d;
}

void Avoid_Run(void)
{
    static uint8_t turnHold = 0;   // 转弯后保持直行的计数
    uint16_t dL, dM, dR;

    // ---- 保持期：强制直行，不读传感器 ----
    if(turnHold > 0)
    {
        turnHold--;
        Motor_Forward(AVOID_SPEED);
        return;
    }

    // ---- 平时：探头朝正前，只测前方 ----
    Servo_SetAngle(ANGLE_MID);
    Delay_ms(60);

    dM = Obstacle_GetDistance();
    if(dM == 0) dM = 999;          // 超时当作很远

    // ---- 前方畅通：直行 ----
    if(dM > OBSTACLE_DIST)
    {
        Motor_Forward(AVOID_SPEED);
        return;
    }
	//隐藏了判断dM≤OBSTACLE_DIST
    Motor_StopAll();
    Delay_ms(100);

    dL = MeasureAt(ANGLE_LEFT);
    dR = MeasureAt(ANGLE_RIGHT);

    if(dL == 0) dL = 999;
    if(dR == 0) dR = 999;

    Servo_SetAngle(ANGLE_MID);     // 扫完回正
    Delay_ms(150);

    // ---- 决策 ----
    if(dL > OBSTACLE_DIST && dR > OBSTACLE_DIST)
    {
        // 两侧都通，哪边远往哪转
        if(dL >= dR)
        {
            Motor_TurnLeft(TURN_SPEED);
            Delay_ms(350);
        }
        else
        {
            Motor_TurnRight(TURN_SPEED);
            Delay_ms(350);
        }
        turnHold = 12;
    }
    else if(dL > OBSTACLE_DIST)
    {
        // 只有左边通：左转
        Motor_TurnLeft(TURN_SPEED);
        Delay_ms(400);
        turnHold = 12;
    }
    else if(dR > OBSTACLE_DIST)
    {
        // 只有右边通：右转
        Motor_TurnRight(TURN_SPEED);
        Delay_ms(400);
        turnHold = 12;
    }
    else
    {
        // 三面都堵：后退 + 掉头
        Motor_Backward(BACK_SPEED);
        Delay_ms(400);

        Motor_StopAll();
        Delay_ms(100);

        Motor_TurnLeft(TURN_SPEED);
        Delay_ms(600);
        turnHold = 15;
    }
}

