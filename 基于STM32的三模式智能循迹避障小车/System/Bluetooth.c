#include "stm32f10x.h"                  // Device header
#include "HC05.h"
#include "OLED.h"
#include "MOTOR.h"
#include "DELAY.h"

void Bluetooth_Init(void)
{	
	//初始化
	HC05_Init();

}
void BluetoothRun(void)
{
	//OLED显示模式
	
	
	//蓝牙逻辑
	if(HC05_GetRxFlag()==1)
	{
		uint16_t ins=HC05_GetRxData();
		switch(ins)
		{
			case 'F':
			{
				OLED_ShowChar(2,1,'F');
				Motor_Forward(600);
//				Delay_ms(500);
				break;
			}
			case 'B':
			{
				OLED_ShowChar(2,1,'B');
				Motor_Backward(600);
//				Delay_ms(500);
				break;
			}
			case 'R':
			{
				OLED_ShowChar(2,1,'R');
				Motor_TurnRight(600);
//				Delay_ms(500);
				break;
			}
			case 'L':
			{
				OLED_ShowChar(2,1,'L');
				Motor_TurnLeft(600);
//				Delay_ms(500);
				break;
			}
			case 'S':
			{
				OLED_ShowChar(2,1,'S');
				Motor_StopAll();
				break;
			}
				
		}

	}
	
}
