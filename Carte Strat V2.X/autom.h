/* 
 * File:   autom.h
 * Author: Quentin
 *
 * Created on 1 f�vrier 2015, 17:12
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
    /*************************** DEFINE G�n�ral *******************************/
    /**************************************************************************/

    //Couleurs de d�parts
#define VERT                0
#define JAUNE               1

    //Conditions

#define NON                 2
#define OUI                 3
#define EN_ATTENTE          17
#define EN_DESCENTE         18
    //EN_COURS = 6
#define EN_MONTER           16
#define LIBRE               15

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

#define PINCE_ASCENSEUR     9
#define PINCE_BAS           15
#define PINCE_MILIEU        8
#define PINCE_HAUT          5
#define ASCENSEUR           13
#define BRAS_DROIT          7
#define BRAS_GAUCHE         4
#define SYS_BALLE           11

    /**************************************************************************/
    /*************************** POSITIONS AX12 *******************************/
    /**************************************************************************/

#define PINCE_ASC_FERME     780//760
#define PINCE_ASC_RELACHE   738 
#define PINCE_ASC_RACLETTE  658
#define PINCE_ASC_RANGE     516

#define PINCES_FERME        520//538
#define PINCES_RELACHE      570//545
#define PINCES_RANGE        770

#define ASC_BAS             0
#define ASC_DEPOSE          320
#define ASC_DEMARAGE        500
#define ASC_HAUT            975

#define INIT_BALLE          735
#define EJECTER_BALLE       470

    /**************************************************************************/
    /************************* DEFINE ETAT PINCES *****************************/
    /**************************************************************************/

#define FERMER              0
#define RELACHE             1
#define RACLETTE            2
#define RANGEMENT           3

#define MONTER              4
#define DESCENDRE           5
//#define EN_COURS            6 -> fichier asserv
#define HAUTEUR_DEMMARAGE   7
#define EN_HAUT             8
#define ACCOMPLI            9
#define DEPOSE              10

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

#define NE_RIEN_FAIRE               0
#define ATTRAPE_PIEDS               1
#define INIT_PINCES_DEMARRAGE       2
#define DEPOSE_PIEDS                3
#define PREPARATION_DEPOSE_PIEDS    4
#define ATTRAPE_GOBELET             5
#define DEPOSE_GOBELET              6
#define ATTRAPE_BALLE               7

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void jack();
    void autom_10ms (void);
    void init_pinces_demarage();
    void init_pinces_jack();
    void depose_pieds();
    void preparation_descente_pieds ();
    void depose_banc ();
    void attrape_gobelet();
    void depose_gobelet();
    void attrape_balle();

    void ejecter_balle();
    void init_balle();

    //Fonctions autom ax12
    void pinces (uint8_t ID, uint8_t etat);
    void ascenseur (uint8_t direction);
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

