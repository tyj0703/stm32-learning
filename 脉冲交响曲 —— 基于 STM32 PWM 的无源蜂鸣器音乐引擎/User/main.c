#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Music.h"
#include "notes.h"
#include "song.h"

#define WaitingTime 5	//µ¥Î»s


int main(void)
{
	
	Music_Init();
	
	while(1)
	{
		Delay_s(WaitingTime);		
		MusicPlay(song,Length);
		Delay_s(WaitingTime);
	}
}
