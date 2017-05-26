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

#ifdef PETIT_ROBOT

/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

volatile bool inihibit_montage_tour[LES_DEUX] = {0};
static uint8_t modules_tour[LES_DEUX] = {0};
static const uint8_t hauteur_tour_max = 2;

/******************************************************************************/
/***************************** FONCTIONS DIVERSES *****************************/
/******************************************************************************/

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

uint8_t get_module_tour(_cote cote)
{
    uint8_t nb_modules = 0;
    
    if (cote == AVANT || cote == LES_DEUX)
    {
        nb_modules += modules_tour[AVANT];
    }
    else if (cote == ARRIERE || cote == LES_DEUX)
    {
        nb_modules += modules_tour[ARRIERE];
    }
    
    return nb_modules;
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
    
    angle_AX12(BITE_AV, BITE_POS_INIT, VITESSE_MAX_AX, SANS_ATTENTE);    
    
    delay_ms(1000);
    
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
    
    delay_ms(850);
    
    angle_AX12(BITE_AV, BITE_POS_RANGE, VITESSE_MAX_AX, SANS_ATTENTE);
}

void init_system_arriere()
{
    const uint8_t tentatives_max = 5;
    uint8_t tentatives = 0;
    int16_t position_asc = 0;
    
    angle_AX12(BITE_AR, BITE_POS_INIT, VITESSE_MAX_AX, SANS_ATTENTE);
    delay_ms(1000);
       
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
    
    delay_ms(850);
    
    angle_AX12(BITE_AR, BITE_POS_RANGE, VITESSE_MAX_AX, SANS_ATTENTE);
}


// Fonctions utiles

void fermer_pinces_bas (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AV, PINCE_POS_FERME, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AR, PINCE_POS_FERME, 1023, SANS_ATTENTE);
    }
}

void fermer_pinces_haut (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AV, PINCE_POS_FERME, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AR, PINCE_POS_FERME, 1023, SANS_ATTENTE);
    }
}

void ouvrir_pinces_bas (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AV, PINCE_POS_OUVERTE, VITESSE_MAX_AX, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AR, PINCE_POS_OUVERTE, VITESSE_MAX_AX, SANS_ATTENTE);
    }
}

void ouvrir_pinces_haut (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AV, PINCE_POS_ENTROUVERTE, VITESSE_MAX_AX, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AR, PINCE_POS_ENTROUVERTE, VITESSE_MAX_AX, SANS_ATTENTE);
    }
}

void repli_pince_haut(_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AV, PINCE_POS_REPLI, VITESSE_MAX_AX, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_HAUT_AR, PINCE_POS_REPLI, VITESSE_MAX_AX, SANS_ATTENTE);
    }
}

void descendre_ascenseur (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(ASC_AVANT, ASC_AV_POS_BAS, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(ASC_ARRIERE, ASC_AR_POS_BAS, 1023, SANS_ATTENTE);
    }
}

void monter_ascenseur (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(ASC_AVANT, ASC_AV_POS_HAUT, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(ASC_ARRIERE, ASC_AR_POS_HAUT, 1023, SANS_ATTENTE);
    }
}

void depose_ascenseur_from_bas (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(ASC_AVANT, ASC_AV_POS_DEPOSE_BAS, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(ASC_ARRIERE, ASC_AR_POS_DEPOSE_BAS, 1023, SANS_ATTENTE);
    }
}

void depose_ascenseur_from_haut (_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(ASC_AVANT, ASC_AV_POS_DEPOSE_BAS, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(ASC_ARRIERE, ASC_AR_POS_DEPOSE_BAS, 1023, SANS_ATTENTE);
    }
}

uint16_t get_asc_pos_depose_haut(_cote cote)
{
    if (cote == AVANT)
    {
        return ASC_AV_POS_DEPOSE_HAUT;
    }
    else
    {
        return ASC_AR_POS_DEPOSE_HAUT;
    }
}

uint16_t get_asc_pos_depose_bas(_cote cote)
{
    if (cote == AVANT)
    {
        return ASC_AV_POS_DEPOSE_BAS;
    }
    else 
    {
        return ASC_AR_POS_DEPOSE_BAS;
    }
}

void degage_depose_module(_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(BITE_AV, BITE_POS_INIT, VITESSE_MAX_AX, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(BITE_AR, BITE_POS_INIT, VITESSE_MAX_AX, SANS_ATTENTE);
    }
}

void ouvrir_depose_module(_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(BITE_AV, BITE_POS_OUVERTE, 1023, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(BITE_AR, BITE_POS_OUVERTE, 1023, SANS_ATTENTE);
    } 
}

void fermer_depose_module(_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        angle_AX12(BITE_AV, BITE_POS_FERME, VITESSE_MAX_AX, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(BITE_AR, BITE_POS_FERME, VITESSE_MAX_AX, SANS_ATTENTE);
    }
}

/******************************************************************************/
/******************************* AUTOM HAUT NIVEAU  ***************************/
/******************************************************************************/

/******************/
/*** INIT ROBOT ***/
/******************/

// ETAPE 1 : ou ouvre les dépose modules
void SR_start_robot()
{
    degage_depose_module(LES_DEUX);
    register_ax12_event(BITE_AV, AUTOM_AVANT, SR_DEPOSE_MODULE_OUVERT, 0);
    register_ax12_event(BITE_AR, AUTOM_ARRIERE, SR_DEPOSE_MODULE_OUVERT, 0);
    
    register_sync_event(AUTOM_PRINCIPALE, SR_ROBOT_READY, 0, 2, 
        AUTOM_AVANT,   SR_ROBOT_READY, 
        AUTOM_ARRIERE, SR_ROBOT_READY  );
}

// ETAPE 2 : On ouvre toutes les pinces 
void SR_ouvrir_pinces (_cote cote)
{
    ouvrir_pinces_bas(cote);
    ouvrir_pinces_haut(cote);
    
    register_multiple_ax12_event(2, cote, SR_PINCES_OUVERTES, 100, 
            getIdAx12(PINCE_BAS, cote),
            getIdAx12(PINCE_HAUT, cote));
}

// ETAPE 3 : On descend l'ascenseur, et on met le depose module a sa bonne position
void SR_descendre_ascenseur(_cote cote) 
{
    descendre_ascenseur(cote);
    ouvrir_depose_module(cote);
            
    register_multiple_ax12_event(2, cote, SR_ROBOT_READY, 50, 
        getIdAx12(ASCENSEUR, cote),
        getIdAx12(RETOURNE_MODULE, cote));
}


// ETAPE 1 : ou ouvre les dépose modules
void SR_start_robot_differe()
{    
    register_sync_event(AUTOM_PRINCIPALE, SR_ROBOT_READY, 0, 2, 
        AUTOM_AVANT,   SR_ROBOT_READY, 
        AUTOM_ARRIERE, SR_ROBOT_READY  );
}

void SR_start_robot_cote(_cote cote)
{
    degage_depose_module(cote);
    register_ax12_event(getIdAx12(RETOURNE_MODULE, cote), cote, SR_DEPOSE_MODULE_OUVERT, 0);
}

void montage_tour(_cote cote)
{
    if (modules_tour[cote] < hauteur_tour_max)
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
        arm_timer_event(cote, 100, MT_MODULE_DETECTE, true);
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
    if (modules_tour[cote] >= hauteur_tour_max || inihibit_montage_tour[cote] == true) //TODO
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
    register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, MT_ASC_EN_HAUT , 200); // TODO : CHANGE EVENT, change timeout
}

/**
 * MT_attrape_module_en_haut()
 * ETAPE 5 : On réceptionne le module en haut
 * @param cote
 */
void MT_attrape_module_en_haut (_cote cote)
{
    fermer_pinces_haut(cote);
    register_ax12_event(getIdAx12(PINCE_HAUT, cote), cote, MT_MODULE_ATTRAPE_EN_HAUT, 350); // TODO CHANGE timer
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

/**************************/
/*** DEPOSE DES MODULES ***/
/**************************/

/** DEPOSE CAS 1 ***/
void DX_depose_module(_cote cote)
{
    ouvrir_depose_module(cote);
    if (check_capteur_pince(cote))
    {
        arm_timer_event(cote, 800, DX_DEPOSE_READY, true);
//        register_ax12_event(getIdAx12(RETOURNE_MODULE, cote), cote, DX_DEPOSE_READY, 200);
    }
    else if (modules_tour[cote] > 0)
    {
        arm_timer_event(cote, 800, DINT_START_DEPOSE, true);
//        register_ax12_event(getIdAx12(RETOURNE_MODULE, cote), cote, DINT_START_DEPOSE, 200);
    }
    else
    {
        arm_timer_event(cote, 800, DX_DEPOSE_FINIT, true);
        register_ax12_event(getIdAx12(RETOURNE_MODULE, cote), cote, DX_DEPOSE_FINIT, 200);
    }
}

void DINT_ouverture_pince_bas_avant_montee (_cote cote)
{
    ouvrir_pinces_bas(cote);
    register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, DINT_PINCE_BAS_OUVERTE, 100);
}

void DINT_monter_ascenseur(_cote cote)
{
    monter_ascenseur(cote);
    register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, DINT_ASC_EN_HAUT, 200);
}

void DINT_recup_module (_cote cote)
{
    fermer_pinces_bas(cote);
    register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, DINT_MODULE_ATTRAPE, 200);
}

void DINT_relacher_tour (_cote cote)
{
    ouvrir_pinces_haut(cote);
    register_ax12_event(getIdAx12(PINCE_HAUT, cote), cote, DINT_PRET_A_DESCENDRE_TOUR, 40); 
}

void DINT_descendre_tour(_cote cote)
{
//    if (check_capteur_pince(cote))
//    {
        descendre_ascenseur(cote);
        register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, DINT_TOUR_EN_BAS, 100);
//    }
//    else
//    {
//        modules_tour[cote]--;
//        FLAG_ACTION[cote] = DX_DEPOSE_FINIT;
//    }
}

void DINT_fermer_pince_haut(_cote cote)
{
    fermer_pinces_haut(cote);
    register_ax12_event(getIdAx12(PINCE_HAUT, cote), cote, DX_DEPOSE_READY, 300);
}

void DX_ouverture_pince_haut_avant_montee (_cote cote)
{
    ouvrir_pinces_haut(cote);
    FLAG_ACTION[cote] = DX_PRET_A_ALLER_EN_DEPOSE; 
    
}

/**
 * on DX_PRET_A_ALLER_EN_DEPOSE
 * @param cote
 */
void DX_positionnement_pour_depose(_cote cote)
{
    if (position_AX12[getIdAx12(ASCENSEUR, cote)].point == get_asc_pos_depose_haut(cote))
    {
        depose_ascenseur_from_haut(cote);
    }
    else
    {
       depose_ascenseur_from_bas(cote); 
    }
    
    register_ax12_event(getIdAx12(ASCENSEUR, cote), cote, DX_ASCENSEUR_POS_DEPOSE, 400);
}

void DX_resserage_tour(_cote cote)
{
    fermer_pinces_haut(cote);
    fermer_depose_module(cote);
    
    register_multiple_ax12_event(1, cote, DX_PRET_A_DEPOSER, 150, 
        getIdAx12(PINCE_HAUT, cote));
//    register_multiple_ax12_event(2, cote, DX_PRET_A_DEPOSER, 150, 
//        getIdAx12(PINCE_HAUT, cote),
//        getIdAx12(RETOURNE_MODULE, cote));
}

void DX_relache_module (_cote cote)
{
    ouvrir_pinces_bas(cote);
    modules_tour[cote]--;
    inihibit_montage_tour[cote] = false;
    register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, DX_DEPOSE_FINIT, 400);
}

void DX_refermer_depose_module(_cote cote)
{
//    ouvrir_depose_module(cote);
//    register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, DX_DEPOSE_FINIT, 800);
}


/*********************/
/*** INTERRACTIONS ***/
/*********************/

/**
 * on AF_PREPARE_FUSEE
 * @param cote
 */
void AF_prepare_fusee (_cote cote)
{
    ouvrir_depose_module(cote);
    repli_pince_haut(cote);
    ouvrir_pinces_bas(cote);
    descendre_ascenseur(cote);

    register_multiple_ax12_event(4, cote, AF_CHOPAGE_MODULE_READY, 500,
            getIdAx12(PINCE_BAS, cote),
            getIdAx12(PINCE_BAS, cote),
            getIdAx12(RETOURNE_MODULE, cote),
            getIdAx12(ASCENSEUR, cote));
}


/*********************/
/*** ATTRAPE FUSEE ***/
/*********************/

//void AF_choppage_module (_cote cote)
//{
//    if (modules_tour[cote] == 0)
//    {
//        repli_pince_haut(cote);
//        ouvrir_pinces_bas(cote);
//        ouvrir_depose_module(cote);
//        
//        register_multiple_ax12_event(3, cote, AF_CHOPAGE_MODULE_READY, 500,
//                getIdAx12(PINCE_BAS, cote),
//                getIdAx12(PINCE_BAS, cote),
//                getIdAx12(RETOURNE_MODULE, cote));
//    }
//    else
//    {
//        // TODO
//    }
//}

/**
 * on AF_ATTRAPE_FUSEE
 * @param cote
 */
void AF_detecte_module(_cote cote)
{
    if (check_capteur_pince(cote))
    {
        arm_timer_event(cote, 100, AF_MODULE_DETECTE, true);
    }
}

/**
 * on AF_MODULE_DETECTE
 * @param cote
 */
void AF_attrape_module (_cote cote)
{
    // on vérufie qu'on a toujours un module deant soit
    if (check_capteur_pince(cote))
    {
        fermer_pinces_bas(cote);
        register_ax12_event(getIdAx12(PINCE_BAS, cote), cote, AF_MODULE_ATTRAPE, 500);
    }
    else
    {
        FLAG_ACTION[cote] = AF_ATTRAPE_FUSEE;
    }
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
        case SR_START_ROBOT_DIFFERE :
            SR_start_robot_differe();
            break;
        case SR_ROBOT_READY :
//            FLAG_ACTION = NE_RIEN_FAIRE;
//            FLAG_ACTION[AUTOM_AVANT] = MONTAGE_TOUR_PRET;
            
            arm_timer_event(AUTOM_AVANT, 200, MONTAGE_TOUR_PRET, false);
            arm_timer_event(AUTOM_ARRIERE, 200, MONTAGE_TOUR_PRET, false);
            register_sync_event(AUTOM_PRINCIPALE, MT_TOUR_COMPLETE, 500, 2, 
                    AUTOM_AVANT,   MT_TOUR_COMPLETE,
                    AUTOM_ARRIERE, MT_TOUR_COMPLETE);
            
//            arm_timer_event(AUTOM_AVANT, 2000, AF_ATTRAPE_FUSEE, true);
//            arm_timer_event(AUTOM_ARRIERE, 2000, AF_ATTRAPE_FUSEE, true);
//            FLAG_ACTION[AUTOM_PRINCIPALE] = NE_RIEN_FAIRE;
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
            case SR_START_ROBOT_COTE :
                SR_start_robot_cote(autom_id);
                break;
            case SR_DEPOSE_MODULE_OUVERT :
                SR_ouvrir_pinces(autom_id);
                break;
            case SR_PINCES_OUVERTES :
                SR_descendre_ascenseur(autom_id);
                break;
            case SR_ROBOT_READY :
//                arm_timer_event(autom_id, 200, MONTAGE_TOUR_PRET, false);
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
                
                
            // Event Depose tour :
            case DX_START_DEPOSE :
                DX_depose_module(autom_id);
                break;
            case DINT_START_DEPOSE :
                DINT_ouverture_pince_bas_avant_montee(autom_id);
                break;
            case DINT_PINCE_BAS_OUVERTE :
                DINT_monter_ascenseur(autom_id);
                break;
            case DINT_ASC_EN_HAUT :
                DINT_recup_module(autom_id);
                break;
            case DINT_MODULE_ATTRAPE :
                DINT_relacher_tour(autom_id);
                break;
            case DINT_PRET_A_DESCENDRE_TOUR :
                DINT_descendre_tour(autom_id);
                break;
            case DINT_TOUR_EN_BAS :
                DINT_fermer_pince_haut(autom_id);
                break;
            case DX_DEPOSE_READY :
                // DO NOTHING
                break;
            case DX_INIT_DEPOSE :
                DX_ouverture_pince_haut_avant_montee(autom_id);
                break;
            case DX_PRET_A_ALLER_EN_DEPOSE :
                DX_positionnement_pour_depose(autom_id);
                break;
            case DX_ASCENSEUR_POS_DEPOSE :
                DX_resserage_tour(autom_id);
                break;
            case DX_PRET_A_DEPOSER :
                DX_relache_module(autom_id);
                break;
            case DX_MODULE_RELACHE :
                DX_refermer_depose_module(autom_id);
                break;
            case DX_DEPOSE_FINIT :
                break;
                
            // event attrape module
            case AF_PREPARE_FUSEE :
                AF_prepare_fusee(autom_id);
                break;
            case AF_CHOPAGE_MODULE_READY :
                // Do nothing
                break;
            case AF_ATTRAPE_FUSEE :
                AF_detecte_module(autom_id);
                break;
            case AF_MODULE_DETECTE :
                AF_attrape_module(autom_id);
                break;
            case AF_MODULE_ATTRAPE :
                // Do nothing
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