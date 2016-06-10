/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 		:	reglages_carte.c
* Operating System	:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur		:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE				:	Notepad++
******************************************************************************
* Auteurs 			:	PEB
* Date Création 	:	2008
* Date Révision 	:	11/07/2008
******************************************************************************
* Description		:	- Fonctions permettant le reglage de la carte interface
*						par la dspic grave à l'I2C
******************************************************************************/


#include 	<pic18.h>			// définitions des registres PIC

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Tous les define pour la carte interface

#include 	"v_glob_cod.h"	// Variables globales utilisées par les fonctions

#include 	"reglages_carte.h"	// Prototypes des fonctions utilisées par reglages_carte.c
#include 	"a_affich.h"      	// Debug

#include	"asserv.h"


void GestionToucheUART1(void)
{
	/**** TOUCHES *****
	
	- a 	 : control clavier on/off
	- espace : commande à 0
	- z/s 	 : Avancer/Reculer
	- q/d 	 : tourner
	
	
	*******************/
	
	unsigned char touche;
	
	touche = RCREG;		// touche recuperée par la liaison serie
	
	//while(!TRMT);TXREG = touche;
	/*
	if (touche == '²')
	{
		if (Asserv.flag_affichage_debug == AFF_DEBUG_AIDE)
			Asserv.flag_affichage_debug = AFF_DEBUG_ROBOT;
		else
			Asserv.flag_affichage_debug = AFF_DEBUG_AIDE;
	}
	if (touche == '\t')
	{
		if (Asserv.flag_affichage_debug != AFF_DEBUG_STANDBY)
			Asserv.flag_affichage_debug = AFF_DEBUG_STANDBY;
		else
			Asserv.flag_affichage_debug = AFF_DEBUG_ROBOT;		
	}	
	if (touche == '.')
	{
		//
	}	
	if (touche == ' ')
	{
		Asserv.commande_roue_G_clavier = 0;
		Asserv.commande_roue_D_clavier = 0;
	}	
	if (touche == 'a')
	{
		Asserv.flag_control_clavier = !Asserv.flag_control_clavier;
	}
	if (touche == 'b')
	{
		//
	}
	if (touche == 'c')
	{
		//
	}
	if (touche == 'd')
	{
		Asserv.commande_roue_D_clavier+=PAS_COMMANDE_MOTEURS;
	}
	if (touche == 'e')
	{
		//
	}	
	if (touche == 'f')
	{
		//
	}
	if (touche == 'g')
	{
		//
	}
	if (touche == 'h')
	{
		//
	}
	if (touche == 'i')
	{	
		//
	}	
	if (touche == 'j')
	{
		//
	}			
	if (touche == 'k')
	{
		//
	}	
	if (touche == 'l')
	{
		//
	}
	if (touche == 'm')
	{
		//
	}
	if (touche == 'n')
	{
		//
	}
	if (touche == 'o') 
	{
		//
	}
	if (touche == 'p') 
	{
		//	
	}
	if (touche == 'q')
	{
		Asserv.commande_roue_G_clavier+=PAS_COMMANDE_MOTEURS;
	}
	if (touche == 'r')
	{
		while(!TRMT);TXREG = 'R';
		while(!TRMT);TXREG = '\r';
		RESET();
	}
	if (touche == 's')
	{
		if (Asserv.flag_control_clavier)	//-
		{
			Asserv.commande_roue_G_clavier-=PAS_COMMANDE_MOTEURS;
			Asserv.commande_roue_D_clavier-=PAS_COMMANDE_MOTEURS;
		}	
	}
	if (touche == 't')
	{
		//
	}
	if (touche == 'u')
	{
		//
	}
	if (touche == 'v') 
	{
		//
	}
	if (touche == 'w')
	{
		//
	}
	if (touche == 'x')
	{
		//
	}
	if (touche == 'y')
	{
		//
	}	
	if (touche == 'z')
	{
		if (Asserv.flag_control_clavier)	//+
		{
			Asserv.commande_roue_G_clavier+=PAS_COMMANDE_MOTEURS;
			Asserv.commande_roue_D_clavier+=PAS_COMMANDE_MOTEURS;
		}	
	}
	*/
}

