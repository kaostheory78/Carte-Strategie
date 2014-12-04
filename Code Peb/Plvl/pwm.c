/** \file pwm.c
 *  \brief Implementation for the PWM module.
 */

#include "usr.h"

// global init
void pwm_invaders_init(void)
{
	P1TCONbits.PTMOD = 0;//free running mode !
	P1TCONbits.PTOPS = 0;//postscaler 1:1
	/*P1TCONbits.PTCKPS = PRESCBITS;*///prescaler
	
	/*P1TPER = PERIOD;*/
	
	/*PWM1CON1=0b0000001100110000;*/ //output PWM1H et 2H

	/* Configure tristates as outputs */
	OUT_LMD18200_1_DIR = LEFT_PWM_MOVE_FORWARD;
	OUT_LMD18200_2_DIR = RIGHT_PWM_MOVE_FORWARD;

	P1DC1 = P1DC2 = 0;

	P1TCONbits.PTEN=1; //time base Enable
}
/***********************************************************/


/***********************************************************/

/* global SET FUNCTION : we use a (void *) to be compliant with
 * control_system functions. For instance pwm_set((void *)(PWM1B_NUM), x) 
 * is equivalent to pwm_set_1B(x) */
void pwm_invaders_set( void * data, int32_t value )
{
	const int16_t num = (int16_t) data;

	// Adjust value to the PWM direction
	if( num == (int16_t) LEFT_PWM )
	{
		uint16_t val = (uint16_t) abs( value );
		val = (int16_t) ((double) LEFT_PWM_GAIN * (double) val);
		val = (val > LEFT_PWM_MAX_VALUE) ? LEFT_PWM_MAX_VALUE : val;
		OUT_LMD18200_1_DIR = (value < 0 ) ? LEFT_PWM_MOVE_BACKWARD : LEFT_PWM_MOVE_FORWARD;
		//UART_printf( "Set left PWM with command 0x%x\n\r", val );
		P1DC1 = val;
	}
	else //if( num == (int16_t) RIGHT_PWM )
	{
		uint16_t val = (uint16_t) abs( value );
		val = (int16_t) ((double) RIGHT_PWM_GAIN * (double) val);
		val = (val > RIGHT_PWM_MAX_VALUE) ? RIGHT_PWM_MAX_VALUE : val;
		OUT_LMD18200_2_DIR = (value < 0 ) ? RIGHT_PWM_MOVE_BACKWARD : RIGHT_PWM_MOVE_FORWARD;
		//UART_printf( "Set right PWM with command 0x%x\n\r", val );
		P1DC2 = val;
	}
}


int32_t pwm_invaders_get( void* data)
{
	int32_t val;
	int8_t sens;
	const int16_t num = (int16_t) data;

	if( num == (int16_t) LEFT_PWM )
	{
		val = (int32_t) P1DC1;
		sens = OUT_LMD18200_1_DIR;
		if( sens == LEFT_PWM_MOVE_BACKWARD )
			val = -val;
			
	}
	else //if( num == (int16_t) RIGHT_PWM )
	{
		val = (int32_t) P1DC2;
		sens = OUT_LMD18200_2_DIR;
		if( sens == RIGHT_PWM_MOVE_BACKWARD )
			val = -val;
	}

	return val;
}
