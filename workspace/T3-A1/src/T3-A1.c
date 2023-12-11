/* EGR */

#include <hw_types.h>
#include <uartStdio.h>
#include <interrupt.h>
#include <soc_AM335x.h>
#include <hw_gpio_v2.h>
#include "EGR_Cape.h"
#include "Conf_mod.h"
#include "GPIO.h"
#include <hw_tsc_adc_ss.h>

int main() {

	HWREG(SOC_CM_WKUP_REGS + (0xbc)) &= ~(0b11); //Bit 0 und 1 löschen
	HWREG(SOC_CM_WKUP_REGS + (0xbc)) |= (1<<1); //Bit 1 setzen


	//Schreibschutz der ADC-TSC Konfiguration deaktivieren
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_CTRL) |= (1 << 2);
	//Bitnummer des -SchreibschutzBit- aus dem Datenblatt suchen


	//STEP aktivieren
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPENABLE) |= ((1 << 1)|(1<<2));

	//step einstellen
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) &=~ ((1 << 0)|(1<<1)|(1 << 3)|(1<<4)|(1 << 12)|(1<<13)|(1 << 14)|(1<<19)|(1<<20)|(1 << 21)|(1<<22)|(1 << 26));
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) &=~ ((1 << 0)|(1<<1)|(1 << 3)|(1<<4)|(1 << 12)|(1<<13)|(1 << 14)|(1<<19)|(1<<20)|(1 << 21)|(1<<22)|(1 << 26));

	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(0)) |= ((1 << 0)|(4<<2)|(1<<19));
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= ((1 << 0)|(4<<2)|(1<<19));

	//enable module
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_CTRL) |= (1 << 0);

	unsigned int meinAdcWert;

	while (1)
	{

		//Aktuellen FIFO-Wert in eigene Variable schreiben
		meinAdcWert = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(0));
		UARTprintf("Sensorwert: %d\n",meinAdcWert);
	}
	return 0;
}
