/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: autom.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 01 février 2015, 17:11
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

uint8_t DETECTION;
uint8_t EVITEMENT_ADV_AVANT;
uint8_t STOP_DETECTION;

void jack()
{
    while(!SYS_JACK);
    while(SYS_JACK);
}


void autom_10ms (void)
{



    /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/


        //fonction qui definit les actions
/*        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            default :
                break;
        }*/

        /**********************************************************************/
        /**************************** Evitemenr *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if (CAPT_US_BALISE == 1 && DETECTION == OFF)
            {
                DETECTION = ON;
                STOP_DETECTION = ON;
                brake();
            }
            else if (DETECTION == ON )//&& CAPT_US_BALISE == 0)
            {
                //if (CAPT_US_BALISE == 0)
                {
                    DETECTION = OFF;
                    delay_ms(200);
                    unbrake();
                }
            }
        }

      /*  if (EVITEMENT_ADV_ARRIERE == STOP)
        {
            if ( DETECTION == OFF && (CAPT_US_ARRIERE_D == 1 || CAPT_US_ARRIERE_G == 1) )//Rajouter des capteurs IR ?
            {
                DETECTION = ON;
                STOP_DETECTION_AR = ON;
                brake();
            }
            else if (DETECTION == ON)
            {
                DETECTION = OFF;
                delay_ms(200);
                unbrake();
            }
        }*/
}