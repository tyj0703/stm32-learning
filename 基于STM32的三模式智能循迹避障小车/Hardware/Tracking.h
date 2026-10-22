#ifndef __TRACKING_H
#define __TRACKING_H


//从左至右
#define Sensor0_Pin GPIO_Pin_12				//GPIOB
#define Sensor1_Pin GPIO_Pin_13				//GPIOB
#define Sensor2_Pin GPIO_Pin_14				//GPIOB
#define Sensor3_Pin GPIO_Pin_15				//GPIOB

#define Sensor0_Bit		0x01				//最左，位置权重-1.5
#define Sensor1_Bit		0x02				//左中，位置权重-0.5
#define Sensor2_Bit		0x04				//右中，位置权重+0.5
#define Sensor3_Bit		0x08				//最右，位置权重+1.5


#define Sensor_State_TooRight 				0x01
#define Sensor_State_LittleRight 			0x02
#define Sensor_State_LittleRight_2			0x03
#define Sensor_State_TooLeft 			   	0x08
#define Sensor_State_LittleLeft 			0x04
#define Sensor_State_LittleLeft_2			0x0C
#define Sensor_State_Lost					0x00
#define Sensor_State_Normal					0x06

#define TRACK_BASE_SPEED   350      /* 基础速度，约 PWM 量程一半 */
#define TRACK_OUT_MAX      400      /* PID输出限幅，防止单侧反转 */
#define TRACK_KP           150.0f
#define TRACK_KI           0.0f     /* 循迹一般不用 I */
#define TRACK_KD           40.0f



#include "stm32f10x.h"


void  Track_Init(void);
void  Track_PID_Init(void);
void  Track_PID_Update(void);
void Track_Run(void);
float Track_GetError(void);
uint8_t Track_Read(void);


#endif


