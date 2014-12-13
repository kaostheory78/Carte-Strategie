/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: sserv.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08/11/2014
 *******************************************************************************
 *
 *
 ******************************************************************************/

#include "system.h"


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

_robot ROBOT;

_PID PID;

_systeme_asserv DISTANCE;
_systeme_asserv ORIENTATION;
_systeme_asserv VITESSE[3];

_acc acc;

_erreur ERREUR_DISTANCE;
_erreur ERREUR_VITESSE[2];
_erreur ERREUR_ORIENTATION;

_commande_moteur COMMANDE;

_flag_asserv FLAG_ASSERV;



/******************************************************************************/
/***************************** Fonctions Inits ********************************/
/******************************************************************************/

void init_flag()
{
    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

    acc.acceleration.position = 6; //4
    acc.deceleration.position = 6; //3

    acc.acceleration.orientation = 0;
    acc.deceleration.orientation = 0;

    VITESSE[SYS_ROBOT].theorique = 0;

    DISTANCE.consigne = 2500 *  TICKS_PAR_MM;
    reinit_asserv();
    reglage_PID();

    //PutsUART(UART_XBEE, "demarage asserv \n\r");
    //delay_ms(500);

    PORTCbits.RC5 = 1;
    FLAG_ASSERV.totale = ON;
}

void init_position_robot (float x0, float y0, uint32_t teta0)
{
    ROBOT.X_mm = x0;
    ROBOT.Y_mm = y0;

    ROBOT.X_point = ROBOT.X_mm * TICKS_PAR_MM;
    ROBOT.Y_point = ROBOT.Y_mm * TICKS_PAR_MM;
    
    ROBOT.orientation_init = teta0 * Pi / 180;
    //ROBOT.orientation_precedante = teta0;
    //ROBOT.distance = 0;
    //ROBOT.distance_precedante = 0;
}

void init_commande_moteur(void)
{
    COMMANDE.droit = 0;
    COMMANDE.gauche = 0;
}

void reinit_asserv(void)
{
    DISTANCE.actuelle = 0;
    DISTANCE.theorique = 0;
    ORIENTATION.actuelle = 0;

    ERREUR_DISTANCE.actuelle = 0;
    ERREUR_DISTANCE.integralle = 0;
    ERREUR_DISTANCE.precedente = 0;

    VITESSE[ROUE_DROITE].actuelle = 0;
    VITESSE[ROUE_DROITE].consigne = 0;
    VITESSE[ROUE_DROITE].theorique = 0;

    VITESSE[ROUE_GAUCHE].actuelle = 0;
    VITESSE[ROUE_GAUCHE].consigne = 0;
    VITESSE[ROUE_GAUCHE].theorique = 0;
}


/******************************************************************************/
/************************** Fonctions Saturations *****************************/
/******************************************************************************/

/**
 * Ecretage des consignes maximum pour respecter la tention max des moteurs 
 */
void ecretage_consignes(void)
{
    if (COMMANDE.droit > CONSIGNE_MAX)
        COMMANDE.droit = CONSIGNE_MAX;
    else if (COMMANDE.droit < - CONSIGNE_MAX)
        COMMANDE.droit = -CONSIGNE_MAX;

    if (COMMANDE.gauche > CONSIGNE_MAX) 
        COMMANDE.gauche = CONSIGNE_MAX;
    else if (COMMANDE.gauche < -CONSIGNE_MAX)
        COMMANDE.gauche = -CONSIGNE_MAX;
}


/******************************************************************************/
/**************************** Fonctions Asserv ********************************/
/******************************************************************************/

void asserv()
{
    //calcul_position_robot();
    if (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT)
    {
        if (FLAG_ASSERV.position == ON)
            asserv_distance();
        if(FLAG_ASSERV.vitesse == ON)
            asserv_vitesse_distance();
        if(FLAG_ASSERV.orientation == ON)
            asserv_orientation();

        init_commande_moteur();

        fonction_PID(ASSERV_VITESSE_DISTANCE);

        ecretage_consignes();

        envoit_pwm(MOTEUR_DROIT, (int32_t) COMMANDE.droit);
        envoit_pwm(MOTEUR_GAUCHE, (int32_t) COMMANDE.gauche);
    }
    

}

void asserv_distance(void)
{
    static double distance_restante = 0;
    static int32_t compteur  = 0;


    //DISTANCE.consigne = sqrt()

    compteur ++;
    distance_restante = fonction_PID(ASSERV_POSITION); //DISTANCE.consigne - DISTANCE.actuelle;

    if (abs(DISTANCE.consigne - DISTANCE.actuelle) > 10 * SEUIL_DISTANCE_MINI_PAS) //arrivé à la position
    {
        if (distance_restante > 0) //distance restante > 0
        {
            VITESSE[SYS_ROBOT].consigne =  120;//VITESSE_CONSIGNE_PAS; //vmax
        }
        else if (distance_restante < 0)
        {
            VITESSE[SYS_ROBOT].consigne = - 120;//VITESSE_CONSIGNE_PAS; //Vmin
        }

        if ((VITESSE[SYS_ROBOT].theorique / (acc.deceleration.position/10) ) > ((distance_restante / VITESSE[SYS_ROBOT].theorique) ) ) //-20 = anticipation freinage
        {
            VITESSE[SYS_ROBOT].consigne = 0;
        }
    }
    else
    {
        static int compteur = 0;
        compteur ++;

        if (compteur == 100)
        {
            envoit_pwm(MOTEUR_DROIT, 0);
            envoit_pwm(MOTEUR_GAUCHE, 0);

            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
            FLAG_ASSERV.totale = OFF;

            PORTCbits.RC5 = 0;
            
            COMMANDE.droit = 0;
            COMMANDE.gauche = 0;

            envoit_pwm(MOTEUR_DROIT, 0);
            envoit_pwm(MOTEUR_GAUCHE, 0);
        }

        //FLAG_ASSERV.totale = OFF;
    }
       
}

void asserv_vitesse_distance (void)
{
    static int compteur = 0;

    if (compteur == 10)
    {
        compteur = 0;

        if (VITESSE[SYS_ROBOT].theorique < VITESSE[SYS_ROBOT].consigne)
           VITESSE[SYS_ROBOT].theorique += acc.acceleration.position;
            

        else if (VITESSE[SYS_ROBOT].theorique > VITESSE[SYS_ROBOT].consigne)
            VITESSE[SYS_ROBOT].theorique -= acc.deceleration.position;
    }

     VITESSE[ROUE_DROITE].consigne = VITESSE[SYS_ROBOT].theorique;
     VITESSE[ROUE_GAUCHE].consigne = VITESSE[SYS_ROBOT].theorique;

    compteur++;
}

void asserv_orientation (void)
{
    
}

/******************************************************************************/
/************************************ PID *************************************/
/******************************************************************************/

void reglage_PID (void)
{
    //PID VITESSE DISTANCE
    PID.VITESSE_DIS.KP = VITESSE_DIS_KP;
    PID.VITESSE_DIS.KI = VITESSE_DIS_KI;
    PID.VITESSE_DIS.KD = VITESSE_DIS_KD;

    //PID DISTANCE
    PID.DISTANCE.KP = POSITION_KP;
    PID.DISTANCE.KI = POSITION_KI;
    PID.DISTANCE.KD = POSITION_KD;

    //PID ORIENTATION
    PID.ORIENTATION.KP = ORIENTATION_KP;
    PID.ORIENTATION.KI = ORIENTATION_KI;
    PID.ORIENTATION.KD = ORIENTATION_KD;
}

double fonction_PID (unsigned char type)
{
    if (type == ASSERV_VITESSE_DISTANCE)
    {
        //VITESSE[ROUE_GAUCHE].consigne = VITESSE_CONSIGNE_PAS/6;
        //VITESSE[ROUE_DROITE].consigne = VITESSE_CONSIGNE_PAS/6;

        ERREUR_VITESSE[ROUE_DROITE].actuelle = VITESSE[ROUE_DROITE].consigne - VITESSE[ROUE_DROITE].actuelle;
        ERREUR_VITESSE[ROUE_DROITE].integralle += ERREUR_VITESSE[ROUE_DROITE].actuelle;

        ERREUR_VITESSE[ROUE_GAUCHE].actuelle = VITESSE[ROUE_GAUCHE].consigne - VITESSE[ROUE_GAUCHE].actuelle;
        ERREUR_VITESSE[ROUE_GAUCHE].integralle += ERREUR_VITESSE[ROUE_GAUCHE].actuelle;

        COMMANDE.droit  = ERREUR_VITESSE[ROUE_DROITE].actuelle * PID.VITESSE_DIS.KP + ERREUR_VITESSE[ROUE_DROITE].integralle * PID.VITESSE_DIS.KI + (ERREUR_VITESSE[ROUE_DROITE].actuelle - ERREUR_VITESSE[ROUE_DROITE].precedente ) * PID.VITESSE_DIS.KD;
        COMMANDE.gauche = ERREUR_VITESSE[ROUE_GAUCHE].actuelle * PID.VITESSE_DIS.KP + ERREUR_VITESSE[ROUE_GAUCHE].integralle * PID.VITESSE_DIS.KI + (ERREUR_VITESSE[ROUE_GAUCHE].actuelle - ERREUR_VITESSE[ROUE_GAUCHE].precedente ) * PID.VITESSE_DIS.KD;

        ERREUR_VITESSE[ROUE_DROITE].precedente = ERREUR_VITESSE[ROUE_DROITE].actuelle;
        ERREUR_VITESSE[ROUE_GAUCHE].precedente = ERREUR_VITESSE[ROUE_GAUCHE].actuelle;
    }
    else if (type == ASSERV_POSITION)
    {
        static double duty;

        ERREUR_DISTANCE.actuelle = DISTANCE.consigne - DISTANCE.actuelle;
        ERREUR_DISTANCE.integralle += ERREUR_DISTANCE.actuelle;

         duty =  ERREUR_DISTANCE.actuelle * PID.DISTANCE.KP  + ERREUR_DISTANCE.integralle * PID.DISTANCE.KI - (ERREUR_DISTANCE.actuelle - ERREUR_DISTANCE.precedente) * PID.DISTANCE.KD;

         ERREUR_DISTANCE.maximum = duty;
        ERREUR_DISTANCE.precedente = ERREUR_DISTANCE.actuelle;

        return duty;
    }

    return (double) 0;

}

/******************************************************************************/
/****************************** ODOMETRIE *************************************/
/******************************************************************************/

void calcul_position_robot (void)
{
    static int32_t delta_o, delta_d, alpha_ticks = 0;
    static double d_X = 0, d_Y = 0;

    get_valeur_codeur (CODEUR_D);
    get_valeur_codeur (CODEUR_G);

    //calcul des modifs
    delta_o = (position[CODEUR_D].ecart - position[CODEUR_G].ecart) /2;
    delta_d = (int32_t) (position[CODEUR_D].ecart + position[CODEUR_G].ecart) /2;

    //cumul des valeurs pour l'asserv
    DISTANCE.actuelle += (double) delta_d;

    VITESSE[SYS_ROBOT].actuelle = (double) delta_d;
    VITESSE[ROUE_DROITE].actuelle = (double) position[CODEUR_D].ecart;
    VITESSE[ROUE_GAUCHE].actuelle = (double) position[CODEUR_G].ecart;

    ORIENTATION.actuelle += delta_o;

    //calul de l'angle en pas
    alpha_ticks += delta_o;

    ROBOT.orientation = (double) ROBOT.orientation_init + alpha_ticks / (ENTRAXE_TICKS / 2);

    //Calcul des positions
    d_X = (double) cos (ROBOT.orientation) * delta_d;
    d_Y = (double) sin (ROBOT.orientation) * delta_d;

    ROBOT.X_point += d_X;
    ROBOT.Y_point += d_Y;

    //pas besoin de le calculer toutes les 5 ms --> perte de temps innutile
    //ROBOT.X_mm += d_X * MM_PAR_TICKS;
    //ROBOT.Y_mm += d_Y * MM_PAR_TICKS;
    //ROBOT.orientation_degre = ROBOT.orientation * 180 / Pi;
}


