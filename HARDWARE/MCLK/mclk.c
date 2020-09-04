#include "stm32f10x.h"
#include "mclk.h"
void freq_init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
	RCC_MCOConfig(RCC_MCO_HSE); //RCC_MCO_NoClock --- 无时钟输出
								//RCC_MCO_SYSCLK --- 输出系统时钟(SysCLK) 
								//RCC_MCO_HSI --- 输出内部告高速8MHzRC振荡器时钟(HSI) 
								//RCC_MCO_HSE --- 输出外部高速时钟(HSE) 
								//RCC_MCO_PLLCLK_Div2 --- 输出PLL倍频后的二分频时钟(PLLCLK/2)		
}



