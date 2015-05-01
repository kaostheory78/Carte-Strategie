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
        P1TCONbits.PTCKPS       = 0;            //Prescaller  1 : 1

	P1TCONbits.PTMOD	= 0;		//Base de temps en free running mode (11 bits vmax = 2048)

        P1TPER                  = 999;		//F=20kHz 11 bits

	PWM1CON1bits.PMOD1	= 1;		//Mode indépendant             //Mode complementaire sur pwm1L1
	PWM1CON1bits.PMOD2	= 1;		//Mode indépendant             //Mode complementaire sur pwm1L2
        PWM1CON1bits.PMOD3      = 1;            //Mode indépendant         

    #ifdef CARTE_V1
	PWM1CON1bits.PEN1H	= 0;		//PWM1H1 inactif
	PWM1CON1bits.PEN1L	= 0;		//PWM1L1 inactif => I/O
	PWM1CON1bits.PEN2H	= 1;		//PWM1H2 pour PWM_GAUCHE
	PWM1CON1bits.PEN2L	= 0;		//PWM1L2 inactif => I/O
	PWM1CON1bits.PEN3H	= 1;		//PWM1H3 pour pour PWM_DROIT
	PWM1CON1bits.PEN3L	= 0;		//PWM1L3 inactif => I/O

	PWM2CON1bits.PEN1H	= 0;		//PWM2H1 inactif => I/O
	PWM2CON1bits.PEN1L	= 0;		//PWM2L1 inactif => I/O
    #endif

    #ifdef CARTE_V2
	PWM1CON1bits.PEN1L	= 1;		//PWM1L1 pour PWM moteur X
	PWM1CON1bits.PEN1H	= 0;		//PWM1H1 inactif => I/O
	PWM1CON1bits.PEN2H	= 0;		//PWM1H2 inactif => I/O
	PWM1CON1bits.PEN2L	= 1;		//PWM1L2 pour PWM moteur droit
	PWM1CON1bits.PEN3H	= 0;		//PWM1H3 inactif => I/O
	PWM1CON1bits.PEN3L	= 1;		//PWM1L3 pour PWM moteur gauche

	PWM2CON1bits.PEN1H	= 0;		//PWM2H1 inactif => I/O
	PWM2CON1bits.PEN1L	= 0;		//PWM2L1 inactif => I/O
    #endif


	// Mise a zero des PWM
	REGISTRE_MOTEUR_D       = 0;
	REGISTRE_MOTEUR_G	= 0;
    #ifdef CARTE_V2
        REGISTRE_MOTEUR_X       = 0;
    #endif
}


/******************************************************************************/
/******************************** MODFIFS PWM *********************************/
/******************************************************************************/

void envoit_pwm (char moteur, double valeur)
{
    double abs_valeur;

    if (valeur < 0.)
        abs_valeur = (double) (-valeur);
    else
      abs_valeur = (double) valeur;

    abs_valeur *= PWM_DROIT_VALEUR_MAX / 100;

    if (moteur == MOTEUR_DROIT)
    {
        if (abs_valeur > PWM_DROIT_VALEUR_MAX)
            abs_valeur = PWM_DROIT_VALEUR_MAX;

        if (valeur < (double) 0.) //>
        {
           SENS_MOTEUR_DROIT = AVANCER_MOTEUR_D;
        }
        else
        {
            SENS_MOTEUR_DROIT = RECULER_MOTEUR_D;
        } 

        REGISTRE_MOTEUR_D = (uint16_t) abs_valeur;
    }
    else if (moteur == MOTEUR_GAUCHE)
    {
         if (abs_valeur > PWM_DROIT_VALEUR_MAX)
            abs_valeur = PWM_DROIT_VALEUR_MAX;

        if (valeur < (int32_t) 0.)
            SENS_MOTEUR_GAUCHE = RECULER_MOTEUR_G;
        else
            SENS_MOTEUR_GAUCHE = AVANCER_MOTEUR_G;

        REGISTRE_MOTEUR_G = (uint16_t) abs_valeur;
    }
#ifdef CARTE_V2
    else if (moteur == MOTEUR_X)
    {
        if (abs_valeur > PWM_X_VALEUR_MAX)
            abs_valeur = PWM_X_VALEUR_MAX;

        if (valeur < (double) 0.)
            SENS_MOTEUR_X = RECULER_MOTEUR_X;
        else
            SENS_MOTEUR_X = AVANCER_MOTEUR_X;

        REGISTRE_MOTEUR_X = (uint16_t) abs_valeur;
    }
#endif
}

#ifdef CARTE_V2
void alimenter_moteur_Y (uint8_t alim, int8_t sens_marche)
{
    if (alim == ON)
    {
        if (sens_marche == MARCHE_AVANT)
            SENS_MOTEUR_Y = AVANCER_MOTEUR_Y;
        else
            SENS_MOTEUR_Y = RECULER_MOTEUR_Y;
        ALIM_MOTEUR_Y = ACTIVE;
    }
    else
        ALIM_MOTEUR_Y = DESACTIVE;
}

#endif


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
