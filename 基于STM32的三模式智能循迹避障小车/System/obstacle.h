#ifndef __OBSTACLE_H
#define __OBSTACLE_H


#define ultrasonic_Trig	GPIO_Pin_8				//GPIOB
#define ultrasonic_Echo GPIO_Pin_9				//GPIOB

#define OBSTACLE_DIST   40      // 障碍判定距离 cm
#define AVOID_SPEED     650     // 直行速度
#define TURN_SPEED      650     // 转弯速度
#define BACK_SPEED      500     // 后退速度

#define ANGLE_LEFT      130     // 左扫角度
#define ANGLE_MID       90      // 正前
#define ANGLE_RIGHT     40      // 右扫角度



void Obstacle_Init(void);
void Avoid_Run(void);
uint16_t Obstacle_GetDistance(void);
uint16_t MeasureAt(uint8_t angle);

#endif
