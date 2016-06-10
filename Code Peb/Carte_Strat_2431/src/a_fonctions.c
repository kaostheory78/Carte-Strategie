/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	a_fonctions.c
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Fonctions applicati<on carte interface
******************************************************************************/


#include 	<pic18.h>			// définitions des registres PIC
#include	"delay.h"

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Tous les define pour la carte interface

#include 	"v_glob_cod.h"	// Variables globales utilisées par les fonctions

#include 	"a_fonctions.h"		// Prototypes des fonctions utilisées par main_cod.c
#include 	"a_affich.h"      	// Debug
#include 	"g_fonctions.h"    	// g_fonctions
#include 	"reglages_carte.h"  // reglages_carte

#include	"asserv.h"
#include	"strategies.h"


/** V A R I A B L E S *************************************************/
	//Buffer : X
	union {
		struct {
			char L;
			char H;
		} Octet;
		int Full;
	} X;
	
	//Buffer : Y
	union {
		struct {
			char L;
			char H;
		} Octet;
		int Full;
	} Y;
	
	//Buffer : THETA
	union {
		struct {
			char L;
			char H;
		} Octet;
		int Full;
	} THETA;
	

/**************************************************************************************/
// Fonctions Initialisation variables logicielles                                     
/**************************************************************************************/
void a_init_log (void)
{
	//
	temporisation_raf_debug = 0;
	
	//HTR
	//----
	phase_htr = 1;
	
	//I2C
	//----
	traitement_i2c_nb_octet = 0;
	
	//Init asserv
	//-------------
	Asserv.inversion_repere					= 0;
	Asserv.orientation_depart 				= 1;
	Asserv.angle_inversion_repere			= 0;
	
	Asserv.flag_stop_asserv						= 0;
	Asserv.flag_evitement_adv 					= 0;
	Asserv.flag_sortie_evitement_adv			= 0;
	Asserv.flag_calcul_atan2					= 0;
	Asserv.flag_affichage_debug					= AFF_DEBUG_STANDBY;
	Asserv.flag_control_clavier 				= 0;
	Asserv.flag_ass_distance 					= 0;
	Asserv.flag_ass_angle 						= 0;
	Asserv.flag_ass_vitesse 					= 0;
	Asserv.flag_ass_saturation_vitesse			= 0;
	Asserv.flag_ass_saturation_acceleration 	= 0;
	Asserv.flag_ass_commande_moteur_polaire 	= 0;
	Asserv.flag_ass_callage						= 0;
		
	Asserv.commande_roue_G_clavier 				= 1;
	Asserv.commande_roue_D_clavier 				= 1;
	Asserv.commande_roue_G			 			= 0;
	Asserv.commande_roue_D 						= 0;
	Asserv.commande_roue_G_precedente 			= 0;
	Asserv.commande_roue_D_precedente 			= 0;
	
	Asserv.commande_correction_epis_g			= 0;
	Asserv.commande_correction_epis_d			= 0;
	
	Asserv.imp_roue_G 							= 0;
	Asserv.imp_roue_D 							= 0;
	
//	Asserv.imp_roue_G_back						= 0;
//	Asserv.imp_roue_D_back						= 0;
	
//	Asserv.distance_parcourue 					= 0;
//	Asserv.distance_parcourue_precedent 		= 0;
	
	Asserv.angle_depart_theta_0 				= 0;	//CALCUL A RECUPERER EN IMPULSION EN PARTANT DES DEGRET((ANGLE_DEPART_THETA)
	Asserv.theta_actuel 						= Asserv.angle_depart_theta_0;
	Asserv.theta_precedent 						= Asserv.angle_depart_theta_0;
	
	Asserv.x_actuel 							= 0;
	Asserv.y_actuel 							= 0;
	//Asserv.x_precedent						= 0;
	//Asserv.y_precedent						= 0;
	
	Asserv.vitesse_ass_distance 				= 0;
	Asserv.vitesse_ass_orientation 				= 0;
	Asserv.vitesse_ass_distance_precedente 	  	= 0;
	Asserv.vitesse_ass_orientation_precedente 	= 0;
	Asserv.vitesse_roue_D 						= 0;
	Asserv.vitesse_roue_G 						= 0;
	Asserv.vitesse_roue_D_precedente 			= 0;
	Asserv.vitesse_roue_G_precedente 			= 0;
	Asserv.acceleration_roue_D 					= 0;
	Asserv.acceleration_roue_G 					= 0;
	
	Asserv.x_consigne 							= 0;
	Asserv.y_consigne 							= 0;
	//Asserv.theta_consigne 					= 0;
	Asserv.consigne_vitesse 					= 0;
	Asserv.vitesse_max 							= 0;
	Asserv.acceleration_max 					= 0;
	//Asserv.sens_deplacement 					= 0;
	//Asserv.direction_orientation 				= 0;
	
	Asserv.commande_distance 					= 0;
	Asserv.commande_distance_precedente			= 0;
	Asserv.offset_correcteur_distance 			= 0;
	Asserv.tableau_correcteur_distance[KP][DISTANCE_1] = KP_DISTANCE_1;
	Asserv.tableau_correcteur_distance[KP][DISTANCE_2] = KP_DISTANCE_2;
	Asserv.tableau_correcteur_distance[KP][DISTANCE_3] = KP_DISTANCE_3;
	Asserv.tableau_correcteur_distance[KD][DISTANCE_1] = KD_DISTANCE_1;
	Asserv.tableau_correcteur_distance[KD][DISTANCE_2] = KD_DISTANCE_2;
	Asserv.tableau_correcteur_distance[KD][DISTANCE_3] = KD_DISTANCE_3;
	
	Asserv.consigne_angle 						= 0;
	Asserv.commande_angle 						= 0;
	Asserv.commande_angle_precedente 			= 0;
	Asserv.offset_correcteur_angle 				= 0;
	Asserv.tableau_correcteur_angle[KP][ANGLE_1] = KP_ANGLE_1;
	Asserv.tableau_correcteur_angle[KP][ANGLE_2] = KP_ANGLE_2;
	Asserv.tableau_correcteur_angle[KP][ANGLE_3] = KP_ANGLE_3;
	Asserv.tableau_correcteur_angle[KD][ANGLE_1] = KD_ANGLE_1;
	Asserv.tableau_correcteur_angle[KD][ANGLE_2] = KD_ANGLE_2;
	Asserv.tableau_correcteur_angle[KD][ANGLE_3] = KD_ANGLE_3;
	
	Asserv.offset_coef_saturation_vitesse = 0;
	Asserv.tableau_coef_saturation_vitesse[COEF_1] = COEF_SAT_VIT_1;
	Asserv.tableau_coef_saturation_vitesse[COEF_2] = COEF_SAT_VIT_2;
	Asserv.tableau_coef_saturation_vitesse[COEF_3] = COEF_SAT_VIT_3;
	
	//Timeout
	timeout.match 						= 0;
	timeout.evitement 					= 0;
	timeout.deplacement 				= 0;
	timeout.callage						= 0;
	timeout.autom						= 0;
	timeout.balise						= 0;
	timeout.blocage_timeout_match 		= 1;
	timeout.blocage_timeout_evitement 	= 1;
	timeout.blocage_timeout_deplacement = 1;
	timeout.blocage_timeout_callage 	= 1;
	timeout.blocage_timeout_autom		= 1;
	timeout.blocage_timeout_balise		= 1;
	
	//Variable de la strat
	Strategie.etat_match = MATCH_EN_COURS;
	Strategie.enchainement_deplacement = 0;
	Strategie.octet_recu_rx = 0;
	Strategie.etape			= 0;
	
	//Flag Strat
	/*
	FLAG_RAMASSAGE_ORANGES 	= 0;
	FLAG_RAMASSAGE_TOMATES 	= 0;
	FLAG_RAMASSAGE_EPIS 	= 0;
	FLAG_DEPOSE_BALLES 		= 0;
	FLAG_DEPOSE_EPIS		= 0;
	*/
	
	ActionAutomatismeBalise2(_ROUE_ARRIERE_OFF);
}

void InitPositionRobot(int x_mm, int y_mm, int theta_deg, int numero_init)
{
	//On coupe la commande_distance
	Asserv.commande_distance = 0;
	
	//Déblocage du timeout callage
	timeout.blocage_timeout_callage = 0;
	
	//Pause
	while (timeout.callage < TIMEOUT_CALLAGE/3);
	
	//Blocage du timeout callage
	timeout.blocage_timeout_callage = 1;
	timeout.callage = 0;
	
	//Init de X
	if (x_mm != PAS_INIT_SUR_AXE)
	{
		if (numero_init == PREMIERE_INIT)
		{
			Strategie.x_init = (-Asserv.x_actuel) + x_mm*10;	//L'inverse de x_actuel pour calibrer à 0, puis ajout de l'init
		}
		else
		{
			Strategie.x_init += (-Asserv.x_actuel) + x_mm*10;
		}
	}	
	
	//Init de Y
	if (y_mm != PAS_INIT_SUR_AXE)
	{
		if (numero_init == PREMIERE_INIT)
		{
			Strategie.y_init = (-Asserv.y_actuel) + (Asserv.orientation_depart*y_mm*10);	//L'inverse de y_actuel pour calibrer à 0, puis ajout de l'init
			//Strategie.y_init *= Asserv.orientation_depart;		//Y en fonction de la couleur de depart
		}
		else
		{
			//Strategie.y_init *= Asserv.orientation_depart;
			Strategie.y_init += (-Asserv.y_actuel) + (Asserv.orientation_depart*y_mm*10);
			//Strategie.y_init *= Asserv.orientation_depart;
		}		
	}	
	
	//Init de theta
	/*
	if (theta_deg != PAS_INIT_SUR_AXE)
	{
		Strategie.theta_init = (-Asserv.theta_actuel) + theta_deg*10;
		//Asserv.consigne_angle = theta_deg*10;
	}
	*/
}
	
void TraitementI2C(void)
{	
	//Mise à jour des coordonnées précentes
	Asserv.x_precedent = Asserv.x_actuel;
	Asserv.y_precedent = Asserv.y_actuel;
	Asserv.theta_precedent = Asserv.theta_actuel;
	
	//Position du robot
	X.Octet.H = buffer_i2c_rx[0];
	X.Octet.L = buffer_i2c_rx[1];
	Y.Octet.H = buffer_i2c_rx[2];
	Y.Octet.L = buffer_i2c_rx[3];
	THETA.Octet.H = buffer_i2c_rx[4];
	THETA.Octet.L = buffer_i2c_rx[5];
	
	//Flags et capteurs
	Strat.Flag.Octet = buffer_i2c_rx[13];
	SYS_STRAT_1 = buffer_i2c_rx[14];
	
	//Position de l'adv
	Adv.X.Octet.L = buffer_i2c_rx[15];
	Adv.X.Octet.H = buffer_i2c_rx[16];
	Adv.Y.Octet.L = buffer_i2c_rx[17];
	Adv.Y.Octet.H = buffer_i2c_rx[18];	
	
	
	//Traitement
	if (Asserv.inversion_repere)
		Asserv.x_actuel = -(X.Full + Strategie.x_init);
	else
		Asserv.x_actuel = X.Full + Strategie.x_init;
	
	if (Asserv.inversion_repere)	
		Asserv.y_actuel = -(Y.Full + Strategie.y_init);
	else
		Asserv.y_actuel = Y.Full + Strategie.y_init;
		
	
	//Recadrage de l'angle si on a inversé le repère
	if (Asserv.inversion_repere == 1)
	{
		//Si l'angle reel du robot est positif
		if (THETA.Full >= 0)
		{
			Asserv.theta_actuel = -1800 + THETA.Full + Strategie.theta_init;
		}	
		else
		{
			Asserv.theta_actuel = 1800 + THETA.Full + Strategie.theta_init;
		}
	}
	else
	{
		Asserv.theta_actuel = THETA.Full + Strategie.theta_init;
	}	
	
	
	
	/*
	
		if (Strategie.theta_init > 0)
		{
			Asserv.theta_actuel = THETA.Full - Strategie.theta_init*10;
		}
		else
		{	
			Asserv.theta_actuel = THETA.Full + Strategie.theta_init*10;
		}
	}
	else
	{
		Asserv.theta_actuel = THETA.Full + Strategie.theta_init*10;
	}*/	
}

