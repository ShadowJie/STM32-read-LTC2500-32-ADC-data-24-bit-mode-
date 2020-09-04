#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//SPI驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//这里针是对SPI2的初始化



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );		//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );		//SPI2时钟使能 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//PB12 NSS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//B13 SCKB   B14  MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					//PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//初始化GPIOB

	//RDLB---->LTC2500--RDLB
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//RDLB  PB8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
 	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);                        //PB13/14/15上拉  xiala	
////  Configure MOSI pin as Alternate Function Push Pull
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
//	SPI_Cmd(SPI2, DISABLE);
	SPI_Cmd(SPI2, DISABLE); //必须先禁能，才可以改变MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                      //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		                	//设置SPI的数据大小:SPI发送接收8位帧结构 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                        //串行同步时钟的空闲状态为高电平	低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                        //串行同步时钟的第二个跳变沿（上升或下降）数据被采样   1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256  4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;															//CRC值计算的多项式
	
	SPI_Init(SPI2, &SPI_InitStructure);  																	//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI2, ENABLE); 																								//使能SPI外设
	
//	SPI2_ReadWriteByte(0xff);																						//启动传输		 
//	LTC2500Readsg(0xffff);

}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 			//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); 																		//通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 		//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); 																	//返回通过SPIx最近接收的数据					    
}

//2个字节
u16 SPI2_ReadWrite2Byte(u16 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 			//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); 																		//通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 		//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); 																	//返回通过SPIx最近接收的数据					    
}

//16位模式下的读取转换 读取32位
unsigned int LTC2500Read32(void)
{
	u8 i;
	u16 Buffer[2];
	unsigned int adcvalue = 0;
	unsigned int Adcvalue = 0;
//	SPI2_ReadWriteByte(DummyByte);
	for(i=0;i<2;i++)
		{
			Buffer[i]=SPI2_ReadWrite2Byte(DummyByte);
		}
	adcvalue = (unsigned int)Buffer[0]<<16|(unsigned int)Buffer[1];
	Adcvalue = adcvalue>>7;
	Adcvalue = Adcvalue&0x00ffffff;
	return Adcvalue;
}

//8位模式下的读取转换
//unsigned int LTC2500Read32(void)
//{
//	u8 i;
//	u8 Buffer[4]={0};
//	unsigned int adcvalue = 0;
//	unsigned int Adcvalue = 0;
////	SPI2_ReadWriteByte(DummyByte);
//	for(i=0;i<4;i++)
//		{
//			Buffer[i]=SPI2_ReadWriteByte(DummyByte);
//		}
//	adcvalue = (unsigned int)Buffer[0]<<24|(unsigned int)Buffer[1]<<16|(unsigned int)Buffer[2]<<8|(unsigned int)Buffer[3];
//	Adcvalue = adcvalue>>7;
//	Adcvalue = Adcvalue&0x00ffffff;
//	return Adcvalue;
//}

unsigned int LTC2500Readsg(unsigned short TxData)
{
	unsigned short DATL;	//低16位
	unsigned short DATH;
	unsigned short retry=0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 			//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>300)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); 																		//通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 		//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>300)return 0;
		}	  						    
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	DATH = SPI_I2S_ReceiveData(SPI2);//高16位
		
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 			//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>300)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); 																		//通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 		//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>300)return 0;
		}	  						    	
		
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	DATL = SPI_I2S_ReceiveData(SPI2);//低16位
	return ( (unsigned int)DATH << 16 | (unsigned int)DATL );
}	


unsigned int GetLTC2500Valuesg(void)
{	
	unsigned int adcvalue = 0;
	unsigned int Adcvalue = 0;
	adcvalue = LTC2500Readsg(0xffff);
	Adcvalue = adcvalue>>7;
	Adcvalue = Adcvalue&0x00ffffff;
	return Adcvalue;	
	
}	

unsigned int LTC2500_Average(u32 times)
{
	unsigned int temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=LTC2500Read32();
		delay_ms(5);
	}
	return temp_val/times;
} 	 






//16bit
//u16 SPI2_ReadWriteByte(u16 TxData)
//{               
//	unsigned char retry=0;                                        
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //
//					{
//					retry++;
//					if(retry>200)return 0;
//					}                          
//	SPI_I2S_SendData(SPI2, TxData); //
//	retry=0;

//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //
//					{
//					retry++;
//					if(retry>200)return 0;
//					}
//					
//	return SPI_I2S_ReceiveData(SPI2); //                                          
//}       


































