#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//#define LTC2500_RDLB_LOW 			GPIO_ResetBits(GPIOB,GPIO_Pin_9)
//#define LTC2500_RDLB_HIGH		 	GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define DummyByte	((u16)(0xffff)) 
 				  	    													  
void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u8 SPI2_ReadByte(void);//SPI总线读写一个字节

unsigned int LTC2500Readsg(unsigned short TxData);

u16 SPI2_ReadWrite2Byte(u16 TxData);
unsigned int LTC2500Read(void);
unsigned int GetLTC2500Value(void);
unsigned int GetLTC2500Valuesg(void);
unsigned int LTC2500_Average(u32 times);
unsigned int LTC2500Read32(void);
//u16 SPI2_ReadWriteByte(u16 TxData);//SPI总线读写两个字节	
//s32 Get_24BitADc(void);


#endif

