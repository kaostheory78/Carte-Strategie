/******************************************************************************/
/* PLVL2 Software - dsPIC33EP256MU810                                         */
/* http://pebdev.eu                                                           */
/* http://sussusinvaders.fr                                                   */
/*                                                                            */
/* Author   : PEB                                                             */
/* Revision : 1.0                                                             */
/* Date     : 29.05.2012                                                      */
/******************************************************************************/


/*** I N C L U D E S **********************************************************/
#include "includes.h"



/*** F U N C T I O N S ********************************************************/
void configOscillator (void)
{
    /* 1- Prescaler configuration  (10MHz/N1   => 2MHz)                       */
    CLKDIVbits.PLLPRE	= 3;                /* N1=5 : PLLPRE = (N1 - 2)       */

    /* 2- PLL divisor              (2MHz*M     => 256MHz)                     */
    PLLFBDbits.PLLDIV	= 128;              /* M=128 : PLLDIV = (M - 2)       */

    /* 3- Postscaler configuration (256MHz/N2  => 128MHz)                     */
    CLKDIVbits.PLLPOST	= 0;                /* N2=2 : PLLPOST = (N2/2 - 1)    */
    
    /* Disable Watch Dog Timer                                                */
    RCONbits.SWDTEN     = 0;

    /* When clock switch occurs switch to Primary Osc (HS, XT, EC)            */
    __builtin_write_OSCCONH(0x03);          /* Set OSCCONH for clock switch   */
    __builtin_write_OSCCONL(OSCCON|0x01);   /* Start clock switching          */
    while(OSCCONbits.COSC != 0b011);        /* Wait for Clock switch to occur */
    
    /* Wait for PLL to lock, only if PLL is needed                            */
    while(OSCCONbits.LOCK != 1);
}


void configBoard (void)
{
    /* Configuring basic ports                                                */
    configPorts ();
    
    /* List of all board into board.h                                         */
    /* 1-6 : PLVL2                                                            */
    if (_RD1 == 0)
    {
        /* _RD1 isn't connected on the PLVL2 board                            */
        config.system.board         = (_RD5<<2)+(_RD13<<1)+_RD3;
        config.system.isMotorBoard  = NO;
    }
    /* 7-8 : MOTORS                                                           */
    else
    {
        config.system.board         = (_RD1<<3)+(_RD5<<2)+(_RD13<<1)+_RD3;
        config.system.isMotorBoard  = YES;
    }

    /* Configuring board                                                      */
    switch (config.system.board)
    {
    case ID_BOARD_1:
        config_usrBoard1 ();
        config_usrAversiveBoard1 ();
        configPortsForPlvlBoard ();
        break;

    case ID_BOARD_2:
        config_usrBoard2 ();
        config_usrAversiveBoard2 ();
        configPortsForPlvlBoard ();
        break;

    case ID_BOARD_3:
        config_usrBoard3 ();
        config_usrAversiveBoard3 ();
        configPortsForPlvlBoard ();
        break;

    case ID_BOARD_4:
        config_usrBoard4 ();
        config_usrAversiveBoard4 ();
        configPortsForPlvlBoard ();
        break;

    case ID_BOARD_5:
        config_usrBoard5 ();
        config_usrAversiveBoard5 ();
        configPortsForPlvlBoard ();
        break;

    case ID_BOARD_6:
        config_usrBoard6 ();
        config_usrAversiveBoard6 ();
        configPortsForPlvlBoard ();
        break;

    case ID_BOARD_8:
        config_usrBoardMotors1 ();
        configPortsForMotorsBoard ();
        break;

    case ID_BOARD_9:
        config_usrBoardMotors2 ();
        configPortsForMotorsBoard ();
        break;
        
    default:
        _TRACE_HL("NO VALID ID!\n");
        break;
    }
}

void configDisplayBoardInfo (void)
{
    if (config.system.isMotorBoard == NO)
    {
        printfn (" ------------------------------------\n");
        printfn (" PLVL2 - BOARD %d  [ROBOT %d]\n", config.system.board, config.system.robotId);
        printfn (" ------------------------------------\n");
    }
    else
    {
        printfn ("  ------------------------------------\n");
        printfn (" MOTORS - BOARD %d  [ROBOT %d]\n", (config.system.board-ID_BOARD_8+1), config.system.robotId);
        printfn ("  ------------------------------------\n");
    }
}

void configRemapping (void)
{
    /* UART 1 Remapping                                                       */
    _U1RXR  = 0x20;     /* IN   : UART 1 RX  on RPI32                         */
    _RP100R = 0x01;     /* OUT  : UART 1 TX  on RP100                         */
    _U1CTSR = 0x4E;     /* IN   : UART 1 CTS on RPI78                         */
    _RP101R = 0x02;     /* OUT  : UART 1 RTS on RP101                         */

    /* UART 2 Remapping                                                       */
    _U2RXR  = 0x14;     /* IN   : UART 2 RX on RPI20                          */
    _RP79R  = 0x03;     /* OUT  : UART 2 TX on RP79                           */

    /* QEI 1 Remapping                                                        */
    _QEA1R  = 0x11;     /* IN   : QEI QEA1   on RPI17                         */
    _QEB1R  = 0x2B;     /* IN   : QEI QEB1   on RPI43                         */
    _INDX1R = 0x6D;     /* IN   : QEI INDEX1 on RP109                         */

    /* QEI 2 Remapping                                                        */
    _QEA2R  = 0x28;     /* IN   : QEI QEA2   on RPI40                         */
    _QEB2R  = 0x29;     /* IN   : QEI QEB2   on RPI41                         */
    _INDX2R = 0x2A;     /* IN   : QEI INDEX2 on RPI42                         */
}


void configPorts (void)
{    
    /* Analog Port                                                            */
    _ANSA6  = 0;
    _ANSA7  = 0;
    _ANSA9  = 0;
    _ANSA10 = 0;

    _ANSB0  = 0;
    _ANSB1  = 0;
    _ANSB2  = 0;
    _ANSB3  = 0;
    _ANSB4  = 0;
    _ANSB5  = 0;
    _ANSB6  = 0;
    _ANSB7  = 0;
    _ANSB8  = 0;
    _ANSB9  = 0;
    _ANSB10 = 0;
    _ANSB11 = 0;
    _ANSB12 = 0;
    _ANSB13 = 0;
    _ANSB14 = 0;
    _ANSB15 = 0;

    _ANSC1  = 0;
    _ANSC2  = 0;
    _ANSC3  = 0;
    _ANSC4  = 0;
    _ANSC13 = 0;
    _ANSC14 = 0;

    _ANSD6  = 0;
    _ANSD7  = 0;

    _ANSE0  = 0;
    _ANSE1  = 0;
    _ANSE2  = 0;
    _ANSE3  = 0;
    _ANSE4  = 0;
    _ANSE5  = 0;
    _ANSE6  = 0;
    _ANSE7  = 0;
    _ANSE8  = 0;
    _ANSE9  = 0;

    _ANSG6  = 0;
    _ANSG7  = 0;
    _ANSG8  = 0;
    _ANSG9  = 0;

    /* PWM output                                                             */
    IOCON1bits.PENL     = 0;                            /* Not used as PWM    */
    IOCON1bits.POLL     = 0;                            /* Not used as PWM    */
    IOCON2bits.PENL     = 0;                            /* Not used as PWM    */
    IOCON3bits.PENH     = 0;                            /* Not used as PWM    */
    IOCON3bits.PENL     = 0;                            /* Not used as PWM    */
    IOCON4bits.PENH     = 0;                            /* Not used as PWM    */
    IOCON4bits.PENL     = 0;                            /* Not used as PWM    */
    IOCON5bits.PENH     = 0;                            /* Not used as PWM    */
    IOCON5bits.PENL     = 0;                            /* Not used as PWM    */
    IOCON6bits.PENH     = 0;                            /* Not used as PWM    */
    IOCON6bits.PENL     = 0;                            /* Not used as PWM    */

    /* Open Drain                                                             */
    _ODCA0  = 0;
    _ODCA1  = 0;
    _ODCA2  = 0;
    _ODCA3  = 0;
    _ODCA4  = 0;
    _ODCA5  = 0;
    _ODCA14 = 0;
    _ODCA15 = 0;

    _ODCD0  = 0;
    _ODCD1  = 0;
    _ODCD2  = 0;
    _ODCD3  = 0;
    _ODCD4  = 0;
    _ODCD5  = 0;
    _ODCD8  = 0;
    _ODCD9  = 0;
    _ODCD10 = 0;
    _ODCD11 = 0;
    _ODCD12 = 0;
    _ODCD13 = 0;
    _ODCD14 = 0;
    _ODCD15 = 0;
    
    _ODCF0  = 0;
    _ODCF1  = 0;
    _ODCF2  = 0;
    _ODCF3  = 0;
    _ODCF4  = 0;
    _ODCF5  = 0;
    _ODCF8  = 0;
    _ODCF12 = 0;
    _ODCF13 = 0;

    _ODCG0  = 0;
    _ODCG1  = 0;
    _ODCG12 = 0;
    _ODCG13 = 0;
    _ODCG14 = 0;
    _ODCG15 = 0;
    
    /* PORT x configuration                                                   */
    /* _CNIExy  : Input Change Notification Interrupt Enable bits             */
    /* _CNPUxy  : Input Change Notification Pull-up Enable bits               */
    /* _CNPDxy  : Input Change Notification Pull-down Enable bits             */
    _TRISA1  = 1; _RA1  = 0; _CNIEA1  = 0; _CNPUA1  = 0; _CNPDA1  = 0; /* ENC1 CHA   */
    _TRISA4  = 1; _RA4  = 0; _CNIEA4  = 0; _CNPUA4  = 0; _CNPDA4  = 0; /* UART2 RX   */
    _TRISA5  = 0; _RA5  = 0; _CNIEA5  = 0; _CNPUA5  = 0; _CNPDA5  = 0; /* FREE       */
    _TRISA6  = 1; _RA6  = 0; _CNIEA6  = 0; _CNPUA6  = 0; _CNPDA6  = 0; /* VBATTERY   */
    _TRISA7  = 1; _RA7  = 0; _CNIEA7  = 0; _CNPUA7  = 0; _CNPDA7  = 0; /* LMD2 IMOT  */
    _TRISA10 = 0; _RA10 = 0; _CNIEA10 = 0; _CNPUA10 = 0; _CNPDA10 = 0; /* FREE       */

    _TRISB0  = 1; _RB0  = 0; _CNIEB0  = 0; _CNPUB0  = 0; _CNPDB0  = 0; /* UART1 RX   */
    _TRISB1  = 0; _RB1  = 0; _CNIEB1  = 0; _CNPUB1  = 0; _CNPDB1  = 0; /* FREE       */
    _TRISB6  = 0; _RB6  = 0; _CNIEB6  = 0; _CNPUB6  = 0; _CNPDB6  = 0; /* PGD        */
    _TRISB7  = 0; _RB7  = 0; _CNIEB7  = 0; _CNPUB7  = 0; _CNPDB7  = 0; /* PGC        */
    _TRISB8  = 1; _RB8  = 0; _CNIEB8  = 0; _CNPUB8  = 0; _CNPDB8  = 0; /* ENC2 CHA   */
    _TRISB9  = 1; _RB9  = 0; _CNIEB9  = 0; _CNPUB9  = 0; _CNPDB9  = 0; /* ENC2 CHB   */
    _TRISB10 = 1; _RB10 = 0; _CNIEB10 = 0; _CNPUB10 = 0; _CNPDB10 = 0; /* ENC2 INDEX */
    _TRISB11 = 1; _RB11 = 0; _CNIEB11 = 0; _CNPUB11 = 0; _CNPDB11 = 0; /* ENC1 CHB   */
    _TRISB14 = 0; _RB14 = 0; _CNIEB14 = 0; _CNPUB14 = 0; _CNPDB14 = 0; /* FREE       */
    _TRISB15 = 0; _RB15 = 0; _CNIEB15 = 0; _CNPUB15 = 0; _CNPDB15 = 0; /* FREE       */

    _TRISC12 = 1; _RC12 = 0; _CNIEC12 = 0; _CNPUC12 = 0; _CNPDC12 = 0; /* OSC 10MHZ   */
    _TRISC13 = 0; _RC13 = 0; _CNIEC13 = 0; _CNPUC13 = 0; _CNPDC13 = 0; /* FREE        */

    _TRISD0  = 1; _RD0  = 0; _CNIED0  = 0; _CNPUD0  = 0; _CNPDD0  = 0; /* HTR         */
    _TRISD1  = 1; _RD1  = 0; _CNIED1  = 0; _CNPUD1  = 0; _CNPDD1  = 0; /* PLVL OR LMD */
    _TRISD2  = 0; _RD2  = 0; _CNIED2  = 0; _CNPUD2  = 0; _CNPDD2  = 0; /* FREE        */
    _TRISD3  = 1; _RD3  = 0; _CNIED3  = 0; _CNPUD3  = 0; _CNPDD3  = 0; /* RES ID 0    */
    _TRISD4  = 0; _RD4  = 0; _CNIED4  = 0; _CNPUD4  = 0; _CNPDD4  = 0; /* FREE        */
    _TRISD5  = 1; _RD5  = 0; _CNIED5  = 0; _CNPUD5  = 0; _CNPDD5  = 0; /* RES ID2     */
    _TRISD6  = 0; _RD6  = 0; _CNIED6  = 0; _CNPUD6  = 0; _CNPDD6  = 0; /* FREE        */
    _TRISD7  = 0; _RD7  = 0; _CNIED7  = 0; _CNPUD7  = 0; _CNPDD7  = 0; /* FREE        */
    _TRISD9  = 0; _RD9  = 0; _CNIED9  = 0; _CNPUD9  = 0; _CNPDD9  = 0; /* I2C1 SDA    */
    _TRISD10 = 0; _RD10 = 0; _CNIED10 = 0; _CNPUD10 = 0; _CNPDD10 = 0; /* I2C1 SCL    */
    _TRISD11 = 0; _RD11 = 0; _CNIED11 = 0; _CNPUD11 = 0; _CNPDD11 = 0; /* FREE        */
    _TRISD12 = 0; _RD12 = 0; _CNIED12 = 0; _CNPUD12 = 0; _CNPDD12 = 0; /* FREE        */
    _TRISD13 = 1; _RD13 = 0; _CNIED13 = 0; _CNPUD13 = 0; _CNPDD13 = 0; /* RES ID 1    */
    _TRISD14 = 1; _RD14 = 0; _CNIED14 = 0; _CNPUD14 = 0; _CNPDD14 = 0; /* UART1 CTS   */
    _TRISD15 = 0; _RD15 = 0; _CNIED15 = 0; _CNPUD15 = 0; _CNPDD15 = 0; /* UART2 TX    */

    _TRISE0  = 1; _RE0  = 0; _CNIEE0  = 0; _CNPUE0  = 0; _CNPDE0  = 0; /* LMD1 IMOT   */
    _TRISE1  = 0; _RE1  = 0; _CNIEE1  = 0; _CNPUE1  = 0; _CNPDE1  = 0; /* LMD1 PWM    */
    _TRISE3  = 0; _RE3  = 0; _CNIEE3  = 0; _CNPUE3  = 0; _CNPDE3  = 0; /* LMD2 PWM    */
    _TRISE5  = 0; _RE5  = 0; _CNIEE5  = 0; _CNPUE5  = 0; _CNPDE5  = 0; /* LED DEBUG 2 */

    _TRISF0  = 0; _RF0  = 0; _CNIEF0  = 0; _CNPUF0  = 0; _CNPDF0  = 0; /* FREE        */
    _TRISF1  = 0; _RF1  = 0; _CNIEF1  = 0; _CNPUF1  = 0; _CNPDF1  = 0; /* FREE        */
    _TRISF2  = 0; _RF2  = 0; _CNIEF2  = 0; _CNPUF2  = 0; _CNPDF2  = 0; /* FREE        */
    _TRISF4  = 0; _RF4  = 0; _CNIEF4  = 0; _CNPUF4  = 0; _CNPDF4  = 0; /* UART1 TX    */
    _TRISF5  = 0; _RF5  = 0; _CNIEF5  = 0; _CNPUF5  = 0; _CNPDF5  = 0; /* UART1 RTS   */
    _TRISF8  = 0; _RF8  = 0; _CNIEF8  = 0; _CNPUF8  = 0; _CNPDF8  = 0; /* FREE        */
    _TRISF12 = 0; _RF12 = 0; _CNIEF12 = 0; _CNPUF12 = 0; _CNPDF12 = 0; /* FREE        */
    _TRISF13 = 1; _RF13 = 0; _CNIEF13 = 0; _CNPUF13 = 0; _CNPDF13 = 0; /* ENC1 INDEX  */

    _TRISG0  = 0; _RG0  = 0; _CNIEG0  = 0; _CNPUG0  = 0; _CNPDG0  = 0; /* FREE        */
    _TRISG1  = 0; _RG1  = 0; _CNIEG1  = 0; _CNPUG1  = 0; _CNPDG1  = 0; /* FREE        */
    _TRISG12 = 0; _RG12 = 0; _CNIEG12 = 0; _CNPUG12 = 0; _CNPDG12 = 0; /* LMD1 DIR    */
    _TRISG13 = 0; _RG13 = 0; _CNIEG13 = 0; _CNPUG13 = 0; _CNPDG13 = 0; /* FREE        */
    _TRISG14 = 0; _RG14 = 0; _CNIEG14 = 0; _CNPUG14 = 0; _CNPDG14 = 0; /* LMD2 DIR    */
    _TRISG15 = 0; _RG15 = 0; _CNIEG15 = 0; _CNPUG15 = 0; _CNPDG15 = 0; /* LED DEBUG 1 */
}


void configPortsForPlvlBoard (void)
{
    _TRISA0  = 0; _RA0  = 0; _CNIEA0  = 0; _CNPUA0  = 0; _CNPDA0  = 0; /* FREE       */
    _TRISA2  = 0; _RA2  = 0; _CNIEA2  = 0; _CNPUA2  = 0; _CNPDA2  = 0; /* I2C2 SCL   */
    _TRISA3  = 0; _RA3  = 0; _CNIEA3  = 0; _CNPUA3  = 0; _CNPDA3  = 0; /* I2C2 SDA   */
    _TRISA9  = 0; _RA9  = 0; _CNIEA9  = 0; _CNPUA9  = 0; _CNPDA9  = 0; /* FREE       */
    _TRISA14 = 0; _RA14 = 0; _CNIEA14 = 0; _CNPUA14 = 0; _CNPDA14 = 0; /* FREE       */
    _TRISA15 = 0; _RA15 = 0; _CNIEA15 = 0; _CNPUA15 = 0; _CNPDA15 = 0; /* FREE       */

    _TRISB2  = 1; _RB2  = 0; _CNIEB2  = 0; _CNPUB2  = 0; _CNPDB2  = 0; /* SENSOR 1   */
    _TRISB3  = 1; _RB3  = 0; _CNIEB3  = 0; _CNPUB3  = 0; _CNPDB3  = 0; /* SENSOR 2   */
    _TRISB4  = 1; _RB4  = 0; _CNIEB4  = 0; _CNPUB4  = 0; _CNPDB4  = 0; /* SENSOR 3   */
    _TRISB5  = 1; _RB5  = 0; _CNIEB5  = 0; _CNPUB5  = 0; _CNPDB5  = 0; /* SENSOR 4   */
    _TRISB12 = 0; _RB12 = 0; _CNIEB12 = 0; _CNPUB12 = 0; _CNPDB12 = 0; /* FREE       */
    _TRISB13 = 0; _RB13 = 0; _CNIEB13 = 0; _CNPUB13 = 0; _CNPDB13 = 0; /* FREE       */

    _TRISC1  = 1; _RC1  = 0; _CNIEC1  = 0; _CNPUC1  = 0; _CNPDC1  = 0; /* SENSOR 10   */
    _TRISC2  = 1; _RC2  = 0; _CNIEC2  = 0; _CNPUC2  = 0; _CNPDC2  = 0; /* SENSOR 9    */
    _TRISC3  = 1; _RC3  = 0; _CNIEC3  = 0; _CNPUC3  = 0; _CNPDC3  = 0; /* SENSOR 8    */
    _TRISC4  = 1; _RC4  = 0; _CNIEC4  = 0; _CNPUC4  = 0; _CNPDC4  = 0; /* SENSOR 7    */
    _TRISC14 = 0; _RC14 = 0; _CNIEC14 = 0; _CNPUC14 = 0; _CNPDC14 = 0; /* POWER AX    */
    _TRISC15 = 0; _RC15 = 0; _CNIEC15 = 0; _CNPUC15 = 0; _CNPDC15 = 0; /* FREE        */

    _TRISD8  = 0; _RD8  = 0; _CNIED8  = 0; _CNPUD8  = 0; _CNPDD8  = 0; /* FREE        */

    _TRISE2  = 1; _RE2  = 0; _CNIEE2  = 0; _CNPUE2  = 0; _CNPDE2  = 0; /* LMD1 THERM  */
    _TRISE4  = 1; _RE4  = 0; _CNIEE4  = 0; _CNPUE4  = 0; _CNPDE4  = 0; /* LMD2 THERM  */
    _TRISE6  = 1; _RE6  = 0; _CNIEE6  = 0; _CNPUE6  = 0; _CNPDE6  = 0; /* SENSOR 12   */
    _TRISE7  = 1; _RE7  = 0; _CNIEE7  = 0; _CNPUE7  = 0; _CNPDE7  = 0; /* SENSOR 11   */
    _TRISE8  = 1; _RE8  = 0; _CNIEE8  = 0; _CNPUE8  = 0; _CNPDE8  = 0; /* SENSOR 6    */
    _TRISE9  = 1; _RE9  = 0; _CNIEE9  = 0; _CNPUE9  = 0; _CNPDE9  = 0; /* SENSOR 5    */

    _TRISF3  = 0; _RF3  = 0; _CNIEF3  = 0; _CNPUF3  = 0; _CNPDF3  = 0; /* CTRL CMD AX */

    _TRISG6  = 0; _RG6  = 0; _CNIEG6  = 0; _CNPUG6  = 0; _CNPDG6  = 0; /* FREE        */
    _TRISG7  = 0; _RG7  = 0; _CNIEG7  = 0; _CNPUG7  = 0; _CNPDG7  = 0; /* FREE        */
    _TRISG8  = 0; _RG8  = 0; _CNIEG8  = 0; _CNPUG8  = 0; _CNPDG8  = 0; /* FREE        */
    _TRISG9  = 0; _RG9  = 0; _CNIEG9  = 0; _CNPUG9  = 0; _CNPDG9  = 0; /* FREE        */

    /* Specific configuration                                                 */
    if ((config.sensors[ID_SENSOR12].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR12].type == OUTPUT_DIGITAL))
    {
        _TRISE6  = 0; _CNIEE6  = 0; _CNPUE6  = 0; _CNPDE6  = 0; /* SENSOR 12   */
        _RE6 = config.sensors[ID_SENSOR12].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR11].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR11].type == OUTPUT_DIGITAL))
    {
        _TRISE7  = 0; _CNIEE7  = 0; _CNPUE7  = 0; _CNPDE7  = 0; /* SENSOR 11   */
        _RE7 = config.sensors[ID_SENSOR11].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR10].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR10].type == OUTPUT_DIGITAL))
    {
        _TRISC1  = 0; _CNIEC1  = 0; _CNPUC1  = 0; _CNPDC1  = 0; /* SENSOR 10   */
        _RC1 = config.sensors[ID_SENSOR10].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR9].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR9].type == OUTPUT_DIGITAL))
    {
        _TRISC2  = 0; _CNIEC2  = 0; _CNPUC2  = 0; _CNPDC2  = 0; /* SENSOR 9    */
        _RC2 = config.sensors[ID_SENSOR9].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR8].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR8].type == OUTPUT_DIGITAL))
    {
        _TRISC3  = 0; _CNIEC3  = 0; _CNPUC3  = 0; _CNPDC3  = 0; /* SENSOR 8    */
        _RC3 = config.sensors[ID_SENSOR8].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR7].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR7].type == OUTPUT_DIGITAL))
    {
        _TRISC4  = 0; _CNIEC4  = 0; _CNPUC4  = 0; _CNPDC4  = 0; /* SENSOR 7    */
        _RC4 = config.sensors[ID_SENSOR7].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR6].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR6].type == OUTPUT_DIGITAL))
    {
        _TRISE8  = 0; _CNIEE8  = 0; _CNPUE8  = 0; _CNPDE8  = 0; /* SENSOR 6    */
        _RE8 = config.sensors[ID_SENSOR6].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR5].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR5].type == OUTPUT_DIGITAL))
    {
        _TRISE9  = 0; _CNIEE9  = 0; _CNPUE9  = 0; _CNPDE9  = 0; /* SENSOR 5    */
        _RE9 = config.sensors[ID_SENSOR5].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR4].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR4].type == OUTPUT_DIGITAL))
    {
        _TRISB5  = 0; _CNIEB5  = 0; _CNPUB5  = 0; _CNPDB5  = 0; /* SENSOR 4   */
        _RB5 = config.sensors[ID_SENSOR4].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR3].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR3].type == OUTPUT_DIGITAL))
    {
        _TRISB4  = 0; _CNIEB4  = 0; _CNPUB4  = 0; _CNPDB4  = 0; /* SENSOR 3   */
        _RB4 = config.sensors[ID_SENSOR3].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR2].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR2].type == OUTPUT_DIGITAL))
    {
        _TRISB3  = 0; _CNIEB3  = 0; _CNPUB3  = 0; _CNPDB3  = 0; /* SENSOR 2   */
        _RB3 = config.sensors[ID_SENSOR2].dftOutputValue;
    }

    if ((config.sensors[ID_SENSOR1].type == OUTPUT_PWM)
     || (config.sensors[ID_SENSOR1].type == OUTPUT_DIGITAL))
    {
        _TRISB2  = 0; _CNIEB2  = 0; _CNPUB2  = 0; _CNPDB2  = 0; /* SENSOR 1   */
        _RB2 = config.sensors[ID_SENSOR1].dftOutputValue;
    }
}


void configPortsForMotorsBoard (void)
{
    _TRISA0  = 0; _RA0  = 0; _CNIEA0  = 0; _CNPUA0  = 0; _CNPDA0  = 0; /* DIR 7      */
    _TRISA2  = 0; _RA2  = 0; _CNIEA2  = 0; _CNPUA2  = 0; _CNPDA2  = 0; /* FREE       */
    _TRISA3  = 0; _RA3  = 0; _CNIEA3  = 0; _CNPUA3  = 0; _CNPDA3  = 0; /* FREE       */
    _TRISA9  = 0; _RA9  = 0; _CNIEA9  = 0; _CNPUA9  = 0; _CNPDA9  = 0; /* DIR 4      */
    _TRISA14 = 0; _RA14 = 0; _CNIEA14 = 0; _CNPUA14 = 0; _CNPDA14 = 0; /* LED MOT 3  */
    _TRISA15 = 0; _RA15 = 0; _CNIEA15 = 0; _CNPUA15 = 0; _CNPDA15 = 0; /* LED MOT 2  */

    _TRISB2  = 1; _RB2  = 0; _CNIEB2  = 0; _CNPUB2  = 0; _CNPDB2  = 0; /* iMot 6     */
    _TRISB3  = 1; _RB3  = 0; _CNIEB3  = 0; _CNPUB3  = 0; _CNPDB3  = 0; /* iMot 5     */
    _TRISB4  = 1; _RB4  = 0; _CNIEB4  = 0; _CNPUB4  = 0; _CNPDB4  = 0; /* iMot 4     */
    _TRISB5  = 1; _RB5  = 0; _CNIEB5  = 0; _CNPUB5  = 0; _CNPDB5  = 0; /* iMot 3     */
    _TRISB12 = 1; _RB12 = 0; _CNIEB12 = 0; _CNPUB12 = 0; _CNPDB12 = 0; /* iMot 7     */
    _TRISB13 = 1; _RB13 = 0; _CNIEB13 = 0; _CNPUB13 = 0; _CNPDB13 = 0; /* iMot 8     */

    _TRISC1  = 0; _RC1  = 0; _CNIEC1  = 0; _CNPUC1  = 0; _CNPDC1  = 0; /* PWM 5      */
    _TRISC2  = 0; _RC2  = 0; _CNIEC2  = 0; _CNPUC2  = 0; _CNPDC2  = 0; /* PWM 6      */
    _TRISC3  = 0; _RC3  = 0; _CNIEC3  = 0; _CNPUC3  = 0; _CNPDC3  = 0; /* PWM 7      */
    _TRISC4  = 0; _RC4  = 0; _CNIEC4  = 0; _CNPUC4  = 0; _CNPDC4  = 0; /* PWM 8      */
    _TRISC14 = 0; _RC14 = 0; _CNIEC14 = 0; _CNPUC14 = 0; _CNPDC14 = 0; /* FREE       */
    _TRISC15 = 0; _RC15 = 0; _CNIEC15 = 0; _CNPUC15 = 0; _CNPDC15 = 0; /* LED MOT 4  */

    _TRISD8  = 0; _RD8  = 0; _CNIED8  = 0; _CNPUD8  = 0; _CNPDD8  = 0; /* LED MOT 1  */

    _TRISE2  = 0; _RE2  = 0; _CNIEE2  = 0; _CNPUE2  = 0; _CNPDE2  = 0; /* FREE       */
    _TRISE4  = 0; _RE4  = 0; _CNIEE4  = 0; _CNPUE4  = 0; _CNPDE4  = 0; /* FREE       */
    _TRISE6  = 0; _RE6  = 0; _CNIEE6  = 0; _CNPUE6  = 0; _CNPDE6  = 0; /* PWM 3      */
    _TRISE7  = 0; _RE7  = 0; _CNIEE7  = 0; _CNPUE7  = 0; _CNPDE7  = 0; /* PWM 4      */
    _TRISE8  = 1; _RE8  = 0; _CNIEE8  = 0; _CNPUE8  = 0; _CNPDE8  = 0; /* VBAT 1     */
    _TRISE9  = 1; _RE9  = 0; _CNIEE9  = 0; _CNPUE9  = 0; _CNPDE9  = 0; /* VBAT 2     */

    _TRISF3  = 0; _RF3  = 0; _CNIEF3  = 0; _CNPUF3  = 0; _CNPDF3  = 0; /* FREE       */

    _TRISG6  = 0; _RG6  = 0; _CNIEG6  = 0; _CNPUG6  = 0; _CNPDG6  = 0; /* DIR 3      */
    _TRISG7  = 0; _RG7  = 0; _CNIEG7  = 0; _CNPUG7  = 0; _CNPDG7  = 0; /* DIR 5      */
    _TRISG8  = 0; _RG8  = 0; _CNIEG8  = 0; _CNPUG8  = 0; _CNPDG8  = 0; /* DIR 8      */
    _TRISG9  = 0; _RG9  = 0; _CNIEG9  = 0; _CNPUG9  = 0; _CNPDG9  = 0; /* DIR 7      */
}


void configTimers (void)
{
    /* TIMER1                                                                 */
    T1CONbits.TON   = 0;        /* Stop Timer                                 */
    T1CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T1CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T1CONbits.TCKPS = config.timers[ID_TIMER1].prescale;  /* Presscaler value */
    T1CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR1            = 0;        /* Clearing timer register                    */
    PR1             = config.timers[ID_TIMER1].period;  /* Period value       */

    /* TIMER2                                                                 */
    T2CONbits.TON   = 0;        /* Stop Timer                                 */
    T2CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T2CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T2CONbits.TCKPS = config.timers[ID_TIMER2].prescale;  /* Presscaler value */
    T2CONbits.T32   = config.timers[ID_TIMER2].t32bits;   /* 16 or 32 bits    */
    T2CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR2            = 0;        /* Clearing timer register                    */
    PR2             = config.timers[ID_TIMER2].period;  /* Period value       */

    /* TIMER3                                                                 */
    T3CONbits.TON   = 0;        /* Stop Timer                                 */
    T3CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T3CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T3CONbits.TCKPS = config.timers[ID_TIMER3].prescale;  /* Presscaler value */
    T3CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR3            = 0;        /* Clearing timer register                    */
    PR3             = config.timers[ID_TIMER3].period;  /* Period value       */

    /* TIMER4                                                                 */
    T4CONbits.TON   = 0;        /* Stop Timer                                 */
    T4CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T4CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T4CONbits.TCKPS = config.timers[ID_TIMER4].prescale;  /* Presscaler value */
    T4CONbits.T32   = config.timers[ID_TIMER4].t32bits;   /* 16 or 32 bits    */
    T4CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR4            = 0;        /* Clearing timer register                    */
    PR4             = config.timers[ID_TIMER4].period;  /* Period value       */

    /* TIMER5                                                                 */
    T5CONbits.TON   = 0;        /* Stop Timer                                 */
    T5CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T5CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T5CONbits.TCKPS = config.timers[ID_TIMER5].prescale;  /* Presscaler value */
    T5CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR5            = 0;        /* Clearing timer register                    */
    PR5             = config.timers[ID_TIMER5].period;  /* Period value       */

    /* TIMER6                                                                 */
    T6CONbits.TON   = 0;        /* Stop Timer                                 */
    T6CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T6CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T6CONbits.TCKPS = config.timers[ID_TIMER6].prescale;  /* Presscaler value */
    T6CONbits.T32   = config.timers[ID_TIMER6].t32bits;   /* 16 or 32 bits    */
    T6CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR6            = 0;        /* Clearing timer register                    */
    PR6             = config.timers[ID_TIMER6].period;  /* Period value       */

    /* TIMER7                                                                 */
    T7CONbits.TON   = 0;        /* Stop Timer                                 */
    T7CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T7CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T7CONbits.TCKPS = config.timers[ID_TIMER7].prescale;  /* Presscaler value */
    T7CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR7            = 0;        /* Clearing timer register                    */
    PR7             = config.timers[ID_TIMER7].period;  /* Period value       */

    /* TIMER8                                                                 */
    T8CONbits.TON   = 0;        /* Stop Timer                                 */
    T8CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T8CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T8CONbits.TCKPS = config.timers[ID_TIMER8].prescale;  /* Presscaler value */
    T8CONbits.T32   = config.timers[ID_TIMER8].t32bits;   /* 16 or 32 bits    */
    T8CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR8            = 0;        /* Clearing timer register                    */
    PR8             = config.timers[ID_TIMER8].period;  /* Period value       */

    /* TIMER9                                                                 */
    T9CONbits.TON   = 0;        /* Stop Timer                                 */
    T9CONbits.TSIDL = 0;        /* Continue timer operation in idle mode      */
    T9CONbits.TGATE = 0;        /* Gated time accumulation disabled           */
    T9CONbits.TCKPS = config.timers[ID_TIMER9].prescale;  /* Presscaler value */
    T9CONbits.TCS   = 0;        /* Internal Clock                             */
    TMR9            = 0;        /* Clearing timer register                    */
    PR9             = config.timers[ID_TIMER9].period;  /* Period value       */


    /* Enable/Disable timers                                                  */
    T1CONbits.TON   = config.timers[ID_TIMER1].used;
    T2CONbits.TON   = config.timers[ID_TIMER2].used;
    T3CONbits.TON   = config.timers[ID_TIMER3].used;
    T4CONbits.TON   = config.timers[ID_TIMER4].used;
    T5CONbits.TON   = config.timers[ID_TIMER5].used;
    T6CONbits.TON   = config.timers[ID_TIMER6].used;
    T7CONbits.TON   = config.timers[ID_TIMER7].used;
    T8CONbits.TON   = config.timers[ID_TIMER8].used;
    T9CONbits.TON   = config.timers[ID_TIMER9].used;

    /* Reseting interrupts flags                                              */
    IFS0bits.T1IF   = 0;
    IFS0bits.T2IF   = 0;
    IFS0bits.T3IF   = 0;
    IFS1bits.T4IF   = 0;
    IFS1bits.T5IF   = 0;
    IFS2bits.T6IF   = 0;
    IFS3bits.T7IF   = 0;
    IFS3bits.T8IF   = 0;
    IFS3bits.T9IF   = 0;

    /* Interrupt priority level                                               */
    IPC0bits.T1IP   = config.interrupts[ID_IRQ_T1].priority;
    IPC1bits.T2IP   = config.interrupts[ID_IRQ_T2].priority;
    IPC2bits.T3IP   = config.interrupts[ID_IRQ_T3].priority;
    IPC6bits.T4IP   = config.interrupts[ID_IRQ_T4].priority;
    IPC7bits.T5IP   = config.interrupts[ID_IRQ_T5].priority;
    IPC11bits.T6IP  = config.interrupts[ID_IRQ_T6].priority;
    IPC12bits.T7IP  = config.interrupts[ID_IRQ_T7].priority;
    IPC12bits.T8IP  = config.interrupts[ID_IRQ_T8].priority;
    IPC13bits.T9IP  = config.interrupts[ID_IRQ_T9].priority;

    /* Enable/Disable interrupts                                              */
    IEC0bits.T1IE   = config.interrupts[ID_IRQ_T1].used;
    IEC0bits.T2IE   = config.interrupts[ID_IRQ_T2].used;
    IEC0bits.T3IE   = config.interrupts[ID_IRQ_T3].used;
    IEC1bits.T4IE   = config.interrupts[ID_IRQ_T4].used;
    IEC1bits.T5IE   = config.interrupts[ID_IRQ_T5].used;
    IEC2bits.T6IE   = config.interrupts[ID_IRQ_T6].used;
    IEC3bits.T7IE   = config.interrupts[ID_IRQ_T7].used;
    IEC3bits.T8IE   = config.interrupts[ID_IRQ_T8].used;
    IEC3bits.T9IE   = config.interrupts[ID_IRQ_T9].used;
}


void configExternalInterrupt (void)
{
    /* Interrupt on positive edge         */
    INTCON2bits.INT0EP  = 0;

    /* Reseting interrupts flags                                              */
    IFS0bits.INT0IF     = 0;

    /* Interrupt priority level                                               */
    IPC0bits.INT0IP     = config.interrupts[ID_IRQ_INT0].priority;

    /* Enable/Disable interrupts                                              */
    IEC0bits.INT0IE     = config.interrupts[ID_IRQ_INT0].used;
}


void configQEI (void)
{
    /* Encoder 1                                                              */
    QEI1CONbits.QEISIDL = 0;        /* Continue idle                          */
    QEI1CONbits.INTDIV  = 0;        /* Prescaler                              */
    QEI1CONbits.CNTPOL  = 0;        /* Default connter direction              */
    QEI1CONbits.GATEN   = 0;        /* External gate does not affect counter  */
    QEI1CONbits.CCM     = 0;        /* Quadrature encoder mode                */

    /* reset mode                                                             */
    if (config.qei[ID_ENCODER1].indexPin == USED)
    {
        QEI1CONbits.PIMOD   = 1;    /* Index pin reset counter                */
        QEI1CONbits.IMV     = 3;    /* Index timing                           */
    }
    else
    {
        QEI1CONbits.PIMOD   = 6;    /* Index pin not used                     */
        QEI1GECL            = config.qei[ID_ENCODER1].impulsions;
        QEI1GECH            = 0;
    }

    QEI1IOCbits.QCAPEN  = 0;        /* Home event not used                    */
    QEI1IOCbits.FLTREN  = 0;        /* No filter                              */
    QEI1IOCbits.QFDIV   = 0;        /* Clock divider                          */
    QEI1IOCbits.OUTFNC  = 0;        /* Output disabled                        */
    QEI1IOCbits.SWPAB   = 0;        /* QEA and QEB not swaped                 */
    QEI1IOCbits.HOMPOL  = 0;        /* Polarity not inverted                  */
    QEI1IOCbits.IDXPOL  = 0;        /* Polarity not inverted                  */
    QEI1IOCbits.QEBPOL  = 0;        /* Polarity not inverted                  */
    QEI1IOCbits.QEAPOL  = 0;        /* Polarity not inverted                  */

    QEI1STATbits.POSOVIEN   = 1;    /* Overflow interrupt enabled             */
    QEI1STATbits.IDXIEN     = 1;    /* Index interrupt enabled                */

    QEI1CONbits.QEIEN   = config.qei[ID_ENCODER1].used; /* Enabled this qei   */


    /* Encoder 2                                                              */
    QEI2CONbits.QEISIDL = 0;        /* Continue idle                          */
    QEI2CONbits.INTDIV  = 0;        /* Prescaler                              */
    QEI2CONbits.CNTPOL  = 0;        /* Default connter direction              */
    QEI2CONbits.GATEN   = 0;        /* External gate does not affect counter  */
    QEI2CONbits.CCM     = 0;        /* Quadrature encoder mode                */

    /* reset mode                                                             */
    if (config.qei[ID_ENCODER2].indexPin == USED)
    {
        QEI2CONbits.PIMOD   = 1;    /* Index pin reset counter                */
        QEI2CONbits.IMV     = 3;    /* Index timing                           */
    }
    else
    {
        QEI2CONbits.PIMOD   = 6;    /* Index pin not used                     */
        QEI2GECL            = config.qei[ID_ENCODER2].impulsions;
        QEI2GECH            = 0;
    }

    QEI2IOCbits.QCAPEN  = 0;        /* Home event not used                    */
    QEI2IOCbits.FLTREN  = 0;        /* No filter                              */
    QEI2IOCbits.QFDIV   = 0;        /* Clock divider                          */
    QEI2IOCbits.OUTFNC  = 0;        /* Output disabled                        */
    QEI2IOCbits.SWPAB   = 0;        /* QEA and QEB not swaped                 */
    QEI2IOCbits.HOMPOL  = 0;        /* Polarity not inverted                  */
    QEI2IOCbits.IDXPOL  = 0;        /* Polarity not inverted                  */
    QEI2IOCbits.QEBPOL  = 0;        /* Polarity not inverted                  */
    QEI2IOCbits.QEAPOL  = 0;        /* Polarity not inverted                  */

    QEI2STATbits.POSOVIEN   = 1;    /* Overflow interrupt enabled             */
    QEI2STATbits.IDXIEN     = 1;    /* Index interrupt enabled                */

    QEI2CONbits.QEIEN   = config.qei[ID_ENCODER2].used; /* Enabled this qei   */


    /* Reseting interrupts flags                                              */
    IFS3bits.QEI1IF     = 0;
    IFS4bits.QEI2IF     = 0;

    /* Interrupt priority level                                               */
    IPC14bits.QEI1IP    = config.interrupts[ID_IRQ_QEI1_OVERFLOW].priority;
    IPC18bits.QEI2IP    = config.interrupts[ID_IRQ_QEI2_OVERFLOW].priority;

    /* Enable/Disable interrupts                                              */
    IEC3bits.QEI1IE     = config.interrupts[ID_IRQ_QEI1_OVERFLOW].used;
    IEC4bits.QEI2IE     = config.interrupts[ID_IRQ_QEI2_OVERFLOW].used;
}


void configPWM (void)
{
    /* PDCx  : duty cycle for PWMxH                                           */
    /* SDCx  : duty cycle for PWMxL                                           */
    /* PTPER : Time Base for PWMxH                                            */
    /* STPER : Time Base for PWMxL                                            */

    PTCONbits.PTEN      = 0;                /* Disbale the PWM mod            */
    PTCON2              = 0;                /* Prescaler for PWMxH            */
    STCON2              = 0;                /* Prescaler for PWMxL            */
    
    /* PWM Input Clock Prescaler : */
    PTCON2bits.PCLKDIV = 0b101;             /* Divide by 32                   */


    /***** PWM 1 : MOTOR 1 (PWM1H)                                            */
    IOCON1bits.PENH     = config.pwm[ID_PWM1].used;     /* PWM 1              */
    IOCON1bits.POLH     = config.pwm[ID_PWM1].polarity; /* Pin polarity       */
    IOCON1bits.PMOD     = 3;                            /* Independent PWM    */

    /* PWCONx (Independent Time Bases, Edge-Aligned Mode and Independent DC)  */
    PWMCON1bits.ITB     = 1;                            /* using PHASE        */
    PWMCON1bits.MDCS    = 0;                            /* using PDCx & SDCx  */
    PWMCON1bits.CAM     = 0;                            /* using Edge-Aligned */

    PHASE1              = config.pwm[ID_PWM1].phase;    /* Period             */
    DTR1                = 0;                /* No Dead Time Values            */
    ALTDTR1             = 0;                /* Not Alternate Dead Time Values */
    FCLCON1bits.FLTMOD  = 3;                /* Disabled Faults                */
    PDC1                = 0;                /* Default duty cycle to 0        */


    /***** PWM 2 : MOTOR 2 (PWM2H)                                            */
    IOCON2bits.PENH     = config.pwm[ID_PWM2].used;     /* PWM2               */
    IOCON2bits.POLH     = config.pwm[ID_PWM2].polarity; /* Pin polarity       */
    IOCON2bits.PMOD     = 3;                            /* Independent PWM    */

    /* PWCONx (Independent Time Bases, Edge-Aligned Mode and Independent DC)  */
    PWMCON2bits.ITB     = 1;                            /* using PHASE        */
    PWMCON2bits.MDCS    = 0;                            /* using PDCx & SDCx  */
    PWMCON2bits.CAM     = 0;                            /* using Edge-Aligned */

    PHASE2              = config.pwm[ID_PWM2].phase;    /* Period             */
    DTR2                = 0;                /* No Dead Time Values            */
    ALTDTR2             = 0;                /* Not Alternate Dead Time Values */
    FCLCON2bits.FLTMOD  = 3;                /* Disabled Faults                */    
    PDC2                = 0;                /* Default duty cycle to 0        */
    

    /***** PWM 4 : MOTORS 3 & 4 (PWM4L PWM4H)                                 */
    IOCON4bits.PENL     = config.pwm[ID_PWM3].used;     /* PWM3               */
    IOCON4bits.POLL     = config.pwm[ID_PWM3].polarity; /* Pin polarity       */
    
    IOCON4bits.PENH     = config.pwm[ID_PWM4].used;     /* PWM4               */
    IOCON4bits.POLH     = config.pwm[ID_PWM4].polarity; /* Pin polarity       */

    IOCON4bits.PMOD     = 3;                            /* Independent PWM    */

    /* PWCONx (Independent Time Bases, Edge-Aligned Mode and Independent DC)  */
    PWMCON4bits.ITB     = 1;                            /* using PHASE        */
    PWMCON4bits.MDCS    = 0;                            /* using PDCx & SDCx  */
    PWMCON4bits.CAM     = 0;                            /* using Edge-Aligned */

    SPHASE4             = config.pwm[ID_PWM3].phase;    /* Period             */
    PHASE4              = config.pwm[ID_PWM4].phase;    /* Period             */
    DTR4                = 0;                /* No Dead Time Values            */
    ALTDTR4             = 0;                /* Not Alternate Dead Time Values */
    FCLCON4bits.FLTMOD  = 3;                /* Disabled Faults                */
    SDC4                = 0;                /* Default duty cycle to 0        */
    PDC4                = 0;                /* Default duty cycle to 0        */


    /* PWM 5 : MOTORS 5 & 6 (PWM5L PWM5H)                                     */
    IOCON5bits.PENL     = config.pwm[ID_PWM5].used;     /* PWM5               */
    IOCON5bits.POLL     = config.pwm[ID_PWM5].polarity; /* Pin polarity       */
    
    IOCON5bits.PENH     = config.pwm[ID_PWM6].used;     /* PWM5               */
    IOCON5bits.POLH     = config.pwm[ID_PWM6].polarity; /* Pin polarity       */

    IOCON5bits.PMOD     = 3;                            /* Independent PWM    */

    /* PWCONx (Independent Time Bases, Edge-Aligned Mode and Independent DC)  */
    PWMCON5bits.ITB     = 1;                            /* using PHASE        */
    PWMCON5bits.MDCS    = 0;                            /* using PDCx & SDCx  */
    PWMCON5bits.CAM     = 0;                            /* using Edge-Aligned */

    SPHASE5             = config.pwm[ID_PWM5].phase;    /* Period             */
    PHASE5              = config.pwm[ID_PWM6].phase;    /* Period             */
    DTR5                = 0;                /* No Dead Time Values            */
    ALTDTR5             = 0;                /* Not Alternate Dead Time Values */
    FCLCON5bits.FLTMOD  = 3;                /* Disabled Faults                */
    SDC5                = 0;                /* Default duty cycle to 0        */
    PDC5                = 0;                /* Default duty cycle to 0        */


    /* PWM 6 : MOTORS 7 & 8 (PWM6L PWM6H)                                     */
    IOCON6bits.PENL     = config.pwm[ID_PWM7].used;     /* PWM6               */
    IOCON6bits.POLL     = config.pwm[ID_PWM7].polarity; /* Pin polarity       */

    IOCON6bits.PENH     = config.pwm[ID_PWM8].used;     /* PWM6               */
    IOCON6bits.POLH     = config.pwm[ID_PWM8].polarity; /* Pin polarity       */

    IOCON6bits.PMOD     = 3;                            /* Independent PWM    */

    /* PWCONx (Independent Time Bases, Edge-Aligned Mode and Independent DC)  */
    PWMCON6bits.ITB     = 1;                            /* using PHASE        */
    PWMCON6bits.MDCS    = 0;                            /* using PDCx & SDCx  */
    PWMCON6bits.CAM     = 0;                            /* using Edge-Aligned */

    SPHASE6             = config.pwm[ID_PWM7].phase;    /* Period             */
    PHASE6              = config.pwm[ID_PWM8].phase;    /* Period             */
    DTR6                = 0;                /* No Dead Time Values            */
    ALTDTR6             = 0;                /* Not Alternate Dead Time Values */
    FCLCON6bits.FLTMOD  = 3;                /* Disabled Faults                */
    SDC6                = 0;                /* Default duty cycle to 0        */
    PDC6                = 0;                /* Default duty cycle to 0        */

    PTCONbits.PTEN      = 1;                /* Enable the PWM mod             */
}


void configADC (void)
{
    /* Analog Port                                                            */
    if (config.system.isMotorBoard == NO)
    {
        if (config.sensors[ID_SENSOR1].type <= 1)
        {
            _ANSB2  = config.sensors[ID_SENSOR1].type;      /* Sensor 1       */
        }

        if (config.sensors[ID_SENSOR2].type <= 1)
        {
            _ANSB3  = config.sensors[ID_SENSOR2].type;      /* Sensor 2       */
        }

        if (config.sensors[ID_SENSOR3].type <= 1)
        {
            _ANSB4  = config.sensors[ID_SENSOR3].type;    /* Sensor 3         */
        }

        if (config.sensors[ID_SENSOR4].type <= 1)
        {
            _ANSB5  = config.sensors[ID_SENSOR4].type;    /* Sensor 4         */
        }

        if (config.sensors[ID_SENSOR5].type <= 1)
        {
            _ANSE9  = config.sensors[ID_SENSOR5].type;    /* Sensor 5         */
        }

        if (config.sensors[ID_SENSOR6].type <= 1)
        {
            _ANSE8  = config.sensors[ID_SENSOR6].type;    /* Sensor 6         */
        }

        if (config.sensors[ID_SENSOR7].type <= 1)
        {
            _ANSC4  = config.sensors[ID_SENSOR7].type;    /* Sensor 7         */
        }

        if (config.sensors[ID_SENSOR8].type <= 1)
        {
            _ANSC3  = config.sensors[ID_SENSOR8].type;    /* Sensor 8         */
        }

        if (config.sensors[ID_SENSOR9].type <= 1)
        {
            _ANSC2  = config.sensors[ID_SENSOR9].type;    /* Sensor 9         */
        }

        if (config.sensors[ID_SENSOR10].type <= 1)
        {
            _ANSC1  = config.sensors[ID_SENSOR10].type;    /* Sensor 10       */
        }

        if (config.sensors[ID_SENSOR11].type <= 1)
        {
            _ANSE7  = config.sensors[ID_SENSOR11].type;    /* Sensor 11       */
        }

        if (config.sensors[ID_SENSOR12].type <= 1)
        {
            _ANSE6  = config.sensors[ID_SENSOR12].type;    /* Sensor 12       */
        }
    
        _ANSE0  = 1;    /* iMot 1                                             */
        _ANSA7  = 1;    /* iMot 2                                             */
        _ANSA6  = 1;    /* vBat                                               */
    }
    else
    {
        _ANSE0  = 1;    /* iMot 1                                             */
        _ANSA7  = 1;    /* iMot 2                                             */
        _ANSB5  = 1;    /* iMot 3                                             */
        _ANSB4  = 1;    /* iMot 4                                             */
        _ANSB3  = 1;    /* iMot 5                                             */
        _ANSB2  = 1;    /* iMot 6                                             */
        _ANSB12 = 1;    /* iMot 7                                             */
        _ANSB13 = 1;    /* iMot 8                                             */
        _ANSA6  = 1;    /* vBat                                               */
        _ANSE8  = 1;    /* vBat1                                              */
        _ANSE9  = 1;    /* vBat2                                              */
    }

    /* Configuring ADC registers                                              */
    AD1CON1bits.ADON    = 0;    /* ADC disabled                               */
    AD1CON1bits.ADSIDL  = 0;    /* Continue mode in IDLE                      */
    AD1CON1bits.ADDMABM = 0;    /* DMA buffers are built in scatter           */
    AD1CON1bits.AD12B   = 1;    /* 10-bit ADC operation                       */

    AD1CON1bits.FORM    = 0;    /* Integer                                    */
    AD1CON1bits.SSRC    = 0;    /* Manual conversion                          */
    AD1CON1bits.SSRCG   = 0;    /* Manual conversion                          */
    AD1CON1bits.SIMSAM  = 0;    /* Unused                                     */
    AD1CON1bits.ASAM    = 0;    /* Sampling begins when SAMP bit is set       */
    AD1CON1bits.SAMP    = 0;    /* SAMP=0 : stop sampling / SAMP=1 : sampling */

    AD1CON2bits.VCFG    = 0;    /* Vref : AVdd / AVss                         */
    AD1CON2bits.CSCNA   = 0;    /* Do not scan inputs                         */
    AD1CON2bits.CHPS    = 0;    /* Convert Ch0                                */
    AD1CON2bits.SMPI    = 0;    /* DMA note used                              */
    AD1CON2bits.BUFM    = 0;    /* starts filling the buffer from the start @ */
    AD1CON2bits.ALTS    = 0;    /* Always uses channel input selected         */

    AD1CON3bits.ADRC    = 0;    /* ADC Clock is derived from Systems          */
    AD1CON3bits.SAMC    = 0;    /* Manual sampling                            */
    AD1CON3bits.ADCS    = 0x0F; /* ADC Conversion Clock Select bits           */

    AD1CON4bits.ADDMAEN = 0;    /* DMA disabled                               */
    AD1CON4bits.DMABL   = 0;    /* DMA not used                               */

    AD1CHS0bits.CH0NA   = 0;    /* Negative Input Vrefl                       */
    AD1CHS0bits.CH0SA   = 22;   /* Positive Input default Vbat                */

    AD1CHS123           = 0;    /* Channel selection (not used in 12b)        */

    AD1CSSH             = 0;    /* Register selection (only used for scan)    */
    AD1CSSL             = 0;    /* Register selection (only used for scan)    */

    /* ADC enabled                                                            */
    AD1CON1bits.ADON = 1;
}


void configUART (uint8_t uartId)
{
    double baudDouble1;
    double baudDouble2;
    uint16_t u1BRGvalue = 0;
    uint16_t u2BRGvalue = 0;

    /* Calculating baudrate                                                   */
    /*   UxBRG = ((Fcy/BaudRate)/16)-1                                        */
    baudDouble1 = (((double)config.system.fcy / (double)config.uart[ID_UART1].baudrate) / (double)4) - 1.0;
    baudDouble2 = (((double)config.system.fcy / (double)config.uart[ID_UART2].baudrate) / (double)4) - 1.0;
    
    u1BRGvalue = (uint16_t)(baudDouble1);
    u2BRGvalue = (uint16_t)(baudDouble2);

    if (uartId == ID_UART1)
    {
        /* Configuring UART 1                                                     */
        if (config.uart[ID_UART1].used == USED)
        {
            U1MODEbits.UARTEN   = 0;            /* UART disabled                  */
            U1MODEbits.USIDL    = 0;            /* Continue in Idle               */
            U1MODEbits.IREN     = 0;            /* No IR translation              */
            U1MODEbits.RTSMD    = 0;            /* Flow Control mode              */

            if (config.uart[ID_UART1].ctsRtsUsed == USED)
            {
                U1MODEbits.UEN  = 2;            /* TX,RX,CTS,RTS enabled          */
            }
            else
            {
                U1MODEbits.UEN  = 0;            /* TX,RX enabled, CTS,RTS not     */
            }

            if (config.uart[ID_UART1].polarityInverted == USED)
            {
                U1MODEbits.URXINV   = 1;            /* UxRX idle state is 1       */
                U1STAbits.UTXINV    = 1;            /* Idle state is 1            */
            }
            else
            {
                U1MODEbits.URXINV   = 0;            /* UxRX idle state is 1       */
                U1STAbits.UTXINV    = 0;            /* Idle state is 1            */
            }

            U1MODEbits.WAKE     = 0;            /* No Wake up                     */
            U1MODEbits.LPBACK   = 0;            /* No Loop Back                   */
            U1MODEbits.ABAUD    = 0;            /* No Autobaud                    */
            U1MODEbits.BRGH     = 1;            /* 16 clocks per bit period       */
            U1MODEbits.PDSEL    = 0;            /* 8bit, No Parity                */
            U1MODEbits.STSEL    = 0;            /* One Stop Bit                   */

            U1BRG               = u1BRGvalue;   /* Baudrate                       */

            U1STAbits.UTXISEL1  = 0;            /* Any character transferred      */
            U1STAbits.UTXISEL0  = 0;            /* Any character transferred      */
            U1STAbits.UTXBRK    = 0;            /* SyncBreak transmission disabled*/
            U1STAbits.UTXEN     = 0;            /* TX disabeld                    */
            U1STAbits.URXISEL   = 0;            /* IT on character recieved       */
            U1STAbits.ADDEN     = 0;            /* Address Detect Disabled        */

            U1MODEbits.UARTEN   = 1;            /* UART enabled                   */
            U1STAbits.UTXEN     = 1;            /* TX enabled                     */

            /* Reseting interrupts flags                                          */
            IFS0bits.U1RXIF = 0;
            IFS0bits.U1TXIF = 0;

            /* Interrupt priority level                                           */
            IPC2bits.U1RXIP = config.interrupts[ID_IRQ_UART1RX].priority;

            /* Enable/Disable interrupts                                          */
            IEC0bits.U1RXIE = config.interrupts[ID_IRQ_UART1RX].used;

            IFS4bits.U1EIF = 0;
            IEC4bits.U1EIE = 1;
        }
    }
    else if (uartId == ID_UART2)
    {
        /* Configuring UART 2 */
        if (config.uart[ID_UART2].used == USED)
        {
            U2MODEbits.UARTEN   = 0;            /* UART disabled                  */
            U2MODEbits.USIDL    = 0;            /* Continue in Idle               */
            U2MODEbits.IREN     = 0;            /* No IR translation              */
            U2MODEbits.RTSMD    = 0;            /* Flow Control mode              */

            if (config.uart[ID_UART2].ctsRtsUsed == USED)
            {
                U2MODEbits.UEN  = 2;            /* TX,RX,CTS,RTS enabled          */
            }
            else
            {
                U2MODEbits.UEN  = 0;            /* TX,RX enabled, CTS,RTS not     */
            }

            if (config.uart[ID_UART2].polarityInverted == USED)
            {
                U2MODEbits.URXINV   = 1;            /* UxRX idle state is 1       */
                U2STAbits.UTXINV    = 1;            /* Idle state is 1            */
            }
            else
            {
                U2MODEbits.URXINV   = 0;            /* UxRX idle state is 1       */
                U2STAbits.UTXINV    = 0;            /* Idle state is 1            */
            }

            U2MODEbits.WAKE     = 0;            /* No Wake up                     */
            U2MODEbits.LPBACK   = 0;            /* No Loop Back                   */
            U2MODEbits.ABAUD    = 0;            /* No Autobaud                    */
            U2MODEbits.BRGH     = 1;            /* 16 clocks per bit period       */
            U2MODEbits.PDSEL    = 0;            /* 8bit, No Parity                */
            U2MODEbits.STSEL    = 0;            /* One Stop Bit                   */

            U2BRG               = u2BRGvalue;   /* Baudrate                       */

            U2STAbits.UTXISEL1  = 0;            /* Any character transferred      */
            U2STAbits.UTXISEL0  = 0;            /* Any character transferred      */
            U2STAbits.UTXBRK    = 0;            /* SyncBreak transmission disabled*/
            U2STAbits.UTXEN     = 0;            /* TX disabeld                    */
            U2STAbits.URXISEL   = 0;            /* IT on character recieved       */
            U2STAbits.ADDEN     = 0;            /* Address Detect Disabled        */

            U2MODEbits.UARTEN   = 1;            /* UART enabled                   */
            U2STAbits.UTXEN     = 1;            /* TX enabled                     */

            /* Reseting interrupts flags                                          */
            IFS1bits.U2RXIF = 0;
            IFS1bits.U2TXIF = 0;

            /* Interrupt priority level                                           */
            IPC7bits.U2RXIP = config.interrupts[ID_IRQ_UART2RX].priority;
            IPC7bits.U2TXIP = config.interrupts[ID_IRQ_UART2TX].priority;

            /* Enable/Disable interrupts                                          */
            IEC1bits.U2RXIE = config.interrupts[ID_IRQ_UART2RX].used;
            IEC1bits.U2TXIE = config.interrupts[ID_IRQ_UART2TX].used;

            IFS4bits.U2EIF = 0;
            IEC4bits.U2EIE = 1;
        }
    }
}


void configI2C (uint8_t busNumber)
{
    double i2c1BRGvalue = 0;

    if ((busNumber == I2C_BUS1) && (config.i2c[ID_I2C1].used == USED))
    {
        I2C1CONbits.I2CSIDL = 0;        /* Continue in idle mode              */
        I2C1CONbits.IPMIEN  = 0;        /* IPMI mode disabled                 */
        I2C1CONbits.A10M    = 0;        /* 7bit slave address                 */
        I2C1CONbits.DISSLW  = 1;        /* Slex rate control enabled          */
        I2C1CONbits.SMEN    = 0;        /* SMBUS disabled                     */

        /* Master Only                                                        */
        if (config.i2c[ID_I2C1].type == MASTER)
        {
            I2C1CONbits.ACKDT   = 0;    /* Send ACK during Acknowledge        */
            I2C1CONbits.ACKEN   = 0;    /* Acknowledge seq. not in progress   */
            I2C1CONbits.RCEN    = 0;    /* Receive seq. not in progress       */
            I2C1CONbits.PEN     = 0;    /* Stop condition not in progress     */
            I2C1CONbits.RSEN    = 0;    /* Start condition not in progr.      */
            I2C1CONbits.SEN     = 0;    /* Start condition not in progress    */

            /* Calculate baudrate                                             */
            /* PGD  = 130ns                                                   */
            /* BRG = Fcy*((1/Fsdl)-PGD) - 1                                   */
            i2c1BRGvalue    = ((double)(config.system.fcy)/(double)(I2C_FSCL));
            i2c1BRGvalue    -= ((double)((130.0)*(config.system.fcy))/(double)(1000000000.0));
            i2c1BRGvalue    -= (double)(1.0);

            I2C1BRG         = i2c1BRGvalue;

            /* Reseting interrupts flags                                      */
            IFS1bits.MI2C1IF    = 0;

            /* Interrupt priority level                                       */
            IPC4bits.MI2C1IP    = config.interrupts[ID_IRQ_I2C1].priority;

            /* Enable/Disable interrupts                                      */
            IEC1bits.MI2C1IE    = config.interrupts[ID_IRQ_I2C1].used;
        }

        /* Slave Only                                                         */
        else
        {
            I2C1CONbits.GCEN    = 0;    /* General call disabled              */
            I2C1MSK             = 0;    /* No mask                            */
            I2C1ADD             = config.i2c[ID_I2C1].address;

            /* Reseting interrupts flags                                      */
            IFS1bits.SI2C1IF    = 0;

            /* Interrupt priority level                                       */
            IPC4bits.SI2C1IP    = config.interrupts[ID_IRQ_I2C1].priority;
            
            /* Enable/Disable interrupts                                      */
            IEC1bits.SI2C1IE    = config.interrupts[ID_IRQ_I2C1].used;

            I2C1CONbits.STREN   = config.i2c[ID_I2C1].useClockStrech;
            I2C1CONbits.SCLREL  = config.i2c[ID_I2C1].useClockStrech;
        }

        /* Reseting buffers                                                   */
        I2C1RCV = 0x0000;
        I2C1TRN = 0x0000;

        I2C1CONbits.I2CEN       = 1;        /* Enable I2C1                    */
    }
}

void configSysIT (void)
{
    INTCON2bits.SWTRAP = 0;     /* Software trap is disabled                  */
}

