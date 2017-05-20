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
/*************************** Variables Globales *******************************/
/******************************************************************************/

static uint8_t modules_tour[LES_DEUX] = {0};

/******************************************************************************/
/***************************** FONCTIONS DIVERSES *****************************/
/******************************************************************************/

#ifdef PETIT_ROBOT

void jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
    torque_enable_ax12(TOUS_LES_AX12, true);

    while(!SYS_JACK);
    
    if (checkup_com_ax12() == PAS_D_ERREUR)
    {
        init_system_avant();
        init_system_arriere();
    }
    else
    {
        printf("\n\r checkup_com_ax12 FAILED");
        // TODO - qu'est ce qu'on fait ??????
        init_system_avant();
        init_system_arriere();
    }
    
    while(SYS_JACK);
}

void allumer_pompes ()
{
    envoit_pwm(MOTEUR_X, 100);
}

void eteindre_pompe ()
{
    envoit_pwm(MOTEUR_X, 0);
}

void son_evitement (uint8_t melodie)
{/*
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10);
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie);

  */
}

uint8_t getIdAx12 (_type_actionneur type_actionneur, _cote cote)
{
    uint8_t idAx12;
    
    switch(type_actionneur)
    {
        case PINCE_BAS :
            if (cote == AVANT)
            {
                idAx12 = PINCE_BAS_AV;
            }
            else
            {
                idAx12 = PINCE_BAS_AR;
            }
            break;
        case PINCE_HAUT :
            if (cote == AVANT)
            {
                idAx12 = PINCE_HAUT_AV;
            }
            else
            {
                idAx12 = PINCE_HAUT_AR;
            }
            break;
        case ASCENSEUR :
            if (cote == AVANT)
            {
                idAx12 = ASC_AVANT;
            }
            else
            {
                idAx12 = ASC_ARRIERE;
            }
            break;
        case RETOURNE_MODULE :
            if (cote == AVANT)
            {
                idAx12 = BITE_AV;
            }
            else
            {
                idAx12 = BITE_AR;
            }
            break;        
    }
    
    return idAx12;
}

bool check_capteur_pince (_cote cote)
{
    bool result = false;
    
    if (cote == AVANT)
    {
        result = (CAPT_PINCE_AV == ETAT_CAPTEUR_PINCE_AV);
    }
    else if (cote == ARRIERE)
    {
        result = (CAPT_PINCE_AR == ETAT_CAPTEUR_PINCE_AR);
    }
    
    return result;
}

/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/

// INITIALISATION

void init_system_avant()
{
    const uint8_t tentatives_max = 5;
    uint8_t tentatives = 0;
    int16_t position_asc = 0;
       
    do
    {
        position_asc = read_data(ASC_AVANT, LIRE_POSITION_ACTU);
        delay_ms(50);
    }while(position_asc == -1 && tentatives < tentatives_max);
    
    // pas d'echec lecture et on n'est pas en position haute
    if (position_asc != -1 && position_asc > ASC_AV_POS_INIT_MAX)
    {
        angle_AX12(PINCE_BAS_AV, PINCE_POS_OUVERTE, VITESSE_MAX_AX, SANS_ATTENTE);
        
        // on synchronise l'ax
        do
        {
            delay_ms(300);
        }while(read_data(PINCE_BAS_AV, LIRE_MOUV_FLAG) != 0);
    }
    
    // On monte l'asc à la position d'init
    angle_AX12(ASC_AVANT, ASC_AV_POS_INIT, VITESSE_MAX_AX, SANS_ATTENTE);
    do
    {
        delay_ms(300);
    }while(read_data(ASC_AVANT, LIRE_MOUV_FLAG) != 0);

    // On range les pinces
    angle_AX12(PINCE_BAS_AV, PINCE_POS_REPLI, VITESSE_MAX_AX, SANS_ATTENTE);
    angle_AX12(PINCE_HAUT_AV, PINCE_POS_REPLI, VITESSE_MAX_AX, SANS_ATTENTE);
}

void init_system_arriere()
{
    const uint8_t tentatives_max = 5;
    uint8_t tentatives = 0;
    int16_t position_asc = 0;
       
    do
    {
        position_asc = read_data(ASC_ARRIERE, LIRE_POSITION_ACTU);
        delay_ms(50);
        printf("\n\rerreur lectire");
    }while(position_asc == -1 && tentatives < tentatives_max);
    
    // pas d'echec lecture et on n'est pas en position haute
    if (position_asc != -1 && position_asc > ASC_AR_POS_INIT_MAX)
    {
        angle_AX12(PINCE_BAS_AR, PINCE_POS_OUVERTE, VITESSE_MAX_AX, SANS_ATTENTE);
        
        // on synchronise l'ax
        do
        {
            delay_ms(300);
        }while(read_data(PINCE_BAS_AR, LIRE_MOUV_FLAG) != 0);
    }
    
    // On monte l'asc à la position d'init
    angle_AX12(ASC_ARRIERE, ASC_AR_POS_INIT, VITESSE_MAX_AX, SANS_ATTENTE);
    do
    {
        delay_ms(300);
    }while(read_data(ASC_ARRIERE, LIRE_MOUV_FLAG) != 0);

    // On range les pinces
    angle_AX12(PINCE_BAS_AR, PINCE_POS_REPLI, VITESSE_MAX_AX, SANS_ATTENTE);
    angle_AX12(PINCE_HAUT_AR, PINCE_POS_REPLI, VITESSE_MAX_AX, SANS_ATTENTE);
}


// Fonctions utiles

void fermer_pinces_bas (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AV, PINCE_POS_FERME, 600, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AR, PINCE_POS_FERME, 600, SANS_ATTENTE);
    }
}

void fermer_pinces_haut (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AV, PINCE_POS_FERME, 250, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AR, PINCE_POS_FERME, 250, SANS_ATTENTE);
    }
}

void ouvrir_pinces_bas (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AV, PINCE_POS_OUVERTE, 512, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AR, PINCE_POS_OUVERTE, 512, SANS_ATTENTE);
    }
}

void ouvrir_pinces_haut (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AV, PINCE_POS_ENTROUVERTE, 512, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AR, PINCE_POS_ENTROUVERTE, 512, SANS_ATTENTE);
    }
}

void descendre_ascenseur (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(ASC_AVANT, ASC_AV_POS_BAS, 250, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(ASC_ARRIERE, ASC_AR_POS_BAS, 250, SANS_ATTENTE);
    }
}

void monter_ascenseur (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(ASC_AVANT, ASC_AV_POS_HAUT, 250, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(ASC_ARRIERE, ASC_AR_POS_HAUT, 250, SANS_ATTENTE);
    }
}

/******************************************************************************/
/******************************* AUTOM HAUT NIVEAU  ***************************/
/******************************************************************************/

/******************/
/*** INIT ROBOT ***/
/******************/

// ETAPE 1 : ou ouvre toutes les pinces
void SR_start_robot()
{
    ouvrir_pinces_bas(LES_DEUX);
    ouvrir_pinces_haut(LES_DEUX);
    
    register_multiple_ax12_event(2, AUTOM_AVANT, SR_PINCES_OUVERTES, 200, 
            PINCE_BAS_AV, PINCE_HAUT_AV);
    register_multiple_ax12_event(2, AUTOM_ARRIERE, SR_PINCES_OUVERTES, 200, 
            PINCE_BAS_AR, PINCE_HAUT_AR);
    register_sync_event(AUTOM_PRINCIPALE, SR_ROBOT_READY, 0, 2, 
            AUTOM_AVANT,   SR_ROBOT_READY, 
            AUTOM_ARRIERE, SR_ROBOT_READY  );
    
}

// ETAPE 2 : On descend l'ascenseur
void SR_descendre_ascenseur(_cote cote) 
{
    descendre_ascenseur(cote);
    register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, SR_ROBOT_READY, 150);
}


void montage_tour(_cote cote)
{
    if (modules_tour[cote] < 3)
    {
        FLAG_ACTION[cote] = MT_RECHERCHE_MODULE_EN_COURS;
    }
    else
    {
        FLAG_ACTION[cote] = MT_TOUR_COMPLETE;
    }
}

/********************/
/*** MONTAGE TOUR ***/
/********************/

/**
 * MT_recherche_modules_pince()
 * ETATE 1 : on recherche un module dans les pinces
 * @param cote
 */
void MT_recherche_modules_pince(_cote cote)
{
    if ( check_capteur_pince(cote) )
    {
        // On attends 300 ms pour vérfier que le module est bien dans la pince
        // et que ce n'est pas un faux positif
        arm_timer_event(cote, 300, MT_MODULE_DETECTE, true);
    }
}

/**
 * MT_serrage_module_apres_detection()
 * ETAPE 2 : Le module est detecté : on commence à le serrer
 * @param cote
 */
void MT_serrage_module_apres_detection(_cote cote)
{
    // on recheck les capteurs, si toujours présent, 
    // alors on a vraiment détecté un module
    if ( check_capteur_pince(cote) )
    {
        // on ferme les pinces sur le module
        // Et on attends confirmation de la fermeture
        modules_tour[cote]++;
        fermer_pinces_bas(cote);
        register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, MT_MODULE_ATTRAPE, 0); // TODO : change timer
    }
    else // on recommence la recherche
    {
        FLAG_ACTION[cote] = MT_RECHERCHE_MODULE_EN_COURS;
    }
}

/**
 * MT_ouverture_pince_haut_avant_montee()
 * ETAPE 3 : La pince du bas est fermé, on ouvre la pince du haut
 * @param cote
 */
void MT_ouverture_pince_haut_avant_montee (_cote cote)
{
    // Tour complète = montage finit
    if (modules_tour[cote] >= 3)
    {
        FLAG_ACTION[cote] = MT_TOUR_COMPLETE;
    }
    else // on monte la tour en hauteur
    {
        ouvrir_pinces_haut(cote);
        FLAG_ACTION[cote] = MT_PRET_A_MONTER;
//        register_ax12_event(getIdAx12(PINCE_HAUT, cote), cote, MT_PRET_A_MONTER, 0); 
    }
}

/**
 * MT_montage_du_module()
 * ETAPE 4 : on monte l'ascenseur avec le module
 * @param cote
 */
void MT_montage_du_module (_cote cote)
{
    monter_ascenseur(cote);
    register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, MT_ASC_EN_HAUT , 500); // TODO : CHANGE EVENT, change timeout
}

/**
 * MT_attrape_module_en_haut()
 * ETAPE 5 : On réceptionne le module en haut
 * @param cote
 */
void MT_attrape_module_en_haut (_cote cote)
{
    fermer_pinces_haut(cote);
    register_ax12_event(getIdAx12(PINCE_HAUT, cote), cote, MT_MODULE_ATTRAPE_EN_HAUT, 750); // TODO CHANGE timer
}

/**
 * MT_ouvrir_pinces_bas_avant_redescente()
 * ETAPE 6 : on rouvre la pince du bas, pour pouvoir redescendre l'ascenseur
 * @param cote
 */
void MT_ouvrir_pinces_bas_avant_redescente(_cote cote)
{
    ouvrir_pinces_bas(cote);
    register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, MT_PRET_A_REDESCENDRE, 0); // TODO CHANGE event
}

/**
 * MT_redescendre_ascensseur()
 * ETAPE 7 : on redescend l'ascenseur : on est pret pour remonter un autre module
 * @param cote
 */
void MT_redescendre_ascensseur(_cote cote)
{
    descendre_ascenseur(cote);
    register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, MONTAGE_TOUR_PRET, 200); // Change event / timer
}

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_20ms (void)
{
    _autom_id autom_id;

    check_autom_events();
    
    //fonction qui definit les actions
    switch (FLAG_ACTION[AUTOM_PRINCIPALE])
    {
        case NE_RIEN_FAIRE:
        case EN_ATTENTE_EVENT :
            // do nothing
            break;
        case SR_START_ROBOT :
            SR_start_robot();
            break;
        case SR_ROBOT_READY :
//            FLAG_ACTION = NE_RIEN_FAIRE;
            arm_timer_event(AUTOM_AVANT, 2000, MONTAGE_TOUR_PRET, false);
            arm_timer_event(AUTOM_ARRIERE, 2000, MONTAGE_TOUR_PRET, false);
            register_sync_event(AUTOM_PRINCIPALE, MT_TOUR_COMPLETE, 500, 2, 
                    AUTOM_AVANT,   MT_TOUR_COMPLETE,
                    AUTOM_ARRIERE, MT_TOUR_COMPLETE);
            break;
        case MT_TOUR_COMPLETE :
            // do nothing
            break;
        default :
            break;
    }
    
    for (autom_id = AUTOM_AVANT ; autom_id <= AUTOM_ARRIERE ; autom_id++)
    {
        switch(FLAG_ACTION[autom_id])
        {
            // Event standard
            case NE_RIEN_FAIRE:
            case EN_ATTENTE_EVENT :
                // do nothing
                break;
                
            // Event Init Robot
            case SR_PINCES_OUVERTES :
                SR_descendre_ascenseur(autom_id);
                break;
            case SR_ROBOT_READY :
                // do nothing (here to wait synchro)
                break;

            // Event Montage tour
            case MONTAGE_TOUR_PRET :
                montage_tour(autom_id);
                break;
            case MT_RECHERCHE_MODULE_EN_COURS :
                MT_recherche_modules_pince(autom_id);
                break;
            case MT_MODULE_DETECTE :
                MT_serrage_module_apres_detection(autom_id);
                break;
            case MT_MODULE_ATTRAPE :
                MT_ouverture_pince_haut_avant_montee(autom_id);
                break;
            case MT_PRET_A_MONTER :
                MT_montage_du_module(autom_id);
                break;
            case MT_ASC_EN_HAUT :
                MT_attrape_module_en_haut(autom_id);
                break;
            case MT_MODULE_ATTRAPE_EN_HAUT :
                MT_ouvrir_pinces_bas_avant_redescente(autom_id);
                break;
            case MT_PRET_A_REDESCENDRE :
                MT_redescendre_ascensseur(autom_id);
                break;
            case MT_TOUR_COMPLETE :
                // do nothing
                break;
            default :
                break;
        }
    }
    
//    if (EVITEMENT_ADV.actif == true && EVITEMENT_ADV.detection == OFF)
//    {
////        if (EVITEMENT_ADV.sens == MARCHE_AVANT)
////            rotation_us_avant();
////        else
////            rotation_us();
//    }
//    //else
//        //position standart
    
}


void autom_avant()
{
    
}

void autom_arriere()
{
    
}

#endif