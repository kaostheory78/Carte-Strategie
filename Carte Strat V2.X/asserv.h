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

#define ON                          1
#define OFF                         2

#define Pi                          3.14159265359

#define ROUE_DROITE                 0
#define ROUE_GAUCHE                 1
#define SYS_ROBOT                   2

#define KP_VITESSE                  0.5
#define KI_VITESSE                  0
#define KD_VITESSE                  0.



#define ENTRAXE_MM                  _ENTRAXE_MM
#define DIAMETRE_ROUE_CODEUSE       _DIAMETRE_ROUE_CODEUSE
#define PERIMETRE_ROUE_MM           _PERIMETRE_ROUE_MM


#define MM_PAR_TICKS                (double) ( PERIMETRE_ROUE_MM / CODEUR_D_NB_IMP_LOG )
#define TICKS_PAR_MM                (double) ( CODEUR_D_NB_IMP_LOG / PERIMETRE_ROUE_MM )
#define ENTRAXE_TICKS               (double) ( CODEUR_D_NB_IMP_LOG * ENTRAXE_MM / PERIMETRE_ROUE_MM )

#define SEUIL_DISTANCE_MINI_MM      1
#define SEUIL_DISTANCE_MINI_PAS     (int32_t) SEUIL_DISTANCE_MINI_MM * TICKS_PAR_MM


#define FIN_DEPLACEMENT             2344

#define CONSIGNE_MAX                100
#define VITESSE_CONSIGNE_MM         1 //10
#define VITESSE_CONSIGNE_MAX_MM     5

#define VITESSE_CONSIGNE_PERCENT    60

#define VITESSE_CONSIGNE_MAX_PAS    VITESSE_CONSIGNE_MAX_MM * TICKS_PAR_MM
#define VITESSE_CONSIGNE_PAS        VITESSE_CONSIGNE_MM * TICKS_PAR_MM




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    typedef struct
    {
        double KP;
        double KD;
        double KI;
    }_PID;

    typedef struct
    {
        double consigne;
        double theorique;
        double actuelle;
        //int32_t consigne;
        double acceleration;
    }_systeme_asserv;

    typedef struct
    {
        int32_t droit;
        int32_t gauche;
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
    }_flag_asserv;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/    



void init_position_robot (float x0, float y0, uint32_t teta0);
void calcul_position_robot (void);

void init_commande_moteur(void);
void ecretage_consignes(void);
void reinit_asserv(void);
void asserv_distance(void);
void asserv_vitesse (void);
void asserv_orientation (void);
void asserv();
void PID_distance (void);

void init_flag();

#endif	/* ASSERV_H */

