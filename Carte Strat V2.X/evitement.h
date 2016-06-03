/* 
 * File:   evitement.h
 * Author: KaosTheory
 *
 * Created on 3 juin 2016, 22:55
 */

#ifndef EVITEMENT_H
#define	EVITEMENT_H

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
    
    // CAPTEURS AVANTS :
#define CAPT_ADV_AVANT_G           _CAPT_ADV_AVANT_G         
#define CAPT_ADV_AVANT_C           _CAPT_ADV_AVANT_C
#define CAPT_ADV_AVANT_D           _CAPT_ADV_AVANT_D

#define ETAT_ADV_AVANT_G           _ETAT_ADV_AVANT_G
#define ETAT_ADV_AVANT_C           _ETAT_ADV_AVANT_C
#define ETAT_ADV_AVANT_D           _ETAT_ADV_AVANT_D

    // CAPTEURS ARRIERES : 
#define CAPT_ADV_ARRIERE_G         _CAPT_ADV_ARRIERE_G 
#define CAPT_ADV_ARRIERE_C         _CAPT_ADV_ARRIERE_C 
#define CAPT_ADV_ARRIERE_D         _CAPT_ADV_ARRIERE_D 

#define ETAT_ADV_ARRIERE_G         _ETAT_ADV_ARRIERE_G
#define ETAT_ADV_ARRIERE_C         _ETAT_ADV_ARRIERE_C
#define ETAT_ADV_ARRIERE_D         _ETAT_ADV_ARRIERE_D
    
/******************************************************************************/
/************************** DEFINES DES ENUMS *********************************/
/******************************************************************************/
   
        //Evitement
    // /!\ CODE SUR 3 BIT DANS LA STRUCTURE /!\ //
    typedef enum
    {
        ACTION_EVITEMENT,
        EVITEMENT_NORMAL,
        STOP
    }_enum_mode_evitement;
    
    // /!\ CODE SUR 3 BIT DANS LA STRUCTURE /!\ //
    typedef enum
    {
        AUCUN     = 0b000,
        EV_DROIT  = 0b001,
        EV_CENTRE = 0b010,
        EV_GAUCHE = 0b100,
        EV_TOTALE = 0b111
    }_enum_cote_evitement;
    
/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

    typedef struct
    {
        _enum_on_off actif          : 1;
        _enum_on_off detection      : 1;
        _enum_sens_deplacement sens : 2;
        _enum_mode_evitement mode   : 3;
        uint8_t cote,               : 3; // Definit à partir d'association de _enum_cote_evitement
    }_evitement;
    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/


    /**
     * Fonction qui initialise la structure evitemet avec des valeurs par défaut
     */
    void init_evitement();
    
    /**
     * Fonction qui check l'évitement avant en fonction des capteurs activés
     * @return true or false
     */
    _Bool check_evitement_avant();
    
    /**
     * Fonction qui check l'évitement arrière en fonction des capteurs activés
     * @return true or false
     */
    _Bool check_evitement_arriere();
    
    /**
     * Fonction qui check les évitement selon les parmètres actifs
     * (capteurs à checker, sens de l'évitement)
     * @return true si évitement false sinon
     */
    _Bool check_evitement();
    
    /**
    * Cette fonction a pour but de désactiver automatiquement l'évitement dans 
    * des zones critiques comme les bordures de terrain
    */
    void ajustement_evitement_autonome();
    
    /**
     * Fonction qui gère tout l'évitement
     */
    void evitement();
    
 /******************************************************************************/
/******************************************************************************/
/******************************************************************************/   
    
#ifdef	__cplusplus
}
#endif

#endif	/* EVITEMENT_H */

