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
/********************* DECLARATION DES VARIABLES GLOBALES *********************/
/******************************************************************************/

static uint8_t espace_libre_depose1 = 6;

void depose (_cote cote);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void strategie()
{
    uint8_t espace_libre_depose1 = 6;
//    uint8_t espace_libre_depose2 = 4;
//    uint8_t espace_libre_depose3 = 4;
    COULEUR = couleur_depart();
    
    #ifdef GROS_ROBOT


        // Inits avant démarage du robot :
        init_jack();
        
        while(!SYS_JACK);

        // Démarage du match
        CPT_TEMPS_MATCH.actif = true;
        EVITEMENT_ADV.actif = OFF;
        EVITEMENT_ADV.mode = STOP;
        
        brake();

        

        delay_ms(1000);   
    #endif

        
    #ifdef PETIT_ROBOT
        
        EVITEMENT_ADV.actif = ON;
        EVITEMENT_ADV.mode = STOP;
        
        jack(); 
        
        // Calage
//        init_position_robot(1070 - LARGEUR_ROBOT /2., 94, 90); // TODO à adapter  
//        rejoindre(950, 360 - LONGUEUR_ROBOT / 2., MARCHE_AVANT, 100);
//        cibler(1000, 600, 100); // On cible le premier module  
        
//        while(!SYS_JACK);
//        delay_ms(1000);
//        while(SYS_JACK);
        
        /*******************/
        /*** DEBUT MATCH ***/
        /*******************/
        
        init_position_robot(710 + LARGEUR_ROBOT / 2., 360 - LONGUEUR_ROBOT / 2., 90);
        CPT_TEMPS_MATCH.actif = true;
        arm_timer_event(AUTOM_PRINCIPALE, 500, SR_START_ROBOT, true);
        
        // on va chercher le premier module
        rejoindre (850, 430, MARCHE_AVANT, 100);
        while(FLAG_ACTION[AUTOM_AVANT] != SR_ROBOT_READY);
        
        rejoindre(1000, 600, MARCHE_AVANT, 100);
       
        // On va chercher le module monochrome avec les pinces avant
        passe_part(700,900, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        inihibit_montage_tour[AUTOM_AVANT] = true;
        passe_part(350,900, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(275,700, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(200,600, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
        delay_ms(200);
        
        // On va chercher le module polychrome au milieu 
        // (module 1 arrière)
        rejoindre(500, 800, MARCHE_ARRIERE, 100);
        rejoindre(500, 1100, MARCHE_ARRIERE, 100);
        delay_ms(500);
        
        // on va chercher le module devant la zone de dépose
        // module 2 arrière
        rejoindre(700, 1200, MARCHE_ARRIERE, 100);
        inihibit_montage_tour[AUTOM_ARRIERE] = true;
        rejoindre(836, 1336, MARCHE_ARRIERE, 100); // déjà un peu enfoncé dans la barre
        
        /***********************/
        /*** PREMIERE DEPOSE ***/
        /***********************/
        
        // on dépose les 4 modules 
        // dépose ARRIERE
        while ( (get_module_tour(ARRIERE) > 0) && (espace_libre_depose1 > 0) )
        {
            depose(ARRIERE);
            avancer_reculer(70, 75);
        }
//        rejoindre(840, 1340, MARCHE_AVANT, 100);
//        rejoindre(800, 1300, MARCHE_AVANT, 100);
        FLAG_ACTION[AUTOM_ARRIERE] = AF_PREPARE_FUSEE;
        
        // on se met en position pour déposer
        rejoindre(868, 1368, MARCHE_AVANT, 100);
            
        // dépose AVANT
        while ( (get_module_tour(AUTOM_AVANT) > 0) && (espace_libre_depose1 > 0) )
        {
            depose(AVANT);
            avancer_reculer(-70, 75);
        }
        
        // on tasse les modules
//        cibler(1150, 2000, 100);
        calage(200, 25);
         
        /**********************/
        /*** VIDAGE FUSEE 1 ***/
        /**********************/
        
        // on va vider la fusée
        FLAG_ACTION[AUTOM_AVANT] = AF_PREPARE_FUSEE;
        passe_part(730, 1230, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
        passe_part(530, 1000, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        passe_part(1150, 875, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        passe_part(1150, 340, MARCHE_ARRIERE, 80, MILIEU_TRAJECTOIRE);
        //        passe_part(1150, LONGUEUR_ROBOT / 2. + 25, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        
        EVITEMENT_ADV.actif = OFF;
        while(FLAG_ACTION[AUTOM_ARRIERE] != AF_CHOPAGE_MODULE_READY);
        FLAG_ACTION[AUTOM_ARRIERE] = AF_ATTRAPE_FUSEE;
        passe_part(1150, 200, MARCHE_ARRIERE, 80, FIN_TRAJECTOIRE);
        
        calage (-400, 40);
        delay_ms(500);
        rejoindre(1150, 270, MARCHE_AVANT, 100);
        
        // on prend le deuxième module dans la fusée
        cibler(1150, 40, 100);
        while(FLAG_ACTION[AUTOM_AVANT] != AF_CHOPAGE_MODULE_READY);
        FLAG_ACTION[AUTOM_AVANT] = AF_ATTRAPE_FUSEE;
        calage(400, 40);
        delay_ms(500);
        
        /***********************/
        /*** DEUXIEME DEPOSE ***/
        /***********************/
        
        // On va déposer au milieu
        EVITEMENT_ADV.actif = ON;
        passe_part(1150, 300, MARCHE_ARRIERE, 100, DEBUT_TRAJECTOIRE);
        passe_part(1500, 650, MARCHE_ARRIERE, 100, MILIEU_TRAJECTOIRE);
        passe_part(1500, 1000, MARCHE_ARRIERE, 100, FIN_TRAJECTOIRE);

        // on tasse (on est à l'envers))
        cibler(1500, 0, 100);
        calage(-300, 50);
        //TODO Init Teta + Y ?
   
        // on recule pour déposer
        avancer_reculer(60, 100);
        
        // on dépose le premier module
        FLAG_ACTION[AUTOM_ARRIERE] = DX_INIT_DEPOSE;
        while( FLAG_ACTION[AUTOM_ARRIERE] != DX_DEPOSE_FINIT );
        
        avancer_reculer(30, 75);
        cibler (1500, 2000, 100);
        
        // on tasse (on est à l'endroit)
        calage(300, 50);
        
        // on recule pour déposer
        avancer_reculer(-60, 75);
        
        // on dépose le deuxième module module
        FLAG_ACTION[AUTOM_AVANT] = DX_INIT_DEPOSE;
        while( FLAG_ACTION[AUTOM_AVANT] != DX_DEPOSE_FINIT );   
                  
    #endif
}

void depose (_cote cote)
{
    int8_t sens = 1;
    
    if (cote == AVANT)
    {
        sens = 1;
    }
    else
    {
        sens = -1;
    }
    
    // autom de montage finit
    while( (FLAG_ACTION[cote] != MT_TOUR_COMPLETE) && 
           (FLAG_ACTION[cote] != MT_RECHERCHE_MODULE_EN_COURS) &&
           (FLAG_ACTION[cote] != DX_DEPOSE_FINIT) );
    
    // on s'assure qu'un module soit en bas pour bourrer le range module
    FLAG_ACTION[cote] = DX_START_DEPOSE;
    while( (FLAG_ACTION[cote] != DX_DEPOSE_READY) &&
           (FLAG_ACTION[cote] != DX_DEPOSE_FINIT) );

    if (FLAG_ACTION[cote] != DX_DEPOSE_FINIT)
    {
        // on bourre le range module
//        rejoindre(868, 1368, cote * MARCHE_AVANT, 100);
        calage(sens * 200, 100);
        
        // on s'éloigne
//        rejoindre(820, 1320, cote * MARCHE_ARRIERE, 100);
        avancer_reculer(sens * (0-50), 50);

        // On initie une dépose
        FLAG_ACTION[cote] = DX_INIT_DEPOSE;
        espace_libre_depose1--;

        // autom de montage finit
        while( FLAG_ACTION[cote] != DX_DEPOSE_FINIT );
    }    
}


void homologation()
{
    COULEUR = couleur_depart();

#ifdef GROS_ROBOT

      // Inits avant démarage du robot :
        init_jack();
        
        while(!SYS_JACK);

        // Démarage du match
        CPT_TEMPS_MATCH.actif = true;
        EVITEMENT_ADV.actif = OFF;
        EVITEMENT_ADV.mode = STOP;

        init_position_robot(180., 988., 0.);
#endif

#ifdef PETIT_ROBOT

        init_position_robot (153, 1030, 0);

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
    EVITEMENT_ADV.actif = OFF;
    
      init_position_robot (0., 0., 0.);
          //orienter(5, 100);




            init_position_robot (-100., 0., 0.);
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