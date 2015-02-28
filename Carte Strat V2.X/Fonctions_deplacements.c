/******************************************************************************/
/************** Carte polyvalente Botik  : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: fonctions_deplacements.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 14 décembre 2014, 14:29
 *******************************************************************************
 *
 *
 ******************************************************************************/


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/**************************** FONCTIONS COULEURS ******************************/
/******************************************************************************/

uint8_t couleur_depart()
{
    if (SYS_COULEUR == 0)
        return JAUNE;
    else
        return VERT;
}

double inversion_couleur (double param_inversable)
{
    if (COULEUR == JAUNE)
        return param_inversable;
    else
        return (- param_inversable);
}


/******************************************************************************/
/********************* FONCTIONS USUELLES (TEST ASSERV) ***********************/
/******************************************************************************/

void trapeze (int8_t sens_marche)
{
    //trapèze type 8
    passe_part (1000, 500, sens_marche, 50, DEBUT_TRAJECTOIRE);
    passe_part (1000, 0  , sens_marche, 50, MILIEU_TRAJECTOIRE);
    passe_part (0   , 500, sens_marche, 50, MILIEU_TRAJECTOIRE);
    passe_part (0   , 0  , sens_marche, 50, FIN_TRAJECTOIRE);
}

void petit_demi_cercle (int8_t sens_marche)
{
    //petit demi cercle
    passe_part (200, 0,    sens_marche, 300, DEBUT_TRAJECTOIRE);
    passe_part (450, 67,   sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 250,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (700, 500,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 750,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (450, 933,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (200, 1000, sens_marche, 100, FIN_TRAJECTOIRE);
}

void grand_demi_cercle (int8_t sens_marche)
{
    passe_part (200 , 0   , sens_marche, 500, DEBUT_TRAJECTOIRE);
    passe_part (700 , 134 , sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (1066, 500 , sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (1200, 1000, sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (1066, 1500, sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (700 , 1866, sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (200 , 2000, sens_marche, 100, FIN_TRAJECTOIRE);
}

void carre (int8_t sens_marche)
{
    rejoindre (500, 0  , sens_marche, 100);
    rejoindre (500, 500, sens_marche, 100);
    rejoindre (0  , 500, sens_marche, 100);
    rejoindre (0  , 0  , sens_marche, 100);
}

/******************************************************************************/
/******************* FONCTIONS ASSERV HAUT NIVEAU (EVITEMENT) *****************/
/******************************************************************************/


void action_evitement (void)
{
    //Stratégie alternatives possibles en focntion des évitements 
}

void cibler (double x, double y, double pourcentage_vitesse)
{
    uint8_t erreur = _cibler (x, y, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

void orienter (double angle, double pourcentage_vitesse)
{
    uint8_t erreur = _orienter (angle, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

void rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse)
{
    uint8_t erreur = _rejoindre (x, y, sens_marche, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

void avancer_reculer (double distance, double pourcentage_vitesse)
{
    uint8_t erreur = _avancer_reculer (distance, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

void passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last)
{
    uint8_t erreur = _passe_part (x, y, sens_marche, pourcentage_vitesse, last);
    if ( erreur == EVITEMENT)
    {
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
        } 
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}



/******************************************************************************/
/***************** FONCTIONS ASSERV BAS NIVEAU (config asserv) ****************/
/******************************************************************************/

uint8_t _cibler (double x, double y, double pourcentage_vitesse)
{
    y = inversion_couleur(y);
    FLAG_ASSERV.brake = OFF;
    delay_ms(10);

    reinit_asserv();

    x *= TICKS_PAR_MM;
    y *= TICKS_PAR_MM;

    TYPE_CONSIGNE = MM;

    if ((y - Y.actuelle) != 0 || (x - X.actuelle) != 0 )//&& (Y.consigne - Y.actuelle) != 0)
    {
        ORIENTATION.consigne =  atan2((y - Y.actuelle), (x - X.actuelle)) * ENTRAXE_TICKS/2 ;

        calcul_vitesse_orientation(pourcentage_vitesse);
        calcul_acceleration_orientation();

        FLAG_ASSERV.position = OFF;
        FLAG_ASSERV.orientation = ON;
        FLAG_ASSERV.vitesse = ON;

        FLAG_ASSERV.type_deplacement = ORIENTER;

        FLAG_ASSERV.etat_angle = EN_COURS;
        FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
        FLAG_ASSERV.immobilite = 0;

        FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

        while(FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    }
    return FLAG_ASSERV.erreur;
}
 
uint8_t _orienter (double angle, double pourcentage_vitesse)
{
    angle = inversion_couleur(angle);
    FLAG_ASSERV.brake = OFF;
    delay_ms(10);

    reinit_asserv();
    
    TYPE_CONSIGNE = MM;

    ORIENTATION.consigne = (angle * Pi)/ 180 * (ENTRAXE_TICKS/2);

    calcul_vitesse_orientation(pourcentage_vitesse);
    calcul_acceleration_orientation();

    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.type_deplacement = ORIENTER;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

    while(FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

uint8_t _rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse)
{
    y = inversion_couleur(y);
    FLAG_ASSERV.brake = OFF;
    delay_ms(10);

    reinit_asserv();
    
    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    TYPE_CONSIGNE = XY;

    FLAG_ASSERV.sens_deplacement = sens_marche;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;

    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.type_deplacement = AVANCER;
    FLAG_ASSERV.vitesse_fin_nulle = ON;

    FLAG_ASSERV.immobilite = 0;


    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

uint8_t _avancer_reculer (double distance, double pourcentage_vitesse)
{
    FLAG_ASSERV.brake = OFF;
    reinit_asserv();

    TYPE_CONSIGNE = MM;

    DISTANCE.consigne = distance * TICKS_PAR_MM;
    if (distance < 0)
        FLAG_ASSERV.sens_deplacement = MARCHE_ARRIERE;
    else
        FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

uint8_t _passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last)
{
    if (last == DEBUT_TRAJECTOIRE)
    {
        FLAG_ASSERV.brake = OFF;
        delay_ms(10);
        reinit_asserv();

        TYPE_CONSIGNE = XY;
    }

    y = inversion_couleur(y);
    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;

    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.sens_deplacement = sens_marche;
    FLAG_ASSERV.phase_deceleration_distance = PHASE_NORMAL;
    FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;

    FLAG_ASSERV.type_deplacement = PASSE_PART;
    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = EN_COURS;

    FLAG_ASSERV.immobilite = 0;

    if (last == FIN_TRAJECTOIRE)
        FLAG_ASSERV.vitesse_fin_nulle = ON;
    else
        FLAG_ASSERV.vitesse_fin_nulle = OFF;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/