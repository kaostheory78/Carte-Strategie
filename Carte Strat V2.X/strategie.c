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

    
        init_position_robot (185, 1000, 0);
        FLAG_ACTION = INIT_JACK;

        delay_ms(1000);
        while(!SYS_JACK);
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        STRATEGIE_EVITEMENT = STOP;
        FLAG_ACTION = INIT_DEPART;

        //Direction premier gobelet pour libérer les deux pieds pour le gros robot
        passe_part(600,1000,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
        passe_part(650,600,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        FLAG_ACTION = ATTRAPE_GOBELET;
        passe_part(357,347,MARCHE_AVANT,100,FIN_TRAJECTOIRE);

        //petite attente
        //delay_ms(1000);

        //retour au bercail pour déposer le gobelet
        passe_part(570,455,MARCHE_ARRIERE,100,DEBUT_TRAJECTOIRE);
        passe_part(550,550,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(650,800,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        EVITEMENT_ADV_AVANT = OFF;
        passe_part(558,990,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(270,990,MARCHE_AVANT,100,FIN_TRAJECTOIRE);

        pince(OUVERTE);
        delay_ms(1000);

        // direction dépose tapis
        passe_part(700, 990,MARCHE_ARRIERE,100,DEBUT_TRAJECTOIRE);
        EVITEMENT_ADV_AVANT = ON;
        passe_part(920,920,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(1225,940,MARCHE_AVANT,100,FIN_TRAJECTOIRE);
        cibler (1225, 2000, 100);
        depose_tapis(EN_BAS,DROITE);
        delay_ms(1000);
        depose_tapis(EN_HAUT,DROITE);
        delay_ms(1000);
        depose_tapis(EN_BAS,GAUCHE);
        delay_ms(1000);
        depose_tapis(EN_HAUT,GAUCHE);
        delay_ms(1000);
        avancer_reculer(400,50);
        depose_tapis(DEPOSE,DROITE);
        delay_ms(1000);
        pince_tapis(DEPOSE,DROITE);
        delay_ms(1000);
        depose_tapis(EN_HAUT,DROITE);
        delay_ms(1000);
        pince_tapis(EN_HAUT,DROITE);
        delay_ms(1000);
        depose_tapis(DEPOSE,GAUCHE);
        delay_ms(1000);
        pince_tapis(DEPOSE,GAUCHE);
        delay_ms(1000);
        depose_tapis(EN_HAUT,GAUCHE);
        delay_ms(1000);
        pince_tapis(EN_HAUT,GAUCHE);
        delay_ms(1000);
        init_Y(1370);
        init_orientation(90);
        rejoindre(1225,1000,MARCHE_ARRIERE, 50);
        
 pince_tapis(EN_HAUT,GAUCHE);
    #endif

        
    #ifdef PETIT_ROBOT
        //init_position_robot (145, 1045, 0);
        init_position_robot (153, 1030, 0);
       // init_position_robot (0, 0, 0);

        /*rejoindre(2500, 1030, MARCHE_AVANT, 100);
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


        //faire_des_tours(32);
        while(1);*/

        //Init départ
        init_pinces_jack();
        rejoindre (490, 1030, MARCHE_AVANT, 50);
        cibler(1300, 600, 80);

        while(!SYS_JACK);

        EVITEMENT_ADV_AVANT = ON;
        FLAG_ACTION = INIT_PINCES_DEMARRAGE;


        

        //Pieds 1
        rejoindre(1210, 645, MARCHE_AVANT, 100);
        delay_ms(1000);     //1 sec
        //Pieds 2
        rejoindre (1125, 330, MARCHE_AVANT, 100);
        delay_ms(1000);
        //Pieds 3
        rejoindre (935, 570, MARCHE_AVANT, 100);
        delay_ms(1000);



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

        //Alignement vers les pieds
        passe_part(600, 200, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        FLAG_ACTION = FERMETURE_PINCE;
        //Confirmation de l'axe des Y
        EVITEMENT_ADV_ARRIERE = ON;
        EVITEMENT_ADV_AVANT = OFF;
        passe_part(300, 200, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        //on recule jusqu'au pieds
        allumer_pompes();
        passe_part(148, 200, MARCHE_ARRIERE, 50, FIN_TRAJECTOIRE);

        //Danse du cul pour les chopper
        orienter(20, 100);
        orienter(0, 100);
        avancer_reculer(-20, 200);
        //orienter(-20, 200);

        ouvrir_bras(BRAS_GAUCHE);
        FLAG_ACTION = CLAP;
        EVITEMENT_ADV_AVANT = ON;
        EVITEMENT_ADV_ARRIERE = OFF;
        passe_part(200, 170, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
        //claps
        passe_part(500, 160, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
        passe_part(890, 160, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);

        //dernier clap
        passe_part(1100, 240, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        fermer_bras(BRAS_GAUCHE);

        //direction depose centre
        passe_part(600, 800, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(460, 1010, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
        //passe_part(650, 600, MARCHE_AVANT, 200, MILIEU_TRAJECTOIRE);
        //passe_part(550, 800, MARCHE_AVANT, 200, MILIEU_TRAJECTOIRE);
        //passe_part(540, 1060, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //réalignement
        orienter(90, 500);

        //Dépose des pieds
        eteindre_pompe();
        FLAG_ACTION = PIEDS_4;

 

        //Sortie zone de départ
        passe_part(700, 1300, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);

        passe_part (580, 1670, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);

        ////Récupération 4ème pieds
        passe_part(170, 1770, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //Aspiration 2 pieds
        allumer_pompes();
        EVITEMENT_ADV_AVANT = OFF;
        EVITEMENT_ADV_ARRIERE = ON;
        rejoindre(785, 1745,  MARCHE_ARRIERE, 50);
        //orienter(-170, 100);
        //orienter(170, 100);
        EVITEMENT_ADV_ARRIERE = OFF;
        orienter(-90, 15);
        rejoindre(785, 1830, MARCHE_ARRIERE, 25);
        orienter(-70, 50);
        //orienter(-90, 50);
        //avancer_reculer(-5, 200);
        //orienter(-100, 50);


        // Placement zone de départ
        EVITEMENT_ADV_AVANT = ON;
        rejoindre ( 700, 1125, MARCHE_AVANT, 100);

        //rentrée dans la zone de départ
        EVITEMENT_ADV_AVANT = OFF;
        rejoindre(640, 1090, MARCHE_AVANT, 100);

        //Montage de la tour (dépassement des 350, ejection de la balle
        //descente des pieds pour laisser passer la balle
        angle_AX12(PINCE_HAUT, 555, 1023, SANS_ATTENTE);
        angle_AX12(PINCE_MILIEU, 555, 1023, SANS_ATTENTE);
        angle_AX12(PINCE_BAS, 555, 1023, SANS_ATTENTE);
        delay_ms(2000);

        //on ressert le tout pour que la tour soit bien droite
        pinces(PINCE_HAUT, FERMER);
        pinces(PINCE_MILIEU, FERMER);
        pinces(PINCE_BAS, FERMER);

        //Ejection de la balle
        delay_ms(1000);
        angle_AX12(PINCE_HAUT, 550, 1023, SANS_ATTENTE);
        ejecter_balle();
        delay_ms(4000);

        //empilement si nécéssaire
        FLAG_ACTION = ZONE_DEPART;
        delay_ms (4000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        //Pieds 5
        rejoindre(550, 1000, MARCHE_AVANT, 100);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        // Pieds 6
        rejoindre(450, 975, MARCHE_AVANT, 100);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);

        rejoindre(300, 1000, MARCHE_AVANT, 50);
        FLAG_ACTION = DEPOSE_PIEDS;
        delay_ms(3000);
        rejoindre(500, 1000, MARCHE_ARRIERE, 50);
        


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