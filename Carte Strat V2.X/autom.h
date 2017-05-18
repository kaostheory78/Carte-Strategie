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

    /**
     * Couleurs des zondes de départ
     */
    typedef enum
    {
        VERT,
        JAUNE
    }_enum_couleurs;
    
    typedef enum
    {
        AUTOM_ID_MIN_NB = 0,
        AUTOM_PRINCIPALE = AUTOM_ID_MIN_NB,
        AUTOM_AVANT,
        AUTOM_ARRIERE,
        AUTOM_ID_MAX_NB        
    }_autom_id;
    
    typedef enum
    {
        AVANT = AUTOM_AVANT,
        ARRIERE = AUTOM_ARRIERE,
        LES_DEUX
    }_cote;
    
    typedef enum
    {
        PINCE_BAS = 0,
        PINCE_HAUT,
        ASCENSEUR,
        RETOURNE_MODULE
    }_type_actionneur;
    

    /**************************************************************************/
    /*************************** DEFINE ID AX12 *******************************/
    /**************************************************************************/

#ifdef PETIT_ROBOT
    #define PINCE_BAS_AV            7
    #define PINCE_BAS_AR            5
    #define PINCE_HAUT_AV           1
    #define PINCE_HAUT_AR           2
    #define ASC_AVANT               9
    #define ASC_ARRIERE             10
    #define BITE_AV                 6
    #define BITE_AR                 8
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
    #define PINCE_POS_REPLI                 848
    #define PINCE_POS_OUVERTE               630
    #define PINCE_POS_ENTROUVERTE           575
    #define PINCE_POS_FERME                 535  // 540

    #define ASC_AR_POS_BAS                     320    
    #define ASC_AR_POS_INIT                    440 
    #define ASC_AR_POS_INIT_MAX                560
    #define ASC_AR_POS_HAUT                    803 // 793

    #define ASC_AV_POS_BAS                     291    
    #define ASC_AV_POS_INIT                    392
    #define ASC_AV_POS_INIT_MAX                510
    #define ASC_AV_POS_HAUT                    770 // 760
            
#endif

#ifdef GROS_ROBOT

#endif

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

    typedef enum
    {
        NE_RIEN_FAIRE,   
        EN_ATTENTE_EVENT,
        CHECK_AX12_EVENT,

#ifdef  PETIT_ROBOT
        // FLAG_ACTION DU PETIT ROBOT
                
        /*** INIT ***/        
        START_ROBOT,
        INIT_PINCES_OUVERTES,
        WAIT_INIT_ROBOT_COMPLETE,
        INIT_ROBOT_COMPLETE,
                
        /*** RECHERCHE MODULE ***/
        MONTAGE_TOUR_PRET,
        MT_RECHERCHE_MODULE_EN_COURS,
        MT_MODULE_DETECTE,
        MT_MODULE_ATTRAPE,
        MT_PRET_A_MONTER,
        MT_ASC_EN_HAUT,
        MT_MODULE_ATTRAPE_EN_HAUT,
        MT_PRET_A_REDESCENDRE,
        MT_TOUR_COMPLETE,
                
#endif

#ifdef GROS_ROBOT
       // FLAG ACTION DU GROS ROBOT       
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
    // Fonctions init
    void init_system_arriere();
    void init_system_avant();
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
     * Fonction qui permet d'inverser le côté en fonction de la couleur
     * @param cote : le cote (DROITE ou GAUCHE)
     * @return le côté opposé GAUCHE ou DROITE
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


