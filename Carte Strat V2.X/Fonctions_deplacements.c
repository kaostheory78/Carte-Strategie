/******************************************************************************/
/************** Carte polyvalente Botik  : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: fonctions_deplacements.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 14 d�cembre 2014, 14:29
 *******************************************************************************
 *
 *
 ******************************************************************************/


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/


#include "system.h"

void cibler (double x, double y, double pourcentage_vitesse)
{
    reinit_asserv();

    x *= TICKS_PAR_MM;
    y *= TICKS_PAR_MM;

    TYPE_CONSIGNE = MM;

    if ((y - Y.actuelle) != 0 || (x * - X.actuelle) != 0 )//&& (Y.consigne - Y.actuelle) != 0)
    {
        ORIENTATION.consigne =  atan2((y - Y.actuelle), (x - X.actuelle)) * ENTRAXE_TICKS/2 ;
    }
    
    
    

  /*  if (ORIENTATION.consigne < -Pi * ENTRAXE_TICKS /2)
        ORIENTATION.consigne += Pi * ENTRAXE_TICKS/2;
    else if (ORIENTATION.consigne > Pi * ENTRAXE_TICKS/2)
        ORIENTATION.consigne -= Pi * ENTRAXE_TICKS /2; */

    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

    while(FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    
}
 
void orienter (double angle, double pourcentage_vitesse)
{

    reinit_asserv();
    
    TYPE_CONSIGNE = MM;

    ORIENTATION.consigne = (angle * Pi)/ 180 * (ENTRAXE_TICKS/2);

    calcul_vitesse_orientation(pourcentage_vitesse);
    calcul_acceleration_orientation();

    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

    while(FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
}


void rejoindre (double x, double y, double pourcentage_vitesse)
{
    _rejoindre(x, y, pourcentage_vitesse, ON);
}

void _rejoindre (double x, double y, double pourcentage_vitesse, char vitesse_fin_deplacement_nulle)
{
    reinit_asserv();
    FLAG_ASSERV.brake = OFF;

    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    TYPE_CONSIGNE = XY;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation = DCC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;

    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.type_deplacement = AVANCER;
    FLAG_ASSERV.vitesse_fin_nulle = vitesse_fin_deplacement_nulle;

    FLAG_ASSERV.immobilite = 0;


    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
}

void avancer_reculer (double distance, double pourcentage_vitesse)
{
    reinit_asserv();

    TYPE_CONSIGNE = MM;

    DISTANCE.consigne = distance * TICKS_PAR_MM;

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
}

void passe_part (double x, double y)
{
    //_rejoindre(x, y, OFF);
}

void passe_part2 (double x, double y)
{
    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = EN_COURS;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);


}