#include "stm32f10x.h"                  // Device header
#include "TRACKING.h"
#include "PID.h"
#include "MOTOR.h"


static PID_t  g_pid;
static float  g_last_error = 0.0f;  /* 丢线时沿用上次误差 */


void Track_PID_Init(void)
{
    PID_Init(&g_pid, TRACK_KP, TRACK_KI, TRACK_KD, TRACK_OUT_MAX);
    g_last_error = 0.0f;
}


void Track_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=Sensor0_Pin|Sensor1_Pin|Sensor2_Pin|Sensor3_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	Track_PID_Init();
	
}

uint8_t Track_Read(void)
{
	uint16_t port=GPIO_ReadInputData(GPIOB);
	uint8_t s=0;
	if((port&Sensor0_Pin)==0)
	{
		s|=Sensor0_Bit;
	}
	if((port&Sensor1_Pin)==0)
	{
		s|=Sensor1_Bit;
	}
	if((port&Sensor2_Pin)==0)
	{
		s|=Sensor2_Bit;
	}
	if((port&Sensor3_Pin)==0)
	{
		s|=Sensor3_Bit;
	}
	
	return s&(Sensor0_Bit|Sensor1_Bit|Sensor2_Bit|Sensor3_Bit);
}

float Track_GetError(void)
{
    static const uint8_t bits[4]={Sensor0_Bit, Sensor1_Bit,Sensor2_Bit, Sensor3_Bit};
    static const float weight[4]={-1.5f, -0.5f, 0.5f, 1.5f};

    uint8_t s = Track_Read();
    float sum=0.0f;
    uint8_t count=0;
    uint8_t i;

    for (i=0;i<4;i++)
    {
        if (s&bits[i])
        {
            sum+=weight[i];
            count++;
        }
    }

    if (count==0)
    {
        return 0.0f;      /* 丢线，单独处理，见第六步 */
    }

    return sum/(float)count;
}






void Track_PID_Update(void)
{
    uint8_t s = Track_Read();
    float error, out;
    int16_t left, right;

    if (s == 0)
    {
        /* 丢线：沿用上次误差，让车继续按原方向找回线 */
        error = g_last_error;
    }
    else
    {
        error = Track_GetError();
        g_last_error = error;
    }

    out   = PID_Calc(&g_pid, error);
    left  = (int16_t)(TRACK_BASE_SPEED + out);
    right = (int16_t)(TRACK_BASE_SPEED - out);

    /* 左右轮限幅，保证不越界 */
    if (left  >  1000) left  =  1000;
    if (left  < -1000) left  = -1000;
    if (right >  1000) right =  1000;
    if (right < -1000) right = -1000;

    Motor_SetSpeed(1, left);
    Motor_SetSpeed(2, left);
    Motor_SetSpeed(3, right);
    Motor_SetSpeed(4, right);
	
}

void Track_Run(void)
{
	Track_PID_Update();
}
