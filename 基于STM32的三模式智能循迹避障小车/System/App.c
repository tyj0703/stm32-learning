#include "stm32f10x.h"                  // Device header
#include "MOTOR.h"
#include "BLUETOOTH.h"
#include "OBSTACLE.h"
#include "OLED.h"
#include "APP.h"
#include "KEY.h"
#include "TRACKING.h"
#include "OLED_IMAGE.H"

static uint8_t current_mode=0xFF;


void App_Init(void)
{
	OLED_Init();
	OLED_Clear();
//	OLED_ShowImage(128,64,0,1,Genshin);
	Key_Init();
	Motor_Init();
	Motor_StopAll();

}

void App_Run()
{
	KeyNum=Key_GetNum();
	switch(KeyNum)
	{
		case 0:
		{
			if(current_mode!=0)
			{
				Motor_StopAll();
				current_mode=0;
				OLED_Clear();
				OLED_ShowString(1,1,"AVOIDING");
				Obstacle_Init();
			}
			Avoid_Run();
			break;
		}
		case 1:
		{
			if(current_mode!=1)
			{
				Motor_StopAll();
				current_mode=1;
				OLED_Clear();
				OLED_ShowString(1,1,"TRACKING");
				Track_Init();
			}
			Track_Run();
			break;
		}
		case 2:
		{
			if(current_mode!=2)
			{
				Motor_StopAll();
				current_mode=2;
				OLED_Clear();
				OLED_ShowString(1,1,"BLUETOOTH");
				Bluetooth_Init();
			}
			Bluetooth_Run();
			break;
		}
		default:
			break;
	}
	
}

