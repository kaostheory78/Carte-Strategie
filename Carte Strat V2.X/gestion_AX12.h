/* 
 * File:   gestion_AX12.h
 * Author: Quentin
 *
 * Created on 28 janvier 2015, 13:23
 */

#ifndef GESTION_AX12_H
#define	GESTION_AX12_H

#ifdef	__cplusplus
extern "C" {
#endif


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/



    /**************************************************************************/
    /********************** COMMANDES AX - 12 *********************************/
    /**************************************************************************/


#define START_BYTE              0xFF
    
// /!\ VARIABLE CODEE SUR 16 BIT !!! /!\ //
// /!\      VALEUR MAX : 65535       /!\ //
#define TIME_LIMIT              1350
    
// /!\ VARIABLE CODEE SUR 4 BIT !!! /!\ //
// /!\      VALEUR MAX : 15         /!\ //
#define MAX_TENTATIVES          10
    
    
#define CKECK_LIMITATION_COURANT _CKECK_LIMITATION_COURANT 
    
#define _ID                     2
#define LONGUEUR                3
#define INSTRUCTION             4
#define PARAM1                  5
#define PARAM2                  6
#define PARAM3                  7
#define PARAM4                  8
#define PARAM5                  9
#define CHSUM                   10

        //instructions � envoyer
#define NC                  0x00
#define PING                0x01
#define READ_DATA           0x02
#define WRITE_DATA          0x03
#define REG_WRITE           0x04
#define ACTION              0x05
#define RESET_AX            0x06

    //nb de param�tres
#define _2PARAM             0x02
#define _3PARAM             0x03
#define _4PARAM             0x04
#define _5PARAM             0x05
#define _6PARAM             0x06
#define _ALL_PARAM          0x07


//types de donn�es � lire :
#define LIRE_TEMPS_REPONSE  0x05
#define LIRE_POSITION_ACTU  0x24
#define LIRE_POSITION_CONSIGNE 0x30
#define LIRE_VITESSE_ACTU   0x26
#define LIRE_TENSION        0x2A
#define LIRE_TEMPERATURE    0x2B
#define LIRE_MOUV_FLAG      0x2E


    //flag param�tres fonctions
#define SANS_ATTENTE        0
#define AVEC_ATTENTE        1

#define TOUTES_LES_INFOS    2
#define READ_ONLY           1
#define AUCUNE_INFO         0


    //param�tres rotation (roue libre)
#define MODE_INFINI         5
#define MODE_NORMAL         6

#define ANTI_HORAIRE        0
#define HORAIRE             1


    /**************************************************************************/
    /********************* Reglage synchro AX - 12 ****************************/
    /**************************************************************************/

    //param�tres r�glage d�calage
#define PAS_DE_SYMETRIQUE       0
    
    typedef enum 
    {
        // /!\ ENUM CODEE SUR 2 BIT !!! /!\ //
        // /!\      VALEUR MIN : -2     /!\ //
        // /!\      VALEUR MAX :  1     /!\ //
        SENS_INDIRECT   = -1,
        ROT_EN_BAS      = -1,
        SENS_DIRECT     =  1,
        ROT_EN_HAUT     =  1
    }_enum_sens_rotation;
    
    typedef enum
    {
        // /!\ ENUM CODEE SUR 1 BIT !!! /!\ //
        // /!\      VALEUR MAX : 1      /!\ //
        DEPENDANT,      // 0
        INDEPENDANT     // 1
    }_enum_dependance;



    /**************************************************************************/
    /************************** Argumemnts G�n�raux ***************************/
    /**************************************************************************/

    //valeurs par defaut AX12
#define POINT_MINI_DEFAUT       0
#define POINT_MAXI_DEFAUT       1023

    //ID AX12 
    // /!\ SI ID_MAX_AX12  > 31 => IL FAUT ADAPTER LE CHAMP DE BIT 
    // /!\ DANS LA STRUCTURE decal !!!!!! /!\ //
#define TOUS_LES_AX12           0xFE
#define ID_MAX_AX12             26
#define AUCUN_AX                (-ID_MAX_AX12 - 1)

    //Param�tres sp�ciaux
#define POSITION_MAX_AX         1024.
#define ANGLE_MAX_AX            300.
#define PAS_DE_CORRECTION_ANGLE_FIN 3000

    //defines vitesses de com
#define _9600b                  0xCF
#define _19200b                 0x67
#define _57600b                 0x22
#define _115200b                0x10
#define _200000b                0x09
#define _250000b                0x07
#define _400000b                0x04
#define _500000b                0x03
#define _1000000b               0x01


/******************************************************************************/
/************************ Declaration des enums *******************************/
/******************************************************************************/
    
    typedef enum 
    {
        // /!\ ENUM CODEE SUR 3 BIT !!! /!\ //
        // /!\      VALEUR MAX : 7      /!\ //
        PAS_D_ERREUR,           // 0
        TIME_OUT,               // 1
        REPONSE_OK,             // 2
        PAS_DE_REPONSE,         // 3
        ERREUR_CS,              // 4
        LIMITATION_DE_COURANT,  // 5
        AUTRE_ERREUR            // 6
    }_enum_erreur_ax12;
    
    typedef enum
    {
        RECEPTION   = 0,
        EMISSION    = 1
    }_enum_rx_tx;

    typedef enum
    {
        ETEINT      = 0,
        ALLUME      = 1
    }_enum_alim_ax12;

    
    typedef enum
    {
        // /!\ ENUM CODEE SUR 1 BIT !!! /!\ //
        // /!\      VALEUR MAX : 1      /!\ //
        EN_COURS_ENVOIT,    // 0
        ENVOIT_FINI         // 1    
    }_enum_etat_envoit_uart;

/******************************************************************************/
/*********************** Declaration de structures ****************************/
/******************************************************************************/


    typedef struct decal
    {
        // /!\ suivant et sym�trique limit� de -32 � + 31 /!\ //
        int16_t angle                       : 16;   // 16 : Word 1
        uint16_t position                   : 16;   // 16 : Word 2 
        _enum_dependance etat               : 1;    //  1 : Word 3
        int8_t suivant                      : 6;    //  7 : Word 3
        _enum_sens_rotation sens_rotation   : 2;    //  9 : Word 3
        int8_t symetrique                   : 6;    // 15 : Word 3
    }decal;

    typedef struct pos
    {
        _Bool present               :  1;
        _enum_erreur_ax12 erreur    :  3;
        uint16_t nb_echec           : 12;
        uint16_t point;
        //int point_mini;   //si impl�mentation de limite mini et max dans le code
        //int point_maxi;
        float angle;
       
    }pos;


    typedef struct
    {
        // Valeur max offset : 10    ->  4 bit
        // Erreur max        : 4     ->  3 bit
        // Max tentatives    : 10    ->  4 bit
        // timer max         : 1350  -> 16 bit
        uint8_t offset                      : 4;    // 4
        uint8_t nb_octet_attente            : 4;    // 8
        uint8_t tentatives                  : 4;    // 12        
        _enum_erreur_ax12 erreur            : 3;    // 15
        _enum_etat_envoit_uart etat_uart    : 1;    // 16
        
        uint8_t buffer[CHSUM+1];
        uint16_t timer;
    }_ax12;


/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    
/******************************************************************************/
/*************************** FONCTIONS INITS **********************************/
/******************************************************************************/    
    
/**
 * Fonction qui ninitialise les d�calege des AX12
 * Suivre le mode d'emploi dans le .C pour comprendre le fonctionnement
 */
void init_decalage_AX12 (void);

/**
 * Fonction qui sert � associer une premi�re position aux AX12 pour le calcul
 */
void init_position_AX12 (void);


/******************************************************************************/
/************* Fonction de conversion des positions primaires *****************/
/******************************************************************************/

/**
 * Produit en croix pour d�terminer l'angle �quivalent � une position
 * @param position : en points
 * @return : angle en �
 */
float convertion_position (uint16_t position);

/**
 * Produit en croix pour d�terminer une position �quivalente � un angle
 * @param angle en � (nombre r�el)
 * @return position (nombre entier)
 */
int convertion_angle (float angle);

/******************************************************************************/
/*************** Fonction de calcul des positions � envoyer *******************/
/******************************************************************************/

/**
 * calcul la position � envoyer � partir de l'angle souhait�e dans le plan fictif
 * @param ID : ID de l'AX12 dont on veut calculer le mouvement
 * @param angle : angle consigne en � (nombre r�el)
 * @return : position en point � donner en consigne � l'AX12
 */
int calcul_position (uint8_t ID, float angle);


/******************************************************************************/
/******************** Fonctions pour faire bouger les AX12 ********************/
/******************************************************************************/

/**
 * Bouge les AX12 � partir d'un angle donn�.
 * Cet angle est relatif � un plan, 0� = parall�le au terrain
 * \n
 * La fonction prend aussi en compte la couleur de d�part et inverse les bras � utiliser
 * Elle permet aussi d'inverser les angles sur certains AX12
 * \n
 * Toute l'autom peut donc �tre fait pour en rouge et sera adapt�e automatiquement en Jaune
 *
 * @param ID : ID de l'AX12 � d�placer
 * @param angle : en � (nombre r�el)
 * @param vitesse : VIT_AX_NORMAL, VIT_AX_REDUITE, VIT_AX_MOITIE ( de 0 � 1023)
 * @param attente : AVEC_ATTENTE, SANS_ATTENTE, pour permettre de synchro les mouvements
 */
void synchro_AX12 (uint8_t ID, float angle, uint16_t vitesse, uint8_t attente);


/**
 * Fonction de commande simple des AX12, pour les faire bouger
 *
 * @param ID
 * @param position : position de 0 � 1023 points
 * @param vitesse : VIT_AX_NORMAL, VIT_AX_MOITIE, VIT_AX_REDUITE (de 0 � 1023)
 * @param ATTENTE : AVEC_ATTENTE, SANS_ATTENTE
 * \n Il faut bien penser � lancer la fonction lancer_autom_AX12(); si AVEC_ATTENTE
 */
void angle_AX12 (uint8_t ID,  uint16_t position, uint16_t vitesse, uint8_t ATTENTE );


/**
 * Flag de lancement de l'autom des AX12
 * \n Il faut avoir utilis� l'option AVEC_ATTENTE lors du lancement des mouvements
 */
void lancer_autom_AX12 (void);


/**
 * Permet de configurer un AX12 en mode roue inifi ou en mode normal
 * Le mode est enregistr� en m�moire EPPROM (non volatile)
 * @param ID
 * @param mode : MODE_INFINI ou MODE_NORMAL
 */
void rotation_AX12 (uint8_t ID, uint8_t sens, uint16_t vitesse);


/******************************************************************************/
/****************** Fonctions de reconfiguration des AX12**********************/
/******************************************************************************/

/**
 * Fonction qui permet de configurer le renvois de donn�es par les AX12
 * \n
 * Le mode TOUTES_LES_INFOS, permet de les programmer � la chaine sans se soucier des bugs de com
 *
 * @param ID : un ID, ou TOUS_LES_AX12
 * @param type_de_retour : TOUTES_LES_INFOS, READ_ONLY, AUCUNE_INFO
 */
void configurer_status_returning_level (uint8_t ID, uint8_t type_de_retour);


/**
 * Permet de modifier le temps mort de r�ponse entre l'envoit de la donn�e, et la r�ponse de l'AX12
 * \n
 * DANS LE ROBOT, IL FAUT CONFIGURER LES AX12 POUR 20us
 * @param   ID                      : un ID, ou TOUS_LES_AX12
 * @param   temps_de_reponse_us     : Temps en � secondes
 */
void configurer_temps_de_reponse_AX12 (uint8_t ID, uint16_t  temps_de_reponse_us);


/**
 * Fonction qui v�rifie la pr�sence d'un AX12
 * @param ID
 * @return : renvoit REPONSE_OK ou PAS_DE_REPONSE
 */
uint8_t Ping (uint8_t ID);

/**
* configure l'AX12 � l'�tat d'usine
* \n ID mis � 1 automatiquement
* @param ID
*/
void reset_AX12 (uint8_t ID);


/**
 * Permet de changer l'ID d'un AX12
 *
 * @param ancien_ID : TOUS_LES_AX12 si l'on ne connait pas l'ID
 * \n ATTENTION NE CONNECTER QU'UN AX12 PENDANT LA MANIP SINON ILS PRENDRONT TOUS LE NOUVEAU ID !!
 * @param nouveau_ID
 */
void changer_ID_AX12 (uint8_t ancien_ID, uint8_t nouveau_ID);


/**
 * Permet de changer a vitesse de com des AX12 (PAS DU ROBOT !!!)
 * \n#define _9600b              0x01
 * \n#define _19200b             0x03
 * \n#define _57600b             0x04
 * \n#define _115200b            0x07
 * \n#define _200000b            0x09
 * \n#define _250000b            0x10
 * \n#define _400000b            0x22
 * \n#define _500000b            0x67
 * \n#define _1000000b           0xCF
 *
 * \n\n
 * Par d�faut les AX12 focntionnent � 500.000 bauds ici
 *
 * @param ID
 * @param bauds : defines du dessus
 */
void baud_AX12 (uint8_t ID, uint8_t bauds);


/**
 * Permet de configurer un AX12 en mode roue inifi ou en mode normal
 * Le mode est enregistr� en m�moire EPPROM (non volatile)
 * @param ID
 * @param mode : MODE_INFINI ou MODE_NORMAL
 */
void mode_rotation_AX12 (uint8_t ID, uint8_t mode);

/******************************************************************************/
/********************** Fonctions diverses AX12 *******************************/
/******************************************************************************/


/**
 * Fonction pour allumer les LEDS des AX12 ... =)
 * @param ID : TOUS_LES_AX12 pour tous les allumer
 */
void allumer_LED_AX12 (uint8_t ID);


/**
 * Fonction pour �teindre toutes la led d'un AX12 ... =(
 * @param ID : TOUS_LES_AX12 pour toutes les �teindre
 */
void eteindre_LED_AX12 (uint8_t ID);


/**
 * Fonction qui permet d'activer ou de d�sactiver l'asservissement en position
 * de l'ax12
 * @param ID        : ID de l'ax12 ou TOUS_LES_AX12 
 * @param mode      : true or false
 */
void torque_enable_ax12(uint8_t ID, _Bool mode);



/******************************************************************************/
/***************** Fonction de communication AX - 12 **************************/
/******************************************************************************/


/**
 *  @brief : Fonction qui est appel� lors de l'interrution de reception sur l'uart AX12
 */
void reception_uart_ax12 (void);


/**
 *  Fonction qui traite les octets re�us
 */
void traitement_reception_ax12 ();


/**
 * R�initialisation du buffer contenant les octets re�us
 */
void reinit_buffer (void);


/**
 *  Fonction qui envoit les trames de commande des AX12 sur l'uart puis g�re la r�ception
 *  \n
 * Elle s'assure de v�rifier que les AX12 ont correctement re�us les infos, et les renvois
 * si n�c�ssaire jusqu'� 5 fois. Sinon elle passe � autre chose
 *
 * @param ID : ID de l'AX12
 * @param longueur : nombre de param�tre envoy� en argument + 2 : _2PARAM, _3PARAM ... _6PARAM, _ALL_PARAM
 * @param instruction : PING, READ_DATA, WRITE_DATA, REG_WRITE, ACTION, RESET_AX
 * @param param1      : si pas de param�tre : NC
 * @param param2      : si pas de param�tre : NC
 * @param param3      : si pas de param�tre : NC
 * @param param4      : si pas de param�tre : NC
 * @param param5      : si pas de param�tre : NC
 */
void commande_AX12 (uint8_t ID, uint8_t longueur, uint8_t instruction, ...);


/**
 *  Fonction qui permet de calculer le checksum de v�rification de trames
 * \n
 * Prend en argument les m�mes param�tres que la fonction de commande
 * @param ID : ID de l'AX12
 * @param longueur : nombre de param�tre envoy� en argument + 2 : _2PARAM, _3PARAM ... _6PARAM, _ALL_PARAM
 * @param instruction : PING, READ_DATA, WRITE_DATA, REG_WRITE, ACTION, RESET_AX
 * @param param1      : si pas de param�tre : NC
 * @param param2      : si pas de param�tre : NC
 * @param param3      : si pas de param�tre : NC
 * @param param4      : si pas de param�tre : NC
 * @param param5      : si pas de param�tre : NC
 * @return            : renvoit la valeur de checksum sur un octet
 */
uint8_t calcul_checksum (uint8_t ID, uint8_t longueur, uint8_t instruction, ...);


/**
 * Fonction qui permet de calculer le checksum de v�rification de trames � partir d'une va_list
 * \n
 * @param ID            : ID de l'AX12
 * @param longueur      : nombre de param�tre envoy� en argument + 2 : _2PARAM, _3PARAM ... _6PARAM, _ALL_PARAM
 * @param instruction   : PING, READ_DATA, WRITE_DATA, REG_WRITE, ACTION, RESET_AX
 * @param liste_param   : liste de taille non d�fini d'arguments
 * @return              : renvoit la valeur de checksum sur un octet
 */
uint8_t calcul_checksum_variadic (uint8_t ID, uint8_t longueur, uint8_t instruction, va_list liste_param);


/**
 *  Fonction qui demande une donn�e � un AX-12
 * @param ID : AX12 de qui on peut connaitre la donn�e
 * @param type_donnee : temps de r�ponse    : LIRE_TEMPS_REPONSE (valeur * 2�s)\n
 *                      position actuelle   : LIRE_POSITION_ACTU\n
 *                      vitesse actuelle    : LIRE_VITESSE_ACTU\n
 *                      tension de l'AX12   : LIRE_TENSION (valeur / 10)\n
 *                      Temp�rature interne : LIRE_TEMPERATURE\n
 *                      Est en mouvement ?  : LIRE_MOUV_FLAG\n
 * @return Renvoit la donn�e | -1 en cas d'erreur
 */
int16_t read_data (uint8_t ID, uint8_t type_donnee);


/**
 * Fonction qui permet d'afficher sur la liaison s�rie la position en temps r�el
 * des ax12 choisis
 * @param ax12      : tableau avec les id des ax12 � checker
 * @param taille    : taille du tableau
 */
void lecture_position_AX12 (uint8_t *ax12, int taille);

/******************************************************************************/
/*************************** GESTION DES ERREURS ******************************/
/******************************************************************************/

/**
 * Fonction qui va pinguer r�guli�rement les ax12 configur�s comme pr�sent dans 
 * le code
 * Si jamais un ax12 n� r�pond pas, son �tat d'erreur va �tre mis � jour lors du 
 * Ping et l'on pourra traiter l'erreur plus tard en temps voulu
 */
void checkup_com_ax12();


/**
 * Fonction qui va r�initialis� l'alim et renvoyer les derni�res positions 
 * enregistr�s des ax12 si l'on d�tecte qu'un ax12 et en limitation de courant
 */
void clean_des_erreurs_ax12();


/**
 * Fonction � appeler � la fin du code, ou m�me avant le d�but du match pour 
 * se rendre compte si il y a des probl�mes de com r�curent avec certains ax12
 */
void print_statistique_ax12();


/**
 * Pour clearer des erreurs d'overload, il faut couper l'alimentation des ax12
 * puis la rallumer
 */
void reinit_alim_ax12();


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



#ifdef	__cplusplus
}
#endif

#endif	/* GESTION_AX12_H */

