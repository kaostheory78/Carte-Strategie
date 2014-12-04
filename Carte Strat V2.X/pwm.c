/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: pwm.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 19 novembre 2014, 12:07
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


/******************************************************************************/
/**************************** Configurations Des PWM **************************/
/******************************************************************************/


void ConfigPWM (void)
{
	//****************
	//PWM
	//****************
	P1TCONbits.PTEN		= 1;		//PWM1 ON
        P2TCONbits.PTEN         = 0;            //PWM2 OFF

        P1TCONbits.PTSIDL       = 1;            //continu en idle
        P1TCONbits.PTOPS        = 0;            //Postscaller 1 : 1
        P1TCONbits.PTCKPS       = 0;            //Prescaller 1 : 1

	P1TCONbits.PTMOD	= 0;		//Base de temps en free running mode (11 bits vmax= 2048)
	
        P1TPER                  = 999;		//F=40kHz 11 bits


	PWM1CON1bits.PMOD1	= 0;		//Mode complementaire sur pwm1L1
	PWM1CON1bits.PMOD2	= 0;		//Mode complementaire sur pwm1L2
	PWM1CON1bits.PEN1H	= 0;		//PWM1H1 inactif
	PWM1CON1bits.PEN1L	= 0;		//PWM1L1 inactif => I/O
	PWM1CON1bits.PEN2H	= 1;		//PWM1H2 pour PWM_GAUCHE
	PWM1CON1bits.PEN2L	= 0;		//PWM1L2 inactif => I/O
	PWM1CON1bits.PEN3H	= 1;		//PWM1H3 pour pour PWM_DROIT
	PWM1CON1bits.PEN3L	= 0;		//PWM1L3 inactif => I/O

	PWM2CON1bits.PEN1H	= 0;		//PWM2H1 inactif => I/O
	PWM2CON1bits.PEN1L	= 0;		//PWM2L1 inactif => I/O


	// Mise a zero des PWM
	P1DC2				= 0;
	P1DC3				= 0;
}

void envoit_pwm (char moteur, int32_t valeur)
{
    uint16_t abs_valeur;

    abs_valeur = (uint16_t) abs(valeur);
    abs_valeur *= PWM_DROIT_VALEUR_MAX / 100;

    if (moteur == MOTEUR_DROIT)
    {
        if (abs_valeur > PWM_DROIT_VALEUR_MAX)
            abs_valeur = PWM_DROIT_VALEUR_MAX;

        if (valeur < (int32_t) 0) //>
        {
           PORTBbits.RB11 = AVANCER_MOTEUR_D;
        }
        else
        {
            PORTBbits.RB11 = RECULER_MOTEUR_D;
        } 

        P1DC3 = abs_valeur;
    }
    else if (moteur == MOTEUR_GAUCHE)
    {
         if (abs_valeur > PWM_DROIT_VALEUR_MAX)
            abs_valeur = PWM_DROIT_VALEUR_MAX;

        if (valeur < (int32_t) 0)
            PORTBbits.RB13 = RECULER_MOTEUR_G;
        else
            PORTBbits.RB13 = AVANCER_MOTEUR_G;

        P1DC2 = abs_valeur;
    }
}

