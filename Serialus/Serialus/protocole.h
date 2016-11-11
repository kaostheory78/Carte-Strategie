/* 
 * File:   Serialus.h
 * Author: KaosTheory78 : Quentin Mercier
 *
 * Created on 09 juin 2016, 21:50
 */

#ifndef PROTOCOLE_H
#define	PROTOCOLE_H

#ifdef	__cplusplus
extern "C" {
#endif


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include <stdint.h>

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

#define SIZE_FRAME_TX		32
#define SIZE_FRAME_RX		16

/******************************************************************************/
/************************** DEFINES DES ENUMS *********************************/
/******************************************************************************/


	typedef enum
	{
		GET_VALEUR,
		SET_VALEUR,
		MOUVEMENT
	}_eser_main_header;
	
	typedef enum
	{
		ASSERV,
		ODOMETRIE,
		AX12
	}_eser_sub_header; 

	typedef enum
	{
		BOOL,
		UINT8,
		INT8,
		UINT16,
		INT16,
		UINT32,
		INT32,
		UINT64,
		INT64,
		FLOAT,
		DOUBLE
	}_eser_data_type;

	typedef enum
	{
		// Accelerations :
		ACCEL_DISTANCE_MAX,
		ACCEL_DISTANCE_MIN,
		DECEL_DISTANCE_MAX,
		DECEL_DISTANCE_MIN,
		ACCEL_ORIENTATION_MAX,
		ACCEL_ORIENTATION_MIN,
		DECEL_ORIENTATION_MAX,
		DECEL_ORIENTATION_MIN,

		// Vitesse
		VITESSE_DISTANCE_1,
		DISTANCE_1,
		VITESSE_ORIENTATION_1,
		ANGLE_1,
		VMAX_ROBOT,

		//PID
		KP_DISTANCE,
		KI_DISTANCE,
		KD_DISTANCE,
		KP_ORIENTATION, 
		KI_ORIENTATION,
		KD_ORIENTATION,
		I_VITESSE_MAX,
		KP_BRAKE, 
		KI_BRAKE, 
		KD_BRAKE,
		I_BRAKE_MAX,
		KP_HYBRIDE,

		// AUtres paramètres
		COEF_DECEL,
		IMMOBILITE,
		TENSION_MOTEUR,
		TENSION_ALIM,

		// Mode de rotation
		POLARITE_MOTEUR_D,
		POLARITE_MOTEUR_G,
		POLARITE_MOTEUR_X,
		POLARITE_MOTEUR_Y,
		POLARITE_CODEUR_D,
		POLARITE_CODEUR_G
	}_eser_data_asserv;

	typedef enum
	{
		ENTRAXE,
		PERIMETRE_ROUE,
		COEF_CODEUR_D,
		COEF_CODEUR_G,
		NB_IMPULSION_CODEUR,
		NOMBRE_IMPULSION_CODEUR
	}_eser_data_odo;

	typedef enum
	{
		AX_ANGLE,
		AX_POSITION,
		LED, 
		TORQUE, 
		ALIM,
		PING,
		DATA

	}_eser_ax12;

	typedef enum
	{
		AVANCER_RECULER, 
		REJOINDRE, 
		CIBLER, 
		ORIENTER, 
		CALAGE,
		CALAGE_X, 
		CALAGE_Y,
		CALAGE_ORIENTATION,
		TRAPEZE, 
		CARRE, 
		BRAKE, 
		UNBRAKE, 
		FIN_DEPLACEMENT,
		INIT_POSITION,
		INIT_X, 
		INIT_Y,
		INIT_TETA
	}_eser_fct_asserv;


	typedef enum
	{
		MARCHE_AVANT,
		MARCHE_ARRIERE
	}_eser_sens_asserv;

/******************************************************************************/
/************************** DEFINITION DES UNIONS *****************************/
/******************************************************************************/

	typedef union
	{
		_eser_fct_asserv	fct_asserv		: 8;
		_eser_data_asserv   data_asserv		: 8;
		_eser_data_odo      data_odo		: 8;
		_eser_ax12			ax12			: 8;
		uint8_t				value			: 8;
	}_user_var_id;

	typedef union
	{
		_eser_data_type		data_type		: 8;
		_eser_sens_asserv	sens			: 8;
		uint8_t				id_ax12			: 8;
	}_user_type_id;

	typedef union 
	{
		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;
		int8_t int8;
		int16_t int16;
		int32_t int32;
		int64_t int64;
		float reel32;
		double reel64; 
	}_user_data_type ;

/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

	// user = union serialus
	// eser = enum  serialus



	typedef struct
	{
		uint8_t h1					: 8;
		uint8_t h2					: 8;
		uint8_t main_header			: 8;				
		uint8_t sub_header			: 8;					
		uint8_t	var_id				: 8;		
		uint8_t type_id				: 8;

	}_frame_header;

	typedef struct
	{
		uint8_t cs1					: 8;
		uint8_t cs2					: 8;
	}_fletcher_checksum;

	typedef struct
	{
		_user_data_type data1;
		_user_data_type data2;
		_user_data_type data3;
	}_frame_data_tx;

	typedef struct
	{
		_user_data_type data3;
	}_frame_data_rx;

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/


#ifdef	__cplusplus
}
#endif

#endif
