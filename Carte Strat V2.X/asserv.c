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
double KP_hybride;

_systeme_asserv X, Y;
_systeme_asserv DISTANCE;
_systeme_asserv ORIENTATION;
_systeme_asserv VITESSE_ORIENTATION[3];
_systeme_asserv VITESSE[3];

_acc acc;

_erreur ERREUR_DISTANCE;
_erreur ERREUR_VITESSE[2];
_erreur ERREUR_ORIENTATION;

_commande_moteur COMMANDE;

_flag_asserv FLAG_ASSERV;

char TYPE_CONSIGNE;



/******************************************************************************/
/***************************** Fonctions Inits ********************************/
/******************************************************************************/

void init_flag()
{
    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.vitesse = OFF;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;

    acc.acceleration.position = 4; //4 //6
    acc.deceleration.position = 4; //3 //6

    acc.acceleration.orientation = 4.;
    acc.deceleration.orientation = 4.;


    //TYPE_CONSIGNE = XY;

    //DISTANCE.consigne = 800 *  TICKS_PAR_MM;
    //ORIENTATION.consigne = 0 * (ENTRAXE_TICKS / 2) * Pi / 180;

    //X.consigne = 300 * TICKS_PAR_MM;
    //Y.consigne = 300 * TICKS_PAR_MM;

    


    reinit_asserv();
    reglage_PID();

    PORTCbits.RC5 = 1;
    FLAG_ASSERV.totale = ON;
}

void init_position_robot (float x0, float y0, uint32_t teta0)
{
    ROBOT.X_mm = x0;
    ROBOT.Y_mm = y0;

    X.actuelle = ROBOT.X_mm * TICKS_PAR_MM;
    Y.actuelle = ROBOT.Y_mm * TICKS_PAR_MM;
    
    ROBOT.orientation_init = teta0 * Pi / 180 * ENTRAXE_TICKS/2;
    ORIENTATION.actuelle = ROBOT.orientation_init;
}

void init_commande_moteur(void)
{
    COMMANDE.droit = 0;
    COMMANDE.gauche = 0;
}

void reinit_asserv(void)
{
    if (FLAG_ASSERV.vitesse_fin_nulle == OFF)
    {
        FLAG_ASSERV.immobilite = 0;

    DISTANCE.actuelle = 0;
    DISTANCE.theorique = 0;
    DISTANCE.consigne = 0;
    //ORIENTATION.actuelle = 0;
    ORIENTATION.theorique = 0;
    ORIENTATION.consigne = 0;

    ERREUR_DISTANCE.actuelle = 0;
    ERREUR_DISTANCE.integralle = 0;
    ERREUR_DISTANCE.precedente = 0;

    ERREUR_ORIENTATION.actuelle = 0;
    ERREUR_ORIENTATION.precedente = 0;
    ERREUR_ORIENTATION.integralle = 0;

    ERREUR_VITESSE[ROUE_DROITE].actuelle = 0;
    ERREUR_VITESSE[ROUE_DROITE].integralle = 0;
    ERREUR_VITESSE[ROUE_DROITE].precedente = 0;

    ERREUR_VITESSE[ROUE_GAUCHE].actuelle = 0;
    ERREUR_VITESSE[ROUE_GAUCHE].integralle = 0;
    ERREUR_VITESSE[ROUE_GAUCHE].precedente = 0;


    VITESSE[ROUE_DROITE].actuelle = 0;
    VITESSE[ROUE_DROITE].consigne = 0;
    VITESSE[ROUE_DROITE].theorique = 0;

    VITESSE[ROUE_GAUCHE].actuelle = 0;
    VITESSE[ROUE_GAUCHE].consigne = 0;
    VITESSE[ROUE_GAUCHE].theorique = 0;

    VITESSE_ORIENTATION[ROUE_DROITE].actuelle = 0;
    VITESSE_ORIENTATION[ROUE_DROITE].consigne = 0;
    VITESSE_ORIENTATION[ROUE_DROITE].theorique = 0;

    VITESSE_ORIENTATION[ROUE_GAUCHE].actuelle = 0;
    VITESSE_ORIENTATION[ROUE_GAUCHE].consigne = 0;
    VITESSE_ORIENTATION[ROUE_GAUCHE].theorique = 0;
    }
    PORTCbits.RC5 = 1;

    
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


    VITESSE[ROUE_DROITE].consigne = 0;
    VITESSE[ROUE_GAUCHE].consigne = 0;

    if (FLAG_ASSERV.etat_angle == ANGLE_ATTEINT && FLAG_ASSERV.etat_distance == DISTANCE_ATTEINTE)
    {
        FLAG_ASSERV.immobilite ++;
        PORTCbits.RC5 = 0;
     
        if (FLAG_ASSERV.immobilite >= SEUIL_IMMOBILITE)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
        else if (FLAG_ASSERV.vitesse_fin_nulle == OFF)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
    }

    /*if (FLAG_ASSERV.immobilite >= SEUIL_IMMOBILITE)
    {
        FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
        PORTCbits.RC5 = 0;
    }*/
        


    if (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT && (FLAG_ASSERV.etat_angle == EN_COURS || FLAG_ASSERV.etat_distance == EN_COURS))
    {
        if (FLAG_ASSERV.position == ON)
        {
            asserv_distance();
        }
        if(FLAG_ASSERV.orientation == ON)
        {
            asserv_orientation();
        }
        if(FLAG_ASSERV.vitesse == ON)
        {
            if (FLAG_ASSERV.position == ON)
                asserv_vitesse_distance();
            if (FLAG_ASSERV.orientation == ON)
                asserv_vitesse_orientation();
        }
        

        init_commande_moteur();

        fonction_PID(ASSERV_VITESSE_DISTANCE);

        ecretage_consignes();

        envoit_pwm(MOTEUR_DROIT, (int32_t) COMMANDE.droit);
        envoit_pwm(MOTEUR_GAUCHE, (int32_t) COMMANDE.gauche);
    }
    else
    {
        envoit_pwm(MOTEUR_DROIT, 0);
        envoit_pwm(MOTEUR_GAUCHE, 0);
    }
    

}

void asserv_distance(void)
{
    static double distance_restante = 0;
    static int compteur = 0;
    
    if (TYPE_CONSIGNE == XY)
    {
        static double delta_x, delta_y;
        delta_x = (double) (X.consigne - X.actuelle);
        delta_y = (double) (Y.consigne - Y.actuelle);

        ORIENTATION.consigne = atan2(delta_y, delta_x ) * (ENTRAXE_TICKS /2);


        DISTANCE.consigne = sqrt(delta_x * delta_x + delta_y * delta_y);
        DISTANCE.actuelle = 0;

       // if (abs(DISTANCE.consigne) < 50 * SEUIL_DISTANCE_MINI_PAS)
       //     FLAG_ASSERV.orientation = OFF;
       // else
       //     FLAG_ASSERV.orientation = ON;

        if ((ORIENTATION.consigne - ORIENTATION.actuelle) > Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne -= Pi * ENTRAXE_TICKS;
        else if (ORIENTATION.consigne - ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne += Pi * ENTRAXE_TICKS;

       // if (abs(ORIENTATION.consigne - ORIENTATION.actuelle) > Pi * ENTRAXE_TICKS/3)
       //     DISTANCE.consigne *= -1;
    }

    distance_restante = fonction_PID(ASSERV_POSITION); //DISTANCE.consigne - DISTANCE.actuelle;


   // if (abs(DISTANCE.consigne - DISTANCE.actuelle) > 10 * SEUIL_DISTANCE_MINI_PAS) //arrivé à la position
    if (abs(distance_restante) > 10 * SEUIL_DISTANCE_MINI_PAS)
    {
        compteur = 0;
        FLAG_ASSERV.etat_distance = EN_COURS;

        if (distance_restante > 0) //distance restante > 0
        {
            VITESSE[SYS_ROBOT].consigne =  VITESSE_CONSIGNE_PAS; //vmax
        }
        else if (distance_restante < 0)
        {
            VITESSE[SYS_ROBOT].consigne = - VITESSE_CONSIGNE_PAS; //Vmin //-120
        }

        //if (FLAG_ASSERV.vitesse_fin_nulle == ON)
            if ((abs(VITESSE[SYS_ROBOT].theorique / (acc.deceleration.position/10) )) > abs(((distance_restante / VITESSE[SYS_ROBOT].theorique) ) )) //-20 = anticipation freinage
            {
                VITESSE[SYS_ROBOT].consigne = 0;
            }
    }
    else if (FLAG_ASSERV.vitesse_fin_nulle == ON)
    {
        VITESSE[SYS_ROBOT].theorique = 0;
        compteur ++;
        if (compteur == 50)
            FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;  
    }
    else
    {
        FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
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

     VITESSE[ROUE_DROITE].consigne += VITESSE[SYS_ROBOT].theorique;
     VITESSE[ROUE_GAUCHE].consigne += VITESSE[SYS_ROBOT].theorique;

    compteur++;
}

void asserv_orientation (void)
{
    static double angle_restant = 0;
    static int32_t compteur  = 0;


    angle_restant = fonction_PID(ASSERV_ORIENTATION); //DISTANCE.consigne - DISTANCE.actuelle;

    //if (abs(ORIENTATION.consigne - ORIENTATION.actuelle) > 0.5 * SEUIL_ORIENTATION_MINI_PAS) //arrivé à la position
    if (abs(angle_restant) >  Pi / 180 * ENTRAXE_TICKS/2)//0.5 * SEUIL_ORIENTATION_MINI_PAS)
    {
        compteur = 0;
        FLAG_ASSERV.etat_angle = EN_COURS;

        if (angle_restant > 0) //distance restante > 0
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne =  VITESSE_ANGLE_PAS; //vmax
        }
        else if (angle_restant < 0)
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne = - VITESSE_ANGLE_PAS; //vmax
        }

        //if (FLAG_ASSERV.vitesse_fin_nulle == ON)
            if (abs((VITESSE_ORIENTATION[SYS_ROBOT].theorique / (acc.deceleration.orientation/10) ) > abs((angle_restant / VITESSE_ORIENTATION[SYS_ROBOT].theorique)) ) )
            {
                VITESSE_ORIENTATION[SYS_ROBOT].consigne = 0;
            }
    }
    else //if (FLAG_ASSERV.vitesse_fin_nulle == ON)
    {
            compteur ++;
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0;
            if (compteur == 50)
                FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    }
}

void asserv_vitesse_orientation (void)
{
     static int compteur = 0;

    if (compteur == 10)
    {
        compteur = 0;

        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < VITESSE_ORIENTATION[SYS_ROBOT].consigne)
           VITESSE_ORIENTATION[SYS_ROBOT].theorique += acc.acceleration.orientation;


        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_ORIENTATION[SYS_ROBOT].consigne)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique -= acc.deceleration.orientation;
    }

     VITESSE[ROUE_DROITE].consigne += VITESSE_ORIENTATION[SYS_ROBOT].theorique;
     VITESSE[ROUE_GAUCHE].consigne -= VITESSE_ORIENTATION[SYS_ROBOT].theorique;

    compteur++;
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


    KP_hybride = KP_HYBRIDE;
}

double fonction_PID (unsigned char type)
{
    if (type == ASSERV_VITESSE_DISTANCE)
    {
        /*if (FLAG_ASSERV.etat_distance == DISTANCE_ATTEINTE && FLAG_ASSERV.etat_angle == ANGLE_ATTEINT)
        {
            if (ERREUR_VITESSE[ROUE_DROITE].actuelle == ERREUR_VITESSE[ROUE_DROITE].precedente && ERREUR_VITESSE[ROUE_GAUCHE].actuelle == ERREUR_VITESSE[ROUE_GAUCHE].actuelle && FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT)
            {
                FLAG_ASSERV.immobilite++;
            }
            else
            {
                FLAG_ASSERV.immobilite = 0;
            }
        }*/

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
         //duty *= KP_hybride * (1 - ((abs(ORIENTATION.consigne - ORIENTATION.actuelle))/(Pi * ENTRAXE_TICKS/2)));

        ERREUR_DISTANCE.precedente = ERREUR_DISTANCE.actuelle;

        return duty;
    }
    else if ( type == ASSERV_ORIENTATION)
    {
        static double duty;

        if ((ORIENTATION.consigne - ORIENTATION.actuelle) > Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne -= Pi * ENTRAXE_TICKS;
        else if (ORIENTATION.consigne - ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne += Pi * ENTRAXE_TICKS;

        ERREUR_ORIENTATION.actuelle = ORIENTATION.consigne - ORIENTATION.actuelle;

        if (ERREUR_ORIENTATION.actuelle > (Pi * ENTRAXE_TICKS/2))
            ERREUR_ORIENTATION.actuelle -=  Pi * ENTRAXE_TICKS;
        else if (ERREUR_ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2)
            ERREUR_ORIENTATION.actuelle +=  Pi * ENTRAXE_TICKS;
 
        ERREUR_ORIENTATION.integralle += ERREUR_ORIENTATION.actuelle;

         duty =  ERREUR_ORIENTATION.actuelle * PID.ORIENTATION.KP  + ERREUR_ORIENTATION.integralle * PID.ORIENTATION.KI - (ERREUR_ORIENTATION.actuelle - ERREUR_ORIENTATION.precedente) * PID.ORIENTATION.KD;
        ERREUR_ORIENTATION.precedente = ERREUR_ORIENTATION.actuelle;

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

    VITESSE_ORIENTATION[SYS_ROBOT].actuelle = (double) delta_o;

    ORIENTATION.actuelle += delta_o;

 //   if (ORIENTATION.actuelle > Pi * ENTRAXE_TICKS/2 || ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2)
 //       ORIENTATION.actuelle *= -1;


    if (ORIENTATION.actuelle >  2* Pi * ENTRAXE_TICKS/2)
        ORIENTATION.actuelle -= 2 * (Pi* (ENTRAXE_TICKS /2));
    else if(ORIENTATION.actuelle <- 2 * Pi * (ENTRAXE_TICKS/2))
        ORIENTATION.actuelle += 2 * Pi * (ENTRAXE_TICKS/2);

    //calul de l'angle en pas
    alpha_ticks += delta_o;

    ROBOT.orientation = (double) ROBOT.orientation_init + alpha_ticks / (ENTRAXE_TICKS / 2);

    //Calcul des positions
    d_X = (double) cos (ROBOT.orientation) * delta_d;
    d_Y = (double) sin (ROBOT.orientation) * delta_d;

    X.actuelle +=d_X;
    Y.actuelle += d_Y;

    ROBOT.X_point += d_X;
    ROBOT.Y_point += d_Y;

    //pas besoin de le calculer toutes les 5 ms --> perte de temps innutile
    //ROBOT.X_mm += d_X * MM_PAR_TICKS;
    //ROBOT.Y_mm += d_Y * MM_PAR_TICKS;
    //ROBOT.orientation_degre = ROBOT.orientation * 180 / Pi;
}


