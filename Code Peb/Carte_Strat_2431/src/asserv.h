//-----------------------------------------------
//	I2C SUR 18F45K20 (DEMOBOARD)
//
//	Auteurs : PEB / La Poutre
//	Compilateur : MCC18 - Microchip	
//
//	id@asserv.h
//	revision@1.0.0
//	04/01/10 : 03h31
//-----------------------------------------------

/** T Y P E S *********************************************************/
	#ifdef TYPE_ASSERV_EXT
		#define TYPE_ASSERV
	#else
		#define TYPE_ASSERV	extern
	#endif


/** I N C L U D E S **************************************************/
	#include "define_cod.h"
	

/** D E F I N I T I O N S *********************************************/

	//Fonctions de DELPACEMENT
	#define RECULE_CALLAGE_NORMALE			1
	#define RECULE_CALLAGE_EPIS				2
	#define RECULE_CALLAGE_DECA_GAUCHE		3
	#define RECULE_CALLAGE_DECA_DROITE		4
	
	#define SEUIL_RECULE					1500
	
	#define TEMPS_ATTENTE_STAB_ANGLE		2000	//2300	//400 modif du 13/04 à 00h		//2000
	#define SEUIL_ANGLE_BAS					10.0	//10.0
	#define SEUIL_ANGLE_HAUT				10.0	//10.0
	
	#define TEMPS_ATTENTE_STAB_DIST			1000	//600
	#define TEMPS_ATTENTE_STAB_DIST_V350	547
	#define SEUIL_DISTANCE_AV				300.0
	#define SEUIL_DISTANCE_AR				300.0
	
	#define FORCE_SUR_ANGLE_ACTUEL			10000
	#define PAS_DE_CORRECTION_ANGLE_FIN		10001
	#define PAS_DE_CORRECTION_ANGLE			10002

	//
	#define _STOP_ROBOT						Asserv.flag_stop_asserv = 1;
	#define _GO_ROBOT						Asserv.flag_stop_asserv = 0;
	
	//COMMANDE UART
	#define PAS_COMMANDE_MOTEURS		10
	
	//PIN I/O
	#define SENS_MOT_G					RC0
	#define	SENS_MOT_D					RC1

	#define PWM_OUT_1					RB1
	#define PWM_OUT_2					RB3
	
	//Informations robot
	#define PI							3.14159265359
	#define CORRECTION_NB_IMP_CODEUR	2.769252071	//Calcul du coef : 20000/7222.16666666=2.76925207117
	#define DIAM_ROUE_COD 				40.4		//41.0	// en mm
	#define LARGEUR_ENTRE_ROUES_FOLLES 	314.0		//324.0	// en mm
	
	//Coeficient
	#define KP						0
	#define KD						1
	
	#define DISTANCE_1				0
	#define DISTANCE_2				1
	#define DISTANCE_3				2
	
	#define ANGLE_1					0
	#define ANGLE_2					1
	#define ANGLE_3					2
	
	#define COEF_1					0
	#define COEF_2					1
	#define COEF_3					2
	
	#define ANGLE_DEPART_THETA		0
	
	#define KP_DISTANCE_1			0.4
	#define KP_DISTANCE_2			0
	#define KP_DISTANCE_3			0
	
	#define KD_DISTANCE_1			0.5
	#define KD_DISTANCE_2			0
	#define KD_DISTANCE_3			0
	
	#define KP_ANGLE_1				10		//40 reglage de gain en rotation)
	#define KP_ANGLE_2				0
	#define KP_ANGLE_3				30		//Reglage pour l'angle
	
	#define KD_ANGLE_1				150		//100 reglage de gain en rotation)		
	#define KD_ANGLE_2				0
	#define KD_ANGLE_3				100		//avant test pompage : 100	//100<=val avant correction du pb 180		//Reglage pour l'angle
	
	#define COEF_SAT_VIT_1			0.3
	#define COEF_SAT_VIT_2			0
	#define COEF_SAT_VIT_3			1
	
	
	#define KP_VITESSE				1
	#define KD_VITESSE				0
	
	#define AJUST_RAPPORT_ROUES_FOLLES	1
	#define SENS_CODEUR					-1		//-1 si le bloc moteur+codeur est inversé


/** V A R I A B L E S *************************************************/
	//Structure 16bit
	typedef struct {
		union {
			struct {
				char L;
				char H;
			} Octet;
			int Full;
		} u16b;
	} Struct_16bit;
	
	//Structure de l'asservicement
	TYPE_ASSERV	volatile struct {
		
		//FLAG
		INT_S16B flag_stop_asserv;		//pour faire eviter les bugs de deplacement non souhaités
		INT_S16B flag_evitement_adv;
		INT_S16B flag_sortie_evitement_adv;
		INT_S16B flag_calcul_atan2;
		INT_S16B flag_affichage_debug;
		INT_S16B flag_control_clavier;
		INT_S16B flag_ass_distance;
		INT_S16B flag_ass_angle;
		INT_S16B flag_ass_vitesse;
		INT_S16B flag_ass_saturation_vitesse;
		INT_S16B flag_ass_saturation_acceleration;
		INT_S16B flag_ass_commande_moteur_polaire;
		INT_S16B flag_ass_callage;
		
		//PRINCIPALES
		INT_S16B inversion_repere;
		INT_S16B angle_inversion_repere;
		INT_S16B commande_roue_G_clavier;
		INT_S16B commande_roue_D_clavier;
		INT_S16B commande_roue_G;
		INT_S16B commande_roue_D;
		INT_S16B commande_roue_G_precedente;
		INT_S16B commande_roue_D_precedente;
		
		INT_S16B commande_correction_epis_g;
		INT_S16B commande_correction_epis_d;
		
		INT_S32B imp_roue_G;
		INT_S32B imp_roue_D;
		
//		INT_S32B imp_roue_G_back;	//permet une compensation en cas de non reception lors d'une phase
//		INT_S32B imp_roue_D_back;	//permet une compensation en cas de non reception lors d'une phase
		
//		INT_S32B distance_parcourue;
//		INT_S32B distance_parcourue_precedent;
		
		INT_S32B theta_actuel;
		INT_S32B theta_precedent;
		INT_S32B angle_depart_theta_0;
		
		INT_S32B x_actuel;
		INT_S32B y_actuel;
		INT_S32B x_precedent;
		INT_S32B y_precedent;
		
		INT_S32B x_actuel_preparation;	//Permet de faire le calcul de x pour la 1er vas vers
		
		INT_S32B vitesse_ass_distance;
		INT_S32B vitesse_ass_orientation;
		INT_S32B vitesse_ass_distance_precedente;
		INT_S32B vitesse_ass_orientation_precedente;
		INT_S32B vitesse_roue_D;
		INT_S32B vitesse_roue_G;
		INT_S32B vitesse_roue_D_precedente;
		INT_S32B vitesse_roue_G_precedente;
		INT_S32B acceleration_roue_D;
		INT_S32B acceleration_roue_G;
		
		//FONCTIONS DEPLACEMENTS
		INT_S32B x_consigne;
		INT_S32B y_consigne;
		//INT_S32B theta_consigne;
		INT_S32B consigne_vitesse;
		INT_S32B vitesse_max;
		INT_S32B acceleration_max;
		INT_S16B orientation_depart;
		//INT_S16B direction_orientation;
		INT_S32B ecart_distance_precedent;
		
		//FONCTIONS ASS_DISTANCE
		FLOAT_S32B consigne_distance;
		INT_S32B commande_distance;
		INT_S32B commande_distance_precedente;
		INT_S32B offset_correcteur_distance;
		FLOAT_S32B tableau_correcteur_distance[2][3];
		
		//FONCTIONS ASS_ANGLE
		FLOAT_S32B consigne_angle;
		FLOAT_S32B consigne_angle_atan2;
		FLOAT_S32B consigne_angle_precedent;
		INT_S32B commande_angle;
		INT_S32B commande_angle_precedente;
		INT_S32B offset_correcteur_angle;
		FLOAT_S32B tableau_correcteur_angle[2][3];
		
		FLOAT_S32B tableau_coef_saturation_vitesse[3];	//raport entre la saturation distance et angle
		INT_S32B offset_coef_saturation_vitesse;
		
		//Debug
		FLOAT_S32B debug[2];
	} Asserv;


/** P R O T O T Y P E S ***********************************************/
	void Asservissement(void);
	void ass_angle(void);
	void ass_distance(void);
	void ass_vitesse(void);
	void ass_saturation_vitesse(void);
	void ass_saturation_acceleration(void);
	void ass_commande_moteur_polaire(void);
	void ass_envoi_pwm(void);
	
	void inversion_repaire(int consigne_angle_deg);
	
	void tourner_angle(INT_S32B consigne_angle_deg, INT_S32B vitesse);
	
	void tourner_cible(INT_S32B x_consigne_mm, INT_S32B y_consigne_mm, INT_S32B vitesse);
	
	void vas_vers(INT_S32B x_consigne_mm, INT_S32B y_consigne_mm, INT_S32B vitesse, INT_S32B angle_force_deg);

	void avance_callage(INT_US32B timeout_final);
	
	void recule_callage(INT_S32B angle, INT_S32B x_mm, INT_S32B y_mm, INT_US32B timeout_final, int mode);
	
	void Stop(int timeout_sec);
	
	void StopAdv(void);
	
	void Attente(INT_US32B timeout_final);
	
	
