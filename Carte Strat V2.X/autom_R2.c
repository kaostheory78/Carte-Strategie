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

static uint8_t modules_tour_avant = 0;
static uint8_t modules_tour_arriere = 0;

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

  */}

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
        angle_AX12(PINCE_BAS_AV, PINCE_POS_FERME, 100, SANS_ATTENTE);
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        angle_AX12(PINCE_BAS_AR, PINCE_POS_FERME, 100, SANS_ATTENTE);
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

// ETAPE 1 : ou ouvre les pinces
void start_robot()
{
    ouvrir_pinces_bas(LES_DEUX);
    ouvrir_pinces_haut(LES_DEUX);
    FLAG_ACTION = WAIT_PINCES_OUVERTES;
}

// ETAPE 2 : On descend l'ascenseur
void attente_ouverture_pinces()
{
    static bool status_pinces_av = false;
    static bool status_pinces_ar = false;
    
    if (status_pinces_av == false)
    {
        if (is_target_ax12_reachead(PINCE_BAS_AV))
        {
            status_pinces_av = true;
            descendre_ascenseur (AVANT);
        }
    }
    
    if (status_pinces_ar == false)
    {
        if (is_target_ax12_reachead(PINCE_BAS_AR))
        {
            status_pinces_ar = true;
            descendre_ascenseur (ARRIERE);
        }
    }
    
    if (status_pinces_av == true && status_pinces_ar == true)
    {
        arm_timer(200, WAIT_INIT_ROBOT_COMPLETE);
        FLAG_ACTION = EN_ATTENTE_EVENT;    
    }
}

// ETAPE3 : on attend que  tous les ax12 soient arrivés en position finales
void wait_init_robot_complete()
{
    static bool status_pinces_av = false;
    static bool status_pinces_ar = false;
    static bool status_asc_av = false;
    static bool status_asc_ar = false;
    
    if ( status_pinces_av == false)
    {
        if (is_target_ax12_reachead(PINCE_HAUT_AV) == true)
        {
            status_pinces_av = true;
        }
    }
    
    if ( status_pinces_ar == false)
    {
        if (is_target_ax12_reachead(PINCE_HAUT_AR) == true)
        {
            status_pinces_ar = true;
        }
    }
    
    if ( status_asc_av == false)
    {
        if (is_target_ax12_reachead(ASC_AVANT) == true)
        {
            status_asc_av = true;
        }
    }
    
    if ( status_asc_ar == false)
    {
        if (is_target_ax12_reachead(ASC_ARRIERE) == true)
        {
            status_asc_ar = true;
        }
    }
    
    if (status_pinces_av == true && status_pinces_ar == true 
        && status_asc_av == true && status_asc_ar == true)
    {
        // Robot initialisé, on attends 400 ms pour lancer l'event
        arm_timer(400, INIT_ROBOT_COMPLETE);
        FLAG_ACTION = EN_ATTENTE_EVENT;
    }
}

void montage_tour()
{
    if (modules_tour_avant < 3)
    {
        FLAG_ACTION = RECHERCHE_MODULE_AVANT;
    }
    else if (modules_tour_arriere <  3)
    {
        FLAG_ACTION = RECHERCHE_MODULE_ARRIERE;
    }
    else
        FLAG_ACTION = TOUR_COMPLETE;
}

/********************/
/*** MONTAGE TOUR ***/
/********************/

void recherche_modules_pince(_cote cote)
{
    if (cote == AVANT || cote == LES_DEUX)
    {
        if (CAPT_PINCE_AV == ETAT_CAPTEUR_PINCE_AV)
        {
            // On attends 300 ms pour vérfier que le module est bien dans la pince
            // et que ce n'est pas un faux positif
            arm_timer(AUTOM_AVANT, 300, MODULE_DETECTED_AVANT, true);          
        }
    }
    
    if (cote == ARRIERE || cote == LES_DEUX)
    {
        if (CAPT_PINCE_AR == ETAT_CAPTEUR_PINCE_AR)
        {
            // On attends 300 ms pour vérfier que le module est bien dans la pince
            // et que ce n'est pas un faux positif
            arm_timer(AUTOM_ARRIERE, 300, MODULE_DETECTED_AVANT, true);     
        }
    }
}

void debut_attrape_module (_cote cote)
{
    if (cote == AVANT)
    {
        // Faux positif, on reprend la recherche
        if (CAPT_PINCE_AV != ETAT_CAPTEUR_PINCE_AV)
        {
            FLAG_ACTION = RECHERCHE_MODULE_AVANT;
        }
        else // on commence la procédure
        {
            modules_tour_avant++;
            fermer_pinces_bas(AVANT);
            arm_timer(200, ATTENTE_MONTEE_AVANT);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
    else if (cote == ARRIERE)
    {
        // Faux positif, on reprend la recherche
        if (CAPT_PINCE_AR != ETAT_CAPTEUR_PINCE_AR)
        {
            FLAG_ACTION = RECHERCHE_MODULE_ARRIERE;
        }
        else // on commence la procédure
        {
            modules_tour_arriere++;
            fermer_pinces_bas(ARRIERE);
            arm_timer(200, ATTENTE_MONTEE_ARRIERE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
}

void attente_fermeture_pinces_avant_montee (_cote cote)
{
    if (cote == AVANT)
    {
        // Pinces pas encore fermée, on attend
        if (is_target_ax12_reachead(PINCE_BAS_AV) == false)
        {
            arm_timer(200, ATTENTE_MONTEE_AVANT);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // Tour complète = montage finit
            if (modules_tour_avant >= 3)
            {
                FLAG_ACTION = PROCESS_MODULE_READY;
            }
            else // on monte la tour en hauteur
            {
                ouvrir_pinces_haut(cote);
                monter_ascenseur(cote);
                arm_timer(200, ASC_AVANT_EN_MONTEE);
                FLAG_ACTION = EN_ATTENTE_EVENT;
            }
        }
    }
    else if (cote == ARRIERE)
    {
        // Pinces pas encore fermée, on attend
        if (is_target_ax12_reachead(PINCE_BAS_AR) == false)
        {
            arm_timer(200, ATTENTE_MONTEE_ARRIERE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // Tour complète = montage finit
            if (modules_tour_arriere >= 3)
            {
                FLAG_ACTION = PROCESS_MODULE_READY;
            }
            else // onmonte la tour en hauteur
            {
                ouvrir_pinces_haut(cote);
                monter_ascenseur(cote);
                arm_timer(200, ASC_ARRIERE_EN_MONTEE);
                FLAG_ACTION = EN_ATTENTE_EVENT;
            }
        }
    }

}

void attente_ascenseur_en_haut (_cote cote)
{
    if (cote == AVANT)
    {
        // Ascenseur pas encore arrivé : on attend
        if (is_target_ax12_reachead(ASC_AVANT) == false)
        {
            arm_timer(200, ASC_AVANT_EN_MONTEE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // On referme les pinces du haut
            fermer_pinces_haut(cote);
            arm_timer(500, FERMETURE_PINCE_HAUT_AVANT);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
    else if (cote == ARRIERE)
    {
        // Pinces pas encore fermée, on attend
        if (is_target_ax12_reachead(ASC_ARRIERE) == false)
        {
            arm_timer(200, ASC_ARRIERE_EN_MONTEE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // On referme les pinces du haut
            fermer_pinces_haut(cote);
            arm_timer(500, FERMETURE_PINCE_HAUT_ARRIERE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }

}

void attente_fermeture_pince_en_haut (_cote cote)
{
    if (cote == AVANT)
    {
        // Pinces pas encore fermée, on attend
        if (is_target_ax12_reachead(PINCE_HAUT_AV) == false)
        {
            arm_timer(200, FERMETURE_PINCE_HAUT_AVANT);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // On attends avant de rouvrir  les pinces du bas
            arm_timer(750, PREPARE_REDESCENTE_AVANT);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
    else if (cote == ARRIERE)
    {
        // Pinces pas encore fermée, on attend
        if (is_target_ax12_reachead(PINCE_HAUT_AR) == false)
        {
            arm_timer(200, FERMETURE_PINCE_HAUT_ARRIERE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // On attends avant de rouvrir  les pinces du bas
            arm_timer(750, PREPARE_REDESCENTE_ARRIERE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
}

void ouvrir_pince_pour_redescenre(_cote cote)
{
    if (cote == AVANT)
    {

        // Les pinces du haut sont fermés, on peut rouvrir
        // Les pinces du bas pour redescendre l'ascenseur
        ouvrir_pinces_bas(cote);
        arm_timer(200, ATTENTE_REDESCENTE_AVANT);
        FLAG_ACTION = EN_ATTENTE_EVENT;

    }
    else if (cote == ARRIERE)
    {
        // Les pinces du haut sont fermés, on peut rouvrir
        // Les pinces du bas pour redescendre l'ascenseur
        ouvrir_pinces_bas(cote);
        arm_timer(200, ATTENTE_REDESCENTE_ARRIERE);
        FLAG_ACTION = EN_ATTENTE_EVENT;

    }
}

void attente_ouverture_pince_pour_redescente (_cote cote)
{
    if (cote == AVANT)
    {
        // Pinces pas encore ouverte, on attend
        if (is_target_ax12_reachead(PINCE_BAS_AV) == false)
        {
            arm_timer(200, ATTENTE_REDESCENTE_AVANT);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // Les pinces sont ouvertes on peut descendre l'ascenseur
            descendre_ascenseur(cote);
            arm_timer(500, ATTENTE_CYCLE_MONTAGE_AVANT_COMPLET);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
    else if (cote == ARRIERE)
    {
        // Pinces pas encore ouverte, on attend
        if (is_target_ax12_reachead(PINCE_HAUT_AR) == false)
        {
            arm_timer(200, ATTENTE_REDESCENTE_ARRIERE);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // Les pinces sont ouvertes on peut descendre l'ascenseur
            descendre_ascenseur(cote);
            arm_timer(500, ATTENTE_CYCLE_MONTAGE_ARRIERE_COMPLET);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
    }
}

void attente_rdescente_complete (_cote cote)
{
    if (cote == AVANT)
    {
        // ascenseur pas encore en bas, on attend
        if (is_target_ax12_reachead(ASC_AVANT) == false)
        {
            arm_timer(200, ATTENTE_CYCLE_MONTAGE_AVANT_COMPLET);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // Processus de montage dans la tour terminé
            FLAG_ACTION = PROCESS_MODULE_READY;
        }
    }
    else if (cote == ARRIERE)
    {
        // ascenseur pas encore en bas, on attend
        if (is_target_ax12_reachead(ASC_ARRIERE) == false)
        {
            arm_timer(200, ATTENTE_CYCLE_MONTAGE_ARRIERE_COMPLET);
            FLAG_ACTION = EN_ATTENTE_EVENT;
        }
        else
        {
            // Processus de montage dans la tour terminé
            FLAG_ACTION = PROCESS_MODULE_READY;
        }
    }
}

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_20ms (void)
{
    _autom_id autom_id;

    check_timer_event();
    
    //fonction qui definit les actions
    switch (FLAG_ACTION[AUTOM_PRINCIPALE])
    {
        case NE_RIEN_FAIRE:
        case EN_ATTENTE_EVENT :
            break;
        case CHECK_AX12_EVENT :
            check_ax12_event(AUTOM_PRINCIPALE);
            break;
        case START_ROBOT :
            start_robot();
            break;
        case WAIT_PINCES_OUVERTES :
            attente_ouverture_pinces();
            break;
        case WAIT_INIT_ROBOT_COMPLETE :
            wait_init_robot_complete();
            break;
        case INIT_ROBOT_COMPLETE :
//            FLAG_ACTION = NE_RIEN_FAIRE;
            arm_timer(4000, PROCESS_MODULE_READY);
            FLAG_ACTION = EN_ATTENTE_EVENT;
            break;
        case PROCESS_MODULE_READY :
            montage_tour();
            break;
        default :
            break;
    }
    
    for (autom_id = AUTOM_AVANT ; autom_id < AUTOM_ARRIERE ; autom_id++)
    {
        switch(FLAG_ACTION[autom_id])
        {
            case NE_RIEN_FAIRE:
            case EN_ATTENTE_EVENT :
                break;
            case CHECK_AX12_EVENT :
                check_ax12_event(autom_id);
                break;
            case RECHERCHE_MODULE_AVANT :
            case RECHERCHE_MODULE_ARRIERE :
                recherche_modules_pince(autom_id);
                break;
            case MODULE_DETECTED_AVANT :
            case MODULE_DETECTED_ARRIERE :
                debut_attrape_module(autom_id);
                break;
            case ATTENTE_MONTEE_AVANT :
            case ATTENTE_MONTEE_ARRIERE :
                attente_fermeture_pinces_avant_montee(autom_id);
                break;
            case ASC_AVANT_EN_MONTEE :
            case ASC_ARRIERE_EN_MONTEE :
                attente_ascenseur_en_haut(autom_id);
                break;
            case FERMETURE_PINCE_HAUT_AVANT :
            case FERMETURE_PINCE_HAUT_ARRIERE :
                attente_fermeture_pince_en_haut(autom_id);
                break;
            case PREPARE_REDESCENTE_AVANT :
            case PREPARE_REDESCENTE_ARRIERE :
                ouvrir_pince_pour_redescenre(autom_id);
                break;
            case ATTENTE_REDESCENTE_AVANT :
            case ATTENTE_REDESCENTE_ARRIERE :
                attente_ouverture_pince_pour_redescente(autom_id);
                break;
            case ATTENTE_CYCLE_MONTAGE_AVANT_COMPLET :
            case ATTENTE_CYCLE_MONTAGE_ARRIERE_COMPLET :
                attente_rdescente_complete(autom_id);
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