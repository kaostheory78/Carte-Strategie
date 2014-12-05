/*****************************************************************************
* SUDRIABOTIK
******************************************************************************
* Nom Fichier 	:	define_cod.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Constant definitions
******************************************************************************/

//CONTROL CLAVIER
#define AFF_DEBUG_STANDBY				0
#define AFF_DEBUG_ROBOT					1
#define AFF_DEBUG_AIDE					2


//TYPES VARIALBES
	#define INT_S16B					signed int
	#define INT_S32B					signed long long
	#define INT_US32B					unsigned long long
	#define INT_S64B					signed long long
	#define FLOAT_S32B					float
	

#define 	VISU_OSCILLO	RC2
#define 	HTR_OUT			RC3

#define 	PSA				T0PS3
#define 	S				START

// Adresse I2C de l'esclave
#define SLAVE_NODE_ADDR 	0x10	// I2C address pour PIC GAUCHE

#define SL_WRITE_ADDRESS	1
#define SL_WRITE_OFFSET		2
#define SL_WRITE_NBDATA		3
#define SL_WRITE_DATA		4
#define SL_READ_ADDRESS		5
#define SL_READ_DATA		6



//********************************
// PHASES HTR
//********************************
// Variable : phase_htr
#define PHTR_ATTENTE_2			102
#define PHTR_ATTENTE			100

//#define PHTR_1				1
//#define PHTR_2				2
//#define PHTR_3				3
//#define PHTR_4				4

// Variable : flag_phase_htr
#define TRAITEMENT			0
#define ATTENTE				1

// Variable : timer_htr
#define HTR_TIMER_NONE		00000	// 12.5ms	=> 12500/0.8 = 15625 => 65535 - 15625 = 49910
#define HTR_TIMER_1			49910 	// 18ms 	=> 18000/0.8 = 22500 => 65535 - 22500 = 43035
#define HTR_TIMER_2			49910 	// 10ms 	=> 10000/0.8 = 12500 => 65535 - 12500 = 53035
#define HTR_TIMER_3			49910 	// 25ms 	=> 25000/0.8 = 31250 => 65535 - 31250 = 34285
#define HTR_TIMER_4			49910 	// 1ms  	=> 1000/0.8  = 1250  => 65535 - 1250  = 64285
#define PERIODE_2_5MS		62410	// 2.5ms	=> 2500/0.8 = 3125 => 65535 - 3125 = 62410
#define PERIODE_25MS		34285
#define PERIODE_12_5MS		49910
#define PERIODE_10MS		53035
#define PERIODE_5MS			59285	// 5ms		=> 5000/0.8 = 6250 => 65535 - 6250 = 62410
#define PERIODE_3MS			61785	// 3ms	=> 3000/0.8 = 3750 => 65535 - 3125 = 61785
#define PERIODE_1MS			64285	// 1ms	=> 1000/0.8 = 1250 => 65535 - 1250 = 64285
#define NB_PERIODE_2_5MS	20		//
#define NB_PERIODE_5MS		10		//
#define NB_PERIODE_10MS		5		//
#define NB_PERIODE_12_5MS	4		//
#define NB_PERIODE_25MS		2		//

#define DUREE_PHASE_TRAITEMENT		25
#define PERIODE_HTR					PERIODE_25MS		//PERIODE_12_5MS
#define NB_PERIODE_HTR				NB_PERIODE_25MS		//NB_PERIODE_12_5MS


//---------------------------------------------------------------------
// Receive Buffer Index Values (Réception du Master)
//---------------------------------------------------------------------
#define	SLAVE_ADDR		0	// 
#define	DATA_LEN		1	//
#define	DATA_OFFS		2	//
#define	RX_DATA			3	//


//********************************
// PHASES I2C
//********************************
// pour master
#define P_I2C_ATTENTE			0
#define P_I2C_WR_START			1
#define P_I2C_WR_ADDR			2
#define P_I2C_WR_DATA			3
#define P_I2C_WR_ACK_DATA		4
#define P_I2C_RD_START			5
#define P_I2C_RD_ADDR			6
#define P_I2C_RD_DATA			7
#define P_I2C_RD_DATA_OK 		8
#define P_I2C_RD_ACK_DATA		9
// pour slave
#define P_I2C_SL_ATTENTE_START	0
#define P_I2C_SL_START			1
#define P_I2C_SL_REC_DATA		2
#define P_I2C_SL_EMI_DATA		3
#define P_I2C_SL_ATTENTE_STOP	4

#define BUF_I2C_LENGTH_REC		31
#define BUF_I2C_LENGTH_EMI		31


//********************************
// ERREURS
//********************************
// Variable : erreur_htr
#define ERHTR_PHASE				9
#define ERHTR_DEP_HTR_1			1
#define ERHTR_DEP_HTR_2			2
#define ERHTR_DEP_HTR_3			3
#define ERHTR_DEP_HTR_4			4

#define ERPSP_WR	1
#define ERPSP_RD	2

