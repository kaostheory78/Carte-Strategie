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
    #define PINCE_POS_ENTROUVERTE           585 //575
    #define PINCE_POS_FERME                 535  // 540

    #define ASC_AR_POS_BAS                     320    
    #define ASC_AR_POS_INIT                    410 
    #define ASC_AR_POS_INIT_MAX                560
    #define ASC_AR_POS_HAUT                    803 
    #define ASC_AR_POS_DEPOSE                   530 //517

    #define ASC_AV_POS_BAS                     291    
    #define ASC_AV_POS_INIT                    340
    #define ASC_AV_POS_INIT_MAX                510
    #define ASC_AV_POS_HAUT                    770 
    #define ASC_AV_POS_DEPOSE                  488 //488

    #define BITE_POS_INIT                       830
    #define BITE_POS_RANGE                      305
    #define BITE_POS_FERME                      320
    #define BITE_POS_OUVERTE                    540 
            
#endif

#ifdef GROS_ROBOT

#endif

    /**************************************************************************/
    /************************* DEFINE FLAG_ACTION *****************************/
    /**************************************************************************/

    typedef enum
    {
        NE_RIEN_FAIRE = 0,                      // 0
        EN_ATTENTE_EVENT,                       // 1
        CHECK_AX12_EVENT,                       // 2
        CHECK_SYNC_EVENT,                       // 3

#ifdef  PETIT_ROBOT
        // FLAG_ACTION DU PETIT ROBOT
                
        /*** INIT ***/        
        SR_START_ROBOT,                         // 4
        SR_DEPOSE_MODULE_OUVERT,                // 5
        SR_PINCES_OUVERTES,                     // 6
        SR_ROBOT_READY,                         // 7
                
        /*** RECHERCHE MODULE ***/
        MONTAGE_TOUR_PRET,                      // 8
        MT_RECHERCHE_MODULE_EN_COURS,           // 9
        MT_MODULE_DETECTE,
        MT_MODULE_ATTRAPE,
        MT_PRET_A_MONTER,
        MT_ASC_EN_HAUT,
        MT_MODULE_ATTRAPE_EN_HAUT,
        MT_PRET_A_REDESCENDRE,
        MT_TOUR_COMPLETE,
                
        /*** DEPOSE MODULE ****/
        DX_START_DEPOSE,
        D1_START_DEPOSE,
        D2_START_DEPOSE,
        D2_PINCE_BAS_OUVERTE,
        D2_ASC_EN_HAUT,
        D2_MODULE_ATTRAPE,
        DX_PRET_A_DEPOSER,
        DX_PRET_A_ALLER_EN_DEPOSE,
        DX_ASCENSEUR_POS_DEPOSE, 
        DX_MODULE_RELACHE,
        DX_DEPOSE_FINIT,
                
        /*** CHOPPAGE FUSEE ***/
        AF_ATTRAPE_FUSEE,
        AF_CHOPAGE_MODULE_READY,
        AF_MODULE_DETECTE,
        AF_MODULE_ATTRAPE,
        
                
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
    uint8_t get_module_tour(_cote cote);
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


