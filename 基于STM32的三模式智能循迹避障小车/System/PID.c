#include "PID.h"

void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float out_max)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
    pid->out_max    = out_max;
}

void PID_Reset(PID_t *pid)
{
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
}

float PID_Calc(PID_t *pid, float error)
{
    float derivative, output;

    /* 积分项 + 积分限幅（抗饱和） */
    pid->integral += error;
    if (pid->integral >  pid->out_max) pid->integral =  pid->out_max;
    if (pid->integral < -pid->out_max) pid->integral = -pid->out_max;

    /* 微分项 */
    derivative = error - pid->prev_error;
    pid->prev_error = error;

    output = pid->Kp * error
           + pid->Ki * pid->integral
           + pid->Kd * derivative;

    /* 输出限幅 */
    if (output >  pid->out_max) output =  pid->out_max;
    if (output < -pid->out_max) output = -pid->out_max;

    return output;
}


