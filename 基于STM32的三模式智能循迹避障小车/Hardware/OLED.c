#include "stm32f10x.h"                  // Device header
#include "DELAY.h"
#include "OLED.h"
#include "OLED_Font.h"

#define	OLED_W_SCL(x)	GPIO_WriteBit(GPIOB,OLED_SCL,(BitAction)x)
#define	OLED_W_SDA(x)	GPIO_WriteBit(GPIOB,OLED_SDA,(BitAction)x)

//内部函数函数
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
void OLED_I2C_SendByte(uint8_t Byte);
void OLED_W_Command(uint8_t Command);
void OLED_W_Data(uint8_t Data);
void OLED_SetCursor(uint8_t Page,uint8_t Column);
void OLED_W_PageBytes(uint8_t Page,uint8_t Column,const uint8_t *Data,uint8_t Length);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);




void OLED_Clear(void)
{
	uint8_t Page,Column;
	uint8_t zero[OLED_Width];
	
	for(Column=0;Column<OLED_Width;Column++)
	{
		zero[Column]=0x00;
	}
	
	for(Page=0;Page<OLED_Pages;Page++)
	{
		OLED_W_PageBytes(Page,0,zero,OLED_Width);
	}
}	


void OLED_Init(void)
{
	Delay_ms(100);
	
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;  /* 开漏输出 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = OLED_SCL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = OLED_SDA;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    OLED_W_SCL(1);
    OLED_W_SDA(1);
	
	
	OLED_W_Command(0xAE);	//关闭显示
	
	OLED_W_Command(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_W_Command(0x80);
	
	OLED_W_Command(0xA8);	//设置多路复用率
	OLED_W_Command(0x3F);
	
	OLED_W_Command(0xD3);	//设置显示偏移
	OLED_W_Command(0x00);
	
	OLED_W_Command(0x40);	//设置显示开始行
	
	OLED_W_Command(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_W_Command(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_W_Command(0xDA);	//设置COM引脚硬件配置
	OLED_W_Command(0x12);
	
	OLED_W_Command(0x81);	//设置对比度控制
	OLED_W_Command(0xCF);

	OLED_W_Command(0xD9);	//设置预充电周期
	OLED_W_Command(0xF1);

	OLED_W_Command(0xDB);	//设置VCOMH取消选择级别
	OLED_W_Command(0x30);

	OLED_W_Command(0xA4);	//设置整个显示打开/关闭

	OLED_W_Command(0xA6);	//设置正常/倒转显示

	OLED_W_Command(0x8D);	//设置充电泵
	OLED_W_Command(0x14);

	OLED_W_Command(0xAF);	//开启显示
	
	OLED_Clear();				//OLED清屏

}





void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_W_Data(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_W_Data(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}


void OLED_ShowImage(uint16_t pixel_x,uint8_t pixel_y,uint8_t Page,uint8_t Column,const uint8_t *Image)
{
    uint8_t p;
    uint8_t pages = pixel_y / 8;         

    for (p = 0; p < pages; p++)
    {
        OLED_W_PageBytes(Page + p, Column,Image + (uint16_t)p * pixel_x,(uint8_t)pixel_x);
    }
}


//内部函数

void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
	
}
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
	
}
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	//应答位时钟？
	OLED_W_SCL(1);
	OLED_W_SCL(0);
}
void OLED_W_Command(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLED_I2C_Address);
	OLED_I2C_SendByte(0x00);
	OLED_I2C_SendByte(Command);
	OLED_I2C_Stop();
}

void OLED_W_Data(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLED_I2C_Address);
	OLED_I2C_SendByte(0x40);
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}
void OLED_SetCursor(uint8_t Page,uint8_t Column)
{
	OLED_W_Command(0xB0|Page);
	OLED_W_Command(0x10|((Column&0xF0)>>4));
	OLED_W_Command(0x00|(Column&0x0F));
	
}
void OLED_W_PageBytes(uint8_t Page,uint8_t Column,const uint8_t *Data,uint8_t Length)
{
	uint8_t i;
	OLED_SetCursor(Page,Column);
	
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLED_I2C_Address);
	OLED_I2C_SendByte(0x40);
	
	for(i=0;i<Length;i++)
	{
		OLED_I2C_SendByte(Data[i]);
		
	}
	OLED_I2C_Stop();
}	
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}
