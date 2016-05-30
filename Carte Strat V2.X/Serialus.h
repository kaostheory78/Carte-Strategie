/* 
 * File:   Serialus.h
 * Author: KaosTheory
 *
 * Created on 29 mai 2016, 13:09
 */

#ifndef SERIALUS_H
#define	SERIALUS_H

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
    
#define MAX_BUFF        8
#define MAX_PARAM       8
    
/******************************************************************************/
/************************** DEFINES DES ENUMS *********************************/
/******************************************************************************/

    // eser = enum_serialus
    
    
    typedef enum
    {
        SER_DEPLACEMENT,
        SER_ASSERV,
        SER_CALAGE,
        SER_ODOMETRIE,
        SER_POSITION,
        SER_AX12,
        SER_CAPTEUR,
        SER_LIST,
        SER_ERROR
    }_eser_type;
    
    typedef enum
    {
        SER_CIBLER = SER_ERROR + 1,
        SER_ORIENTER,
        SER_REJOINDRE,
        SER_AVANCER,
        SER_RECULER,
        SER_TOURNER,
        SER_TRAPEZE,
        SER_CARRE,
        SER_BRAKE,
        SER_UNBRAKE
    }_eser_DEPLACEMENT;
    
    typedef enum
    {
        SER_CAL_ROBOT = SER_UNBRAKE,
        SER_CAL_X,
        SER_CAL_Y,
        SER_CAL_TETA,
    }_eser_CALAGE;
    
    typedef enum
    {
        SER_GET = SER_CAL_TETA +1,
        SER_SET
    }_eser_accesseur;
    
    
    /*************************/
    /****** Réglage asserv ***/
    /*************************/
    
    typedef enum
    {
        SER_PID = SER_SET +1,
        SER_DISTANCE,
        SER_ORIENTATION,      
        SER_ENTRAXE,
        SER_PERIMETRE,
        SER_GAIN_D,
        SER_GAIN_G,
        SER_SEUIL_ARRET,
        SER_COEF_FREIN,
        SER_TENSION_MOT_X,
        SER_TENSION_MOT_D,
        SER_TENSION_MOT_G
                
    }_eser_ASSERV;
    
    typedef enum
    {
        SER_V_CONS_MAX = SER_TENSION_MOT_G +1,
        SER_V_CONS_MIN,
        SER_V_MAX_ROBOT,
        SER_DISTANCE_V_MAX,
        SER_ACC_CONS,
        SER_DCC_CONS,
        SER_ACC_MIN,
        SER_DCC_MIN 
    }_eser_AS_DIS_OR;
    
    typedef enum 
    {
        SER_PID_VITESSE = SER_DCC_MIN + 1,
        SER_PID_BRAKE
    }_eser_type_PID;
    
    typedef enum
    {
        SER_KP = SER_PID_BRAKE +1,
        SER_KI,
        SER_KD,
        SER_MAX_I
    }_eser_AS_PID;
    
    
    /*************************/
    /****** REGLAGE AX12 *****/
    /*************************/
    
    typedef enum
    {
        SER_AX_ANGLE = SER_MAX_I +1,
        SER_AX_POSITION,
        SER_AX_AUTOM,
        SER_AX_ROTATION,
        SER_AX_CONFIG_ROTATION,
        SER_AX_RETURN_STATUS,
        SER_AX_RESPONSE_TIME,
        SER_AX_PING_AX12,
        SER_AX_RESET,
        SER_AX_MODIF_ID,
        SER_AX_LED,
        SER_AX_DATA
    }_eser_AX12;

    
    
    
/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

    typedef struct
    {
        _Bool info_a_traiter            : 1;
        _Bool attente_confirmation      : 1;
        _Bool confirmation              : 1;
        _Bool erreur_confirmation       : 1;
        _Bool deplacement_en_cours      : 1;
        uint8_t index[MAX_PARAM];
        uint8_t nb_param;
        uint8_t buffer[MAX_PARAM][MAX_BUFF];
        
    }_serialus;
    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/
    
    void action_reception_serialus(uint8_t buf);
    void reception_serialus(uint8_t octet);
    void reinit_buffer_serialus();
    _Bool check_string (uint8_t param, char* string);
    void list_serialus (uint8_t truc_a_lister);
    void conversion_string_to_function_code();
    _Bool check_erreur_serialus();
    _Bool check_est_un_nombre(uint8_t* buffer);
    _Bool check_sont_des_nombres(uint8_t nb_nombre, ...);
    int16_t check_id_ax12(uint8_t param);
    
    /**
    * Fonction qui vérifie si une liste de chaine représente bien des nombres flottants
    * @param nb_nombre : nombre de nombre à vérifier
    * @param ...       : liste de chaine de nombre (de type (int8_t *))
    * @return          : true / false
    */
   _Bool check_sont_des_nombres_flottants(uint8_t nb_nombre, ...);

   /**
    * Fonction qui vérifie que la chaine reçu est bien un nombre flottant
    * @param buffer : buffer contenant le nombre
    * @return       : true / false
    */
    _Bool check_est_un_nombre_flottant(uint8_t* buffer);
    _Bool check_confirmation();
    _Bool check_nb_param (uint8_t nombre_attendu);
    _Bool check_coherence (_Bool test);
    void list_serialus (uint8_t truc_a_lister);
    void traitement_serialus ();
    
    
    // Fonctions Print : 
    void print_non_reconnu(uint8_t numero);
    void print_manque_parametre();
    void print_trop_parametre();
    void print_pas_nombre();
    void print_pas_nombre_flottant();
    void print_abort_confirmation();
    void print_incoherent();
    void print_confirm();
    void print_position();
    void print_erreur_deplacement(_enum_erreur_asserv erreur);
    void print_abort(char* raison);
    void print_erreur_ax12();
    
    // Fonctions traitements : 
    void serialus_traitement_deplacement ();
    void serialus_traitement_calage();
    void serialus_traitement_asserv();
    void serialus_traitement_odometrie();
    void serialus_traitement_position();
    void serialus_traitement_ax12();
    void serialus_traitement_list();
    

    
    
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/    


#ifdef	__cplusplus
}
#endif

#endif	/* SERIALUS_H */

