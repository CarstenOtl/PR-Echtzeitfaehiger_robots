/* EGR */

#include <hw_types.h>
#include <uartStdio.h>
#include <interrupt.h>
#include <soc_AM335x.h>
#include <hw_gpio_v2.h>
#include "EGR_Cape.h"
#include "delay_ms.h"
#include "Conf_mod.h"
#include "GPIO.h"
#include "EGR_DCMotor.h"

#define EHRPWM_DO_NOTHING 0x0
#define EHRPWM_SET_OUTPUT_LOW 0x1
#define EHRPWM_SET_OUTPUT_HIGH 0x2
#define EHRPWM_OUTPUT_TOGGLE 0x3

volatile int DutyCycle = 0 ;


void myGPIOI1sr(){
//	UARTprintf("1\n") ;
//	if(DutyCylce <= 100){
//		DutyCycle += 10 ;
//		if(DutyCycle< 0){
//
//		} else {
//
//		}
//	}

	//interrupt loeschen
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(0)) |= (1<<GPIO_PORT1_PIN2) ;
}

void myGPIOI2sr(){
//	UARTprintf("2\n") ;


	//interrupt loeschen
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS(1)) |= (1<<GPIO_PORT1_PIN4) ;
}

int main() {

	//Taster & Pullup
	PinMuxing(CONF_PORT1_PIN2, PULL_ENABLE, PULL_UP, 7) ;
	PinMuxing(CONF_PORT1_PIN4, PULL_ENABLE, PULL_UP, 7) ;

	//Taster Als Input
	EGR_GPIODirSet(GPIO_PORT1_PIN2_MODUL, GPIO_PORT1_PIN2, 1) ;
	EGR_GPIODirSet(GPIO_PORT1_PIN4_MODUL, GPIO_PORT1_PIN4, 1) ;

	//HBruecken
	PinMuxing(CONF_H_BRUECKE_MOTOR1_1, PULL_DISABLE, 0, 6) ;
	PinMuxing(CONF_H_BRUECKE_MOTOR1_2, PULL_DISABLE, 0, 6) ;
	PinMuxing(CONF_H_BRUECKE_MOTOR2_1, PULL_DISABLE, 0, 4) ;
	PinMuxing(CONF_H_BRUECKE_MOTOR2_2, PULL_DISABLE, 0, 4) ;

	//initialise motor
	EHRPWMinitForDCMotor() ;
	//set PWM
	EHRPWMsetDutyCycle (SOC_EPWM_1_REGS, 100) ;
	EHRPWMsetDutyCycle (SOC_EPWM_2_REGS, 100) ;
	configEHRPWM_A (SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_B (SOC_EPWM_1_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_A (SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_HIGH, EHRPWM_SET_OUTPUT_LOW);
	configEHRPWM_B (SOC_EPWM_2_REGS, EHRPWM_SET_OUTPUT_LOW, EHRPWM_SET_OUTPUT_LOW);


	//interrupt
	//initialise interrupt
	IntAINTCInit() ;

	//global interrupt steuerung
	IntMasterIRQEnable() ;

	//lokale interrupt quelle aktivieren
	IntSystemEnable(SYS_INT_TINT2) ;
	IntSystemEnable(SYS_INT_GPIOINT2A) ;
	IntSystemEnable(SYS_INT_GPIOINT2B) ;

	//interrupt function
//	IntRegister(SYS_INT_TINT2, myTimer2Isr) ;
	IntRegister(SYS_INT_GPIOINT2A, myGPIOI1sr) ; //pin2
	IntRegister(SYS_INT_GPIOINT2B, myGPIOI2sr) ; //pin4
	IntPrioritySet(SYS_INT_GPIOINT2A, 0, AINTC_HOSTINT_ROUTE_IRQ);
	IntPrioritySet(SYS_INT_GPIOINT2B, 0, AINTC_HOSTINT_ROUTE_IRQ);

	//input trigger
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(0)) |= (1<<GPIO_PORT1_PIN2) ;
	HWREG(SOC_GPIO_2_REGS + GPIO_FALLINGDETECT) |= (1<<GPIO_PORT1_PIN2) ;

	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET(1)) |= (1<<GPIO_PORT1_PIN4) ;
	HWREG(SOC_GPIO_2_REGS + GPIO_FALLINGDETECT) |= (1<<GPIO_PORT1_PIN4) ;

	while (1)
	{
		/* Place main loop code here */
//
//		EGR_PinWrite(GPIO_H_BRUECKE_MOTOR1_1_MODUL, GPIO_H_BRUECKE_MOTOR1_1, hBruecke1_1) ;
//		EGR_PinWrite(GPIO_H_BRUECKE_MOTOR1_2_MODUL, GPIO_H_BRUECKE_MOTOR1_2, hBruecke1_2) ;
//		EGR_PinWrite(GPIO_H_BRUECKE_MOTOR2_1_MODUL, GPIO_H_BRUECKE_MOTOR2_1, hBruecke2_1) ;
//		EGR_PinWrite(GPIO_H_BRUECKE_MOTOR2_2_MODUL, GPIO_H_BRUECKE_MOTOR2_2, hBruecke2_2) ;
	}
	return 0;
}
