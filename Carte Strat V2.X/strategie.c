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
        COULEUR = JAUNE;

        init_jack();
        delay_ms(2000);

        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        STRATEGIE_EVITEMENT = STOP;
        init_position_robot(180, 988, 0);
        FLAG_ACTION = INIT_ASCENSEUR;

        // Direction Gobelet face zone départ
        passe_part(620, 988, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(683, 991, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(675, 998, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(742, 1112,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        // Direction Gobelet pour libérer les deux pieds pour R2
        passe_part(663, 334, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(445, 324, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(400, 310, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(325, 310, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);


        // Retour au bercail pour dépose N°1
        //EVITEMENT_ADV_AVANT = OFF;
        //EVITEMENT_ADV_ARRIERE = ON;
        passe_part(570,455,MARCHE_ARRIERE,100,DEBUT_TRAJECTOIRE);
        FLAG_ACTION = NE_RIEN_FAIRE;
        //EVITEMENT_ADV_ARRIERE = OFF;
        //EVITEMENT_ADV_AVANT = ON;
        passe_part(550,550,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(650,800,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        //EVITEMENT_ADV_AVANT = OFF;
        passe_part(558,990,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(270,990,MARCHE_AVANT,100,FIN_TRAJECTOIRE);

        // On vérifie confirme que le gobelet a bien été attrapé
        uint8_t status_pince_G = LIBRE, status_pince_D = LIBRE;
        if (CAPT_GOBELET_D == 0)
            status_pince_D = FERMER;
        if (CAPT_GOBELET_G == 0)
            status_pince_G = FERMER;


        if (status_pince_G == FERMER)
        {
            pince(GAUCHE, OUVERTE);
            status_pince_G = LIBRE;
        }
        else if (status_pince_D == FERMER)
        {
            pince(DROITE, OUVERTE);
            status_pince_D = LIBRE;
        }
        else
        {
            pince(DROITE, RANGEMENT);
            pince(GAUCHE, RANGEMENT);
            ascenseur(ARRIERE);
        }

        // On a encore un gobelet à déposer
        if (status_pince_D == FERMER)
        {
            delay_ms(500);
            avancer_reculer(-100, 50);
            passe_part(750, 1000, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);
            passe_part(1100, 900, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
            passe_part(1900, 900, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
            passe_part(2300, 575, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
            passe_part(2760, 575, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
            // Dépose du gobelet
            pince(DROITE, OUVERTE);

            // On essaye d'avoir celui de l'adversaire
            avancer_reculer(-80, 50);
            FLAG_ACTION = ATTRAPE_GOBELET;

            // On va chercher celui de l'adversaire si dispo
            rejoindre (2670, get_Y(), MARCHE_ARRIERE, 50);
            rejoindre (2670, 425, MARCHE_AVANT, 50);

            // On se dirige vers le clap
            rejoindre(2670, 240, MARCHE_AVANT, 50);
            rejoindre(2550, 210, MARCHE_AVANT, 50);
            bras(GAUCHE, OUVERT);
            rejoindre (2220, 210, MARCHE_AVANT, 50);

            if (CAPT_GOBELET_G == 0)
            {
                //depose du gobelet dans la zone de droite
                //+ Alignement marches
                bras(GAUCHE, FERMER);
                rejoindre (2100, 800, MARCHE_AVANT, 50);
                rejoindre (2200, 1210, MARCHE_AVANT, 50);
                rejoindre (2300, 1300, MARCHE_AVANT, 50);
                rejoindre (2400, 1450, MARCHE_AVANT, 50);
                rejoindre (2700, 1450, MARCHE_AVANT, 50);

            }
            else
            {
                //se diriger vers les marches
            }


            //rejoindre(2600, 400, MARCHE_AVANT, 100);
        }
        else
        {
            // direction clap direct
        }

        //Montée des marches
//
//        EVITEMENT_ADV_ARRIERE = OFF;
//        EVITEMENT_ADV_AVANT = OFF;
//
//        FLAG_ASSERV.totale = OFF;
//
//        //init_jack();
////        delay_ms(2000);
////        //FLAG_ACTION = INIT_ASCENSEUR;
////        //delay_ms(5000);
////
////        FLAG_ACTION = MONTEE_MARCHE;
//        allumer_LED_AX12(TOUS_LES_AX12);
//
//        while(1)
//        {
//            chenilles(MONTER);
//            delay_ms(2000);
//            chenilles(DESCENDRE);
//            delay_ms(2000);
//            chenilles(INTERMEDIAIRE);
//            delay_ms(2000);
//            chenilles(MONTER);
//            delay_ms(4000);
//        }

        
    #endif

        
    #ifdef PETIT_ROBOT
        init_position_robot (153, 1030, 0);

        //Init départ
        init_pinces_jack();
        rejoindre (490, 1030, MARCHE_AVANT, 50);
        cibler(1300, 600, 80);

        while(!SYS_JACK);
        TIMER_90s = ACTIVE;

        EVITEMENT_ADV_AVANT = ON;
        FLAG_ACTION = INIT_PINCES_DEMARRAGE;


        

        //Pieds 1
        rejoindre(1210, 645, MARCHE_AVANT, 100);
        delay_ms(500);     //1 sec
        //Pieds 2
        rejoindre (1125, 330, MARCHE_AVANT, 100);
        delay_ms(500);
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

        //réalignement
        orienter(90, 500);

        //Dépose des pieds
        eteindre_pompe();
        FLAG_ACTION = PIEDS_4;

 

        //Sortie zone de départ
        passe_part(700, 1300, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);

        passe_part (580, 1670, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);

        ////Récupération 4ème pieds
        EVITEMENT_ADV_AVANT = OFF;
        passe_part(300,1770,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(170, 1770, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //Aspiration 2 pieds
        allumer_pompes();
        EVITEMENT_ADV_AVANT = OFF;
        EVITEMENT_ADV_ARRIERE = ON;
        rejoindre(785, 1745,  MARCHE_ARRIERE, 50);
        EVITEMENT_ADV_ARRIERE = OFF;
        // orienter(-90, 15);
        //rejoindre(785, 1830, MARCHE_ARRIERE, 25);
        //orienter(-70, 50);

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
        delay_ms(6000);

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

        rejoindre(320, 1000, MARCHE_AVANT, 50);
        FLAG_ACTION = DEPOSE_PIEDS;
        delay_ms(3000);
        rejoindre(500, 1000, MARCHE_ARRIERE, 50);

        //orienter (0, 20);
        eteindre_pompe();
        


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


void homologation()
{
    COULEUR = couleur_depart();

#ifdef GROS_ROBOT
     

#endif

#ifdef PETIT_ROBOT
        init_position_robot (153, 1030, 0);

        //Init départ
        init_pinces_jack();
        rejoindre (490, 1030, MARCHE_AVANT, 50);
        cibler(1300, 600, 80);

        while(!SYS_JACK);

        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        init_evitement();
        STRATEGIE_EVITEMENT = EVITEMENT_NORMAL;
        FLAG_ACTION = INIT_PINCES_DEMARRAGE;

        //Pieds 1
        rejoindre(1210, 645, MARCHE_AVANT, 50);
        delay_ms(4000);     //1 sec
        //Pieds 2
        rejoindre (1125, 330, MARCHE_AVANT, 50);
        delay_ms(5000);
        //Pieds 3
        rejoindre (935, 570, MARCHE_AVANT, 50);
        delay_ms(1000);

        rejoindre (600, 200, MARCHE_AVANT, 50);
        FLAG_ACTION = FERMETURE_PINCE;
        //Confirmation de l'axe des Y
        EVITEMENT_ADV_ARRIERE = ON;
        EVITEMENT_ADV_AVANT = OFF;
        rejoindre (300, 200, MARCHE_AVANT, 50);
        //on recule jusqu'au pieds
        allumer_pompes();
        rejoindre(148, 200, MARCHE_ARRIERE, 50);

        //Danse du cul pour les chopper
        orienter(20, 100);
        orienter(0, 100);
        avancer_reculer(-20, 200);
        
        ouvrir_bras(BRAS_GAUCHE);

        FLAG_ACTION = CLAP;
        EVITEMENT_ADV_AVANT = ON;
        EVITEMENT_ADV_ARRIERE = OFF;
        rejoindre(200, 170, MARCHE_AVANT, 50);

        //claps
        rejoindre(500, 160, MARCHE_AVANT, 50);
        rejoindre(890, 160, MARCHE_AVANT, 50);

        //dernier clap
        rejoindre (1100, 240, MARCHE_AVANT, 50);
        fermer_bras(BRAS_GAUCHE);

        //direction depose centre
       rejoindre (600, 800, MARCHE_AVANT, 50);
       rejoindre (460, 1010, MARCHE_AVANT, 100);

        //réalignement
        orienter(90, 500);

        //Dépose des pieds
        eteindre_pompe();
        FLAG_ACTION = PIEDS_4;

        //Sortie zone de départ
        rejoindre (700, 1300, MARCHE_AVANT, 50);

        rejoindre(580, 1670, MARCHE_AVANT, 50);

        //Récupération 4ème pieds
        rejoindre(170, 1770, MARCHE_AVANT, 50);

         rejoindre ( 700, 1125, MARCHE_AVANT, 50);

        //rentrée dans la zone de départ
        EVITEMENT_ADV_AVANT = OFF;
        rejoindre(640, 1090, MARCHE_AVANT, 50);

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
        rejoindre(550, 1000, MARCHE_AVANT, 50);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        // Pieds 6
        rejoindre(450, 975, MARCHE_AVANT, 50);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);

        rejoindre(300, 1000, MARCHE_AVANT, 50);
        FLAG_ACTION = DEPOSE_PIEDS;
        delay_ms(3000);
        rejoindre(500, 1000, MARCHE_ARRIERE, 50);

#endif

}

void reglage_odometrie()
{

    delay_ms(2000);
    while(!SYS_JACK);
    COULEUR = couleur_depart();



//          init_position_robot (0, 0, 0);
          //orienter(5, 100);




            init_position_robot (-300, 0, 0);
            rejoindre(0, 0, MARCHE_AVANT, 100);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);

          //  faire_des_tours(-32);

//            TIMER_DEBUG = ACTIVE;
//            init_position_robot(0, 0, 0);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2200, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(500, 0, MARCHE_AVANT, 100);
//        TIMER_DEBUG = DESACTIVE;
}