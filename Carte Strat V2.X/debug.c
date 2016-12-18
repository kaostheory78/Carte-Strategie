/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: codeurs.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 9 novembre 2014, 02:39
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


void debug ()
{
    //asserv
    printf("%lf \t%lf \t%lf \t%lf", ORIENTATION.consigne, ORIENTATION.actuelle, DISTANCE.consigne, DISTANCE.actuelle);
    printf("\t%lf ", VITESSE[SYS_ROBOT].theorique);
    printf("\t%lf \t%lf ", VITESSE[ROUE_GAUCHE].consigne, VITESSE[ROUE_GAUCHE].actuelle);
    printf("\t%lf \t%lf ", VITESSE[ROUE_DROITE].consigne, VITESSE[ROUE_DROITE].actuelle);
    printf("\t%lf \t%lf ", ERREUR_VITESSE[ROUE_GAUCHE].actuelle, ERREUR_VITESSE[ROUE_GAUCHE].integralle);
    printf("\t%lf \t%lf ", ERREUR_VITESSE[ROUE_DROITE].actuelle, ERREUR_VITESSE[ROUE_DROITE].integralle);
    printf("\t%lf \t%lf ", COMMANDE.droit, COMMANDE.gauche);
    printf("\t%lf \t%lf ", ERREUR_ORIENTATION.actuelle, ERREUR_DISTANCE.actuelle);
    printf("\t%lld \t%lf ", FLAG_ASSERV.immobilite, Pi);
    printf("\n\r");
}

