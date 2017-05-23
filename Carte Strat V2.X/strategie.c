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
        
//        jack(); 
//        while(1);
        
        // Calage
        init_position_robot(1070 - LARGEUR_ROBOT /2., 94, 90); // TODO à adapter  
        rejoindre(950, 360 - LONGUEUR_ROBOT / 2., MARCHE_AVANT, 100);
        cibler(1000, 600, 100); // On cible le premier module  
        
//        rejoindre(1500, 800, MARCHE_AVANT, 100);
//        rejoindre(1500, 1100, MARCHE_AVANT, 100);
//        
//        while(1);
        
        // DEBUT DU MATCH
        while(!SYS_JACK);
        while(SYS_JACK);
        
        CPT_TEMPS_MATCH.actif = true;
        FLAG_ACTION[AUTOM_PRINCIPALE] = SR_START_ROBOT;
        
        delay_ms(4000);
        
        // on va chercher le premier module
        rejoindre(1000, 600, MARCHE_AVANT, 100);
       
        // On va chercher le module monochrome
        passe_part(700,900, MARCHE_AVANT, 100, DEBUT_TRAJECTOIRE);
        passe_part(350,900, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(275,700, MARCHE_AVANT, 100, MILIEU_TRAJECTOIRE);
        passe_part(200,600, MARCHE_AVANT, 100, FIN_TRAJECTOIRE);
        delay_ms(2000);
        
        // On va chercher le module monochrome au milieu
//        FLAG_ACTION[AUTOM_ARRIERE] = MONTAGE_TOUR_PRET;
        rejoindre(500, 800, MARCHE_ARRIERE, 100);
        rejoindre(500, 1100, MARCHE_ARRIERE, 100);
        delay_ms(2000);
        
        // on va chercher le module devant la zone de dépose
//        rejoindre(950, 1400 - DISTANCE_CENTRE_ROBOT_PINCE, MARCHE_ARRIERE, 100);
        rejoindre(700, 1200, MARCHE_ARRIERE, 100);
        rejoindre(836, 1336, MARCHE_ARRIERE, 100);
        rejoindre(868, 1368, MARCHE_ARRIERE, 100);
        delay_ms(4000);
        
        calage(-100, 25); 
        
        if (get_module_tour(LES_DEUX > 0))
        {
             
            // On cible la dépose
//            cibler(-1500, 0, 100);
            calage(-100, 25); 
            
            // dépose ARRIERE
            while ( (get_module_tour(ARRIERE) > 0) && (espace_libre_depose1 > 0) )
            {
                // autom de montage finit
                while( (FLAG_ACTION[AUTOM_ARRIERE] != MT_TOUR_COMPLETE) && 
                       (FLAG_ACTION[AUTOM_ARRIERE] != MT_RECHERCHE_MODULE_EN_COURS) &&
                       (FLAG_ACTION[AUTOM_ARRIERE] != DX_DEPOSE_FINIT) );

                // Début dépose
                FLAG_ACTION[AUTOM_ARRIERE] = DX_START_DEPOSE;
                while(FLAG_ACTION[AUTOM_ARRIERE] != DX_DEPOSE_FINIT);
                espace_libre_depose1--;
                
                // truc pour coller le module 
                avancer_reculer(200, 100);
                avancer_reculer(-200, 100);
            }
            
            cibler(1500, 2000, 100);
            calage(-100, 25);
            
            // dépose AVANT
            while ( (get_module_tour(AVANT) > 0) && (espace_libre_depose1 > 0) )
            {
                // autom de montage finit
                while( (FLAG_ACTION[AUTOM_AVANT] != MT_TOUR_COMPLETE) && 
                       (FLAG_ACTION[AUTOM_AVANT] != MT_RECHERCHE_MODULE_EN_COURS) &&
                       (FLAG_ACTION[AUTOM_ARRIERE] != DX_DEPOSE_FINIT));

                // Début dépose
                FLAG_ACTION[AUTOM_AVANT] = DX_START_DEPOSE;
                while(FLAG_ACTION[AUTOM_AVANT] != DX_DEPOSE_FINIT);
                espace_libre_depose1--;
                
                // truc pour coller le module 
                avancer_reculer(-200, 100); 
                avancer_reculer(200, 100); 
            }          
        }
        
        
        
        
                  
    #endif
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