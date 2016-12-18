/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: asserv.c
 * Auteur  	: Quentin
 * Revision	: 1.2
 * Date		: 08/11/2014
 *******************************************************************************
 *
 *
 ******************************************************************************/

#include "system.h"


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

volatile __attribute__((near)) _robot ROBOT;

volatile __attribute__((near)) _PID PID;
volatile __attribute__((near)) double KP_hybride;
volatile __attribute__((near)) _vitesse VITESSE_MAX;

volatile __attribute__((near)) _systeme_asserv X, Y;
volatile __attribute__((near)) _systeme_asserv BRAKE[2];
volatile __attribute__((near)) _systeme_asserv DISTANCE;
volatile __attribute__((near)) _systeme_asserv ORIENTATION;
volatile __attribute__((near)) _systeme_asserv VITESSE_ORIENTATION[3];
volatile __attribute__((near)) _systeme_asserv VITESSE[3];

volatile __attribute__((near)) _acc acc;

volatile __attribute__((near)) _erreur ERREUR_DISTANCE;
volatile __attribute__((near)) _erreur ERREUR_VITESSE[2];
volatile __attribute__((near)) _erreur ERREUR_ORIENTATION;
volatile __attribute__((near)) _erreur ERREUR_BRAKE[2];

volatile __attribute__((near)) _commande_moteur COMMANDE;

volatile __attribute__((near)) _flag_asserv FLAG_ASSERV;




/******************************************************************************/
/***************************** Fonctions Inits ********************************/
/******************************************************************************/

/**
 * Fonction qui initialise les flag de l'asserv pour d�marer l'asserv
 */
void init_flag_asserv()
{
    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.vitesse = OFF;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
    FLAG_ASSERV.brake = OFF;
    FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    
    FLAG_ASSERV.vitesse_fin_nulle = ON;
    FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;

    reinit_asserv();
    reglage_PID();

    PORTCbits.RC5 = 1;
    FLAG_ASSERV.totale = ON;
}

void init_position_robot (double x0, double y0, uint32_t teta0)
{
    init_X (x0);
    init_Y (y0);
    init_orientation (teta0);
}

void init_X (double x)
{
    ROBOT.X_mm = x;
    X.actuelle = ROBOT.X_mm * TICKS_PAR_MM;
}

void init_Y (double y)
{
    ROBOT.Y_mm = inversion_couleur(y);
    Y.actuelle = ROBOT.Y_mm * TICKS_PAR_MM;
}

void init_orientation (double teta)
{
    teta = inversion_couleur(teta);
    ORIENTATION.actuelle =  teta * Pi / 180. * ENTRAXE_TICKS/2;
}

void init_commande_moteur(void)
{
    COMMANDE.droit = 0;
    COMMANDE.gauche = 0;
}

void reinit_asserv(void)
{

    FLAG_ASSERV.brake = OFF;
    FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    FLAG_ASSERV.vitesse_fin_nulle = ON;
    FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
    FLAG_ASSERV.phase_deceleration_distance = PHASE_NORMAL;

    FLAG_ASSERV.immobilite = 0ULL;

    DISTANCE.actuelle = 0.;
    DISTANCE.theorique = 0.;
    DISTANCE.consigne = 0.;

    ORIENTATION.theorique = 0.;
    ORIENTATION.consigne = 0.;

    ERREUR_DISTANCE.actuelle = 0.;
    ERREUR_DISTANCE.integralle = 0.;
    ERREUR_DISTANCE.precedente = 0.;

    ERREUR_ORIENTATION.actuelle = 0.;
    ERREUR_ORIENTATION.precedente = 0.;
    ERREUR_ORIENTATION.integralle = 0.;

    ERREUR_VITESSE[ROUE_DROITE].actuelle = 0.;
    ERREUR_VITESSE[ROUE_DROITE].integralle = 0.;
    ERREUR_VITESSE[ROUE_DROITE].precedente = 0.;

    ERREUR_VITESSE[ROUE_GAUCHE].actuelle = 0.;
    ERREUR_VITESSE[ROUE_GAUCHE].integralle = 0.;
    ERREUR_VITESSE[ROUE_GAUCHE].precedente = 0.;

    VITESSE[ROUE_DROITE].actuelle = 0.;
    VITESSE[ROUE_DROITE].consigne = 0.;
    VITESSE[ROUE_DROITE].theorique = 0.;

    VITESSE[ROUE_GAUCHE].actuelle = 0.;
    VITESSE[ROUE_GAUCHE].consigne = 0.;
    VITESSE[ROUE_GAUCHE].theorique = 0.;

    VITESSE[SYS_ROBOT].theorique = 0.;
    VITESSE[SYS_ROBOT].consigne = 0.;
    VITESSE[SYS_ROBOT].actuelle = 0.;

    VITESSE_ORIENTATION[ROUE_DROITE].actuelle = 0.;
    VITESSE_ORIENTATION[ROUE_DROITE].consigne = 0.;
    VITESSE_ORIENTATION[ROUE_DROITE].theorique = 0.;

    VITESSE_ORIENTATION[ROUE_GAUCHE].actuelle = 0.;
    VITESSE_ORIENTATION[ROUE_GAUCHE].consigne = 0.;
    VITESSE_ORIENTATION[ROUE_GAUCHE].theorique = 0.;

    VITESSE_ORIENTATION[SYS_ROBOT].actuelle = 0.;
    VITESSE_ORIENTATION[SYS_ROBOT].consigne = 0.;
    VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;

    PID.VITESSE_DIS.seuil_immobilite = SEUIL_IMMOBILITE;
    PID.VITESSE_DIS.max_I = MAX_ERREUR_INTEGRALLE_V;
    PID.BRAKE.max_I = MAX_E_INTEGRALLE_BRAKE;
    
    acc.acceleration.orientation.max = ACC_ORIENTATION_CONSIGNE;
    acc.acceleration.orientation.min = ACC_ORIENTATION_MIN;
    acc.acceleration.position.max = ACC_POSITION_CONSIGNE;
    acc.acceleration.position.min = ACC_POSITION_MIN;
    acc.deceleration.orientation.max = DCC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation.min = DCC_ORIENTATION_MIN;
    acc.deceleration.position.max = DCC_POSITION_CONSIGNE;
    acc.deceleration.position.min = DCC_POSITION_MIN;
    
    COMMANDE.max = CONSIGNE_MAX;

    PORTCbits.RC5 = 1;
}

/******************************************************************************/
/***************************  Fonctions Getters  ******************************/
/******************************************************************************/

double get_X (void)
{
    return ROBOT.X_mm;
}

double get_Y (void)
{
    return inversion_couleur (ROBOT.Y_mm);
}

double get_orientation (void)
{
    return inversion_couleur(ROBOT.orientation_degre);
}



/******************************************************************************/
/************************** Fonctions Saturations *****************************/
/******************************************************************************/

/**
 * Ecretage des consignes maximum pour respecter la tention max des moteurs 
 */
void ecretage_consignes(void)
{
    if (COMMANDE.droit > COMMANDE.max)
        COMMANDE.droit = COMMANDE.max;
    else if (COMMANDE.droit < - COMMANDE.max)
        COMMANDE.droit = -COMMANDE.max;

    if (COMMANDE.gauche > COMMANDE.max) 
        COMMANDE.gauche = COMMANDE.max;
    else if (COMMANDE.gauche < -COMMANDE.max)
        COMMANDE.gauche = -COMMANDE.max;
}

void saturation_vitesse_max (_enum_type_PID type)
{
    if (type == ASSERV_POSITION)
    {
        //Pour �viter d'osciller autour d'une valeur consigne en haut du trap�ze
        //on �cr�te � la valeur max
        if (VITESSE[SYS_ROBOT].theorique > VITESSE_MAX.position)
            VITESSE[SYS_ROBOT].theorique = VITESSE_MAX.position;
        else if (VITESSE[SYS_ROBOT].theorique < - VITESSE_MAX.position)
            VITESSE[SYS_ROBOT].theorique = - VITESSE_MAX.position;

        //Pour ne pas osciller autour d'une consigne � 0,
        //On check, lors de la g�n�ration de la courbe de freinage,
        //Que si on va passer n�gatif alors on �crette � 0.
        else if (VITESSE[SYS_ROBOT].consigne == 0.)
            if (abs(VITESSE[SYS_ROBOT].theorique) < acc.deceleration.position.consigne )
                VITESSE[SYS_ROBOT].theorique = 0.;
    }
    else if (type == ASSERV_ORIENTATION)
    {
        //Pour �viter d'osciller autour d'une valeur consigne en haut du trap�ze
        //on �cr�te � la valeur max
        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_MAX.orientation)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = VITESSE_MAX.orientation;
        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < - VITESSE_MAX.orientation)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = - VITESSE_MAX.orientation;

        //Pour ne pas osciller autour d'une consigne � 0,
        //On check, lors de la g�n�ration de la courbe de freinage,
        //Que si on va passer n�gatif alors on �crette � 0.
        else if (VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0.)
            if (abs(VITESSE_ORIENTATION[SYS_ROBOT].theorique) < acc.deceleration.orientation.consigne )
                VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;

    }
}

void saturation_erreur_integralle_vitesse (void)
{
    if (ERREUR_VITESSE[ROUE_DROITE].integralle > PID.VITESSE_DIS.max_I)
        ERREUR_VITESSE[ROUE_DROITE].integralle = PID.VITESSE_DIS.max_I;
    else if (ERREUR_VITESSE[ROUE_DROITE].integralle < - PID.VITESSE_DIS.max_I)
        ERREUR_VITESSE[ROUE_DROITE].integralle = - PID.VITESSE_DIS.max_I;

    if (ERREUR_VITESSE[ROUE_GAUCHE].integralle > PID.VITESSE_DIS.max_I)
        ERREUR_VITESSE[ROUE_GAUCHE].integralle = PID.VITESSE_DIS.max_I;
    else if (ERREUR_VITESSE[ROUE_GAUCHE].integralle < - PID.VITESSE_DIS.max_I)
        ERREUR_VITESSE[ROUE_GAUCHE].integralle = - PID.VITESSE_DIS.max_I;
}

void detection_blocage (void)
{
    double pourcentage_vitesse_mini = 0.2;
    if (VITESSE[ROUE_DROITE].actuelle < pourcentage_vitesse_mini * (VITESSE[ROUE_DROITE].consigne * FLAG_ASSERV.sens_deplacement))
    {
       if (ERREUR_VITESSE[ROUE_DROITE].integralle == PID.VITESSE_DIS.max_I || ERREUR_VITESSE[ROUE_DROITE].integralle == - PID.VITESSE_DIS.max_I )
        {
            FLAG_ASSERV.immobilite++;
            FLAG_ASSERV.erreur = BLOCAGE;
        }
    }
    else if (VITESSE[ROUE_GAUCHE].actuelle < pourcentage_vitesse_mini * (VITESSE[ROUE_GAUCHE].consigne * FLAG_ASSERV.sens_deplacement))
    {
        if (ERREUR_VITESSE[ROUE_GAUCHE].integralle == PID.VITESSE_DIS.max_I || ERREUR_VITESSE[ROUE_GAUCHE].integralle == - PID.VITESSE_DIS.max_I)
        {
            FLAG_ASSERV.immobilite++;
            FLAG_ASSERV.erreur = BLOCAGE;
        }
    }
    else
    {
        FLAG_ASSERV.immobilite = 0ULL;
        FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    }
}

void calcul_vitesse_position (double pourcentage_vitesse)
{
    calcul_distance_consigne_XY();
    double temp;

    // diminue la v max calcul�
    // on ne consid�re pas la courbe comme �tant lin�aire
    // mais je ne me souviens plus exactment
    if (DISTANCE.consigne > DISTANCE_CONSIGNE_PAS)
    {
        temp =  DISTANCE.consigne - DISTANCE_CONSIGNE_PAS;
        temp /= 2.;
        temp += DISTANCE_CONSIGNE_PAS;
    }
    else
        temp = DISTANCE.consigne;

    VITESSE_MAX.position = VITESSE_CONSIGNE_MAX_PAS * temp;
    VITESSE_MAX.position /= DISTANCE_CONSIGNE_PAS;

    // On garde la valeur absolue
    if (VITESSE_MAX.position < 0.)
        VITESSE_MAX.position *= -1.;

    // On �cr�te par le haut avant la mise � l'�chelle pour que cette derni�re 
    // ai une r�elle signifiacation (la fonction de calcul de vitesse, dans le cas de grande
    // vitesse, obtiens des vitesse bien superieur au max)
    if (VITESSE_MAX.position > VITESSE_MAX_TENSION)
        VITESSE_MAX.position = VITESSE_MAX_TENSION;
    
    // Mise � l'�chelle de la vitesse par rapport � la requ�te utilisateur 
    // (pourcentage de la vitesse max calcul�e)
    VITESSE_MAX.position *= pourcentage_vitesse;
    VITESSE_MAX.position /= 100.;
    
    
    // On �cr�te la valeur min apr�s la mise � l'�chelle pour emp�cher que la mise � l'�chelle
    // induise des vitesse inf�rieur au minimum
    if (VITESSE_MAX.position < VITESSE_DISTANCE_MIN_PAS)
        VITESSE_MAX.position = VITESSE_DISTANCE_MIN_PAS;
}

void calcul_acceleration_position (void)
{
    if (VITESSE_MAX.position < VITESSE_CONSIGNE_MAX_PAS) //Si on est inferieur au r�glage consigne
    {
        if (DISTANCE.consigne > 0.)
        {
            acc.acceleration.position.consigne = VITESSE_MAX.position;
            acc.acceleration.position.consigne *= acc.acceleration.position.max; 
            acc.acceleration.position.consigne /= VITESSE_CONSIGNE_MAX_PAS;
            
            acc.deceleration.position.consigne = VITESSE_MAX.position;
            acc.deceleration.position.consigne *= acc.deceleration.position.max; 
            acc.deceleration.position.consigne /= VITESSE_CONSIGNE_MAX_PAS;
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.position.consigne = VITESSE_MAX.position;
            acc.acceleration.position.consigne *= acc.deceleration.position.max; 
            acc.acceleration.position.consigne /= VITESSE_CONSIGNE_MAX_PAS;

            acc.deceleration.position.consigne = VITESSE_MAX.position;
            acc.deceleration.position.consigne *= acc.acceleration.position.max; 
            acc.deceleration.position.consigne /= VITESSE_CONSIGNE_MAX_PAS;
        }
    }
    else //sinon on sature l'acceleration
    {
        if (DISTANCE.consigne > 0.)
        {
            acc.acceleration.position.consigne = acc.acceleration.position.max; 
            acc.deceleration.position.consigne = acc.deceleration.position.max; 
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.position.consigne = acc.deceleration.position.max; 
            acc.deceleration.position.consigne = acc.acceleration.position.max; 
        }
    }

    if (acc.acceleration.position.consigne < acc.acceleration.position.min) 
        acc.acceleration.position.consigne = acc.acceleration.position.min; 
    if (acc.deceleration.position.consigne < acc.deceleration.position.min) // /!\ REMPLACEMENT DU SIGNE > PAR <
        acc.deceleration.position.consigne = acc.deceleration.position.min; 
}

void calcul_vitesse_orientation (double pourcentage_vitesse)
{
    calcul_distance_consigne_XY();

    fonction_PID(ASSERV_ORIENTATION);

    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS;
    VITESSE_MAX.orientation *= ERREUR_ORIENTATION.actuelle; // orientation restante � parcourir
    VITESSE_MAX.orientation /= ORIENTATION_CONSIGNE_PAS;
    VITESSE_MAX.orientation *= pourcentage_vitesse;
    VITESSE_MAX.orientation /= 100.;

    if (VITESSE_MAX.orientation < 0.)
        VITESSE_MAX.orientation *= - 1.;

    if (VITESSE_MAX.orientation > VITESSE_MAX_TENSION)
        VITESSE_MAX.orientation = VITESSE_MAX_TENSION;
    else if (VITESSE_MAX.orientation < VITESSE_ANGLE_MIN_PAS)
        VITESSE_MAX.orientation = VITESSE_ANGLE_MIN_PAS;
}

void calcul_acceleration_orientation (void)
{
    if (VITESSE_MAX.orientation < VITESSE_ANGLE_PAS) //Si on est inferieur au r�glage consigne
    {
        if (ERREUR_ORIENTATION.actuelle > 0.)
        {
            acc.acceleration.orientation.consigne = VITESSE_MAX.orientation;
            acc.acceleration.orientation.consigne *= acc.acceleration.orientation.max; 
            acc.acceleration.orientation.consigne /= VITESSE_ANGLE_PAS;

            acc.deceleration.orientation.consigne = VITESSE_MAX.orientation;
            acc.deceleration.orientation.consigne *= acc.deceleration.orientation.max; 
            acc.deceleration.orientation.consigne /= VITESSE_ANGLE_PAS;
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.orientation.consigne = VITESSE_MAX.orientation;
            acc.acceleration.orientation.consigne *= acc.deceleration.orientation.max; 
            acc.acceleration.orientation.consigne /= VITESSE_ANGLE_PAS;

            acc.deceleration.orientation.consigne = VITESSE_MAX.orientation;
            acc.deceleration.orientation.consigne *= acc.acceleration.orientation.max; 
            acc.deceleration.orientation.consigne /= VITESSE_ANGLE_PAS;
        }
    }
    else //sinon on sature l'acceleration
    {
        if ( ERREUR_ORIENTATION.actuelle > 0.)
        {
            acc.acceleration.orientation.consigne = acc.acceleration.orientation.max; 
            acc.deceleration.orientation.consigne = acc.deceleration.orientation.max; 
        }
        else //inversion acceleration et deceleration
        {
            acc.acceleration.orientation.consigne = acc.deceleration.orientation.max; 
            acc.deceleration.orientation.consigne = acc.acceleration.orientation.max; 
        }
    }

    if (acc.acceleration.orientation.consigne < acc.acceleration.orientation.min) 
        acc.acceleration.orientation.consigne = acc.acceleration.orientation.min; 
    if (acc.deceleration.orientation.consigne < acc.deceleration.orientation.min) 
        acc.deceleration.orientation.consigne = acc.deceleration.orientation.min;
}

void calcul_distance_consigne_XY (void)
{
    if (FLAG_ASSERV.type_consigne == XY)
    {
        __attribute__((near)) static double delta_x, delta_y;
        delta_x = (double) (X.consigne - X.actuelle);
        delta_y = (double) (Y.consigne - Y.actuelle);

        ORIENTATION.consigne = atan2(delta_y, delta_x ) * (ENTRAXE_TICKS /2.);

        if (FLAG_ASSERV.sens_deplacement == MARCHE_ARRIERE)
        {
            if (ORIENTATION.consigne < 0.)
                ORIENTATION.consigne += Pi * ENTRAXE_TICKS/2.;
            else
                ORIENTATION.consigne -= Pi * ENTRAXE_TICKS/2.;
        }


        DISTANCE.consigne = sqrt(delta_x * delta_x + delta_y * delta_y);
        DISTANCE.consigne *= FLAG_ASSERV.sens_deplacement;
        DISTANCE.actuelle = 0.;


        if ((ORIENTATION.consigne - ORIENTATION.actuelle) > Pi * ENTRAXE_TICKS/2.)
            ORIENTATION.consigne -= Pi * ENTRAXE_TICKS;
        else if (ORIENTATION.consigne - ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2.)
            ORIENTATION.consigne += Pi * ENTRAXE_TICKS;
    }
}


/******************************************************************************/
/**************************** Fonctions Asserv ********************************/
/******************************************************************************/


void brake(void)
{
    ERREUR_BRAKE[ROUE_DROITE].actuelle = 0.;
    ERREUR_BRAKE[ROUE_DROITE].integralle = 0.;
    ERREUR_BRAKE[ROUE_DROITE].precedente = 0.;

    ERREUR_BRAKE[ROUE_GAUCHE].actuelle = 0.;
    ERREUR_BRAKE[ROUE_GAUCHE].integralle = 0.;
    ERREUR_BRAKE[ROUE_GAUCHE].precedente = 0.;

    BRAKE[ROUE_DROITE].actuelle = 0.;
    BRAKE[ROUE_GAUCHE].actuelle = 0.;

    FLAG_ASSERV.brake = ON;
}

void unbrake (void)
{
    ERREUR_VITESSE[ROUE_DROITE].actuelle = 0.;
    ERREUR_VITESSE[ROUE_DROITE].integralle = 0.;
    ERREUR_VITESSE[ROUE_DROITE].precedente = 0.;

    ERREUR_VITESSE[ROUE_GAUCHE].actuelle = 0.;
    ERREUR_VITESSE[ROUE_GAUCHE].integralle = 0.;
    ERREUR_VITESSE[ROUE_GAUCHE].precedente = 0.;

    VITESSE[ROUE_DROITE].actuelle = 0.;
    VITESSE[ROUE_DROITE].consigne = 0.;
    VITESSE[ROUE_DROITE].theorique = 0.;

    VITESSE[ROUE_GAUCHE].actuelle = 0.;
    VITESSE[ROUE_GAUCHE].consigne = 0.;
    VITESSE[ROUE_GAUCHE].theorique = 0.;

    VITESSE_ORIENTATION[ROUE_DROITE].actuelle = 0.;
    VITESSE_ORIENTATION[ROUE_DROITE].consigne = 0.;
    VITESSE_ORIENTATION[ROUE_DROITE].theorique = 0.;

    VITESSE_ORIENTATION[ROUE_GAUCHE].actuelle = 0.;
    VITESSE_ORIENTATION[ROUE_GAUCHE].consigne = 0.;
    VITESSE_ORIENTATION[ROUE_GAUCHE].theorique = 0.;

    FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    FLAG_ASSERV.brake = OFF;
}

void fin_deplacement (void)
{
    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
}

//Fonction principale de l'asserv, qui permet d'activer les diff�rents asserv
//D'envoyer la commande aux moteurs, et de sortir des fonctions de d�placements
void asserv()
{
    VITESSE[ROUE_DROITE].consigne = 0.;
    VITESSE[ROUE_GAUCHE].consigne = 0.;

 
    //Fonction de sortie de l'asserv, v�rifie que le robot est bien a sa position.
    if (FLAG_ASSERV.etat_angle == ANGLE_ATTEINT && FLAG_ASSERV.etat_distance == DISTANCE_ATTEINTE && FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT)
    {
        PORTCbits.RC5 = 0;
        FLAG_ASSERV.immobilite ++;

        if (FLAG_ASSERV.vitesse_fin_nulle == ON && FLAG_ASSERV.brake == OFF)
            brake();
     
        if (FLAG_ASSERV.immobilite >= SEUIL_IMMOBILITE)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
        else if (FLAG_ASSERV.vitesse_fin_nulle == OFF)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
    }
    else
    {
        if (FLAG_ASSERV.immobilite >= PID.VITESSE_DIS.seuil_immobilite )
        {
            FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
            FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
        }
    }

    //Fonction d'appel de l'asserv
    if ( (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT && (FLAG_ASSERV.etat_angle == EN_COURS || FLAG_ASSERV.etat_distance == EN_COURS)) || FLAG_ASSERV.brake == ON )
    {
        if (FLAG_ASSERV.brake == OFF)
        {
            if (FLAG_ASSERV.position == ON)
            {
                asserv_distance();
            }
            if (FLAG_ASSERV.orientation == ON)
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
        }
       
        //R�initialisation des commandes moteurs � 0
        init_commande_moteur();

        if (FLAG_ASSERV.brake == OFF)
        {
            //Calcul des consignes moteurs gauche et droit, asserv indispendable
            fonction_PID(ASSERV_VITESSE_DISTANCE);
        }
        else
            asserv_brake();
        
        ecretage_consignes();

        //envoit sur les moteurs
        envoit_pwm(MOTEUR_DROIT,  COMMANDE.droit);
        envoit_pwm(MOTEUR_GAUCHE, COMMANDE.gauche);
        
    }
    else
    {
        if (FLAG_ASSERV.vitesse_fin_nulle == ON)
        {
            //Si aucun asserv, on bloque les moteurs � 0;
            envoit_pwm(MOTEUR_DROIT, 0.);
            envoit_pwm(MOTEUR_GAUCHE, 0.);
        }
    }     
}

void asserv_brake(void)
{
    ERREUR_BRAKE[ROUE_DROITE].actuelle = BRAKE[ROUE_DROITE].actuelle;
    ERREUR_BRAKE[ROUE_GAUCHE].actuelle = BRAKE[ROUE_GAUCHE].actuelle;

    ERREUR_BRAKE[ROUE_DROITE].integralle += ERREUR_BRAKE[ROUE_DROITE].actuelle;
    ERREUR_BRAKE[ROUE_GAUCHE].integralle += ERREUR_BRAKE[ROUE_GAUCHE].actuelle;

    if (ERREUR_BRAKE[ROUE_DROITE].integralle > PID.BRAKE.max_I)
        ERREUR_BRAKE[ROUE_DROITE].integralle = PID.BRAKE.max_I;
    if (ERREUR_BRAKE[ROUE_GAUCHE].integralle > PID.BRAKE.max_I)
        ERREUR_BRAKE[ROUE_GAUCHE].integralle = PID.BRAKE.max_I;

    if (ERREUR_BRAKE[ROUE_DROITE].integralle < -PID.BRAKE.max_I)
        ERREUR_BRAKE[ROUE_DROITE].integralle = -PID.BRAKE.max_I;
    if (ERREUR_BRAKE[ROUE_GAUCHE].integralle < -PID.BRAKE.max_I)
        ERREUR_BRAKE[ROUE_GAUCHE].integralle = -PID.BRAKE.max_I;

    COMMANDE.droit  =  ERREUR_BRAKE[ROUE_DROITE].actuelle * PID.BRAKE.KP + ERREUR_BRAKE[ROUE_DROITE].integralle * PID.BRAKE.KI + (ERREUR_BRAKE[ROUE_DROITE].actuelle - ERREUR_BRAKE[ROUE_DROITE].precedente ) * PID.BRAKE.KD;
    COMMANDE.gauche =  ERREUR_BRAKE[ROUE_GAUCHE].actuelle * PID.BRAKE.KP + ERREUR_BRAKE[ROUE_GAUCHE].integralle * PID.BRAKE.KI + (ERREUR_BRAKE[ROUE_GAUCHE].actuelle - ERREUR_BRAKE[ROUE_GAUCHE].precedente ) * PID.BRAKE.KD;

    ERREUR_BRAKE[ROUE_DROITE].precedente = ERREUR_BRAKE[ROUE_DROITE].actuelle;
    ERREUR_BRAKE[ROUE_GAUCHE].precedente = ERREUR_BRAKE[ROUE_GAUCHE].actuelle;
}

// Fonction qui g�re l'asserv de Distance, donne les consignes pour g�n�rer les rampes
void asserv_distance(void)
{
    __attribute__((near)) static double distance_restante = 0.;
    __attribute__((near)) static double temps_freinage, temps_restant;
    
    calcul_distance_consigne_XY();
    distance_restante = fonction_PID(ASSERV_POSITION);

    if (FLAG_ASSERV.type_consigne == MM)
    {
        if (distance_restante < 0.)
            FLAG_ASSERV.sens_deplacement = MARCHE_ARRIERE;
        else
            FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;
    }

    if ((FLAG_ASSERV.sens_deplacement * distance_restante > 2. * TICKS_PAR_MM))
    {
        //si on se trouve dans un cercle de 5 cm autour du point d'arriv�
        if (FLAG_ASSERV.sens_deplacement * distance_restante < 30. * TICKS_PAR_MM) //30
        {
            FLAG_ASSERV.orientation = OFF;
            //SI on s'�loigne de notre consigne on s'arr�te
            if (ERREUR_DISTANCE.actuelle > ERREUR_DISTANCE.precedente)
            {
                FLAG_ASSERV.position = OFF;
                FLAG_ASSERV.orientation = OFF;
                FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
                FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
                return;
            }
        }

        if (FLAG_ASSERV.vitesse_fin_nulle == OFF)
        {
            if (FLAG_ASSERV.sens_deplacement * distance_restante < 150. * TICKS_PAR_MM) //150
            {
                FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
                //FLAG_ASSERV.position = OFF;
                //FLAG_ASSERV.orientation =  OFF;
                FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
                return;
            }
        }

        FLAG_ASSERV.etat_distance = EN_COURS;
        if (distance_restante > 0.)
        {
            VITESSE[SYS_ROBOT].consigne =  VITESSE_MAX.position; //vmax
            //temps_freinage = VITESSE[SYS_ROBOT].theorique / (2 * acc.deceleration.position.consigne);
        }
        else if (distance_restante < 0.)
        {
            VITESSE[SYS_ROBOT].consigne = - VITESSE_MAX.position; //Vmin //-120
            //temps_freinage = VITESSE[SYS_ROBOT].theorique / (acc.acceleration.position.consigne);
        }

        //G�n�ration de la courbe de freinage
       if (FLAG_ASSERV.vitesse_fin_nulle == ON)
       {
            temps_restant = distance_restante / VITESSE[SYS_ROBOT].theorique;
            temps_freinage = VITESSE[SYS_ROBOT].theorique / (acc.deceleration.position.consigne);
            temps_freinage /= COEF_FREINAGE;

            if (temps_freinage < 0.)
                temps_freinage *= -1.;

            if (temps_restant < 0.)
                temps_restant *= -1.;

            // Si le robot doit freiner
            if (temps_freinage > temps_restant)
            {
                    FLAG_ASSERV.phase_deceleration_distance = EN_COURS;
                    VITESSE[SYS_ROBOT].consigne = 0.;
            }
        }
        

    }
    // Si on arrive � 2mm du point et que l'on veut une vitesse d'arr�t nulle
    else 
    {
        FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
        FLAG_ASSERV.position = OFF;
        FLAG_ASSERV.orientation = OFF;
        FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    }
   /* else //Sinon on passe au d�placement suivant sans arr�t
    {
        FLAG_ASSERV.etat_distance = FIN_DEPLACEMENT;
    }*/
       
}

//Fonction qui g�n�re les rampes de vitesse pour l'asserv en Distance
void asserv_vitesse_distance (void)
{
    if (FLAG_ASSERV.phase_deceleration_distance == PHASE_NORMAL || VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0. )
    {
        if (VITESSE[SYS_ROBOT].theorique < VITESSE[SYS_ROBOT].consigne)
            VITESSE[SYS_ROBOT].theorique += acc.acceleration.position.consigne;
        else if (VITESSE[SYS_ROBOT].theorique > VITESSE[SYS_ROBOT].consigne)
            VITESSE[SYS_ROBOT].theorique -= acc.deceleration.position.consigne;

        saturation_vitesse_max(ASSERV_POSITION);
    }
    
    if (FLAG_ASSERV.orientation == ON)
    {
        fonction_PID(KP_HYBRIDE);
        //VITESSE[ROUE_DROITE].consigne += VITESSE[SYS_ROBOT].theorique * KP_hybride;
        //VITESSE[ROUE_GAUCHE].consigne += VITESSE[SYS_ROBOT].theorique * KP_hybride;
    }
    //else
    //{
        VITESSE[ROUE_DROITE].consigne += VITESSE[SYS_ROBOT].theorique;
        VITESSE[ROUE_GAUCHE].consigne += VITESSE[SYS_ROBOT].theorique;
    //}
}


//Fonction qui permet de cr�er les consignes pour g�n�rer les rampes de Vitesse
void asserv_orientation (void)
{
    __attribute__((near)) static double angle_restant = 0.;
    __attribute__((near)) static double temps_freinage, temps_restant;
    //static int32_t compteur  = 0;

    angle_restant = fonction_PID(ASSERV_ORIENTATION); 

    if ( (angle_restant >  0.1 * Pi / 180. * ENTRAXE_TICKS) || (angle_restant < - 0.1 * Pi / 180. * ENTRAXE_TICKS ))//|| FLAG_ASSERV.position == ON)) //2�
    {
        FLAG_ASSERV.etat_angle = EN_COURS;

        if (angle_restant > 0.) 
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne =  VITESSE_MAX.orientation;
            temps_freinage = VITESSE_ORIENTATION[SYS_ROBOT].theorique / (acc.deceleration.orientation.consigne);
        }
        else if (angle_restant < 0.)
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne = - VITESSE_MAX.orientation;
            temps_freinage = VITESSE_ORIENTATION[SYS_ROBOT].theorique / (acc.acceleration.orientation.consigne);
        }

        //G�n�ration de la courbe de freinage
        temps_restant = angle_restant / VITESSE_ORIENTATION[SYS_ROBOT].theorique;

        if (temps_freinage <0.)
            temps_freinage *= -1.;
        if (temps_restant <0.)
            temps_restant *= -1.;


        if (temps_freinage > temps_restant)
        {
            if (FLAG_ASSERV.type_deplacement == ORIENTER)
                FLAG_ASSERV.phase_decelaration_orientation = EN_COURS;

            if (FLAG_ASSERV.type_deplacement == FAIRE_DES_TOURS && FLAG_ASSERV.vitesse_fin_nulle == OFF)
                FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
            else
                VITESSE_ORIENTATION[SYS_ROBOT].consigne = 0.;
        }
    }
    else //(FLAG_ASSERV.vitesse_fin_nulle == ON)
    {
        VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;
        FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
        FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
    }
}


//Fonction qui g�n�re les rampes de Vitesse pour la rotation du robot
void asserv_vitesse_orientation (void)
{
    //Si on a attaqu� la phase de d�c�l�ration, on ne permet plus de r�acc�l�rer
    if (FLAG_ASSERV.phase_decelaration_orientation == PHASE_NORMAL || VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0. )
    {
         if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < VITESSE_ORIENTATION[SYS_ROBOT].consigne)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique += acc.acceleration.orientation.consigne;


        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_ORIENTATION[SYS_ROBOT].consigne)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique -= acc.deceleration.orientation.consigne;

         saturation_vitesse_max(ASSERV_ORIENTATION);

         //d�bloquage de la l'interdiction d'acceleration si on arrive � une consigne nulle pour enlever l'erreur statiqye
        // if (VITESSE_ORIENTATION[SYS_ROBOT].theorique == 0)
        //     FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
    }

     VITESSE[ROUE_DROITE].consigne += VITESSE_ORIENTATION[SYS_ROBOT].theorique;
     VITESSE[ROUE_GAUCHE].consigne -= VITESSE_ORIENTATION[SYS_ROBOT].theorique;
}

/******************************************************************************/
/************************************ PID *************************************/
/******************************************************************************/

void reglage_PID (void)
{
    // PID VITESSE DISTANCE
    PID.VITESSE_DIS.KP = VITESSE_DIS_KP;
    PID.VITESSE_DIS.KI = VITESSE_DIS_KI;
    PID.VITESSE_DIS.KD = VITESSE_DIS_KD;

    // PID DISTANCE
    PID.DISTANCE.KP = POSITION_KP;
    PID.DISTANCE.KI = POSITION_KI;
    PID.DISTANCE.KD = POSITION_KD;

    // PID ORIENTATION
    PID.ORIENTATION.KP = ORIENTATION_KP;
    PID.ORIENTATION.KI = ORIENTATION_KI;
    PID.ORIENTATION.KD = ORIENTATION_KD;
    
    // PID BRAKE
    PID.BRAKE.KP = KP_BRAKE;
    PID.BRAKE.KP = KI_BRAKE;
    PID.BRAKE.KP = KD_BRAKE;
}

double fonction_PID (_enum_type_PID type)
{
    if (type == ASSERV_VITESSE_DISTANCE)
    {
        ERREUR_VITESSE[ROUE_DROITE].actuelle = VITESSE[ROUE_DROITE].consigne - VITESSE[ROUE_DROITE].actuelle;
        ERREUR_VITESSE[ROUE_DROITE].integralle += ERREUR_VITESSE[ROUE_DROITE].actuelle;      

        ERREUR_VITESSE[ROUE_GAUCHE].actuelle = VITESSE[ROUE_GAUCHE].consigne - VITESSE[ROUE_GAUCHE].actuelle;
        ERREUR_VITESSE[ROUE_GAUCHE].integralle += ERREUR_VITESSE[ROUE_GAUCHE].actuelle;

        saturation_erreur_integralle_vitesse();
        detection_blocage();

        COMMANDE.droit  = ERREUR_VITESSE[ROUE_DROITE].actuelle * PID.VITESSE_DIS.KP + ERREUR_VITESSE[ROUE_DROITE].integralle * PID.VITESSE_DIS.KI + (ERREUR_VITESSE[ROUE_DROITE].actuelle - ERREUR_VITESSE[ROUE_DROITE].precedente ) * PID.VITESSE_DIS.KD;
        COMMANDE.gauche = ERREUR_VITESSE[ROUE_GAUCHE].actuelle * PID.VITESSE_DIS.KP + ERREUR_VITESSE[ROUE_GAUCHE].integralle * PID.VITESSE_DIS.KI + (ERREUR_VITESSE[ROUE_GAUCHE].actuelle - ERREUR_VITESSE[ROUE_GAUCHE].precedente ) * PID.VITESSE_DIS.KD;

        ERREUR_VITESSE[ROUE_DROITE].precedente = ERREUR_VITESSE[ROUE_DROITE].actuelle;
        ERREUR_VITESSE[ROUE_GAUCHE].precedente = ERREUR_VITESSE[ROUE_GAUCHE].actuelle;
    }
    else if (type == ASSERV_POSITION)
    {
        __attribute__((near)) static double duty;

        ERREUR_DISTANCE.actuelle = DISTANCE.consigne - DISTANCE.actuelle;
        ERREUR_DISTANCE.integralle += ERREUR_DISTANCE.actuelle;

         duty =  ERREUR_DISTANCE.actuelle;// * PID.DISTANCE.KP  + ERREUR_DISTANCE.integralle * PID.DISTANCE.KI - (ERREUR_DISTANCE.actuelle - ERREUR_DISTANCE.precedente) * PID.DISTANCE.KD;
 
        ERREUR_DISTANCE.precedente = ERREUR_DISTANCE.actuelle;

        return duty;
    }
    else if ( type == ASSERV_ORIENTATION)
    {
        __attribute__((near)) static double duty;

        /*if ((ORIENTATION.consigne - ORIENTATION.actuelle) > Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne -= Pi * ENTRAXE_TICKS;
        else if (ORIENTATION.consigne - ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne += Pi * ENTRAXE_TICKS;*/
       /* while (ORIENTATION.consigne > Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne -= Pi * ENTRAXE_TICKS;
        while (ORIENTATION.consigne < - Pi * ENTRAXE_TICKS/2)
            ORIENTATION.consigne += Pi * ENTRAXE_TICKS/2;*/


        ERREUR_ORIENTATION.actuelle = ORIENTATION.consigne - ORIENTATION.actuelle;

        while (ERREUR_ORIENTATION.actuelle > (Pi * ENTRAXE_TICKS/2.))
            ERREUR_ORIENTATION.actuelle -=  Pi * ENTRAXE_TICKS;
        while (ERREUR_ORIENTATION.actuelle < - Pi * ENTRAXE_TICKS/2.)
            ERREUR_ORIENTATION.actuelle +=  Pi * ENTRAXE_TICKS;

        ERREUR_ORIENTATION.integralle += ERREUR_ORIENTATION.actuelle;
        if (ERREUR_ORIENTATION.integralle > Pi * ENTRAXE_TICKS/2.)
            ERREUR_ORIENTATION.integralle = Pi * ENTRAXE_TICKS/2.;
        else if (ERREUR_ORIENTATION.integralle < -Pi * ENTRAXE_TICKS/2.)
            ERREUR_ORIENTATION.integralle  = -Pi * ENTRAXE_TICKS/2.;

         duty =  ERREUR_ORIENTATION.actuelle; // * PID.ORIENTATION.KP + ERREUR_ORIENTATION.integralle * PID.ORIENTATION.KI - (ERREUR_ORIENTATION.actuelle - ERREUR_ORIENTATION.precedente) * PID.ORIENTATION.KD;
        ERREUR_ORIENTATION.precedente = ERREUR_ORIENTATION.actuelle;

        return duty;

    }
    else if (type == KP_HYBRIDE)
    {
        if (ERREUR_ORIENTATION.actuelle > 0.)
            KP_hybride = ERREUR_ORIENTATION.actuelle / (Pi * (ENTRAXE_TICKS/2.));
        else
            KP_hybride = - ERREUR_ORIENTATION.actuelle / (Pi * (ENTRAXE_TICKS/2.));



        if (FLAG_ASSERV.type_deplacement == PASSE_PART)
        {
            if (FLAG_ASSERV.phase_deceleration_distance == PHASE_NORMAL)
            {
                #ifdef PETIT_ROBOT
                    KP_hybride *= 0.1; //0.2
                #else
                    KP_hybride *= 0.05;
                #endif
            }
                
        }

        KP_hybride = 1. - KP_hybride;
        
        VITESSE[SYS_ROBOT].theorique *= KP_hybride;
        //return KP_hybride;
    }

    return (double) 0;

}

/******************************************************************************/
/****************************** ODOMETRIE *************************************/
/******************************************************************************/

void calcul_position_robot (void)
{
    __attribute__((near)) static int32_t delta_o, delta_d;
    __attribute__((near)) static double d_X = 0., d_Y = 0.;

    get_valeur_codeur (CODEUR_D);
    get_valeur_codeur (CODEUR_G);

    POSITION[CODEUR_D].ecart *= COEF_D;
    POSITION[CODEUR_G].ecart *= COEF_G;

    //calcul des modifs
    delta_o = (POSITION[CODEUR_D].ecart - POSITION[CODEUR_G].ecart) /2.;
    delta_d = (int32_t) (POSITION[CODEUR_D].ecart + POSITION[CODEUR_G].ecart) /2.;

    if (FLAG_ASSERV.brake == ON)
    {
        BRAKE[ROUE_DROITE].actuelle -= POSITION[CODEUR_D].ecart;
        BRAKE[ROUE_GAUCHE].actuelle -= POSITION[CODEUR_G].ecart;
    }

    //cumul des valeurs pour l'asserv
    DISTANCE.actuelle += (double) delta_d;

    VITESSE[SYS_ROBOT].actuelle = (double) delta_d;
    VITESSE[ROUE_DROITE].actuelle = (double) POSITION[CODEUR_D].ecart;
    VITESSE[ROUE_GAUCHE].actuelle = (double) POSITION[CODEUR_G].ecart;

    VITESSE_ORIENTATION[SYS_ROBOT].actuelle = (double) delta_o;

    ORIENTATION.actuelle += delta_o;

    //Angle compris entre - 180 et + 180
    if (ORIENTATION.actuelle > Pi * ENTRAXE_TICKS/2.)
        ORIENTATION.actuelle -= Pi * ENTRAXE_TICKS ;
    else if(ORIENTATION.actuelle < - Pi * (ENTRAXE_TICKS/2.))
        ORIENTATION.actuelle += Pi * ENTRAXE_TICKS;

    //Orientation en radian
    ROBOT.orientation = (double) ORIENTATION.actuelle / (ENTRAXE_TICKS / 2.);

    //Calcul des positions
    d_X = (double) cos (ROBOT.orientation) * delta_d;
    d_Y = (double) sin (ROBOT.orientation) * delta_d;

    X.actuelle += d_X;
    Y.actuelle += d_Y;

    //pas besoin de le calculer toutes les 5 ms --> perte de temps innutile
    ROBOT.X_mm += d_X * MM_PAR_TICKS;
    ROBOT.Y_mm += d_Y * MM_PAR_TICKS;
    ROBOT.orientation_degre = ROBOT.orientation * 180. / Pi;
}


