#include "stm32f10x.h"
#include "MOTOR.h"
#include "DELAY.h"

//内部函数
static void Motor_SetPWM(uint8_t motor, uint16_t pwm);
static void Motor_SetDirection(uint8_t motor, uint8_t dir);
static uint16_t Motor_SpeedToPWM(int16_t speed);


void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//PWM控制转速
    GPIO_InitStructure.GPIO_Pin = TB6612_Motor_1_PWMA | TB6612_Motor_2_PWMB;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PWM控制转速
    GPIO_InitStructure.GPIO_Pin = TB6612_Motor_3_PWMA | TB6612_Motor_4_PWMB;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	//控制转向
    GPIO_InitStructure.GPIO_Pin = TB6612_Motor_1_AIN1 | TB6612_Motor_1_AIN2 |
                                  TB6612_Motor_2_BIN1 | TB6612_Motor_2_BIN2 |
                                  TB6612_Motor_3_AIN1 | TB6612_Motor_3_AIN2 |
                                  TB6612_Motor_4_BIN1 | TB6612_Motor_4_BIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 初始状态：全部停止 */
    GPIO_ResetBits(GPIOA, TB6612_Motor_1_AIN1 | TB6612_Motor_1_AIN2 |
                           TB6612_Motor_2_BIN1 | TB6612_Motor_2_BIN2 |
                           TB6612_Motor_3_AIN1 | TB6612_Motor_3_AIN2 |
                           TB6612_Motor_4_BIN1 | TB6612_Motor_4_BIN2);

	
    TIM_TimeBaseStructure.TIM_Period = MOTOR_PWM_ARR;
    TIM_TimeBaseStructure.TIM_Prescaler = MOTOR_PWM_PSC;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	//4个PWM通道
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

	
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

}

void Motor_SetSpeed(uint8_t motor, int16_t speed)
{
    if (motor < 1 || motor > 4) return;

    if (speed > 0) 
	{
        Motor_SetDirection(motor, DIR_FORWARD);
        Motor_SetPWM(motor, Motor_SpeedToPWM(speed));
    } 
	else if (speed < 0) 
	{
        Motor_SetDirection(motor, DIR_REVERSE);
        Motor_SetPWM(motor, Motor_SpeedToPWM(-speed));
    } 
	else 
	{
        Motor_SetDirection(motor, DIR_STOP);
        Motor_SetPWM(motor, 0);
    }
}

void Motor_Stop(uint8_t motor)
{
    if (motor < 1 || motor > 4) return;
    Motor_SetDirection(motor, DIR_STOP);
    Motor_SetPWM(motor, 0);
}

void Motor_Brake(uint8_t motor)
{
    if (motor < 1 || motor > 4) return;
    Motor_SetDirection(motor, DIR_BRAKE);
    Motor_SetPWM(motor, 0);
}

void Motor_StopAll(void)
{
    uint8_t i;
    for (i = 1; i <= 4; i++) {
        Motor_Stop(i);
    }
}

void Motor_BrakeAll(void)
{
    uint8_t i;
    for (i = 1; i <= 4; i++) {
        Motor_Brake(i);
    }
}

void Motor_Forward(int16_t speed)
{
    uint8_t i;
    for (i = 1; i <= 4; i++) {
        Motor_SetSpeed(i, speed);
    }
}

void Motor_Backward(int16_t speed)
{
    uint8_t i;
    for (i = 1; i <= 4; i++) {
        Motor_SetSpeed(i, -speed);
    }
}

void Motor_TurnLeft(int16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, -speed);
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, speed);
}

void Motor_TurnRight(int16_t speed)
{
    Motor_SetSpeed(MOTOR_LEFT_FRONT, speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, speed);
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, -speed);
}

//通过转动的时间来确定转动的角度
void Motor_TurnAngle(int16_t angle, int16_t speed)
{
    uint32_t time_ms;
    uint16_t abs_angle;
    int16_t turn_speed;

    if (angle == 0) return;

    abs_angle = (angle > 0) ? (uint16_t)angle : (uint16_t)(-angle);
    turn_speed = (speed > 0) ? speed : -speed;
    if (turn_speed == 0) 
	{
        turn_speed = MOTOR_TURN_DEFAULT_SPEED;
    }
    if (turn_speed > MOTOR_SPEED_MAX) 
	{
        turn_speed = MOTOR_SPEED_MAX;
    }
	//原地转90度的时间为MOTOR_TURN_90_DEG_TIME_MS，通过转动角度与90度的比值确定转向时间
    time_ms = (uint32_t)abs_angle * MOTOR_TURN_90_DEG_TIME_MS / 90;

    if (angle > 0) 
	{
        Motor_TurnRight(turn_speed);
    } 
	else 
	{
        Motor_TurnLeft(turn_speed);
    }

    Delay_ms(time_ms);
    Motor_StopAll();
}


static void Motor_SetDirection(uint8_t motor, uint8_t dir)
{
    switch (motor) 
	{
        case 1:
            if (dir == DIR_FORWARD) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_1_AIN1);
                GPIO_ResetBits(GPIOA, TB6612_Motor_1_AIN2);
            } 
			else if (dir == DIR_REVERSE) 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_1_AIN1);
                GPIO_SetBits(GPIOA, TB6612_Motor_1_AIN2);
            } 
			else if (dir == DIR_BRAKE) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_1_AIN1 | TB6612_Motor_1_AIN2);
            } 
			else 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_1_AIN1 | TB6612_Motor_1_AIN2);
            }
            break;

        case 2:
            if (dir == DIR_FORWARD) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_2_BIN1);
                GPIO_ResetBits(GPIOA, TB6612_Motor_2_BIN2);
            } 
			else if (dir == DIR_REVERSE) 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_2_BIN1);
                GPIO_SetBits(GPIOA, TB6612_Motor_2_BIN2);
            } 
			else if (dir == DIR_BRAKE) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_2_BIN1 | TB6612_Motor_2_BIN2);
            } 
			else 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_2_BIN1 | TB6612_Motor_2_BIN2);
            }
            break;

        case 3:
            if (dir == DIR_FORWARD) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_3_AIN1);
                GPIO_ResetBits(GPIOA, TB6612_Motor_3_AIN2);
            } 
			else if (dir == DIR_REVERSE) 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_3_AIN1);
                GPIO_SetBits(GPIOA, TB6612_Motor_3_AIN2);
            } 
			else if (dir == DIR_BRAKE) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_3_AIN1 | TB6612_Motor_3_AIN2);
            } 
			else 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_3_AIN1 | TB6612_Motor_3_AIN2);
            }
            break;

        case 4:
            if (dir == DIR_FORWARD) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_4_BIN1);
                GPIO_ResetBits(GPIOA, TB6612_Motor_4_BIN2);
            } 
			else if (dir == DIR_REVERSE) 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_4_BIN1);
                GPIO_SetBits(GPIOA, TB6612_Motor_4_BIN2);
            } 
			else if (dir == DIR_BRAKE) 
			{
                GPIO_SetBits(GPIOA, TB6612_Motor_4_BIN1 | TB6612_Motor_4_BIN2);
            } 
			else 
			{
                GPIO_ResetBits(GPIOA, TB6612_Motor_4_BIN1 | TB6612_Motor_4_BIN2);
            }
            break;

        default:
            break;
    }
}

static void Motor_SetPWM(uint8_t motor, uint16_t pwm)
{
    switch (motor) 
	{
        case 1: TIM_SetCompare1(TIM3, pwm); break;
        case 2: TIM_SetCompare2(TIM3, pwm); break;
        case 3: TIM_SetCompare3(TIM3, pwm); break;
        case 4: TIM_SetCompare4(TIM3, pwm); break;
        default: break;
    }
}

static uint16_t Motor_SpeedToPWM(int16_t speed)
{	
	//取速度的绝对值
    uint32_t abs_speed = (speed >= 0) ? (uint32_t)speed : (uint32_t)(-speed);

    if (abs_speed > MOTOR_SPEED_MAX) {
        abs_speed = MOTOR_SPEED_MAX;
    }
	
	//映射函数：设置速度占最大速度的百分比
    return (uint16_t)(abs_speed * (MOTOR_PWM_ARR + 1) / MOTOR_SPEED_MAX);
}

