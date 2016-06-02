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

    /**
     * Couleurs des zondes de d�part
     */
    typedef enum
    {
        VERT,
        JAUNE
    }_enum_couleurs;

    //Evitement
    typedef enum
    {
        ACTIF,
        INACTIF,
        ACTION_EVITEMENT,
        EVITEMENT_NORMAL,
        STOP,
        DISTANCE_STOP = STOP,
        ANGLE_STOP = STOP
    }_enum_evitement;
    

    /**************************************************************************/
    /*************************** DEFINE ID AX12 *******************************/
    /**************************************************************************/

#ifdef PETIT_ROBOT
    
#endif

#ifdef  GROS_ROBOT
    #define AX_US                   17
    #define AX_US_DROIT             23
    #define AX_US_GAUCHE            4

#endif

    /**************************************************************************/
    /*************************** POSITIONS AX12 *******************************/
    /**************************************************************************/

#ifdef  PETIT_ROBOT
    
#endif

#ifdef GROS_ROBOT

#endif

    /**************************************************************************/
    /************************* DEFINE ETAT PINCES *****************************/
    /**************************************************************************/

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

    typedef enum
    {
        NE_RIEN_FAIRE,                                       

#ifdef  PETIT_ROBOT

#endif

#ifdef GROS_ROBOT

#endif 
     
        FIN_DE_MATCH
            
    }_enum_flag_action;




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void jack();
    void allumer_pompes ();
    void eteindre_pompe();
    void autom_20ms (void);
    void son_evitement (uint8_t melodie);

#ifdef  PETIT_ROBOT

#endif

#ifdef  GROS_ROBOT

    void rotation_us(void);

    // Fonctions d'init
    void init_jack();
    void init_depart();

    void rotation_us_avant();;
    
    //Fonction AX12

    uint8_t check_capteur (uint8_t cote);

    /**
     * Fonction qui permet d'inverser le c�t� en fonction de la couleur
     * @param cote : le cote (DROITE ou GAUCHE)
     * @return le c�t� oppos� GAUCHE ou DROITE
     */
    uint8_t inversion_autom (uint8_t cote);

   
    /**************************************************************************/
    /**************************************************************************/
    /**************************************************************************/
#endif


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* AUTOM_H */

