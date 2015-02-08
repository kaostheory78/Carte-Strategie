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
    /*************************** Define Général *******************************/
    /**************************************************************************/

    //Couleurs de départs
#define VERT            0
#define JAUNE           1

    //Conditions

#define NON             2
#define OUI             3
#define EN_ATTENTE      4

    //Evitement
#define ACTIF           0
#define INACTIF         1
#define STOP            5

#define DISTANCE_STOP   5
#define ANGLE_STOP      5

    /**************************************************************************/
    /************************ Define PORT Carte *******************************/
    /**************************************************************************/

    //Du JACK  vers les codeurs

#define SYS_JACK            PORTBbits.RB4
#define CAPT_FEU_D          PORTAbits.RA4   //Capteur 1
#define CAPT_US_BALISE      PORTAbits.RA9   //Capteur 2
#define ND_1                PORTCbits.RC3   //Capteur 3
#define CAPT_FEU_BORDURE_G  PORTCbits.RC4   //Capteur 4
#define LED                 PORTCbits.RC5

    //Carte d'extenssion
    //De la gauche vers la droite (nappe en bas)

#define CAPT_US_ARRIERE_D   PORTAbits.RA8   //Capteur 1
#define ND_2                  PORTAbits.RA3   //Capteur 2
#define CAPT_FEU_BORDURE_D  PORTAbits.RA2   //Capteur 3
#define CAPT_FEU_G          PORTCbits.RC1   //Capteur 4
#define SYS_COULEUR         PORTCbits.RC2   //Capteur 5
#define CAPT_US_ARRIERE_G   PORTCbits.RC0   //capteur 6

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/


    void autom_10ms (void);

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

