/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: srategie.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 07 f�vrier 2015, 22:47
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


        // Inits avant d�marage du robot :
        init_jack();
        delay_ms(2000);

        while(!SYS_JACK);

        // D�marage du match
        TIMER_90s = ACTIVE;
        EVITEMENT_ADV_AVANT = ON;
        STRATEGIE_EVITEMENT = STOP;

        init_position_robot(180., 988., 0.);
        FLAG_ACTION = INIT_ASCENSEUR;

        // Direction Gobelet face zone d�part
        passe_part(620, 988, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(683, 991, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(675, 998, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(742, 1112,MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        // Direction Gobelet pour lib�rer les deux pieds pour R2
        passe_part(663, 334, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(445, 324, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(400, 310, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(325, 310, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);


        // Retour au bercail pour d�pose N�1
        EVITEMENT_ADV_AVANT = OFF;
        EVITEMENT_ADV_ARRIERE = ON;
        passe_part(570,455,MARCHE_ARRIERE,100,DEBUT_TRAJECTOIRE);

        FLAG_ACTION = NE_RIEN_FAIRE;
        EVITEMENT_ADV_ARRIERE = OFF;
        EVITEMENT_ADV_AVANT = ON;
        passe_part(550,550,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(650,800,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);

        // On d�sactive dans la zone de d�part
        EVITEMENT_ADV_AVANT = OFF;
        passe_part(558,990,MARCHE_AVANT,100,MILIEU_TRAJECTOIRE);
        passe_part(270,990,MARCHE_AVANT,100,FIN_TRAJECTOIRE);

        // On v�rifie confirme que le gobelet a bien �t� attrap�
        uint8_t status_pince_G = LIBRE, status_pince_D = LIBRE;
        
        if (check_capteur(DROIT) == 0)
            status_pince_D = FERMER;
        if (check_capteur(GAUCHE) == 0)
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

        // On a encore un gobelet � d�poser
        if (status_pince_D == FERMER) //strat normale (2 gobelets du d�part)
        {
            delay_ms(500);
            avancer_reculer(-100, 50);

            EVITEMENT_ADV_ARRIERE = ON;
            passe_part(750, 1000, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);

            EVITEMENT_ADV_ARRIERE = OFF;
            EVITEMENT_ADV_AVANT = ON;

            passe_part(1100, 900, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
            passe_part(1900, 900, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
            passe_part(2300, 545, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
            passe_part(2760, 545, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
            // D�pose du gobelet
            pince(DROITE, OUVERTE);

            // On essaye d'avoir celui de l'adversaire
            avancer_reculer(-80, 50);
            FLAG_ACTION = ATTRAPE_GOBELET;

            // On s'�loigne du gobelet
            EVITEMENT_ADV_AVANT = OFF;
            EVITEMENT_ADV_ARRIERE = ON;
            rejoindre (2670, get_Y(), MARCHE_ARRIERE, 50);
            
//            // Recalage contre la bordure
//            orienter(-90, 100);
//            //ne pas mettre d'�vitement adv : risque de voir un robot dans la zone adverse
//            EVITEMENT_ADV_ARRIERE = OFF;
//            avancer_reculer(-200, 50);
//            init_Y(667);
//            init_orientation(-90);


            // On se dirige vers le clap en essayant d'attraper le gobelet adverse au passage
            EVITEMENT_ADV_ARRIERE = OFF;
            EVITEMENT_ADV_ARRIERE = ON;
            rejoindre(2670, 220, MARCHE_AVANT, 50);
            rejoindre(2550, 190, MARCHE_AVANT, 50);

            FLAG_ACTION = NE_RIEN_FAIRE;

            // On fait le clap
            bras(GAUCHE, OUVERT);
            rejoindre (2220, 210, MARCHE_AVANT, 50);

            if (check_capteur(GAUCHE) == 0) // 2 gobelets du d�part + gobelet adverse
            {
                //depose du gobelet dans la zone de gauche
                //+ Alignement marches
                bras(GAUCHE, FERMER);
                passe_part (2100, 800, MARCHE_AVANT, 80, DEBUT_TRAJECTOIRE);
                passe_part (2200, 1210, MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                passe_part (2300, 1300, MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                passe_part (2400, 1450, MARCHE_AVANT, 80, MILIEU_TRAJECTOIRE);
                passe_part (2700, 1450, MARCHE_AVANT, 80, FIN_TRAJECTOIRE);
                pince(GAUCHE, OUVERTE);

                delay_ms(500);

                EVITEMENT_ADV_AVANT = OFF;
                EVITEMENT_ADV_ARRIERE = ON;
                rejoindre (2200, 1450, MARCHE_ARRIERE, 100);
                pince(DROITE, RANGEMENT);
                pince(GAUCHE, RANGEMENT);
                delay_ms(500);
                ascenseur(ARRIERE);

                //desactiver evitement
                EVITEMENT_ADV_ARRIERE = OFF;
                avancer_reculer(-200, 100);
                init_X(2143);
                init_orientation(0);

                EVITEMENT_ADV_AVANT = ON;
                rejoindre (2300, 1450, MARCHE_AVANT, 100);

                passe_part(2220, 1220, MARCHE_AVANT, 50, DEBUT_TRAJECTOIRE);
                passe_part(1930, 830, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                passe_part(1240, 830, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
                passe_part(1240, 1260, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
                //passe_part(1180, 1160, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);
                //rejoindre (1180, 1160, MARCHE_AVANT, 50);
                orienter(90, 100);

            }
            // Strate normale 2 gobelet d�part mais pas gobelet adverse
            else // Pas de gobelet => direction les marches
            {
                bras(GAUCHE, FERMER);
                pince(DROITE, RANGEMENT);
                pince(GAUCHE, RANGEMENT);
                delay_ms(500);
                ascenseur(ARRIERE);

                //se diriger vers les marches
                rejoindre(1240, 830, MARCHE_AVANT, 50);
                rejoindre(1240, 1260, MARCHE_AVANT, 50);
                orienter(90, 100);
            }


            //rejoindre(2600, 400, MARCHE_AVANT, 100);
        }
        else // 1 ou 0 gobelets au d�part
        {
            // Pas de gobelets dans les pinces
            pince(GAUCHE, RANGEMENT);
            pince(DROITE, RANGEMENT);
            delay_ms(500);
            ascenseur(ARRIERE);

            // On sors de la zone de d�part
            avancer_reculer(-100, 50);

            EVITEMENT_ADV_ARRIERE = ON;
            passe_part(750, 1000, MARCHE_ARRIERE, 50, DEBUT_TRAJECTOIRE);

            EVITEMENT_ADV_ARRIERE = OFF;
            EVITEMENT_ADV_AVANT = ON;

            // On se dirige dans la zone de droite
            passe_part(1100, 900, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
            passe_part(1900, 900, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
            passe_part(2300, 545, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
            ascenseur(AVANT);
            passe_part(2670, 545, MARCHE_AVANT, 50, FIN_TRAJECTOIRE);

            // on va faire le clap en tentant le gobelet adverse au passage
            pince(GAUCHE, OUVERTE);
            delay_ms(500);
            FLAG_ACTION = ATTRAPE_GOBELET;
            rejoindre(2670, 220, MARCHE_AVANT, 50);
            rejoindre(2550, 190, MARCHE_AVANT, 50);

            FLAG_ACTION = NE_RIEN_FAIRE;

            // On fait le clap
            bras(GAUCHE, OUVERT);
            rejoindre (2220, 190, MARCHE_AVANT, 50);



            if (check_capteur(GAUCHE) == 0) // 1 ou 0 gobelet au d�part + gobelet adverse
            {
                // Si on a eu le gobelet adverse
                bras(GAUCHE, FERMER);
                EVITEMENT_ADV_AVANT = OFF;
                EVITEMENT_ADV_ARRIERE = ON;
                passe_part(2500, 400, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
                passe_part(2650, 550, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
                passe_part(2680, 550, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
                EVITEMENT_ADV_ARRIERE = OFF;
                passe_part(2720, 550, MARCHE_ARRIERE, 100, FIN_TRAJECTOIRE);

                // calage
                orienter(180, 100);
                avancer_reculer(-300, 100);
                init_X(2890);
                init_orientation(180);

                // on se pr�pare
                EVITEMENT_ADV_AVANT = ON;
                avancer_reculer(200, 100);
                
                // (on coupe l'�vitement pour la rotation)
                EVITEMENT_ADV_AVANT = OFF;
                orienter(0, 50);

                // on relache le gobelet
                pince(GAUCHE, OUVERTE);
                EVITEMENT_ADV_ARRIERE = ON;
                avancer_reculer(-100, 100);

                //rangement des pinces
                bras(GAUCHE, FERMER);
                pince(DROITE, RANGEMENT);
                pince(GAUCHE, RANGEMENT);
                delay_ms(500);
                ascenseur(ARRIERE);

                EVITEMENT_ADV_ARRIERE = OFF;
                EVITEMENT_ADV_AVANT = ON;

                //direction les marches
                rejoindre(1240, 830, MARCHE_AVANT, 50);
                rejoindre(1240, 1260, MARCHE_AVANT, 50);
                orienter(90, 100);

            }
            else // Aucun gobelet = match de merde
            {
                //direction les marches direct
                //direction les marches :
                bras(GAUCHE, FERMER);
                pince(DROITE, RANGEMENT);
                pince(GAUCHE, RANGEMENT);
                delay_ms(500);
                ascenseur(ARRIERE);

                //se diriger vers les marches
                rejoindre(1200, 830, MARCHE_AVANT, 50);
                rejoindre(1200, 1260, MARCHE_AVANT, 50);
                orienter(90, 100);
            }
        }

        //Mont�e des marches
        EVITEMENT_ADV_ARRIERE = OFF;
        EVITEMENT_ADV_AVANT = OFF;

        FLAG_ASSERV.totale = OFF;
        FLAG_ACTION = MONTEE_MARCHE;
        delay_ms(10000);

        
    #endif

        
    #ifdef PETIT_ROBOT
        init_position_robot (153, 1030, 0);
        //Init d�part
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
        delay_ms(1200);



        /******** D�pose pile de 3 */
        /*
        //Placement d�pose
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
        passe_part(500, 170, MARCHE_AVANT, 50, MILIEU_TRAJECTOIRE);
        passe_part(890, 170, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);

        //dernier clap
        passe_part(1000, 250, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        fermer_bras(BRAS_GAUCHE);

        //direction depose centre
        passe_part(600, 800, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(460, 1010, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //r�alignement
        orienter(90, 500);

        //D�pose des pieds
        eteindre_pompe();
        FLAG_ACTION = PIEDS_4; 

        //Sortie zone de d�part
        passe_part(700, 1300, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);

        passe_part (580, 1670, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);

        ////R�cup�ration 4�me pieds
        EVITEMENT_ADV_AVANT = OFF;
        passe_part(300,1770,MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(170, 1770, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);

        //Aspiration 2 pieds
        allumer_pompes();
        EVITEMENT_ADV_AVANT = OFF;
        EVITEMENT_ADV_ARRIERE = ON;
        rejoindre(785, 1745,  MARCHE_ARRIERE, 50);
        EVITEMENT_ADV_ARRIERE = OFF;
        
        /***** Pied un peu chiant **************
        orienter(-90, 15);
        rejoindre(785, 1830, MARCHE_ARRIERE, 25);
        orienter(-70, 50);
        *******************************/

        // Placement zone de d�part
        EVITEMENT_ADV_AVANT = ON;
        rejoindre(700, 1125, MARCHE_AVANT, 100);

        //rentr�e dans la zone de d�part
        EVITEMENT_ADV_AVANT = OFF;
        rejoindre(640, 1090, MARCHE_AVANT, 100);

        //Montage de la tour (d�passement des 350, ejection de la balle
        //descente des pieds pour laisser passer la balle
        angle_AX12(PINCE_HAUT, 555, 1023, SANS_ATTENTE);
        angle_AX12(PINCE_MILIEU, 555, 1023, SANS_ATTENTE);
        angle_AX12(PINCE_BAS, 555, 1023, SANS_ATTENTE);
        delay_ms(1500);

        //on ressert le tout pour que la tour soit bien droite
        pinces(PINCE_HAUT, FERMER);
        pinces(PINCE_MILIEU, FERMER);
        pinces(PINCE_BAS, FERMER);

        //Ejection de la balle
        delay_ms(500);
        //angle_AX12(PINCE_HAUT, 550, 1023, SANS_ATTENTE);
        ejecter_balle();
        delay_ms(500);

        //empilement si n�c�ssaire
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        //Pieds 5
        rejoindre(550, 1000, MARCHE_AVANT, 100);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(3000);
        FLAG_ACTION = NE_RIEN_FAIRE;

        // Pieds 6
        rejoindre(450, 975, MARCHE_AVANT, 100);
        FLAG_ACTION = ZONE_DEPART;
        delay_ms(2000);

        rejoindre(320, 1000, MARCHE_AVANT, 50);
        FLAG_ACTION = DEPOSE_PIEDS;
        delay_ms(3000);
        //rejoindre(600, 975, MARCHE_ARRIERE, 50);

        avancer_reculer(-300, 100);

        //orienter (0, 20);
        eteindre_pompe();
        
        /******** R�cup gobelet en passe part*/
        /*
        passe_part (1340, 400, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
        FLAG_ACTION = ATTRAPE_GOBELET;
        passe_part (910, 1000, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part (910, 1250, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part (360, 1000, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
        FLAG_ACTION = DEPOSE_GOBELET;*/

        /******* Placement c�t� marche ***

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

        //Init d�part
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

        //r�alignement
        orienter(90, 500);

        //D�pose des pieds
        eteindre_pompe();
        FLAG_ACTION = PIEDS_4;

        //Sortie zone de d�part
        rejoindre (700, 1300, MARCHE_AVANT, 50);

        rejoindre(580, 1670, MARCHE_AVANT, 50);

        //R�cup�ration 4�me pieds
        rejoindre(170, 1770, MARCHE_AVANT, 50);

         rejoindre ( 700, 1125, MARCHE_AVANT, 50);

        //rentr�e dans la zone de d�part
        EVITEMENT_ADV_AVANT = OFF;
        rejoindre(640, 1090, MARCHE_AVANT, 50);

        //Montage de la tour (d�passement des 350, ejection de la balle
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

        //empilement si n�c�ssaire
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
    //while(!SYS_JACK);
    COULEUR = couleur_depart();
#ifdef GROS_ROBOT
    init_jack();
#endif
    EVITEMENT_ADV_ARRIERE = OFF;
    EVITEMENT_ADV_AVANT = OFF;




      init_position_robot (0, 0, 0);
          //orienter(5, 100);




            init_position_robot (-100, 0, 0);
            rejoindre(0, 0, MARCHE_AVANT, 100);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);
            trapeze(MARCHE_AVANT);

//           faire_des_tours(-32);
            while(1);
////
////            TIMER_DEBUG = ACTIVE;
//            init_position_robot(0, 0, 0);
            ////Horraire
//       rejoindre(2000, 0, MARCHE_AVANT, 50);
//       orienter(90, 50);
//        rejoindre(300, 0, MARCHE_AVANT, 50);
//        orienter(-90, 50);
//        rejoindre(2000, 0, MARCHE_AVANT, 50);
//        orienter(90, 50);
//        rejoindre(300, 0, MARCHE_AVANT, 50);
//        orienter (-90, 50);
//        rejoindre(2000, 0, MARCHE_AVANT, 50);
//        orienter(90, 50);
//        rejoindre(300, 0, MARCHE_AVANT, 50);
//        orienter (-90, 50);
//        rejoindre(2000, 0, MARCHE_AVANT, 50);
//        orienter(90, 50);
//        rejoindre(300, 0, MARCHE_AVANT, 50);
//        orienter (-90, 50);
//        rejoindre(2000, 0, MARCHE_AVANT, 50);
//        orienter(90, 50);
//        rejoindre(300, 0, MARCHE_AVANT, 50);
//        orienter (-90, 50);
//        rejoindre(2000, 0, MARCHE_AVANT, 50);
//        orienter(90, 50);
//        rejoindre(300, 0, MARCHE_AVANT, 50);
//        orienter(-90, 50);


            //// Anti horaire
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter(90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter (90, 50);
        rejoindre(2000, 0, MARCHE_AVANT, 50);
        orienter(-90, 50);
        rejoindre(300, 0, MARCHE_AVANT, 50);
        orienter(90, 50);
        rejoindre(500, 0, MARCHE_AVANT, 100);


//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(2000, 0, MARCHE_AVANT, 100);
//        rejoindre(300, 0, MARCHE_AVANT, 100);
//        rejoindre(500, 0, MARCHE_AVANT, 100);
//        TIMER_DEBUG = DESACTIVE;

        delay_ms(10000);
}