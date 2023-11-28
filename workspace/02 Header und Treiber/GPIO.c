// Diese Datei wird nur verlinkt. D.h. ihr editiert ueber alle Projekte hinweg die selbe (diese) Datei.

/*
 * EGR_GPIO.c
 *
 *  Created on: 24.01.2014
 *      Author: Jonas Pfeiffer, Johannes Coy
 *   Copyright: TUM MiMed
 * Last Change: 28.10.2014
 */
#include "GPIO.h"

/*****************************************************************************
**                       FUNCTION DEFINITIONS
*****************************************************************************/

void EGR_GPIODirSet(unsigned int gpio_base_addr,
					unsigned int pinNumber,
					unsigned int pinDirection)
{
	//TODO: Ergaenzen Sie hier den code zum Einstellen der Pin-Funktion (Input oder Output)
	unsigned int EO = 0x134 ;

	//input: pinDir = 1
	//output: pinDir = 0

	if (pinDirection){
		HWREG(gpio_base_addr + EO) |= (1<<pinNumber);
	}else {
		HWREG(gpio_base_addr + EO) &= ~(1<<pinNumber);
	}

}

void EGR_PinWrite	(unsigned int gpio_base_addr,
				  	 unsigned int pinNumber,
					 unsigned int pinValue)
{
	//TODO: Ergaenzen Sie hier den code zum Einstellen des Pin-Ausgabe Werts (0V oder 3,3V)
	//Tipp: Verzweigung ob an- oder aus-geschaltet werden soll
	// entscheiden ob pull up oder pull down (Pullup: 1 , Pulldown: 0)
	unsigned int Dataout = 0x13c ;

	if (pinValue){
		//
		HWREG(gpio_base_addr + Dataout) |= (1<<pinNumber) ;
	}else {
		//
		HWREG(gpio_base_addr + Dataout) &= ~(1<<pinNumber) ;
	}

}


unsigned int EGR_PinRead (unsigned int gpio_base_addr,
						  unsigned int pinNumber)
{
	//TODO: Ergaenzen Sie hier den code zum Auslesen des Pin-Werts (0V oder 3,3V)
	//Auslesen des Pinwerts und Rueckgabe ueber return
	unsigned int Datain = 0x138 ;
	unsigned int low = 0 ;
	unsigned int high = 1 ;

	if (HWREG(gpio_base_addr + Datain) & (1<<pinNumber)){
		return high ;
	} else {
		return low ;
	}
}

/******************************* End Of File ********************************/
