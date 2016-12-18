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
 *  /!\ FONCTION BLOQUANTE INTERDITE !!!!  
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    FLAG_TIMER_5ms = 0;

    // odométrie
   calcul_position_robot();

   if (FLAG_ASSERV.totale == ON)
        asserv();
   
   if (CPT_TEMPS_MATCH.actif == true)
       CPT_TEMPS_MATCH.t_ms +=  5;
   
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
}

/**
 *  Timer 10 ms : Check capteurs
 *  /!\ TIMER SAFE : FONCTION BLOQUANTE INTERDITE /!\ //
 *      /!\ PAS DE FONCTIONS AX12 ICI !!! /!\ //
 */
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    FLAG_TIMER_10ms = 0;
    
    evitement();
    
    // Compteur pour évitement Hugo
    if(calcul_en_cours == ON)
    {
        compteur_evitement++;
        if(compteur_evitement > ATTENTE_EVITEMENT)
            compteur_evitement = ATTENTE_EVITEMENT;
    }
    else
    {
        compteur_evitement =0;
    }
}

/**
 * Timer d'autom : 20 ms
 * Décompte relancé à la fin de la fonctions -> temps non fiable
 * UNSFAE : fonction bloquante autorisée
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    
    TIMER_20ms = DESACTIVE;
    
    autom_20ms();
    
    #ifndef NO_SERIALUS   
    
    if (serialus.clignotement_en_cours == true)
    {
        #ifdef DEBUG_ACTIF
            debug();
        #endif        
    }
        
    #endif

    TMR3 = 0;
    FLAG_TIMER_20ms = 0;        //On clear le flag d'interruption du timer
    TIMER_20ms = ACTIVE;
}

/**
 * Timer 100 ms : Scheduleur temps de match
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{  
    if (CPT_TEMPS_MATCH.t_ms >= 90000UL)
    {
        PORTCbits.RC5 = 0;

        //On désactive toutes les interruptions :
        IEC0bits.T1IE = 0;
        IEC0bits.T2IE = 0;
        IEC0bits.T3IE = 0;
        IEC1bits.T4IE = 0;
        IEC1bits.T5IE = 0;
        
        IEC3bits.QEI1IE = 0;
        IEC4bits.QEI2IE = 0;

    #ifdef CARTE_V2
        ALIM_MOTEUR_Y = DESACTIVE;
    #endif
        TIMER_5ms   = DESACTIVE;
        TIMER_10ms  = DESACTIVE;
        TIMER_20ms  = DESACTIVE;
        TIMER_100ms = DESACTIVE;

        envoit_pwm(MOTEUR_DROIT, 0);
        envoit_pwm(MOTEUR_GAUCHE, 0);
        envoit_pwm(MOTEUR_X, 0);

        // Pour permettre l'envoit de commande AX12 
        // Depuis ici
        IPC7bits.U2TXIP	= 7;
        IPC7bits.U2RXIP = 7;     
        
        while(1);
    }
    
#ifndef NO_SERIALUS   
    if (serialus.clignotement_en_cours == true)
    {
        #ifndef DEBUG_ACTIF
            print_clignotement();
        #endif        
    }
        
#endif
    
    FLAG_TIMER_100ms = 0;        //On clear le flag d'interruption du timer
}

/**
 *  Timer 200 ms : Liaison série
 *  UNSAFE : Fonction bloquante autorisée 
 */
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
    TIMER_200ms = DESACTIVE;
    
#ifndef NO_SERIALUS
    if (serialus.actif == true)
        traitement_serialus();
#endif
    
    TMR5 = 0;
    FLAG_TIMER_200ms = 0;        //On clear le flag d'interruption du timer
    TIMER_200ms = ACTIVE;
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
 
#ifndef NO_SERIALUS
    if (serialus.actif == true)
        action_reception_serialus(buf);
#endif
        
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