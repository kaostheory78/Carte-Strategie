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
        //init_position_robot (145, 1045, 0);
        init_position_robot (153, 1030, 0);
       // init_position_robot (0, 0, 0);

       /* rejoindre(2500, 1030, MARCHE_AVANT, 100);
        rejoindre(500, 1030, MARCHE_AVANT, 100);
        rejoindre(2500, 1030, MARCHE_AVANT, 100);
        rejoindre(500, 1030, MARCHE_AVANT, 100);
        rejoindre(2500, 1030, MARCHE_AVANT, 100);
        rejoindre(500, 1030, MARCHE_AVANT, 100);
        rejoindre(2500, 1030, MARCHE_AVANT, 100);
        rejoindre(500, 1030, MARCHE_AVANT, 100);
        rejoindre(2500, 1030, MARCHE_AVANT, 100);
        rejoindre(300, 1030, MARCHE_AVANT, 100);
        rejoindre(500, 1030, MARCHE_AVANT, 100);


        while(1);*/

        faire_des_tours(32);
       /* trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        rejoindre(100, 0, MARCHE_AVANT, 50);*/
        while(1);

        //Init départ
        init_pinces_jack();
        rejoindre (490, 1030, MARCHE_AVANT, 50);
        cibler(1300, 600, 80);

        while(!SYS_JACK);

        FLAG_ACTION = INIT_PINCES_DEMARRAGE;
        //Pieds 1
        rejoindre(1200, 660, MARCHE_AVANT, 100);
        //Pieds 2
        rejoindre (1140, 330, MARCHE_AVANT, 100);
        //Pieds 3
        rejoindre (940, 580, MARCHE_AVANT, 100);


        /******** Dépose pile de 3 */
        /*
        //Placement dépose
        rejoindre(1355, 265, MARCHE_AVANT, 100);

        orienter(-90, 100);

        FLAG_ACTION = PREPARATION_DEPOSE_PIEDS;
        delay_ms(500);

        avancer_reculer(250, 50);
        init_orientation(-90);
        init_Y(189);

        depose_banc();

        //Chercher balle :
        rejoindre (1355, 250, MARCHE_ARRIERE, 100);
        rejoindre (1245, 250, MARCHE_AVANT, 100);

        pinces(PINCE_BAS, FERMER);

        rejoindre (1245, 187, MARCHE_AVANT, 50);

        FLAG_ACTION = ATTRAPE_BALLE;
        //delay_ms(500);*/


        passe_part(600, 200, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        envoit_pwm(MOTEUR_X, 100);
        passe_part(300, 200, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(115, 200, MARCHE_ARRIERE, 50, FIN_TRAJECTOIRE);

        rejoindre( 500, 200, MARCHE_AVANT, 100);


        /******** Récup gobelet en passe part*/
        /*
        passe_part (1340, 400, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
        FLAG_ACTION = ATTRAPE_GOBELET;
        passe_part (910, 1000, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part (910, 1250, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part (360, 1000, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
        FLAG_ACTION = DEPOSE_GOBELET;*/

        /******* Placement côté marche ***

        passe_part (580, 1000, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
        passe_part (825, 1260, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        rejoindre(825, 1695, MARCHE_AVANT, 20);*/



       /* while(ETAT_AUTOM != ACCOMPLI);
        delay_ms(3000);
        FLAG_ACTION = DEPOSE_PIEDS;*/
        
                
       
        // passe_part(630, 950, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
       // passe_part(1300, 600, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //TIMER_DEBUG = DESACTIVE;


        
    #endif
}