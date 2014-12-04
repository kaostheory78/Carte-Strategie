/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: interruptions.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08/11/2014
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


/******************************************************************************/
/*********************** INTERRUPTION DES TIMERS ******************************/
/******************************************************************************/


/**
 *  Timer 5 ms : Asserv
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
   FLAG_TIMER_5ms = 0;
   //calcul_position_robot();

   calcul_position_robot();

   //envoit_pwm(MOTEUR_DROIT, 50);

   if (FLAG_ASSERV.totale == ON)
        asserv();
   
}

/**
 * Timer fin de match : 90 secondes
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    PORTCbits.RC5 = 0;

    //On désactive toutes les interruptions :
    IEC0bits.T2IE = 0;
    IEC0bits.T1IE = 0;
    IEC1bits.T4IE = 0;
    IEC1bits.T5IE = 0;
    IEC3bits.QEI1IE = 0;
    IEC4bits.QEI2IE = 0;

    TIMER_10ms = DESACTIVE;
    TIMER_5ms = DESACTIVE;
    TIMER_90s = DESACTIVE;
    TIMER_DEBUG = DESACTIVE;

    while(1);
    FLAG_TIMER_90s = 0;        //On clear le flag d'interruption du timer
}

/**
 * Timer 10 ms : Autom et capteurs
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{

    FLAG_TIMER_10ms = 0;        //On clear le flag d'interruption du timer
}

void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{

    FLAG_TIMER_DEBUG = 0;        //On clear le flag d'interruption du timer
    debug();
}

/******************************************************************************/
/************************** INTERRUPTION DES QEI ******************************/
/******************************************************************************/

/**
 * Interruption sur codeur droit
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0;            //Clear du flag de l'event

    if (QEI1CONbits.UPDN == 1)      //codeur croit
        OVERFLOW_CODEUR [CODEUR_D] ++;
    else                            // codeur decroit
        OVERFLOW_CODEUR [CODEUR_D] --;
}

/**
 * Interruption sur codeur gauche
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI2Interrupt(void)
{
    IFS4bits.QEI2IF = 0;            //Clear du flag de l'event


    if (QEI2CONbits.UPDN == 1)      //codeur croit
        OVERFLOW_CODEUR [CODEUR_G] ++;
    else                            // codeur decroit
        OVERFLOW_CODEUR [CODEUR_G] --;
}

/******************************************************************************/
/***************************** INTERRUPTIONS UART *****************************/
/******************************************************************************/

void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.U1RXIE	= 0;
	IFS0bits.U1RXIF = 0;

	// Traitement
	//char buff = U1RXREG;

	// Activation de l'interruption
	IEC0bits.U1RXIE	= 1;
}



void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void)
{
    /*
    int erreur = 0;

	// Desactivation de l'interruption
	IEC1bits.U2RXIE	= 0;
	IFS1bits.U2RXIF = 0;

	uint8_t buf = U2RXREG;

	// On attend des données
	if (ax12.nb_octet_attente > 0)
	{
		ax12.buffer[ax12.offset] = buf;
		ax12.offset++;

		// Si les 1ers octets sont mauvais
		if ((ax12.offset >= 1) && (ax12.buffer[0] != START_BYTE)) erreur = 1;
		if ((ax12.offset >= 2) && (ax12.buffer[1] != START_BYTE)) erreur = 1;

		// En cas d'erreur on reset
		if (erreur == 1)
		{
			ax12.offset	= 0;
		}
	}

	// Activation de l'interruption
	IEC1bits.U2RXIE	= 1;*/
}





/******************************************************************************/
/******************************************************************************/
/******************************************************************************/