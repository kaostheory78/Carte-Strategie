//-----------------------------------------------
//	I2C SUR 18F45K20 (DEMOBOARD)
//
//	Auteurs : PEB
//	Compilateur : MCC18 - Microchip	
//
//	id@strategies.c
//	revision@1.0.0
//	04/01/10 : 03h31
//-----------------------------------------------

/******** REPERE DU ROBOT *****************

				  ^ X
		  		  |
		+-------------------+
		|	    			|
   Y <--|	 	  +			|
		|--+	(0,0)	 +--|
		+--+-------------+--+

*******************************************/

/** T Y P E S *********************************************************/
	#define TYPE_STRAT_EXT


/** I N C L U D E S **************************************************/
	#include <pic18.h>
	#include "a_fonctions.h"
	#include "strategies.h"
	#include "asserv.h"
	#include "a_affich.h"
	#include "v_glob_cod.h"


/** F O N C T I O N S**************************************************/
void TraitementPrincipal(void)
{
	LED_JAUNE 	= 1;		
	
	/*** SEQUENCE JACK ***/	
	//Configuration avant mise en place du JACK

	while (!SYS_JACK)
	{
		LedCouleurDepart();
	}
		
	//Callage du robot sur la bordure arrière
	//Attente de DEPART aussi
	//Lancement du timeout_match aussi inclus ;)
	CallageDepart();
	
		
	/*** STRATEGIES ***/
	//Stratégie d'homologation
	//StratHomologation();
	
	//Stratégie FRA
	Test();
	
	/*** FIN DE MATCH ***/
	FinMatch();
}

void ActionAutomatisme(int type, int attente_ou_pas)
{
	while(1)
	{
		//On envoi le flag en cours
		buffer_i2c_tx[0] = type;
		
		//Si le bit7 est à 1 le traitement est terminé
		if ((Strat.Flag.bits.bit7 == 1) || (attente_ou_pas == AUTOM_SANS_ATTENTE))
			break;
	}
	
	//Remise à 0 du flag d'envoi de strat
	if (attente_ou_pas == AUTOM_AVEC_ATTENTE)
	{
		timeout.autom = 0;
		timeout.blocage_timeout_autom = 0;
		
		while (timeout.autom <= TIMEOUT_AUTOM)
		{
			buffer_i2c_tx[0] = 0;
		}
		
		timeout.autom = 0;
		timeout.blocage_timeout_autom = 0;
	}	
}

int ActionAutomatismeBalise2(char autom)
{
	//Déclaration
	int i, j;
	
	//Remise à à
	Strategie.octet_recu_rx = 0;
	
	//Boucle d'attente
	for (i=0; i<10; i++)
	{
		putch(autom);
		
		for (j=0; j<200; j++)
		{
			//Prevention d'une erreur
			if (OERR)
			{
				CREN = 0;
				CREN = 1;
			}
			
			//Reception d'une réponse
			if (Strategie.octet_recu_rx == autom)
			{
				Strategie.octet_recu_rx = 0;
				return 1;
			}
		}
	}
	
	return 0;
}	

void Test(void)
{
	//Déclaration
	int ret = 0;
	
	
	/*** BONNE STRAT ***/
	//--------------
	//DEPART COLLINE
	//--------------
	ActionAutomatismeBalise2(_ROUE_ARRIERE_ON);
	ActionAutomatisme(ACT_CUL_ROBOT_MONT_COL, AUTOM_SANS_ATTENTE);
	vas_vers(Asserv.x_actuel_preparation, 280, 360, PAS_DE_CORRECTION_ANGLE_FIN);	//Changement après match2 : 290 sur y
	
	//Si on est JAUNE
	if (SYS_COULEUR_DEPART == JAUNE)
	{
		ActionAutomatisme(ACT_ORANGE_COL_JAUNE, AUTOM_AVEC_ATTENTE);
	}
	
	//Si on est BLEU
	else
	{
		ActionAutomatisme(ACT_ORANGE_COL_BLEU, AUTOM_AVEC_ATTENTE);
	}
	
	
	//----------
	//REDESCENTE
	//----------
	ActionAutomatismeBalise2(_ROUE_ARRIERE_ON_SENS2);
	Attente(TIMEOUT_DESCENTE_COLLINE);
	recule_callage(-90, 0, 0, TIMEOUT_RECULE_COLINE, RECULE_CALLAGE_NORMALE);
	
	ActionAutomatismeBalise2(_ROUE_ARRIERE_OFF);
	ActionAutomatisme(ACT_CUL_ROBOT_LEVE, AUTOM_AVEC_ATTENTE);
	
	tourner_angle(-95, 380);
	
	//Callage
	//Si on est JAUNE
	if (SYS_COULEUR_DEPART == JAUNE)
	{
		recule_callage(-95, 0, 0, TIMEOUT_CALLAGE_SORTIE_COL_1, RECULE_CALLAGE_NORMALE);
		Stop(1);
		recule_callage(-90, 0, 0, TIMEOUT_CALLAGE_SORTIE_COL_2, RECULE_CALLAGE_NORMALE);
	}
	//Si on est BLEU
	else
	{
		recule_callage(-90, 0, 0, TIMEOUT_CALLAGE_SORTIE_COL_2, RECULE_CALLAGE_NORMALE);
	}
	
	InitPositionRobot(PAS_INIT_SUR_AXE , 1280, 0, NIEME_INIT);
	
	vas_vers(-800, 1150, 350, 0);
	//tourner_cible(Asserv.x_actuel/10, 1225, 390);	//395
	//vas_vers(Asserv.x_actuel/10, 1225, 390, PAS_DE_CORRECTION_ANGLE_FIN);
	
	
	//------------
	//1ER POSITION
	//------------
	_EVITEMENT_ADV_AV_ACTIF
	tourner_cible(-320, 1000, 350);	//260	//370 sur x
	vas_vers(-320, 1000, 300, PAS_DE_CORRECTION_ANGLE_FIN);	//280
	ActionAutomatisme(ACT_CUL_ROBOT_MOITIER, AUTOM_AVEC_ATTENTE);
	
	//VERS 1ER POSITION
	tourner_angle(-90, 350);		//260
	tourner_cible(-250, 30, 350);	//240
	vas_vers(-250, 30, 300, PAS_DE_CORRECTION_ANGLE_FIN);	//-90);	//280
	
	//STRAT ADV
	//DETECTION ADV SUR : x=[15;-60] , y=[0;-150] => c=[-24;-75] f=[36;75]
	//Si l'adversaire n'est pas dans cette zone on enchaine nos déplacements
	/*
	if (!PositionAdv(-450, -750, 500, 600, TIMEOUT_BALISE_0, PAS_ATTENTE_FIN_TIMEOUT))
	{
		_ENCH_DEP_STRAT_POSSIBLE
		LED_BLEU = 1;
	}*/
	
	
	//-------------------
	//TRAJET PRINCIPAL 1
	//-------------------
	//DETECTION ADV SUR : x=[0;130] , y=[5;55] => c=[65;25] f=[65;30]
	//On attend on peu, si l'adversaire n'est pas dans cette zone on prend notre trajet principal
	//97,12
	ret = 0;	//PositionAdv(650, 250, 700, 300, TIMEOUT_BALISE_1, PAS_ATTENTE_FIN_TIMEOUT);	//-250, -750, 250, 600
	if (ret == 0)
	{
		tourner_cible(-300, -600, 350);	//240
		vas_vers(-300, -600, 300, PAS_DE_CORRECTION_ANGLE_FIN);	//280
		
		//TRAJET PRINCIPAL 2
		//DETECTION ADV SUR : x=[-40;105] , y=[-80;150] => c=[32;35] f=[73;115]
		//Si l'adversaire EST dans la zone on prend notre trajet principal
		//ret = PositionAdv(-250, -750, 250, 600, TIMEOUT_BALISE_0, PAS_ATTENTE_FIN_TIMEOUT);
		//if (ret == 0)
		//{
			tourner_cible(-360, -1000, 300);	//400 sur X avant 2ieme jour	//260
			vas_vers(-360, -1000, 300, PAS_DE_CORRECTION_ANGLE_FIN);	//280
			
			_EVITEMENT_ADV_INACTIF
			tourner_angle(172, 350);	//280
			
			//Ejection
			EjectionBalles();
			
			//Retour au point
			
			ActionAutomatisme(ACT_CUL_ROBOT_MOITIER, AUTOM_AVEC_ATTENTE);
			Stop(2);
			tourner_cible(-300, -600, 350);
			
			_EVITEMENT_ADV_AV_ACTIF
			tourner_cible(-250, 30, 350);	//240
			vas_vers(-250, 30, 300, -90);	//280

			//Diagonal_1();
			
			//On avance pour mettre d'autre tomates au cas où
			//recule_callage(122, 800, -1000, TIMEOUT_DIAGONALE_EPIS, RECULE_CALLAGE_EPIS);

		//}
		//else
		//{
			//Diagonale
			//Diagonal_1();
		//}	
	}
	
	//--------------------
	//TRAJET SECONDAIRE 1
	//--------------------
	else
	{
		/*
		//Première diagonale
		Diagonal_1();
		
		//Ejection
		EjectionBalles();
		*/
		//On avance pour mettre d'autre tomates au cas où
		//recule_callage(122, 800, -1000, TIMEOUT_DIAGONALE_EPIS, RECULE_CALLAGE_EPIS);
	}
}

void Diagonal_1(void)
{
	/*
	if (SYS_COULEUR_DEPART == JAUNE)
	{
		tourner_cible(130, 30, 350);		//280	//172 sur x en theorie							//tourner_cible(50, 270, 280);
		vas_vers(130, 30, 300, PAS_DE_CORRECTION_ANGLE_FIN);	//280	//vas_vers(50, 270, 280, PAS_DE_CORRECTION_ANGLE_FIN);
	}
	else
	{
		Stop(1);
		tourner_angle(0, 350);
		tourner_cible(100, 200, 350);		//280	//172 sur x en theorie							//tourner_cible(50, 270, 280);
		vas_vers(100, 200, 300, PAS_DE_CORRECTION_ANGLE_FIN);	//280	//vas_vers(50, 270, 280, PAS_DE_CORRECTION_ANGLE_FIN);
	}	
	
	ActionAutomatismeBalise2(_ROUE_ARRIERE_EJEC);
	tourner_angle(40, 350);		//260
	tourner_angle(80, 350);		//260
	tourner_angle(118, 350);	//260
	
	_EVITEMENT_ADV_AR_ACTIF
	recule_callage(122, 550, -650, TIMEOUT_DIAGONALE_EPIS, RECULE_CALLAGE_EPIS);
	
	tourner_angle(130, 300);	//280
	*/
}

void Diagonal_2(void)
{
	/*
	tourner_angle(0, 260);
	tourner_angle(40, 260);
	tourner_angle(80, 260);
	tourner_angle(122, 260);
	
	_EVITEMENT_ADV_AR_ACTIF
	ActionAutomatismeBalise2(_ROUE_ARRIERE_EJEC);
	recule_callage(122, 300, -1000, TIMEOUT_DIAGONALE_EPIS, RECULE_CALLAGE_EPIS);
	
	_EVITEMENT_ADV_INACTIF
	tourner_angle(175, 260);
	*/
}

void EjectionBalles(void)
{
	_STOP_ROBOT
	ActionAutomatismeBalise2(_ROUE_ARRIERE_EJEC);
	ActionAutomatisme(ACT_CUL_ROBOT_LEVE, AUTOM_AVEC_ATTENTE);
	ActionAutomatisme(ACT_ORANGE_LARGAGE, AUTOM_AVEC_ATTENTE);
	_GO_ROBOT
	
	
	ActionAutomatismeBalise2(_ROUE_ARRIERE_OFF);
	
	//Reprise
	//ActionAutomatisme(ACT_CUL_ROBOT_MOITIER, AUTOM_AVEC_ATTENTE);
	_ENCH_DEP_STRAT_IMPOSSIBLE
}	
	
void StratHomologation(void)
{
	/** DESCRIPTION : HOMOLOGATION *******************
	 *
	 *  
	 *
	 *************************************************/
	 /*
	//PHASE 1 :
	tourner_cible(-350, 900, 300);
	
	vas_vers(-350, 900, 300, FORCE_SUR_ANGLE_ACTUEL);
	
	tourner_cible(170, 0, 300);
	vas_vers(170, 0, 300, FORCE_SUR_ANGLE_ACTUEL);
	
	tourner_cible(790, -1090, 300);
	vas_vers(790, -1090, 300, 0);
	
	_EVITEMENT_ADV_INACTIF
	vas_vers(970, -1090, 300, 0);
	
	_EVITEMENT_ADV_ACTIF
	*/
}

void CallageDepart(void)
{
	//LED CALLAGE
	LED_JAUNE 	= 1;
	LED_BLEU	= 1;
	
	//On se calle contre la bordure
	recule_callage(-90, 0, 0, TIMEOUT_CALLAGE, RECULE_CALLAGE_NORMALE);
	
	//On réinit la position du robot
	InitPositionRobot(-800, 1300, 0, PREMIERE_INIT);
	
	//On avance pour terminer l'init
	vas_vers(-800, 1150, 310, -90);	//v=170
	tourner_angle(-90, 350);
	
	//On baisse la roue arrière
	ActionAutomatisme(ACT_CUL_ROBOT_MOITIER, AUTOM_AVEC_ATTENTE);
	
	//On coupe la commande_distance
	Asserv.commande_distance = 0;
	
	Asserv.x_actuel_preparation = Asserv.x_actuel/10;
	
	//Attente du retrait du JACK
	while (SYS_JACK)
	{
		LedCouleurDepart();
		
		//Désactivation du flag de sortie d'evitement adv
		Asserv.flag_sortie_evitement_adv = 0;
	}
	
	//Lancement du compteur d'1min30
	timeout.match = 0;
	timeout.blocage_timeout_match = 0;
}

void FinMatch(void)
{
	while(1);
}

void LedCouleurDepart(void)
{
	//DEPART BLEU
	if (SYS_COULEUR_DEPART == BLEU)
	{
		LED_JAUNE 	= 0;
		LED_BLEU	= 1;
		Asserv.orientation_depart = -1;
	}
	
	//DEPART JAUNE
	else
	{
		LED_JAUNE 	= 1;
		LED_BLEU	= 0;
		Asserv.orientation_depart = 1;
	}
}

void _CoupureAutom(void)
{
	//On coupe l'autom pour la fin du match
	while (1)
	{
		//Led de fin de match
		LED_JAUNE 	= 0;
		LED_BLEU	= 0;
		
		//Coupure de l'autom
		buffer_i2c_tx[0] = COUPURE_AUTOM;

		//Coupure du moteur arrière
		_putch(_ROUE_ARRIERE_OFF);
		
		//Prevention de plantage UART
		if (OERR) {CREN = 0;CREN = 1;}
		
		//On continue d'envoyer des PWM nulle pour stoper le robot
		ass_envoi_pwm();
	}	
}


/*** BACK HIER ***
int PositionAdv(long x_mm, long y_mm, long fourchette_x_mm, long fourchette_y_mm, INT_US32B timeout_fin, int attente_timeout)
{
	//Déclaration
	int ret 			= 0;
	int sortie 			= 0;
	int compteur 		= 0;
	//int prem_passage 	= 1;
	
	//Si le flag d'enchainement de deplacement est à un on saute
	if (Strategie.enchainement_deplacement)
		return 2;
	
	//Adaptation des dimentions
	x_mm /= 10;
	y_mm /= 10;
	y_mm *= Asserv.orientation_depart;
	
	fourchette_x_mm /= 10;
	fourchette_y_mm /= 10;
	
	//Mise en route du timeout
	timeout.balise						= 0;
	timeout.blocage_timeout_balise		= 0;
	
	//Boucle d'acquistion
	while ((timeout.balise <= timeout_fin) && (!sortie))
	{
		//Si l'adversaire est dans la fourchette sur x
		if (((Adv.X.Full < x_mm+fourchette_x_mm) && (Adv.X.Full > x_mm-fourchette_x_mm)) || (x_mm == POS_ADV_SUR_AUTRE_AXE))
		{
			//Si l'adversaire est dans la fourchette sur y
			if (((Adv.Y.Full < y_mm+fourchette_y_mm) && (Adv.Y.Full > y_mm-fourchette_y_mm)) || (y_mm == POS_ADV_SUR_AUTRE_AXE))
			{
				//On s'assure de la valildité de la position
				if (compteur >= 200)
				{
					if (attente_timeout == PAS_ATTENTE_FIN_TIMEOUT)
						sortie = 1;
				}
				else
				{
					compteur++;
				}
			}
			else
			{
				if (compteur > 0)
					compteur--;
			}
		}
		else
		{
			if (compteur > 0)
				compteur--;
		}	
	}
	
	//Deduction de la position
	if (compteur >= 200)
	{
		ret = 1;
	}
	
	//Mise en route du timeout
	timeout.blocage_timeout_balise		= 1;
	timeout.balise						= 0;
	
	return ret;
}*/


int PositionAdv(long x_mm, long y_mm, long fourchette_x_mm, long fourchette_y_mm, INT_US32B timeout_fin, int attente_timeout)
{
	//Déclaration
	int ret 			= 0;
	int sortie 			= 0;
	int compteur 		= 0;
	int compteur_max	= 150;
	
	//Si le flag d'enchainement de deplacement est à un on saute
	if (Strategie.enchainement_deplacement)
		return 2;
	
	//Adaptation des dimentions
	x_mm /= 10;
	y_mm /= 10;
	y_mm *= Asserv.orientation_depart;
	
	fourchette_x_mm /= 10;
	fourchette_y_mm /= 10;
	
	//Mise en route du timeout
	timeout.balise						= 0;
	timeout.blocage_timeout_balise		= 0;
	
	//Attente de maj des données
	while (timeout.balise <= TIMEOUT_BALISE_ATTENTE);
	
	//Boucle d'acquistion
	timeout.balise = 0;
	while ((timeout.balise <= timeout_fin) && (!sortie))
	{
		//Si l'adversaire est dans la fourchette sur x (or 0, considéré comme erreur)
		if (((Adv.X.Full < x_mm+fourchette_x_mm) && (Adv.X.Full > x_mm-fourchette_x_mm) && (Adv.X.Full != 0)))	// || (x_mm == POS_ADV_SUR_AUTRE_AXE))
		{
			//Si l'adversaire est dans la fourchette sur y (or 0, considéré comme erreur)
			if (((Adv.Y.Full < y_mm+fourchette_y_mm) && (Adv.Y.Full > y_mm-fourchette_y_mm) && (Adv.Y.Full != 0)))	// || (y_mm == POS_ADV_SUR_AUTRE_AXE))
			{
				//On s'assure de la valildité de la position
				if (((compteur >= compteur_max) || (compteur <= -compteur_max)) && (attente_timeout == PAS_ATTENTE_FIN_TIMEOUT))
				{
					sortie = 1;
					
					//Sortie immediate
					if (compteur >= compteur_max)
						return 1;
						
					if (compteur <= -compteur_max)
						return 0;
				}
				else
				{
					compteur++;
				}
			}
			else
			{
				compteur--;
			}
		}
		else
		{
			compteur--;
		}	
	}
	
	//Deduction de la position
	if (compteur >= compteur_max/2)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	
	//Mise en route du timeout
	timeout.blocage_timeout_balise		= 1;
	timeout.balise						= 0;
	
	return ret;
}

