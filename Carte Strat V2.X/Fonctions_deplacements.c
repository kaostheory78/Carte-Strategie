/******************************************************************************/
/************** Carte polyvalente Botik  : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: fonctions_deplacements.c
    * Auteur  	: Quentin
    * Revision	: 1.0
    * Date		: 14 décembre 2014, 14:29
    *******************************************************************************
    *
    *
******************************************************************************/


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/**************************** FONCTIONS COULEURS ******************************/
/******************************************************************************/

uint8_t couleur_depart()
{
    if (SYS_COULEUR == 0)
        return JAUNE;
    else
        return VERT;
}

double inversion_couleur (double param_inversable)
{
    if (COULEUR == JAUNE)
        return param_inversable;
    else
        return (- param_inversable);
}

/******************************************************************************/
/********************* FONCTIONS USUELLES (TEST ASSERV) ***********************/
/******************************************************************************/

void trapeze (int8_t sens_marche)
{
    //trapèze type 8
    //    passe_part (1000, 500, sens_marche, 50, DEBUT_TRAJECTOIRE);
    //    passe_part (1000, 0  , sens_marche, 50, MILIEU_TRAJECTOIRE);
    //    passe_part (0   , 500, sens_marche, 50, MILIEU_TRAJECTOIRE);
    //    passe_part (0   , 0  , sens_marche, 50, FIN_TRAJECTOIRE);
    rejoindre (1000, 500, sens_marche, 100);
    rejoindre (1000, 0  , sens_marche, 100);
    rejoindre (0   , 500, sens_marche, 100);
    rejoindre (0   , 0  , sens_marche, 100);
    orienter(0, 100);
}

void petit_demi_cercle (int8_t sens_marche)
{
    //petit demi cercle
    passe_part (200, 0,    sens_marche, 300, DEBUT_TRAJECTOIRE);
    passe_part (450, 67,   sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 250,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (700, 500,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (633, 750,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (450, 933,  sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (200, 1000, sens_marche, 100, FIN_TRAJECTOIRE);
}

void grand_demi_cercle (int8_t sens_marche)
{
    passe_part (200 , 0   , sens_marche, 500, DEBUT_TRAJECTOIRE);
    passe_part (700 , 134 , sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (1066, 500 , sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (1200, 1000, sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (1066, 1500, sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (700 , 1866, sens_marche, 100, MILIEU_TRAJECTOIRE);
    passe_part (200 , 2000, sens_marche, 100, FIN_TRAJECTOIRE);
}

void carre (int8_t sens_marche)
{
    rejoindre (500, 0  , sens_marche, 100);
    rejoindre (500, 500, sens_marche, 100);
    rejoindre (0  , 500, sens_marche, 100);
    rejoindre (0  , 0  , sens_marche, 100);
}

/******************************************************************************/
/******************* FONCTIONS ASSERV HAUT NIVEAU (EVITEMENT) *****************/
/******************************************************************************/


void action_evitement (void)
{

}

void cibler (double x, double y, double pourcentage_vitesse)
{
    uint8_t erreur = _cibler (x, y, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (EVITEMENT_ADV.mode == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (EVITEMENT_ADV.mode == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
            cibler(x,y,pourcentage_vitesse);
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

void orienter (double angle, double pourcentage_vitesse)
{
    uint8_t erreur = _orienter (angle, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (EVITEMENT_ADV.mode == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (EVITEMENT_ADV.mode == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
            orienter ( angle, pourcentage_vitesse);
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

void rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse)
{
    uint8_t erreur = _rejoindre (x, y, sens_marche, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (EVITEMENT_ADV.mode == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (EVITEMENT_ADV.mode == EVITEMENT_NORMAL)
        {
            plus_court(x,y,sens_marche,pourcentage_vitesse,rej,1);
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }    
}

void avancer_reculer (double distance, double pourcentage_vitesse)
{
    uint8_t erreur = _avancer_reculer (distance, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (EVITEMENT_ADV.mode == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (EVITEMENT_ADV.mode == EVITEMENT_NORMAL)
        {
            //action en cas d'évitements
        }
    }
    else if (erreur == BLOCAGE)
    {

        //Actions a faire en cas de blocage
    }
}

void passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last)
{
    uint8_t erreur = _passe_part (x, y, sens_marche, pourcentage_vitesse, last);
    if ( erreur == EVITEMENT)
    {
        if (EVITEMENT_ADV.mode == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (EVITEMENT_ADV.mode == EVITEMENT_NORMAL)
        {

            plus_court(x,y,sens_marche,pourcentage_vitesse,last,1);
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}

/******************************************************************************/
/******************* FONCTIONS ASSERV BAS NIVEAU (calage) *********************/
/******************************************************************************/

void calage (double distance, double pourcentage_vitesse)
{
    uint8_t erreur = _calage (distance, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        // Evitement sur un calage ?
    }
    else if (erreur == BLOCAGE)
    {
        // Mode de sortie normal pour un calage
    }
}

void calage_X (double x, double teta, int8_t sens_marche, double pourcentage_vitesse)
{
    // calage sur 1m pour limiter la vitesse
    uint8_t erreur = _calage (sens_marche * 1000, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        // Eviitement sur un calage ?
    }
    else if (erreur == BLOCAGE)
    {
        init_X(x);
        init_orientation(teta);
    }
    else //si pas encore calé on recommence
        calage_X (x, teta, sens_marche, pourcentage_vitesse);
}

void calage_Y (double y, double teta, int8_t sens_marche, double pourcentage_vitesse)
{
    // calage sur 1m pour limiter la vitesse
    uint8_t erreur = _calage (sens_marche * 1000, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        // Evitement sur un calage ?
    }
    else if (erreur == BLOCAGE)
    {
        init_Y(y);
        init_orientation(teta);
    }
    else //si pas encore calé on recommence
        calage_Y (y, teta, sens_marche, pourcentage_vitesse);
}

void calage_teta (double teta, int8_t sens_marche, double pourcentage_vitesse)
{
    // calage sur 1m pour limiter la vitesse
    uint8_t erreur = _calage (sens_marche * 1000, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        // Evitement sur un calage ?
    }
    else if (erreur == BLOCAGE)
    {
        init_orientation(teta);
    }
    else //si pas encore calé on recommence
        calage_teta (teta, sens_marche, pourcentage_vitesse);
}

/******************************************************************************/
/***************** FONCTIONS ASSERV BAS NIVEAU (config asserv) ****************/
/******************************************************************************/


_enum_erreur_asserv _calage (double distance, double pourcentage_vitesse)
{
    FLAG_ASSERV.brake = OFF;
    reinit_asserv();
	
    FLAG_ASSERV.type_consigne = MM;
	
    DISTANCE.consigne = distance * (TICKS_PAR_MM);

    if (distance < 0.)
        FLAG_ASSERV.sens_deplacement = MARCHE_ARRIERE;
    else
        FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;
    
    EVITEMENT_ADV.sens = FLAG_ASSERV.sens_deplacement;
	
    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();
	
    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.vitesse_fin_nulle = ON;

    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.immobilite = 0;
    PID.VITESSE_DIS.max_I = MAX_ERREUR_INTEGRALLE_V / 4;
    PID.VITESSE_DIS.seuil_immobilite = SEUIL_IMMOBILITE /4;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}


_enum_erreur_asserv _cibler (double x, double y, double pourcentage_vitesse)
{
    y = inversion_couleur(y);
    FLAG_ASSERV.brake = OFF;
    //delay_ms(10);

    reinit_asserv();

    x *= TICKS_PAR_MM;
    y *= TICKS_PAR_MM;

    FLAG_ASSERV.type_consigne = MM;

    if ((y - Y.actuelle) != 0 || (x - X.actuelle) != 0 )//&& (Y.consigne - Y.actuelle) != 0)
    {
        ORIENTATION.consigne =  atan2((y - Y.actuelle), (x - X.actuelle)) * ENTRAXE_TICKS/2 ;

        calcul_vitesse_orientation(pourcentage_vitesse);
        calcul_acceleration_orientation();

        FLAG_ASSERV.position = OFF;
        FLAG_ASSERV.orientation = ON;
        FLAG_ASSERV.vitesse = ON;

        FLAG_ASSERV.type_deplacement = ORIENTER;

        FLAG_ASSERV.etat_angle = EN_COURS;
        FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
        FLAG_ASSERV.immobilite = 0;

        FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

        while(FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    }
    return FLAG_ASSERV.erreur;
}

_enum_erreur_asserv _orienter (double angle, double pourcentage_vitesse)
{
    angle = inversion_couleur(angle);
    FLAG_ASSERV.brake = OFF;
    //delay_ms(10);

    reinit_asserv();

    FLAG_ASSERV.type_consigne = MM;

    ORIENTATION.consigne = (angle * Pi)/ 180 * (ENTRAXE_TICKS/2);

    calcul_vitesse_orientation(pourcentage_vitesse);
    calcul_acceleration_orientation();

    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.type_deplacement = ORIENTER;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;

    while(FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

void faire_des_tours (int nb_tour)
{
    int i;
    double j;

    if (nb_tour < 0)
    {
        nb_tour *= -1;
        j = -1.;
    }
    else
        j= 1.;

    _fdt (120 * j  , DEBUT_TRAJECTOIRE);
    for (i = 0 ; i < nb_tour - 1 ; i++)
    {
        _fdt(- 120 * j, MILIEU_TRAJECTOIRE);
        _fdt (0,  MILIEU_TRAJECTOIRE);
        _fdt (120 * j, MILIEU_TRAJECTOIRE);
    }

    _fdt (- 120 * j, MILIEU_TRAJECTOIRE);
    _fdt (0, FIN_TRAJECTOIRE);
}

void _fdt (double angle, char last)
{
    if (last == DEBUT_TRAJECTOIRE)
    {
        FLAG_ASSERV.brake = OFF;
        // delay_ms(10);
        reinit_asserv();

        FLAG_ASSERV.type_consigne = MM;
    }

    angle = inversion_couleur(angle);
    
    ORIENTATION.consigne = (angle * Pi)/ 180 * (ENTRAXE_TICKS/2);
    
    printf("_fdt : angle = %lf, Orientation = %lf\n\r", angle, ORIENTATION.consigne);

#ifdef PETIT_ROBOT
    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation.consigne = acc.acceleration.orientation.max;
    acc.deceleration.orientation.consigne = acc.deceleration.orientation.max;
#endif
#ifdef GROS_ROBOT
    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS;
    calcul_acceleration_orientation();
#endif

    FLAG_ASSERV.position = OFF;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.phase_deceleration_distance = PHASE_NORMAL;
    FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;

    FLAG_ASSERV.type_deplacement = FAIRE_DES_TOURS;
    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = DISTANCE_ATTEINTE;

    FLAG_ASSERV.immobilite = 0;

    if (last == FIN_TRAJECTOIRE)
        FLAG_ASSERV.vitesse_fin_nulle = ON;
    else
        FLAG_ASSERV.vitesse_fin_nulle = OFF;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
}

_enum_erreur_asserv _rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse)
{
    EVITEMENT_ADV.sens = sens_marche;
    y = inversion_couleur(y);
    FLAG_ASSERV.brake = OFF;
    //delay_ms(10);

    reinit_asserv();

    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    FLAG_ASSERV.type_consigne = XY;

    FLAG_ASSERV.sens_deplacement = sens_marche;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    
#ifdef PETIT_ROBOT
    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation.consigne = acc.deceleration.orientation.max;
    acc.deceleration.orientation.consigne = acc.deceleration.orientation.max;
#else
    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS / 2;
    calcul_acceleration_orientation();
#endif


    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.type_deplacement = AVANCER;
    FLAG_ASSERV.vitesse_fin_nulle = ON;

    FLAG_ASSERV.immobilite = 0;


    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

_enum_erreur_asserv _avancer_reculer (double distance, double pourcentage_vitesse)
{
    FLAG_ASSERV.brake = OFF;
    reinit_asserv();

    FLAG_ASSERV.type_consigne = MM;

    DISTANCE.consigne = distance * (TICKS_PAR_MM);

    if (distance < 0.)
        FLAG_ASSERV.sens_deplacement = MARCHE_ARRIERE;
    else
        FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;
    
    EVITEMENT_ADV.sens = FLAG_ASSERV.sens_deplacement;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.vitesse_fin_nulle = ON;

    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

_enum_erreur_asserv _passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last)
{
    EVITEMENT_ADV.sens = sens_marche;
    
    if (last == DEBUT_TRAJECTOIRE)
    {
        FLAG_ASSERV.brake = OFF;
        //  delay_ms(10);
        reinit_asserv();

        FLAG_ASSERV.type_consigne = XY;
    }

    y = inversion_couleur(y);
    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    VITESSE_MAX.orientation = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation.consigne = acc.acceleration.orientation.max;
    acc.deceleration.orientation.consigne = acc.deceleration.orientation.max;

    //FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = ON;
    FLAG_ASSERV.vitesse = ON;
    FLAG_ASSERV.sens_deplacement = sens_marche;
    FLAG_ASSERV.phase_deceleration_distance = PHASE_NORMAL;
    FLAG_ASSERV.phase_decelaration_orientation = PHASE_NORMAL;

    FLAG_ASSERV.type_deplacement = PASSE_PART;
    FLAG_ASSERV.etat_angle = EN_COURS;
    FLAG_ASSERV.etat_distance = EN_COURS;

    FLAG_ASSERV.immobilite = 0;

    if (last == FIN_TRAJECTOIRE)
        FLAG_ASSERV.vitesse_fin_nulle = ON;
    else
        FLAG_ASSERV.vitesse_fin_nulle = OFF;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*
    * repere du terrain => 0:0 en bas a gauche quand on fait face au robotmovie (jaune a gauche et vert à droite)
    * repere de la matrice => 0:0 en haut a gauche quand on fait face au robotmovie (jaune a gauche et vert à droite)
    *
    * direction et repere dans le terrain
    * 812 1 => y+1
    * 703 5 => y-1
    * 654 3 => x+1
    *
    *
    * direction dans la matrice
    *    654
    *    703
    *    812
    *
    * direction de longement : on se déplace suivant la flèche et on observe la case ou est le numéro
    * par exemple si on a 3, on ira vers le haut du terrain et on testera si notre gauche est libre
    * 1 | 2  3 ^ 4    6     8
    *   |      |    -->     <--
    *   V      |      5     7
    *

*/



void deplacement(int8_t sens_marche,double pourcentage_deplacement,char last) //transforme les points en commandes de deplacements
{
    int i=1;
    int id = id_evitement;

    #ifdef ORDI
        if(itineraire_court[i][0] != -1);
        cout << "cibler("<<(double)(itineraire_court[i][0]*100)+50<<","<<(double)((itineraire_court[i][1])*100)+50<<","<<pourcentage_deplacement<<")"<<endl;

        while(i<curseur && id == id_evitement && itineraire_court[i][0] != -1 )//affiche l'itineraire
        {
            if(last == rej)
            {
                cout << "rejoindre("<<(double)(itineraire_court[i][0]*100)+50<<","<<(double)((itineraire_court[i][1])*100)+50<<","<<sens_marche<<","<<pourcentage_deplacement<<")"<<endl;
            }
            else
            {
                if(i == 0)
                {
                    last = DEBUT_TRAJECTOIRE;
                }
                else
                {
                    last = MILIEU_TRAJECTOIRE;
                }
                cout << "passe_part("<<(double)(itineraire_court[i+1][0]*100)+50<<","<<(double)((itineraire_court[i+1][1])*100)+50<<","<<sens_marche<<","<<pourcentage_deplacement<<","<<last<<")"<<endl;
            }
            i++;
        }
        #else
        //permet le cibler sans probleme
        EVITEMENT_ADV.actif = false;
        EVITEMENT_ADV.detection = OFF;

        if(itineraire_court[i][0] != -1);
        cibler((double)(itineraire_court[i][0]*100)+50,(double)((itineraire_court[i][1])*100)+50,pourcentage_deplacement);
        

        //on remet l'evitement

        EVITEMENT_ADV.detection = OFF;
        EVITEMENT_ADV.actif = true;

        while(i<curseur && id == id_evitement && itineraire_court[i][0] != -1 )//affiche l'itineraire
        {
            if(last == rej && id == id_evitement)
            {

                rejoindre((double)(itineraire_court[i][0]*100)+50,(double)((itineraire_court[i][1])*100)+50,MARCHE_AVANT,pourcentage_deplacement);
            }
            else
            {
                if(i == 1)
                {
                    last = DEBUT_TRAJECTOIRE;
                }
                else
                {
                    last = MILIEU_TRAJECTOIRE;
                }
                if(id == id_evitement)
                {
                    passe_part((double)(itineraire_court[i+1][0]*100)+50,(double)((itineraire_court[i+1][1])*100)+50,MARCHE_AVANT,pourcentage_deplacement,last);

                }
            }
            i++;
        }


    #endif
}

int distance()
{
    int i=0;
    int dist = 0;
    int addition;
    while(i <curseur && compteur_evitement < ATTENTE_EVITEMENT)
    {
        addition = itineraire[i][0] - itineraire[i+1][0];
        if(addition < 0)
        addition = addition * -1;
        dist = dist + addition;
        addition = itineraire[i][1] - itineraire[i+1][1];
        if(addition < 0)
        addition = addition * -1;
        dist = dist + addition;
        i++;
    }
    return dist;
}

void init_evitement_hugo()
{
    int i,j;

    //le 0;0 de la matrice correspond au 0;y_max du terrain

    id_evitement = 0;
    id_evitement_initial = 0;
    curseur_obstacle=0;
    precedent_obstacle[0]=-1;
    precedent_obstacle[1]=-1;
    precedent_obstacle[2]=-1;
    precedent_obstacle[3]=-1;

    for (j=0; j<y_max; j++) // initialisation de la carte et de ses contours
    {
        for(i=0; i<x_max; i++)
        {
            #ifdef PETIT_ROBOT
                if(i<1 || i > x_max-2 || j<1 || j>y_max-2)
                obstacle[i][j]=1;
            #endif

            #ifdef GROS_ROBOT
                if(i<2 || i > x_max-3 || j<2 || j>y_max-3)
                obstacle[i][j]=1;
            #endif
            #ifdef CARTE_BALISE
                if(i<2 || i > x_max-3 || j<2 || j>y_max-3)
                obstacle[i][j]=1;
            #endif
            else
            obstacle[i][j]=0;
        }
    }


    obstacle[2][18]=1;
    obstacle[3][18]=1;//petit palet
    obstacle[4][18]=1;
    obstacle[5][18]=1;
    obstacle[6][18]=1;//petit palet bis
    obstacle[7][18]=1;


    obstacle[22][18]=1;
    obstacle[27][18]=1;//petit palet
    obstacle[26][18]=1;
    obstacle[25][18]=1;
    obstacle[24][18]=1;//petit palet bis
    obstacle[23][18]=1;


    #ifdef PETIT_ROBOT
        for(i=9; i<=20; i++) //escalier
        {
            for(j=12; j<=19; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif

    #ifdef GROS_ROBOT

        for(i=9; i<=20; i++) //escalier
        {
            for(j=11; j<=19; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif

    for(i=10; i<=19; i++) // marche rose
    {
        #ifdef PETIT_ROBOT
            for(j=0; j<=1; j++)
            {
                obstacle[i][j] =1;
            }
        #endif
        #ifdef GROS_ROBOT
            for(j=0; j<=2; j++)
            {
                obstacle[i][j] =1;
            }
        #endif
    }


    #ifdef PETIT_ROBOT
        for(i=0; i<=4; i++) //carre gauche haut
        {
            for(j=4; j<=7; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif
    #ifdef GROS_ROBOT
        for(i=0; i<=5; i++) //carre gauche haut
        {
            for(j=4; j<=8; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif

    #ifdef PETIT_ROBOT
        for(i=0; i<=4; i++) //carre gauche bas
        {
            for(j=12; j<=15; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif

    #ifdef GROS_ROBOT

        for(i=0; i<=5; i++) //carre gauche bas
        {
            for(j=11; j<=15; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif

    #ifdef PETIT_ROBOT
        for(i=23; i<=x_max; i++) //zone depart adverse
        {
            for(j=6; j<=13; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif
    #ifdef GROS_ROBOT
        for(i=22; i<=x_max; i++) //zone depart adverse
        {
            for(j=5; j<=14; j++)
            {
                obstacle[i][j]=1;
            }
        }
    #endif


}

int conversion_direction(int direction)//transforme la direction du blocage en une direction de longement
{
    switch (direction)
    {
        case 1://5 ou 7
        return 5;
        break;
        case 3://2 ou 4
        return 2;
        break;
        case 5://6 ou 8
        return 6;
        break;
        case 7://1 ou 3
        return 1;
        break;
    }
    return -1;
}

int longement ( int x_objectif,int y_objectif,int direction_longement) //longe un obstacle selon une direction de longement donnée
{
    int nombre_point_parcouru=0;

    switch (direction_longement)
    {
        case 1 :
        while (y_actuel+1 <= y_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel][y_actuel+1] == 0 //tant que l'on peut descendre
        && obstacle [x_actuel-1][y_actuel] != 0 //et que la gauche de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel+1; // on descend d'une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }

        if (obstacle [x_actuel-1][y_actuel] == 0 //si on peut aller a gauche, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 1; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=7;


                curseur = curseur +1;
                x_actuel=x_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point

            }
            return 0; // on a trouve un passage
        }
        else if((y_actuel == y_max-1 || obstacle [x_actuel][y_actuel+1] != 0) && obstacle [x_actuel+1][y_actuel] == 0)  //si on ne peut aller ni en bas ni a gauche mais a droite
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 1; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=3;


                curseur = curseur +1;
                x_actuel=x_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 5; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en haut, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


        case 2 :
        while (y_actuel+1 <= y_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel][y_actuel+1] == 0 //tant que l'on peut descendre
        && obstacle [x_actuel+1][y_actuel] != 0 //et que la droite de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel+1; // on descend d'une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel+1][y_actuel] == 0 //si on peut aller a droite, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 1; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=3;

                curseur = curseur +1;
                x_actuel=x_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 0; // on a trouve un passage
        }
        else if((y_actuel == y_max-1 ||obstacle [x_actuel][y_actuel+1] != 0 )&& obstacle [x_actuel-1][y_actuel] == 0)  //si on ne peut aller ni en haut ni a droite mais a gauche
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 1; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=7;

                curseur = curseur +1;
                x_actuel=x_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 7; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en haut, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


        case 3 :
        while (y_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel][y_actuel-1] == 0 //tant que l'on peut monter
        && obstacle [x_actuel-1][y_actuel] != 0 //et que la gauche de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel-1; // on monte d'une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel-1][y_actuel] == 0 //si on peut aller a gauche, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 5; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=7;

                curseur = curseur +1;
                x_actuel=x_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 0; // on a trouve un passage
        }
        else if((y_actuel == 0 || obstacle [x_actuel][y_actuel-1] != 0) && obstacle [x_actuel+1][y_actuel] == 0)  //si on ne peut aller ni en bas ni a gauche mais a droite
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 5; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=3;

                curseur = curseur +1;
                x_actuel=x_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 6; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en bas, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


        case 4 :
        while (y_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel][y_actuel-1] == 0 //tant que l'on peut monter
        && obstacle [x_actuel+1][y_actuel] != 0 //et que la droite de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel-1; // on monte d'une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }

        if (obstacle [x_actuel+1][y_actuel] == 0 //si on peut aller a droite, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 5; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=3;

                curseur = curseur +1;
                x_actuel=x_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 0; // on a trouve un passage
        }
        else if((y_actuel == 0 || obstacle [x_actuel][y_actuel-1] != 0) && obstacle [x_actuel-1][y_actuel] == 0)  //si on ne peut aller ni en bas ni a droite mais a gauche
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 5; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=7;

                curseur = curseur +1;
                x_actuel=x_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 8; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en bas, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


        case 5 :
        while (x_actuel +1 <=x_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel+1][y_actuel] == 0 //tant que l'on peut aller sur la droite
        && obstacle [x_actuel][y_actuel+1] != 0 //et que le bas de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel+1; // on va sur la droite de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }

        if (x_actuel == x_max)  // si on est en dehors de la matrice
        {

            return -1; // la recherche de passage de ce cote n'est pas fructueuse. peut être remplacé
        }

        else
        {
            if (obstacle [x_actuel][y_actuel+1] == 0 //si on peut aller en bas, si on a trouve un passage
            || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
            {
                if(nombre_point_parcouru!=0)
                {
                    itineraire [curseur][2] = 3; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                    curseur = curseur +1;
                    itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                    itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                    itineraire [curseur][2]=1;

                    curseur = curseur +1;
                    y_actuel=y_actuel+1;
                    itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                    itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                }
                return 0; // on a trouve un passage
            }
            else if((x_actuel == x_max-1 || obstacle [x_actuel+1][y_actuel] != 0 )&& obstacle [x_actuel][y_actuel-1] == 0)  //si on ne peut aller ni en haut ni a droite mais en bas
            {
                if(nombre_point_parcouru!=0)
                {
                    itineraire [curseur][2] = 3; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                    curseur = curseur +1;
                    itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                    itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                    itineraire [curseur][2]=5;

                    curseur = curseur +1;
                    y_actuel=y_actuel-1;
                    itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                    itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                }
                return 4; // on renvoie la direction de longement a essayer

            }
            else   // si on ne peut aller ni a gauche, ni en haut, ni a droite
            {
                return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
            }
        }
        break;


        case 6 :
        while (x_actuel +1 <=x_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel+1][y_actuel] == 0 //tant que l'on peut aller sur la droite
        && obstacle [x_actuel][y_actuel-1] != 0 //et que en haut de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel+1; // on va sur la droite de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel][y_actuel-1] == 0 //si on peut aller en haut, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 3; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=5;

                curseur = curseur +1;
                y_actuel=y_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 0; // on a trouve un passage
        }
        else if((x_actuel == x_max -1 || obstacle [x_actuel+1][y_actuel] != 0 )&& obstacle [x_actuel][y_actuel+1] == 0)  //si on ne peut aller ni en bas ni a droite mais en haut
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 3; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=1;

                curseur = curseur +1;
                y_actuel=y_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 2; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en haut, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


        case 7 :
        while (x_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel-1][y_actuel] == 0 //tant que l'on peut aller sur la gauche
        && obstacle [x_actuel][y_actuel+1] != 0 //et que en bas de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel-1; // on va sur la gauche de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel][y_actuel+1] == 0 //si on peut aller en bas, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 7; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=1;

                curseur = curseur +1;
                y_actuel=y_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 0; // on a trouve un passage
        }
        else if((x_actuel == 0 ||obstacle [x_actuel-1][y_actuel] != 0) && obstacle [x_actuel][y_actuel-1] == 0)  //si on ne peut aller ni en haut ni a gauche mais en bas
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 7; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=5;

                curseur = curseur +1;
                y_actuel=y_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 3; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en haut, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


        case 8 :
        while (x_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
        && obstacle [x_actuel-1][y_actuel] == 0 //tant que l'on peut aller sur la gauche
        && obstacle [x_actuel][y_actuel-1] != 0 //et que en haut de notre emplacement est condamnee
        && !(x_actuel == x_objectif && y_actuel == y_objectif)
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel-1; // on va sur la gauche de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel][y_actuel-1] == 0 //si on peut aller en haut, si on a trouve un passage
        || (x_actuel == x_objectif && y_actuel == y_objectif) )  // ou que l'on est arrive(pas sur de l'utilite)
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 7; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=5;

                curseur = curseur +1;
                y_actuel=y_actuel-1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 0; // on a trouve un passage
        }
        else if((x_actuel == 0 || obstacle [x_actuel-1][y_actuel] != 0) && obstacle [x_actuel][y_actuel+1] == 0)  //si on ne peut aller ni en bas ni a gauche mais en haut
        {
            if(nombre_point_parcouru!=0)
            {
                itineraire [curseur][2] = 7; // pour aller de la derniere case enregistree a la case presente, on doit prendre cette direction

                curseur = curseur +1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
                itineraire [curseur][2]=1;

                curseur = curseur +1;
                y_actuel=y_actuel+1;
                itineraire [curseur][0] = x_actuel; //coordonnee en x du nouveau point
                itineraire [curseur][1] = y_actuel; //coordonnee en y du nouveau point
            }
            return 1; // on renvoie la direction de longement a essayer

        }
        else   // si on ne peut aller ni a gauche, ni en haut, ni a droite
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;
    }
    return -1;
}

int tracer_ligne_x(int x_objectif) //cree une ligne droite suivant l'axe des x. Fais avancer ou reculer sur l'axe des x
{
    int nombre_point_parcouru=0;

    if(x_objectif>x_actuel)
    {
        while(x_actuel+1 <= x_max-1 //tant que l'on est dans la matrice
        && obstacle[x_actuel+1][y_actuel]==0 //tant que l'on peut aller a droite
        && x_objectif!=x_actuel
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive au niveau des x
        {
            x_actuel = x_actuel+1; // on va de 1 vers la droite
            nombre_point_parcouru=nombre_point_parcouru+1;
        }
        if(nombre_point_parcouru!=0)
        {
            itineraire[curseur][2]=3; // donne la direction pour aller de l'ancien point au nouveau point trouve
            curseur=curseur+1;
            itineraire[curseur][0]=x_actuel; //coordonnee en x du nouveau point
            itineraire[curseur][1]=y_actuel; //coordonnee en y du nouveau point
        }
        if(x_objectif==x_actuel)
        {
            return 0; //ok
        }
        else if(obstacle[x_actuel+1][y_actuel]!=0)
        {
            return 3; // retourne la direction du blocage
        }

    }
    else if (x_objectif<x_actuel)
    {
        while(x_actuel-1 >= 0 //tant que l'on est dans la matrice
        && obstacle[x_actuel-1][y_actuel]==0 //tant que l'on peut aller a gauche
        && x_objectif!=x_actuel
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive au niveau des x
        {
            x_actuel = x_actuel-1; // on va de 1 vers la gauche
            nombre_point_parcouru=nombre_point_parcouru+1;
        }
        if(nombre_point_parcouru!=0)
        {
            itineraire[curseur][2]=7; // donne la direction pour aller de l'ancien point au nouveau point trouve
            curseur=curseur+1;
            itineraire[curseur][0]=x_actuel; //coordonnee en x du nouveau point
            itineraire[curseur][1]=y_actuel; //coordonnee en y du nouveau point
        }

        if(x_objectif==x_actuel)
        {
            return 0; // pas de souci
        }
        else if(obstacle[x_actuel-1][y_actuel]!=0)
        {
            return 7;// retourne la direction du blocage
        }

    }
    return -1;
}

int tracer_ligne_y(int y_objectif) //cree une ligne droite suivant l'axe des y. Fais avancer ou reculer sur l'axe des y
{
    int nombre_point_parcouru=0;

    if(y_objectif>y_actuel)
    {
        while(y_actuel+1 <=y_max-1 //tant que l'on est dans la matrice
        && obstacle[x_actuel][y_actuel+1]==0 //tant que l'on peut aller en haut
        && y_objectif!=y_actuel
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive au niveau des y
        {
            y_actuel = y_actuel+1; // on va de 1 vers le haut
            nombre_point_parcouru=nombre_point_parcouru+1;
        }
        if(nombre_point_parcouru!=0)
        {
            itineraire[curseur][2]=1; // donne la direction pour aller de l'ancien point au nouveau point trouve
            curseur=curseur+1;
            itineraire[curseur][0]=x_actuel; //coordonnee en x du nouveau point
            itineraire[curseur][1]=y_actuel; //coordonnee en y du nouveau point
        }

        if(y_objectif==y_actuel)
        {
            return 0; // pas de souci
        }
        else if(obstacle[x_actuel][y_actuel+1]!=0)
        {
            return 1;// retourne la direction du blocage
        }

    }
    else if(y_objectif<y_actuel)
    {
        while(y_actuel-1 >= 0 //tant que l'on est dans la matrice
        && obstacle[x_actuel][y_actuel-1]==0 //tant que l'on peut aller en bas
        && y_objectif!=y_actuel
        && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive au niveau des y
        {
            y_actuel = y_actuel-1; // on va de 1 vers le bas
            nombre_point_parcouru=nombre_point_parcouru+1;
        }
        if(nombre_point_parcouru!=0)
        {
            itineraire[curseur][2]=5; // donne la direction pour aller de l'ancien point au nouveau point trouve
            curseur=curseur+1;
            itineraire[curseur][0]=x_actuel; //coordonnee en x du nouveau point
            itineraire[curseur][1]=y_actuel; //coordonnee en y du nouveau point
        }

        if(y_objectif==y_actuel)
        {
            return 0; // pas de souci
        }

        else if(obstacle[x_actuel][y_actuel-1]!=0)
        {
            return 5;// retourne la direction du blocage
        }

    }
    return -1;

}

void traitement_direction_identique()
{
    //post traitement 3) si deux directions consecutives sont les memes
    int i=0;
    int k=0;
    int flag;
    while(i<curseur && compteur_evitement < ATTENTE_EVITEMENT)
    {
        flag=0;//on a pu reduire le nombre de point de x points
        while (itineraire[i][2]==itineraire[i+1][2] && compteur_evitement < ATTENTE_EVITEMENT) //si deux directions consecutives ou plus
        {
            i++;
            flag++;
        }

        if(flag>0) // si on a eu le cas souhaité
        {
            //on décale les points
            for(k=i-flag+1; k<=curseur; k++)
            {
                itineraire[k][0]=itineraire[k+flag][0];
                itineraire[k][1]=itineraire[k+flag][1];
                itineraire[k][2]=itineraire[k+flag][2];

            }
            curseur=curseur-flag; //on réduit le nombre de point
            i=i-flag;// et on se place au même point sur le nouvel itinéraire

            if(i<0) // si on est parti trop loin en arrière
            {
                i=0; // on se remet au début
            }
        }
        i++;
    }
}

void traitement_deja_passe()
{
    int i,j,k;
    i=0;
    while(i<curseur+1 && compteur_evitement < ATTENTE_EVITEMENT) //s'occupe de quand on passe deux fois par le même point
    {
        j=curseur;

        //on part de la fin pour trouver la dernière occurence de ce point s'il y en a une
        while(j>i && compteur_evitement < ATTENTE_EVITEMENT)
        {
            if(itineraire[i][0]==itineraire[j][0] && itineraire[i][1]==itineraire[j][1]) //si on a deux fois le même couple de coordonnees
            {
                for(k=i; k<=curseur; k++)//on se met a la premiere apparition et on place les points après la seconde apparition
                {
                    itineraire[k][0]=itineraire[j+k-i][0];
                    itineraire[k][1]=itineraire[j+k-i][1];
                    itineraire[k][2]=itineraire[j+k-i][2];

                }
                curseur=curseur-(j-i);//on reduit le nombre de point pour rester cohérent
                break;
            }
            j--;
        }
        i++;
    }
}

void traitement_escalier()
{
    int i=0;
    int j,k;
    int direction_1 =0;
    int direction_2 =0;

    while (i+1<curseur)
    {
        //si on a une diagonale qui fait un point de distance (+-1/+-1)
        if ((itineraire[i][0]-itineraire[i+2][0] == 1 || itineraire[i][0]-itineraire[i+2][0] == -1) && (itineraire[i][1]-itineraire[i+2][1] == 1 || itineraire[i][1]-itineraire[i+2][1] == -1))
        {
            j=i+2;
            // on sauvegarde les deux directions empruntées
            direction_1 = itineraire[i][2];
            if(i+1 <= curseur)
            direction_2 = itineraire[i+1][2];

            //si on a encore des points, que nous sommes dans les mêmes directions que précédemment et que le déplacement est de 1 case verticale ou horizontale
            while (j+1<curseur && (itineraire[j][2] == direction_1 || itineraire [j][2] == direction_2)  && (((itineraire[j][0]-itineraire[j+1][0] == 1 || itineraire[j][0]-itineraire[j+1][0] == -1) && itineraire[j][1]-itineraire[j+1][1] == 0) || ((itineraire[j][1]-itineraire[j+1][1] == 1 || itineraire[j][1]-itineraire[j+1][1] == -1) && itineraire[j][0]-itineraire[j+1][0] == 0)))
            {
                j++; // on passe au point suivant
            }

            //cas spécifique pour les directions 1 et 7
            if((itineraire[i][2] == 1 && itineraire[i+1][2] == 7) || (itineraire[i][2] == 7 && itineraire[i+1][2] == 1))
            itineraire[i][2]=8;
            else
            itineraire[i][2] = direction_1 > direction_2 ? direction_1+1 : direction_2 +1; //en rajoutant 1 à la plus petite direction, on obtient la diagonale qui nous intéresse

            // on déplace les points qui n'ont pas été touchés
            for(k=0; j+k<=curseur; k++)
            {
                itineraire[i+1+k][0]=itineraire[j+k][0];
                itineraire[i+1+k][1]=itineraire[j+k][1];
                itineraire[i+1+k][2]=itineraire[j+k][2];
            }
            // et on remet à jour le nombre de point
            curseur =curseur - j + i + 1;

        }
        i++;
    }

}

void post_traitement() //simplifie le trajet en elevant le maximum de deplacements inutiles
{
    int i;
    // on sauvegarde notre itineraire de base
    for(i=0; i<=curseur; i++)
    {
        itineraire_basique[i][0]=itineraire[i][0];
        itineraire_basique[i][1]=itineraire[i][1];
        itineraire_basique[i][2]=itineraire[i][2];
    }

    traitement_deja_passe();
    traitement_direction_identique();
    traitement_escalier();

}

int aiguillage_evitement(int x_objectif, int y_objectif, int direction,int haut)  // evitement par le haut ou par le bas
{
    int direction_longement = 0;
    int cul_de_sac = 0;
    int curseur_initial = curseur;
    int obstacle_initial = curseur_obstacle;
    int i;
    int test = 0;

    while(test <=1 && compteur_evitement < ATTENTE_EVITEMENT)
    {


        if(curseur_obstacle >= nb_point_max)
        {
            return -1;
        }

        curseur=curseur_initial;
        curseur_obstacle=obstacle_initial;
        if(haut==1)
        {
            direction_longement = conversion_direction(direction);//on passe de la direction du bloquage a la direction de longement
            cul_de_sac=0;
            while(direction_longement >0 && cul_de_sac == 0 && curseur < nb_point_max-1 && compteur_evitement < ATTENTE_EVITEMENT) //tant que l'on est bloque
            {

                direction_longement = longement(x_objectif,y_objectif,direction_longement);//on longe les obstacles
                for(i=0; i<curseur_initial; i++)
                {
                    if(i != curseur - 1 && itineraire[i][0] == itineraire[curseur-1][0] && itineraire[i][1] == itineraire[curseur-1][1] && itineraire[i][2] == itineraire[curseur-1][2] ) // on regarde si on est pas repasse par le meme point et la meme direction
                    {
                        cul_de_sac = -1;
                        if(chemin_court == 0)
                        {
                            haut =0;
                            test++;
                        }
                        else
                        {
                            test =2;
                        }
                    }

                }


            }
        }
        else if(direction_longement ==-1 || haut == 0) //si on a atteint un cul de sac ou on veut aller pour le haut
        {

            direction_longement = conversion_direction(direction);//on reprend la valeur de longement de depart
            if(direction_longement != -1)
            direction_longement=direction_longement+2; //on lui rajoute 1. Lors d'un blocage, on peut aller dans 2 directions. On tente ici la seconde direction
            cul_de_sac=0;
            while(direction_longement >0 && cul_de_sac == 0 && curseur < nb_point_max-1 && compteur_evitement < ATTENTE_EVITEMENT) //tant que l'on est bloque
            {
                direction_longement = longement(x_objectif,y_objectif,direction_longement);//on longe les obstacles
                for(i=0; i<curseur_initial; i++)
                {
                    if(i!= curseur -1 && itineraire[i][0] == itineraire[curseur-1][0] && itineraire[i][1] == itineraire[curseur-1][1] && itineraire[i][2] == itineraire[curseur-1][2] )// on regarde si on est pas repasse par le meme point et la meme direction
                    {
                        cul_de_sac = -1;
                        if(chemin_court == 0)
                        {
                            haut =1;
                            test++;
                        }
                        else
                        {
                            test =2;
                        }
                    }
                }
            }
        }
        if (direction_longement <=0)
        break;

    }
    if(test >=2 || direction_longement ==-1)
    {
        return -1;
    }
    else return direction_longement;

}

int evitement_hugo(int x_objectif,int y_objectif,int haut) //determine l'itineraire pour aller de notre position a l'objectif en tenant compte des obstacles eventuels
{
    int flag=1;// on essaye selon les x quand flag =1, selon les y si flag = 2
    int direction=0;
    int direction_longement=0;
    int i;

    while (flag > 0 && compteur_evitement < ATTENTE_EVITEMENT)// tant qu'il n'y a eu aucun probleme et qu'on a toujours le temps de calculer
    {

        if(curseur_obstacle >= nb_point_max) // si on a dépassé le nombre d'obstacle maximale
        {
            return -1; // on arrête
        }

        for(i=0; i<curseur-1; i++)
        {
            if(itineraire[i][0] == itineraire[curseur-1][0] && itineraire[i][1] == itineraire[curseur-1][1] && itineraire[i][2] == itineraire[curseur-1][2]) // si on tourne en rond
            return -1; // on arrête
        }

        if(flag == 2 && y_actuel==y_objectif) // si on doit bouger sur les y et que notre coordonnée en y correspond à celle de l'objectif
        {
            flag =1; // on bougera sur les x
        }

        if(flag == 1 && x_actuel==x_objectif) // si on doit bouger sur les x et que notre coordonnée en x correspond à celle de l'objectif
        {
            flag =2; // on bougera sur les y
        }

        if(curseur >= nb_point_max-1) // si on a dépassé le nombre de point d'itinéraire
        {
            return -1; // on arrête
        }

        switch (flag)
        {
            case 1 :
            direction = tracer_ligne_x(x_objectif);//on trace une ligne droite et on voit si il y a bloquage
            if(direction != 0) //si l'on est bloque
            {

                //on sauvegarde notre obstacle
                point_obstacle[curseur_obstacle][0]=curseur;
                point_obstacle[curseur_obstacle][1]=direction;
                point_obstacle[curseur_obstacle][2]=haut;
                curseur_obstacle++;

                if(aiguillage_evitement(x_objectif,y_objectif,direction,haut) == -1) // Si notre tentative de longement ne fonctionne pas
                {
                    return -1; // on arrête
                }
                //on ira ensuite repasser un nouveau tour du while(flag!=-1) pour continuer d'avancer sur les x
            }
            else if(x_actuel == x_objectif && y_actuel==y_objectif)//si on est arrive
            flag =0; // on a trouve un chemin et on sort
            else
            {
                flag = 2; // on essaye de bouger sur les y
            }
            break;

            case 2 :
            direction = tracer_ligne_y(y_objectif);//on trace une ligne droite et on voit si il y a bloquage
            if (direction != 0) //si blocage
            {
                //on sauvegarde notre obstacle
                point_obstacle[curseur_obstacle][0]=curseur;
                point_obstacle[curseur_obstacle][1]=direction;
                point_obstacle[curseur_obstacle][2]=haut;
                curseur_obstacle++;

                if(aiguillage_evitement(x_objectif,y_objectif,direction,haut) == -1)// Si notre tentative de longement ne fonctionne pas
                {
                    return -1;// on arrête
                }

            }



            //on ira ensuite repasser un nouveau tour du while(flag!=-1) pour continuer d'avancer sur les y

            else if(x_actuel == x_objectif && y_actuel==y_objectif)//si on est arrive
            flag =0; // on a trouve un chemin et on sort
            else
            {
                flag = 1; // on essaye de bouger sur les x
            }
            break;
        }

        if(x_actuel == x_objectif && y_actuel==y_objectif)//si on est arrive
        flag =0; // on a trouve un chemin et on sort

        else if (direction_longement == -1) //si on est a un cul de sac
        {
            flag=-1;//on sort de cet itineraire
        }



    }

    if(flag==0) //si on est arrive
    {

        itineraire[curseur][2]=0;
        post_traitement();

        return 0; // on a trouve un chemin
    }
    else
    {
        return -1; // on a pas trouve de chemin
    }
}

void MAJ_obstacle(int x_present, int y_present,int angle,int8_t sens_marche,int action)
{
    int i,j;
    int ajout=1;
    int ajout_diago = 2;
    #ifdef PETIT_ROBOT
        int taille = 1;// il y a deja 3 de largeur (devant nous, a cette gauche et a cette droite) la taille est ce qu'on met en plus de ces 3 la
        int taille_diago = 1;
        int vision = 2;
        int vision_diago = 1;
    #endif
    #ifdef GROS_ROBOT
        int taille = 1;// il y a deja 3 de largeur, devant nous, a cette gauche et a cette droite, la taille est ce qu'on met en plus de ces 3 la
        int taille_diago = 1;
        int vision = 2;
        int vision_diago = 1;
    #endif
    #ifdef CARTE_BALISE
        int taille = 1;// il y a deja 3 de largeur, devant nous, a cette gauche et a cette droite, la taille est ce qu'on met en plus de ces 3 la
        int taille_diago = 1;
        int vision = 2;
        int vision_diago = 1;
    #endif 

    int offset=0; // en fonction de si c'est le capteur gauche ou le droit ou la balise
    int largeur=0; // nombre de case recouverte PAR US (balise = 2us, le gauche et le droit) en plus des 3 cases en face de nous
    int x_obs,y_obs; // les coordonnées d'un point de l'obstacle

    //les différents groupes de détection en fonction de la localisation spatiale
    int droite = 0;
    int gauche = 0;
    int centre = 0;

    #ifdef GROS_ROBOT
        ajout = 2;
        ajout_diago = 2;
    #endif // GROS_ROBOT

    precedent_obstacle[0] = x_present;
    precedent_obstacle[1]= y_present;
    precedent_obstacle[2]= angle;
    precedent_obstacle[3] = sens_marche;

    if(EVITEMENT_ADV.sens == MARCHE_ARRIERE)
    {
        angle += 180;
    }
    if(angle < 0)
    angle = (int)(angle + 360) % 360;
    if(action == AJOUTER)
    {         
            if (EVITEMENT_ADV.sens == MARCHE_AVANT)
            {
                if ( ((EVITEMENT_ADV.cote & EV_CENTRE) != 0) && (CAPT_ADV_AVANT_C == ETAT_ADV_AVANT_C) )
                {
                    centre = 1;
                }
                if ( ((EVITEMENT_ADV.cote & EV_GAUCHE) != 0) && (CAPT_ADV_AVANT_G == ETAT_ADV_AVANT_G) )
                {
                    gauche = 1;
                }
                if ( ((EVITEMENT_ADV.cote & EV_CENTRE) != 0) && (CAPT_ADV_AVANT_D == ETAT_ADV_AVANT_D) )
                {
                    droite = 1;
                }
            }
            else
            {
                if ( ((EVITEMENT_ADV.cote & EV_CENTRE) != 0) && (CAPT_ADV_ARRIERE_C == ETAT_ADV_ARRIERE_C) )
                {
                    centre = 1;
                }
                if ( ((EVITEMENT_ADV.cote & EV_GAUCHE) != 0) && (CAPT_ADV_ARRIERE_G == ETAT_ADV_ARRIERE_G) )
                {
                    gauche = 1;
                }
                if ( ((EVITEMENT_ADV.cote & EV_CENTRE) != 0) && (CAPT_ADV_ARRIERE_D == ETAT_ADV_ARRIERE_D) )
                {
                    droite = 1;
                }                
            }
    }
    else
    {
        gauche = precedent_obstacle[4];
        centre = precedent_obstacle[5];
        droite = precedent_obstacle[6];
    }


    precedent_obstacle[4] = gauche;
    precedent_obstacle[5] = centre;
    precedent_obstacle[6] = droite;

    //ce qui suit correspond aux différents cas de positionnement d'obstacle
    //on définit en premier les valeurs d'offset et de largeur avant d'appliquer l'obstacle
    //pour les obstacles en diagonale, on place un point d'obstacle plus 4 autres autour
    //de lui, en haut, en bas,à gauche et à droite. Pour lisser l'obstacle, certains de ces
    //points supplémentaires sont omis

    if(angle <= 22 || angle >= 339) //horizontale droite
    {
        if (centre == 1)
        {
            offset = - taille;
            largeur =taille*2;
        }
        else
        {

            if(droite == 1)
            {
                offset += -taille;
                largeur += taille;
            }

            if(gauche == 1)
            {
                offset += 0 ;
                largeur += taille;
            }
        }



        for(i=0; i<vision; i++)
        {
            for(j=-1 - ajout ; j<=largeur+1 + ajout; j++)
            {
                if(x_present+i+1 < x_max && y_present +offset +j >= 0 && y_present +offset +j < y_max && obstacle [x_present+i+1][y_present +offset + j] == 0)
                obstacle [x_present+i+1][y_present +offset + j]=action;
            }
        }
    }


    else if(angle <=68 )
    {
        if(centre == 1)
        {
            largeur += 2*(taille_diago);
            offset += -taille_diago;
        }

        else
        {
            if(droite == 1)
            {
                largeur +=taille_diago;
                offset += -taille_diago;
            }


            if(gauche == 1)
            {
                largeur +=taille_diago;
                offset +=0;
            }
        }

        for(i=0; i<vision_diago; i++)
        {
            for(j=-ajout_diago; j<=largeur+ajout_diago; j++)
            {
                x_obs = x_present+1+offset+j+i;
                y_obs = y_present-1+offset+j-i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                obstacle[x_obs][y_obs] = action;
                if(!(j == largeur+ajout_diago)  && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                obstacle[x_obs+1][y_obs] = action;
                if(!(j == largeur+ajout_diago)&& x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                obstacle[x_obs][y_obs+1] = action;
                if(!(j == -ajout_diago )  && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                obstacle[x_obs-1][y_obs] = action;
                if(!(j == -ajout_diago)  && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                obstacle[x_obs][y_obs-1] = action;
            }
        }
    }

    else if (angle <=112) //verticale en haut
    {
        if (centre == 1)
        {
            offset = - taille;
            largeur =taille*2;
        }

        else
        {
            if(droite)
            {
                offset += -taille;
                largeur += taille;
            }

            if(gauche)
            {
                offset += 0 ;
                largeur += taille;
            }

        }

        for(i=0; i<vision; i++)
        {
            for(j=-1-ajout; j<=largeur+1+ajout; j++)
            {
                if(y_present-i-1 >= 0 && x_present +offset + j>= 0 && x_present + offset + j < x_max && obstacle [x_present + offset + j][y_present-i-1] ==0)
                obstacle [x_present + offset + j][y_present-i-1]=action;
            }
        }
    }


    else if(angle <=158 )
    {
        if(centre)
        {
            largeur += 2*taille_diago;
            offset += taille_diago;
        }

        else
        {

            if(gauche)
            {
                largeur +=taille_diago;
                offset += taille_diago;
            }


            if(droite)
            {
                largeur +=taille_diago;
                offset +=0;
            }
        }

        for(i=0; i<vision_diago; i++)
        {
            for(j=-ajout_diago; j<=largeur+ajout_diago; j++)
            {
                x_obs = x_present-1+offset-j-i;
                y_obs = y_present-1-offset+j-i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                obstacle[x_obs][y_obs] = action;
                if(!(j == -ajout_diago) && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                obstacle[x_obs+1][y_obs] = action;
                if(!(j == largeur+ajout_diago) && x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                obstacle[x_obs][y_obs+1] = action;
                if(!(j == largeur+ajout_diago) && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                obstacle[x_obs-1][y_obs] = action;
                if(!(j == -ajout_diago) && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                obstacle[x_obs][y_obs-1] = action;
            }
        }
    }

    else if(angle <= 202) //horizontale gauche
    {


        if (centre)
        {
            offset = - taille;
            largeur =taille*2;
        }
        else
        {
            if(gauche)
            {
                offset += -taille;
                largeur += taille;
            }


            if(droite)
            {
                offset += 0 ;
                largeur += taille;
            }

        }
        for(i=0; i<vision; i++)
        {
            for(j=-1-ajout; j<=largeur+1+ajout; j++)
            {
                if(x_present-i-1 >=0 && y_present +offset + j >= 0 && y_present +offset + j < y_max && obstacle [x_present-i-1][y_present + offset + j]==0)
                obstacle [x_present-i-1][y_present +offset + j]=action;
            }
        }
    }

    else if(angle <=248 )
    {
        if(centre)
        {
            largeur += 2*taille_diago;
            offset += taille_diago;
        }
        else
        {
            if(gauche)
            {
                largeur +=taille_diago;
                offset += taille_diago;
            }


            if(droite)
            {
                largeur +=taille_diago;
                offset +=0;
            }
        }
        for(i=0; i<vision_diago; i++)
        {
            for(j=-ajout_diago; j<=largeur+ajout_diago; j++)
            {
                x_obs = x_present-1-offset+j-i;
                y_obs = y_present+1-offset+j+i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                obstacle[x_obs][y_obs] = action;
                if(!(j == largeur+ajout_diago ) && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                obstacle[x_obs+1][y_obs] = action;
                if(!(j == largeur+ajout_diago ) && x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                obstacle[x_obs][y_obs+1] = action;
                if(!(j == -ajout_diago ) && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                obstacle[x_obs-1][y_obs] = action;
                if(!(j == -ajout_diago ) && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                obstacle[x_obs][y_obs-1] = action;
            }
        }
    }


    else   if(angle <=292 )// verticale en bas
    {

        if (centre)
        {
            offset = - taille;
            largeur =taille*2;
        }
        else
        {
            if(gauche)
            {
                offset += -taille;
                largeur += taille;
            }

            if(droite)
            {
                offset += 0 ;
                largeur += taille;
            }
        }
        for(i=0; i<vision; i++)
        {
            for(j=-1-ajout; j<=largeur+1+ajout; j++)
            {
                if(y_present+i+1 < y_max && x_present+ offset + j>= 0 && x_present + offset + j < x_max && obstacle [x_present + offset + j][y_present+i+1]==0)
                obstacle [x_present + offset + j][y_present+i+1]=action;
            }
        }
    }

    else //if(angle <=338 )
    {
        if(centre)
        {
            largeur += 2*taille_diago;
            offset += taille_diago;
        }
        else
        {
            if(droite)
            {
                largeur +=taille_diago;
                offset += taille_diago;
            }


            if(gauche)
            {
                largeur +=taille_diago;
                offset +=0;
            }
        }
        for(i=0; i<vision_diago; i++)
        {
            for(j=-ajout_diago; j<=largeur+ajout_diago; j++)
            {
                x_obs = x_present+1+offset-j+i;
                y_obs = y_present+1-offset+j+i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                obstacle[x_obs][y_obs] = action;
                if(!(j == -ajout_diago ) && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                obstacle[x_obs+1][y_obs] = action;
                if(!(j == largeur+ajout_diago ) && x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                obstacle[x_obs][y_obs+1] = action;
                if(!(j == largeur+ajout_diago )  && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                obstacle[x_obs-1][y_obs] = action;
                if(!(j == -ajout_diago) && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                obstacle[x_obs][y_obs-1] = action;
            }
        }
    }
}
void deplacement_arriere(int8_t sens_marche)
{
    int i;
    int angle;
    if(sens_marche == MARCHE_AVANT) // si on est en marche avant, alors on reculera donc on multipliera -100 du avancer_reculer par 1 (i), et on change l'evitement
    {
        i=1;
        EVITEMENT_ADV.detection = OFF;
    }
    else // si on est en marche arriere, alors on voudra avancer et non reculer, du coup on multpliera -100 par -1(i), et on change l'evitement
    {
        i=-1;
        EVITEMENT_ADV.detection = OFF;
    }

    angle= - get_orientation();
    if(angle < 0) // get_orientation renvoi des valeurs négatives donc on les repasse en positif
    {
        angle = (int)(angle + 360)%360;
    }

    //a partir de la, on va essayer de reculer si on peut ! les conditions qui suivent sont toutes les mêmes
    //si on est dans un certain angle, on check les 2 cases derrieres ou devant lui selon le sens de la marche
    //si elles sont libres, alors on peut reculer ou avancer
    //si on rentre en evitement dans ce reculer ou avancer, on se stoppera et on continuera la fonction d'evitement

    #ifdef ORDI
        if(angle <= 22 || angle >= 339 ) //horizontale vers la droite
        {
            if(obstacle[x_actuel - i][y_actuel] == 0 && obstacle[x_actuel - 2*i][y_actuel] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }

        else if (angle <= 68) //diagonale en haut à droite
        {
            if( obstacle[x_actuel - i][y_actuel +1*i] == 0 && obstacle[x_actuel - 2*i][y_actuel +2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }
        else if (angle <= 112) //verticale haut
        {
            if(obstacle[x_actuel][y_actuel +i] == 0 && obstacle[x_actuel][y_actuel +2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }

        else if (angle <= 158) //diagonale en haut à gauche
        {
            if(obstacle[x_actuel +i][y_actuel +i] == 0 && obstacle[x_actuel + 2*i][y_actuel +2*i] == 0)
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }

        else if (angle <= 202) //horizontale gauche
        {
            if(obstacle[x_actuel +i][y_actuel] == 0 && obstacle[x_actuel + 2*i][y_actuel] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }
        else if (angle <= 248) //diagonale bas gauche

        {

            if(obstacle[x_actuel +i][y_actuel -i] == 0 && obstacle[x_actuel + 2*i][y_actuel -2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }

        else if (angle <= 292) //verticale bas
        {
            if( obstacle[x_actuel][y_actuel -i] == 0 && obstacle[x_actuel][y_actuel -2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                cout << "avancer_reculer("<<-100*i<<", 50)";
            }
        }

        else if (obstacle[x_actuel -i][y_actuel-i] == 0 && obstacle[x_actuel - 2*i][y_actuel-2*i] == 0 ) //diagonale bas droite
        {
            EVITEMENT_ADV.detection = OFF;
            cout << "avancer_reculer("<<-100*i<<", 50)";
        }
        #else

        if(angle <= 22 || angle >= 339 ) //horizontale vers la droite
        {
            if(obstacle[x_actuel - i][y_actuel] == 0 && obstacle[x_actuel - 2*i][y_actuel] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                avancer_reculer((double)(-100*i), 50.);
            }
        }

        else if (angle <= 68) //diagonale en haut à droite
        {
            if( obstacle[x_actuel - i][y_actuel +1*i] == 0 && obstacle[x_actuel - 2*i][y_actuel +2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                avancer_reculer((double)(-100*i), 50.);
            }
        }
        else if (angle <= 112) //verticale haut
        {
            if(obstacle[x_actuel][y_actuel +i] == 0 && obstacle[x_actuel][y_actuel +2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                avancer_reculer((double)(-100*i), 50.);
            }
        }

        else if (angle <= 158) //diagonale en haut à gauche
        {
            if(obstacle[x_actuel +i][y_actuel +i] == 0 && obstacle[x_actuel + 2*i][y_actuel +2*i] == 0)
            {
                EVITEMENT_ADV.detection = OFF;
                avancer_reculer((double)(-100*i), 50.);
            }
        }

        else if (angle <= 202) //horizontale gauche
        {
            if(obstacle[x_actuel +i][y_actuel] == 0 && obstacle[x_actuel + 2*i][y_actuel] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                avancer_reculer((double)(-100*i), 50.);
            }
        }
        else if (angle <= 248) //diagonale bas gauche

        {
            if(obstacle[x_actuel +i][y_actuel -i] == 0 && obstacle[x_actuel + 2*i][y_actuel -2*i] == 0 )
            {
                //TIMER_DEBUG = ACTIVE;
                EVITEMENT_ADV.detection = OFF;
                //TODO
                //A ENLEVER
                //i=-i;
                avancer_reculer((double)(-100*i), 50.);
            }

        }

        else if (angle <= 292) //verticale bas
        {
            if( obstacle[x_actuel][y_actuel -i] == 0 && obstacle[x_actuel][y_actuel -2*i] == 0 )
            {
                EVITEMENT_ADV.detection = OFF;
                avancer_reculer((double)(-100*i), 50.);
            }
        }

        else if (obstacle[x_actuel -i][y_actuel-i] == 0 && obstacle[x_actuel - 2*i][y_actuel-2*i] == 0 ) //diagonale bas droite
        {
            EVITEMENT_ADV.detection = OFF;
            avancer_reculer((double)(-100*i), 50.);
        }
    #endif

    //on remet l'evitement comme il etait au départ
    if(sens_marche == MARCHE_AVANT)
    {
        EVITEMENT_ADV.detection = OFF;
    }
    else
    {
        EVITEMENT_ADV.detection = OFF;
    }
}

void plus_court(int x_objectif,int y_objectif,int8_t sens_marche,double pourcentage_deplacement,char last,int ajout)
{
    int i,j;
    int retour=0;
    int curseur_finale;
    int curseur_obstacle_finale;
    int haut;
    int test_autre_sens=0;
    int sens_depart=1;
    int distance_courante;
    int nouvelle_distance;
    int id = id_evitement;
    id_evitement++;
    chemin_court=0;
    calcul_en_cours = ON;
    compteur_evitement = 0;

    if(pourcentage_deplacement > 50) // si la vitesse est supérieur à 50% de la vitesse maximale
    {
        pourcentage_deplacement = 50; // on la remet à 50% ce qui nous permet d'éviter les collisions à trop haute vitesse
    }

    if(id == id_evitement_initial) // si c'est le premier evitement lors de cette phase de deplacement
    {
        x_souhaite = x_objectif;
        y_souhaite = y_objectif;
        //on donne la cellule d'objectif
        x_obj = x_objectif/100;
        y_obj = y_objectif/100;
    }

    //on donne notre cellule de départ
    x_actuel = get_X()/100;
    y_actuel = get_Y()/100;


    for(i=0; i<nb_point_max; i++) // on reset les différents itineraires et les obstacles
    {
        itineraire[i][0]=-1;
        itineraire[i][1]=-1;
        itineraire[i][2]=-1;
        itineraire_basique[i][0]=-1;
        itineraire_basique[i][1]=-1;
        itineraire_basique[i][2]=-1;
        itineraire_court[i][0]=-1;
        itineraire_court[i][1]=-1;
        itineraire_court[i][2]=-1;
        point_obstacle[i][0]=-1;
        point_obstacle[i][1]=-1;
        point_obstacle[i][2]=-1;
    }
    curseur_obstacle = 0; ////on remet notre curseur de point d'obstacle au début

    // on efface le précédent obstacle mobile
    for(i=0; i<x_max; i++)
    for(j=0; j<y_max; j++)
    if(obstacle[i][j] == 2)
    obstacle[i][j] = 0;

    if(ajout == 1)
    MAJ_obstacle(x_actuel,y_actuel,- get_orientation(),sens_marche,AJOUTER); // on place notre nouvel obstacle

    #ifdef DEBUG_HUGO
        printf("\n\r%d %d||%d %d\n\r", x_actuel, y_actuel, x_obj, y_obj);
        for(j=0; j<y_max; j++)
        {
            for(i=0; i+4<x_max; i=i+5)
            {
                printf("%d", obstacle[i][j]*10000+obstacle[i+1][j]*1000+obstacle[i+2][j]*100+obstacle[i+3][j]*10+obstacle[i+4][j]);
            }
            printf("\n\r");
        }
    #endif

    #ifdef ORDI
        for(int j=y_max-1; j>=0; j--)
        {
            for(i=0; i<x_max; i++)
            {
                if(i==x_actuel && j==y_actuel)
                cout << "D";
                else if(i==x_obj && j==y_obj)
                cout << "A";
                else if(obstacle[i][j] !=0)
                cout << "X";
                else cout << "-";
            }
            cout << " "<<j<<endl;
        }
        for(i=0; i<x_max; i++)
        {
            cout << i%10;
        }
        cout <<endl;
    #endif

    curseur =0;//on remet notre curseur de point itineraire au début

    //on place notre cellule actuelle comme point de départ
    itineraire[curseur][0]=x_actuel;
    itineraire[curseur][1]=y_actuel;

    if(obstacle[x_obj][y_obj]!=0 || obstacle[x_actuel][y_actuel]!=0 || (x_actuel == x_obj && y_actuel == y_obj)) // si notre objectif est en obstacle, ou notre position, ou si on est dans la meme cellule que l'objectif
    {
        //alors on va reculer puis repartir vers l'objectif
        deplacement_arriere(sens_marche);
    }

    else // si notre cellule et celle de l'objectif sont libres
    {

        retour =evitement_hugo (x_obj,y_obj,sens_depart); // on essaye de trouver un chemin. On renvoi 0 si on a pu trouver un chemin, -1 sinon

        if(retour == 0) // si on a trouver un chemin
        {
            distance_courante = distance();// on calcul la distance entre notre de cellule traversée

            //on stocke notre itineraire comme itineraire le plus court qu'on ait pour le moment (en même temps on en a pas d'autre)
            for(i=0; i<=curseur; i++)
            {
                itineraire_court[i][0] = itineraire[i][0];
                itineraire_court[i][1] = itineraire[i][1];
                itineraire_court[i][2] = itineraire[i][2];
            }

            // On stocke les différentes valeurs qui caractérise cet itineraire, comme le nombre de point et le nombre d'obstacle
            curseur_finale=curseur;
            curseur_obstacle_finale = curseur_obstacle;
            chemin_court=1;//on indique ici qu'on a au moins 1 chemin

        }
        else //sinon on va initialiser les valeurs que l'on peut
        {
            distance_courante=0;
            curseur_finale=curseur;
            curseur_obstacle_finale = curseur_obstacle;
            chemin_court =0;
            for(i=0; i<=curseur; i++)
            {
                itineraire_basique[i][0]=itineraire[i][0];
                itineraire_basique[i][1]=itineraire[i][1];
                itineraire_basique[i][2]=itineraire[i][2];
            }

        }

        //tant qu'on a pas re essaye a partir de tous nos obstacles et que le garde fou n'a pas ete declenche, on va chercher un chemin plus court
        while (curseur_obstacle> 0 && compteur_evitement < ATTENTE_EVITEMENT)
        {
            curseur=point_obstacle[curseur_obstacle-1][0];// on retourne au dernier obstacle qu'on a sauvegarde
            for(i=0; i<curseur; i++) // et on charge l'itineraire que l'on avait
            {
                itineraire[i][0]=itineraire_basique[i][0];
                itineraire[i][1]=itineraire_basique[i][1];
                itineraire[i][2]=itineraire_basique[i][2];
            }

            //on se replace comme il faut
            x_actuel = itineraire_basique[curseur][0];
            y_actuel = itineraire_basique[curseur][1];

            //et on se re inscrit sur l'itineraire
            itineraire[curseur][0]=x_actuel;
            itineraire[curseur][1]=y_actuel;


            haut = point_obstacle[curseur_obstacle-1][2];// on regarde si on est aller vers le haut ou vers le bas
            if(haut == 1)// si on passait par le haut
            {
                haut =0; // alors on passera par le bas
            }
            else // si on passait pas le bas
            {
                haut = 1;// alors on passera par le haut
            }

            retour=evitement_hugo(x_obj,y_obj,haut);//on retente de trouver un evitement


            if(curseur_obstacle >= nb_point_max)
            {
                break;
            }

            if(retour ==0)  // si on en a trouver un
            {
                chemin_court = 1;
                nouvelle_distance = distance(); //on recalcule le nombre de point
            }
            if(retour == 0 &&  (itineraire_court[0][0] == -1 || curseur < curseur_finale || nouvelle_distance < distance_courante))  // si on a un chemin valide soit on en avait pas soit plus court
            {

                for(i=0; i<curseur+1; i++) // on sauvegarde le nouvel itineraire
                {
                    itineraire_court[i][0] = itineraire[i][0];
                    itineraire_court[i][1] = itineraire[i][1];
                    itineraire_court[i][2] = itineraire[i][2];
                }

                // ainsi que les données qui le caractérisent
                curseur_finale = curseur;
                distance_courante = nouvelle_distance;
                curseur_obstacle_finale = curseur_obstacle;
            }
            else    // si on a pas un chemin plus court
            {
                if(chemin_court ==1)
                for(i=0; i<curseur_finale+1; i++) // on remet l'itineraire comme il était
                {
                    itineraire[i][0] = itineraire_court[i][0];
                    itineraire[i][1] = itineraire_court[i][1];
                    itineraire[i][2] = itineraire_court[i][2];
                }

                curseur_obstacle_finale = curseur_obstacle_finale-1; // et on recharge le curseur d'obstacle
            }
            curseur_obstacle = curseur_obstacle_finale;
            if(curseur_obstacle == 0 && test_autre_sens == 0) // si on a essaye tous les chemins d'un côté mais pas de l'autre
            {

                //alors on va essayer de l'autre côté
                test_autre_sens =1;
                for(i=0; i<nb_point_max; i++) // on reset les différents itineraires et les obstacles
                {
                    point_obstacle[i][0]=-1;
                    point_obstacle[i][1]=-1;
                    point_obstacle[i][2]=-1;
                }
                curseur_obstacle = 0; ////on remet notre curseur de point d'obstacle au début
                curseur = 0;
                x_actuel = get_X()/100;
                y_actuel = get_Y()/100;

                itineraire[curseur][0]=x_actuel;
                itineraire[curseur][1]=y_actuel;

                retour =evitement_hugo (x_obj,y_obj,(sens_depart+1)%2); // on essaye de trouver un chemin. On renvoi 0 si on a pu trouver un chemin, -1 sinon

                if(retour ==0)  // si on en a trouver un
                {
                    chemin_court = 1;
                    nouvelle_distance = distance(); //on recalcule le nombre de point
                }
                if(retour == 0 &&  (itineraire_court[0][0] == -1 || curseur < curseur_finale || nouvelle_distance < distance_courante))  // si on a un chemin valide soit on en avait pas soit plus court
                {

                    for(i=0; i<curseur+1; i++) // on sauvegarde le nouvel itineraire
                    {
                        itineraire_court[i][0] = itineraire[i][0];
                        itineraire_court[i][1] = itineraire[i][1];
                        itineraire_court[i][2] = itineraire[i][2];
                    }

                    // ainsi que les données qui le caractérisent
                    curseur_finale = curseur;
                    distance_courante = nouvelle_distance;
                    curseur_obstacle_finale = curseur_obstacle;
                }
                else //sinon on va initialiser les valeurs que l'on peut
                {
                    curseur_obstacle_finale = curseur_obstacle;
                    for(i=0; i<=curseur; i++)
                    {
                        itineraire_basique[i][0]=itineraire[i][0];
                        itineraire_basique[i][1]=itineraire[i][1];
                        itineraire_basique[i][2]=itineraire[i][2];
                    }
                }
            }
        }


        if(chemin_court == 1) // si on a trouve un chemin
        {


            curseur=curseur_finale; //on reprend notre curseur de l'itineraire le plus court

            deplacement(sens_marche,pourcentage_deplacement,last);// et on lance les deplacements
        }

        else // si on a pas trouve de chemin plus court
        {
            //alors on va reculer puis repartir vers l'objectif
            curseur = 0;
            x_actuel = get_X()/100;
            y_actuel = get_Y()/100;
            deplacement_arriere(sens_marche);
        }
    }

    EVITEMENT_ADV.detection = OFF;

if(id == id_evitement_initial){

            id_evitement_initial = id_evitement;

    #ifdef ORDI
        //on redonne la consigne de base
        if(last == rej || last == FIN_TRAJECTOIRE)
        {
            cout << "rejoindre("<<x_souhaite<<","<<y_souhaite<<","<<sens_marche<<","<<pourcentage_deplacement<<")"<<endl;
        }
        else
        {
            cout <<"passe_part("<<x_souhaite<<","<<y_souhaite<<","<<sens_marche<<","<<pourcentage_deplacement<<","<<DEBUT_TRAJECTOIRE<<")"<<endl;
        }
        #else
        //on redonne la consigne de base

        if(last == rej) // si nous utilisions un rejoindre
        {
            rejoindre(x_souhaite,y_souhaite,sens_marche,pourcentage_deplacement);
        }
        else if (last == FIN_TRAJECTOIRE)// si on utilise un passe-part
        {


            if(chemin_court == 1 && curseur >1)// si on a eu un chemin avec des points intermédiaires
            {
                passe_part(x_souhaite,y_souhaite,sens_marche,pourcentage_deplacement,last);// on renvoi l'ordre initial
            }
            else
            {
                rejoindre(x_souhaite,y_souhaite,sens_marche,pourcentage_deplacement); // sinon on envoi un rejoindre pour éviter les conflits de passe-part
            }
        }
        else
        {
            if(chemin_court == 1 && curseur > 1) //si on a un chemin et qu'il y'a au moins deux points dans le passe part
            {


                passe_part(x_souhaite,y_souhaite,sens_marche,pourcentage_deplacement,MILIEU_TRAJECTOIRE);

            }
            else
            {
                passe_part(x_souhaite,y_souhaite,sens_marche,pourcentage_deplacement,DEBUT_TRAJECTOIRE);
            }
        }
    #endif

        }

    EVITEMENT_ADV.detection = OFF;
    calcul_en_cours = OFF;
}

void detect (uint8_t arg) 
{
    EVITEMENT_ADV.detection = arg;
}

