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
    else if (direction == DEPOSE)
        angle_AX12(ASCENSEUR, ASC_DEPOSE, 1023, SANS_ATTENTE);
}

void init_pinces_jack()
{
    pinces(PINCE_HAUT, RANGEMENT);
    pinces(PINCE_MILIEU, RANGEMENT);
    pinces(PINCE_BAS, RANGEMENT);
    ascenseur(HAUTEUR_DEMMARAGE);
    while(read_data(ASCENSEUR, LIRE_MOUV_FLAG) != 0);
    pinces(PINCE_ASCENSEUR, RANGEMENT);
}

void init_balle()
{
    angle_AX12(SYS_BALLE, INIT_BALLE, 1023, SANS_ATTENTE);
}

void init_pinces_demarage()
{
    static char etat_pince_demarage = 0;
    if (etat_pince_demarage == 0)
    {
        pinces(PINCE_HAUT, RELACHE);
        pinces(PINCE_MILIEU, RELACHE);
        pinces(PINCE_BAS, RELACHE);
        pinces(PINCE_ASCENSEUR, RACLETTE);
        init_balle();
        etat_pince_demarage = 1;
    }
    else if (read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {  
        ascenseur(DESCENDRE);
        FLAG_ACTION = ATTRAPE_PIEDS;
    }
}

void ejecter_balle()
{
    angle_AX12(SYS_BALLE, EJECTER_BALLE, 400, SANS_ATTENTE);
}

void depose_pieds ()
{
    static int occupation_pince = -1, etat_ascenceur = 0, etat_pince = 0;
    if (occupation_pince == -1 && CAPT_PINCE == 0)
    {
        occupation_pince = 1;
    }
    else if (occupation_pince == -1)
    {
        occupation_pince = 0;
    }

    if (occupation_pince == 1)
    {
        if (etat_ascenceur == 0)
        {
            ascenseur(DESCENDRE);
            etat_ascenceur = EN_DESCENTE;
        }
        else if (etat_ascenceur == EN_DESCENTE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_HAUT, RANGEMENT);
            pinces(PINCE_MILIEU, RANGEMENT);
            pinces(PINCE_BAS, RANGEMENT);
            pinces (PINCE_ASCENSEUR, RACLETTE);
            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
    else if (occupation_pince == 0)
    {
        if (etat_ascenceur == 0)
        {
            pinces(PINCE_ASCENSEUR, RELACHE);
            ascenseur (MONTER);
            etat_ascenceur = EN_MONTER;
        }
        else if (etat_ascenceur == EN_MONTER && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_ASCENSEUR, FERMER);
            etat_ascenceur = EN_HAUT;
        }
        else if (etat_ascenceur == EN_HAUT && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0 && etat_pince == 0)
        {
            pinces(PINCE_HAUT, RELACHE);
            pinces(PINCE_MILIEU, RELACHE);
            pinces(PINCE_BAS, RELACHE);
            etat_pince = EN_COURS;
        }
        else if (etat_pince == EN_COURS && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0)
        {
            ascenseur(DESCENDRE);
            etat_ascenceur = EN_DESCENTE;
            etat_pince = 0;
        }
        else if (etat_ascenceur == EN_DESCENTE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_ASCENSEUR, RACLETTE);
            pinces(PINCE_HAUT, RANGEMENT);
            pinces(PINCE_MILIEU, RANGEMENT);
            pinces(PINCE_BAS, RANGEMENT);

            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
}

void preparation_descente_pieds ()
{
    static int occupation_pince = -1, etat_ascenceur = 0, etat_pince = 0;
    ETAT_AUTOM = NE_RIEN_FAIRE;

    if (occupation_pince == - 1 && CAPT_PINCE == 0)
    {
        occupation_pince = 1;
    }
    else if (occupation_pince == -1)
    {
        occupation_pince = 0;
    }

    if (occupation_pince == 1)
    {
        //if (etat_ascenceur == 0)
        //{
        //   ascenseur(DESCENDRE);
        //   etat_ascenceur = EN_DESCENTE;
        //}
        //else if (etat_ascenceur == EN_DESCENTE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0 && etat_pince != EN_COURS)
        //{
        //    pinces(PINCE_HAUT, FERMER);
        //    pinces(PINCE_MILIEU, FERMER);
        //    pinces(PINCE_BAS, FERMER);
        //    pinces (PINCE_ASCENSEUR, FERMER);
        //    etat_pince = EN_COURS;
        //}
        //else if (etat_pince == EN_COURS && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        if (etat_pince == 0)
        {
            ascenseur(DEPOSE);//
            pinces(PINCE_BAS, RELACHE);
            pinces(PINCE_MILIEU, RELACHE);
            pinces(PINCE_HAUT, RELACHE);
            etat_pince = RELACHE;
            etat_ascenceur = DEPOSE;//
        }
        //else if (etat_pince == RELACHE && read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) == 0)
        //{
        //    ascenseur(DEPOSE);
        //    etat_pince = 0;
        //    etat_ascenceur = DEPOSE;
        //}
        else if (etat_ascenceur == DEPOSE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_HAUT, FERMER);
            pinces(PINCE_MILIEU, FERMER);
            pinces(PINCE_BAS, FERMER);
            
            //Reinit fonction
            occupation_pince = -1;
            etat_ascenceur = 0;
            etat_pince = 0;

            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
    else if (occupation_pince == 0)
    {
        if (etat_ascenceur == 0)
        {
            pinces(PINCE_ASCENSEUR, RELACHE);
            ascenseur (MONTER);
            etat_ascenceur = EN_MONTER;
        }
        else if (etat_ascenceur == EN_MONTER && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_ASCENSEUR, FERMER);
            etat_ascenceur = EN_HAUT;
        }
        else if (etat_ascenceur == EN_HAUT && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0 && etat_pince == 0)
        {
            pinces(PINCE_HAUT, RELACHE);
            pinces(PINCE_MILIEU, RELACHE);
            pinces(PINCE_BAS, RELACHE);
            etat_pince = EN_COURS;
        }
        else if (etat_pince == EN_COURS && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0)
        {
            ascenseur(DEPOSE);
            etat_ascenceur = DEPOSE;
            etat_pince = 0;
        }
        else if (etat_ascenceur == DEPOSE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_HAUT, FERMER);
            pinces(PINCE_MILIEU, FERMER);
            pinces(PINCE_BAS, FERMER);

            //Reinit
            occupation_pince = -1;
            etat_ascenceur = 0;
            etat_pince = 0;

            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
}

void depose_banc ()
{
    ejecter_balle();
    pinces(PINCE_ASCENSEUR, RACLETTE);
    pinces(PINCE_HAUT, RANGEMENT);
    pinces(PINCE_MILIEU, RANGEMENT);
    pinces(PINCE_BAS, RANGEMENT);
    while (read_data(PINCE_BAS, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) != 0);
}

void attrape_gobelet()
{
    if (CAPT_PINCE== 0)
    {
        pinces(PINCE_BAS, RELACHE);
        pinces(PINCE_ASCENSEUR, FERMER);
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

void depose_gobelet()
{
    static int etat = 0;
    if (etat == 0)
    {
        etat = 1;
        pinces(PINCE_BAS, RANGEMENT);
        pinces(PINCE_ASCENSEUR, RACLETTE);
    }
    else if (read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {
        etat = 0;
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

void attrape_balle ()
{
    static int etat_ascenseur = 0;

    if (etat_ascenseur == 0)
    {
        pinces(PINCE_ASCENSEUR, FERMER);
        pinces(PINCE_BAS, FERMER);
        etat_ascenseur = 1;
    }
    else if (etat_ascenseur == 1 && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {
        ascenseur(MONTER);

        etat_ascenseur = 0;
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{



        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/
        static uint8_t etat_pince_asc = LIBRE, etat_pince_haut = RANGEMENT, etat_ascenseur = HAUTEUR_DEMMARAGE, compteur_pieds = 0;

        //fonction qui definit les actions
        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            case ATTRAPE_PIEDS :
                ETAT_AUTOM = ATTRAPE_PIEDS;
                FLAG_ACTION = NE_RIEN_FAIRE;
                break;
            case INIT_PINCES_DEMARRAGE :
                init_pinces_demarage();
                break;
            case DEPOSE_PIEDS :
                depose_pieds();
            case PREPARATION_DEPOSE_PIEDS :
                preparation_descente_pieds();
                break;
            case ATTRAPE_GOBELET :
                attrape_gobelet();
                break;
            case DEPOSE_GOBELET :
                depose_gobelet();
                break;
            case ATTRAPE_BALLE :
                attrape_balle();
                break;
            default :
                break;
        }

        if (ETAT_AUTOM == ATTRAPE_PIEDS)
        {

            if (CAPT_PINCE == 0 && etat_pince_asc == LIBRE)
            {
                pinces(PINCE_ASCENSEUR, FERMER);
                if (compteur_pieds < 3)
                    etat_pince_asc = EN_COURS;
                else 
                    ETAT_AUTOM = ACCOMPLI;
                compteur_pieds++;
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
            }
        }

        


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