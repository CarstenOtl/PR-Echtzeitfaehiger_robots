/* EGR */
#include <hw_types.h>

int main() {
	//steckplatz 1
	int Taster1Mux = 0x44e10000+0x8b8 ;
	int Taster2Mux = 0x44e10000+0x8b0 ;
	int Taster3Mux = 0x44e10000+0x8a8 ;

	//Steckplatz 3
	int LED1Mux = 0x44e10000+0x8d8 ;
	int LED2Mux = 0x44e10000+0x8d4 ;
	int LED3Mux = 0x44e10000+0x8d0 ;
 	int LED4Mux = 0x44e10000+0x8c0 ;

	int GPIO_0 = 0x44e07000 ;
	int GPIO_2 = 0x481ac000 ;

	int EO = 0x134 ;
	int Dataout = 0x13c ;
	int Datain = 0x138 ;

	//MUXING
	//LED
	HWREG(LED1Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(LED1Mux) |= 0x7 ;

	HWREG(LED2Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(LED2Mux) |= 0x7 ;

	HWREG(LED3Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(LED3Mux) |= 0x7 ;

	HWREG(LED4Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(LED4Mux) |= 0x7 ;

	//Taster
	HWREG(Taster1Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(Taster1Mux) |= 0x7 ;

	HWREG(Taster2Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(Taster2Mux) |= 0x7 ;

	HWREG(Taster3Mux) &= ~((1<<0) | (1<<1) | (1<<2)) ;
	HWREG(Taster3Mux) |= 0x7 ;

	//Pullup
	HWREG(Taster1Mux) &= ~((1<<3) | (1<<4));
	HWREG(Taster1Mux) |= (1<<4) ;

	HWREG(Taster2Mux) &= ~((1<<3) | (1<<4));
	HWREG(Taster2Mux) |= (1<<4) ;

	HWREG(Taster3Mux) &= ~((1<<3) | (1<<4));
	HWREG(Taster3Mux) |= (1<<4) ;

	//Taster Als Input
	HWREG(GPIO_2 + EO) |= (1<<12) ;
	HWREG(GPIO_2 + EO) |= (1<<10) ;
	HWREG(GPIO_2 + EO) |= (1<<8) ;

	//LED Pin als output schalten
	HWREG(GPIO_0 + EO) &= ~(1<<10) ;
	HWREG(GPIO_0 + EO) &= ~(1<<9) ;
	HWREG(GPIO_0 + EO) &= ~(1<<8) ;
	HWREG(GPIO_0 + EO) &= ~(1<<14) ;

	//LED abschalten (PIN anschalten)
	HWREG(GPIO_0 + Dataout) |= (1<<10) ;
	HWREG(GPIO_0 + Dataout) |= (1<<9) ;
	HWREG(GPIO_0 + Dataout) |= (1<<8) ;
	HWREG(GPIO_0 + Dataout) |= (1<<14) ;

	while(1){

		//ueberpruefe Taster1
		if ((HWREG(GPIO_2 + Datain) & (1<<12)) == 0){
			//LED anschalten (Pin abschalten)
			HWREG(GPIO_0 + Dataout) &= ~(1<<10) ;
		} else {
			HWREG(GPIO_0 + Dataout) |= (1<<10) ;
		}

		//ueberpruefe Taster2
		if ((HWREG(GPIO_2 + Datain) & (1<<10)) == 0){
		//LED anschalten (Pin abschalten)
		HWREG(GPIO_0 + Dataout) &= ~(1<<9) ;
		} else {
		HWREG(GPIO_0 + Dataout) |= (1<<9) ;
		}

		//ueberpruefe Taster3
		if ((HWREG(GPIO_2 + Datain) & (1<<8)) == 0){
		//LED anschalten (Pin abschalten)
		HWREG(GPIO_0 + Dataout) &= ~(1<<8) ;
		} else {
		HWREG(GPIO_0 + Dataout) |= (1<<8) ;
		}

	}
	return 0 ;
}
