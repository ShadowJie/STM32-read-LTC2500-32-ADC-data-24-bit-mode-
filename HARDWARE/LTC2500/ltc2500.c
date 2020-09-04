#include "ltc2500.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_spi.h"

#define Dummy_Byte		0xFF

void LTC2500_Init(void)
{	
//  GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 推挽 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出 上拉输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
 
//  W25QXX_CS=1;				//SPI FLASH不选中
	SPI2_Init();		   	//初始化SPI
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//设置为18M时钟,高速模式
//	W25QXX_TYPE=W25QXX_ReadID();//读取FLASH ID.  

} 

/*
//读取32位ADC值
s32 SPI_LTC2500_Read(void)
{
  s32 ADCResult=0;
	s32 temp=0, temp0=0, temp1=0,temp2=0, temp3=0;
	
//	 while(1)
//      {
//            //ADC_CS_0;
//            //delayCount(10);
//            if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))//GPIO_Mode_IPU 																上拉输入
//            {
//                  
//            }
//            else
//            {
//                   break;
//            }
//      }
	
//	temp0 = SPI2_ReadWriteByte(Dummy_Byte);      // 
//  temp1 = SPI2_ReadWriteByte(Dummy_Byte);      // 
//  temp2 = SPI2_ReadWriteByte(Dummy_Byte);      // 
//	temp3 = SPI2_ReadWriteByte(Dummy_Byte);			 //

	temp0 = SPI2_ReadByte();      // 
  temp1 = SPI2_ReadByte();      // 
  temp2 = SPI2_ReadByte();      // 
	temp3 = SPI2_ReadByte();			 //

	temp = (temp0 << 24) | (temp1 << 16) | temp2 << 8 | temp3; 																			//组合32位数据
	ADCResult=temp>>7;
	ADCResult=ADCResult&0x00ffffff;
	return ADCResult;

}

*/


//s32 Get_24BitADC(void)
//{
//	int i=0;
//	s32 adcResult=0;
//  s32 ADCResult=0;
//	
//	while(1)
//		{
//					//ADC_CS_0;
//					//delayCount(10);
//					if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))//GPIO_Mode_IPU 上拉输入
//					{
//								
//					}
//					else
//					{
//								 break;
//					}
//		}
//	
////	for(i=0;i<2;i++)
////	{	
//		SPI2_ReadWriteByte(0xFF);
//		adcResult+=SPI_I2S_ReceiveData(SPI2)<<16;
////	}
//	ADCResult=adcResult>>7;	//右移7位,移除CB无用码
//  ADCResult&=0x00ffffff;	//高八位置0（溢出位被置0），得到 ADCResult 24位DB有效值
//	
//	return ADCResult;
//}



