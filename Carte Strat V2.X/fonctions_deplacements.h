/******************************************************************************/
/************** Carte polyvalente Botik  : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: fonctions_deplacements.h
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 14 décembre 2014, 15:00
 *******************************************************************************
 *
 *
 ******************************************************************************/

#ifndef FONCTIONS_DEPLACEMENTS_H
#define	FONCTIONS_DEPLACEMENTS_H

#ifdef	__cplusplus
extern "C" {
#endif

 
/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

    // Define de HUGO ...(c'est pour ça qu'il y a des minuscules ...)
    #define y_max 20
    #define x_max 30
    #define nb_point_max 100
    #define MOYEN 80
    #define rej 9
    #define ATTENTE_EVITEMENT 150
    #define EFFACER 0
    #define AJOUTER 2

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

/******************************************************************************/
/**************************** FONCTIONS COULEURS ******************************/
/******************************************************************************/

    
    
/**
 * Fonction qui retourne la valeur du bouton de couleur pour savoir de quel côté du terrain on démare
 * @return JAUNE ou VERT
 */
uint8_t couleur_depart();

/**
 * Cette fonction permet d'inverser n'importe quelle paramètre selon la couleur de départ
 * (Si on démare du côté opposé, alors y est inversé et l'angle aussi ..)
 *
 * @param   param_inversable : le paramètre qui peut changer de signe en fonction de la couleur
 * @return  renvoit le paramètre signé selon la couleur de départ
 */
double inversion_couleur (double param_inversable);


/******************************************************************************/
/********************* FONCTIONS USUELLES (TEST ASSERV) ***********************/
/******************************************************************************/

void faire_des_tours (int nb_tour);

void trapeze (int8_t sens_marche);

void petit_demi_cercle (int8_t sens_marche);

void grand_demi_cercle (int8_t sens_marche);

void carre (int8_t sens_marche);

/******************************************************************************/
/******************* FONCTIONS ASSERV HAUT NIVEAU (EVITEMENT) *****************/
/******************************************************************************/


void action_evitement (void);

/**
 * Fonction qui sert à cibler un point de coordonnées ( X, Y ) en mm !!!
 * @param x : Coordonée X en mm
 * @param y : Coordonée Y en mm
 * @param pourcentage_vitesse : Vitesse désirée de 0 à 100 %
 */
void cibler (double x, double y, double pourcentage_vitesse);


/**
 * Fonction qui sert à orienter le robot par rapport à son repère de départ
 * @param angle : valeur en degré
 * @param pourcentage_vitesse : Vitesse désiré de 0 à 100%
 */
void orienter (double angle, double pourcentage_vitesse);


/**
 * Fonction qui oriente le robot et se déplace vers les coordonnées (X, Y) en mm !!!
 * @param x : Coordonnée X en mm
 * @param y : Coordonnée Y en mm
 * @param sens_marche : MARCHE_AVANT ou MARCHE_ARRIERE
 * @param pourcentage_vitesse : Vitesse désirée de 0 à 100 %
 */
void rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse);


/**
 * Fonction qui fait avancer le robot de x mm (ATTENTION : PAS D'ASSERVISSEMENT EN ANGLE !!!)
 * @param distance : distance en mm (distance négative pour reculer)
 * @param pourcentage_vitesse : Vitesse désirée de 0 à 100%
 */
void avancer_reculer (double distance, double pourcentage_vitesse);


/**
 * Fonction qui permet de passer par plusieurs points sans s'arrêter.
 * Il faut définir des points par lesquels on veut que le robot passe. En pratique il passe à 15 cm de ces points !
 * \n\n
 * Attention à bien définir le déplacement, chaque trajectoire à un début, des déplacements de milieu et une fin.
 * Si on ne commence pas la trajectoire par un début, le robot ne démarera pas, si on ne termine pas par une fin de trajectoire, le robot ne freinera pas, et continuera à avancer à pleine vitesse ...
 * \n
 * @param x : Coordonnée X du point par lequel passer (en mm)
 * @param y : Coordonnée Y du point par lequel passer (en mm)
 * @param sens_marche : MARCHE_AVANT ou MARCHE_ARRIERE
 * @param pourcentage_vitesse : Vitesse désiré de 0 à 100 % (voire plus .. 100% = vitesse max calculée par le robot, mais on peut parfois aller plus vite)
 * @param last : DEBUT_TRAJECTOIRE, MILIEU_TRAJECTOIRE, FIN_TRAJECTOIRE (NE PAS SE TROMPER !!!!!!!)
 */
void passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last);


/******************************************************************************/
/**************** FONCTIONS ASSERV HAUT NIVEAU CALAGES BORDURES ***************/
/******************************************************************************/

/**
 * Fonction pour faire un calage quelconque
 * \n
 * Au moindre obstacle le déplacement s'arrêtera/
 * Aucun paramètre de position n'est réinitialisé pendant l'opération
 * \n
 * A UTILISER EN CAS DE CALAGE INCERTAIN !!
 * @param distance : distance en mm à parcourir (prévoir plus grand)
 * @param pourcentage_vitesse : Vitesse de 0 à 100 %
 */
void calage (double distance, double pourcentage_vitesse);


/**
 * Recalage de l'axe X et de l'angle Teta contre une bordure
 * /!\ Réinitialisation automatique des paramètres dès la détection de blocage /!\
 * \n
 * /!\ Pas de vérification de la cohérence de la position /!\
 * @param x : Nouvelle position en X en mm
 * @param teta : Nouvel angle Teta en degré
 * @param sens_marche : sens dans lequel le robot doit se caler
 * @param pourcentage_vitesse : Vitesse de 0 à 100 %
 */
void calage_X (double x, double teta, int8_t sens_marche, double pourcentage_vitesse);


/**
 *  * Recalage de l'axe Y et de l'angle Teta contre une bordure
 * /!\ Réinitialisation automatique des paramètres dès la détection de blocage /!\
 * \n
 * /!\ Pas de vérification de la cohérence de la position /!\
 * @param y : Nouvelle position en Y en mm
 * @param teta : Nouvel angle Teta en degré
 * @param sens_marche : sens dans lequel le robot doit se caler
 * @param pourcentage_vitesse : Vitesse de 0 à 100%
 */
void calage_Y (double y, double teta, int8_t sens_marche, double pourcentage_vitesse);


/**
 *  * Recalage de l'angle Teta contre une bordure
 * /!\ Réinitialisation automatique des paramètres dès la détection de blocage /!\
 * \n
 * /!\ Pas de vérification de la cohérence de la position /!\
 * @param teta : Nouvel angle Teta en degré
 * @param sens_marche : sens dans lequel le robot doit se caler
 * @param pourcentage_vitesse : Vitesse de 0 à 100%
 */
void calage_teta (double teta, int8_t sens_marche, double pourcentage_vitesse);


/******************************************************************************/
/***************** FONCTIONS ASSERV BAS NIVEAU (config asserv) ****************/
/******************************************************************************/



_enum_erreur_asserv _calage (double distance, double pourcentage_vitesse);

_enum_erreur_asserv _cibler (double x, double y, double pourcentage_vitesse);

_enum_erreur_asserv _orienter (double angle, double pourcentage_vitesse);

_enum_erreur_asserv _rejoindre (double x, double y, int8_t sens_marche, double pourcentage_vitesse);

_enum_erreur_asserv _avancer_reculer (double distance, double pourcentage_vitesse);

_enum_erreur_asserv _passe_part (double x, double y, int8_t sens_marche, double pourcentage_vitesse, char last);

/**
 * Fonction qui ajuste en temps réel la consigne d'angle pour que le robot
 * fasse plusieurs tous dans s'arrêter
 * \n
 * /!\ Cette fonction n'est pas prévu pour une utilisation manuelle /!\
 * /!\ Elle a pour but d'être appellée par une fonction plus haut niveau : faire_des_tours() /!\
 * @param angle : angle consigne
 * @param last : DEBUT_TRAJECTOIRE | FIN_TRAJECTOIRE
 */
void _fdt (double angle, char last);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    void mettre_obstacle(int x_actuel, int y_actuel,int8_t sens_marche);
    int conversion_direction(int direction);//transforme la direction du blocage en une direction de longement
    int longement ( int x_objectif,int y_objectif,int direction_longement);//longe un obstacle selon une direction de longement donnée
    int tracer_ligne_x(int x_objectif); //cree une ligne droite suivant l'axe des x. Fais avancer ou reculer sur l'axe des x
    int tracer_ligne_y(int y_objectif); //cree une ligne droite suivant l'axe des y. Fais avancer ou reculer sur l'axe des y
    void post_traitement(); //simplifie le trajet en elevant le maximum de deplacements inutiles
    void deplacement(int8_t sens_marche,double pourcentage_deplacement,char last); //transforme l'itineraire en commande
    int evitement_hugo(int x_objectif,int y_objectif,int haut); //determine l'itineraire pour aller de notre position a l'objectif en tenant compte des obstacles eventuels
    void init_evitement_hugo();
    int distance();
    int aiguillage_evitement(int x_objectif, int y_objectif, int direction,int haut);
    void plus_court(int x_objectif,int y_objectif,int8_t sens_marche,double pourcentage_deplacement,char last,int ajout);

#ifdef	__cplusplus
}
#endif

#endif	/* FONCTIONS_DEPLACEMENTS_H */

