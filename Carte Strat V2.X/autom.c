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

void jack()
{
    while(!SYS_JACK);
    while(SYS_JACK);
}


/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/

void pinces (uint8_t ID, uint8_t etat)
{
    if (ID == PINCE_ASCENSEUR)
    {
        switch (etat)
        {
            case FERMER :
                angle_AX12(ID, PINCE_ASC_FERME, 1023, SANS_ATTENTE);
                break;
            case RELACHE :
                angle_AX12(ID, PINCE_ASC_RELACHE, 1023, SANS_ATTENTE);
                break;
            case RACLETTE :
                angle_AX12(ID, PINCE_ASC_RACLETTE, 1023, SANS_ATTENTE);
                break;
            case RANGEMENT :
                angle_AX12(ID, PINCE_ASC_RANGE, 1023, SANS_ATTENTE);
                break;
        }
    }
    else 
    {
        switch (etat)
        {
            case FERMER :
                angle_AX12(ID, PINCES_FERME, 1023, SANS_ATTENTE);
                break;
            case RELACHE :
                angle_AX12(ID, PINCES_RELACHE, 1023, SANS_ATTENTE);
                break;
            case RANGEMENT :
                angle_AX12(ID, PINCES_RANGE, 1023, SANS_ATTENTE);
        }
    }
}

void ascenseur (uint8_t direction)
{
    if (direction == MONTER)
        angle_AX12(ASCENSEUR, ASC_HAUT, 1023, SANS_ATTENTE);
    else if (direction == DESCENDRE)
        angle_AX12(ASCENSEUR, ASC_BAS, 1023, SANS_ATTENTE);
    else if (direction == HAUTEUR_DEMMARAGE)
        angle_AX12(ASCENSEUR, ASC_DEMARAGE, 1023, SANS_ATTENTE);
}


/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{



        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/
        static uint8_t etat_pince_asc = LIBRE, etat_pince_haut = RANGEMENT, etat_ascenseur = HAUTEUR_DEMMARAGE;

        //fonction qui definit les actions
/*       switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            default :
                break;
        }*/

     /*   if (CAPT_PINCE == 0 && etat_pince_asc == LIBRE)
        {
            pinces(PINCE_ASCENSEUR, FERMER);
            etat_pince_asc = EN_COURS;
        }
        else if (etat_pince_asc == EN_COURS)
        {
            if (read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0 )
            {
                etat_pince_asc = FERMER;
                ascenseur(MONTER);
                etat_ascenseur = EN_MONTER;
                pinces(PINCE_BAS, RELACHE);
                pinces(PINCE_MILIEU, RELACHE);
                pinces(PINCE_HAUT, RELACHE);
            }
        }
        if (etat_ascenseur == EN_MONTER)
        {
            if (read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
            {
                etat_ascenseur = MONTER;
                pinces(PINCE_BAS, FERMER);
                pinces(PINCE_MILIEU, FERMER);
                pinces(PINCE_HAUT, FERMER);
                etat_pince_haut = EN_COURS;
            }
        }
        if (etat_pince_haut == EN_COURS)
        {
            if (read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0 )
            {
                etat_pince_haut = FERMER;
                pinces(PINCE_ASCENSEUR, RELACHE);
                etat_ascenseur = EN_DESCENTE;
                delay_ms(10);
               ascenseur(DESCENDRE);
            }
        }
        if (etat_ascenseur == EN_DESCENTE)
        {
            pinces(PINCE_ASCENSEUR, RACLETTE);
            if (read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
            {
                etat_ascenseur = DESCENDRE;
                etat_pince_asc = LIBRE;
            }
                
        }*/


        


        /**********************************************************************/
        /**************************** Evitement *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == STOP)
        {
            if (CAPT_US_BALISE == 1 && DETECTION == OFF)
            {
                DETECTION = ON;
                FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                DETECTION = OFF;
                delay_ms(200);
                unbrake();
                
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                DETECTION = OFF;
                delay_ms(200);
                fin_deplacement();
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