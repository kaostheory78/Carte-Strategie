/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: srategie.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 07 février 2015, 22:47
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void strategie()
{
    COULEUR = couleur_depart();
    
    #ifdef GROS_ROBOT
        init_position_robot (0, 0, 0);
        int8_t sens_marche = MARCHE_AVANT;
    passe_part (200, 0,    sens_marche, 300, DEBUT_TRAJECTOIRE);
    passe_part (450, 67,   sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 250,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    sens_marche = MARCHE_ARRIERE;
    passe_part (700, 500,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 750,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (450, 933,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (200, 1000, sens_marche, 100, FIN_TRAJECTOIRE);

    #endif

        
    #ifdef PETIT_ROBOT
        init_position_robot (0, 0, 0);
        TIMER_DEBUG = ACTIVE;
        brake();
        avancer_reculer(750, 100);

        TIMER_DEBUG = DESACTIVE;
    #endif
}