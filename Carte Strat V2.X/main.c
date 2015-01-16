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
    delay_ms(500);
    init_clock();

    TRISCbits.TRISC5 = 0; //led en sortie


    /**************************************************************************/
    /******************* Init variables olables *******************************/
    /**************************************************************************/

    OVERFLOW_CODEUR[CODEUR_D] = PAS_D_OVERFLOW_CODEUR;
    OVERFLOW_CODEUR[CODEUR_G] = PAS_D_OVERFLOW_CODEUR;

    position[CODEUR_D].ancien = 0;
    position[CODEUR_G].ancien = 0;
    position[CODEUR_D].nouvelle = 0;
    position[CODEUR_G].nouvelle = 0;

    init_position_robot (0, 0, 0);

    



    /**************************************************************************/
    /*********************** Lancement des inits ******************************/
    /**************************************************************************/

    config_timer_10ms();
    config_timer_5ms();
    config_timer_90s();

    ConfigMapping ();
    ConfigPorts ();
    ConfigQEI ();
    ConfigInterrupt ();

    InitUART(UART_XBEE, 115200);
    PutsUART(UART_XBEE, "UART configure \n\r");

    ConfigPWM();
    PutsUART(UART_XBEE, "PWM configure \n\r");

   /* envoit_pwm(MOTEUR_DROIT, 0);
    envoit_pwm(MOTEUR_GAUCHE, 0);

    delay_ms(270);
    envoit_pwm(MOTEUR_DROIT, 2046 * 3,3 / 10.8);
    envoit_pwm(MOTEUR_GAUCHE, 2046 * 3,3 / 10,8);

    delay_ms(40);

    envoit_pwm (MOTEUR_DROIT, 1800);
    envoit_pwm (MOTEUR_GAUCHE, 1800);

    delay_ms(5000);

    envoit_pwm (MOTEUR_DROIT, 0);
    envoit_pwm (MOTEUR_GAUCHE, 0);*/

    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/


 
    //TIMER_10ms = ACTIVE;
    TIMER_5ms = ACTIVE;
    //TIMER_90s = ACTIVE;
    //TIMER_DEBUG = ACTIVE;

    PutsUART(UART_XBEE, "TIMER active \n\r");
    delay_ms(500);

    delay_ms(300);
    

    PutsUART(UART_XBEE, "\n\n\rasserv active \n\n\n\r");
    delay_ms(500);

    TIMER_DEBUG = ACTIVE;

    init_flag();


    //rejoindre(-1000, 0);

    /*cibler(500, 0);
    rejoindre(500, 0);
    cibler(500, 500);

    rejoindre (500, 500);
    cibler (0, 500);
    rejoindre (0, 500);
    cibler (0, 0);
    rejoindre(0, 0);
    cibler(250, 0);*/



    //orienter(180, 100);
    //orienter(180);
    //orienter (-90);
    //orienter(0);

    avancer_reculer(1500, 100);
    orienter(90, 100);
    avancer_reculer(500, 100);
    orienter(180, 100);
    avancer_reculer (1000, 100);
    orienter(-90, 100);
    avancer_reculer (500, 100);
    orienter(1, 100);
    avancer_reculer(-500, 100);

    //orienter (90, 100);
    //orienter (180, 100);
   // orienter (-10, 100);
    //orienter (0, 100);

    //avancer_reculer (-100, 100);
    //avancer_reculer(100);

    //delay_ms(500);

   /* cibler(0, -500);
    cibler(0, 500);
    cibler(0, -500);
    cibler(0, 500);
    cibler(0, -500);*/


    while(1);

    return (EXIT_SUCCESS);
}

