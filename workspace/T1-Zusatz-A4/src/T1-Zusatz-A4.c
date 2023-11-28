/* EGR */
//#include <stdio.h>
#include <hw_types.h>
#include "delay_ms.h"
#include "Conf_mod.h"
#include "GPIO.h"

int main() {

	//GPIO_BASE_ADDRESS
	unsigned int GPIO_0 = 0x44e07000 ;
	unsigned int GPIO_2 = 0x481ac000 ;

	//steckplatz 1
	unsigned int Taster1Offset = 0x8b8 ;
	unsigned int Taster2Offset = 0x8b0 ;
	unsigned int Taster3Offset = 0x8a8 ;

	//Steckplatz 3
	unsigned int LED1Offset = 0x8d8 ;
	unsigned int LED2Offset = 0x8d4 ;
	unsigned int LED3Offset = 0x8d0 ;
	unsigned int LED4Offset = 0x8c0 ;

	//MUXING
	//LED & no pullup
	PinMuxing(LED1Offset, 0, 1, 7) ;
	PinMuxing(LED2Offset, 0, 1, 7) ;
	PinMuxing(LED3Offset, 0, 1, 7) ;
	PinMuxing(LED4Offset, 0, 1, 7) ;

	//Taster & Pullup
	PinMuxing(Taster1Offset, 0, 1, 7) ;
	PinMuxing(Taster2Offset, 0, 1, 7) ;
	PinMuxing(Taster3Offset, 0, 1, 7) ;

	//Taster Als Input
	EGR_GPIODirSet(GPIO_2, 12, 1) ;
	EGR_GPIODirSet(GPIO_2, 10, 1) ;
	EGR_GPIODirSet(GPIO_2, 8, 1) ;

	//LED als output
	EGR_GPIODirSet(GPIO_0, 10, 0) ;
	EGR_GPIODirSet(GPIO_0, 9, 0) ;
	EGR_GPIODirSet(GPIO_0, 8, 0) ;
	EGR_GPIODirSet(GPIO_0, 14, 0) ;

	//LEDs ausschalten
	EGR_PinWrite(GPIO_0, 10, 1) ;
	EGR_PinWrite(GPIO_0, 9, 1) ;
	EGR_PinWrite(GPIO_0, 8, 1) ;
	EGR_PinWrite(GPIO_0, 14, 1) ;

	int loopActive = 0 ;
	int speed = 0 ;

	void toggleButtons(){
		if (!(EGR_PinRead(GPIO_2, 12))){
			//toggle LEDS on/off
			loopActive = !loopActive ;
		}
		if (!(EGR_PinRead(GPIO_2, 10))){
			//toggle through 3 speed settings
			speed = ((speed+1) % 3) ;
		}
	}

	unsigned int toggleButtonsi(){
		unsigned int loopActive ;
		if (!(EGR_PinRead(GPIO_2, 12))){
			//toggle LEDS on/off
			loopActive = !loopActive ;
		}
		if (!(EGR_PinRead(GPIO_2, 10))){
			//toggle through 3 speed settings
			speed = ((speed+1) % 3) ;
		}
		return loopActive ;
	}

	void loop(int speedSetting){
		EGR_PinWrite(GPIO_0, 10, 0) ;
		toggleButtons() ;

		delay_ms(speedSetting) ;
		EGR_PinWrite(GPIO_0, 10, 1) ;
		EGR_PinWrite(GPIO_0, 9, 0) ;
		toggleButtons() ;

		delay_ms(speedSetting) ;
		EGR_PinWrite(GPIO_0, 9, 1) ;
		EGR_PinWrite(GPIO_0, 8, 0) ;
		toggleButtons() ;


		delay_ms(speedSetting) ;
		EGR_PinWrite(GPIO_0, 8, 1) ;
	}



	while (1)
	{

		toggleButtons() ;

		if (loopActive){
			switch (speed){
				case 0:
					loopActive = toggleButtonsi() ;
					if (!loopActive){
						break;
					}
					loop(500);
					break ;
				case 1:
					loopActive = toggleButtonsi() ;
					if (!loopActive){
						break;
					}
					loop(300) ;
					break ;
				case 2:
					loopActive = toggleButtonsi() ;
					if (!loopActive){
						break;
					}
					loop(100);
					break;
			}
//			loopActive = 0 ;
		}
	}
	return 0;
}
