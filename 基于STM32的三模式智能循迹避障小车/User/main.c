#include "stm32f10x.h"
#include "APP.h"

int main(void)
{
	App_Init();
	while(1)
	{
		App_Run();
	}		
}




