/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: interruptions.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 23 octobre 2014, 12:00
 *******************************************************************************
 *
 *
 ******************************************************************************/


#include "system.h"


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void init_system (void)
{
    init_clock();

    OVERFLOW_CODEUR[CODEUR_D] = PAS_D_OVERFLOW_CODEUR;
    OVERFLOW_CODEUR[CODEUR_G] = PAS_D_OVERFLOW_CODEUR;

    position[CODEUR_D].ancien = 0;
    position[CODEUR_G].ancien = 0;
    position[CODEUR_D].nouvelle = 0;
    position[CODEUR_G].nouvelle = 0;
    
    config_timer_10ms();
    config_timer_5ms();
    config_timer_90s();

    ConfigMapping ();
    ConfigPorts ();
    ConfigQEI ();
    ConfigInterrupt ();
    ConfigPWM();
    //ConfigADC();

    InitUART(UART_XBEE, 115200);
    InitUART(UART_AX12, 500000);

    DETECTION = OFF;
    EVITEMENT_ADV_AVANT = OFF;
    EVITEMENT_ADV_ARRIERE = OFF;
    STRATEGIE_EVITEMENT = STOP;
    FLAG_ACTION = NE_RIEN_FAIRE;
    ETAT_AUTOM = NE_RIEN_FAIRE;
    COULEUR = JAUNE;

    TIMER_5ms = ACTIVE;
    TIMER_10ms = ACTIVE;

#ifdef PETIT_ROBOT
    TRISCbits.TRISC2 = 0;
#endif
#ifdef GROS_ROBOT
    TRISAbits.TRISA3 = 0;
#endif
    
    init_flag();
}



/******************************************************************************/
/***************************** Configurations Timers **************************/
/******************************************************************************/
void config_timer_5ms()
{
    //Timer de 5,00373 ms

    TIMER_5ms = DESACTIVE;
    T1CONbits.TCS = 0;          //clock sur FCY
    T1CONbits.TGATE = 0;        //Mode Gate désactivé
    T1CONbits.TCKPS = 0b11;     //Divise la fréquence par 256 (prescaler 1:256)
    TMR1 = 0x00;                //Reset du timer

    // FOSC = 80 MhZ, FCY = FOSC/2 = 40 MHz
    // T = 0, 250 ns
    // Presacler 1 : 256 => T = 6,4 µs
    // 782 * 6,4µs = 5 ms
    PR1 = 782;                 //décompte de 5ms

    //IPC0bits.T1IP = 0x02;       //priorité à 2  -> définit dans la table des vecteurs
    FLAG_TIMER_5ms = 0;         //Clear flag interrupt timer
    IEC0bits.T1IE = 1;          //Enable Timer1 interrupt

    //TIMER_5ms = ACTIVE;
}

void config_timer_10ms() //Timer autom
{
    //Timer de 10,00105 ms

    TIMER_10ms = DESACTIVE;
    T4CONbits.TCS = 0;          //clock sur FCY
    T4CONbits.T32 = 0;          //Timer 16 bits
    T4CONbits.TGATE = 0;        //Mode Gate désactivé
    T4CONbits.TCKPS = 0b11;     //Divise la fréquence par 256 (prescaler 1:256)
    TMR4 = 0x00;                //Reset du timer

    // FOSC = 80 MhZ, FCY = FOSC/2 = 40 MHz
    // T = 0, 250 ns
    // Presacler 1 : 256 => T = 6,4 µs
    // 1563 * 6,4 µs = 10 ms
    PR4 = 1563;                 //décompte de 10ms

    //IPC6bits.T4IP = 0x03;       //priorité à 3 --> définit dans la table des interrupt
    FLAG_TIMER_10ms = 0;        //Clear flag interrupt timer
    IEC1bits.T4IE = 1;          //Enable Timer4 interrupt

    //TIMER_10ms = ACTIVE;
}


void config_timer_debug() //int periode_ms
{
    TIMER_DEBUG = DESACTIVE;
    T5CONbits.TCS = 0;          //clock sur FCY
    T5CONbits.TGATE = 0;        //Mode Gate désactivé
    T5CONbits.TCKPS = 0b11;     //Divise la fréquence par 256 (prescaler 1:256)
    TMR4 = 0x00;                //Reset du timer

    PR4 = 31256 ;                 //décompte de 200ms

    //IPC6bits.T4IP = 0x03;       //priorité à 3 --> définit dans la table des interrupt
    FLAG_TIMER_DEBUG = 0;        //Clear flag interrupt timer
    IEC1bits.T5IE = 1;          //Enable Timer4 interrupt
}

void config_timer_90s()
{
    //Timer de 90,000 004 secondes

    T3CONbits.TON = 0;          //On arrête le timer 3
    T2CONbits.TON = 0;          //On arrête le Timer 2
    T2CONbits.T32 = 1;          //On choisit un Timer 32 bits
    T2CONbits.TCS = 0;          //On se synchro sur FCY = 40 MHz
    T2CONbits.TGATE = 0;        //Desactive le mode Gate
    T2CONbits.TCKPS = 0b11;     //Precaler = 1:256

    TMR3 = 0x00;                //On remet le TIMER3 à 0
    TMR2 = 0x00;                //On remet le Timer2 à 0

    //On décompte 14 065 514 pour faire 90 secondes
    PR3 = 0x00D6;               //adresse haute
    PR2 = 0x9F6A;               //adresse basse

    //IPC2bits.T3IP = 0x01;       //Interruption sur ptiotité 1  -> définit dans la table des interrupt
    FLAG_TIMER_90s = 0;         //On clear le flag du timer
    IEC0bits.T3IE = 1;          //On enable l'interrup
}

void start_timer_90s (void)
{
    TIMER_90s = ACTIVE;         //on active le timer de 90 secondes
}

/******************************************************************************/
/********************************** Init Clock ********************************/
/******************************************************************************/

void init_clock(void)
{
    //Tunage de la fréquence : Ftune = 8,0056625
    OSCTUNbits.TUN = 0;        //SEMBLE NE PAS FONCTIONNER ....

    //New Osc = FRC sans PLL
    OSCCONbits.NOSC = 0b000;
    OSCCONbits.OSWEN = 1;
    while(OSCCONbits.OSWEN != 0);


    // Configure PLL prescaler, PLL postscaler, PLL divisor
    // Fext = Fin * M / (N1 * N2)
    // F =  80,017 142 MHz

    PLLFBD = 150; // M = 152
    CLKDIVbits.PLLPOST= 0b00; // N2 = 2 
    CLKDIVbits.PLLPRE= 5; // N1 = 7

    CLKDIVbits.DOZE = 0b000; //FRC = 1:1 FRC

    //On switch sur la nouvelle clock avec PLL
    OSCCONbits.NOSC = 0b001;
    OSCCONbits.OSWEN = 1;
    while(OSCCONbits.OSWEN != 0);

    // Wait for PLL to lock
    while(OSCCONbits.LOCK!=1);
}

/******************************************************************************/
/************************ Mapping des pins de la carte ************************/
/******************************************************************************/


void ConfigMapping (void)
{
    #ifdef CARTE_V1
        // Mapping UART1 : XBee
	_U1RXR	= 0x0E;		// IN	: UART1 RX sur RP14
	_RP15R	= 0x03;		// OUT	: UART1 TX sur RP15

	// Mapping UART2 : AX12
	_U2RXR	= 0x05;         // IN	: UART2 RX sur RP5
	_RP6R	= 0x05;         // OUT	: UART2 TX sur RP6

        // Mapping QEI1 : Codeur Droit
	_QEB1R	= 0x19;         // IN	: QEB1 sur RP25
	_QEA1R	= 0x18;         // IN	: QEA1 sur RP24

	// Mapping QEI2 : Codeur Gauche
	_QEB2R	= 0x17;		// QEB2 sur RP23
	_QEA2R	= 0x16;		// QEA2 sur RP22
    #endif

    #ifdef CARTE_V2
        // Mapping UART1 : XBee
	_U1RXR	= 0x05;		// IN	: UART1 RX sur RP5
	_RP6R	= 0x03;		// OUT	: UART1 TX sur RP6

	// Mapping UART2 : AX12
	_U2RXR	= 0x03;         // IN	: UART2 RX sur RP3
	_RP2R	= 0x05;         // OUT	: UART2 TX sur RP2

        // Mapping QEI1 : Codeur Droit
	_QEB1R	= 0x18;         // IN	: QEB1 sur RP24
	_QEA1R	= 0x19;         // IN	: QEA1 sur RP25

	// Mapping QEI2 : Codeur Gauche
	_QEB2R	= 0x16;		// QEB2 sur RP22
	_QEA2R	= 0x17;		// QEA2 sur RP23
    #endif

	// Temporisation
	delay_ms (50);
}


/*********************************************************/
void ConfigPorts (void)
{
	// Desactivation du module CAN
	_ADON		= 0;

	// Desactivation des entrees analogiques
	_PCFG0		= 1;
	_PCFG1		= 1;
	_PCFG2		= 1;
	_PCFG3		= 1;
	_PCFG4		= 1;
	_PCFG5		= 1;
	_PCFG6		= 1;
	_PCFG7		= 1;
	_PCFG8		= 1;

	// Desactivation PWM
	_PEN1L		= 0;
	_PEN1H		= 0;
	_PEN2L		= 0;
	_PEN2H		= 0;
	_PEN3L		= 0;
	_PEN3H		= 0;


    #ifdef CARTE_V1
	//****************
	// PORT x
	//****************
	//_CNxIE  : interrup sur broche	| _CN6PUE : pull-up sur broche
	// Config PORTA
	_TRISA0		= 1;	_CN2IE	= 0;	_CN2PUE		= 0;	// IN  : (ANALOG) Courant_G (AN0)
	_TRISA1		= 1;	_CN3IE	= 0;	_CN3PUE		= 0;	// IN  : (ANALOG) Courant_D (AN1)
        _TRISA2         = 1;                                            // IN  : (DIGIT) capteur 1
	_TRISA3		= 1;	_CN29IE	= 0;	_CN29PUE	= 0;	// IN  : (DIGIT) capteur 2
	_TRISA4		= 1;	_CN0IE	= 0;	_CN0PUE		= 0;	// IN  : (DIGIT) capteur 3
        _TRISA7		= 0;						// OUT : (DIGIT) XBEE RSET
	_TRISA8		= 1;						// IN  : (DIGIT) capteur 4
	_TRISA9		= 1;						// IN  : (DIGIT) capteur 5
	_TRISA10	= 0;						// OUT : (DIGIT) Commande Alim AX12

	// Config PORTB
	_TRISB0		= 0;	_CN4IE	= 0;	_CN4PUE		= 0;	// RES : PGED1 pour programmation
	_TRISB1		= 0;	_CN5IE	= 0;	_CN5PUE		= 0;	// RES : PGEC1 pour programmation
	_TRISB2		= 0;	_CN6IE	= 0;	_CN6PUE		= 0;	// OUT : (DIGIT)  Photomos 1
	_TRISB3		= 0;	_CN7IE	= 0;	_CN7PUE		= 0;	// OUT : (DIGIT)  Photomos 2
	_TRISB4		= 1;	_CN1IE	= 0;	_CN1PUE		= 0;	// IN  : Jack
	_TRISB5		= 1;	_CN27IE	= 0;	_CN27PUE	= 0;	// IN  : RX AX12
	_TRISB6		= 0;	_CN24IE	= 0;	_CN24PUE	= 0;	// OUT : TX AX12
	_TRISB7		= 0;	_CN23IE	= 0;	_CN23PUE	= 0;	// OUT : DIR UART AX12
	_TRISB8		= 0;	_CN22IE	= 0;	_CN22PUE	= 0;	// RES : SCL1
	_TRISB9		= 0;	_CN21IE	= 0;	_CN21PUE	= 0;	// RES : SDA1
	_TRISB10	= 0;	_CN16IE	= 0;	_CN16PUE	= 0;	// OUT : PWM1H3 : moteur gauche
	_TRISB11	= 0;	_CN15IE	= 0;	_CN15PUE	= 0;	// OUT : SENS   : moteur gauche
	_TRISB12	= 0;	_CN14IE	= 0;	_CN14PUE	= 0;	// OUT : PWM1H2 : moteur droit
	_TRISB13	= 0;	_CN13IE	= 0;	_CN13PUE	= 0;	// OUT : SENS   : moteur droit
	_TRISB14	= 1;	_CN12IE	= 0;	_CN12PUE	= 0;	// IN  : RX XBEE
	_TRISB15	= 0;	_CN11IE	= 0;	_CN11PUE	= 0;	// OUT : TX XBEE

	// Config PORTC
	_TRISC0		= 1;	_CN8IE	= 0;	_CN8PUE		= 0;	// IN  : (ANALOG/DIGIT)  capteur 6 (AN6)
	_TRISC1		= 1;	_CN9IE	= 0;	_CN9PUE		= 0;	// IN  : (ANALOG/DIGIT)  capteur 7 (AN7)
	_TRISC2		= 1;	_CN10IE	= 0;	_CN10PUE	= 0;	// IN  : (ANALOG/DIGIT)  capteur 8 (AN8)
	_TRISC3		= 1;	_CN28IE	= 0;	_CN28PUE	= 0;	// IN  : (DIGIT) : capteur 9
	_TRISC4		= 1;	_CN25IE	= 0;	_CN25PUE	= 0;	// IN  : (DIGIT) : capteur 10
	_TRISC5		= 0;	_CN26IE	= 0;	_CN26PUE	= 0;	// OUT : (DIGIT) : LED Debug
	_TRISC6		= 1;	_CN18IE	= 0;	_CN18PUE	= 0;	// IN  : QEA2
	_TRISC7		= 1;	_CN17IE	= 0;	_CN17PUE	= 0;	// IN  : QEB2
	_TRISC8		= 1;	_CN20IE	= 0;	_CN20PUE	= 0;	// IN  : QEA1
	_TRISC9		= 1;	_CN19IE	= 0;	_CN19PUE	= 0;	// IN  : QEB1

    #endif

    #ifdef CARTE_V2
	//****************
	// PORT x
	//****************
	//_CNxIE  : interrup sur broche	| _CN6PUE : pull-up sur broche
	// Config PORTA
	_TRISA0		= 0;	_CN2IE	= 0;	_CN2PUE		= 0;	// OUT : (DIGIT) : Inhibit AX12
	_TRISA1		= 0;	_CN3IE	= 0;	_CN3PUE		= 0;	// OUT : (DIGIT) : DIR_UART_AX12
        _TRISA2         = 1;                                            // IN  : (DIGIT) : Capteur 1
	_TRISA3		= 1;	_CN29IE	= 0;	_CN29PUE	= 0;	// IN  : (DIGIT) : capteur 2
	_TRISA4		= 1;	_CN0IE	= 0;	_CN0PUE		= 0;	// IN  : (DIGIT) : capteur 3
        _TRISA7		= 0;						// OUT : PWM Y
	_TRISA8		= 1;						// IN  : (DIGIT) : capteur 4
	_TRISA9		= 1;						// IN  : (DIGIT) : couleur
	_TRISA10	= 0;						// OUT : (DIGIT) : Commande Alim AX12

	// Config PORTB
	_TRISB0		= 0;	_CN4IE	= 0;	_CN4PUE		= 0;	// RES : PGED1 pour programmation
	_TRISB1		= 0;	_CN5IE	= 0;	_CN5PUE		= 0;	// RES : PGEC1 pour programmation
	_TRISB2		= 0;	_CN6IE	= 0;	_CN6PUE		= 0;	// INT : (UART)  : TX AX12
	_TRISB3		= 1;	_CN7IE	= 0;	_CN7PUE		= 0;	// OUT : (UART)  : RX AX12
	_TRISB4		= 1;	_CN1IE	= 0;	_CN1PUE		= 0;	// IN  : (DIGIT) : Capteur 5
	_TRISB5		= 1;	_CN27IE	= 0;	_CN27PUE	= 0;	// IN  : (UART)  : RX XBEE
	_TRISB6		= 0;	_CN24IE	= 0;	_CN24PUE	= 0;	// OUT : (UART)  : TX XBEE
	_TRISB7		= 0;	_CN23IE	= 0;	_CN23PUE	= 0;	// OUT : (DIGIT) : RESET XBEE
	_TRISB8		= 0;	_CN22IE	= 0;	_CN22PUE	= 0;	// RES : (I²C)   : SCL1
	_TRISB9		= 0;	_CN21IE	= 0;	_CN21PUE	= 0;	// RES : (I²C)   : SDA1
	_TRISB10	= 0;	_CN16IE	= 0;	_CN16PUE	= 0;	// OUT : (DIGIT) : sens moteur gauche
	_TRISB11	= 0;	_CN15IE	= 0;	_CN15PUE	= 0;	// OUT : (PWM)   : PWM1L3 (moteur gauche)
	_TRISB12	= 0;	_CN14IE	= 0;	_CN14PUE	= 0;	// OUT : (DIGIT) : sens moteur droit
	_TRISB13	= 0;	_CN13IE	= 0;	_CN13PUE	= 0;	// OUT : (PWM)   : PWM1L2 (moteur droit)
	_TRISB14	= 0;	_CN12IE	= 0;	_CN12PUE	= 0;	// OUT : (DIGIT) : sens moteur X
	_TRISB15	= 0;	_CN11IE	= 0;	_CN11PUE	= 0;	// OUT : (PWM)   : PWM1H1 (moteur X)

	// Config PORTC
	_TRISC0		= 1;	_CN8IE	= 0;	_CN8PUE		= 0;	// IN  : (ANALOG/DIGIT) : capteur 6 (AN6)
	_TRISC1		= 1;	_CN9IE	= 0;	_CN9PUE		= 0;	// IN  : (ANALOG/DIGIT) : capteur 7 (AN7)
	_TRISC2		= 1;	_CN10IE	= 0;	_CN10PUE	= 0;	// IN  : (ANALOG/DIGIT) : capteur 8 (AN8)
	_TRISC3		= 1;	_CN28IE	= 0;	_CN28PUE	= 0;	// IN  : (DIGIT) : Jack
	_TRISC4		= 1;	_CN25IE	= 0;	_CN25PUE	= 0;	// IN  : (DIGIT) : Strat
	_TRISC5		= 0;	_CN26IE	= 0;	_CN26PUE	= 0;	// OUT : (DIGIT) : LED Debug
	_TRISC6		= 1;	_CN18IE	= 0;	_CN18PUE	= 0;	// IN  : QEA2
	_TRISC7		= 1;	_CN17IE	= 0;	_CN17PUE	= 0;	// IN  : QEB2
	_TRISC8		= 1;	_CN20IE	= 0;	_CN20PUE	= 0;	// IN  : QEA1
	_TRISC9		= 1;	_CN19IE	= 0;	_CN19PUE	= 0;	// IN  : QEB1

    #endif

	//****************
	// INITIALISATION
	//****************

	// Init PORTA
	_LATA0		= 0;	// Valeur initiale a  l'etat bas
	_LATA1		= 0;	// Valeur initiale a  l'etat bas
	_LATA3		= 0;	// Valeur initiale a  l'etat bas
	_LATA4		= 0;	// Valeur initiale a  l'etat bas
	_LATA7		= 0;	// Valeur initiale a  l'etat bas
	_LATA8		= 0;	// Valeur initiale a  l'etat bas
	_LATA9		= 0;	// Valeur initiale a  l'etat bas
	_LATA10		= 0;	// Valeur initiale a  l'etat bas

	// Init PORTB
	_LATB0		= 0;	// Valeur initiale a  l'etat bas
	_LATB1		= 0;	// Valeur initiale a  l'etat bas
	_LATB2		= 0;	// Valeur initiale a  l'etat bas
	_LATB3		= 0;	// Valeur initiale a  l'etat bas
	_LATB4		= 0;	// Valeur initiale a  l'etat bas
	_LATB5		= 0;	// Valeur initiale a  l'etat bas
	_LATB6		= 0;	// Valeur initiale a  l'etat bas
	_LATB7		= 0;	// Valeur initiale a  l'etat bas
	_LATB8		= 0;	// Valeur initiale a  l'etat bas
	_LATB9		= 0;	// Valeur initiale a  l'etat bas
	_LATB10		= 0;	// Valeur initiale a  l'etat bas
	_LATB11		= 0;	// Valeur initiale a  l'etat bas
	_LATB12		= 0;	// Valeur initiale a  l'etat bas
	_LATB13		= 0;	// Valeur initiale a  l'etat bas
	_LATB14		= 0;	// Valeur initiale a  l'etat bas
	_LATB15		= 0;	// Valeur initiale a  l'etat bas

	// Init PORTC
	_LATC0		= 0;	// Valeur initiale a  l'etat bas
	_LATC1		= 0;	// Valeur initiale a  l'etat bas
	_LATC2		= 0;	// Valeur initiale a  l'etat bas
	_LATC3		= 0;	// Valeur initiale a  l'etat bas
	_LATC4		= 0;	// Valeur initiale a  l'etat bas
	_LATC5		= 0;	// Valeur initiale a  l'etat bas
	_LATC6		= 0;	// Valeur initiale a  l'etat bas
	_LATC7		= 0;	// Valeur initiale a  l'etat bas
	_LATC8		= 0;	// Valeur initiale a  l'etat bas
	_LATC9		= 0;	// Valeur initiale a  l'etat bas
}

/******************************************************************************/
/********************* Configurations Des Interruptions ***********************/
/******************************************************************************/


void ConfigInterrupt (void)
{
	// INTCON2
	INTCON2bits.ALTIVT	= 0;			// Table de Vecteur standard
	INTCON2bits.INT0EP	= 0;			// Interruption INT0 sur front montant


	//****************
	// FLAG
	//****************
	// IFS0 : Remise a  zero des Flags
	IFS0bits.DMA1IF		= 0;			//
	IFS0bits.AD1IF		= 0;			//
	IFS0bits.U1TXIF		= 0;			//
	IFS0bits.U1RXIF		= 0;			//
	IFS0bits.SPI1IF		= 0;			//
	IFS0bits.SPI1EIF	= 0;			//
	IFS0bits.T3IF		= 0;			//
	IFS0bits.T2IF		= 0;			//
	IFS0bits.OC2IF		= 0;			//
	IFS0bits.IC2IF		= 0;			//
	IFS0bits.DMA0IF		= 0;			//
	IFS0bits.T1IF		= 0;			//
	IFS0bits.OC1IF		= 0;			//
	IFS0bits.IC1IF		= 0;			//
	IFS0bits.INT0IF		= 0;			//

	// IFS1 : Remise a  zero des Flags
	IFS1bits.U2TXIF		= 0;			//
	IFS1bits.U2RXIF		= 0;			//
	IFS1bits.INT2IF		= 0;			//
	IFS1bits.T5IF		= 0;			//
	IFS1bits.T4IF		= 0;			//
	IFS1bits.OC4IF		= 0;			//
	IFS1bits.OC3IF		= 0;			//
	IFS1bits.DMA2IF		= 0;			//
	IFS1bits.IC8IF		= 0;			//
	IFS1bits.IC7IF		= 0;			//
	IFS1bits.INT1IF		= 0;			//
	IFS1bits.CNIF		= 0;			//
	IFS1bits.CMIF		= 0;			//
	IFS1bits.MI2C1IF	= 0;			//
	IFS1bits.SI2C1IF	= 0;			//

	// IFS2 : Remise a  zero des Flags
	IFS2bits.DMA4IF		= 0;			//
	IFS2bits.PMPIF		= 0;			//
	IFS2bits.DMA3IF		= 0;			//
	IFS2bits.C1IF		= 0;			//
	IFS2bits.C1RXIF		= 0;			//
	IFS2bits.SPI2IF		= 0;			//
	IFS2bits.SPI2EIF	= 0;			//

	// IFS3 : Remise a  zero des Flags
	IFS3bits.FLTA1IF	= 0;			//
	IFS3bits.RTCIF		= 0;			//
	IFS3bits.DMA5IF		= 0;			//
	IFS3bits.QEI1IF		= 0;			//
	IFS3bits.PWM1IF		= 0;			//

	// IFS4 : Remise a  zero des Flags
	IFS4bits.DAC1LIF	= 0;			//
	IFS4bits.DAC1RIF	= 0;			//
	IFS4bits.QEI2IF		= 0;			//
	IFS4bits.FLTA2IF	= 0;			//
	IFS4bits.PWM2IF		= 0;			//
	IFS4bits.C1TXIF		= 0;			//
	IFS4bits.DMA7IF		= 0;			//
	IFS4bits.DMA6IF		= 0;			//
	IFS4bits.CRCIF		= 0;			//
	IFS4bits.U2EIF		= 0;			//
	IFS4bits.U1EIF		= 0;			//


	//****************
	// Priorites
	//****************
	// IPC0
	IPC0bits.T1IP		= PRIO_INTER_TIMER1;		//
	IPC0bits.OC1IP		= 0x00;				//
	IPC0bits.IC1IP		= 0x00;				//
	IPC0bits.INT0IP		= 0x00;                         //Pas de INT0

	// IPC1
	IPC1bits.T2IP		= PRIO_INTER_TIMER2;		// Timer2
	IPC1bits.OC2IP		= 0x00;				//
	IPC1bits.IC2IP		= 0x00;				//
	IPC1bits.DMA0IP		= 0x00;				//

	// IPC2
	IPC2bits.U1RXIP		= PRIO_INTER_UART1_RX;		// UART1 RX
	IPC2bits.SPI1IP		= 0x00;				//
	IPC2bits.SPI1EIP	= 0x00;				//
	IPC2bits.T3IP		= PRIO_INTER_TIMER3;		// Timer 3

	// IPC3
	IPC3bits.DMA1IP		= 0x00;				//
	IPC3bits.AD1IP		= 0x00;				//
	IPC3bits.U1TXIP		= 0x00;				//

	// IPC4
	IPC4bits.CNIP		= 0x00;				//
	IPC4bits.MI2C1IP	= PRIO_INTER_I2C_MAITRE;	// Maitre I2C1
	IPC4bits.SI2C1IP	= PRIO_INTER_I2C_ESCLAVE; 	// Esclave I2C1

	// IPC5
	IPC5bits.IC8IP		= 0x00;				//
	IPC5bits.IC7IP		= 0x00;				//
	IPC5bits.INT1IP		= 0x00;				//

	// IPC6
	IPC6bits.T4IP		= PRIO_INTER_TIMER4;		// Timer 4
	IPC6bits.OC4IP		= 0x00;				//
	IPC6bits.OC3IP		= 0x00;				//
	IPC6bits.DMA2IP		= 0x00;				//

	// IPC7
	IPC7bits.U2TXIP		= PRIO_INTER_UART2_TX;
	IPC7bits.U2RXIP		= PRIO_INTER_UART2_RX;		// UART2 RX
	IPC7bits.INT2IP		= 0x00;				//
	IPC7bits.T5IP		= PRIO_INTER_TIMER5;		//

	// IPC8
	IPC8bits.C1IP		= 0x00;				//
	IPC8bits.C1RXIP		= 0x00;				//
	IPC8bits.SPI2IP		= 0x00;				//
	IPC8bits.SPI2EIP	= 0x00;				//

	// IPC9
	IPC9bits.DMA3IP		= 0x00;			

	// IPC10
	// PAS de IPC10

	// IPC11
	IPC11bits.DMA4IP	= 0x00;
	IPC11bits.PMPIP		= 0x00;

	// IPC12
	// PAS de IPC12

	// IPC13
	// PAS de IPC13

	// IPC14
	IPC14bits.QEI1IP	= PRIO_INTER_QEI1;          // Overflow sur QEI1 -> Codeur Droit
	IPC14bits.PWM1IP	= 0x00;                     //

	// IPC15
	IPC15bits.FLTA1IP	= 0x00;                     //
	IPC15bits.RTCIP		= 0x00;                     //
	IPC15bits.DMA5IP	= 0x00;                     //

	// IPC16
	IPC16bits.U2EIP		= 0x00;                     //
	IPC16bits.U1EIP		= 0x00;                     //
	IPC16bits.CRCIP		= 0x00;                     //

	// IPC17
	IPC17bits.C1TXIP	= 0x00;                     //
	IPC17bits.DMA7IP	= 0x00;                     //
	IPC17bits.DMA6IP	= 0x00;                     //

	// IPC18
	IPC18bits.QEI2IP	= PRIO_INTER_QEI2;          // Overflow sur QEI2 -> Codeur Gauche
	IPC18bits.FLTA2IP	= 0x00;                     //
	IPC18bits.PWM2IP	= 0x00;                     //

	// IPC19
	IPC19bits.DAC1LIP	= 0x00;                     //
	IPC19bits.DAC1RIP	= 0x00;                     //


	//****************
	// Activation
	//****************
	// IEC0
	IEC0bits.DMA1IE		= 0;                            // NC
	IEC0bits.AD1IE		= 0;                    	// NC
	IEC0bits.U1TXIE		= 0;                            // NC
	IEC0bits.U1RXIE		= ACTIV_INTER_UART1_RX;         // Interruption RX sur UART1
	IEC0bits.SPI1IE		= 0;                            // NC
	IEC0bits.SPI1EIE	= 0;            		// NC
	IEC0bits.T3IE		= ACTIV_INTER_TIMER3;		// Interruption sur Timer3
	IEC0bits.T2IE		= ACTIV_INTER_TIMER2;		// Interruption sur Timer2
	IEC0bits.OC2IE		= 0;				// NC
	IEC0bits.IC2IE		= 0;				// NC
	IEC0bits.DMA0IE		= 0;				// NC
	IEC0bits.T1IE		= ACTIV_INTER_TIMER1;		// Interruption Timer1
	IEC0bits.OC1IE		= 0;				// NC
	IEC0bits.IC1IE		= 0;				// NC
	IEC0bits.INT0IE		= 0;                            // NC

	// IEC1
	IEC1bits.U2TXIE		= ACTIV_INTER_UART2_TX;         // Interruption TX sur UART2
	IEC1bits.U2RXIE		= ACTIV_INTER_UART2_RX;		// Interruption RX sur UART2
	IEC1bits.INT2IE		= 0;                    	// NC
	IEC1bits.T5IE		= ACTIV_INTER_TIMER5;    	// Interruption sur Timer5
	IEC1bits.T4IE		= ACTIV_INTER_TIMER4;		// Interruption sur Timer4
	IEC1bits.OC4IE		= 0;                            // NC
	IEC1bits.OC3IE		= 0;            		// NC
	IEC1bits.DMA2IE		= 0;				// NC
	IEC1bits.IC8IE		= 0;				// NC
	IEC1bits.IC7IE		= 0;				// NC
	IEC1bits.INT1IE		= 0;				// NC
	IEC1bits.CNIE		= 0;				// NC
	IEC1bits.MI2C1IE	= ACTIV_INTER_I2C_MAITRE;	// Interruption I2C MAITRE
	IEC1bits.SI2C1IE	= ACTIV_INTER_I2C_ESCLAVE;	// Interruption I2C ESCLAVE

	// IEC2
	IEC2bits.DMA4IE		= 0;				// NC
	IEC2bits.PMPIE		= 0;				// NC
	IEC2bits.DMA3IE		= 0;				// NC
	IEC2bits.C1IE		= 0;				// NC
	IEC2bits.C1RXIE		= 0;				// NC
	IEC2bits.SPI2IE		= 0;				// NC
	IEC2bits.SPI2EIE	= 0;				// NC

	// IEC3
	IEC3bits.FLTA1IE	= 0;				// NC
	IEC3bits.RTCIE		= 0;				// NC
	IEC3bits.DMA5IE		= 0;				// NC
	IEC3bits.QEI1IE		= ACTIV_INTER_QEI1;             // Interrupt Codeur Droit
	IEC3bits.PWM1IE		= 0;            		// NC

	// IEC4
	IEC4bits.DAC1LIE	= 0;				// NC
	IEC4bits.DAC1RIE	= 0;				// NC
	IEC4bits.QEI2IE		= ACTIV_INTER_QEI2;             // Interrupt Codeur Gauche
	IEC4bits.FLTA2IE	= 0;				// NC
	IEC4bits.PWM2IE		= 0;				// NC
	IEC4bits.C1TXIE		= 0;				// NC
	IEC4bits.DMA7IE		= 0;				// NC
	IEC4bits.DMA6IE		= 0;				// NC
	IEC4bits.CRCIE		= 0;				// NC
	IEC4bits.U2EIE		= 0;				// NC
	IEC4bits.U1EIE		= 0;				// NC
}

/******************************************************************************/
/**************************** Configurations   ADC ****************************/
/******************************************************************************/


void ConfigADC (void)
{
#ifdef CARTE_V2
    if (CAPTEUR1_ANALOGIQUE == ANALOGIQUE ||CAPTEUR2_ANALOGIQUE == ANALOGIQUE || CAPTEUR3_ANALOGIQUE == ANALOGIQUE)
    {
      // Entrée digitale / Analogique
        AD1PCFGLbits.PCFG0 = 0;
        AD1PCFGLbits.PCFG1 = 0;
        AD1PCFGLbits.PCFG2 = 0;
        AD1PCFGLbits.PCFG3 = 0;
        AD1PCFGLbits.PCFG4 = 0;
        AD1PCFGLbits.PCFG5 = 0;
        AD1PCFGLbits.PCFG6 = CAPTEUR1_ANALOGIQUE;
        AD1PCFGLbits.PCFG7 = CAPTEUR2_ANALOGIQUE;
        AD1PCFGLbits.PCFG8 = CAPTEUR3_ANALOGIQUE;

        /*AD1CHS0bits.CH0NB = 0;                                  // VREF- = 0V
        AD1CHS0bits.CH0NA = 0;                                  // VREF- = 0V

        if (CAPTEUR1_ANALOGIQUE == ANALOGIQUE)
        {
            AD1CHS0bits.CH0SB = 0b00110;
            AD1CHS0bits.CH0SA = 0b00110;
        }
        else if (CAPTEUR2_ANALOGIQUE == ANALOGIQUE)
        {
            AD1CHS0bits.CH0SB = 0b00111;
            AD1CHS0bits.CH0SA = 0b00111;
        }
        else if (CAPTEUR3_ANALOGIQUE == ANALOGIQUE)
        {
            AD1CHS0bits.CH0SB = 0b 1000;
            AD1CHS0bits.CH0SA = 0b 1000;
        }*/

        AD1CON2bits.VCFG = 0;                                   // VDD, VSS
        AD1CON2bits.SMPI = 0;                                   // Incréement du DMA à chaque échantillon
        AD1CON2bits.BUFM = 0;                                   // On commence à 0x0
        AD1CON2bits.ALTS = 0;                                   //
        AD1CON2bits.CSCNA = 1;                                   //

        AD1CON3bits.ADRC = 0;
        AD1CON3bits.ADCS = 254;                                 // TAD = 25 * TCY

        AD1CON1bits.ADDMABM = 0;                                // ?
        AD1CON1bits.AD12B = 1;                                  // 1 channel de 12 bits
        AD1CON1bits.FORM = 0b00;                                // Fractionnaire non signé Dout = dddd dddd dddd 0000
        AD1CON1bits.SSRC = 0b111;                               // Auto convert
        AD1CON1bits.ASAM = 1;                                   // Auto reload


        AD1CON3bits.SAMC = 0;//0b11111;                             // Sample Time = 31 TAD;

        AD1CON4bits.DMABL = 0;                                  // 1 mot dans le buffer

        AD1CON1bits.ADON = 1;                                   // ADC ON

    }
#endif
    
}