/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: Config_robots.h
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 4 décembre 2014, 12:52
 *******************************************************************************
 *  Fichier qui permet très facilemet d'adapter les paramètres pour changer
 *  de robots.
 *
 ******************************************************************************/

#ifndef CONFIG_ROBOTS_H
#define	CONFIG_ROBOTS_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "system.h"

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

#define GROS_ROBOT
//#define PETIT_ROBOT
//#define ROBOT_TEST

#define _Pi                             3.14159265359


#ifdef GROS_ROBOT
    
    #define CARTE_V2

    /**************************************************************************/
    /****************************** ODOMETRIE *********************************/
    /**************************************************************************/
    #define _ENTRAXE_MM                  308.264 //308.2     //Pas assez tourné -> augmenter entraxe
    #define _DIAMETRE_ROUE_CODEUSE       0
    #define _PERIMETRE_ROUE_MM           194.455 //194.455      // Si distance parcourue trop grande -> Augmenter la taille des roues

        //+COEF_D -> + a droite
    #define  _COEF_G                    (double) 1.00515 //1.00531 474
    #define  _COEF_D                    (double) 1.


    /**************************************************************************/
    /******************************** ASSERV **********************************/
    /**************************************************************************/
        //Association distance - Vitesse - Accélérations max
    #define _VITESSE_CONSIGNE_MAX_MM     2.5
    #define _VITESSE_DISTANCE_MIN        0.75
    #define _VITESSE_MAX_MM_TENSION      6.
    #define _DISTANCE_CONSIGNE_MM        500. 

    #define _ACC_POSITION_CONSIGNE       3.
    #define _DCC_POSITION_CONSIGNE       4. //8
    #define _ACC_POSITION_MIN            1.5
    #define _DCC_POSITION_MIN            1.5

    #define _COEF_FREINAGE              1.25

        //Association Angle - Vitesse - Accélérations max
    #define _VITESSE_ANGLE_MAX           0.02 //0.02
    #define _VITESSE_ANGLE_MIN           0.005
    #define _ORIENTATION_CONSIGNE_DEG    90.

    #define _ACC_ORIENTATION_CONSIGNE    4.
    #define _DCC_ORIENTATION_CONSIGNE    6.
    #define _ACC_ORIENTATION_MIN         2.
    #define _DCC_ORIENTATION_MIN         3.

        //Autres réglages asserv
    #define _SEUIL_IMMOBILITE            100 //200
    #define _MAX_ERREUR_INTEGRALLE_V     4500
    #define _MAX_E_INTEGRALLE_BRAKE      500.

    /**************************************************************************/
    /********************************* PID ************************************/
    /**************************************************************************/
   
    //PID
    #define _VITESSE_DIS_KP              0.45  //0.4
    #define _VITESSE_DIS_KI              0.02 //0.02
    #define _VITESSE_DIS_KD              0.25 //0.2

    #define _POSITION_KP                 1.
    #define _POSITION_KI                 0.
    #define _POSITION_KD                 0.

    #define _ORIENTATION_KP              1.
    #define _ORIENTATION_KI              0.
    #define _ORIENTATION_KD              0.

    #define KP_BRAKE                    0.05
    #define KI_BRAKE                    0.0001
    #define KD_BRAKE                    0.5

    /**************************************************************************/
    /******************************* TENSIONS *********************************/
    /**************************************************************************/

    //Tensions d'alimentations
    #define _TENSION_MOTEUR_DROIT        24L
    #define _TENSION_MOTEUR_GAUCHE       24L

    #define _TENSION_MOTEUR_X            12L

    #define _TENSION_SORTIE_PTN          34L 


    /**************************************************************************/
    /*************************** CONFIGS MOTEURS ******************************/
    /**************************************************************************/


    //Paramètres codeurs, et moteurs
    #define _RAPPORT_REDUCTION           1

    #define _CODEUR_D_NB_IMP             3600
    #define _CODEUR_G_NB_IMP             3600

    #define _RESOLUTION_LOGICIELLE       1

    //PWM
#ifdef CARTE_V1
    #define _SENS_ROT_D                  1L
    #define _SENS_ROT_G                  -1L

    #define _AVANCER_MOTEUR_D            1 
    #define _RECULER_MOTEUR_D            0   

    #define _AVANCER_MOTEUR_G            0
    #define _RECULER_MOTEUR_G            1
#endif
#ifdef CARTE_V2
    #define _SENS_ROT_D                  -1L
    #define _SENS_ROT_G                  1L

    #define _AVANCER_MOTEUR_D            1
    #define _RECULER_MOTEUR_D            0

    #define _AVANCER_MOTEUR_G            1
    #define _RECULER_MOTEUR_G            0

    #define _AVANCER_MOTEUR_X            0
    #define _RECULER_MOTEUR_X            1

    #define _AVANCER_MOTEUR_Y            0
    #define _RECULER_MOTEUR_Y            1
#endif

    /**************************************************************************/
    /***************************** PORTS CARTE ********************************/
    /**************************************************************************/

    
    //La définition des ports relatif à chaque carte se trouve
    //Dans le fichier system.h
    
    #define CAPT_US_BALISE              CAPTEUR1
    #define CAPT_GOBELET_D              CAPTEUR3
    #define CAPT_GOBELET_G              CAPTEUR4
    #define CAPT_US_ARRIERE             CAPTEUR7
    #define CAPT_US_AV_GAUCHE           CAPTEUR2
    #define CAPT_US_AV_DROIT            CAPTEUR8

    #define INCLINOMETRE                CAPTEUR5

    #define _CAPTEUR1_ANALOGIQUE        DIGITAL
    #define _CAPTEUR2_ANALOGIQUE        DIGITAL
    #define _CAPTEUR3_ANALOGIQUE        DIGITAL

#ifdef CARTE_V1
    #define _SYS_COULEUR                CAPTEUR2
#endif


    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/



#endif




    #ifdef PETIT_ROBOT

    //#define CARTE_V1
    #define CARTE_V2

    /**************************************************************************/
    /****************************** ODOMETRIE *********************************/
    /**************************************************************************/
    #define _ENTRAXE_MM                  236.11
    #define _DIAMETRE_ROUE_CODEUSE       51.98//51.9715     //Toujours dans le sens de l'erreur : aler plus loin = diminuer D
    #define _PERIMETRE_ROUE_MM           (_DIAMETRE_ROUE_CODEUSE * _Pi)


    //+COEF_D -> + a droite
    #define  _COEF_D                    (double) 1.  //346  //1.00372
    #define  _COEF_G                    (double) 1.0034

    /**************************************************************************/
    /******************************** ASSERV **********************************/
    /**************************************************************************/
        //Association distance - Vitesse - Accélérations max
    #define _VITESSE_CONSIGNE_MAX_MM     2.8 
    #define _VITESSE_DISTANCE_MIN        0.6
    #define _VITESSE_MAX_MM_TENSION      3.
    #define _DISTANCE_CONSIGNE_MM        500.

    #define _ACC_POSITION_CONSIGNE       2.5
    #define _DCC_POSITION_CONSIGNE       2.8 //2.8
    #define _ACC_POSITION_MIN             1.5 //1.
    #define _DCC_POSITION_MIN             1.

    #define _COEF_FREINAGE                1.

        //Association Angle - Vitesse - Accélérations max
    #define _VITESSE_ANGLE_MAX           0.015 
    #define _VITESSE_ANGLE_MIN           0.005
    #define _ORIENTATION_CONSIGNE_DEG    90.

    #define _ACC_ORIENTATION_CONSIGNE    4. 
    #define _DCC_ORIENTATION_CONSIGNE    3. 
    #define _ACC_ORIENTATION_MIN         0.5
    #define _DCC_ORIENTATION_MIN         0.5

        //Autres réglages asserv
    #define _SEUIL_IMMOBILITE            75 //200
    #define _MAX_ERREUR_INTEGRALLE_V     3500
    #define _MAX_E_INTEGRALLE_BRAKE      12000.

    /**************************************************************************/
    /********************************* PID ************************************/
    /**************************************************************************/

    //PID
    #define _VITESSE_DIS_KP              0.8 
    #define _VITESSE_DIS_KI              0.03
    #define _VITESSE_DIS_KD              0.2

    #define _POSITION_KP                 1.
    #define _POSITION_KI                 0.
    #define _POSITION_KD                 0.

    #define _ORIENTATION_KP              1.
    #define _ORIENTATION_KI              0.
    #define _ORIENTATION_KD              0.

    #define KP_BRAKE                    0.04 
    #define KI_BRAKE                    0.0015
    #define KD_BRAKE                    0.8

    /**************************************************************************/
    /******************************* TENSIONS *********************************/
    /**************************************************************************/

    //Tensions d'alimentations
    #define _TENSION_MOTEUR_DROIT        12L
    #define _TENSION_MOTEUR_GAUCHE       12L

    #define _TENSION_MOTEUR_X            12L

    #define _TENSION_SORTIE_PTN          17L


    /**************************************************************************/
    /*************************** CONFIGS MOTEURS ******************************/
    /**************************************************************************/


    //Paramètres codeurs, et moteurs
    //Sens rotation codeur
    #define _SENS_ROT_D                  -1L
    #define _SENS_ROT_G                  1L

    #define _RAPPORT_REDUCTION           1

    #define _CODEUR_D_NB_IMP             3600
    #define _CODEUR_G_NB_IMP             3600

    #define _RESOLUTION_LOGICIELLE       1

    //PWM : sens de rotation du moteur
    #define _AVANCER_MOTEUR_D            0
    #define _RECULER_MOTEUR_D            1

    #define _AVANCER_MOTEUR_G            0
    #define _RECULER_MOTEUR_G            1

    #define _AVANCER_MOTEUR_X            0
    #define _RECULER_MOTEUR_X            1

    #define _AVANCER_MOTEUR_Y            0
    #define _RECULER_MOTEUR_Y            1

    /**************************************************************************/
    /***************************** PORTS CARTE ********************************/
    /**************************************************************************/


    //La définition des ports relatif à chaque carte se trouve
    //Dans le fichier system.h

    #define CAPT_PINCE                 CAPTEUR1
    #define CAPT_IR_ARRIERE_GAUCHE     CAPTEUR2
    #define CAPT_IR_ARRIERE_DROIT      CAPTEUR6
    #define CAPT_IR_ARRIERE_CENTRE     CAPTEUR7
    #define CAPT_US_DROIT              CAPTEUR5
    #define CAPT_US_GAUCHE             CAPTEUR4
    #define CAPT_US_BALISE             CAPTEUR8

    #define _CAPTEUR1_ANALOGIQUE        DIGITAL
    #define _CAPTEUR2_ANALOGIQUE        DIGITAL
    #define _CAPTEUR3_ANALOGIQUE        DIGITAL

#ifdef CARTE_V1
    #define _SYS_COULEUR                CAPTEUR2
#endif


    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/



#endif



#ifdef ROBOT_TEST

    //Paramètres odométries
    #define _ENTRAXE_MM                  218.
    #define _DIAMETRE_ROUE_CODEUSE       51.9
    #define _PERIMETRE_ROUE_MM           (_DIAMETRE_ROUE_CODEUSE * _Pi)

    //PID
    #define _VITESSE_DIS_KP              0.2 //4 //0.2
    #define _VITESSE_DIS_KI              0.2 //0.5 //0.2
    #define _VITESSE_DIS_KD              0. //20 //0.35

    #define _POSITION_KP                 1.
    #define _POSITION_KI                 0.
    #define _POSITION_KD                 0.

    #define _ORIENTATION_KP              1.
    #define _ORIENTATION_KI              0.
    #define _ORIENTATION_KD              0.

    //Tensions cartes
    #define _TENSION_MOTEUR_DROIT        12L
    #define _TENSION_MOTEUR_GAUCHE       12L

    #define _TENSION_SORTIE_PTN          17L


    //Codeurs
    #define _SENS_ROT_D                  1L
    #define _SENS_ROT_G                  -1L

    #define _CODEUR_D_NB_IMP             100
    #define _CODEUR_G_NB_IMP             100

    #define _RAPPORT_REDUCTION           4

    #define _RESOLUTION_LOGICIELLE       4


    //PWM
    #define _AVANCER_MOTEUR_D           1 //0
    #define _RECULER_MOTEUR_D           0   //1

    #define _AVANCER_MOTEUR_G           1 //1
    #define _RECULER_MOTEUR_G           0   //0


#endif



#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_ROBOTS_H */

