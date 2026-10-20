#ifndef __SERVO_H
#define __SERVO_H

#define Servo_Pin GPIO_Pin_8					//GPIOA
#define SERVO_OFFSET  -50    		//舵机角度偏移校准



void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);   // 0~180

#endif
