<<<<<<< HEAD
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
    #define PINCE_ASCENSEUR     9
    #define PINCE_BAS           15
    #define PINCE_MILIEU        8
    #define PINCE_HAUT          5
    #define ASCENSEUR           13
    #define BRAS_DROIT          7
    #define BRAS_GAUCHE         4
    #define SYS_BALLE           11
#endif

#ifdef  GROS_ROBOT
    #define PINCE                   10
    #define DEPOSE_TAPIS_D          25
    #define DEPOSE_TAPIS_G          12
    #define PINCE_TAPIS_D           26
    #define PINCE_TAPIS_G           1
    #define AX_US                   17
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

#define EN_ATTENTE          16
#define EN_DESCENTE         17
#define EN_MONTER           18
#define LIBRE               19

#define DROITE              30
#define GAUCHE              31

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

#define NE_RIEN_FAIRE                   0

#ifdef  PETIT_ROBOT
    #define ATTRAPE_PIEDS               1
    #define INIT_PINCES_DEMARRAGE       2
    #define DEPOSE_PIEDS                3
    #define PREPARATION_DEPOSE_PIEDS    4
    #define ATTRAPE_GOBELET             5
    #define DEPOSE_GOBELET              6
    #define ATTRAPE_BALLE               7
    #define CLAP                        8
    #define ZONE_DEPART                 9
    #define FERMETURE_PINCE             10
    #define PIEDS_4                     11
    #define EMPILEMENT                  12
#endif

#ifdef GROS_ROBOT
    #define ATTRAPE_GOBELET             1
    #define INIT_DEPART                 2
    #define INIT_JACK                   3


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

    void ejecter_balle();
    void init_balle();

    //Fonctions autom ax12
    void pinces (uint8_t ID, uint8_t etat);
    void ascenseur (uint8_t direction);
#endif

#ifdef  GROS_ROBOT

    void rotation_us(void);
    void pince(uint8_t action);
    void eclairage_robot (void);
    void attrape_gobelet (void);
    void init_jack();
    void init_depart();

#endif


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

=======
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
    #define PINCE_ASCENSEUR     9
    #define PINCE_BAS           15
    #define PINCE_MILIEU        8
    #define PINCE_HAUT          5
    #define ASCENSEUR           13
    #define BRAS_DROIT          7
    #define BRAS_GAUCHE         4
    #define SYS_BALLE           11
#endif

#ifdef  GROS_ROBOT
    #define PINCE                   10
    #define DEPOSE_TAPIS_D          25
    #define DEPOSE_TAPIS_G          12
    #define PINCE_TAPIS_D           26
    #define PINCE_TAPIS_G           1
    #define AX_US                   17
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

#define EN_ATTENTE          16
#define EN_DESCENTE         17
#define EN_MONTER           18
#define LIBRE               19

#define DROITE              30
#define GAUCHE              31

    
    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

#define NE_RIEN_FAIRE                   0

#ifdef  PETIT_ROBOT
    #define ATTRAPE_PIEDS               1
    #define INIT_PINCES_DEMARRAGE       2
    #define DEPOSE_PIEDS                3
    #define PREPARATION_DEPOSE_PIEDS    4
    #define ATTRAPE_GOBELET             5
    #define DEPOSE_GOBELET              6
    #define ATTRAPE_BALLE               7
    #define CLAP                        8
    #define ZONE_DEPART                 9
    #define FERMETURE_PINCE             10
    #define PIEDS_4                     11
    #define EMPILEMENT                  12
#endif

#ifdef GROS_ROBOT
    #define ATTRAPE_GOBELET             1
    #define INIT_DEPART                 2
    #define INIT_JACK                   3


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

    void ejecter_balle();
    void init_balle();

    //Fonctions autom ax12
    void pinces (uint8_t ID, uint8_t etat);
    void ascenseur (uint8_t direction);
#endif

#ifdef  GROS_ROBOT

    void rotation_us(void);
    void pince(uint8_t action);
    void eclairage_robot (void);
    void attrape_gobelet (void);
    void init_jack();
    void init_depart();

#endif


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

>>>>>>> 9a019010a256ac2bc994928a02d6bca411f60506
