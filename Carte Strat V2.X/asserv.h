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

#include "Config_robots.h"
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

#define DEBUT_DEPLACEMENT           0
#define FIN_DEPLACEMENT             1


/******************************************************************************/
/****************************** REGLAGE DU PID ********************************/
/******************************************************************************/


#define VITESSE_DIS_KP              _VITESSE_DIS_KP
#define VITESSE_DIS_KI              _VITESSE_DIS_KI
#define VITESSE_DIS_KD              _VITESSE_DIS_KD

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
#define SEUIL_DISTANCE_MINI_PAS     (int32_t) SEUIL_DISTANCE_MINI_MM * TICKS_PAR_MM


#define CONSIGNE_MAX                100L
#define VITESSE_CONSIGNE_MM         3//3
#define VITESSE_CONSIGNE_MAX_MM     5

#define VITESSE_CONSIGNE_PERCENT    80

#define VITESSE_CONSIGNE_MAX_PAS    VITESSE_CONSIGNE_MAX_MM * TICKS_PAR_MM
#define VITESSE_CONSIGNE_PAS        VITESSE_CONSIGNE_MM * TICKS_PAR_MM


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
        double precedente;
        double maximum;
    }_erreur;

    typedef struct
    {
        char totale;
        char position;
        char vitesse;
        char orientation;
        char fin_deplacement;
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

