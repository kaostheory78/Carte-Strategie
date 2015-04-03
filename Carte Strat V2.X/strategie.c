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

        passe_part(600,1000,MARCHE_AVANT,100,DEBUT_TRAJECTOIRE);
        passe_part(650,600,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(360,335,MARCHE_AVANT,100,FIN_TRAJECTOIRE);

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
       /* trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        trapeze(MARCHE_AVANT);
        rejoindre(100, 0, MARCHE_AVANT, 50);

        delay_ms(5000);
        PutsUART(UART_XBEE, " X : ");
        PutLongUART((int32_t) get_X());
        PutsUART(UART_XBEE, " Y : ");
        PutLongUART((int32_t) get_Y());
        PutsUART(UART_XBEE, " Teta : ");
        PutLongUART((int32_t) get_orientation());
        while(1);*/

        //Init départ
        init_pinces_jack();
        rejoindre (490, 1030, MARCHE_AVANT, 50);
        cibler(1300, 600, 80);

        while(!SYS_JACK);

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
        rejoindre(680, 1780, MARCHE_ARRIERE, 100);
        allumer_pompes();
        rejoindre(770, 1820, MARCHE_ARRIERE, 50);
        orienter(180, 100);
        avancer_reculer(-20, 100);
        orienter(-70, 100);
        avancer_reculer(-20, 100);

        //Placement zone départ empilement
        passe_part(620, 1800, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
        passe_part(600, 1500, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
        passe_part(720, 1370, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(1000, 1130, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(900, 970, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);

        //rentrée dans la zone de départ
        passe_part(700, 970, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //Montage de la tour (dépassement des 350, ejection de la balle
        angle_AX12(PINCE_HAUT, 555, 1023, SANS_ATTENTE);
        angle_AX12(PINCE_MILIEU, 555, 1023, SANS_ATTENTE);
        angle_AX12(PINCE_BAS, 555, 1023, SANS_ATTENTE);
        delay_ms(500);
        pinces(PINCE_HAUT, FERMER);
        pinces(PINCE_MILIEU, FERMER);
        pinces(PINCE_BAS, FERMER);
        delay_ms(500);
        ejecter_balle();
        delay_ms(4000);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms (4000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        //Pieds 5
        rejoindre(625, 955, MARCHE_AVANT, 100);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        // Pieds 6
        rejoindre(505, 955, MARCHE_AVANT, 100);
        FLAG_ACTION = ZONE_DEPART;


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