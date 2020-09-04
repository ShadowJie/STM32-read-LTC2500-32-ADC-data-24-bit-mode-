#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
//#include "w25qxx.h"	 
#include "mclk.h"
#include "ltc2500.h" 
#include "exti.h"
#include "pwm.h" 
/************************************************
 LTC2500-32  STM32
 24位模式
 A8 PWM
 串口输出
 作者：王杰 @WangJie
 单位：CISD of HDU
 以正点原子SPI例程做基础进行编写
************************************************/
 
 				 	
//要写入到W25Q64的字符串数组
//const u8 TEXT_Buffer[]={"ELITE STM32 SPI TEST"};
//#define SIZE sizeof(TEXT_Buffer)
 int main(void)
 {	 

	u16 pwmval=100; 
//	freq_init();
	delay_init();	    	 //延时函数初始化	  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	NVIC_Configuration();
	uart_init(115200);	 	//串口初始化为115200
	
	LED_Init();		  		//初始化与LED连接的硬件接口
//	LCD_Init();			   	//初始化LCD 	
	
	TIM1_PWM_Init(719,0);//不分频。PWM频率=72000/(899+1)=80Khz /71=1MHz对应pwmval=40 /719=100KHz /143=500KHz  120  /89=800khz 80/102=700khz/119=600khz/79=900k
	//84=850K  359=200k 179=400k

	TIM_SetCompare1(TIM1,pwmval);

	LED0=0;//先点亮红灯
	
//	delay_ms(50);
	EXTI_Busy_Config();
//	delay_ms(10);
	LTC2500_Init();
	while(1)	//等待中断产生
	{
		//delay_ms(10);
	
	}
	
}


