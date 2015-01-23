/******************************************************************************/
/************** Carte polyvalente Botik  : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: fonctions_deplacements.h
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 14 d�cembre 2014, 15:00
 *******************************************************************************
 *
 *
 ******************************************************************************/

#ifndef FONCTIONS_DEPLACEMENTS_H
#define	FONCTIONS_DEPLACEMENTS_H

#ifdef	__cplusplus
extern "C" {
#endif

 
/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/


/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void cibler (double x, double y, double pourcentage_vitesse);
    void rejoindre (double x, double y, double pourcentage_vitesse);
    void orienter (double angle, double pourcentage);
    void avancer_reculer (double distance, double pourcentage_vitesse);
    void passe_part (double x, double y);
    void passe_part2 (double x, double y);
    void _rejoindre (double x, double y, double pourcentage_vitesse, char vitesse_fin_deplacement_nulle);


#ifdef	__cplusplus
}
#endif

#endif	/* FONCTIONS_DEPLACEMENTS_H */

