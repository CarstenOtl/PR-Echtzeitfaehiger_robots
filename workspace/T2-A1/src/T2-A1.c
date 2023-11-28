/* EGR */
#include <interrupt.h>
#include <soc_AM335x.h>
#include <hw_gpio_v2.h>
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
	EGR_GPIODirSet(GPIO_2, 14, 0) ;

	//LEDs ausschalten
	EGR_PinWrite(GPIO_0, 10, 1) ;
	EGR_PinWrite(GPIO_0, 9, 1) ;
	EGR_PinWrite(GPIO_0, 8, 1) ;
	EGR_PinWrite(GPIO_2, 14, 1) ;

	//initialise interrupt
	IntAINTCInit() ;

	//global interrupt steuerung
	IntMasterIRQEnable() ;

	//lokale interrupt quelle aktivieren
	IntSystemEnable(SYS_INT_GPIOINT2A) ;

	volatile int notaus = 0 ;
	//interrupt function
	void myGPIO2Isr(loopActive){
		// LEDS ausschalten
		EGR_PinWrite(GPIO_0, 10, 1) ;
		EGR_PinWrite(GPIO_0, 9, 1) ;
		EGR_PinWrite(GPIO_0, 8, 1) ;
		EGR_PinWrite(GPIO_2, 14, 1) ;
		notaus = 1 ;
//		notaus = 0 ;/

		//Loeschen des Interrupts
		HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1<<8) ;
	}

	//Register auf GPIO interrupt einstellen
	IntRegister(SYS_INT_GPIOINT2A, myGPIO2Isr) ;

	//input trigger
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(0)) |= (1<<8) ;
	HWREG(SOC_GPIO_2_REGS + GPIO_FALLINGDETECT) |= (1<<8) ;



	volatile int loopActive = 0 ;
//	int loopActive = 0 ;
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
		EGR_PinWrite(GPIO_2, 14, 0) ;
		toggleButtons() ;

		delay_ms(speedSetting) ;
		EGR_PinWrite(GPIO_2, 14, 1) ;
	}



	while (!notaus)
	{

		toggleButtons() ;

		int i = 1 ;
		UARTprintf("i = %d\n", i);

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
