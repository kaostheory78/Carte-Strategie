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
double VITESSE_MAX_ORIENTATION;
double VITESSE_MAX_POSITION;

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

    acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE.;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;



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

void saturation_vitesse_max (unsigned char type)
{
    if (type == ASSERV_POSITION)
    {
        //Pour éviter d'osciller autour d'une valeur consigne en haut du trapèze
        //on écrète à la valeur max
        if (VITESSE[SYS_ROBOT].theorique > VITESSE_MAX_POSITION)
            VITESSE[SYS_ROBOT].theorique = VITESSE_MAX_POSITION;
        else if (VITESSE[SYS_ROBOT].theorique < - VITESSE_MAX_POSITION)
            VITESSE[SYS_ROBOT].theorique = - VITESSE_MAX_POSITION;

        //Pour ne pas osciller autour d'une consigne à 0,
        //On check, lors de la génération de la courbe de freinage,
        //Que si on va passer négatif alors on écrette à 0.
        else if (VITESSE[SYS_ROBOT].consigne == 0)
            if (abs(VITESSE[SYS_ROBOT].theorique) < acc.deceleration.position )
                VITESSE[SYS_ROBOT].theorique = 0;
    }
    else if (type == ASSERV_ORIENTATION)
    {
        //Pour éviter d'osciller autour d'une valeur consigne en haut du trapèze
        //on écrète à la valeur max
        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_MAX_ORIENTATION)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = VITESSE_MAX_ORIENTATION;
        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < - VITESSE_MAX_ORIENTATION)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = - VITESSE_MAX_ORIENTATION;

        //Pour ne pas osciller autour d'une consigne à 0,
        //On check, lors de la génération de la courbe de freinage,
        //Que si on va passer négatif alors on écrette à 0.
        else if (VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0)
            if (abs(VITESSE_ORIENTATION[SYS_ROBOT].theorique) < acc.deceleration.orientation )
                VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0;

    }
}

void calcul_vitesse_position (double pourcentage_vitesse)
{
    calcul_distance_consigne_XY();

    VITESSE_MAX_POSITION = VITESSE_CONSIGNE_MAX_PAS * DISTANCE.consigne;
    VITESSE_MAX_POSITION /= DISTANCE_CONSIGNE_PAS;
    VITESSE_MAX_POSITION *= pourcentage_vitesse;
    VITESSE_MAX_POSITION /= 100;

    if (VITESSE_MAX_POSITION < 0)
        VITESSE_MAX_POSITION *= -1;
}

void calcul_acceleration_position (void)
{
    if (VITESSE_MAX_POSITION < VITESSE_CONSIGNE_MAX_PAS) //Si on est inferieur au réglage consigne
    {
        if (DISTANCE.consigne > 0)
        {
            acc.acceleration.position = VITESSE_MAX_POSITION;
            acc.acceleration.position *= ACC_POSITION_CONSIGNE;
            acc.acceleration.position /= VITESSE_CONSIGNE_MAX_PAS;
            
            acc.deceleration.position = VITESSE_MAX_POSITION;
            acc.deceleration.position *= DCC_POSITION_CONSIGNE;
            acc.deceleration.position /= VITESSE_CONSIGNE_MAX_PAS;
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.position = VITESSE_MAX_POSITION;
            acc.acceleration.position *= DCC_POSITION_CONSIGNE;
            acc.acceleration.position /= VITESSE_CONSIGNE_MAX_PAS;

            acc.deceleration.position = VITESSE_MAX_POSITION;
            acc.deceleration.position *= ACC_POSITION_CONSIGNE;
            acc.deceleration.position /= VITESSE_CONSIGNE_MAX_PAS;
        }
    }
    else //sinon on sature l'acceleration
    {
        if (DISTANCE.consigne > 0)
        {
            acc.acceleration.position = ACC_POSITION_CONSIGNE;
            acc.deceleration.position = DCC_POSITION_CONSIGNE;
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.position = DCC_POSITION_CONSIGNE;
            acc.deceleration.position = ACC_POSITION_CONSIGNE;
        }
    }
}

void calcul_vitesse_orientation (double pourcentage_vitesse)
{
    calcul_distance_consigne_XY();

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS * ORIENTATION.consigne / ORIENTATION_CONSIGNE_PAS;
    //VITESSE_MAX_ORIENTATION /= ORIENTATION_CONSIGNE_PAS;
   // VITESSE_MAX_ORIENTATION *= pourcentage_vitesse;
    //VITESSE_MAX_ORIENTATION /= 100;
    VITESSE_MAX_ORIENTATION *= pourcentage_vitesse / 100;

    if (VITESSE_MAX_ORIENTATION < 0)
        VITESSE_MAX_POSITION *= -1;
}

void calcul_acceleration_orientation (void)
{
    if (VITESSE_MAX_ORIENTATION < VITESSE_ANGLE_PAS) //Si on est inferieur au réglage consigne
    {
        if (ORIENTATION.consigne > 0)
        {
            acc.acceleration.orientation = VITESSE_MAX_ORIENTATION;
            acc.acceleration.orientation *= ACC_ORIENTATION_CONSIGNE;
            acc.acceleration.orientation /= VITESSE_ANGLE_PAS;

            acc.deceleration.orientation = VITESSE_MAX_ORIENTATION;
            acc.deceleration.orientation *= DCC_ORIENTATION_CONSIGNE;
            acc.deceleration.orientation /= VITESSE_ANGLE_PAS;
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.orientation = VITESSE_MAX_ORIENTATION;
            acc.acceleration.orientation *= DCC_ORIENTATION_CONSIGNE;
            acc.acceleration.orientation /= VITESSE_ANGLE_PAS;

            acc.deceleration.orientation = VITESSE_MAX_ORIENTATION;
            acc.deceleration.orientation *= ACC_ORIENTATION_CONSIGNE;
            acc.deceleration.orientation /= VITESSE_ANGLE_PAS;
        }
    }
    else //sinon on sature l'acceleration
    {
        if ( ORIENTATION.consigne > 0)
        {
            acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE;
            acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.orientation = DCC_ORIENTATION_CONSIGNE;
            acc.deceleration.orientation = ACC_ORIENTATION_CONSIGNE;
        }
    }
}

void calcul_distance_consigne_XY (void)
{
    if (TYPE_CONSIGNE == XY)
    {
        static double delta_x, delta_y;
        delta_x = (double) (X.consigne - X.actuelle);
        delta_y = (double) (Y.consigne - Y.actuelle);

        ORIENTATION.consigne = atan2(delta_y, delta_x ) * (ENTRAXE_TICKS /2);


        DISTANCE.consigne = sqrt(delta_x * delta_x + delta_y * delta_y);
        DISTANCE.actuelle = 0;


        if ((ORIENTATION.consigne - ORIENTATION.actuelle) > Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne -= Pi * ENTRAXE_TICKS;
        else if (ORIENTATION.consigne - ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne += Pi * ENTRAXE_TICKS;
    }
}


/******************************************************************************/
/**************************** Fonctions Asserv ********************************/
/******************************************************************************/

//Fonction principale de l'asserv, qui permet d'activer les différents asserv
//D'envoyer la commande aux moteurs, et de sortir des fonctions de déplacements
void asserv()
{
    VITESSE[ROUE_DROITE].consigne = 0;
    VITESSE[ROUE_GAUCHE].consigne = 0;

 
    //Fonction de sortie de l'asserv, vérifie que le robot est bien a sa position.
    if (FLAG_ASSERV.etat_angle == ANGLE_ATTEINT && FLAG_ASSERV.etat_distance == DISTANCE_ATTEINTE)
    {
        FLAG_ASSERV.immobilite ++;
        PORTCbits.RC5 = 0;
     
        if (FLAG_ASSERV.immobilite >= SEUIL_IMMOBILITE)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
        else if (FLAG_ASSERV.vitesse_fin_nulle == OFF)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
    }

    //Fonction d'appel de l'asserv
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
        

        //Réinitialisation des commandes moteurs à 0
        init_commande_moteur();

        //Calcul des consignes moteurs gauche et droit, asserv indispendable
        fonction_PID(ASSERV_VITESSE_DISTANCE);
        ecretage_consignes();

        //envoit sur les moteurs
        envoit_pwm(MOTEUR_DROIT, (int32_t) COMMANDE.droit);
        envoit_pwm(MOTEUR_GAUCHE, (int32_t) COMMANDE.gauche);
    }
    else
    {
        //Si aucun asserv, on bloque les moteurs à 0;
        envoit_pwm(MOTEUR_DROIT, 0);
        envoit_pwm(MOTEUR_GAUCHE, 0);
    }
    

}


// Fonction qui gère l'asserv de Distance, donne les consignes pour générer les rampes
void asserv_distance(void)
{
    static double distance_restante = 0;
    static int compteur = 0;
    
    calcul_distance_consigne_XY();
    distance_restante = fonction_PID(ASSERV_POSITION); //DISTANCE.consigne - DISTANCE.actuelle;

    if (abs(distance_restante) >  10)
    {
        compteur = 0;
        FLAG_ASSERV.etat_distance = EN_COURS;

        if (distance_restante > 0) 
        {
            VITESSE[SYS_ROBOT].consigne =  VITESSE_MAX_POSITION; //vmax
        }
        else if (distance_restante < 0)
        {
            VITESSE[SYS_ROBOT].consigne = - VITESSE_MAX_POSITION; //Vmin //-120
        }

        // Si le robot doit freiner
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

//Fonction qui génère les rampes de vitesse pour l'asserv en Distance
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

        saturation_vitesse_max(ASSERV_POSITION);
    }
    
     VITESSE[ROUE_DROITE].consigne += VITESSE[SYS_ROBOT].theorique;
     VITESSE[ROUE_GAUCHE].consigne += VITESSE[SYS_ROBOT].theorique;

    compteur++;
}


//Fonction qui permet de créer les consignes pour générer les rampes de Vitesse
void asserv_orientation (void)
{
    static double angle_restant = 0;
    static int32_t compteur  = 0;

    angle_restant = fonction_PID(ASSERV_ORIENTATION); 

    if (abs(angle_restant) >  Pi / 180 * ENTRAXE_TICKS/2)//0.5 * SEUIL_ORIENTATION_MINI_PAS)
    {
        compteur = 0;
        FLAG_ASSERV.etat_angle = EN_COURS;

        if (angle_restant > 0) //distance restante > 0
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne =  VITESSE_MAX_ORIENTATION;
        }
        else if (angle_restant < 0)
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne = - VITESSE_MAX_ORIENTATION;
        }

        //Génération de la courbe de freinage
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


//Fonction qui génère les rampes de Vitesse pour la rotation du robot
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

        saturation_vitesse_max(ASSERV_ORIENTATION);
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

    //Angle compris entre - 180 et + 180
    if (ORIENTATION.actuelle > Pi * ENTRAXE_TICKS/2)
        ORIENTATION.actuelle -= Pi * ENTRAXE_TICKS ;
    else if(ORIENTATION.actuelle < - Pi * (ENTRAXE_TICKS/2))
        ORIENTATION.actuelle += Pi * ENTRAXE_TICKS;

    //calul de l'angle en pas
    alpha_ticks += delta_o;

    //Orientation en radian
    ROBOT.orientation = (double) ROBOT.orientation_init + alpha_ticks / (ENTRAXE_TICKS / 2);

    //Calcul des positions
    d_X = (double) cos (ROBOT.orientation) * delta_d;
    d_Y = (double) sin (ROBOT.orientation) * delta_d;

    X.actuelle +=d_X;
    Y.actuelle += d_Y;

    //pas besoin de le calculer toutes les 5 ms --> perte de temps innutile
    //ROBOT.X_mm += d_X * MM_PAR_TICKS;
    //ROBOT.Y_mm += d_Y * MM_PAR_TICKS;
    //ROBOT.orientation_degre = ROBOT.orientation * 180 / Pi;
}


