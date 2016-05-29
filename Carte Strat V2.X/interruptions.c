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

    // odométrie
   calcul_position_robot();

   if (FLAG_ASSERV.totale == ON)
        asserv();
   
   // Clignotement d'une led de ALIVE
   __attribute__((near)) static int led = 1, compteur = 0;
   compteur++;

    if (compteur == 100)
    {
       if (led == 1)
            led = 0;
        else
            led = 1;

#ifdef PETIT_ROBOT
        CAPTEUR3 = led;
#endif
        compteur = 0;
    }

    // Compteur pour évitement Hugo
    if(calcul_en_cours == ON)
    {
        compteur_evitement++;
        if(compteur_evitement > ATTENTE_EVITEMENT * 2)
            compteur_evitement = ATTENTE_EVITEMENT * 2;
    }
    else
    {
        compteur_evitement =0;
    }

   Nop();
   
}

/**
 * Timer fin de match : 90 secondes
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    COMPTEUR_TEMPS_MATCH ++;

    if (COMPTEUR_TEMPS_MATCH >= 90)
    {
        PORTCbits.RC5 = 0;

        //On désactive toutes les interruptions :
        IEC0bits.T2IE = 0;
        IEC0bits.T1IE = 0;
        IEC1bits.T4IE = 0;
        IEC1bits.T5IE = 0;
        IEC3bits.QEI1IE = 0;
        IEC4bits.QEI2IE = 0;

        ALIM_MOTEUR_Y = DESACTIVE;

        TIMER_10ms = DESACTIVE;
        TIMER_5ms = DESACTIVE;
        TIMER_90s = DESACTIVE;
        TIMER_DEBUG = DESACTIVE;

        envoit_pwm(MOTEUR_DROIT, 0);
        envoit_pwm(MOTEUR_GAUCHE, 0);
        envoit_pwm(MOTEUR_X, 0);

        IPC7bits.U2TXIP	= 7;
        IPC7bits.U2RXIP = 7;     
        
        while(1);
    }
    FLAG_TIMER_90s = 0;        //On clear le flag d'interruption du timer
}

/**
 * Timer 10 ms : Autom et capteurs
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
    TIMER_10ms = DESACTIVE;
    autom_10ms();

    TMR4 = 0;
    FLAG_TIMER_10ms = 0;        //On clear le flag d'interruption du timer
    TIMER_10ms = ACTIVE;
}

void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
    TIMER_DEBUG = DESACTIVE;
    traitement_serialus();
    //debug();
    FLAG_TIMER_DEBUG = 0;        //On clear le flag d'interruption du timer
    TIMER_DEBUG = ACTIVE;
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

    uint8_t buf;
    buf= U1RXREG;
        
    action_reception_serialus(buf);
        
	// Activation de l'interruption
	IEC0bits.U1RXIE	= 1;
}



void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void)
{
    IEC1bits.U2RXIE = 0;
    IFS1bits.U2RXIF = 0;

    reception_uart_ax12();

    IEC1bits.U2RXIE = 1;
}

void __attribute__ ((interrupt, no_auto_psv)) 	_U2TXInterrupt (void)
{
    IFS1bits.U2TXIF = 0; //remise du flag à 0 quand le buffer de transmission est vide
    ax12.etat_uart = ENVOIT_FINI;
}




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/