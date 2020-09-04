#include "exti.h"
#include "led.h"
//#include "key.h"
#include "delay.h"
#include "usart.h"
//#include "beep.h"
#include "ltc2500.h"
#include "lcd.h"
#include "spi.h"
#include "filter.h"
//#include "stm32f10x_exti.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序

/**
  * @brief  NVIC配置
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;               
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //配置优先级分组
	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;          //按键中断源
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能中断寄存器

    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  EXTI BUSY中断配置
  * @param  无
  * @retval 无
  */
void EXTI_Busy_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
	
    /* 调用函数配置NVIC */
    NVIC_Configuration();
	
    /* 初始化GPIO */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//BUSY引脚 A0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  								//浮空输入 GPIO_Mode_IN_FLOATING    上拉输入 GPIO_Mode_IPU
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /* 初始化EXTI */
    // 配置中断线的输入源
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;            			// 配置中断线EXTI0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  				// 配置为中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;			//下降沿触发中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;             			// 使能中断
    EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  EXTI0线中断服务函数
  * @param  无 
  * @retval 无
  */
void EXTI0_IRQHandler(void)
{
		unsigned int ADCx;
		double temp;
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) // 确保产生了EXTI0线中断 
    {
//				ADCx =  filter10();
			
//				ADCx = LTC2500_Average(50);
//				ADCx=GetLTC2500Valuesg();
				ADCx=LTC2500Read32();
				temp=(double)ADCx*9.9782/(16777216-1);// 4.9883/16777216;//Vref = 5v    5.0/16777216      备用 5.0/16777215 = 0.00000029802324164052257779375182352971    2^24 = 16777216
	
				printf("%d %.9f\r\n",ADCx,temp);//1.0195V 4.9832v 4.8953
			
//				LCD_ShowxNum(124,130,ADCx,8,16,0);//显示ADC的值  8位
//				temp=(double)ADCx*4.9883/16777216;//Vref = 5v    5.0/16777216      备用 5.0/16777215 = 0.00000029802324164052257779375182352971    2^24 = 16777216
//				ADCx=temp;
//				LCD_ShowxNum(124,150,ADCx,1,16,0);//显示电压值整数部分
//				temp-=ADCx;
//				temp*=1000000;//小数点后6位
//				LCD_ShowxNum(139,150,temp,6,16,0X80);//显示电压值小数部分
        EXTI_ClearITPendingBit(EXTI_Line0);// 清除中断标志位
    }
}








/*
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟



   //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOA.0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(WK_UP==1)	 	 //WK_UP按键
	{				 
		BEEP=!BEEP;	
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}
 

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 
		LED1=!LED1;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0
	{
		LED0=!LED0;
		LED1=!LED1; 
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
*/ 
