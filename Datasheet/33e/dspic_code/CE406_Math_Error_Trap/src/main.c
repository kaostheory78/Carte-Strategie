/**********************************************************************
* © 2006 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Exxxx/PIC24Exxxx
* Compiler:        MPLAB® C30 v3.30 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* RK               	03/22/06  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Srikar D			04/27/11  Updated for dsPIC33E / PIC24E
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with dsPIC33EP512MU810 controller
* The Processor starts with the Internal oscillator without PLL enabled and then the Clock is switched to PLL Mode.
**********************************************************************/

#if defined(__dsPIC33E__)
#include "p33exxxx.h"
#elif defined(__PIC24E__)
#include "p24exxxx.h"
#endif




// Select the Source that will cause Math Error
#define ACCSHT_ERR  0
#define DIV0_ERR    1
#define ACCA_OV     0
#define ACCB_OV     0
#define ACCA_COV    0
#define ACCB_COV    0



//Macros for Configuration Fuse Registers:
//Invoke macros to set up  device configuration fuse registers.
//The fuses will select the oscillator source, power-up timers, watch-dog
//timers etc. The macros are defined within the device
//header files. The configuration fuse registers reside in Flash memory.


// Internal FRC Oscillator
_FOSCSEL(FNOSC_FRC);		// FRC Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_XT); 
								// Clock Switching is enabled and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: Disabled


_FWDT(FWDTEN_OFF);              // Watchdog Timer Enabled/disabled by user software
												
												

int main (void)
{
// Configure Oscillator to operate the device at 40Mhz
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// Fosc= 8M*40/(2*2)=80Mhz for 8M input clock
	PLLFBD=38;					// M=40
	CLKDIVbits.PLLPOST=0;		// N1=2
	CLKDIVbits.PLLPRE=0;		// N2=2
	OSCTUN=0;					// Tune FRC oscillator, if FRC is used

// Disable Watch Dog Timer
	RCONbits.SWDTEN=0;

// Clock switch to incorporate PLL
	__builtin_write_OSCCONH(0x03);		// Initiate Clock Switch to Primary
													// Oscillator with PLL (NOSC=0b011)
	__builtin_write_OSCCONL(0x01);		// Start clock switching
	while (OSCCONbits.COSC != 0b011);	// Wait for Clock switch to occur

// Wait for PLL to lock
	while(OSCCONbits.LOCK!=1) {};


//================================================
// Create Acc Shift error
#if (ACCSHT_ERR ) 
    createAccShtError();          // Create Divide by 0 Error
#endif



//================================================
// Create DIVIDE BY 0 Error
#if (DIV0_ERR) 
    createDiv0Error();          // Create Divide by 0 Error
#endif

//================================================
// Create ACCA Overflow Trap condition
#if (ACCA_OV)
    INTCON1bits.OVATE = 1;      // Enable Accumulator A Overflow traps 
    createAccaOflow();          // Create ACCA Overflow
#endif

//================================================
// Create ACCB Overflow Trap condition
#if (ACCB_OV)
    INTCON1bits.OVBTE = 1;  	// Enable Accumulator B Overflow traps 
    createAccbOflow();          // Create ACCB Overflow
#endif

//================================================
// Create ACCA Catastrophic Overflow Trap condition
#if (ACCA_COV)
    INTCON1bits.COVTE = 1;  	// Enable Catastrophic (i.e., permanent loss of sign bit)  
    createAccaCoflow();         // Create ACCA Overflow
#endif

//================================================
// Create ACCB Catastrophic Overflow Trap condition
#if (ACCB_COV)
        INTCON1bits.COVTE = 1;  	// Enable Catastrophic (i.e., permanent loss of sign bit) 
        createAccbCoflow();        // Create ACCB Overflow
#endif



        while (1);              		

}


/*
EOF
*/


