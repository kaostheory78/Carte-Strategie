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
    /*passe_part (200, 0,    sens_marche, 300, DEBUT_TRAJECTOIRE);
    passe_part (450, 67,   sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 250,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    sens_marche = MARCHE_ARRIERE;
    passe_part (700, 500,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 750,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (450, 933,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (200, 1000, sens_marche, 100, FIN_TRAJECTOIRE);*/

        passe_part(300, 0, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(600, 0, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        passe_part(900, 0, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(1200, 0, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        passe_part(1500, 0, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

    #endif

        
    #ifdef PETIT_ROBOT
        init_position_robot (0, 0, 0);
       // TIMER_DEBUG = ACTIVE;

       /* uint8_t tab[5];
        tab[0] = 9;
        tab[1] = 15;
        tab[2] = 8;
        tab[3] = 5;
        tab[4] = 13;

       lecture_position_AX12 (tab, 5);*/

        //rejoindre(2000, 0, MARCHE_AVANT,  10);
        faire_des_tours(-32);
        //rejoindre(750, 0, MARCHE_AVANT, 100);

        //TIMER_DEBUG = DESACTIVE;
        PutsUART(UART_XBEE, " X : ");
        PutLongUART((int32_t) get_X());
        PutsUART(UART_XBEE, " Y : ");
        PutLongUART((int32_t) get_Y());
        PutsUART(UART_XBEE, " Teta : ");
        PutLongUART((int32_t) get_orientation());
        while(1);

        
    #endif
}