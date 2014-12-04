/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: config.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_CONFIG


	
/** I N C L U D E S **************************************/
	#include "../usr.h"


	
/** F O N C T I O N S ************************************/
void ConfigMapping (void)
{
	// Deverouillage mapping : __builtin_write_OSCCONL();
	//OSCCONL = 0x46;
	//OSCCONL = 0x57;
	//_IOLOCK = 0;
	
	
	// Mapping UART1 : XBee
	_U1RXR	= 0x19;		// IN	: UART1 RX sur RP25
	_RP24R	= 0x03;		// OUT	: UART1 TX sur RP24
	_RP18R	= 0x04;		// UART1 RTS sur RP18
	_U1CTSR	= 0x16;		// UART1 CTS sur RP22
			
	// Mapping UART2 : AX12
	_U2RXR	= 0x17;		// IN	: UART2 RX sur RP23
	_RP11R	= 0x05;		// OUT	: UART2 TX sur RP11
	
	// Mapping QEI1
	_QEB1R	= 0x05; 	// IN	: QEB1 sur RP5
	_QEA1R	= 0x04;		// IN	: QEA1 sur RP4
	_INDX1R	= 0x06;		// IN	: INDEX1 sur RP6
	
	// Mapping QEI2
	_QEB2R	= 0x14;		// QEB2 sur RP20
	_QEA2R	= 0x13;		// QEA2 sur RP19
	_INDX2R	= 0x15;		// INDEX2 sur RP21
	
	// Temporisation
	Delay_Xms (50);
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



	//****************
	// PORT x
	//****************
	//_CNxIE  : interrup sur broche	| _CN6PUE : pull-up sur broche
	// Config PORTA
	_TRISA0		= 1;	_CN2IE	= 0;	_CN2PUE		= 0;	// IN  : (ANALOG) Imot1 (AN0)
	_TRISA1		= 1;	_CN3IE	= 0;	_CN3PUE		= 0;	// IN  : (ANALOG) Imot2 (AN1)
	_TRISA3		= 0;	_CN29IE	= 0;	_CN29PUE	= 0;	// OUT : (DIGIT) commande alimentation AX12
	_TRISA4		= 0;	_CN0IE	= 0;	_CN0PUE		= 0;	// OUT : (DIGIT) controle AX12
	_TRISA7		= 0;										// OUT : (DIGIT) LED debug 1
	_TRISA8		= 0;										// OUT : (DIGIT) LED debug 2
	_TRISA9		= 1;										// IN  : (DIGIT)  capteur 5
	_TRISA10	= 1;										// IN  : (DIGIT)  capteur 6
	
	// Config PORTB
	_TRISB0		= 0;	_CN4IE	= 0;	_CN4PUE		= 0;	// RES : PGED1 pour programmation
	_TRISB1		= 0;	_CN5IE	= 0;	_CN5PUE		= 0;	// RES : PGEC1 pour programmation
	_TRISB2		= 1;	_CN6IE	= 0;	_CN6PUE		= 0;	// IN  : (ANALOG/DIGIT)  capteur 1 (AN4)
	_TRISB3		= 1;	_CN7IE	= 0;	_CN7PUE		= 0;	// IN  : (ANALOG/DIGIT)  capteur 2 (AN5)
	_TRISB4		= 1;	_CN1IE	= 0;	_CN1PUE		= 0;	// IN  : QEIA1
	_TRISB5		= 1;	_CN27IE	= 0;	_CN27PUE	= 0;	// IN  : QEIB1
	_TRISB6		= 1;	_CN24IE	= 0;	_CN24PUE	= 0;	// IN  : INDEX1
	_TRISB7		= 1;	_CN23IE	= 0;	_CN23PUE	= 0;	// IN  : HTR
	_TRISB8		= 0;	_CN22IE	= 0;	_CN22PUE	= 0;	// RES : SCL1
	_TRISB9		= 0;	_CN21IE	= 0;	_CN21PUE	= 0;	// RES : SDA1
	_TRISB10	= 0;	_CN16IE	= 0;	_CN16PUE	= 0;	// OUT : PWM1H3
	_TRISB11	= 0;	_CN15IE	= 0;	_CN15PUE	= 0;	// OUT : UART TX AX12
	_TRISB12	= 0;	_CN14IE	= 0;	_CN14PUE	= 0;	// OUT : PWM1H2
	_TRISB13	= 0;	_CN13IE	= 0;	_CN13PUE	= 0;	// OUT : DIR_2
	_TRISB14	= 0;	_CN12IE	= 0;	_CN12PUE	= 0;	// OUT : PWM1H1
	_TRISB15	= 0;	_CN11IE	= 0;	_CN11PUE	= 0;	// OUT : DIR_1

	// Config PORTC
	_TRISC0		= 1;	_CN8IE	= 0;	_CN8PUE		= 0;	// IN  : (ANALOG/DIGIT)  capteur 3 (AN6)
	_TRISC1		= 1;	_CN9IE	= 0;	_CN9PUE		= 0;	// IN  : (ANALOG/DIGIT)  capteur 4 (AN7)
	_TRISC2		= 0;	_CN10IE	= 0;	_CN10PUE	= 0;	// OUT : RTS UART 1
	_TRISC3		= 1;	_CN28IE	= 0;	_CN28PUE	= 0;	// IN  : QEIA2
	_TRISC4		= 1;	_CN25IE	= 0;	_CN25PUE	= 0;	// IN  : QEIB2
	_TRISC5		= 1;	_CN26IE	= 0;	_CN26PUE	= 0;	// IN  : INDEX2
	_TRISC6		= 1;	_CN18IE	= 0;	_CN18PUE	= 0;	// IN  : CTS UART 1
	_TRISC7		= 1;	_CN17IE	= 0;	_CN17PUE	= 0;	// IN  : RX UART AX12
	_TRISC8		= 0;	_CN20IE	= 0;	_CN20PUE	= 0;	// OUT : TX UART 1
	_TRISC9		= 1;	_CN19IE	= 0;	_CN19PUE	= 0;	// IN  : RX UART 1


	
	//****************
	// INITIALISATION
	//****************
	
	// Init PORTA
	_LATA0		= 0;	// Valeur initiale a  l'etat bas
	_LATA1		= 0;	// Valeur initiale a  l'etat bas
	_LATA3		= 0;	// Valeur initiale a  l'etat bas
	_LATA4		= 0;	// Valeur initiale a  l'etat bas
	_LATA7		= 1;	// Valeur initiale a  l'etat bas
	_LATA8		= 1;	// Valeur initiale a  l'etat bas
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


/*********************************************************/
void ConfigTimers (void)
{
	// **************************************************
	// 40Mhz		-> 25ns
	// TCKPS = 8	-> 25*8	= 200ns
	// PR2			-> 10ms/tcy = 0.01/0.0000002 = 50000
	// **************************************************
	// TCKPS :
	//   > 11	= 1:256		: tcy = 6400ns
	//   > 10	= 1:64		: tcy = 1600ns
	//   > 01	= 1:8		: tcy = 200ns
	//   > 00	= 1:1		: tcy = 25ns
	// **************************************************
	
	
	//****************
	// TIMER 2 : (BOARD1) Asservissement
	//****************
	T2CONbits.TON	= 0;		// Desactivation du Timer2
	T2CONbits.TSIDL	= 0;		// Mode continue en IDLE
	T2CONbits.TGATE	= 0;		// Gated time accumulation desactive
	T2CONbits.TCKPS	= 0b01;		// presscaler de 1:8 (35ms=0b01 | 10ms=0b01)
	T2CONbits.T32	= 0;		// valeur sur 16bits
	T2CONbits.TCS	= 0;		// Internal Clock
	TMR2			= 0x00;		// Effacement du registre timer
#if CARTE_DEFINIE == CARTE_BOARD1
	PR2				= 0xC34E;	// valeur sur 16bits : 10ms = 5000 x tcy (35ms=5468 | 10ms=0xC34E)
#else
	PR2				= 0x61A7;	// valeur sur 16bits : 10ms = 5000 x tcy (35ms=5468 | 10ms=0xC34E)
#endif
	T2CONbits.TON	= 1;		// Activation du Timer2


	//****************
	// TIMER 3 : (BOARD2) Automatisme
	//****************
	T3CONbits.TON	= 0;		// Desactivation du Timer3
	T3CONbits.TSIDL	= 0;		// Mode continue en IDLE
	T3CONbits.TGATE	= 0;		// Gated time accumulation desactive
	T3CONbits.TCKPS	= 0b01;		// presscaler de 1:8 (ms=0b01)
	T3CONbits.TCS	= 0;		// Internal Clock
	TMR3			= 0x00;		// Effacement du registre timer
	PR3				= 0xC34E;	// valeur sur 16bits : 10ms = 5000 x tcy (ms=0xD34E)
	T3CONbits.TON	= 1;		// Activation du Timer3


	//****************
	//TIMER4 : led
	//****************
	T4CONbits.TON	= 0;		// Desactivation du Timer2
	T4CONbits.TSIDL	= 0;		// Mode continue en IDLE
	T4CONbits.TGATE	= 0;		// Gated time accumulation desactive
	T4CONbits.TCKPS	= 0b01;		// presscaler de 1:8 (!!! DOIT ETRE INFERIEUR OU EGAL A 0b01 !!!)
	T4CONbits.T32	= 0;		// valeur sur 16bits
	T4CONbits.TCS	= 0;		// Internal Clock
	TMR4			= 0x00;		// Effacement du registre timer
	PR4				= 0x9FFF;	// valeur sur 16bits : 10ms = 5000 x tcy
	T4CONbits.TON	= 1;		// Activation du Timer4
}


/*********************************************************/
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
	IPC0bits.T1IP		= 0x00;						//
	IPC0bits.OC1IP		= 0x00;						//
	IPC0bits.IC1IP		= 0x00;						//
	IPC0bits.INT0IP		= PRIO_INTER_INT0_HTR;		// INT0 pour HTR
	
	// IPC1
	IPC1bits.T2IP		= PRIO_INTER_TIMER2;		// Timer2
	IPC1bits.OC2IP		= 0x00;						//
	IPC1bits.IC2IP		= 0x00;						//
	IPC1bits.DMA0IP		= 0x00;						//
	
	// IPC2
	IPC2bits.U1RXIP		= PRIO_INTER_UART1_RX;		// UART1 RX
	IPC2bits.SPI1IP		= 0x00;						//
	IPC2bits.SPI1EIP	= 0x00;						//
	IPC2bits.T3IP		= PRIO_INTER_TIMER3;		// Timer 3
	
	// IPC3
	IPC3bits.DMA1IP		= 0x00;						//
	IPC3bits.AD1IP		= 0x00;						//
	IPC3bits.U1TXIP		= 0x00;						//
	
	// IPC4
	IPC4bits.CNIP		= 0x00;						//
	IPC4bits.MI2C1IP	= PRIO_INTER_I2C_MAITRE;	// Maitre I2C1
	IPC4bits.SI2C1IP	= PRIO_INTER_I2C_ESCLAVE; 	// Esclave I2C1
	
	// IPC5
	IPC5bits.IC8IP		= 0x00;						//
	IPC5bits.IC7IP		= 0x00;						//
	IPC5bits.INT1IP		= 0x00;						//
	
	// IPC6
	IPC6bits.T4IP		= PRIO_INTER_TIMER4;		// Timer 4
	IPC6bits.OC4IP		= 0x00;						//
	IPC6bits.OC3IP		= 0x00;						//
	IPC6bits.DMA2IP		= 0x00;						//
	
	// IPC7
	IPC7bits.U2TXIP		= PRIO_INTER_UART2_TX;		// UART2 TX
	IPC7bits.U2RXIP		= PRIO_INTER_UART2_RX;		// UART2 RX
	IPC7bits.INT2IP		= 0x00;						//
	IPC7bits.T5IP		= 0x00;						//
	
	// IPC8
	IPC8bits.C1IP		= 0x00;						//
	IPC8bits.C1RXIP		= 0x00;						//
	IPC8bits.SPI2IP		= 0x00;						//
	IPC8bits.SPI2EIP	= 0x00;						//
	
	// IPC9
	IPC9bits.DMA3IP		= 0x00;						//
	
	// IPC10
	// PAS de IPC10
	
	// IPC11
	IPC11bits.DMA4IP	= 0x00;						//
	IPC11bits.PMPIP		= 0x00;						//
	
	// IPC12
	// PAS de IPC12
	
	// IPC13
	// PAS de IPC13
	
	// IPC14
	IPC14bits.QEI1IP	= PRIO_INTER_QEI1;			// Overflow sur QEI1
	IPC14bits.PWM1IP	= 0x00;						//
	
	// IPC15
	IPC15bits.FLTA1IP	= 0x00;						//
	IPC15bits.RTCIP		= 0x00;						//
	IPC15bits.DMA5IP	= 0x00;						//
	
	// IPC16
	IPC16bits.U2EIP		= 0x00;						//
	IPC16bits.U1EIP		= 0x00;						//
	IPC16bits.CRCIP		= 0x00;						//
	
	// IPC17
	IPC17bits.C1TXIP	= 0x00;						//
	IPC17bits.DMA7IP	= 0x00;						//
	IPC17bits.DMA6IP	= 0x00;						//
	
	// IPC18
	IPC18bits.QEI2IP	= PRIO_INTER_QEI2;			// Overflow sur QEI2
	IPC18bits.FLTA2IP	= 0x00;						//
	IPC18bits.PWM2IP	= 0x00;						//
	
	// IPC19
	IPC19bits.DAC1LIP	= 0x00;						//
	IPC19bits.DAC1RIP	= 0x00;						//
	
	
	//****************
	// Activation
	//****************
	// IEC0
	IEC0bits.DMA1IE		= 0;						// Non utilisee
	IEC0bits.AD1IE		= 0;						// Non utilisee
	IEC0bits.U1TXIE		= 0;						// Non utilisee
	IEC0bits.U1RXIE		= ACTIV_INTER_UART1_RX;		// Interruption RX sur UART1
	IEC0bits.SPI1IE		= 0;						// Non utilisee
	IEC0bits.SPI1EIE	= 0;						// Non utilisee
	IEC0bits.T3IE		= ACTIV_INTER_TIMER3;		// Interruption sur Timer3
	IEC0bits.T2IE		= ACTIV_INTER_TIMER2;		// Interruption sur Timer2
	IEC0bits.OC2IE		= 0;						// Non utilisee
	IEC0bits.IC2IE		= 0;						// Non utilisee
	IEC0bits.DMA0IE		= 0;						// Non utilisee
	IEC0bits.T1IE		= 0;						// Non utilisee
	IEC0bits.OC1IE		= 0;						// Non utilisee
	IEC0bits.IC1IE		= 0;						// Non utilisee
	IEC0bits.INT0IE		= ACTIV_INTER_INT0_HTR;		// Interruption sur INT0

	// IEC1
	IEC1bits.U2TXIE		= ACTIV_INTER_UART2_TX;		// Interruption TX sur UART2
	IEC1bits.U2RXIE		= ACTIV_INTER_UART2_RX;		// Interruption RX sur UART2
	IEC1bits.INT2IE		= 0;						// Non utilisee
	IEC1bits.T5IE		= 0;						// Non utilisee
	IEC1bits.T4IE		= ACTIV_INTER_TIMER4;		// Interruption sur Timer4
	IEC1bits.OC4IE		= 0;						// Non utilisee
	IEC1bits.OC3IE		= 0;						// Non utilisee
	IEC1bits.DMA2IE		= 0;						// Non utilisee
	IEC1bits.IC8IE		= 0;						// Non utilisee
	IEC1bits.IC7IE		= 0;						// Non utilisee
	IEC1bits.INT1IE		= 0;						// Non utilisee
	IEC1bits.CNIE		= 0;						// Non utilisee
	IEC1bits.MI2C1IE	= ACTIV_INTER_I2C_MAITRE;	// Interruption I2C MAITRE
	IEC1bits.SI2C1IE	= ACTIV_INTER_I2C_ESCLAVE;	// Interruption I2C ESCLAVE
	
	// IEC2
	IEC2bits.DMA4IE		= 0;						// Non utilisee
	IEC2bits.PMPIE		= 0;						// Non utilisee
	IEC2bits.DMA3IE		= 0;						// Non utilisee
	IEC2bits.C1IE		= 0;						// Non utilisee
	IEC2bits.C1RXIE		= 0;						// Non utilisee
	IEC2bits.SPI2IE		= 0;						// Non utilisee
	IEC2bits.SPI2EIE	= 0;						// Non utilisee
	
	// IEC3
	IEC3bits.FLTA1IE	= 0;						// Non utilisee
	IEC3bits.RTCIE		= 0;						// Non utilisee
	IEC3bits.DMA5IE		= 0;						// Non utilisee
	IEC3bits.QEI1IE		= ACTIV_INTER_QEI1;			// Non utilisee
	IEC3bits.PWM1IE		= 0;						// Non utilisee
	
	// IEC4
	IEC4bits.DAC1LIE	= 0;						// Non utilisee
	IEC4bits.DAC1RIE	= 0;						// Non utilisee
	IEC4bits.QEI2IE		= ACTIV_INTER_QEI2;			// Non utilisee
	IEC4bits.FLTA2IE	= 0;						// Non utilisee
	IEC4bits.PWM2IE		= 0;						// Non utilisee
	IEC4bits.C1TXIE		= 0;						// Non utilisee
	IEC4bits.DMA7IE		= 0;						// Non utilisee
	IEC4bits.DMA6IE		= 0;						// Non utilisee
	IEC4bits.CRCIE		= 0;						// Non utilisee
	IEC4bits.U2EIE		= 0;						// Non utilisee
	IEC4bits.U1EIE		= 0;						// Non utilisee
}


/*********************************************************/
void ConfigQEI (void)
{
	// QEI1CON
	QEI1CONbits.CNTERR	= 0;				// Reset flag erreur sur compteur
	QEI1CONbits.QEISIDL	= 0;				// Mode continu en idle
	QEI1CONbits.SWPAB	= 1;				// PA et PB entrelaces
	QEI1CONbits.PCDOUT	= 1;				// Position counter dir. status output active (IO controle par QEI)
	QEI1CONbits.POSRES	= 1;				// Reset du compteur sur INDEX (uniquement si signal INDEX)
	
	// DFLT1CON
	DFLT1CONbits.IMV1	= 0;				// Etat de PB lors d'un INDEX
	DFLT1CONbits.IMV0	= 0;				// Etat de PA lors d'un INDEX
	DFLT1CONbits.CEID	= 0;				// Interruption sur erreur compteur desactive
	DFLT1CONbits.QEOUT	= 1;				// Activation des filtres
	DFLT1CONbits.QECK	= 0b010;			// Ration de 1:4

	MAX1CNT				= CODEUR_1_NB_IMP;	// Limite du compteur codeur



	// QEI2CON
	QEI2CONbits.CNTERR	= 0;				// Reset flag erreur sur compteur
	QEI2CONbits.QEISIDL	= 0;				// Mode continu en idle
	QEI2CONbits.SWPAB	= 1;				// PA et PB entrelaces
	QEI2CONbits.PCDOUT	= 1;				// Position counter dir. status output active (IO controle par QEI)
	QEI2CONbits.POSRES	= 1;				// Reset du compteur sur INDEX (uniquement si signal INDEX)

	// DFLT2CON
	DFLT2CONbits.IMV1	= 0;				// Etat de PB lors d'un INDEX
	DFLT2CONbits.IMV0	= 0;				// Etat de PA lors d'un INDEX
	DFLT2CONbits.CEID	= 0;				// Interruption sur erreur compteur desactivee
	DFLT2CONbits.QEOUT	= 1;				// Activation des filtres
	DFLT2CONbits.QECK	= 0b010;			// Ration de 1:4

	MAX2CNT				= CODEUR_2_NB_IMP;	// Limite du compteur codeur
}


/*********************************************************/
void ConfigPWM (void)
{
	//****************
	//PWM
	//****************
	P1TCONbits.PTEN		= 1;		//PWM1 ON
	P1TCONbits.PTMOD	= 0;		//Base de temps en free running mode (11 bits vmax= 2048)
	P1TPER				= 999;		//F=40kHz 11 bits
	PWM1CON1bits.PMOD1	= 0;		//Mode complementaire sur pwm1L1
	PWM1CON1bits.PMOD2	= 0;		//Mode complementaire sur pwm1L2
	PWM1CON1bits.PEN1H	= 1;		//PWM1H1 pour PWM1
	PWM1CON1bits.PEN1L	= 0;		//PWM1L1 inactif => I/O
	PWM1CON1bits.PEN2H	= 1;		//PWM1H2 pour PWM2
	PWM1CON1bits.PEN2L	= 0;		//PWM1L2 inactif => I/O
	PWM1CON1bits.PEN3H	= 1;		//PWM1H3 pour PWM3
	PWM1CON1bits.PEN3L	= 0;		//PWM1L3 inactif => I/O
	PWM2CON1bits.PEN1H	= 0;		//PWM2H1 inactif => I/O
	PWM2CON1bits.PEN1L	= 0;		//PWM2L1 inactif => I/O
//	PDC1				= 1024;		//Vitesse nulle

	// Mise a zero des PWM
	P1DC1				= 0;
	P1DC2				= 0;
}


/*********************************************************/
void ConfigADC (void)
{
	// #if CONFIG_CAPTEUR_1 == CAPTEUR_ANALOGIQUE
	
	Nop ();
}
