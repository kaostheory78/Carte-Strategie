#include "system.h"


//l'asserv c'est cool =D=D=D

/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

_robot ROBOT;

_PID PID_DISTANCE;
_PID PID_ORIENTATION;

_systeme_asserv DISTANCE;
_systeme_asserv ORIENTATION;
_systeme_asserv VITESSE;

_erreur ERREUR_DISTANCE;
_erreur ERREUR_ORIENTATION;

_commande_moteur COMMANDE;

_flag_asserv FLAG_ASSERV;

void init_flag()
{
    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.orientation = OFF;

    VITESSE.acceleration = 0.5;

    DISTANCE.consigne = 500 *  TICKS_PAR_MM;
    reinit_asserv();

    //PutsUART(UART_XBEE, "demarage asserv \n\r");
    //delay_ms(500);

    FLAG_ASSERV.totale = ON;
}

void init_position_robot (float x0, float y0, uint32_t teta0)
{
    //ROBOT.X_point = x0;
    //ROBOT.Y_point = y0;
    ROBOT.orientation = teta0;
    //ROBOT.orientation_precedante = teta0;
    //ROBOT.distance = 0;
    //ROBOT.distance_precedante = 0;
}

void init_commande_moteur(void)
{
    COMMANDE.droit = 0;
    COMMANDE.gauche = 0;
}

void ecretage_consignes(void)
{
    if (COMMANDE.droit > 100)
        COMMANDE.droit = 100;
    else if (COMMANDE.droit < - 100)
        COMMANDE.droit = -100;

    if (COMMANDE.gauche > 100) //100
        COMMANDE.gauche = 100;
    else if (COMMANDE.gauche < -100)
        COMMANDE.gauche = -100;
}

void reinit_asserv(void)
{
    DISTANCE.actuelle = 0;
    DISTANCE.theorique = 0;
    DISTANCE.acceleration = 0.; // 0.0000001
    ORIENTATION.actuelle = 0;

    ERREUR_DISTANCE.actuelle = 0;
    ERREUR_DISTANCE.integralle = 0;
    ERREUR_DISTANCE.precedente = 0;
}

void asserv_distance(void)
{
    static int32_t distance_restante = 0;
    static int32_t compteur  = 0;


    compteur ++;
    distance_restante = DISTANCE.consigne - DISTANCE.actuelle;

    if (distance_restante > 1 * SEUIL_DISTANCE_MINI_PAS) //arrivé à la position
    {
        if (distance_restante > 0) //distance restante > 0
        {
            VITESSE.consigne = 100; //vmax
        }
        else if (distance_restante < 0)
        {
            VITESSE.consigne = -100; //Vmin
        }

        if ( (VITESSE.theorique / VITESSE.acceleration ) > (distance_restante / VITESSE.theorique))
        //if (distance_restante < 10 * SEUIL_DISTANCE_MINI_PAS )
            VITESSE.consigne = 0;
    }
   else
   {
        VITESSE.consigne = FIN_DEPLACEMENT;

        envoit_pwm(MOTEUR_DROIT, 0);
        envoit_pwm(MOTEUR_GAUCHE, 0);
        FLAG_ASSERV.totale = OFF;
    }
        
}

void asserv_vitesse (void)
{
    if (VITESSE.theorique < VITESSE.consigne)
        VITESSE.theorique += VITESSE.acceleration;

    else if (VITESSE.theorique > VITESSE.consigne)
        VITESSE.theorique -= VITESSE.acceleration;

    if (VITESSE.consigne == FIN_DEPLACEMENT)
        VITESSE.theorique = 0;
}

void asserv_orientation (void)
{
    
}

void asserv()
{
    //calcul_position_robot();
    static int test = 0;

    if (FLAG_ASSERV.position == ON)
        asserv_distance();
    if(FLAG_ASSERV.vitesse == ON)
        asserv_vitesse();
    if(FLAG_ASSERV.orientation == ON)
        asserv_orientation();

    DISTANCE.theorique += VITESSE.theorique;
    ERREUR_DISTANCE.actuelle = DISTANCE.theorique - DISTANCE.actuelle;

   /* if (DISTANCE.actuelle <  100000    && test == 0)
    {
        VITESSE.theorique += VITESSE.acceleration;

    }
    else
    {
        //delay_ms(3000);
        test = 1;
        VITESSE.theorique -= VITESSE.acceleration;
    }*/

    //VITESSE.theorique += VITESSE.acceleration;
        

    init_commande_moteur();
    PID_distance();
    ecretage_consignes();

    envoit_pwm(MOTEUR_DROIT, COMMANDE.droit);
    envoit_pwm(MOTEUR_GAUCHE, COMMANDE.gauche);

}

void PID_distance (void)
{
    static double duty;


    duty = VITESSE.theorique; // ERREUR_DISTANCE.actuelle * PID_DISTANCE.KP / 100 + ERREUR_DISTANCE.integralle * PID_DISTANCE.KI / 10000 + (ERREUR_DISTANCE.actuelle - ERREUR_DISTANCE.precedente) * PID_DISTANCE.KD / 1000;

    COMMANDE.droit += duty;
    COMMANDE.gauche += duty;

    ERREUR_DISTANCE.integralle += ERREUR_DISTANCE.actuelle;
    ERREUR_DISTANCE.precedente = ERREUR_DISTANCE.actuelle;
}

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
    VITESSE.actuelle = (double) delta_d;
    ORIENTATION.actuelle += delta_o;

    //calul de l'angle en pas
    alpha_ticks += delta_o;

    ROBOT.orientation = (double) alpha_ticks / (ENTRAXE_TICKS / 2);
    ROBOT.orientation_degre = ROBOT.orientation * 180 / Pi;

    //Calcul des positions
    d_X = (double) cos (ROBOT.orientation) * delta_d;
    d_Y = (double) sin (ROBOT.orientation) * delta_d;

    ROBOT.X_mm += d_X * MM_PAR_TICKS;
    ROBOT.Y_mm += d_Y * MM_PAR_TICKS;
    /*********** Code RCVA ***************************************************
     *
     *
     *
     *  double delta_x, delta_y, orient_moy ;
    double  delta_d, delta_orient;
    double orient_moy_radian, delta_orient_radian;

    static uint32_t COEF_D =  (perimetre_roue_codeuse * (CODEUR_D_NB_IMP_LOG - 1));
    static uint32_t COEF_ROT  = (perimetre_roue_codeuse * distance_roues / 2 ) / (CODEUR_D_NB_IMP_LOG - 1);

    static uint32_t CONV_RADIAN   = (Pi / (180 *(perimetre_roue_codeuse * distance_roues / 2 ) / (CODEUR_D_NB_IMP_LOG - 1)));

     *
    delta_orient =  position[CODEUR_D].ecart - position[CODEUR_G].ecart;
    ROBOT.orientation += ROBOT.orientation_init + (position[CODEUR_D].ecart - position[CODEUR_G].ecart);

    delta_d = ROBOT.distance - ROBOT.distance_precedante;
    delta_orient = ROBOT.orientation - ROBOT.distance_precedante;

    orient_moy = ( ROBOT.orientation + ROBOT.orientation_precedante) / 2;

    delta_orient_radian = CONV_RADIAN * delta_orient;
    orient_moy_radian = CONV_RADIAN * orient_moy;


    delta_x = delta_d * cos (orient_moy_radian);
    delta_y = delta_d * sin (orient_moy_radian);

    ROBOT.X_point += delta_x;
    ROBOT.Y_point += delta_y;

    ROBOT.X_mm = ( ROBOT.X_point * COEF_D);
    ROBOT.Y_mm =  (ROBOT.Y_point * COEF_D);

    ROBOT.orientation_radian = CONV_RADIAN * ROBOT.orientation;

    ROBOT.orientation_precedante = ROBOT.orientation;
    ROBOT.distance_precedante = ROBOT.distance;
     *
     * ***********************************************************************/


    /*delta_orient =  ((position[CODEUR_D].ecart - position[CODEUR_G].ecart)/distance_roues);
    delta_d = (position[CODEUR_D].ecart + position[CODEUR_G].ecart)/2;

    ROBOT.orientation +=  delta_orient;
    ROBOT.distance += delta_d;

    delta_x = delta_d * cos (ROBOT.orientation + delta_orient/2);
    delta_y = delta_d * sin (ROBOT.orientation + delta_orient/2);

    ROBOT.X_point += delta_x;
    ROBOT.Y_point += delta_y;

    ROBOT.X_mm = ( ROBOT.X_point * perimetre_roue_codeuse / CODEUR_D_NB_IMP_LOG);
    ROBOT.Y_mm =  (ROBOT.Y_point * perimetre_roue_codeuse / CODEUR_D_NB_IMP_LOG);*/

    
}


