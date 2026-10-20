#include "stm32f10x.h"                  // Device header
#include "MOTOR.h"
#include "BLUETOOTH.h"
#include "OBSTACLE.h"
#include "OLED.h"
#include "APP.h"
#include "KEY.h"
#include "TRACKING.h"


static uint8_t current_mode=0;


void App_Init(void)
{
	OLED_Init();
	OLED_Clear();
	Key_Init();
	Motor_Init();
	Motor_StopAll();
	Track_Init();

}

void App_Run()
{
	switch(KeyNum)
	{
		case 0:
		{
			if(current_mode!=0)
			{
				current_mode=0;
				OLED_Clear();
				OLED_ShowString(1,1,"AVOIDING");
				Obstacle_Init();
			}
			Obstacle_AvoidRun();
			break;
		}
		case 1:
		{
			if(current_mode!=1)
			{
				current_mode=1;
				OLED_Clear();
				OLED_ShowString(1,1,"TRACKING");
				Track_PID_Init();
			}
			Track_Run();
			break;
		}
		case 2:
		{
			if(current_mode!=2)
			{
				current_mode=2;
				OLED_Clear();
				OLED_ShowString(1,1,"BLUETOOTH");
				Bluetooth_Init();
			}
			BluetoothRun();
			break;
		}
		default:
			break;
	}
	
}

