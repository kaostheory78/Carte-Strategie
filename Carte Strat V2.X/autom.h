/* 
 * File:   autom.h
 * Author: Quentin
 *
 * Created on 1 février 2015, 17:12
 */

#ifndef AUTOM_H
#define	AUTOM_H

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
    /*************************** DEFINE Général *******************************/
    /**************************************************************************/

    //Couleurs de départs
#define VERT                0
#define JAUNE               1

    //Conditions

#define NON                 2
#define OUI                 3
    //EN_COURS = 6

    //Evitement
#define ACTIF               0
#define INACTIF             1

#define ACTION_EVITEMENT    2
#define EVITEMENT_NORMAL    3
#define STOP                5

#define DISTANCE_STOP       5
#define ANGLE_STOP          5

    /**************************************************************************/
    /*************************** DEFINE ID AX12 *******************************/
    /**************************************************************************/

#ifdef PETIT_ROBOT
    #define PINCE_ASCENSEUR         9
    #define PINCE_BAS               15
    #define PINCE_MILIEU            8
    #define PINCE_HAUT              5
    #define ASCENSEUR               13
    #define BRAS_DROIT              7
    #define BRAS_GAUCHE             4
    #define SYS_BALLE               11
#endif

#ifdef  GROS_ROBOT
     #define PINCE_D                24
    #define PINCE_G                 19

    // TAPIS
    #define PINCE_TAPIS_D           12
    #define PINCE_TAPIS_G           3
    #define AX_US                   17

    // BRAS
    #define BRAS_DROIT              25
    #define BRAS_GAUCHE             1

    // ASCENSEUR
    #define ASCENSEUR               26

    //Chenilles
    #define CHENILLE_AV_G           18
    #define CHENILLE_AV_D           21
    #define CHENILLE_AR_G           6
    #define CHENILLE_AR_D           14

    //Balise
    #define BALISE_GAUCHE           2
    #define BALISE_DROITE           10

    #define AX_US_DROIT             23
    #define AX_US_GAUCHE            4

#endif

    /**************************************************************************/
    /*************************** POSITIONS AX12 *******************************/
    /**************************************************************************/

#ifdef  PETIT_ROBOT
    #define PINCE_ASC_FERME     780//760
    #define PINCE_ASC_RELACHE   738
    #define PINCE_ASC_RACLETTE  658
    #define PINCE_ASC_RANGE     516

    #define PINCES_FERME        520//538
    #define PINCES_RELACHE      570//545
    #define PINCES_RANGE        770

    #define ASC_BAS             0
    #define ASC_DEPOSE          330
    #define ASC_DEMARAGE        500
    #define ASC_HAUT            975

    #define INIT_BALLE          785
    #define EJECTER_BALLE       470
#endif

#ifdef GROS_ROBOT

#endif

    /**************************************************************************/
    /************************* DEFINE ETAT PINCES *****************************/
    /**************************************************************************/

#define FERMER              1
#define OUVERT              2
#define OUVERTE             2
#define RELACHE             3
#define RACLETTE            4
#define RANGEMENT           5
//#define EN_COURS            6 -> fichier asserv
#define MONTER              7
#define DESCENDRE           8
#define HAUTEUR_DEMMARAGE   9
#define EN_HAUT             10
#define ACCOMPLI            11
#define DEPOSE              12
#define EN_BAS              13
#define INTERMEDIAIRE       14

#define EN_ATTENTE          16
#define EN_DESCENTE         17
#define EN_MONTER           18
#define LIBRE               19

#define DROITE              30
#define DROIT               30
#define GAUCHE              31

#define AVANT               32
#define ARRIERE             33

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

#define NE_RIEN_FAIRE                   0
#define FIN_DE_MATCH                    1

#ifdef  PETIT_ROBOT
    #define ATTRAPE_PIEDS               2
    #define INIT_PINCES_DEMARRAGE       3
    #define DEPOSE_PIEDS                4
    #define PREPARATION_DEPOSE_PIEDS    5
    #define ATTRAPE_GOBELET             6
    #define DEPOSE_GOBELET              7
    #define ATTRAPE_BALLE               8
    #define CLAP                        9
    #define ZONE_DEPART                 10
    #define FERMETURE_PINCE             11
    #define PIEDS_4                     12
    #define EMPILEMENT                  13
    #define DESACTIVATION_EVITEMENT     14
#endif

#ifdef GROS_ROBOT
    #define INIT_ASCENSEUR              2
    #define INIT_DEPART                 3
    #define INIT_JACK                   4
    #define ATTRAPE_GOBELET             5
    #define ATTRAPE_GOBELET2            6
    #define MONTEE_MARCHE               7
    #define PREPARATION_MARCHE          8
    #define EN_ROUTE_MONTEE_MARCHE      9   
    #define ARRIVEE_MARCHE              10
    #define MONTEE_EVITEMENT_EN_COURS   11


#endif

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void jack();
    void allumer_pompes ();
    void eteindre_pompe();
    void autom_10ms (void);
    void son_evitement (uint8_t melodie);

#ifdef  PETIT_ROBOT

    
    void init_pinces_demarage();
    void init_pinces_jack();
    void depose_pieds();
    void preparation_descente_pieds ();
    void depose_banc ();
    void attrape_gobelet();
    void depose_gobelet();
    void attrape_balle();
    void ouvrir_bras (uint8_t);
    void fermer_bras (uint8_t);
    void faire_les_claps();
    void empilement(int taille_max);

    void desactivation_evitement_reculer ();

    void ejecter_balle();
    void init_balle();

    //Fonctions autom ax12
    void pinces (uint8_t ID, uint8_t etat);
    void ascenseur (uint8_t direction);
#endif

#ifdef  GROS_ROBOT

    void rotation_us(void);

    // Fonctions d'init
    void init_jack();
    void init_depart();

    void marche();
    void rotation_us_avant();;
    
    //Fonction AX12

    uint8_t check_capteur (uint8_t cote);

    /**
     * Fonction qui permet d'inverser le côté en fonction de la couleur
     * @param cote : le cote (DROITE ou GAUCHE)
     * @return le côté opposé GAUCHE ou DROITE
     */
    uint8_t inversion_autom (uint8_t cote);

    /**
     * Fonction qui déploie les chenilles
     * @param action : MONTER pour ranger les chenilles, DESCENDRE pour les descendre
     */
    void chenilles(uint8_t action);

    /**
     * Fonction pour ouvrir les bras du robot
     * @param cote : DROIT / GAUCHE
     * @param action : OUVERT / FERMER
     */
    void bras(uint8_t cote, uint8_t action);

    /**
     * Fonction qui ouvre les bras des tapis
     * @param cote : DROIT / GAUCHE
     * @param action : RANGEMENT / OUVERT / DEPOSE
     */
    void tapis(uint8_t cote, uint8_t action);

    /**
     * Fonction qui ouvre les pinces avant
     * @param cote DROIT / GAUCHE
     * @param action : OUVERTE / FERMER / RANGEMENT
     */
    void pince(uint8_t cote,uint8_t action);

    /**
     * Foncion qui range les pinces à l'interieur du robot
     * @param action : AVANT / ARRIERE
     */
    void ascenseur(uint8_t action);

    void monter_balise ();

    void descendre_balise();



    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/


    void attrape_gobelet (uint8_t reinit);
    void arrive_marche ();
    void montee_des_marches ();
    
#endif


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

