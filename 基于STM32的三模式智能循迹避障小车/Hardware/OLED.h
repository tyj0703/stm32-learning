#ifndef __OLED_H
#define __OLED_H

#define OLED_SCL GPIO_Pin_8						//GPIOB
#define OLED_SDA GPIO_Pin_9						//GPIOB
#define OLED_I2C_Address	0x78				//地址


#define OLED_Width	128	//横向128列
#define OLED_Height	64	//纵向64行
#define OLED_Pages	8	//8页




//外部接口函数
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowImage(uint16_t pixel_x,uint8_t pixel_y,uint8_t Page,uint8_t Column,const uint8_t *Image);



#endif
