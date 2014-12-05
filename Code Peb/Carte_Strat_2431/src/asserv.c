//-----------------------------------------------
//	I2C SUR 18F45K20 (DEMOBOARD)
//
//	Auteurs : PEB / La Poutre
//	Compilateur : MCC18 - Microchip	
//
//	id@asserv.c
//	revision@1.0.0
//	04/01/10 : 03h31
//-----------------------------------------------


/** T Y P E S *********************************************************/
	#define TYPE_ASSERV_EXT


/** I N C L U D E S **************************************************/
	#include <pic18.h>
	#include <math.h>
	#include "asserv.h"
	#include "a_affich.h"
	#include "a_fonctions.h"
	#include "strategies.h"
	

/** F O N C T I O N S**************************************************/	
void Asservissement(void)
{	
	//Déclaration
	FLOAT_S32B delta_x = Asserv.x_consigne - Asserv.x_actuel;
	FLOAT_S32B delta_y = Asserv.y_consigne - Asserv.y_actuel;
	
	//Calcul de l'angle de la cible par rapport à l'angle actuel
	Asserv.consigne_angle_atan2 = atan2(delta_y, delta_x);

	//Passage en radian
	Asserv.consigne_angle_atan2 *=(3600/(2*PI));
	
	//Flag du calcul terminé
	Asserv.flag_calcul_atan2 = 1;
	
	
	//***************
	// ASSERVICEMENT
	//***************
	if (Asserv.flag_ass_distance)
	{
		ass_distance();
	}
	else
	{
		if (!Asserv.flag_ass_callage)
			Asserv.commande_distance = 0;
	}
		
	if (Asserv.flag_ass_angle)
	{
		ass_angle();
	}
	else
	{
		Asserv.commande_angle = 0;
	}		
	
	if (Asserv.flag_ass_vitesse)
		ass_vitesse();
	
	if (Asserv.flag_ass_saturation_vitesse)
		ass_saturation_vitesse();
		
	if (Asserv.flag_ass_saturation_acceleration)
		ass_saturation_acceleration();
	
	if (Asserv.flag_ass_commande_moteur_polaire)
		ass_commande_moteur_polaire();
	
	if (Asserv.flag_evitement_adv)
		StopAdv();
		
	//**************
	//ENVOI DES PWM
	//**************
	ass_envoi_pwm();
}
	
void ass_angle(void)
{
	//************
	//DECLARATION
	//************
	FLOAT_S32B ecart_angle;
	FLOAT_S32B ecart_max_sur_KP;
	FLOAT_S32B correction_KD;
	FLOAT_S32B correction_max_KD;
		
	//*******
	//CALCUL
	//*******
	//Mise à jour de la vitesse d'orientation précedente
	Asserv.vitesse_ass_orientation_precedente = Asserv.vitesse_ass_orientation;
	
	//Calcul de la vitesse d'orientation
	Asserv.vitesse_ass_orientation = (int)(Asserv.theta_actuel- Asserv.theta_precedent);
	
	//Mise à jour du theta précédent
	Asserv.theta_precedent = Asserv.theta_actuel;

	//Deduction de l'erreur en angle	
	ecart_angle = Asserv.consigne_angle - Asserv.theta_actuel;
	
	//Calcul de l'ecart max sur KP possible pour éviter des problèmes de signes sur les valeurs 32bits
	ecart_max_sur_KP = 1500/Asserv.tableau_correcteur_angle[KP][Asserv.offset_correcteur_angle];
	
	//Saturation de l'ecart pour le calcul de la commande
	if (ecart_angle > ecart_max_sur_KP)
	{
		ecart_angle = ecart_max_sur_KP;
	}
	
	Asserv.debug[0] = ecart_angle;
	
	//Calcul de KD max pour eviter les arret si l'acceleration devient trop importante
	correction_KD 		= Asserv.tableau_correcteur_angle[KD][Asserv.offset_correcteur_angle] * (double)Asserv.vitesse_ass_orientation;
	correction_max_KD 	= Asserv.tableau_correcteur_angle[KD][Asserv.offset_correcteur_angle]*13 * ((double)Asserv.vitesse_ass_orientation/100);
	
	//Saturation de la correction KD
	if (correction_KD > correction_max_KD)
	{
		correction_KD = correction_max_KD;
	}	
	
	//Calcul de la commande, application du PID
	Asserv.commande_angle = (int)(ecart_angle * Asserv.tableau_correcteur_angle[KP][Asserv.offset_correcteur_angle]);
	Asserv.commande_angle -= (int)(correction_KD);	//Asserv.tableau_correcteur_angle[KD][Asserv.offset_correcteur_angle] * (double)Asserv.vitesse_ass_orientation);
}

void ass_distance(void)
{
	//************
	//DECLARATION
	//************
	FLOAT_S32B commande_distance;
	FLOAT_S32B ecart_distance;
	FLOAT_S32B coef_Kp_hybride;
	FLOAT_S32B delta_x = Asserv.x_consigne - Asserv.x_actuel;
	FLOAT_S32B delta_y = Asserv.y_consigne - Asserv.y_actuel;
	
	//*******
	//CALCUL
	//*******
	//Mise à jour de la vitesse de la distance précedente
	Asserv.vitesse_ass_distance_precedente = Asserv.vitesse_ass_distance;
	
	//Calcul de la vitesse de la distance
	Asserv.vitesse_ass_distance = (int)(((Asserv.x_precedent - Asserv.x_actuel)+ (Asserv.y_precedent - Asserv.y_actuel))/2);
	
	//Calcul de la consigne de distance
	Asserv.consigne_distance = sqrt(((delta_x)*(delta_x)) + ((delta_y)*(delta_y))) - 100;	//-300;	//-300 : anticipation freinage
	
	//Signe des commandes
	/*
	if ((Asserv.consigne_angle_atan2 >= 180) || (Asserv.consigne_angle_atan2 <= -180))
	{
		Asserv.consigne_distance *= -1;
	}
	*/
	
	//Deduction de l'erreur de distance
	ecart_distance = Asserv.consigne_distance;	
	
	Asserv.debug[1] = ecart_distance;
	
	//Calcul du coef du gain proportionel hybride
	coef_Kp_hybride = 1.2*(1 - (fabs(Asserv.consigne_angle - Asserv.theta_actuel)/1800));	//kph=0.8 pour 10000 -10000
	
	//Calcul de la commande, application du PID
	commande_distance = (ecart_distance * Asserv.tableau_correcteur_distance[KP][Asserv.offset_correcteur_distance] );
	commande_distance -= Asserv.tableau_correcteur_distance[KD][Asserv.offset_correcteur_distance] * Asserv.vitesse_ass_distance;	
	commande_distance *= coef_Kp_hybride;
	Asserv.commande_distance = (int)commande_distance;
	
	//Mise à jour de l'erreur de distance précedente
	Asserv.ecart_distance_precedent = (int)ecart_distance;
	
	//Reculer si l'angle est trop important
	/*
	if ((Asserv.theta_actuel <= Asserv.theta_actuel-90) || (Asserv.theta_actuel >= Asserv.theta_actuel+90))
	{
		Asserv.commande_distance *= -1;
	}
	*/	
}	
	
void ass_vitesse(void)
{
	//************
	//DECLARATION
	//************
	INT_S32B ecart_vitesse_roue_D;
	INT_S32B ecart_vitesse_roue_G;
	
	
	//*******
	//CALCUL
	//*******
	//Calcul de l'ecart vitesse
	ecart_vitesse_roue_D = Asserv.vitesse_roue_D - Asserv.consigne_vitesse;
	ecart_vitesse_roue_G = Asserv.vitesse_roue_G - Asserv.consigne_vitesse;
	
	//Calcul de la commande des roues
	Asserv.commande_roue_D = (ecart_vitesse_roue_D * KP_VITESSE) - (Asserv.acceleration_roue_D * KD_VITESSE);
	Asserv.commande_roue_G = (ecart_vitesse_roue_G * KP_VITESSE) - (Asserv.acceleration_roue_G * KD_VITESSE);
}
	
void ass_saturation_vitesse(void)
{
	//************
	//DECLARATION
	//************
	INT_S32B vitesse_max_angle = Asserv.tableau_coef_saturation_vitesse[Asserv.offset_coef_saturation_vitesse]*Asserv.vitesse_max;	//0.3 rapport entre les saturations pour faire fonctionnenr l'asserv polaire
	
	
	//***********
	//TRAITEMENT
	//***********
	//Saturation de la vitesse pour l'asserv distance
	if (fabs(Asserv.commande_distance) > Asserv.vitesse_max)
	{
		if (Asserv.commande_distance < 0)
			Asserv.commande_distance = -Asserv.vitesse_max;
		else
			Asserv.commande_distance = Asserv.vitesse_max;
	}	
	
	
	if (fabs(Asserv.commande_angle) > vitesse_max_angle)
	{
		if (Asserv.commande_angle < 0)
			Asserv.commande_angle = -vitesse_max_angle;
		else
			Asserv.commande_angle = vitesse_max_angle;
	}
}
	
void ass_saturation_acceleration(void)
{
	//***********
	//TRAITEMENT
	//***********

	//Saturation de l'acceleration pour l'asserv distance
/*	if (fabs(Asserv.vitesse_ass_distance-Asserv.vitesse_ass_distance_precedente) > Asserv.acceleration_max)
	{
		if (Asserv.vitesse_ass_distance-Asserv.vitesse_ass_distance_precedente < 0)
			Asserv.commande_distance = Asserv.commande_distance_precedente - Asserv.acceleration_max;
		else
			Asserv.commande_distance = Asserv.commande_distance_precedente + Asserv.acceleration_max;
	}
*/	
	//Saturation de l'acceleration pour l'asserv orientation
	if (fabs(Asserv.vitesse_ass_orientation-Asserv.vitesse_ass_orientation_precedente) > Asserv.acceleration_max)
	{
		if (Asserv.debug[4] != 1)
			Asserv.debug[4] = 1;
		
		if (Asserv.vitesse_ass_orientation-Asserv.vitesse_ass_orientation_precedente < 0)
			Asserv.commande_angle = Asserv.commande_angle_precedente - Asserv.acceleration_max;
		else
			Asserv.commande_angle = Asserv.commande_angle_precedente + Asserv.acceleration_max;
	}

	/*
	//Saturation de l'acceleration pour la roue Droite
	if (fabs(Asserv.vitesse_roue_D-Asserv.vitesse_roue_D_precedente) > Asserv.acceleration_max)
	{
		if (Asserv.vitesse_roue_D-Asserv.vitesse_roue_D_precedente < 0)
			Asserv.commande_roue_D = Asserv.commande_roue_D_precedente - Asserv.acceleration_max;
		else
			Asserv.commande_roue_D = Asserv.commande_roue_D_precedente + Asserv.acceleration_max;
	}
	
	//Saturation de l'acceleration pour la roue Gauche
	if (fabs(Asserv.vitesse_roue_G-Asserv.vitesse_roue_G_precedente) > Asserv.acceleration_max)
	{
		if (Asserv.vitesse_roue_G-Asserv.vitesse_roue_G_precedente < 0)
			Asserv.commande_roue_G = Asserv.commande_roue_G_precedente - Asserv.acceleration_max;
		else
			Asserv.commande_roue_G = Asserv.commande_roue_G_precedente + Asserv.acceleration_max;
	}
	
	//Mise à jour dde l'acceleration des roues précédent
	Asserv.vitesse_roue_G_precedent = Asserv.vitesse_roue_G;
	Asserv.vitesse_roue_D_precedent = Asserv.vitesse_roue_D;
	*/
}

void ass_commande_moteur_polaire(void)
{
	//***********
	//TRAITEMENT
	//***********	
	Asserv.commande_roue_D = Asserv.commande_distance + Asserv.commande_angle + Asserv.commande_correction_epis_d;
	Asserv.commande_roue_G = Asserv.commande_distance - Asserv.commande_angle + Asserv.commande_correction_epis_g;
}

void ass_envoi_pwm(void)
{
	//************
	//DECLARATION
	//************
	INT_S16B commande_roue_G;
	INT_S16B commande_roue_D;
	Struct_16bit PWM_moteurGauche;
	Struct_16bit PWM_moteurDroit;	
	
	//*********************
	// ENVOI CONSIGNES PWM
	//*********************
	//Si on commande le robot au clavier
	if (Asserv.flag_control_clavier)
	{
		commande_roue_G = Asserv.commande_roue_G_clavier;
		commande_roue_D = Asserv.commande_roue_D_clavier;
	}
	else
	{
		commande_roue_G = Asserv.commande_roue_G;
		commande_roue_D = Asserv.commande_roue_D;
	}
	
	
	//Fin de MATCH
	if ((Strategie.etat_match == MATCH_TERMINE) || (Asserv.flag_stop_asserv))
	{
		commande_roue_G = 0;
		commande_roue_D = 0;
	}
		
	
	// COMMANDE ROUE G
	if (commande_roue_G >= 0)
	{
		SENS_MOT_G = 1;
		PWM_moteurGauche.u16b.Full = commande_roue_G;
		PDC1H = PWM_moteurGauche.u16b.Octet.H;
		PDC1L = PWM_moteurGauche.u16b.Octet.L;	
	}
	else
	{
		SENS_MOT_G = 0;
		PWM_moteurGauche.u16b.Full = -commande_roue_G;
		PDC1H = PWM_moteurGauche.u16b.Octet.H;
		PDC1L = PWM_moteurGauche.u16b.Octet.L;	
	}


	// COMMANDE ROUE D
	if (commande_roue_D >= 0)
	{
		SENS_MOT_D = 1;
		PWM_moteurDroit.u16b.Full = commande_roue_D;
		PDC0H = PWM_moteurDroit.u16b.Octet.H;
		PDC0L = PWM_moteurDroit.u16b.Octet.L;	
	}
	else
	{
		SENS_MOT_D = 0;
		PWM_moteurDroit.u16b.Full = -commande_roue_D;
		PDC0H = PWM_moteurDroit.u16b.Octet.H;
		PDC0L = PWM_moteurDroit.u16b.Octet.L;	
	}
}



/*****************************************************************/
/*****************   FONCTIONS DE DEPLACEMENTS   *****************/
/*****************************************************************/
void inversion_repaire(int consigne_angle_deg)
{	
	//Si la consigne n'est pas la même on met à jour le flag
	Asserv.angle_inversion_repere = consigne_angle_deg;
	Asserv.inversion_repere = !Asserv.inversion_repere;
	
	//Inversion des coordonnées d'init
//	Strategie.x_init *= -1;
//	Strategie.y_init *= -1;
	
	//Strategie.theta_init = 180;
	/*
	if (Strategie.theta_init > 0)
	{
		Strategie.theta_init = 180 - Strategie.theta_init;
	}
	else
	{	
		Strategie.theta_init = 180 + Strategie.theta_init;
	}*/	
	
	/*		
	//Recadrage de l'angle
	if (Strategie.theta_init > 1800)
	{
		Strategie.theta_init -= 3600;
	}	
	else if (Strategie.theta_init <= -1800)
	{
		Strategie.theta_init += 3600;
	}
	*/
	
	//Recalcul de la position du robot
	/*
	Asserv.x_actuel = X.Full + Strategie.x_init*10;
	Asserv.y_actuel = Y.Full + Strategie.y_init*10;
	Asserv.theta_actuel = THETA.Full + Strategie.theta_init*10;
	*/
}
	
void tourner_angle(INT_S32B consigne_angle_deg, INT_S32B vitesse)
{
	//Décalaration
	FLOAT_S32B seuil_angle_bas;
	FLOAT_S32B seuil_angle_haut;
	INT_S32B compteur_sortie = 0;

	//Passage des commandes 1Oième de deg
	consigne_angle_deg *= 10;
	consigne_angle_deg *= Asserv.orientation_depart;	//On prend en compte notre couleur de depart
	
	//Changement de repère si la consigne est 0 ou 180
	if ((consigne_angle_deg > 1700) || (consigne_angle_deg < -1700) || (consigne_angle_deg == 0))
	{
		//On inverse le repère si on ne l'a pas déjà fait avec la même consigne
		if (Asserv.angle_inversion_repere != consigne_angle_deg)
			inversion_repaire(consigne_angle_deg);
		
		//on recalcul notre angle
		if (consigne_angle_deg > 1700)
		{
			consigne_angle_deg = -1800 + consigne_angle_deg;
		}
		else if (consigne_angle_deg < -1700)
		{
			consigne_angle_deg = 1800 + consigne_angle_deg;
		}
	}

	//Configuration de l'asservissement
	Asserv.vitesse_max						= vitesse;
	Asserv.consigne_angle 					= consigne_angle_deg;
	Asserv.flag_ass_angle 					= 1;
	Asserv.flag_ass_distance 				= 0; 		//Attente du modulo pour le mettre à 1 pour qu'il puisse tourner sur lui même
	Asserv.flag_ass_commande_moteur_polaire = 1;
	Asserv.flag_ass_saturation_vitesse		= 1;
	Asserv.flag_ass_saturation_acceleration	= 0;
	Asserv.offset_correcteur_angle			= ANGLE_3;	//tableau coef correcteur
	Asserv.offset_coef_saturation_vitesse	= COEF_3;
	
	//Seuil
	seuil_angle_bas		= Asserv.consigne_angle - SEUIL_ANGLE_BAS;
	seuil_angle_haut	= Asserv.consigne_angle + SEUIL_ANGLE_HAUT;
	
	//Boucle 
	while(1)
	{		
		//Condition de sortie
		if ((Asserv.theta_actuel < seuil_angle_haut) && (Asserv.theta_actuel > seuil_angle_bas))
		{
			//On incremente un compteur pour attendre la stabilisation du robot
			compteur_sortie++;
			
			//Affichage
			puts("+");
			
			//Si le compteur à atteint son maxium on sort
			if (compteur_sortie >= TEMPS_ATTENTE_STAB_ANGLE)
				break;
		}
		else
		{
			//Si on est entre le seuil bas et le seuil haut on reset le compteur de sortie
			compteur_sortie = 0;
		}
	}
	
	Asserv.flag_sortie_evitement_adv = 0;
}

void tourner_cible(INT_S32B x_consigne_mm, INT_S32B y_consigne_mm, INT_S32B vitesse)
{
	//Décalaration
	FLOAT_S32B seuil_angle_bas;
	FLOAT_S32B seuil_angle_haut;
	INT_S32B compteur_sortie = 0;
	INT_S32B consigne_angle_deg = 0;
	
	//Inversion du repere
	if (Asserv.inversion_repere)
	{
		x_consigne_mm *= -1;
		y_consigne_mm *= -1;
	}	
	
	//Passage des commandes 1Oième de mm
	x_consigne_mm *= 10;	//X ne changera pas de sens qu'on soit JAUNE ou BLEU
	y_consigne_mm *= 10;
	y_consigne_mm *= Asserv.orientation_depart;	//Y oui en fonction de notre couleur de depart
	
	//Calcul de la consigne en angle
	Asserv.x_consigne = x_consigne_mm;
	Asserv.y_consigne = y_consigne_mm;
	
	//Attente du calcul
	Asserv.flag_calcul_atan2 = 0;
	while (!Asserv.flag_calcul_atan2);
	
	//On memorise la consigne
	consigne_angle_deg = (int)(Asserv.consigne_angle_atan2);
//	consigne_angle_deg *= Asserv.orientation_depart;	//On prend en compte notre couleur de depart
	
	
	//Changement de repère si la consigne est 0 ou 180
	if ((consigne_angle_deg > 1700) || (consigne_angle_deg < -1700) || (consigne_angle_deg == 0))
	{
		//On inverse le repère si on ne l'a pas déjà fait avec la même consigne
		if (Asserv.angle_inversion_repere != consigne_angle_deg)
			inversion_repaire(consigne_angle_deg);
		
		//on recalcul notre angle
		if (consigne_angle_deg > 1700)
		{
			consigne_angle_deg = -1800 + consigne_angle_deg;
		}
		else if (consigne_angle_deg < -1700)
		{
			consigne_angle_deg = 1800 + consigne_angle_deg;
		}
	}

	//Configuration de l'asservissement
	Asserv.vitesse_max						= vitesse;
	Asserv.consigne_angle 					= consigne_angle_deg;
	Asserv.flag_ass_angle 					= 1;
	Asserv.flag_ass_distance 				= 0; 		//Attente du modulo pour le mettre à 1 pour qu'il puisse tourner sur lui même
	Asserv.flag_ass_commande_moteur_polaire = 1;
	Asserv.flag_ass_saturation_vitesse		= 1;
	Asserv.flag_ass_saturation_acceleration	= 0;
	Asserv.offset_correcteur_angle			= ANGLE_3;	//tableau coef correcteur
	Asserv.offset_coef_saturation_vitesse	= COEF_3;
	
	//Seuil
	seuil_angle_bas		= Asserv.consigne_angle - SEUIL_ANGLE_BAS;
	seuil_angle_haut	= Asserv.consigne_angle + SEUIL_ANGLE_HAUT;
	
	//Boucle 
	while(1)
	{		
		//Condition de sortie
		if ((Asserv.theta_actuel < seuil_angle_haut) && (Asserv.theta_actuel > seuil_angle_bas))
		{
			//On incremente un compteur pour attendre la stabilisation du robot
			compteur_sortie++;
			
			//Affichage
			puts("+");
			/*
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");
			*/
			
			//Si le compteur à atteint son maxium on sort
			if (compteur_sortie >= TEMPS_ATTENTE_STAB_ANGLE)
				break;
		}
		else
		{
			//Si on est entre le seuil bas et le seuil haut on reset le compteur de sortie
			compteur_sortie = 0;
		}
	}
	
	Asserv.flag_sortie_evitement_adv = 0;
}

void vas_vers(INT_S32B x_consigne_mm, INT_S32B y_consigne_mm, INT_S32B vitesse, INT_S32B angle_force_deg)
{
	//Décalaration
	FLOAT_S32B seuil_angle_bas;
	FLOAT_S32B seuil_angle_haut;
	FLOAT_S32B angle_arrive;
	FLOAT_S32B consigne_finale;
	FLOAT_S32B seuil_distance_av;
	FLOAT_S32B seuil_distance_ar;
	INT_S32B compteur_sortie = 0;
	INT_S32B timeout_local	 = 0;
	int sortie_vas_vers		= 0;
	
	//Inversion du repere
	if (Asserv.inversion_repere)
	{
		x_consigne_mm *= -1;
		y_consigne_mm *= -1;
	}
	
	//Activation du timeout
	//timeout.deplacement = 0;
	//timeout.blocage_timeout_deplacement = 0;
	
	//Passage des commandes 1Oième de mm
	x_consigne_mm *= 10;	//X ne changera pas de sens qu'on soit JAUNE ou BLEU
	y_consigne_mm *= 10;
	y_consigne_mm *= Asserv.orientation_depart;	//Y oui en fonction de notre couleur de depart

	seuil_angle_bas		= Asserv.consigne_angle - SEUIL_ANGLE_BAS*2;
	seuil_angle_haut	= Asserv.consigne_angle + SEUIL_ANGLE_HAUT*2;
	
	while (!sortie_vas_vers)
	{
		//Configuration de l'asservissement
		Asserv.vitesse_max						= vitesse;
		Asserv.x_consigne 						= x_consigne_mm;
		Asserv.y_consigne 						= y_consigne_mm;
		
		if (angle_force_deg == PAS_DE_CORRECTION_ANGLE)
			Asserv.flag_ass_angle 				= 0;
		else
			Asserv.flag_ass_angle 				= 1;
		
		Asserv.flag_ass_distance 				= 1; 			//Attente du modulo pour le mettre à 1 pour qu'il puisse tourner sur lui même
		Asserv.flag_ass_commande_moteur_polaire = 1;
		Asserv.flag_ass_saturation_vitesse		= 1;
		Asserv.flag_ass_saturation_acceleration	= 1;
		Asserv.offset_correcteur_angle			= ANGLE_1;		//tableau coef correcteur
		Asserv.offset_coef_saturation_vitesse	= COEF_1;
		Asserv.offset_correcteur_distance		= DISTANCE_1;	//tableau coef correcteur
			
		//Seuil
		consigne_finale		= Asserv.consigne_distance ;		//Voir asserv distance
		seuil_distance_av 	= consigne_finale + SEUIL_DISTANCE_AV;
		seuil_distance_ar 	= consigne_finale - SEUIL_DISTANCE_AR;
		
		//TimeOut
		if ((vitesse > 320) && (vitesse < 400))
			timeout_local = TEMPS_ATTENTE_STAB_DIST_V350;
		else
			timeout_local = TEMPS_ATTENTE_STAB_DIST;
		
		//Boucle
		while(1)
		{
			//Si on a atteind le timeout on recadre notre angle
			/*
			if (timeout.deplacement >= TIMEOUT_VAS_VERS)
			{
				LED_JAUNE = 1;
				LED_BLEU = 1;
				
				if ((Asserv.theta_actuel < seuil_angle_haut) && (Asserv.theta_actuel > seuil_angle_bas))
				{
					//rien
				}
				else
				{	
					tourner_cible((Asserv.x_consigne/10), (Asserv.y_consigne/10), 300);
				}	

				timeout.deplacement = 0;
				timeout.blocage_timeout_deplacement = 0;

				//Couleur
				if (SYS_COULEUR_DEPART == BLEU)
				{
					LED_JAUNE 	= 0;
					LED_BLEU	= 1;
				}
				else
				{
					LED_JAUNE 	= 1;
					LED_BLEU	= 0;
				}

				break;
			}
			*/
				
			//Desactivation de l'asservissement en angle à 5cm de la cible
			if (Asserv.consigne_distance < 300)
			{
				angle_arrive = Asserv.consigne_angle_precedent;
				Asserv.consigne_angle = Asserv.consigne_angle_precedent;
			}
			else
			{
				Asserv.consigne_angle = Asserv.consigne_angle_atan2;
				Asserv.consigne_angle_precedent = Asserv.consigne_angle;
			}
					
			//Condition de sortie
			if ((Asserv.consigne_distance < seuil_distance_av) && (Asserv.consigne_distance > seuil_distance_ar))
			{
				//On incremente un compteur pour attendre la stabilisation du robot
				compteur_sortie++;
				
				//Affichage
				puts("+");
				
				//Si le compteur à atteint son maxium on sort
				if (compteur_sortie >= timeout_local)
				{
					sortie_vas_vers = 1;
					break;
				}	
			}
			else
			{
				//Si on est entre le seuil bas et le seuil haut on reset le compteur de sortie
				compteur_sortie = 0;
			}
			
			//Si le timeout est actif et qu'il est arrivé à sa limite on sort
			/*** COMMENTE LE SOIR AVANT LA COUPE
			if ((timeout.blocage_timeout_callage==0) && (timeout.callage >= TIMEOUT_CALLAGE))
			{
				sortie_vas_vers = 1;
				break;
			}
			*/
		}
	}	
	
	//Désactivation du timeout
	//timeout.blocage_timeout_deplacement = 1;
	//timeout.deplacement = 0;
	
	//On désactive l'asserv en distance
	Asserv.flag_ass_distance = 0;
	
	//Si on est pas en callage
	if ((timeout.blocage_timeout_callage) && (angle_force_deg != PAS_DE_CORRECTION_ANGLE_FIN) && (angle_force_deg != PAS_DE_CORRECTION_ANGLE))
	{
		angle_arrive *= Asserv.orientation_depart;
		
		//On corrige notre angle finale
		if (angle_force_deg == FORCE_SUR_ANGLE_ACTUEL)
			tourner_angle((long)(angle_arrive/10), vitesse);	//Division par 10 pour l'argument
		else
		{
			tourner_angle(angle_force_deg, vitesse);
		}
	}
}

void avance_callage(INT_US32B timeout_final)
{
	//Config Asserv Callage
	Asserv.vitesse_max						= 300;
	Asserv.flag_ass_angle 					= 1;
	Asserv.flag_ass_distance 				= 0;	
	Asserv.flag_ass_commande_moteur_polaire = 1;
	Asserv.flag_ass_saturation_vitesse		= 1;
	Asserv.flag_ass_saturation_acceleration	= 0;
	Asserv.flag_ass_callage					= 1;
	
	//Timeout
	timeout.callage = 0;
	timeout.blocage_timeout_callage = 0;

	while (timeout.callage < timeout_final)
	{
		Asserv.commande_correction_epis_g = 200;
		Asserv.commande_correction_epis_d = 200;
	}
	
	//Reinit commande
	Asserv.commande_correction_epis_g = 0;
	Asserv.commande_correction_epis_d = 0;
	
	timeout.callage = 0;
	timeout.blocage_timeout_callage = 1;
}

void recule_callage(INT_S32B angle, INT_S32B x_mm, INT_S32B y_mm, INT_US32B timeout_final, int mode)
{
	//Déclaration
	int detection_back = 0;
	
	//Config Asserv Callage
	Asserv.vitesse_max						= 300;
	Asserv.flag_ass_angle 					= 1;
	Asserv.flag_ass_distance 				= 0;	
	Asserv.flag_ass_commande_moteur_polaire = 1;
	Asserv.flag_ass_saturation_vitesse		= 1;
	Asserv.flag_ass_saturation_acceleration	= 0;
	Asserv.flag_ass_callage					= 1;

	//Déblocage des timeouts
	timeout.deplacement = 0;
	timeout.callage = 0;
	timeout.blocage_timeout_callage = 0;
	
	//Consigne
	Asserv.consigne_angle = angle*10;
	Asserv.consigne_angle *= Asserv.orientation_depart;
	
	//Adaptation des dimentions
	x_mm *= 10;
	y_mm *= 10;
	y_mm *= Asserv.orientation_depart;
	
	//Boucle
	while (timeout.callage < timeout_final)
	{		
		//Si on est en mode recule epis
		if (mode == RECULE_CALLAGE_EPIS)
		{
			//On desactive l'asserv en orientation
			Asserv.flag_ass_angle 	= 0;
			Asserv.flag_ass_callage	= 0;
			
			//Si le capteur GAUCHE capte un epis
			if (SYS_SICK_AR_G)
			{
				detection_back = 1;
				timeout.deplacement = 0;
				Asserv.commande_correction_epis_g = -100;
				Asserv.commande_correction_epis_d = 325;
			}
			
			//Si le capteur DROIT capte un epis
			else if (SYS_SICK_AR_D)
			{
				detection_back = 2;
				timeout.deplacement = 0;
				Asserv.commande_correction_epis_g = 325;
				Asserv.commande_correction_epis_d = -100;
			}
			
			//Si on ne capte pas d'epis on recule normalement
			else
			{
				//Si on a detecté mais qu'on ne detecte plus rien on termine
				if (((detection_back == 1) && (!SYS_SICK_AR_G)) || ((detection_back == 2) && (!SYS_SICK_AR_D)))
				{
					timeout.blocage_timeout_deplacement = 0;
				
					//On avance un peu
					if (timeout.deplacement > TIMEOUT_EVITEMENT_EPIS)
					{
						//Fin du traitement
						timeout.deplacement = 0;
						timeout.blocage_timeout_deplacement = 1;
						detection_back = 0;
					}
				}
				else
				{
					//if (SYS_COULEUR_DEPART == JAUNE)
					//{
						Asserv.commande_correction_epis_g = -220;
						Asserv.commande_correction_epis_d = -220;
					/*}	
					else
					{
						Asserv.commande_correction_epis_g = -200;
						Asserv.commande_correction_epis_d = -200;
					}*/
				}	
			}
			
				
			//Si on a nos conditions sortie
			if ((Asserv.x_actuel <= x_mm+SEUIL_RECULE) && (Asserv.x_actuel >= x_mm-SEUIL_RECULE) &&
				(Asserv.y_actuel <= y_mm+SEUIL_RECULE) && (Asserv.y_actuel >= y_mm-SEUIL_RECULE))
			{
				break;
			}	
		}
		
		//Sinon
		else if (mode == RECULE_CALLAGE_NORMALE)
		{	
			Asserv.commande_distance = -240;	//-210 au 1er match	//-240
		}
		
		else if (mode == RECULE_CALLAGE_DECA_GAUCHE)
		{	
			Asserv.commande_distance = -240;
			//Asserv.commande_correction_epis_d = -200;
			//Asserv.consigne_angle = -105;
		}
		
		else if (mode == RECULE_CALLAGE_DECA_DROITE)
		{	
			Asserv.commande_distance = -240;
			//Asserv.commande_correction_epis_g = -200;
			//Asserv.consigne_angle = 105;
		}
		
		//Sinon
		else
		{	
			Asserv.commande_distance = -240;	//-210 au 1er match	//-240
		}
	}
	
	//Coupure des commandes de recallage pour epis
	Asserv.commande_correction_epis_g			= 0;
	Asserv.commande_correction_epis_d			= 0;
	
	//On coupe la commande_distance
	Asserv.commande_distance = 0;
	Asserv.flag_ass_angle	 = 0;
	
	//Blocage du timeout callage
	timeout.blocage_timeout_callage 	= 1;
	timeout.blocage_timeout_deplacement = 1;
	timeout.callage 					= 0;
	timeout.deplacement 				= 0;
	
	//Flag d'asservissement
	Asserv.flag_ass_callage			= 0;
}

void Stop(int timeout_sec)
{
	//Déclaration
	int time = (timeout_sec*1000)/DUREE_PHASE_TRAITEMENT;
	
	//Config timeout
	timeout.deplacement = 0;
	timeout.blocage_timeout_deplacement = 0;
	
	_STOP_ROBOT
	
	//Attente
	while (timeout.deplacement <= time);
	
	_GO_ROBOT
	
	//Config timeout
	timeout.deplacement = 0;
	timeout.blocage_timeout_deplacement = 1;
}	

void StopAdv(void)
{
	//Si le flag de detection est actif
	if ((Asserv.flag_evitement_adv == 1) || (Asserv.flag_evitement_adv == 2))
	{
		//Si on detecte l'adversaire
		if (SYS_US_AV)
		{
			//LED
			LED_JAUNE = 1;
			LED_BLEU = 1;
			
			//On coupe la commande_distance
			Asserv.commande_roue_D = 0;
			Asserv.commande_roue_G = 0;
			
			timeout.blocage_timeout_evitement = 0;
			timeout.evitement = 0;
		}
		else
		{
			if ((timeout.evitement >= TIMEOUT_EVITEMENT) && (!timeout.blocage_timeout_evitement))
			{
				//Couleur
				if (SYS_COULEUR_DEPART == BLEU)
				{
					LED_JAUNE 	= 0;
					LED_BLEU	= 1;
				}
				else
				{
					LED_JAUNE 	= 1;
					LED_BLEU	= 0;
				}
				
				//On défige les timeouts
				timeout.blocage_timeout_deplacement = 0;
				timeout.blocage_timeout_callage		= 0;
				
				//Mise a jour du flag de sortie d'évitement
				Asserv.flag_sortie_evitement_adv = 1;
				
				//Arrêt du timeout
				timeout.blocage_timeout_evitement = 1;
			}
			else
			{
				if (!timeout.blocage_timeout_evitement)
				{
					timeout.evitement++;
					Asserv.commande_roue_D = 0;
					Asserv.commande_roue_G = 0;
					
					//On fige les timeouts
					//timeout.blocage_timeout_deplacement = 1;
					//timeout.blocage_timeout_callage		= 1;
				}
			}	
		}
	}	
}

void Attente(INT_US32B timeout_final)
{
	//Déclaration
	int commande = 15;
	Struct_16bit PWM_moteurGauche;
	Struct_16bit PWM_moteurDroit;
	
	//Activation du flag d'asserv
	Asserv.vitesse_max						= 300;
	Asserv.flag_ass_angle 					= 1;
	Asserv.flag_ass_distance 				= 0;	
	Asserv.flag_ass_commande_moteur_polaire = 1;
	Asserv.flag_ass_saturation_vitesse		= 1;
	Asserv.flag_ass_saturation_acceleration	= 0;
	
	//Déblocage du timeout callage
	timeout.blocage_timeout_callage = 0;
	
	//PWM
	// COMMANDE ROUE G
	SENS_MOT_G = 0;
	PWM_moteurGauche.u16b.Full = commande;
	PDC1H = PWM_moteurGauche.u16b.Octet.H;
	PDC1L = PWM_moteurGauche.u16b.Octet.L;

	// COMMANDE ROUE D
	SENS_MOT_D = 0;
	PWM_moteurDroit.u16b.Full = commande;
	PDC0H = PWM_moteurDroit.u16b.Octet.H;
	PDC0L = PWM_moteurDroit.u16b.Octet.L;
	
	//Boucle
	while (timeout.callage < timeout_final)
	{
		asm("NOP");
	}
	
	//Blocage du timeout callage
	timeout.blocage_timeout_callage = 1;
	timeout.callage = 0;
}
	
