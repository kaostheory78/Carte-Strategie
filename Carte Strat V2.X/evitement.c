/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: serialus.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 03 juin 2016, 21:44
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/**************************** VARIABLES GLOBALES ******************************/
/******************************************************************************/

volatile _evitement EVITEMENT_ADV;

/******************************************************************************/
/**************************** FONCTIONS DE CONFIG  ****************************/
/******************************************************************************/

void init_evitement()
{
    EVITEMENT_ADV.actif = ON;
    EVITEMENT_ADV.cote = EV_GAUCHE | EV_CENTRE | EV_DROIT;
    EVITEMENT_ADV.detection = OFF;
    EVITEMENT_ADV.mode = EVITEMENT_NORMAL;
    EVITEMENT_ADV.sens = MARCHE_AVANT;
}

_Bool check_evitement_avant()
{
    _Bool result = false;
    
    if ( ((EVITEMENT_ADV.cote & EV_CENTRE) != 0) && (CAPT_ADV_AVANT_C == ETAT_ADV_AVANT_C) )
        result = true;
    else if ( ((EVITEMENT_ADV.cote & EV_GAUCHE) != 0) && (CAPT_ADV_AVANT_G == ETAT_ADV_AVANT_G))
        result = true;
    else if ( ((EVITEMENT_ADV.cote & EV_DROIT) != 0) && (CAPT_ADV_AVANT_D == ETAT_ADV_AVANT_D))
        result = true;
    
    return result;
}

_Bool check_evitement_arriere()
{
    _Bool result = false;
    
    if ( ((EVITEMENT_ADV.cote & EV_CENTRE) != 0) && (CAPT_ADV_ARRIERE_C == ETAT_ADV_ARRIERE_C) )
        result = true;
    else if ( ((EVITEMENT_ADV.cote & EV_GAUCHE) != 0) && (CAPT_ADV_ARRIERE_G == ETAT_ADV_ARRIERE_G))
        result = true;
    else if ( ((EVITEMENT_ADV.cote & EV_DROIT) != 0) && (CAPT_ADV_ARRIERE_D == ETAT_ADV_ARRIERE_D))
        result = true;
    
    return result;
}

_Bool check_evitement()
{
    _Bool result = false;
    
    if (EVITEMENT_ADV.sens == MARCHE_AVANT && check_evitement_avant())
        result = true;
    else if (EVITEMENT_ADV.sens == MARCHE_ARRIERE && check_evitement_arriere())
        result = true;
    
    return result;
}

/******************************************************************************/
/****************************** GESTION EVITEMENT  ****************************/
/******************************************************************************/

/**
 * Cette fonction a pour but de désactiver automatiquement l'évitement dans 
 * des zones critiques comme les bordures de terrain
 */
void ajustement_evitement_autonome()
{
    // TO DO 
}


/******************************************************************************/
/***************************** FONCTION EVITEMENT  ****************************/
/******************************************************************************/

void evitement()
{
    static uint16_t compteur = 0;
    static _Bool  evitement_en_cours = false;
    
    if (EVITEMENT_ADV.actif == ON)
    {
        ajustement_evitement_autonome();
        
        if (check_evitement() && EVITEMENT_ADV.detection == OFF)
        {
            compteur = 0;
            evitement_en_cours = false;
            EVITEMENT_ADV.detection = ON;
            FLAG_ASSERV.erreur = EVITEMENT;
            brake();
        }
        else if (EVITEMENT_ADV.detection == ON && EVITEMENT_ADV.mode == STOP)
        {
            compteur++;
            if (compteur >  20)
            {
                compteur = 20;
                if (check_evitement() == false)
                {
                    EVITEMENT_ADV.detection = OFF;
                    unbrake();
                }   
            }
        }
        else if (EVITEMENT_ADV.detection == ON && (EVITEMENT_ADV.mode == ACTION_EVITEMENT || EVITEMENT_ADV.mode == EVITEMENT_NORMAL))
        {
            if (evitement_en_cours == false)
            {
                compteur++;
                if (compteur > 40)
                {
                    evitement_en_cours = true;
                    compteur = 0;
                    fin_deplacement();
                }
            }
        }
        else if (EVITEMENT_ADV.detection == ON)
        {
            EVITEMENT_ADV.detection = OFF;
            unbrake();
        }
    }
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
