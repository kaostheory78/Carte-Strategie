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
        AUTOM_PRINCIPALE = 0,
        AUTOM_AVANT,
        AUTOM_ARRIEIRE,
        AUTOM_MAX_NB        
    }_autom_id;
    
    typedef enum
    {
        AVANT = AUTOM_AVANT,
        ARRIERE = AUTOM_ARRIEIRE,
        LES_DEUX
    }_cote;
    
//    typedef
    

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

#ifdef  PETIT_ROBOT
        // FLAG_ACTION DU PETIT ROBOT
                
        /*** INIT ***/        
        START_ROBOT,
        WAIT_PINCES_OUVERTES,
        WAIT_INIT_ROBOT_COMPLETE,
        INIT_ROBOT_COMPLETE,
                
        /*** RECHERCHE MODULE ***/
                PROCESS_MODULE_READY,
                RECHERCHE_MODULE_AVANT,
                RECHERCHE_MODULE_ARRIERE,
                MODULE_DETECTED_AVANT,
                MODULE_DETECTED_ARRIERE,
                ATTENTE_MONTEE_AVANT,
                ATTENTE_MONTEE_ARRIERE,
                ASC_AVANT_EN_MONTEE,
                ASC_ARRIERE_EN_MONTEE,
                FERMETURE_PINCE_HAUT_AVANT,
                FERMETURE_PINCE_HAUT_ARRIERE,
                PREPARE_REDESCENTE_AVANT,
                PREPARE_REDESCENTE_ARRIERE,
                ATTENTE_REDESCENTE_AVANT,
                ATTENTE_REDESCENTE_ARRIERE,
                ATTENTE_CYCLE_MONTAGE_AVANT_COMPLET,
                ATTENTE_CYCLE_MONTAGE_ARRIERE_COMPLET,
                TOUR_COMPLETE,
                
#endif

#ifdef GROS_ROBOT
       // FLAG ACTION DU GROS ROBOT       
#endif 
     
        FIN_DE_MATCH
            
    }_enum_flag_action;
    
    
/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

typedef struct
{
    bool timer_actif;
    uint32_t temps_echeance;
    _enum_flag_action event;
}_timer_event;

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


