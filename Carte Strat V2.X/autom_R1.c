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

/******************************************************************************/
/***************************** FONCTIONS DIVERSES *****************************/
/******************************************************************************/

#ifdef GROS_ROBOT

void son_evitement (uint8_t melodie)
{
    //Nop();
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10, NC, NC, NC);
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie, NC, NC, NC);
}

void rotation_us(void)
{
    static char sens = 0;
    static float angle = 0.;
   // if (read_data(AX_US, LIRE_MOUV_FLAG) == 0)
   // {
        if (sens == 0)
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle -= 6;
            if (angle < -60)
                sens = 1;
        }
        else
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle += 6;
            if (angle > 60)
            sens = 0;
        }
   // }
}

void rotation_us_avant ()
{
    static char sens_D = 0, sens_G = 0;
    static uint16_t position_D = 0, position_G = 0;

    if (sens_D == 0)
    {
        position_D += 10;
        if (position_D > 876)
            sens_D = 1;
    }
    else
    {
        position_D -= 10;
        if (position_D < 703)
            sens_D = 0;
    }

    if (sens_G == 0)
    {
        position_G += 10;
        if (position_G > 666)
            sens_G = 1;
    }
    else
    {
        position_G -= 10;
        if (position_G < 512)
            sens_G = 0;
    }

    angle_AX12(AX_US_DROIT, position_D, 1023, SANS_ATTENTE);
    angle_AX12(AX_US_GAUCHE, position_G, 1023, SANS_ATTENTE);
}

uint8_t inversion_autom (uint8_t cote)
{
    if (COULEUR == JAUNE)
        return cote;
    else
    {
        if (cote == DROIT)
            return GAUCHE;
        else if (cote == GAUCHE)
            return DROIT;
        else
            return cote;
    }
}

uint8_t check_capteur (uint8_t cote)
{
    cote = inversion_autom(cote);

    if (cote == DROIT)
        return CAPT_GOBELET_D;
    else if (cote == GAUCHE)
        return CAPT_GOBELET_G;
    else
        return 1;
}


/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/



/******************************************************************************/
/**************************** FONCTIONS D'INITS *******************************/
/******************************************************************************/


void init_jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
}

void init_depart()
{
    Nop();
}

/******************************************************************************/
/******************************** FONCTIONS AUTOM *****************************/
/******************************************************************************/

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{

    static uint16_t compteur = 0;
    static uint8_t  evitement_en_cours = OFF;

        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/

        //fonction qui definit les actions
        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            default :
                break;
        }

        /**********************************************************************/
        /**************************** Evitement *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == ON)
        {
            if (  (CAPT_US_BALISE == 1 || CAPT_US_AV_DROIT == 0 || CAPT_US_AV_GAUCHE == 0)  && DETECTION == OFF )
            {
                compteur = 0;
                DETECTION = ON;
                evitement_en_cours = OFF;
                FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_US_BALISE == 0 ||CAPT_US_AV_DROIT == 1 || CAPT_US_AV_GAUCHE == 1)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
                    {
                        evitement_en_cours = ON;
                        compteur = 0;
                        fin_deplacement();
                    }
                }
            }
            rotation_us_avant();
        }

        //Evitement arrière
        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_US_ARRIERE == 1)  && DETECTION == OFF)
            {
                compteur = 0;
                DETECTION = ON;
                evitement_en_cours = OFF;
                FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_US_ARRIERE == 0)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
                    {
                        evitement_en_cours = ON;
                        compteur = 0;
                        fin_deplacement();
                        son_evitement(30);
                    }
                     else
                     {
                        son_evitement(2);
                    }
                }
            }
            else
                rotation_us();
        }
        else if (DETECTION == ON)
        {
            DETECTION = OFF;
            unbrake();
        }
}


#endif