/* EGR */

#include <hw_types.h>
#include "GPIO.h"
#include "Conf_mod.h"

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

	while (1)
	{
		/* Place main loop code here */
		//ueberpruefe Taster1

		//taster1 ueberpruefen
		if (EGR_PinRead(GPIO_2, 12)){
			EGR_PinWrite(GPIO_0, 10, 1);
		} else {
			EGR_PinWrite(GPIO_0, 10, 0) ;
		}

		//taster2 ueberpruefen
		if (EGR_PinRead(GPIO_2, 10)){
			EGR_PinWrite(GPIO_0, 9, 1) ;
		} else {
			EGR_PinWrite(GPIO_0, 9, 0) ;
		}

		//taster3 ueberpruefen
		if (EGR_PinRead(GPIO_2, 8)){
			EGR_PinWrite(GPIO_0, 8, 1);
		} else {
			EGR_PinWrite(GPIO_0, 8, 0) ;
		}

	}
	return 0;
}
