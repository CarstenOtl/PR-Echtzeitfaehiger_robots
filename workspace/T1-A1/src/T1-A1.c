/* EGR */

#include <hw_types.h>

int main() {
	HWREG(0x44e10000+0x8b8) &=~ ((1<<0)|(1<<1)|(1<<2)); //löschen des vorherigen Modes

	HWREG(0x44e10000+0x8b8) |= 0x7; //setzen des neuen Modes

	//LED Pin als Output schalten
	HWREG(0x481ac000 + 0x134) &= ~(1 << 12);

	//LED Pin als Output schalten
	HWREG(0x481ac000 + 0x134) &= ~(1 << 12);


	while (1)
	{
		/* Place main loop code here */
	}
	return 0;
}
