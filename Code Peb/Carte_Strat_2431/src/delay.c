/*
 *	Delay functions
 *	See delay.h for details
 *
 *	Make sure this code is compiled with full optimization!!!
 */

#include	"delay.h"

/*
void DelayMs(unsigned char cnt)
{
	unsigned char i;
	while (cnt--) {
		i=4;
		while(i--) {
			DelayUs(uS_CNT);	// Adjust for error
		} ;
	} ;
}

void DelayMms(unsigned char cnt)
{
	int i;
	
	for (i=0; i<cnt; i++)
	{
		DelayMs(200);
		DelayMs(250);
	}
}
*/

