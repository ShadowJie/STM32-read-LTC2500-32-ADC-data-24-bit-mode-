This test is based on STM32F103 and LTC2500-32.
Warning:
	LTC2500-32 is working on 24 bit mode, so you need to use SCKB SDOB BUSY pins to connect to stm32. I use PA8 pin to output the MCLK. The pin IN- of LTC2500 connect with GND, so it can't recognize the negative voltage and you can add ltc6363 to input bipolar voltage. The stm32 and ltc2500 should be placed on one PCB board(don't use jump line,it will produce a lot of noise interference)
	If you have any other good idea, you can use this email to contact me:	maopijie@outlook.com
	
	Vref is 5V.
	MCLK is PA8, freq is 100KHz.(you can change)
	Use serial to output the data(bound = 115200)
	exti_line pin is PA0
	LED is PA1
	Project is in the use file.
