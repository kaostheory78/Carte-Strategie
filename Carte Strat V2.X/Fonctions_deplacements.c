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
    //Stratégie alternatives possibles en focntion des évitements
}

void cibler (double x, double y, double pourcentage_vitesse)
{
    uint8_t erreur = _cibler (x, y, pourcentage_vitesse);
    if ( erreur == EVITEMENT)
    {
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
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
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
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
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {
            plus_court(x,y,sens_marche,pourcentage_vitesse,rej);
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
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
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
        if (STRATEGIE_EVITEMENT == ACTION_EVITEMENT)
        {
            action_evitement();
        }
        else if (STRATEGIE_EVITEMENT == EVITEMENT_NORMAL)
        {

            plus_court(x,y,sens_marche,pourcentage_vitesse,last);
        }
    }
    else if (erreur == BLOCAGE)
    {
        //Actions a faire en cas de blocage
    }
}



/******************************************************************************/
/***************** FONCTIONS ASSERV BAS NIVEAU (config asserv) ****************/
/******************************************************************************/

uint8_t _cibler (double x, double y, double pourcentage_vitesse)
{
    y = inversion_couleur(y);
    FLAG_ASSERV.brake = OFF;
    //delay_ms(10);

    reinit_asserv();

    x *= TICKS_PAR_MM;
    y *= TICKS_PAR_MM;

    TYPE_CONSIGNE = MM;

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

uint8_t _orienter (double angle, double pourcentage_vitesse)
{
    angle = inversion_couleur(angle);
    FLAG_ASSERV.brake = OFF;
    //delay_ms(10);

    reinit_asserv();

    TYPE_CONSIGNE = MM;

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

        TYPE_CONSIGNE = MM;
    }

    angle = inversion_couleur(angle);

    ORIENTATION.consigne = (angle * Pi)/ 180 * (ENTRAXE_TICKS/2);

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;

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

uint8_t _rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse)
{
    y = inversion_couleur(y);
    FLAG_ASSERV.brake = OFF;
    //delay_ms(10);

    reinit_asserv();

    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    TYPE_CONSIGNE = XY;

    FLAG_ASSERV.sens_deplacement = sens_marche;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;
#ifdef PETIT_ROBOT
    acc.acceleration.orientation = DCC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;
#else
    acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;
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

uint8_t _avancer_reculer (double distance, double pourcentage_vitesse)
{
    FLAG_ASSERV.brake = OFF;
    reinit_asserv();

    TYPE_CONSIGNE = MM;

    DISTANCE.consigne = distance * (TICKS_PAR_MM);

    if (distance < 0.)
        FLAG_ASSERV.sens_deplacement = MARCHE_ARRIERE;
    else
        FLAG_ASSERV.sens_deplacement = MARCHE_AVANT;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    FLAG_ASSERV.position = ON;
    FLAG_ASSERV.orientation = OFF;
    FLAG_ASSERV.vitesse = ON;

    FLAG_ASSERV.etat_angle = ANGLE_ATTEINT;
    FLAG_ASSERV.etat_distance = EN_COURS;
    FLAG_ASSERV.immobilite = 0;

    FLAG_ASSERV.fin_deplacement = DEBUT_DEPLACEMENT;
    while (FLAG_ASSERV.fin_deplacement != FIN_DEPLACEMENT);
    return FLAG_ASSERV.erreur;
}

uint8_t _passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last)
{
    if (last == DEBUT_TRAJECTOIRE)
    {
        FLAG_ASSERV.brake = OFF;
        //  delay_ms(10);
        reinit_asserv();

        TYPE_CONSIGNE = XY;
    }

    y = inversion_couleur(y);
    X.consigne = x * TICKS_PAR_MM;
    Y.consigne = y * TICKS_PAR_MM;

    calcul_vitesse_position(pourcentage_vitesse);
    calcul_acceleration_position();

    VITESSE_MAX_ORIENTATION = VITESSE_ANGLE_PAS;
    acc.acceleration.orientation = ACC_ORIENTATION_CONSIGNE;
    acc.deceleration.orientation = DCC_ORIENTATION_CONSIGNE;

    FLAG_ASSERV.erreur = DEPLACEMENT_NORMAL;
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


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/*direction et repere dans le terrain
    *
    * 812 1 => y+1
    * 703 5 => y-1
    * 654 3 => x+1
    *
    *
 * direction et repere dans la matrice
 *    654
 *    703
 *    812
    *repere => 0:0 en bas a gauche quand on fait face au robotmovie (jaune a gauche et vert à CAPT_US_DROIT)
    *
*/


void deplacement(int8_t sens_marche,double pourcentage_deplacement,char last) //transforme les points en commandes de deplacements
{
    int i=0;
    int angle;
    int id = id_evitement;


    if(itineraire_court[i+1][0] != -1);
    cibler((double)(itineraire_court[i+1][0]*100)+50,(double)((y_max - itineraire_court[i+1][1])*100)+50,pourcentage_deplacement);

    while(i<curseur-1 && id == id_evitement && itineraire_court[i+1][0] != -1 )//affiche l'itineraire
    {


        if(last == rej)
        {
            DETECTION = OFF;
            PutsUART(UART_XBEE, "rejoindre, je suis a ");
            PutcUART (UART_XBEE, '{');
            PutIntUART(get_X());
            PutcUART (UART_XBEE, ' ');
            PutIntUART (get_Y());
            PutcUART (UART_XBEE, '}');
            PutsUART(UART_XBEE, " etje vais a ");
            PutcUART (UART_XBEE, '{');
            PutIntUART(get_X());
            PutcUART (UART_XBEE, ' ');
            PutIntUART (get_Y());
            PutcUART (UART_XBEE, '}');
            PutsUART(UART_XBEE, "\n\r");


            rejoindre((double)(itineraire_court[i+1][0]*100)+50,(double)((y_max - itineraire_court[i+1][1])*100)+50,sens_marche,pourcentage_deplacement);
        }
        else
        {

            PutsUART(UART_XBEE, "passe part");
            DETECTION = OFF;
            if(i == 0)
            {
                last = DEBUT_TRAJECTOIRE;
            }
            else
            {
                last = MILIEU_TRAJECTOIRE;
            }
            passe_part((double)(itineraire_court[i+1][0]*100)+50,(double)((y_max - itineraire_court[i+1][1])*100)+50,sens_marche,pourcentage_deplacement,last);
        }

        //passe_part ((double)(itineraire_court[i+1][0]*100), (double)((y_max - itineraire_court[i+1][1])*100), sens_marche,pourcentage_deplacement, last);

        //rejoindre((double)(itineraire_court[i+1][0]*100),(double)((y_max - itineraire_court[i+1][1])*100),MARCHE_AVANT,pourcentage_deplacement);
        i++;
    }

}

int distance()
{
    int i=0;
    int dist = 0;
    int addition;
    while(itineraire[i][0]!=-1 && compteur_evitement < ATTENTE_EVITEMENT)
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


void init_evitement()
{
    int i,j;

    //le 0;0 de la matrice correspond au 0;y_max du terrain

    id_evitement = 0;
    id_evitement_initial = 0;
    curseur_obstacle=0;

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
            else
                obstacle[i][j]=0;
        }
    }

#ifdef PETIT_ROBOT
    for(i=9; i<=20; i++) //escalier
    {
        for(j=0; j<=7; j++)
        {
            obstacle[i][j]=1;
        }
    }
#endif

#ifdef GROS_ROBOT

    for(i=9; i<=20; i++) //escalier
    {
        for(j=0; j<=8; j++)
        {
            obstacle[i][j]=1;
        }
    }
#endif

    for(i=10; i<=19; i++) // marche rose
    {
#ifdef PETIT_ROBOT
        for(j=18; j<=19; j++)
        {
            obstacle[i][j] =1;
        }
#endif
#ifdef GROS_ROBOT
        for(j=17; j<=19; j++)
        {
            obstacle[i][j] =1;
        }
#endif
    }

    if(COULEUR == JAUNE)
    {
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
            for(j=3; j<=8; j++)
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
            for(j=7; j<=13; j++)
            {
                obstacle[i][j]=1;
            }
        }
#endif
#ifdef GROS_ROBOT
        for(i=22; i<=x_max; i++) //zone depart adverse
        {
            for(j=6; j<=12; j++)
            {
                obstacle[i][j]=1;
            }
        }
#endif

    }
    else
    {
#ifdef PETIT_ROBOT
        for(i=25; i<=x_max; i++) //carre droit haut
        {
            for(j=4; j<=7; j++)
            {
                obstacle[i][j]=1;
            }
        }

        for(i=25; i<=x_max; i++) //carre droit bas
        {
            for(j=12; j<=15; j++)
            {
                obstacle[i][j]=1;
            }
        }

        for(i=0; i<=6; i++) //zone depart adverse
        {
            for(j=7; j<=13; j++)
            {
                obstacle[i][j]=1;
            }
        }
#endif
#ifdef GROS_ROBOT
        for(i=24; i<=x_max; i++) //carre droit haut
        {
            for(j=3; j<=8; j++)
            {
                obstacle[i][j]=1;
            }
        }

        for(i=24; i<=x_max; i++) //carre droit bas
        {
            for(j=11; j<=15; j++)
            {
                obstacle[i][j]=1;
            }
        }

        for(i=0; i<=7; i++) //zone depart adverse
        {
            for(j=6; j<=12; j++)
            {
                obstacle[i][j]=1;
            }
        }
#endif
        
    }


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
                && obstacle [x_actuel][y_actuel+1] == 0 //tant que l'on peut monter
                && obstacle [x_actuel-1][y_actuel] != 0 //et que la gauche de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel+1; // on monte d'une case
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
        else if((y_actuel == y_max-1 || obstacle [x_actuel][y_actuel+1] != 0) && obstacle [x_actuel+1][y_actuel] == 0)  //si on ne peut aller ni en haut ni a gauche mais a CAPT_US_DROIT
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
        else   // si on ne peut aller ni a gauche, ni en haut, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


    case 2 :
        while (y_actuel+1 <= y_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel][y_actuel+1] == 0 //tant que l'on peut monter
                && obstacle [x_actuel+1][y_actuel] != 0 //et que la CAPT_US_DROIT de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel+1; // on monte d'une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel+1][y_actuel] == 0 //si on peut aller a CAPT_US_DROIT, si on a trouve un passage
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
        else if((y_actuel == y_max-1 ||obstacle [x_actuel][y_actuel+1] != 0 )&& obstacle [x_actuel-1][y_actuel] == 0)  //si on ne peut aller ni en haut ni a CAPT_US_DROIT mais a gauche
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
        else   // si on ne peut aller ni a gauche, ni en haut, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


    case 3 :
        while (y_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel][y_actuel-1] == 0 //tant que l'on peut descendre
                && obstacle [x_actuel-1][y_actuel] != 0 //et que la gauche de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel-1; // on descend d'une case
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
        else if((y_actuel == 0 || obstacle [x_actuel][y_actuel-1] != 0) && obstacle [x_actuel+1][y_actuel] == 0)  //si on ne peut aller ni en bas ni a gauche mais a CAPT_US_DROIT
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
        else   // si on ne peut aller ni a gauche, ni en bas, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


    case 4 :
        while (y_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel][y_actuel-1] == 0 //tant que l'on peut descendre
                && obstacle [x_actuel+1][y_actuel] != 0 //et que la CAPT_US_DROIT de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            y_actuel = y_actuel-1; // on descend d'une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }

        if (obstacle [x_actuel+1][y_actuel] == 0 //si on peut aller a CAPT_US_DROIT, si on a trouve un passage
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
        else if((y_actuel == 0 || obstacle [x_actuel][y_actuel-1] != 0) && obstacle [x_actuel-1][y_actuel] == 0)  //si on ne peut aller ni en bas ni a CAPT_US_DROIT mais a gauche
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
        else   // si on ne peut aller ni a gauche, ni en bas, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


    case 5 :
        while (x_actuel +1 <=x_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel+1][y_actuel] == 0 //tant que l'on peut aller sur la CAPT_US_DROIT
                && obstacle [x_actuel][y_actuel+1] != 0 //et que en haut de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel+1; // on va sur la CAPT_US_DROIT de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }

        if (x_actuel == x_max)  // si on est en dehors de la matrice
        {

            return -1; // la recherche de passage de ce cote n'est pas fructueuse. peut être remplacé
        }

        else
        {
            if (obstacle [x_actuel][y_actuel+1] == 0 //si on peut aller en haut, si on a trouve un passage
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
            else if((x_actuel == x_max-1 || obstacle [x_actuel+1][y_actuel] != 0 )&& obstacle [x_actuel][y_actuel-1] == 0)  //si on ne peut aller ni en haut ni a CAPT_US_DROIT mais en bas
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
            else   // si on ne peut aller ni a gauche, ni en haut, ni a CAPT_US_DROIT
            {
                return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
            }
        }
        break;


    case 6 :
        while (x_actuel +1 <=x_max-1 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel+1][y_actuel] == 0 //tant que l'on peut aller sur la CAPT_US_DROIT
                && obstacle [x_actuel][y_actuel-1] != 0 //et que en bas de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel+1; // on va sur la CAPT_US_DROIT de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel][y_actuel-1] == 0 //si on peut aller en bas, si on a trouve un passage
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
        else if((x_actuel == x_max -1 || obstacle [x_actuel+1][y_actuel] != 0 )&& obstacle [x_actuel][y_actuel+1] == 0)  //si on ne peut aller ni en bas ni a CAPT_US_DROIT mais en haut
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
        else   // si on ne peut aller ni a gauche, ni en haut, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


    case 7 :
        while (x_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel-1][y_actuel] == 0 //tant que l'on peut aller sur la gauche
                && obstacle [x_actuel][y_actuel+1] != 0 //et que en haut de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel-1; // on va sur la gauche de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel][y_actuel+1] == 0 //si on peut aller en haut, si on a trouve un passage
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
        else   // si on ne peut aller ni a gauche, ni en haut, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;


    case 8 :
        while (x_actuel -1 >=0 //tant que l'on est toujours dans la matrice, que l'on est pas en dehors
                && obstacle [x_actuel-1][y_actuel] == 0 //tant que l'on peut aller sur la gauche
                && obstacle [x_actuel][y_actuel-1] != 0 //et que en bas de notre emplacement est condamnee
                && !(x_actuel == x_objectif && y_actuel == y_objectif)
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive (pas sur de l'utilite)
        {
            x_actuel = x_actuel-1; // on va sur la gauche de une case
            nombre_point_parcouru=nombre_point_parcouru+1;
        }


        if (obstacle [x_actuel][y_actuel-1] == 0 //si on peut aller en bas, si on a trouve un passage
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
        else   // si on ne peut aller ni a gauche, ni en haut, ni a CAPT_US_DROIT
        {
            return -1; //la recherche de passage de ce cote n'est pas fructueuse, on est dans un cul de sac
        }

        break;
    }
    return -1;
}

int tracer_ligne_x(int x_objectif,int y_objectif) //cree une ligne CAPT_US_DROIT suivant l'axe des x. Fais avancer ou reculer sur l'axe des x
{
    int nombre_point_parcouru=0;

    if(x_objectif>x_actuel)
    {
        while(x_actuel+1 <= x_max-1 //tant que l'on est dans la matrice
                && obstacle[x_actuel+1][y_actuel]==0 //tant que l'on peut aller a CAPT_US_DROIT
                && x_objectif!=x_actuel
                && compteur_evitement < ATTENTE_EVITEMENT)  // et que l'on est pas arrive au niveau des x
        {
            x_actuel = x_actuel+1; // on va de 1 vers la CAPT_US_DROIT
            nombre_point_parcouru=nombre_point_parcouru+1;
        }
        if(nombre_point_parcouru!=0)
        {
            itineraire[curseur][2]=3; // donne la direction pour aller de l'ancien point au nouveau point trouve
            curseur=curseur+1;
            itineraire[curseur][0]=x_actuel; //coordonnee en x du nouveau point
            itineraire[curseur][1]=y_actuel; //coordonnee en y du nouveau point
        }
        if(obstacle[x_actuel+1][y_actuel]!=0)
        {
            return 3;// retourne la direction du blocage
        }
        else if(x_objectif==x_actuel)
        {
            return 0; // pas de souci
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
        if(obstacle[x_actuel-1][y_actuel]!=0)
        {
            return 7;// retourne la direction du blocage
        }
        else if(x_objectif==x_actuel)
        {
            return 0; // pas de souci
        }
    }
    return -1;
}

int tracer_ligne_y(int x_objectif,int y_objectif) //cree une ligne CAPT_US_DROIT suivant l'axe des y. Fais avancer ou reculer sur l'axe des y
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
        if(obstacle[x_actuel][y_actuel+1]!=0)
        {
            return 1;// retourne la direction du blocage
        }
        else if(y_objectif==y_actuel)
        {
            return 0; // pas de souci
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
        if(obstacle[x_actuel][y_actuel-1]!=0)
        {
            return 5;// retourne la direction du blocage
        }
        else if(y_objectif==y_actuel)
        {
            return 0; // pas de souci
        }
    }
    return -1;

}

void post_traitement() //simplifie le trajet en elevant le maximum de deplacements inutiles
{
    int i,j,k,x_temporaire,y_temporaire,cible,flag,direction;
    for(i=0; i<=curseur; i++)
    {
        itineraire_basique[i][0]=itineraire[i][0];
        itineraire_basique[i][1]=itineraire[i][1];
        itineraire_basique[i][2]=itineraire[i][2];
    }


    //post traitement (1) si on passe deux fois par le meme point
    i=0;
    while(i<curseur+1 && compteur_evitement < ATTENTE_EVITEMENT) //s'occupe de quand on passe deux fois par le même point
    {
        j=curseur;
        while(j>i && compteur_evitement < ATTENTE_EVITEMENT)
        {
            if(itineraire[i][0]==itineraire[j][0] && itineraire[i][1]==itineraire[j][1]) //si on a deux fois le même couple de coordonnees
            {
                for(k=i; k<=curseur; k++)
                {
                    itineraire[k][0]=itineraire[j+k-i][0];
                    itineraire[k][1]=itineraire[j+k-i][1];
                    itineraire[k][2]=itineraire[j+k-i][2];

                }
                curseur=curseur-(j-i);//on reduit le nombre de point pour rester cohérent

            }
            j--;
        }
        i++;
    }
    /*
        //post traitement (2) si on a un mouvement a point puis le mouvement contraire 2 points apres

        i=0;
        while(i<=curseur-3 && compteur_evitement < ATTENTE_EVITEMENT)
        {
            if ((itineraire[i][2] - itineraire[i+2][2]) == 4 || (itineraire[i][2] - itineraire[i+2][2]) == -4) //si on a un mouvement a point puis le mouvement contraire 2 points apres
            {
                //permet d'eviter d'aller plus loin pour ensuite reculer par exemple

                switch (itineraire[i+2][2])//en fonction de la direction ou l'on recule, on se place et se donne un objectif sur les x ou y
                {
                    case 1:
                    x_temporaire=itineraire[i][0];
                    y_temporaire=itineraire[i+3][1];
                    cible=itineraire[i+3][0];
                    break;
                    case 3:
                    x_temporaire=itineraire[i+3][0];
                    y_temporaire=itineraire[i][1];
                    cible=itineraire[i+3][1];
                    break;
                    case 5:
                    x_temporaire=itineraire[i][0];
                    y_temporaire=itineraire[i+3][1];
                    cible=itineraire[i+3][0];
                    break;
                    case 7:
                    x_temporaire=itineraire[i+3][0];
                    y_temporaire=itineraire[i][1];
                    cible=itineraire[i+3][1];
                    break;
                }

                j=0;
                flag=0;
                switch  (itineraire[i+1][2]) //en fonction de la direction pour lier le point actuel et celui 2 point apres, on va avancer sur les x ou les y
                {
                    case 1:
                    while(y_temporaire+j != cible && compteur_evitement < ATTENTE_EVITEMENT)
                    {
                        j++;
                        if(obstacle[x_temporaire][y_temporaire+j] != 0) //si obstacle
                        {
                            j=0;
                            x_temporaire=x_temporaire+1;
                        }
                    }
                    itineraire[i+1][0]=x_temporaire;
                    itineraire[i+1][1]=y_temporaire;
                    itineraire[i+2][0]=x_temporaire;
                    itineraire[i+2][1]=y_temporaire+j;
                    if(x_temporaire == itineraire[i+3][0]) //si on a pas bouger selon les x (si on a pu faire une ligne CAPT_US_DROIT
                    {
                        flag=1;//flag de deplacement des points
                    }
                    break;

                    case 3:
                    while(x_temporaire+j != cible && compteur_evitement < ATTENTE_EVITEMENT)
                    {
                        j++;
                        if(obstacle[x_temporaire+j][y_temporaire] != 0) //si obstacle
                        {
                            j=0;
                            y_temporaire=y_temporaire+1;
                        }
                    }
                    itineraire[i+1][0]=x_temporaire;
                    itineraire[i+1][1]=y_temporaire;
                    itineraire[i+2][0]=x_temporaire+j;
                    itineraire[i+2][1]=y_temporaire;
                    if(y_temporaire == itineraire[i+3][1]) //si on a pas bouger selon les x (si on a pu faire une ligne CAPT_US_DROIT
                    {
                        flag=1;//flag de deplacement des points
                    }
                    break;
                    case 5:
                    while(y_temporaire-j != cible && compteur_evitement < ATTENTE_EVITEMENT)
                    {
                        j++;
                        if(obstacle[x_temporaire][y_temporaire-j] != 0) //si obstacle
                        {
                            j=0;
                            x_temporaire=x_temporaire-1;
                        }
                    }
                    itineraire[i+1][0]=x_temporaire;
                    itineraire[i+1][1]=y_temporaire;
                    itineraire[i+2][0]=x_temporaire;
                    itineraire[i+2][1]=y_temporaire-j;
                    if(x_temporaire == itineraire[i+3][0]) //si on a pas bouger selon les x (si on a pu faire une ligne CAPT_US_DROIT
                    {
                        flag=1;//flag de deplacement des points
                    }
                    break;
                    case 7:
                    while(x_temporaire-j != cible && compteur_evitement < ATTENTE_EVITEMENT)
                    {
                        j++;
                        if(obstacle[x_temporaire-j][y_temporaire] != 0) //si obstacle
                        {
                            j=0;
                            y_temporaire=y_temporaire-1;
                        }
                    }
                    itineraire[i+1][0]=x_temporaire;
                    itineraire[i+1][1]=y_temporaire;
                    itineraire[i+2][0]=x_temporaire-j;
                    itineraire[i+2][1]=y_temporaire;
                    if(y_temporaire == itineraire[i+3][1]) //si on a pas bouger selon les x (si on a pu faire une ligne CAPT_US_DROIT
                    {
                        flag=1;//flag de deplacement des points
                    }
                    break;

                    break;
                }


                if(flag==1)//si on doit decaler les points (cas de la ligne CAPT_US_DROIT)
                {
                    itineraire[i+1][2]=itineraire[i+3][2];

                    for(k=i+2; k<=curseur; k++)
                    {
                        itineraire[k][0]=itineraire[k+1][0];
                        itineraire[k][1]=itineraire[k+1][1];
                        itineraire[k][2]=itineraire[k+1][2];

                    }
                    curseur=curseur-1;
                }

            }
            i++;
        }

    */
    //post traitement 4) diagonale

    i=0;
    while(i<curseur && compteur_evitement < ATTENTE_EVITEMENT)
    {
        direction=0;
        flag =0;
        while((itineraire[i+flag][2] == 1 && itineraire[i+1+flag][2]==3)||(itineraire[i+flag][2] == 3 && itineraire[i+1+flag][2]==1)) //diagonale bas CAPT_US_DROIT
        {
            flag++;
            direction = 2;
        }
        while(flag ==0 && ((itineraire[i+flag][2] == 7 && itineraire[i+1+flag][2]==1)||(itineraire[i+flag][2] == 1 && itineraire[i+1+flag][2]==7))) //diagonale bas CAPT_US_DROIT
        {
            flag++;
            direction = 8;
        }
        while(flag ==0 && ((itineraire[i+flag][2] == 3 && itineraire[i+1+flag][2]==5)||(itineraire[i+flag][2] == 5 && itineraire[i+1+flag][2]==3))) //diagonale bas CAPT_US_DROIT
        {
            flag++;
            direction = 4;
        }
        while(flag ==0 && ((itineraire[i+flag][2] == 5 && itineraire[i+1+flag][2]==7)||(itineraire[i+flag][2] == 7 && itineraire[i+1+flag][2]==5))) //diagonale bas CAPT_US_DROIT
        {
            flag++;
            direction = 6;
        }

        if(flag != 0)
        {

            itineraire[i][2]=direction;
            for(k=i+1 ; k<=curseur-flag; k++)
            {
                itineraire[k][0]=itineraire[k+flag][0];
                itineraire[k][1]=itineraire[k+flag][1];
                itineraire[k][2]=itineraire[k+flag][2];
            }
            for(k=curseur-flag+1; k<=curseur+1; k++)
            {
                itineraire[k][0]=-1;
                itineraire[k][1]=-1;
                itineraire[k][2]=-1;

            }
            curseur = curseur-flag;
            i=i-flag;
            if(i<0)
            {
                i=0;
            }
        }
        i++;
    }



    //post traitement 3) si deux directions consecutives sont les memes
    i=0;
    while(i<curseur && compteur_evitement < ATTENTE_EVITEMENT)
    {
        flag=0;//on a pu reduire le nombre de point de x points
        while (itineraire[i][2]==itineraire[i+1][2] && compteur_evitement < ATTENTE_EVITEMENT) //si deux directions consecutives ou plus
        {
            i++;
            flag++;
        }
        if(flag>0)
        {
            for(k=i-flag+1; k<=curseur; k++)
            {
                itineraire[k][0]=itineraire[k+flag][0];
                itineraire[k][1]=itineraire[k+flag][1];
                itineraire[k][2]=itineraire[k+flag][2];

            }
            curseur=curseur-flag;
            i=i-flag;
            if(i<0)
            {
                i=0;
            }
        }
        i++;
    }


}
int  aiguillage_evitement(int x_objectif, int y_objectif, int direction,int haut)  // evitement par le haut ou par le bas
{
    int direction_longement;
    int cul_de_sac;
    int curseur_initial=curseur;
    int obstacle_initial = curseur_obstacle;
    int i;
    int test = 0;

    while(test <=1 && compteur_evitement < ATTENTE_EVITEMENT)
    {

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
                    if(itineraire[i][0] == itineraire[curseur-1][0] && itineraire[i][1] == itineraire[curseur-1][1] && itineraire[i][2] == itineraire[curseur-1][2] ) // on regarde si on est pas repasse par le meme point et la meme direction
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
        if(direction_longement ==-1 || haut == 0) //si on a atteint un cul de sac ou on veut aller pour le haut
        {

            direction_longement = conversion_direction(direction);//on reprend la valeur de longement de depart
            direction_longement=direction_longement+2; //on lui rajoute 1. Lors d'un blocage, on peut aller dans 2 directions. On tente ici la seconde direction
            cul_de_sac=0;
            while(direction_longement >0 && cul_de_sac == 0 && curseur < nb_point_max-1 && compteur_evitement < ATTENTE_EVITEMENT) //tant que l'on est bloque
            {
                direction_longement = longement(x_objectif,y_objectif,direction_longement);//on longe les obstacles
                for(i=0; i<curseur_initial; i++)
                {
                    if(itineraire[i][0] == itineraire[curseur-1][0] && itineraire[i][1] == itineraire[curseur-1][1] && itineraire[i][2] == itineraire[curseur-1][2] )// on regarde si on est pas repasse par le meme point et la meme direction
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
        if (direction_longement == 0)
            break;

    }
    if(test >=2 || direction_longement ==-1)
    {
        return -1; 
    }
    else return direction_longement;

}

int evitement(int x_objectif,int y_objectif,int haut) //determine l'itineraire pour aller de notre position a l'objectif en tenant compte des obstacles eventuels
{
    int flag=1;
    int direction=0;
    int direction_longement=0;
    int a_du_longer=0;

    while (flag > 0 && compteur_evitement < ATTENTE_EVITEMENT)
    {



        if(curseur >= nb_point_max-1)
        {
            break;
        }
        switch (flag)
        {
        case 1 :
            direction = tracer_ligne_x(x_objectif,y_objectif);//on trace une ligne CAPT_US_DROIT et on voit si il y a bloquage
            if(direction > 0) //si l'on est bloque
            {
                if(a_du_longer==0)
                {

                    a_du_longer=1;
                    point_obstacle[curseur_obstacle][0]=curseur;
                    point_obstacle[curseur_obstacle][1]=direction;
                    point_obstacle[curseur_obstacle][2]=haut;
                    curseur_obstacle++;


                }
                if(aiguillage_evitement(x_objectif,y_objectif,direction,haut) == -1)
                {
                    flag =-1;
                }

                //on ira ensuite repasser un nouveau tour du while(flag!=-1) pour continuer d'avancer sur les x
            }
            else if(x_actuel == x_objectif && y_actuel==y_objectif)//si on est arrive
                flag =0; // on a trouve un chemin et on sort
            else
            {
                a_du_longer=0;
                flag = 2; // on essaye de bouger sur les y
            }
            break;

        case 2 :
            direction = tracer_ligne_y(x_objectif,y_objectif);//on trace une ligne CAPT_US_DROIT et on voit si il y a bloquage
            if (direction != 0) //si blocage
            {
                if(a_du_longer==0)
                {

                    a_du_longer=1;
                    point_obstacle[curseur_obstacle][0]=x_actuel;
                    point_obstacle[curseur_obstacle][1]=y_actuel;
                    point_obstacle[curseur_obstacle][2]=direction;
                    point_obstacle[curseur_obstacle][3]=haut;
                    curseur_obstacle++;

                }
                if(aiguillage_evitement(x_objectif,y_objectif,direction,haut) == -1)
                {
                    flag =-1;
                }

                //on ira ensuite repasser un nouveau tour du while(flag!=-1) pour continuer d'avancer sur les y
            }
            else if(x_actuel == x_objectif && y_actuel==y_objectif)//si on est arrive
                flag =0; // on a trouve un chemin et on sort
            else
            {
                flag = 1; // on essaye de bouger sur les y
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

        post_traitement();

        return 0; // on a trouve un chemin
    }
    else
    {
        return -1; // on a pas trouve de chemin
    }
}

void nettoyage_nouvel_obstacle()
{
    int i,j;
    for(i=0; i<x_max; i++)
    {
        for(j=0; j<y_max; j++)
        {
            if(obstacle[i][j] == 2)
                obstacle[i][j] = 0;
        }
    }
}

void mettre_obstacle(int x_actuel, int y_actuel,int8_t sens_marche)
{
    int i,j;
#ifdef PETIT_ROBOT
    int taille = 1;// il y a deja 3 de largeur, devant nous, a cette gauche et a cette droite, la taille est ce qu'on met en plus de ces 3 la
    int taille_diago = 1;
    int vision = 2;// pair ou impair
#endif
#ifdef GROS_ROBOT
    int taille = 2;// il y a deja 3 de largeur, devant nous, a cette gauche et a cette droite, la taille est ce qu'on met en plus de ces 3 la
    int taille_diago = 2;
    int vision = 2;// pair ou impair
#endif
    
    int offset=0; // en fonction de si c'est le gauche ou le droit ou la balise
    int largeur=0; // nombre de case recouverte PAR US (balise = 2us, le gauche et le droit) en plus des 3 cases en face de nous
    int x_obs,y_obs;
    int droite = 0;
    int gauche = 0;
    int centre = 0;

    double angle = get_orientation();
    if(EVITEMENT_ADV_ARRIERE == ON)
    {
        angle += 180;
    }
    if(angle < 0)
        angle = (int)(angle + 360) % 360;

#ifdef PETIT_ROBOT
    if((1 == CAPT_US_BALISE && EVITEMENT_ADV_AVANT == ON) || (1 == CAPT_IR_ARRIERE_CENTRE && EVITEMENT_ADV_ARRIERE == ON))
    {
        centre = 1;
    }
    if((1 == CAPT_US_DROIT && EVITEMENT_ADV_AVANT == ON) || (1 == CAPT_IR_ARRIERE_GAUCHE && EVITEMENT_ADV_ARRIERE == ON))
    {
        droite = 1;
    }
    if((1 == CAPT_US_GAUCHE && EVITEMENT_ADV_AVANT == ON) || (1 == CAPT_IR_ARRIERE_DROIT && EVITEMENT_ADV_ARRIERE == ON))
    {
        gauche = 1;
    }
#endif

#ifdef GROS_ROBOT
    if((1 == CAPT_US_BALISE && EVITEMENT_ADV_AVANT == ON) || (1 == CAPT_IR_AR_CENTRE && EVITEMENT_ADV_ARRIERE == ON))
    {
        centre = 1;
    }
    if(1 == CAPT_US_AR_GAUCHE && EVITEMENT_ADV_ARRIERE == ON)
    {
        droite = 1;
    }
    if(1 == CAPT_US_AR_DROIT && EVITEMENT_ADV_ARRIERE == ON)
    {
        gauche = 1;
    }
#endif

    if(angle <= 22 || angle >= 339) //horizontale CAPT_US_DROIT
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
                offset += 0 ;
                largeur += taille;
            }

            if(gauche == 1)
            {
                offset += -taille;
                largeur += taille;
            }
        }

        for(i=0; i<vision; i++)
        {
            for(j=-1; j<=largeur+1; j++)
            {
                if(x_actuel+i+1 < x_max && y_actuel +offset +j >= 0 && y_actuel +offset +j < y_max && obstacle [x_actuel+i+1][y_actuel +offset + j] == 0)
                    obstacle [x_actuel+i+1][y_actuel +offset + j]=2;
            }
        }
    }


    else if(angle <=68 )
    {
        if(centre == 1)
        {
            largeur += 2*taille_diago;
            offset += -taille_diago;
        }

        else
        {
            if(droite == 1)
            {
                largeur +=taille_diago;
                offset +=0;
            }


            if(gauche == 1)
            {
                largeur +=taille_diago;
                offset += -taille_diago;
            }
        }

        for(i=0; i<vision; i++)
        {
            for(j=0; j<=largeur; j++)
            {
                x_obs = x_actuel+1+offset+j+i;
                y_obs = y_actuel-1+offset+j-i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                    obstacle[x_obs][y_obs] = 2;
                if(!(j == largeur && (droite || centre))  && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                    obstacle[x_obs+1][y_obs] = 2;
                if(!(j == 0 && (gauche || centre))&& x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                    obstacle[x_obs][y_obs+1] = 2;
                if(!(j == 0 && (gauche || centre))  && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                    obstacle[x_obs-1][y_obs] = 2;
                if(!(j == largeur && (droite || centre))  && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                    obstacle[x_obs][y_obs-1] = 2;
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
                offset += 0 ;
                largeur += taille;
            }

            if(gauche)
            {
                offset += -taille;
                largeur += taille;
            }

        }

        for(i=0; i<vision; i++)
        {
            for(j=-1; j<=largeur+1; j++)
            {
                if(y_actuel-i-1 >= 0 && x_actuel +offset + j>= 0 && x_actuel + offset + j < x_max && obstacle [x_actuel + offset + j][y_actuel-i-1] ==0)
                    obstacle [x_actuel + offset + j][y_actuel-i-1]=2;
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
                offset +=0;
            }


            if(droite)
            {
                largeur +=taille_diago;
                offset += taille_diago;
            }
        }

        for(i=0; i<vision; i++)
        {
            for(j=0; j<=largeur; j++)
            {
                x_obs = x_actuel-1+offset-j-i;
                y_obs = y_actuel-1-offset+j-i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                    obstacle[x_obs][y_obs] = 2;
                if(!(j == 0 && (droite || centre)) && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                    obstacle[x_obs+1][y_obs] = 2;
                if(!(j == largeur && (gauche || centre)) && x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                    obstacle[x_obs][y_obs+1] = 2;
                if(!(j == largeur && (gauche || centre)) && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                    obstacle[x_obs-1][y_obs] = 2;
                if(!(j == 0 && (droite || centre)) && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                    obstacle[x_obs][y_obs-1] = 2;
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
                offset += 0 ;
                largeur += taille;
            }


            if(droite)
            {
                offset += -taille;
                largeur += taille;
            }

        }
        for(i=0; i<vision; i++)
        {
            for(j=-1; j<=largeur+1; j++)
            {
                if(x_actuel-i-1 >=0 && y_actuel +offset + j >= 0 && y_actuel +offset + j < y_max && obstacle [x_actuel-i-1][y_actuel + offset + j]==0)
                    obstacle [x_actuel-i-1][y_actuel +offset + j]=2;
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
                offset +=0;
            }


            if(droite)
            {
                largeur +=taille_diago;
                offset += taille_diago;
            }
        }
        for(i=0; i<vision; i++)
        {
            for(j=0; j<=largeur; j++)
            {
                x_obs = x_actuel-1-offset+j-i;
                y_obs = y_actuel+1-offset+j+i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                    obstacle[x_obs][y_obs] = 2;
                if(!(j == largeur && (gauche)) && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                    obstacle[x_obs+1][y_obs] = 2;
                if(!(j == largeur && (gauche)) && x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                    obstacle[x_obs][y_obs+1] = 2;
                if(!(j == 0 && (droite)) && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                    obstacle[x_obs-1][y_obs] = 2;
                if(!(j == 0 && (droite)) && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                    obstacle[x_obs][y_obs-1] = 2;
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
                offset += 0 ;
                largeur += taille;
            }

            if(droite)
            {
                offset += -taille;
                largeur += taille;
            }
        }
        for(i=0; i<vision; i++)
        {
            for(j=-1; j<=largeur+1; j++)
            {
                if(y_actuel+i+1 < y_max && x_actuel+ offset + j>= 0 && x_actuel + offset + j < x_max && obstacle [x_actuel + offset + j][y_actuel+i+1]==0)
                    obstacle [x_actuel + offset + j][y_actuel+i+1]=2;
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
                offset +=0;
            }


            if(gauche)
            {
                largeur +=taille_diago;
                offset += taille_diago;
            }
        }
        for(i=0; i<vision; i++)
        {
            for(j=0; j<=largeur; j++)
            {
                x_obs = x_actuel+1+offset-j+i;
                y_obs = y_actuel+1-offset+j+i;
                if(x_obs>=0 && x_obs < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs][y_obs] != 1)
                    obstacle[x_obs][y_obs] = 2;
                if(!(j == 0 && (gauche)) && x_obs+1>=0 && x_obs+1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs+1][y_obs] != 1)
                    obstacle[x_obs+1][y_obs] = 2;
                if(!(j == largeur && (droite)) && x_obs>=0 && x_obs < x_max && y_obs+1 >=0 && y_obs+1 < y_max && obstacle [x_obs][y_obs+1] != 1)
                    obstacle[x_obs][y_obs+1] = 2;
                if(!(j == largeur && (droite))  && x_obs-1>=0 && x_obs-1 < x_max && y_obs >=0 && y_obs < y_max && obstacle [x_obs-1][y_obs] != 1)
                    obstacle[x_obs-1][y_obs] = 2;
                if(!(j == 0 && (gauche)) && x_obs>=0 && x_obs < x_max && y_obs-1 >=0 && y_obs-1 < y_max && obstacle [x_obs][y_obs-1] != 1)
                    obstacle[x_obs][y_obs-1] = 2;
            }
        }
    }


}

void plus_court(int x_objectif,int y_objectif,int8_t sens_marche,double pourcentage_deplacement,char last)
{
    int i,j;
    int retour=0;
    int curseur_finale;
    int curseur_obstacle_finale;
    int angle=0;
    int haut;
    int distance_courante;
    int nouvelle_distance;
    int id = id_evitement;
    id_evitement++;
    chemin_court=0;

    if(pourcentage_deplacement > 50)
    {
        pourcentage_deplacement = 50;
    }



    PutsUART(UART_XBEE,"coucou \n\r");

//
//    if(get_X() - x_objectif>20 || get_X() - x_objectif < -20 || get_Y() - y_objectif>20 || get_Y() - y_objectif < -20)
//    {


    PutsUART(UART_XBEE,"1");

    if(id == id_evitement_initial) // si c'est le premier evitement lors de cette phase de deplacement
    {
        //on donne la cellule d'objectif
        x_obj = ((int)x_objectif)/100;
        y_obj = y_max - ((int)y_objectif)/100;
        //cibler(x_objectif,y_objectif,MOYEN);
        //angle = get_orientation();
    }

    //on donne notre cellule de départ
    x_actuel = ((int) get_X())/100;
    y_actuel = y_max - ((int) get_Y())/100;

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

    nettoyage_nouvel_obstacle();// on enleve les obstacles supplémentaires qu'on a placé lors des evitements précédents
    mettre_obstacle(x_actuel,y_actuel,sens_marche); // on place notre nouvel obstacle

/*
    PutsUART(UART_XBEE, "\n\r");
    PutIntUART(x_actuel);
    PutsUART(UART_XBEE, " ");
    PutIntUART(y_actuel);
    PutsUART(UART_XBEE, "||");
    PutIntUART(x_obj);
    PutsUART(UART_XBEE, " ");
    PutIntUART(y_obj);


    PutsUART(UART_XBEE, "\n\r");
    for(j=0; j<y_max; j++)
    {
        for(i=0; i+4<x_max; i=i+5)
        {
            PutIntUART(obstacle[i][j]*10000+obstacle[i+1][j]*1000+obstacle[i+2][j]*100+obstacle[i+3][j]*10+obstacle[i+4][j]);
        }
        PutsUART(UART_XBEE, "\n\r");
    }
*/

    curseur =0;//on remet notre curseur de point itineraire au début

    //on place notre cellule actuelle comme point de départ
    itineraire[curseur][0]=x_actuel;
    itineraire[curseur][1]=y_actuel;



    if(obstacle[x_obj][y_obj]!=0 || obstacle[x_actuel][y_actuel]!=0 || (x_actuel == x_obj && y_actuel == y_obj)) // si notre objectif est en obstacle, ou notre position, ou si on est dans la meme cellule que l'objectif
    {
        //alors on va reculer puis repartir vers l'objectif

        PutsUART(UART_XBEE,"2");

        if(sens_marche == MARCHE_AVANT) // si on est en marche avant, alors on reculera donc on multipliera -100 du avancer_reculer par 1 (i), et on change l'evitement
        {
            i=1;
            EVITEMENT_ADV_AVANT = OFF;
            EVITEMENT_ADV_ARRIERE = ON;

        }
        else // si on est en marche arriere, alors on voudra avancer et non reculer, du coup on multpliera -100 par -1(i), et on change l'evitement
        {
            i=-1;
            EVITEMENT_ADV_ARRIERE = OFF;
            EVITEMENT_ADV_AVANT = ON;

        }

        angle=get_orientation();
        if(angle < 0) // get_orientation renvoi des valeurs négatives donc on les repasse en positif
        {
        angle = (int)(angle + 360)%360;
        }
        /*
        PutsUART(UART_XBEE, "\n\r l'angle est de ");
        PutIntUART(angle);
        PutsUART(UART_XBEE, "\n\r");
        PutsUART(UART_XBEE, "\n\r i est de ");
        PutIntUART(i);
        PutsUART(UART_XBEE, "\n\r");
*/

        //a partir de la, on va essayer de reculer si on peut ! les conditions qui suivent sont toutes les mêmes
        //si on est dans un certain angle, on check les 2 cases derrieres ou devant lui selon le sens de la marche
        //si elles sont libres, alors on peut reculer ou avancer
        //si on rentre en evitement dans ce reculer ou avancer, on se stoppera et on continuera la fonction d'evitement

        if(angle <= 22 || angle >= 339 ) //horizontale vers la droite
        {
            if(obstacle[x_actuel - i][y_actuel] == 0 && obstacle[x_actuel - 2*i][y_actuel] == 0 ){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 68) //diagonale en haut à droite
        {
            if( obstacle[x_actuel - i][y_actuel +1*i] == 0 && obstacle[x_actuel - 2*i][y_actuel +2*i] == 0 ){
                DETECTION = OFF;
                avancer_reculer(-100*i, 50);
            }
        }
        else if (angle <= 112) //verticale haut
        {
            if(obstacle[x_actuel][y_actuel +i] == 0 && obstacle[x_actuel][y_actuel +2*i] == 0 ){
                DETECTION = OFF;
                avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 158) //diagonale en haut à gauche
        {
            if(obstacle[x_actuel +i][y_actuel +i] == 0 && obstacle[x_actuel + 2*i][y_actuel +2*i] == 0){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 202) //horizontale gauche
        {
            if(obstacle[x_actuel +i][y_actuel] == 0 && obstacle[x_actuel + 2*i][y_actuel] == 0 ){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }
        else if (angle <= 248) //diagonale bas gauche

        {

            if(obstacle[x_actuel +i][y_actuel -i] == 0 && obstacle[x_actuel + 2*i][y_actuel -2*i] == 0 ){
                DETECTION = OFF;
                
                avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 292) //verticale bas
        {
            if( obstacle[x_actuel][y_actuel -i] == 0 && obstacle[x_actuel][y_actuel -2*i] == 0 ){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }

        else if (obstacle[x_actuel -i][y_actuel-i] == 0 && obstacle[x_actuel - 2*i][y_actuel-2*i] == 0 ) //diagonale bas droite
        {
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
        }

        //on remet l'evitement comme il etait au départ
        if(sens_marche == MARCHE_AVANT)
        {
            EVITEMENT_ADV_AVANT = ON;
            EVITEMENT_ADV_ARRIERE = OFF;
        }
        else
        {
            EVITEMENT_ADV_ARRIERE = ON;
            EVITEMENT_ADV_AVANT = OFF;
        }
/*
        PutsUART(UART_XBEE,"objectif ou actuel bloque");
        PutIntUART(x_objectif);
        PutcUART (UART_XBEE, ' ');
        PutIntUART(y_objectif);
        PutcUART (UART_XBEE, ' ');
        PutcUART (UART_XBEE, '|');
*/

//        if(id_evitement_initial == id)
//        {
//            id_evitement_initial = id_evitement;
//            //PutsUART(UART_XBEE, "ordre de fin");
//            DETECTION = OFF;
//            if(last == rej || last == FIN_TRAJECTOIRE)
//            {
//                rejoindre(x_objectif,y_objectif,sens_marche,pourcentage_deplacement);
//            }
//            else
//            {
//                passe_part(x_objectif,y_objectif,sens_marche,pourcentage_deplacement,DEBUT_TRAJECTOIRE);
//            }
//        }
    }

    else // si notre cellule et celle de l'objectif sont libres
    {


        PutsUART(UART_XBEE,"3");
        retour =evitement (x_obj,y_obj,1); // on essaye de trouver un chemin. On renvoi 0 si on a pu trouver un chemin, -1 sinon

        if(retour == 0) // si on a trouver un chemin
        {



            distance_courante = distance();// on calcul la distance entre notre de cellule traversée

            //on stocke notre itineraire comme itineraire le plus court qu'on ait pour le moment (en même temps on en a pas d'autre)
            for(i=0; i<=nb_point_max; i++)
            {
                itineraire_court[i][0] = itineraire[i][0];
                itineraire_court[i][1] = itineraire[i][1];
                itineraire_court[i][2] = itineraire[i][2];
            }

            // On stocke les différentes valeurs qui caractérise cet itineraire, comme le nombre de point et le nombre d'obstacle
            curseur_finale=curseur;
            curseur_obstacle_finale = curseur_obstacle;
            chemin_court=1;//on indique ici qu'on a au moins 1 chemin

            //tant qu'on a pas re essaye a partir de tous nos obstacles et que le garde fou n'a pas ete declenche, on va chercher un chemin plus court
            while (curseur_obstacle> 0 && compteur_evitement < ATTENTE_EVITEMENT)
            {

                //PutIntUART(curseur_obstacle);
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

                retour=evitement(x_obj,y_obj,haut);//on retente de trouver un evitement

                if(retour ==0) // si on en a trouver un
                    nouvelle_distance = distance(); //on recalcule le nombre de point
                if(retour == 0 &&  (curseur < curseur_finale || nouvelle_distance < distance_courante))  // si on a un chemin valide et plus court
                {
                    for(i=0; i<nb_point_max; i++) // on sauvegarde le nouvel itineraire
                    {
                        itineraire_court[i][0] = itineraire[i][0];
                        itineraire_court[i][1] = itineraire[i][1];
                        itineraire_court[i][2] = itineraire[i][2];
                    }
                    curseur_finale = curseur;
                    distance_courante = nouvelle_distance;
                    curseur_obstacle_finale = curseur_obstacle;
                    //PutsUART(UART_XBEE, "chemin plus court possible ! \n\r");
                }
                else   // si on a pas un chemin plus court
                {


                    for(i=0; i<nb_point_max; i++) // on remet l'itineraire comme il etait
                    {
                        itineraire[i][0] = itineraire_court[i][0];
                        itineraire[i][1] = itineraire_court[i][1];
                        itineraire[i][2] = itineraire_court[i][2];
                    }


                    curseur_obstacle = curseur_obstacle_finale; // et on recherche le curseur d'obstacle

                }
                curseur_obstacle=curseur_obstacle-2;//1 car il rencontre l'obstacle juste devant lui et 1 car on veut aller à l'obstacle d'avant
                //PutIntUART(curseur_obstacle);
            }

            if(compteur_evitement < ATTENTE_EVITEMENT || retour ==1)
            {
/*
                PutsUART (UART_XBEE, "\n\r");

                for(i=0; i<=curseur; i++)
                {
                    PutIntUART(itineraire_court[i][0]);
                    PutcUART (UART_XBEE, ' ');
                    PutIntUART(itineraire_court[i][1]);
                    PutcUART (UART_XBEE, ' ');
                    PutIntUART(itineraire_court[i][2]);
                    PutcUART (UART_XBEE, ' ');
                    PutcUART (UART_XBEE, '|');
                    PutcUART (UART_XBEE, ' ');
                }

                PutcUART (UART_XBEE, '{');
                PutIntUART(get_X());
                PutcUART (UART_XBEE, ' ');
                PutIntUART (get_Y());
                PutcUART (UART_XBEE, '}');

                PutcUART (UART_XBEE, '{');
                PutIntUART(x_objectif);
                PutcUART (UART_XBEE, ' ');
                PutIntUART (y_objectif);
                PutcUART (UART_XBEE, '}');
*/

                curseur=curseur_finale; //on reprend notre curseur de l'itineraire le plus court
                deplacement(sens_marche,pourcentage_deplacement,last);// et on lance les deplacements
                if(id == id_evitement_initial) // TODO verifier l'utilite
                {
                    id_evitement_initial = id_evitement;

      //              PutsUART(UART_XBEE, "ordre de fin");
                    DETECTION = OFF;
//                    if(last == rej)
//                    {
//                        rejoindre(x_objectif,y_objectif,sens_marche,pourcentage_deplacement);
//                    }
//                    else
//                    {
//                        if(last == DEBUT_TRAJECTOIRE)
//                        {
//                            last = MILIEU_TRAJECTOIRE;
//                        }
//                        passe_part(x_objectif,y_objectif,sens_marche,pourcentage_deplacement,last);
//                    }

                    /*if(last == FIN_TRAJECTOIRE){
                        orienter(angle,pourcentage_deplacement);
                    }*/
                }
            }
        }
        else // si on a pas trouve de chemin plus court
        {


            //PutsUART(UART_XBEE,"4");


            if(sens_marche == MARCHE_AVANT) // si on allait vers l'avant, alors on va reculer donc on multiplie le -100 d'avancer reculer par -1
            {
                i=1;
                EVITEMENT_ADV_AVANT = OFF;
                EVITEMENT_ADV_ARRIERE = ON;

            }
            else // sinon, on met à -1
            {
                i=-1;
                EVITEMENT_ADV_ARRIERE = OFF;
                EVITEMENT_ADV_AVANT = ON;

            }


            angle=get_orientation(); // on recupere notre orientation
            if(angle < 0)// si l'angle renvoye est negatif, on le repasse en positif
            {
        angle = (int)(angle + 360)%360;
            }
/*
            PutsUART(UART_XBEE, "\n\r l'angle est de ");
            PutIntUART(angle);
            PutsUART(UART_XBEE, "\n\r i est de ");
            PutIntUART(i);
            PutsUART(UART_XBEE, "\n\r");
*/

//a partir de la, on va essayer de reculer si on peut ! les conditions qui suivent sont toutes les mêmes
        //si on est dans un certain angle, on check les 2 cases derrieres ou devant lui selon le sens de la marche
        //si elles sont libres, alors on peut reculer ou avancer
        //si on rentre en evitement dans ce reculer ou avancer, on se stoppera et on continuera la fonction d'evitement


        if(angle <= 22 || angle >= 339 ) //horizontale droite
        {
            if(obstacle[x_actuel - i][y_actuel/100] == 0 && obstacle[x_actuel - 2*i][y_actuel/100] == 0 ){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 68) //diagonale haut droite
        {
            if( obstacle[x_actuel - i][y_actuel +1*i] == 0 && obstacle[x_actuel - 2*i][y_actuel +2*i] == 0 ){
                DETECTION = OFF;
                avancer_reculer(-100*i, 50);
            }
        }
        else if (angle <= 112) //verticale haut
        {
            if(obstacle[x_actuel][y_actuel +i] == 0 && obstacle[x_actuel][y_actuel +2*i] == 0 ){
                DETECTION = OFF;
                avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 158) //diagonale haut gauche
        {
            if(obstacle[x_actuel +i][y_actuel +i] == 0 && obstacle[x_actuel + 2*i][y_actuel +2*i] == 0){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 202) //horizontale gauche
        {
            if(obstacle[x_actuel +i][y_actuel] == 0 && obstacle[x_actuel + 2*i][y_actuel] == 0 ){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }
        else if (angle <= 248) //diagonale bas gauche
        {
            if(obstacle[x_actuel +i][y_actuel -i] == 0 && obstacle[x_actuel + 2*i][y_actuel -2*i] == 0 ){
                DETECTION = OFF;
                
                avancer_reculer(-100*i, 50);
            }
        }

        else if (angle <= 292) //verticale bas
        {
            if( obstacle[x_actuel][y_actuel -i] == 0 && obstacle[x_actuel][y_actuel -2*i] == 0 ){
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
            }
        }

        else if (obstacle[x_actuel -i][y_actuel-i] == 0 && obstacle[x_actuel - 2*i][y_actuel-2*i] == 0 ) //diagonale bas droite
        {
            DETECTION = OFF;
            avancer_reculer(-100*i, 50);
        }


// on remet l'evitement comme il était
            if(sens_marche == MARCHE_AVANT)
            {
                EVITEMENT_ADV_AVANT = ON;
                EVITEMENT_ADV_ARRIERE = OFF;

            }
            else
            {
                EVITEMENT_ADV_ARRIERE = ON;
                EVITEMENT_ADV_AVANT = OFF;

            }
/*
            PutsUART(UART_XBEE,"cul de sac");
            PutIntUART(x_objectif);
            PutcUART (UART_XBEE, ' ');
            PutIntUART(y_objectif);
            PutcUART (UART_XBEE, ' ');
            PutcUART (UART_XBEE, '|');
*/

//            if (id == id_evitement_initial)
//            {
//                id_evitement_initial = id_evitement;
//                //PutsUART(UART_XBEE, "ordre de fin");
//                DETECTION = OFF;
//                if(last == rej)
//                {
//                    rejoindre(x_objectif,y_objectif,sens_marche,pourcentage_deplacement);
//                }
//                else
//                {
//                    passe_part(x_objectif,y_objectif,sens_marche,pourcentage_deplacement,DEBUT_TRAJECTOIRE);
//                }
//            }
        }
    }
    //}

    id_evitement_initial = id_evitement;
    DETECTION = OFF;

    //on redonne la consigne de base
    if(last == rej || last == FIN_TRAJECTOIRE)
    {
        rejoindre(x_objectif,y_objectif,sens_marche,pourcentage_deplacement);
    }
    else
    {
        passe_part(x_objectif,y_objectif,sens_marche,pourcentage_deplacement,DEBUT_TRAJECTOIRE);
    }


    //PutsUART(UART_XBEE,"5");

    DETECTION = OFF;

}
