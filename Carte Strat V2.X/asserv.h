/* 
 * File:   asserv.h
 * Author: Quentin
 *
 * Created on 30 octobre 2014, 23:11
 */

/**
 * @file asserv.h : Prototypes des fonctions d'asserv, enum et structures des variables d'asserv
 */

#ifndef ASSERV_H
#define	ASSERV_H


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "codeurs.h"

/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

//#define ON                          true
//#define OFF                         false

#define Pi                          3.14159265359

//#define ROUE_DROITE                 0
//#define ROUE_GAUCHE                 1
//#define SYS_ROBOT                   2

//#define ASSERV_VITESSE_DISTANCE     0
//#define ASSERV_POSITION             1
//#define ASSERV_ORIENTATION          2
//#define KP_HYBRIDE                  3

//#define ORIENTER                    0
//#define AVANCER                     1
//#define PASSE_PART                  2
//#define FAIRE_DES_TOURS             3

//#define DEBUT_DEPLACEMENT           0
//#define FIN_DEPLACEMENT             1
//#define ANGLE_ATTEINT               2
//#define DISTANCE_ATTEINTE           4
//#define EN_COURS                    6
//#define PHASE_NORMAL                7

//#define DEBUT_TRAJECTOIRE           0
//#define MILIEU_TRAJECTOIRE          2
//#define FIN_TRAJECTOIRE             1

//#define MM                          1
//#define XY                          2


/******************************************************************************/
/****************************** REGLAGE DU PID ********************************/
/******************************************************************************/


#define VITESSE_DIS_KP              _VITESSE_DIS_KP
#define VITESSE_DIS_KI              _VITESSE_DIS_KI
#define VITESSE_DIS_KD              _VITESSE_DIS_KD

#define MAX_ERREUR_INTEGRALLE_V     _MAX_ERREUR_INTEGRALLE_V
#define MAX_E_INTEGRALLE_BRAKE      _MAX_E_INTEGRALLE_BRAKE

#define POSITION_KP                 _POSITION_KP
#define POSITION_KI                 _POSITION_KI
#define POSITION_KD                 _POSITION_KD

#define ORIENTATION_KP              _ORIENTATION_KP
#define ORIENTATION_KI              _ORIENTATION_KI
#define ORIENTATION_KD              _ORIENTATION_KD

/******************************************************************************/
/************************* CONTRAINTES MECANIQUES *****************************/
/******************************************************************************/

#define ENTRAXE_MM                  _ENTRAXE_MM
#define DIAMETRE_ROUE_CODEUSE       _DIAMETRE_ROUE_CODEUSE
#define PERIMETRE_ROUE_MM           _PERIMETRE_ROUE_MM

#define COEF_D                      _COEF_D
#define COEF_G                      _COEF_G


#define MM_PAR_TICKS                (double) ( PERIMETRE_ROUE_MM / CODEUR_D_NB_IMP_LOG )
#define TICKS_PAR_MM                (double) ( CODEUR_D_NB_IMP_LOG / PERIMETRE_ROUE_MM )
#define ENTRAXE_TICKS               (double) ( CODEUR_D_NB_IMP_LOG * ENTRAXE_MM / PERIMETRE_ROUE_MM )

/******************************************************************************/
/************************* CONSIGNES DIVERSES *********************************/
/******************************************************************************/

//#define SEUIL_DISTANCE_MINI_MM      1
//#define SEUIL_DISTANCE_MINI_PAS     (int32_t) ( SEUIL_DISTANCE_MINI_MM * TICKS_PAR_MM )
//
//#define SEUIL_ORIENTATION_MINI      1
//#define SEUIL_ORIENTATION_MINI_PAS  (int32_t) ( SEUIL_ORIENTATION_MINI * Pi / 180 * (ENTRAXE_TICKS/2) )

#define SEUIL_IMMOBILITE            _SEUIL_IMMOBILITE

#define CONSIGNE_MAX                100.

    //Association Distance - Vitesse - Accélérations Max
#define VITESSE_CONSIGNE_MAX_MM     _VITESSE_CONSIGNE_MAX_MM
#define VITESSE_DISTANCE_MIN        _VITESSE_DISTANCE_MIN
#define VITESSE_MAX_MM_TENSION      _VITESSE_MAX_MM_TENSION
#define VITESSE_CONSIGNE_MAX_PAS    ( VITESSE_CONSIGNE_MAX_MM * TICKS_PAR_MM )
#define VITESSE_DISTANCE_MIN_PAS    ( VITESSE_DISTANCE_MIN * TICKS_PAR_MM )
#define VITESSE_MAX_TENSION         ( VITESSE_MAX_MM_TENSION * TICKS_PAR_MM )

#define DISTANCE_CONSIGNE_MM        _DISTANCE_CONSIGNE_MM
#define DISTANCE_CONSIGNE_PAS       ( DISTANCE_CONSIGNE_MM * TICKS_PAR_MM )

#define ACC_POSITION_CONSIGNE       _ACC_POSITION_CONSIGNE
#define DCC_POSITION_CONSIGNE       _DCC_POSITION_CONSIGNE
#define ACC_POSITION_MIN            _ACC_POSITION_MIN
#define DCC_POSITION_MIN            _DCC_POSITION_MIN

#define COEF_FREINAGE               _COEF_FREINAGE

    //Association Orientation - Vitesse - Accélération Max
#define VITESSE_ANGLE_MAX           _VITESSE_ANGLE_MAX
#define VITESSE_ANGLE_MIN           _VITESSE_ANGLE_MIN
#define VITESSE_ANGLE_PAS           ( VITESSE_ANGLE_MAX * (ENTRAXE_TICKS / 2.) )
#define VITESSE_ANGLE_MIN_PAS       ( VITESSE_ANGLE_MIN * (ENTRAXE_TICKS / 2.) )

#define ORIENTATION_CONSIGNE_DEG    _ORIENTATION_CONSIGNE_DEG
#define ORIENTATION_CONSIGNE_PAS    ( ORIENTATION_CONSIGNE_DEG * Pi /180. * (ENTRAXE_TICKS /2.) )

#define ACC_ORIENTATION_CONSIGNE    _ACC_ORIENTATION_CONSIGNE
#define DCC_ORIENTATION_CONSIGNE    _DCC_ORIENTATION_CONSIGNE
#define ACC_ORIENTATION_MIN         _ACC_ORIENTATION_MIN
#define DCC_ORIENTATION_MIN         _DCC_ORIENTATION_MIN


/******************************************************************************/
/************************** DEFINES DES ENUMS *********************************/
/******************************************************************************/

/**
 *  IL FAUT FAIRE TRES ATTENTION AVEC LES ENUMS 
 *  CERTAINES STRUCTURES QUI UTILISENT SES ENUMS SONT DEFINIES PAR DES CHAMPS DE BITS
 *  SI VOUS AUGMENTEZ LE NOMBRE DE CAS ET QUE VOUS DEPASSER LA LIMITE ALORS 
 *  VOUS REVIENDREZ A ZERO
 * 
 *  EN CAS DE MODIFICATION NECESSAIRE PENSEZ A ADAPTER LE BIT FIELD DES 
 *  STRUCTURES DEPENDANTES COMME FLAG_ASSERV
 * 
 *  EGALEMENT : MERCI DE TENIR COMPTE QU'UNE STRUCTURE EST DEFINIE PAR PAQUET DE
 *  DEUX OCTETS 
 *  IL EST JUDICIEUX D'EVITER DE PRENDRE DEUX OCTETS EN MEMOIRE JUSTE POUR 1 BIT  
 *
 */

/**
 * Enum booléene pour activer ou non les asserv
 * 
 * /!\ VARIABLE CODEE SUR 1 BIT /!\
 * /!\      ID MAXIMUM = 1      /!\
 */
typedef enum
{
    // /!\ VALEUR MAX = 1 /!\ //
    OFF = false,    // 0
    ON  = true      // 1
}_enum_on_off;

/**
 * man _enum_roue
 * Enum utilisée pour les tableaux contenant les valeurs de la roue droite / gauche
 * ou du robot en général
 */
typedef enum
{
    ROUE_DROITE,
    ROUE_GAUCHE,
    SYS_ROBOT
}_enum_roue;


/**
 * Enum utilisée dans la fonction_PID() pour savoir quel PID elle doit calculer
 */
typedef enum
{
    ASSERV_VITESSE_DISTANCE,
    ASSERV_POSITION,
    ASSERV_ORIENTATION,
    KP_HYBRIDE
}_enum_type_PID;

/**
 * Enum pour le sens du déplacement du robot (avancer / reculer)
 */
typedef enum
{
    MARCHE_AVANT = 1,
    ROTATION_POSITIVE = 1,
    MARCHE_ARRIERE = -1,
    ROTATION_NEGATIVE = -1
}_enum_sens_deplacement;


/**
 * Enum pour savoir dans quel type de déplacement on est.
 * 
 * /!\ VARIABLE CODEE SUR 3 BIT !!! /!\
 * /!\     ID MAXIMUM = 7           /!\ 
 */
typedef enum 
{
    // /!\ NE SURTOUT PAS DEPASSER 7 !! /!\ //
    ORIENTER,           // 0
    AVANCER,            // 1
    PASSE_PART,         // 2
    FAIRE_DES_TOURS     // 3
}_enum_type_deplacement;

/**
 *  Enum utilisée pour représenter tous les types de déplacements
 *  Egalement pour définir l'état de certains asserv
 *  Aussi pour les passe_part 
 * 
 * /!\ VARIABLE CODEE SUR 3 BIT !!! /!\
 * /!\        ID MAXIMUM = 7        /!\          
 */
typedef enum 
{
    // /!\ NE SURTOUT PAS DEPASSER 7 !! /!\ //
    DEBUT_TRAJECTOIRE,                      // 0
    DEBUT_DEPLACEMENT = DEBUT_TRAJECTOIRE,  // 0
    MILIEU_TRAJECTOIRE,                     // 1
    FIN_TRAJECTOIRE,                        // 2
    FIN_DEPLACEMENT = FIN_TRAJECTOIRE,      // 2
    EN_COURS,                               // 3
    PHASE_NORMAL,                           // 4
    ANGLE_ATTEINT,                          // 5
    DISTANCE_ATTEINTE                       // 6
}_enum_etat_deplacement;


/**
 *  Enum utilisée pour représenter le mode de consigne pour le déplacement
 *  Soit la consigne est donnée en MM soit par une position en XY
 *  Le calcul d'asserv n'est alors pas le même
 * 
 * /!\ VARIABLE CODEE SUR 2 BIT !!! /!\
 * /!\         ID MAXIMUM :  3      /!\
 */
typedef enum 
{
    // /!\ NE SURTOUT PAS DEPASSER 3 /!\ //
    MM,     // 0
    XY      // 1
}_enum_type_consigne;

/**
 * Enum qui reprend les cas d'erreur pour sortir d'une fonction de deplacement
 * 
 * /!\ VARIABLE CODEE SUR 3 BIT !!! /!\
 * /!\         ID MAXIMUM : 7       /!\
 */
typedef enum 
{
    // /!\ NE SURTOUT PAS DEPASSER 7 /!\ //
    DEPLACEMENT_NORMAL,     // 0
    BLOCAGE,                // 1
    EVITEMENT               // 2
}_enum_erreur_asserv;

/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

    /**
     *  Structure qui reprend les deux variables asservies en vitesse
     */
    typedef struct
    {
        double orientation;
        double position;
    }_vitesse;
    
    
    /**
     *  Structure de sous ensemble pour l'accélération
     *  min / max = config
     *  consigne = valeur à asservir
     */
    typedef struct 
    {
        double min;
        double max;
        double consigne;
    }_conf_cons;
    
    /**
     *  Strcutre qui reprend les deux variables asservies pour l'accélération
     */
    typedef struct
    {
        _conf_cons orientation;
        _conf_cons position;
    }_type_acc;

    /**
     *  Structure pour stocker les données relatives aux accélération et 
     *  Déccélérations pour les deux variables asservies en vitesse
     */
    typedef struct
    {
        _type_acc acceleration;
        _type_acc deceleration;
    }_acc;

    /**
     * Définition d'un PID numérique
     */
    typedef struct
    {
        double KP;
        double KI;
        double KD;
        
        double max_I;
	    uint64_t seuil_immobilite;
    }_coef_PID;

    /**
     * Stockage des 3 PID EXISYANT (même si le PID Distance est useless .. )
     * Les deux vraies données asservies sont la vitesse rectiligne et la 
     * vitesse en orientation
     */
    typedef struct
    {
        _coef_PID VITESSE_DIS;
        _coef_PID ORIENTATION;
        _coef_PID DISTANCE;
        _coef_PID BRAKE;
    }_PID;

    /**
     * Principale donnée d'une variable asservie : 
     * - Sa consigne 
     * - ?? a redéfinir ;) même moi j'oublie mon asserv
     * - sa valeure réelle 
     */
    typedef struct
    {
        double consigne;
        double theorique;
        double actuelle;
    }_systeme_asserv;

    /**
     * Commande en % de tension envoyée aux moteurs
     */
    typedef struct
    {
        double droit;
        double gauche;
        double max;
    }_commande_moteur;

    
    /**
     * Variable pour stocker les principales données des variables asservies 
     * pour le calcul du PID
     * à savoir, l'erreur actuelle, l'erreur n-1, la somme des erreurs
     * l'erreur dérivée est elle calculée directement à chaque coup et non sauvegardée
     * 
     * le maximum est non utilisé pour le momment : à changer
     */
    typedef struct
    {
        double actuelle;
        double integralle;
        double precedente;
        double maximum;
    }_erreur;

    
    /**
     *  Grosse structure pour définir gérer tout l'asserv et les modes en marche ou non
     * 
     *  /!\ Cette structure est définie avec des Bit field /!\
     *  /!\ Faire attention à ne pas dépasser leur valeur max si modif des enums /!\ 
     */
    typedef struct
    {
        _enum_on_off totale                                     : 1; // 1
        _enum_on_off position                                   : 1; // 2
        _enum_on_off vitesse                                    : 1; // 3
        _enum_on_off orientation                                : 1; // 4
        _enum_on_off brake                                      : 1; // 5
        _enum_on_off vitesse_fin_nulle                          : 1; // 6
        _enum_type_consigne type_consigne                       : 2; // 8
        
        _enum_sens_deplacement sens_deplacement                 : 3; // 11
        _enum_sens_deplacement sens_rotation                    : 3; // 14 
        _enum_etat_deplacement etat_angle                       : 3; // 17
        _enum_etat_deplacement etat_distance                    : 3; // 20
        _enum_etat_deplacement fin_deplacement                  : 3; // 23
        _enum_etat_deplacement phase_decelaration_orientation   : 3; // 26
        _enum_etat_deplacement phase_deceleration_distance      : 3; // 29 
        _enum_type_deplacement type_deplacement                 : 3; // 32
        
        _enum_erreur_asserv erreur                              : 3; // 35
        unsigned                                                : 13;// 48
        uint64_t immobilite;
    }_flag_asserv;

/******************************************************************************/
/**************************** ODOMETRIE ***************************************/
/******************************************************************************/

    /*
     * Structure qui contient les positions calculées pour le robot
     */
    typedef struct
    {
        double orientation_degre;
        double orientation;
        double X_mm;
        double Y_mm;
    }_robot;

/******************************************************************************/
/******************* FONCTIONS ASSERVISSEMENT BRAKE ****************************/
/******************************************************************************/    

/**
 * Asservissement qui permet d'asservir le robot à une position précise
 */
void asserv_brake(void);

/**
 * Fonction qui arrête immédiatement le robot
 */
void brake();

/**
 * Fonction qui permet au robot de reprendre son déplacement (terminer sa fonction de déplcaement) après un brake
 */
void unbrake (void);


/******************************************************************************/
/************************** FONCTIONS ECRETAGES *******************************/
/******************************************************************************/

/**
 *
 * @param type
 */
 void saturation_vitesse_max (unsigned char type);


 /**
  * 
  */
 void saturation_erreur_integralle_vitesse (void);

 /*****************************************************************************/
 /************************* FONCTIONS DE CALCULS ******************************/
 /*****************************************************************************/

 /**
  * Fonction qui calcule la consigne de distance à partir de consigne en X et Y
  */
 void calcul_distance_consigne_XY (void);


 /**
  * Fonction qui calcul la vitesse du robot à atteindre
  * @param pourcentage_vitesse : pourcentage de la vitesse maximum
  */
 void calcul_vitesse_position (double pourcentage_vitesse);


 /**
  * Fonction qui calcul l'accélération pour générer les rampes
  */
 void calcul_acceleration_position (void);


 /**
  * Calcul la vitesse à atteindre dans la phase constante du déplacement
  * @param pourcentage_vitesse : % de la vitesse maximum théorique calculer à appliquer
  */
 void calcul_vitesse_orientation (double pourcentage_vitesse);


 /**
  * Fonction qui calcul l'accélération er décélération du robot à chaque déplacement
  * Elle est calculée avec un seuil mini pour pas rester à l'arrêt comme un con
  * et maxi pour ne pas drifter sur le terrain.
  * Entre ces deux valeurs, un produit en croix en fonction de la distance à 
  * parcourir donne l'accélération requise
  */
 void calcul_acceleration_orientation (void);

 /*****************************************************************************/
 /**************************** FONCTIONS GETTERS ******************************/
 /*****************************************************************************/

 /**
  * Fonction qui renvoit la coordonnée x du robot en mm
  * @return : x en mm
  */
double get_X (void);


/**
 * Fonction qui renvoit la coordonnée y du robot en mm
 * @return : y en mm
 */
double get_Y (void);


/**
 * Fonction qui renvoit l'orientation du robot en degré
 * @return : angle en degré
 */
double get_orientation (void);

/******************************************************************************/
/*********************** FONCTIONS INIT POSITIONS *****************************/
/******************************************************************************/

/**
 *  Fonction qui permet d'initialiser le robot à une position précise (centre du robot)
 * @param x0 : en mm
 * @param y0 : en mm
 * @param teta0 : en degré
 */
void init_position_robot (double x0, double y0, uint32_t teta0);


/**
 * Permet de réinitialiser la position x du robot (centre du robot)
 * @param x : position x en mm
 */
void init_X (double x);


/**
 * Permet de réinitialiser la position y du robot (centre du robot)
 * @param y : Coordonée y en mm
 */
void init_Y (double y);


/**
 * Permet de réinitialiser l'orientation du robot par rapport au repère de départ
 * \n
 * 0° = longueur du terrain\n
 * -90° = vers la porte du local
 *  90° = de l'autre côté
 * @param teta : angle en °
 */
void init_orientation (double teta);


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/**
 * Fonction qui permet de calculer la position du robot.
 * C'est la fonction ODOMETRIE.
 *
 * Elle est appelée toutes les 5 ms par l'asserv.
 */
void calcul_position_robot (void);

/**
 * Fonction qui permet de finir avec uniquement une consigne moteur à 0
 */
void fin_deplacement_sans_brake (void);

/**
 * Fonction qui permet de terminer un déplacement avec arrêt sur position
 */
void fin_deplacement_avec_brake(void);

/**
 * Fontion qui incrémente l'immobilité en cas de blocage
 */
void detection_blocage (void);

/******************************************************************************/
/************************ FONCTIONS INITS DIVERSES ****************************/
/******************************************************************************/

/**
 * PErmet de réinitialiser les variables de commande moteur
 *
 * Important car on écrase pas la valeur à chaque itération mais ou lui rajoutes la nouvelle
 */
void init_commande_moteur(void);

/**
 * Fonction qui initialise les flag de l'asserv pour démarer l'asserv
 */
void init_flag_asserv();

/**
 * Fonction qui écrete les commandes moteurs pour ne pas envoyer plus que la tension nominale des moteurs
 */
void ecretage_consignes(void);

/**
 * A utiliser pour remettre l'asserv ) 0 avant un nouveau déplacement
 */
void reinit_asserv(void);


/******************************************************************************/
/***************************** ASSERVISSEMENT *********************************/
/******************************************************************************/


/**
 * génère les consignes qui permettent de génrer les rampes
 */
void asserv_distance(void);

/**
 * génère les rampes de vitesse : accélération, décélération, phase constante
 */
void asserv_vitesse_distance (void);

/**
 * génère les consignes d'asserv en orientation
 */
void asserv_orientation (void);

/**
 * Génère les rampes de l'asserv en vitesse
 */
void asserv_vitesse_orientation (void);

/**
 * permet de lancer toutes les fonctions d'asservissement !!
 */
void asserv();

/******************************************************************************/
/******************************* FONCTIONS PID ********************************/
/******************************************************************************/

/**
 * Fonction qui calcul le PID
 *
 * Asserv position : renvoit la valeur de l'erreur multiplié par le PID
 *
 * Asserv de vitesse_distance : applique la commande moteur de la roue droite et gauche
 *
 * @param type : #define ASSERV_VITESSE_DISTANCE, ASSERV_POSITION, ASSERV_ORIENTATION
 * @return
 */
double fonction_PID (unsigned char type);


/**
 * inscrit les valeurs des KI, KP, KD dans leurs variables respectives.
 */
void reglage_PID (void);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#endif	/* ASSERV_H */

