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

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/
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
 * Fonction qui initialise les flag de l'asserv pour démarer l'asserv
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
    ROBOT.orientation_degre = teta;
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
    FLAG_ASSERV.sens_rotation = ROTATION_POSITIVE;

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
    
    ERREUR_VITESSE[SYS_ROBOT].actuelle = 0.;
    ERREUR_VITESSE[SYS_ROBOT].integralle = 0.;
    ERREUR_VITESSE[SYS_ROBOT].precedente = 0.;

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
/*************************  Fonctions Conversions *****************************/
/******************************************************************************/

double MM_TO_TICK (double distance_mm)
{
    return (distance_mm * MM_PAR_TICKS);
}

double TICK_TO_MM (int32_t nb_ticks)
{
    return ((double) (nb_ticks) * (TICKS_PAR_MM));
}


/******************************************************************************/
/************************** Fonctions Saturations *****************************/
/******************************************************************************/

/**
 * ecretage_consignes()
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

/**
 * saturation_vitesse_max()
 * @brief Fonction appelé lors de la génération des rampes de vitesse
 * Permet de saturer la vitesse à sa valeur maximale
 * Permet également d'empêcher l'oscillation entre deux valeurs, si la vmax n'est pas un multiple de l'acel
 * @param type : asserv concerné (position, orientation)
 */
void saturation_vitesse_max (_enum_type_PID type)
{
    if (type == ASSERV_POSITION)
    {
        //Pour éviter d'osciller autour d'une valeur consigne en haut du trapèze
        //on écrète à la valeur max
        if (VITESSE[SYS_ROBOT].theorique > VITESSE_MAX.position)
            VITESSE[SYS_ROBOT].theorique = VITESSE_MAX.position;
        else if (VITESSE[SYS_ROBOT].theorique < - VITESSE_MAX.position)
            VITESSE[SYS_ROBOT].theorique = - VITESSE_MAX.position;

        //Pour ne pas osciller autour d'une consigne à 0,
       // Lors de la génération de la courbe de freinage, on empêche la génération de 
       // vitesse négative
        else if (VITESSE[SYS_ROBOT].consigne == 0.)
        {
            if (abs(VITESSE[SYS_ROBOT].theorique) < acc.deceleration.position.consigne )
                VITESSE[SYS_ROBOT].theorique = 0.;
        }
    }
    else if (type == ASSERV_ORIENTATION)
    {
        //Pour éviter d'osciller autour d'une valeur consigne en haut du trapèze
        //on écrète à la valeur max
        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_MAX.orientation)
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = VITESSE_MAX.orientation;
//        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < - VITESSE_MAX.orientation)
//            VITESSE_ORIENTATION[SYS_ROBOT].theorique = - VITESSE_MAX.orientation;

        //Pour ne pas osciller autour d'une consigne à 0,
        //On check, lors de la génération de la courbe de freinage,
        //Que si on va passer négatif alors on écrette à 0.
        else if (VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0.)
        {
            if (abs(VITESSE_ORIENTATION[SYS_ROBOT].theorique) < acc.deceleration.orientation.consigne )
                VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;
        }

    }
}

/**
 * saturation_erreur_integralle_vitesse()
 * @brief : Saturation du terme intégrale 
 */
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

/**
 * detection_blocage()
 */
void detection_blocage (void)
{
    
    // TODO : Define pourcentage mini ailleurs
    double pourcentage_vitesse_mini = 0.2;
    
    // Si la vitesse est trop faible (20% de la consigne) et que le terme intégralle est saturé,
    // Alors on notifie d'un blocage
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

/**
 * calcul_vitesse_position()
 * @brief : détermine la vitesse max à atteindre pour le déplacement basé sur : 
 *      - La longueur du déplacement
 *      - La vitesse maximale auorisée
 *      - La vitesse minimale autorisée
 *      - le ratio envoyé par l'utilisateur
 * @param pourcentage_vitesse : % appliqué à la valeur max calculée 
 */
void calcul_vitesse_position (double pourcentage_vitesse)
{
    calcul_distance_consigne_XY();
    double temp;

    // diminue la v max calculé
    // on ne considère pas la courbe comme étant linéaire
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

    // On écrète par le haut avant la mise à l'échelle pour que cette dernière 
    // ait une réelle signifiacation (la fonction de calcul de vitesse, dans le cas de grande
    // vitesse, obtiens des vitesse bien superieur au max)
    if (VITESSE_MAX.position > VITESSE_MAX_TENSION)
        VITESSE_MAX.position = VITESSE_MAX_TENSION;
    
    // Mise à l'échelle de la vitesse par rapport à la requête utilisateur 
    // (pourcentage de la vitesse max calculée)
    VITESSE_MAX.position *= pourcentage_vitesse;
    VITESSE_MAX.position /= 100.;
    
    // on écrète une seconde fois (l'utilisateur pouvant demander une vitesse > 100 %)
    // On peut avoir dépassé la valeur max
    if (VITESSE_MAX.position > VITESSE_MAX_TENSION)
        VITESSE_MAX.position = VITESSE_MAX_TENSION;
    
    // On écrète la valeur min après la mise à l'échelle pour empêcher que la mise à l'échelle
    // induise des vitesse inférieur au minimum
    if (VITESSE_MAX.position < VITESSE_DISTANCE_MIN_PAS)
        VITESSE_MAX.position = VITESSE_DISTANCE_MIN_PAS;
}

/**
 * calcul_acceleration_position()
 * @brief FOnction qui calcul les accélérations et décélérations pour construire la rampe de votesse
 * En fonction de la distance à parcourir
 */
void calcul_acceleration_position (void)
{
    double accelMax = 0.;
    double accelMin = 0.;
    double decelMax = 0.;
    double decelMin = 0.;
    
    accelMin = acc.acceleration.position.min;
    accelMax = acc.acceleration.position.max;
    decelMin = acc.deceleration.position.min;
    decelMax = acc.deceleration.position.max;
   
    
    //TODO : VITESSE_CONSIGNE_MAX_PAS ?
    // on fait un produit en croix par rapport à la calib, (petite vitesse, plus faible accélération)
    acc.acceleration.position.consigne = VITESSE_MAX.position;
    acc.acceleration.position.consigne *= accelMax; 
    acc.acceleration.position.consigne /= VITESSE_CONSIGNE_MAX_PAS;

    acc.deceleration.position.consigne = VITESSE_MAX.position;
    acc.deceleration.position.consigne *= decelMax; 
    acc.deceleration.position.consigne /= VITESSE_CONSIGNE_MAX_PAS;

    
    // Puis on sature l'acélération/décélaration aux valuers min/max
    if (acc.acceleration.position.consigne < accelMin)
    {
        acc.acceleration.position.consigne = accelMin;
    }    
    else if (acc.acceleration.position.consigne > accelMax)
    {
        acc.acceleration.position.consigne = accelMax;
    }
    
    if (acc.deceleration.position.consigne < decelMin) // /!\ REMPLACEMENT DU SIGNE > PAR <
    {
        acc.deceleration.position.consigne = decelMin;
    }
    else if (acc.deceleration.position.consigne > decelMax)
    {
        acc.deceleration.position.consigne = decelMax;
    }
}

void calcul_vitesse_orientation (double pourcentage_vitesse)
{
    calcul_distance_consigne_XY();

    fonction_PID(ASSERV_ORIENTATION);

    // Calcul de la vitesse à appliquer pour l'orientation
    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS;
    VITESSE_MAX.orientation *= ERREUR_ORIENTATION.actuelle; // orientation restante à parcourir
    VITESSE_MAX.orientation /= ORIENTATION_CONSIGNE_PAS;
    
    // Valeur absolue
    if (VITESSE_MAX.orientation < 0.)
        VITESSE_MAX.orientation *= - 1.;
    
    // Saturation de la valeur max pour que la mise à l'échelle est un sens réel
    if (VITESSE_MAX.orientation > VITESSE_MAX_TENSION)
        VITESSE_MAX.orientation = VITESSE_MAX_TENSION;
    
    // mise à l'échelle
    VITESSE_MAX.orientation *= pourcentage_vitesse;
    VITESSE_MAX.orientation /= 100.;


    // Re saturation après la mise à l'échelle (pouvant être > 100 %)
    if (VITESSE_MAX.orientation > VITESSE_MAX_TENSION)
    {
        VITESSE_MAX.orientation = VITESSE_MAX_TENSION;
    }
    
    if (VITESSE_MAX.orientation < VITESSE_ANGLE_MIN_PAS)
    {
        VITESSE_MAX.orientation = VITESSE_ANGLE_MIN_PAS;
    }
}

// TODO : faire comme pour la distance ...
void calcul_acceleration_orientation (void)
{
    double accelMax = 0.;
    double accelMin = 0.;
    double decelMax = 0.;
    double decelMin = 0.;
    
    accelMin = acc.acceleration.orientation.min;
    accelMax = acc.acceleration.orientation.max;
    decelMin = acc.deceleration.orientation.min;
    decelMax = acc.deceleration.orientation.max;  
   
    //TODO : VITESSE_ANGLE_PAS ?
    // on fait un produit en croix par rapport à la calib, (petite vitesse, plus faible accélération)
    acc.acceleration.orientation.consigne = VITESSE_MAX.orientation;
    acc.acceleration.orientation.consigne *= accelMax; 
    acc.acceleration.orientation.consigne /= VITESSE_ANGLE_PAS;

    acc.deceleration.orientation.consigne = VITESSE_MAX.orientation;
    acc.deceleration.orientation.consigne *= decelMax; 
    acc.deceleration.orientation.consigne /= VITESSE_ANGLE_PAS;


    // Puis on sature l'acélération/décélaration aux valuers min/max
    if (acc.acceleration.orientation.consigne < accelMin)
    {
        acc.acceleration.orientation.consigne = accelMin; 
    }
    else if (acc.acceleration.orientation.consigne > accelMax)
    {
        acc.acceleration.orientation.consigne = accelMax; 
    }
        
    if (acc.deceleration.orientation.consigne < decelMin)
    {
        acc.deceleration.orientation.consigne = decelMin;
    }
    else if (acc.deceleration.orientation.consigne > decelMax)
    {
        acc.deceleration.orientation.consigne = decelMax;
    }
        
}

void calcul_distance_consigne_XY (void)
{
    if (FLAG_ASSERV.type_consigne == XY)
    {
        __attribute__((near)) static double delta_x, delta_y;
        delta_x = (double) (X.consigne - X.actuelle);
        delta_y = (double) (Y.consigne - Y.actuelle);

        ORIENTATION.consigne = atan2(delta_y, delta_x ) * (ENTRAXE_TICKS /2.);

        // Si on va en marche arrière, alors la consigne d'angle est inversée
        // (une inversion de l'angle revient à lui enlever ou lui ajouter Pi)
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


        // Orientation conprise entre Pi et - Pi
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
    ERREUR_VITESSE[SYS_ROBOT].actuelle = 0.;
    ERREUR_VITESSE[SYS_ROBOT].integralle = 0.;
    ERREUR_VITESSE[SYS_ROBOT].precedente = 0.;
    
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

void fin_deplacement_avec_brake (void)
{
    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    VITESSE[SYS_ROBOT].consigne = 0.;
}


void fin_deplacement_sans_brake (void)
{
    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
}

//Fonction principale de l'asserv, qui permet d'activer les différents asserv
//D'envoyer la commande aux moteurs, et de sortir des fonctions de déplacements
void asserv()
{
    VITESSE[ROUE_DROITE].consigne = 0.;
    VITESSE[ROUE_GAUCHE].consigne = 0.;

 
    //Fonction de sortie de l'asserv, vérifie que le robot est bien a sa position.
    if (FLAG_ASSERV.etat_angle == ANGLE_ATTEINT && FLAG_ASSERV.etat_distance == DISTANCE_ATTEINTE && FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT)
    {
        PORTCbits.RC5 = 0;
        FLAG_ASSERV.immobilite ++;

        if (FLAG_ASSERV.vitesse_fin_nulle == ON && FLAG_ASSERV.brake == OFF)
            brake();
     
        if (FLAG_ASSERV.immobilite >= PID.VITESSE_DIS.seuil_immobilite)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
        else if (FLAG_ASSERV.vitesse_fin_nulle == OFF)
            FLAG_ASSERV.fin_deplacement = FIN_DEPLACEMENT;
    }
    else
    {
        // Le robot est bloqué ou immobile depuis trop longtemps
        if (FLAG_ASSERV.immobilite >= PID.VITESSE_DIS.seuil_immobilite )
        {
            fin_deplacement_sans_brake();
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
       
        //Réinitialisation des commandes moteurs à 0
        init_commande_moteur();

        if (FLAG_ASSERV.brake == OFF)
        {
            //Calcul des consignes moteurs gauche et droit, asserv indispendable
            fonction_PID(ASSERV_VITESSE_DISTANCE);
        }
        else
        {
            asserv_brake();
        }
        
        ecretage_consignes();

        //envoit sur les moteurs
        envoit_pwm(MOTEUR_DROIT,  COMMANDE.droit);
        envoit_pwm(MOTEUR_GAUCHE, COMMANDE.gauche);
        
    }
    else
    {
        if (FLAG_ASSERV.vitesse_fin_nulle == ON)
        {
            //Si aucun asserv, on bloque les moteurs à 0;
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

// Fonction qui gère l'asserv de Distance, donne les consignes pour générer les rampes
void asserv_distance(void)
{
    __attribute__((near)) static double distance_restante = 0.;
    __attribute__((near)) static double distance_freinage = 0.;
    __attribute__((near)) static double erreur_distance_precedente = 0.;
    __attribute__((near)) static double erreur_de_suivie = 0.;
    __attribute__((near)) static double distance_freinage_anticipation = 0.;
    __attribute__((near)) static double coef_distance_freinage = 0.075;
    __attribute__((near)) static double fenetre_arrivee = 30 * TICKS_PAR_MM;
    __attribute__((near)) static double distance_min_passe_part = (150 * TICKS_PAR_MM);
    
    // on sauvegarde l'erreur de distance du cycle précédent avant que la valeur ne soit écrasé
    // Par le calcul de la fonction_PID(ASSERV_POSITION)
    erreur_distance_precedente = ERREUR_DISTANCE.actuelle;
    erreur_de_suivie = abs(VITESSE[SYS_ROBOT].theorique - VITESSE[SYS_ROBOT].actuelle);
    
    // Calcul de la distance restante
    calcul_distance_consigne_XY();
    distance_restante = fonction_PID(ASSERV_POSITION);

    // Génération des vittesses consignes à atteindre (haut du trapèze))
    if (distance_restante > 0.)
    {
        FLAG_ASSERV.sens_deplacement = MARCHE_AVANT; 
        VITESSE[SYS_ROBOT].consigne =  VITESSE_MAX.position; //vmax
    }
    else if (distance_restante < 0.)
    {
        FLAG_ASSERV.sens_deplacement = MARCHE_ARRIERE;
        VITESSE[SYS_ROBOT].consigne = - VITESSE_MAX.position; //Vmin //-120
        
        // ensure the distance is always positive
        distance_restante *= -1;
    }

    //Génération de la courbe de freinage
    if (FLAG_ASSERV.vitesse_fin_nulle == ON)
    {
        // calcul de la distance théorique de freinage (trapèze)
        distance_freinage = (VITESSE[SYS_ROBOT].actuelle * VITESSE[SYS_ROBOT].actuelle) / (2. * acc.deceleration.position.consigne);

        // Recalcul de la distance de freinage par anticipation à rajouter à la 
        // distance de freinage tant qu'on est pas rentré dans la phase de freinage 
        if (FLAG_ASSERV.phase_deceleration_distance != EN_COURS)
        {
            distance_freinage_anticipation = distance_freinage * coef_distance_freinage;
        }

        // Si le robot doit freiner       (distance de freinage + distance parcouru en 1 coup)
        if (distance_restante <= (distance_freinage + distance_freinage_anticipation)) 
        {
            FLAG_ASSERV.phase_deceleration_distance = EN_COURS;
            VITESSE[SYS_ROBOT].consigne = 0.;
        }

        //si on se trouve dans un cercle de 3 cm autour du point d'arrivé
        if (distance_restante < fenetre_arrivee) //30
        {
            FLAG_ASSERV.orientation = OFF;

            // on ajuste la pente de freinage à quand on est proche de la cible 
            // s'assurer d'arriver avec une v=0 au bon endroit
            if (distance_restante < distance_freinage_anticipation) 
            {
                acc.deceleration.position.consigne = (VITESSE[SYS_ROBOT].actuelle * VITESSE[SYS_ROBOT].actuelle) / (2. * (distance_restante));
            }

            // Si le robot est immobile
            if (VITESSE[SYS_ROBOT].theorique == 0 && FLAG_ASSERV.fin_deplacement != DEBUT_DEPLACEMENT)
            {
                fin_deplacement_avec_brake();
            }


            // la vitesse = distance parcouru en 1 cycle d'asserv
            // Si la distance restante < distance_parcouru au cycle n +1
            // distance parcouru au cycle n+1 = (Vactu + Vactu+1)/2
            // (on moyenne la vitesse, et une vitesse = distance parcouru en un cycle)
            // vrai uniquement en phase de decel ...
            // TODO : anticipation sur plus de cycle ?
//            {
//                double nextVActu = VITESSE[SYS_ROBOT].actuelle * FLAG_ASSERV.sens_deplacement - acc.deceleration.position.consigne;
//                if (nextVActu < 0.)
//                    nextVActu = 0.;
//     
//                if ( (distance_restante + erreur_de_suivie) < ((VITESSE[SYS_ROBOT].actuelle * FLAG_ASSERV.sens_deplacement + nextVActu) / 2.) )
//                {
//                    fin_deplacement_avec_brake();
//                }
//            }
            
//            // On aura dépassé la position consigne au prochain coup 
//            if ( (distance_restante + erreur_de_suivie) < abs(( VITESSE[SYS_ROBOT].actuelle - acc.deceleration.position.consigne) / 2.) )
//            {
//                fin_deplacement_avec_brake();
//            }
            
            // on s'éloigne de la cible 
            if ( (((ERREUR_DISTANCE.actuelle - erreur_distance_precedente) * FLAG_ASSERV.sens_deplacement) > 0 ) && FLAG_ASSERV.fin_deplacement != DEBUT_DEPLACEMENT)
            {
                fin_deplacement_avec_brake();
            }
        }

        FLAG_ASSERV.fin_deplacement = EN_COURS;
    }
    else // vitesse fin non nulle
    {
         if (abs(FLAG_ASSERV.sens_deplacement * distance_restante) < distance_min_passe_part) //150
         {
             FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
             FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
             return;
         }
    }      
}

//Fonction qui génère les rampes de vitesse pour l'asserv en Distance
void asserv_vitesse_distance (void)
{
    if (FLAG_ASSERV.phase_deceleration_distance == PHASE_NORMAL || VITESSE[SYS_ROBOT].consigne == 0. )
    {     
        // En marche avant la vitesse consigne est positive
        if (FLAG_ASSERV.sens_deplacement == MARCHE_AVANT)
        {
            // acceleration positive et décélération négative
            if (VITESSE[SYS_ROBOT].theorique < VITESSE[SYS_ROBOT].consigne)
            {
                VITESSE[SYS_ROBOT].theorique += acc.acceleration.position.consigne;
            }
            else if (VITESSE[SYS_ROBOT].theorique > VITESSE[SYS_ROBOT].consigne)
            {
                VITESSE[SYS_ROBOT].theorique -= acc.deceleration.position.consigne;
            }
        }
        // En marche arrière la vitesse consigne est négative (inversion de tous les signes)
        else // MARCHE_ARRIERE
        {
            // acceleration négative et décélération positive
            if (VITESSE[SYS_ROBOT].theorique > VITESSE[SYS_ROBOT].consigne)
            {
                VITESSE[SYS_ROBOT].theorique -= acc.acceleration.position.consigne;
            }
            else if (VITESSE[SYS_ROBOT].theorique < VITESSE[SYS_ROBOT].consigne)
            {
                VITESSE[SYS_ROBOT].theorique += acc.deceleration.position.consigne;
            }
        }

        saturation_vitesse_max(ASSERV_POSITION);
    }
    
    if (FLAG_ASSERV.orientation == ON)
    {
        fonction_PID(KP_HYBRIDE);
    }

    VITESSE[ROUE_DROITE].consigne += VITESSE[SYS_ROBOT].theorique;
    VITESSE[ROUE_GAUCHE].consigne += VITESSE[SYS_ROBOT].theorique;
}


//Fonction qui permet de créer les consignes pour générer les rampes de Vitesse
void asserv_orientation (void)
{
    __attribute__((near)) static double angle_restant = 0.;
    __attribute__((near)) static double temps_freinage, temps_restant;

    //static int32_t compteur  = 0;

    angle_restant = fonction_PID(ASSERV_ORIENTATION); 

    if ( (angle_restant >  0.1 * Pi / 180. * ENTRAXE_TICKS) || (angle_restant < - 0.1 * Pi / 180. * ENTRAXE_TICKS ))//|| FLAG_ASSERV.position == ON)) //2°
    {
        FLAG_ASSERV.etat_angle = EN_COURS;

        // par défaut la vitesse consigne est la vitesse max (haut de la rampe)
        VITESSE_ORIENTATION[SYS_ROBOT].consigne = VITESSE_MAX.orientation;
        
        if (angle_restant > 0.)
        {
            FLAG_ASSERV.sens_rotation = ROTATION_POSITIVE;
        }
        else  if (angle_restant < 0.)
        {
            FLAG_ASSERV.sens_rotation = ROTATION_NEGATIVE;
            
            // on s'assure que l'angle soit toujours positif pour la suite
            angle_restant *= -1;
        }
        else
        {
            VITESSE_ORIENTATION[SYS_ROBOT].consigne = 0;
            VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0;
            FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
        }

        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique != 0)
        {
            //Génération de la courbe de freinage
            temps_freinage = VITESSE_ORIENTATION[SYS_ROBOT].theorique / (acc.deceleration.orientation.consigne);
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
                {
                    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
                }
                else
                {
                    VITESSE_ORIENTATION[SYS_ROBOT].consigne = 0.;
                    FLAG_ASSERV.phase_decelaration_orientation = EN_COURS;
                }
            }
        }  
    }
    else //(FLAG_ASSERV.vitesse_fin_nulle == ON)
    {
        VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;
        FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
        FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
    }
//    __attribute__((near)) static double angle_restant = 0.;
//    __attribute__((near)) static double angle_freinage = 0.;
//    __attribute__((near)) static double erreur_angle_precedent = 0.;
//    __attribute__((near)) static double erreur_de_suivie = 0.;
//    __attribute__((near)) static double angle_freinage_anticipation = 0.;
//    __attribute__((near)) static double coef_angle_freinage = 0.075;
//    __attribute__((near)) static double fenetre_angle_arrivee = 5 * Pi / 180. * ENTRAXE_TICKS; //0.1
//    
//    // on sauvegarde l'erreur de distance du cycle précédent avant que la valeur ne soit écrasé
//    // Par le calcul de la fonction_PID(ASSERV_POSITION)
//    erreur_angle_precedent = ERREUR_ORIENTATION.actuelle;
//    erreur_de_suivie = abs(VITESSE[SYS_ROBOT].theorique - VITESSE[SYS_ROBOT].actuelle);
//    
//    // Calcul de l'angle restant à parcourir
//    angle_restant = fonction_PID(ASSERV_ORIENTATION);
//
//    // Génération des vittesses consignes à atteindre (haut du trapèze))
//    if (angle_restant > 0.)
//    {
//        VITESSE_ORIENTATION[SYS_ROBOT].consigne =  VITESSE_MAX.orientation;
//        FLAG_ASSERV.sens_rotation = ROTATION_POSITIVE;
//    }
//    else if (angle_restant < 0.)
//    {
//        VITESSE_ORIENTATION[SYS_ROBOT].consigne = - VITESSE_MAX.orientation;
//        FLAG_ASSERV.sens_rotation = ROTATION_NEGATIVE;
//        
//        // ensure the angle is always positive
//        angle_restant *= -1;
//    }
//
//    // calcul de l'angle théorique de freinage (trapèze)
//    angle_freinage = (VITESSE_ORIENTATION[SYS_ROBOT].actuelle * VITESSE_ORIENTATION[SYS_ROBOT].actuelle) / (2. * acc.deceleration.orientation.consigne);
//
//    // Recalcul de l'angle de freinage par anticipation à rajouter à l'angle 
//    // de freinage tant qu'on est pas rentré dans la phase de freinage 
//    if (FLAG_ASSERV.phase_decelaration_orientation != EN_COURS)
//    {
//        angle_freinage_anticipation = angle_freinage * coef_angle_freinage;
//    }
//
//    // Si le robot doit freiner       (distance de freinage + distance parcouru en 1 coup)
//    if (angle_restant <= (angle_freinage + angle_freinage_anticipation)) 
//    {
//        FLAG_ASSERV.phase_decelaration_orientation = EN_COURS;
//        VITESSE_ORIENTATION[SYS_ROBOT].consigne = 0.;
//    }
//
//    //si on se trouve dans un cercle de 3 cm autour du point d'arrivé
//    if (angle_restant < fenetre_angle_arrivee) //30
//    {
//
//        // on ajuste la pente de freinage à quand on est proche de la cible 
//        // s'assurer d'arriver avec une v=0 au bon endroit
//        if (angle_restant < angle_freinage_anticipation) 
//        {
//            acc.deceleration.orientation.consigne = (VITESSE_ORIENTATION[SYS_ROBOT].actuelle * VITESSE_ORIENTATION[SYS_ROBOT].actuelle) / (2. * (angle_restant));
//        }
//
//        // Si le robot est immobile
//        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique == 0 && FLAG_ASSERV.fin_deplacement != DEBUT_DEPLACEMENT)
//        {
//            VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;
//            FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
//            FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
//        }
//
//
//        // la vitesse = distance parcouru en 1 cycle d'asserv
//        // Si la distance restante < distance_parcouru au cycle n +1
//        // distance parcouru au cycle n+1 = (Vactu + Vactu+1)/2
//        // (on moyenne la vitesse, et une vitesse = distance parcouru en un cycle)
//        // vrai uniquement en phase de decel ...
//        // TODO : anticipation sur plus de cycle ?
////            {
////                double nextVActu = VITESSE[SYS_ROBOT].actuelle * FLAG_ASSERV.sens_deplacement - acc.deceleration.position.consigne;
////                if (nextVActu < 0.)
////                    nextVActu = 0.;
////     
////                if ( (distance_restante + erreur_de_suivie) < ((VITESSE[SYS_ROBOT].actuelle * FLAG_ASSERV.sens_deplacement + nextVActu) / 2.) )
////                {
////                    fin_deplacement_avec_brake();
////                }
////            }
//
////            // On aura dépassé la position consigne au prochain coup 
////            if ( (distance_restante + erreur_de_suivie) < abs(( VITESSE[SYS_ROBOT].actuelle - acc.deceleration.position.consigne) / 2.) )
////            {
////                fin_deplacement_avec_brake();
////            }
//
//        // on s'éloigne de la cible 
//        if ( (((ERREUR_ORIENTATION.actuelle - erreur_angle_precedent) * FLAG_ASSERV.sens_rotation) > 0 ) && FLAG_ASSERV.fin_deplacement != DEBUT_DEPLACEMENT)
//        {
//            VITESSE_ORIENTATION[SYS_ROBOT].theorique = 0.;
//            FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
//            FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
//        }
//    }          
}


//Fonction qui génère les rampes de Vitesse pour la rotation du robot
void asserv_vitesse_orientation (void)
{
    //Si on a attaqué la phase de décélération, on ne permet plus de réaccélérer
    
    //if (FLAG_ASSERV.phase_decelaration_orientation == PHASE_NORMAL || VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0. )
    {     
        // acceleration positive et décélération négative
        if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < VITESSE_ORIENTATION[SYS_ROBOT].consigne)
        {
            VITESSE_ORIENTATION[SYS_ROBOT].theorique += acc.acceleration.orientation.consigne;
        }
        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_ORIENTATION[SYS_ROBOT].consigne)
        {
            VITESSE_ORIENTATION[SYS_ROBOT].theorique -= acc.deceleration.orientation.consigne;
        }

        saturation_vitesse_max(ASSERV_ORIENTATION);
    
        VITESSE[ROUE_DROITE].consigne += FLAG_ASSERV.sens_rotation * VITESSE_ORIENTATION[SYS_ROBOT].theorique;
        VITESSE[ROUE_GAUCHE].consigne -= FLAG_ASSERV.sens_rotation * VITESSE_ORIENTATION[SYS_ROBOT].theorique;
     }
//    //Si on a attaqué la phase de décélération, on ne permet plus de réaccélérer
//    if (FLAG_ASSERV.phase_decelaration_orientation == PHASE_NORMAL || VITESSE_ORIENTATION[SYS_ROBOT].consigne == 0. )
//    {
//         if (VITESSE_ORIENTATION[SYS_ROBOT].theorique < VITESSE_ORIENTATION[SYS_ROBOT].consigne)
//            VITESSE_ORIENTATION[SYS_ROBOT].theorique += acc.acceleration.orientation.consigne;
//
//
//        else if (VITESSE_ORIENTATION[SYS_ROBOT].theorique > VITESSE_ORIENTATION[SYS_ROBOT].consigne)
//            VITESSE_ORIENTATION[SYS_ROBOT].theorique -= acc.deceleration.orientation.consigne;
//
//         saturation_vitesse_max(ASSERV_ORIENTATION);
//
//         //débloquage de la l'interdiction d'acceleration si on arrive à une consigne nulle pour enlever l'erreur statiqye
//        // if (VITESSE_ORIENTATION[SYS_ROBOT].theorique == 0)
//        //     FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;
//    }
//
//     VITESSE[ROUE_DROITE].consigne += VITESSE_ORIENTATION[SYS_ROBOT].theorique;
//     VITESSE[ROUE_GAUCHE].consigne -= VITESSE_ORIENTATION[SYS_ROBOT].theorique;
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
        
//        ERREUR_VITESSE[SYS_ROBOT].actuelle = (ERREUR_VITESSE[ROUE_DROITE].actuelle + ERREUR_VITESSE[ROUE_GAUCHE].actuelle)/2.;
        
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
//        ERREUR_DISTANCE.integralle += ERREUR_DISTANCE.actuelle;

         duty =  ERREUR_DISTANCE.actuelle;// * PID.DISTANCE.KP  + ERREUR_DISTANCE.integralle * PID.DISTANCE.KI - (ERREUR_DISTANCE.actuelle - ERREUR_DISTANCE.precedente) * PID.DISTANCE.KD;
 
//        ERREUR_DISTANCE.precedente = ERREUR_DISTANCE.actuelle;

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

        while (ERREUR_ORIENTATION.actuelle >= (Pi * ENTRAXE_TICKS/2.))
            ERREUR_ORIENTATION.actuelle -=  Pi * ENTRAXE_TICKS;
        while (ERREUR_ORIENTATION.actuelle <= - Pi * ENTRAXE_TICKS/2.)
            ERREUR_ORIENTATION.actuelle +=  Pi * ENTRAXE_TICKS;

//        ERREUR_ORIENTATION.integralle += ERREUR_ORIENTATION.actuelle;
//        if (ERREUR_ORIENTATION.integralle > Pi * ENTRAXE_TICKS/2.)
//            ERREUR_ORIENTATION.integralle = Pi * ENTRAXE_TICKS/2.;
//        else if (ERREUR_ORIENTATION.integralle < -Pi * ENTRAXE_TICKS/2.)
//            ERREUR_ORIENTATION.integralle  = -Pi * ENTRAXE_TICKS/2.;

        duty =  ERREUR_ORIENTATION.actuelle; // * PID.ORIENTATION.KP + ERREUR_ORIENTATION.integralle * PID.ORIENTATION.KI - (ERREUR_ORIENTATION.actuelle - ERREUR_ORIENTATION.precedente) * PID.ORIENTATION.KD;
//        ERREUR_ORIENTATION.precedente = ERREUR_ORIENTATION.actuelle;

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
                    KP_hybride *= 0.5; //0.2
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
    __attribute__((near)) static double delta_o, delta_d;
    __attribute__((near)) static double d_X = 0., d_Y = 0.;

    get_valeur_codeur (CODEUR_D);
    get_valeur_codeur (CODEUR_G);

    POSITION[CODEUR_D].ecart *= COEF_D;
    POSITION[CODEUR_G].ecart *= COEF_G;

    //calcul des modifs
    delta_o = (POSITION[CODEUR_D].ecart - POSITION[CODEUR_G].ecart) /2.;
    delta_d = (POSITION[CODEUR_D].ecart + POSITION[CODEUR_G].ecart) /2.;

    if (FLAG_ASSERV.brake == ON)
    {
        BRAKE[ROUE_DROITE].actuelle -= POSITION[CODEUR_D].ecart;
        BRAKE[ROUE_GAUCHE].actuelle -= POSITION[CODEUR_G].ecart;
    }

    //cumul des valeurs pour l'asserv
    DISTANCE.actuelle += delta_d;

    VITESSE[SYS_ROBOT].actuelle = delta_d;
    VITESSE[ROUE_DROITE].actuelle = POSITION[CODEUR_D].ecart;
    VITESSE[ROUE_GAUCHE].actuelle = POSITION[CODEUR_G].ecart;

    VITESSE_ORIENTATION[SYS_ROBOT].actuelle = delta_o;

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


