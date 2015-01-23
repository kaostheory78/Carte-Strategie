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


/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

#define GROS_ROBOT
//#define ROBOT_TEST

#define _Pi                             3.14159265359 

#ifdef GROS_ROBOT

    //Paramètres Odométrie
    #define _ENTRAXE_MM                  306.4
    #define _DIAMETRE_ROUE_CODEUSE       0
    #define _PERIMETRE_ROUE_MM           194.4

    //Paramètres Vitesse Asserv
        //Association distance - Vitesse - Accélérations max
    #define _VITESSE_CONSIGNE_MAX_MM     3.
    #define _DISTANCE_CONSIGNE_MM        50.

    #define _ACC_POSITION_CONSIGNE       20.
    #define _DCC_POSITION_CONSIGNE       40.

        //Association Angle - Vitesse - Accélérations max
    #define _VITESSE_ANGLE_MAX           0.02
    #define _ORIENTATION_CONSIGNE_DEG    90.

    #define _ACC_ORIENTATION_CONSIGNE    4.
    #define _DCC_ORIENTATION_CONSIGNE    6.

        //Autres réglages asserv
    #define _SEUIL_IMMOBILITE            100
   
    //PID
    #define _VITESSE_DIS_KP              0.4  //0.2
    #define _VITESSE_DIS_KI              0.02 //0.01
    #define _VITESSE_DIS_KD              0.2

    #define _POSITION_KP                 1.
    #define _POSITION_KI                 0.
    #define _POSITION_KD                 0.

    #define _ORIENTATION_KP              1.
    #define _ORIENTATION_KI              0.
    #define _ORIENTATION_KD              0.

    //Tensions d'alimentations
    #define _TENSION_MOTEUR_DROIT        24L
    #define _TENSION_MOTEUR_GAUCHE       24L

    #define _TENSION_SORTIE_PTN          34L


    //Paramètres codeurs, et moteurs
    #define _SENS_ROT_D                  1L
    #define _SENS_ROT_G                  -1L

    #define _RAPPORT_REDUCTION           1

    #define _CODEUR_D_NB_IMP             3600
    #define _CODEUR_G_NB_IMP             3600

    #define _RESOLUTION_LOGICIELLE       1

    //PWM
    #define _AVANCER_MOTEUR_D            1 //0
    #define _RECULER_MOTEUR_D            0   //1

    #define _AVANCER_MOTEUR_G            0 //1
    #define _RECULER_MOTEUR_G            1   //0



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

