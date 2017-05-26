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
    static uint16_t compteur_avant_deblocage = 0;
    static uint16_t filtre_detection = 0;
    static _Bool  evitement_en_cours = false;
    
    if (EVITEMENT_ADV.actif == ON)
    {
        ajustement_evitement_autonome();
        
        // On filtre le signal reçu pour éliminer les feux positifs (70 ms)
        if (check_evitement() && EVITEMENT_ADV.detection == OFF && filtre_detection < 7)
        {
            filtre_detection++;
        }
        // obstacle confirmé : on brake
        else if (check_evitement() && EVITEMENT_ADV.detection == OFF )
        {
            
            compteur_avant_deblocage = 0;
            evitement_en_cours = false;
            EVITEMENT_ADV.detection = ON;
            FLAG_ASSERV.erreur = EVITEMENT;
            brake();
        }
        else if (EVITEMENT_ADV.detection == ON && EVITEMENT_ADV.mode == STOP)
        {
            compteur_avant_deblocage++;
            if (compteur_avant_deblocage >  20)
            {
                compteur_avant_deblocage = 20;
                if (check_evitement() == false)
                {
                    EVITEMENT_ADV.detection = OFF;
                    filtre_detection = 0;
                    unbrake();
                }   
            }
        }
        else if (EVITEMENT_ADV.detection == ON && (EVITEMENT_ADV.mode == ACTION_EVITEMENT || EVITEMENT_ADV.mode == EVITEMENT_NORMAL))
        {
            if (evitement_en_cours == false)
            {
                compteur_avant_deblocage++;
                if (compteur_avant_deblocage > 40)
                {
                    evitement_en_cours = true;
                    compteur_avant_deblocage = 0;
                    filtre_detection = 0;
                    fin_deplacement_avec_brake(); // pas de brake avant ... on faisait un fin_deplacement_sans_brake
                }
            }
        }
        else if (EVITEMENT_ADV.detection == ON)
        {
            EVITEMENT_ADV.detection = OFF;
            filtre_detection = 0;
            unbrake();
        }
    }
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
