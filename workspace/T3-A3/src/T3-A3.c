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
#include "EGR_DCMotor.h"

#define EHRPWM_DO_NOTHING 0x0
#define EHRPWM_SET_OUTPUT_LOW 0x1
#define EHRPWM_SET_OUTPUT_HIGH 0x2
#define EHRPWM_OUTPUT_TOGGLE 0x3


void left_forwards(){
	configEHRPWM_A (SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_B (SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
}

void right_forwards(){
	configEHRPWM_A (SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_B (SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
}

void left_backwards(){
	configEHRPWM_A (SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_B (SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
}

void right_backwards(){
	configEHRPWM_A (SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_B (SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
}


int main() {

	//HBruecken
	PinMuxing(CONF_H_BRUECKE_MOTOR1_1, PULL_DISABLE, 0, 6) ;
	PinMuxing(CONF_H_BRUECKE_MOTOR1_2, PULL_DISABLE, 0, 6) ;
	PinMuxing(CONF_H_BRUECKE_MOTOR2_1, PULL_DISABLE, 0, 4) ;
	PinMuxing(CONF_H_BRUECKE_MOTOR2_2, PULL_DISABLE, 0, 4) ;

	//initialise motor
	EHRPWMinitForDCMotor() ;

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
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_STEPCONFIG(1)) |= ((1 << 0)|(4<<2)|(3<<19)|(1<<26));

	//enable module
	HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_CTRL) |= (1 << 0);

	unsigned int meinAdcWert0;
	unsigned int meinAdcWert1;
	int speedRight = 0;
	int speedLeft = 0;

	while (1)
	{

		//Aktuellen FIFO-Wert in eigene Variable schreiben
		meinAdcWert0 = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(1));
		meinAdcWert1 = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(0));
		UARTprintf("Sensorwert Turn: %d\n",meinAdcWert1);
		UARTprintf("Sensorwert Drive: %d\n",meinAdcWert0);

		int drive=(meinAdcWert0/20)-100;
		int turn =(meinAdcWert1/20)-100;
		 speedRight= drive-turn;
		 speedLeft= drive+turn;

		// RIGHT MOTOR
		if (speedRight>=0){
			right_backwards() ;

			if (speedRight > 100){
				speedRight = 100;
			}
			//set PWM
			EHRPWMsetDutyCycle (SOC_EPWM_1_REGS, speedRight) ;

			UARTprintf("SpeedRight: %d\n", speedRight) ;

		} else {
			right_forwards() ;
			if (-speedRight > 100){
				speedRight = -100;
			}
//			speedRight = - speedRight ;
			//set PWM
			EHRPWMsetDutyCycle (SOC_EPWM_1_REGS, -speedRight) ;

			UARTprintf("SpeedRight: %d\n", speedRight) ;

		}

		// LEFT MOTOR
		if (speedLeft>=0){
			left_backwards() ;

			if (speedLeft > 100){
				speedLeft = 100;
			}
			//set PWM

			EHRPWMsetDutyCycle (SOC_EPWM_2_REGS, speedLeft) ;
			UARTprintf("SpeedLeft: %d\n", speedLeft) ;

			} else {
			left_forwards();
			if (-speedLeft > 100){
				speedLeft = -100;
			}
			//set PWM

			EHRPWMsetDutyCycle (SOC_EPWM_2_REGS, -speedLeft) ;
			UARTprintf("SpeedLeft: %d\n", speedLeft) ;

			}
		}
	return 0;
}
