/* 
 * File:   asserv.h
 * Author: Quentin
 *
 * Created on 30 octobre 2014, 23:11
 */

#ifndef ASSERV_H
#define	ASSERV_H


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

//#include "Config_robots.h"
#include "codeurs.h"


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

#define ON                          1
#define OFF                         2

#define Pi                          3.14159265359

#define ROUE_DROITE                 0
#define ROUE_GAUCHE                 1
#define SYS_ROBOT                   2

#define ASSERV_VITESSE_DISTANCE     0
#define ASSERV_POSITION             1
#define ASSERV_ORIENTATION          2
#define KP_HYBRIDE                  3

#define MARCHE_AVANT                1
#define MARCHE_ARRIERE              -1

#define ORIENTER                    0
#define AVANCER                     1
#define PASSE_PART                  2

#define DEBUT_DEPLACEMENT           0
#define FIN_DEPLACEMENT             1
#define ANGLE_ATTEINT               2
#define DISTANCE_ATTEINTE           4
#define EN_COURS                    6
#define PHASE_NORMAL                7

#define MM                          1
#define XY                          2


/******************************************************************************/
/****************************** REGLAGE DU PID ********************************/
/******************************************************************************/


#define VITESSE_DIS_KP              _VITESSE_DIS_KP
#define VITESSE_DIS_KI              _VITESSE_DIS_KI
#define VITESSE_DIS_KD              _VITESSE_DIS_KD

#define MAX_ERREUR_INTEGRALLE_V     2000

#define POSITION_KP                 _POSITION_KP
#define POSITION_KI                 _POSITION_KI
#define POSITION_KD                 _POSITION_KD

#define ORIENTATION_KP              _ORIENTATION_KP
#define ORIENTATION_KI              _ORIENTATION_KI
#define ORIENTATION_KD              _ORIENTATION_KD

/******************************************************************************/
/************************* CONTRAINTES MECANIQUES *****************************/
/******************************************************************************/

#define ENTRAXE_MM                  _ENTRAXE_MM
#define DIAMETRE_ROUE_CODEUSE       _DIAMETRE_ROUE_CODEUSE
#define PERIMETRE_ROUE_MM           _PERIMETRE_ROUE_MM


#define MM_PAR_TICKS                (double) ( PERIMETRE_ROUE_MM / CODEUR_D_NB_IMP_LOG )
#define TICKS_PAR_MM                (double) ( CODEUR_D_NB_IMP_LOG / PERIMETRE_ROUE_MM )
#define ENTRAXE_TICKS               (double) ( CODEUR_D_NB_IMP_LOG * ENTRAXE_MM / PERIMETRE_ROUE_MM )

/******************************************************************************/
/************************* CONSIGNES DIVERSES *********************************/
/******************************************************************************/

#define SEUIL_DISTANCE_MINI_MM      1
#define SEUIL_DISTANCE_MINI_PAS     (int32_t) ( SEUIL_DISTANCE_MINI_MM * TICKS_PAR_MM )

#define SEUIL_ORIENTATION_MINI      1
#define SEUIL_ORIENTATION_MINI_PAS  (int32_t) ( SEUIL_ORIENTATION_MINI * Pi / 180 * (ENTRAXE_TICKS/2) )

#define SEUIL_IMMOBILITE            _SEUIL_IMMOBILITE

#define CONSIGNE_MAX                100L

    //Association Distance - Vitesse - Acc�l�rations Max
#define VITESSE_CONSIGNE_MAX_MM     _VITESSE_CONSIGNE_MAX_MM
#define VITESSE_CONSIGNE_MAX_PAS    ( VITESSE_CONSIGNE_MAX_MM * TICKS_PAR_MM )

#define DISTANCE_CONSIGNE_MM        _DISTANCE_CONSIGNE_MM
#define DISTANCE_CONSIGNE_PAS       ( DISTANCE_CONSIGNE_MM * TICKS_PAR_MM )

#define ACC_POSITION_CONSIGNE       _ACC_POSITION_CONSIGNE
#define DCC_POSITION_CONSIGNE       _DCC_POSITION_CONSIGNE

    //Association Orientation - Vitesse - Acc�l�ration Max
#define VITESSE_ANGLE_MAX           _VITESSE_ANGLE_MAX
#define VITESSE_ANGLE_PAS           ( VITESSE_ANGLE_MAX * (ENTRAXE_TICKS / 2) )

#define ORIENTATION_CONSIGNE_DEG    _ORIENTATION_CONSIGNE_DEG
#define ORIENTATION_CONSIGNE_PAS    ( ORIENTATION_CONSIGNE_DEG * Pi /180 * (ENTRAXE_TICKS /2) )

#define ACC_ORIENTATION_CONSIGNE    _ACC_ORIENTATION_CONSIGNE
#define DCC_ORIENTATION_CONSIGNE    _DCC_ORIENTATION_CONSIGNE




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    typedef struct
    {
        double orientation;
        double position;
    }_vitesse;

    typedef struct
    {
        _vitesse acceleration;
        _vitesse deceleration;
    }_acc;

    typedef struct
    {
        double KP;
        double KI;
        double KD;
    }_coef_PID;

    typedef struct
    {
        _coef_PID VITESSE_DIS;
        _coef_PID ORIENTATION;
        _coef_PID DISTANCE;
    }_PID;

    typedef struct
    {
        double consigne;
        double theorique;
        double actuelle;
    }_systeme_asserv;

    typedef struct
    {
        double droit;
        double gauche;
    }_commande_moteur;

    typedef struct
    {
        double actuelle;
        double integralle;
        double integralle_precedente;
        double precedente;
        double maximum;
    }_erreur;

    typedef struct
    {
        char totale;
        char position;
        char vitesse;
        char orientation;
        char brake;
        int8_t sens_deplacement;
        char etat_angle;
        char etat_distance;
        char type_deplacement;
        char fin_deplacement;
        char vitesse_fin_nulle;
        char phase_decelaration_orientation;
        char phase_deceleration_distance;
        uint64_t immobilite;
    }_flag_asserv;

/******************************************************************************/
/**************************** ODOMETRIE ***************************************/
/******************************************************************************/

    typedef struct
    {
        //double distance;
        //double distance_precedante;
        double orientation_init;
        double orientation_degre;
        double orientation;
        //double orientation_precedante;
        double X_point;
        double Y_point;
        double X_mm;
        double Y_mm;
    }_robot;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/    

void asserv_brake(void);

void brake();
void unbrake (void);

void detection_blocage (void);

 void saturation_vitesse_max (unsigned char type);
 void saturation_erreur_integralle_vitesse (void);
 void calcul_distance_consigne_XY (void);
 void calcul_vitesse_position (double pourcentage_vitesse);
 void calcul_acceleration_position (void);
 void calcul_vitesse_orientation (double pourcentage_vitesse);
 void calcul_acceleration_orientation (void);


/**
 *  Fonction qui permet d'initialiser le robot � une position pr�cise
 * @param x0 : en mm
 * @param y0 : en mm
 * @param teta0 : en degr�
 */
void init_position_robot (float x0, float y0, uint32_t teta0);


/**
 * Fonction qui permet de calculer la position du robot.
 * C'est la fonction ODOMETRIE.
 *
 * Elle est appel�e toutes les 5 ms par l'asserv.
 */
void calcul_position_robot (void);

/**
 * PErmet de r�initialiser les variables de commande moteur
 *
 * Important car on �crase pas la valeur � chaque it�ration mais ou lui rajoutes la nouvelle
 */
void init_commande_moteur(void);

/**
 * Fonction qui �crete les commandes moteurs pour ne pas envoyer plus que la tension nominale des moteurs
 */
void ecretage_consignes(void);

/**
 * A utiliser pour remettre l'asserv ) 0 avant un nouveau d�placement
 */
void reinit_asserv(void);

/**
 * g�n�re les consignes qui permettent de g�nrer les rampes
 */
void asserv_distance(void);

/**
 * g�n�re les rampes de vitesse : acc�l�ration, d�c�l�ration, phase constante
 */
void asserv_vitesse_distance (void);

/**
 *
 */
void asserv_orientation (void);

/**
 *
 */
void asserv_vitesse_orientation (void);

/**
 * permet de lancer toutes les fonctions d'asservissement !!
 */
void asserv();


/**
 * Fonction qui calcul le PID
 *
 * Asserv position : renvoit la valeur de l'erreur multipli� par le PID
 *
 * Asserv de vitesse_distance : applique la commande moteur de la roue droite et gauche
 *
 * @param type : #define ASSERV_VITESSE_DISTANCE, ASSERV_POSITION, ASSERV_ORIENTATION
 * @return
 */
double fonction_PID (unsigned char type);


/**
 * inscrit les valeurs des KI, KP, KD dans leurs variables respectives.
 */
void reglage_PID (void);

void init_flag();

#endif	/* ASSERV_H */

