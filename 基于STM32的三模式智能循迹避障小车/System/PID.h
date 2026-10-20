#ifndef __PID_H
#define __PID_H

typedef struct
{
    float Kp, Ki, Kd;
    float integral;
    float prev_error;
    float out_max;
} PID_t;

void  PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float out_max);
void  PID_Reset(PID_t *pid);
float PID_Calc(PID_t *pid, float error);

#endif
