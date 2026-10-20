#ifndef __MOTOR_H
#define __MOTOR_H

/*	AO1/AO2左前电机1	*/
#define TB6612_Motor_1_PWMA GPIO_Pin_6			//GPIOA
#define TB6612_Motor_1_AIN1 GPIO_Pin_4			//GPIOA
#define TB6612_Motor_1_AIN2 GPIO_Pin_5			//GPIOA

/*	BO1/BO2左后电机2	*/
#define TB6612_Motor_2_PWMB GPIO_Pin_7			//GPIOA
#define TB6612_Motor_2_BIN1 GPIO_Pin_0			//GPIOA
#define TB6612_Motor_2_BIN2 GPIO_Pin_1			//GPIOA

/*	AO1/AO2右前电机3	*/
#define TB6612_Motor_3_PWMA GPIO_Pin_0			//GPIOB
#define TB6612_Motor_3_AIN1 GPIO_Pin_2			//GPIOA
#define TB6612_Motor_3_AIN2 GPIO_Pin_3			//GPIOA

/*	BO1/BO2右后电机4	*/
#define TB6612_Motor_4_PWMB GPIO_Pin_1			//GPIOB
#define TB6612_Motor_4_BIN1 GPIO_Pin_11			//GPIOA
#define TB6612_Motor_4_BIN2 GPIO_Pin_12			//GPIOA

#define MOTOR_PWM_ARR               7199		//控制一个PWM周期计多少个计数
#define MOTOR_PWM_PSC               0			//控制PWM的计数时钟的快慢

#define MOTOR_SPEED_MAX             1000

#define MOTOR_LEFT_FRONT            1   /* 左前电机1 */
#define MOTOR_LEFT_REAR             2   /* 左后电机2 */
#define MOTOR_RIGHT_FRONT           3   /* 右前电机3 */
#define MOTOR_RIGHT_REAR            4   /* 右后电机4 */

#define DIR_STOP                    0
#define DIR_FORWARD                 1
#define DIR_REVERSE                 2
#define DIR_BRAKE                   3
//这个要测
#define MOTOR_TURN_90_DEG_TIME_MS   500 /* 原地转90度所需时间，单位ms */
#define MOTOR_TURN_DEFAULT_SPEED    600 /* 转向默认速度，0~1000 */




void Motor_Init(void);
void Motor_SetSpeed(uint8_t motor, int16_t speed);
void Motor_Stop(uint8_t motor);
void Motor_Brake(uint8_t motor);
void Motor_StopAll(void);
void Motor_BrakeAll(void);

/* 整车动作 */
void Motor_Forward(int16_t speed);
void Motor_Backward(int16_t speed);
void Motor_TurnLeft(int16_t speed);
void Motor_TurnRight(int16_t speed);
void Motor_TurnAngle(int16_t angle, int16_t speed);

#endif

