#include "stm32f10x.h"                  // Device header
#include "HC05.h"

volatile uint16_t HC05_RxData;		//定义串口接收的数据变量
volatile uint8_t HC05_RxFlag;		//定义串口接收的标志位变量



void HC05_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//Pin_9,TX,AF_PP
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=HC05_TX;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//Pin_10,RX,IN_FlOATING
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=HC05_RX;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置USART
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate=38400;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	//配置中断
	USART_ITConfig(USART1 ,USART_IT_RXNE,ENABLE);
	
	NVIC_InitTypeDef NVIC_Initstruct;
    NVIC_Initstruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Initstruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initstruct.NVIC_IRQChannelSubPriority=1;
	
	//中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_Init(&NVIC_Initstruct);
	
	//USART使能
	USART_Cmd(USART1,ENABLE);
	
	
}

uint8_t HC05_GetRxFlag(void)
{
	if (HC05_RxFlag == 1)					//如果标志位为1
	{
		HC05_RxFlag = 0;
		return 1;						//则返回1，并自动清零标志位
	}
	return 0;							//如果标志位为0，则返回0
}
uint16_t HC05_GetRxData(void)
{
    return HC05_RxData;   // ← 不再去读 USART_ReceiveData
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        HC05_RxData = USART_ReceiveData(USART1);  // 读数据，同时清 RXNE
        HC05_RxFlag = 1;                          // 置接收标志
    }
}
