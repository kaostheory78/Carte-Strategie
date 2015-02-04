/* 
 * File:   main.c
 * Author: Quentin
 *
 * Created on 22 octobre 2014, 23:55
 */

#include "system.h"
#include <stdlib.h>

/******************************************************************************/
/************************ Configurations Bits *********************************/
/******************************************************************************/

// DSPIC33FJ128MC804 Configuration Bit Settings

#include <xc.h>

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = OFF //ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSECMD  //CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON              // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


int main(int argc, char** argv)
{
    init_system();
    init_position_robot (0, 0, 0);

    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/

    //TIMER_DEBUG = ACTIVE;

    delay_ms(500);
    PutsUART(UART_XBEE, "\n\n\n\n\r INIT ROBOT : \n\n\r");

    //configurer_status_returning_level(TOUS_LES_AX12, TOUTES_LES_INFOS);
    allumer_LED_AX12(TOUS_LES_AX12);
    PORTCbits.RC5 = 0;
    angle_AX12(24, 512, 1023, SANS_ATTENTE);
    angle_AX12(24, 100, 1023, SANS_ATTENTE);
    angle_AX12(24, 700, 1023, SANS_ATTENTE);


  /* //cibler(500, 0, 100);
    rejoindre(500, 0, MARCHE_AVANT, 100);
   // orienter (90, 100);
    //cibler(500, 500, 100);
    rejoindre (500, 500, MARCHE_AVANT, 100);
    //orienter(180, 100);
    //cibler (0, 500, 100);
    rejoindre (0, 500,MARCHE_AVANT, 100);
    //orienter(-90, 100);
    //cibler (0, 0, 100);
    rejoindre(0, 0, MARCHE_AVANT, 100);
    //orienter(0, 100);
    //cibler(250, 0, 100);*/

    //trapèze type 8
    /*passe_part(1000, 500, 50);
    passe_part2 (1000, 0, 50, OFF);
    passe_part2 (0, 500, 50, OFF);
    passe_part2(0, 0, 50, ON);*/


    //grand demi cercle
    /*
    passe_part(200, 0, 500);
    passe_part2(700, 134 , 100, OFF);
    passe_part2 (1066, 500, 100, OFF);
    passe_part2 (1200, 1000, 100, OFF);
    passe_part2 (1066, 1500, 100, OFF);
    passe_part2 (700, 1866, 100, OFF);
    passe_part2 (200, 2000, 100, ON);*/

    //petit demi cercle
    
    /*passe_part(200, 0, 500);
    passe_part2(450, 67 , 100, OFF);
    passe_part2 (633, 250, 100, OFF);
    passe_part2 (700, 500, 100, OFF);
    passe_part2 (633, 750, 100, OFF);
    passe_part2 (450, 933, 100, OFF);
    passe_part2 (200, 1000, 100, ON);*/

   /* passe_part(450, 0, 100);
    passe_part2 (450, -300, 100, OFF);
    passe_part2 (100, -400, 100, ON);
    orienter(180, 100);*/


    /*avancer_reculer(1500, 100);
    orienter(90, 100);
    avancer_reculer(500, 100);
    orienter(180, 100);
    avancer_reculer (1000, 100);
    orienter(-90, 100);
    avancer_reculer (500, 100);
    orienter(0, 100);
    avancer_reculer(-500, 100);*/

    /*orienter(180, 100);
    orienter(170, 100);
    orienter (-170, 100);
    orienter(0, 100);*/



    //rejoindre(-500, 0, 100);
    //brake();


    while(1);

    return (EXIT_SUCCESS);
}

