/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	main_cod.c
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	PEB
* Date Création 	:	2010
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- PROGRAMME PRINCIPAL PIC2431 carte interface ROBOT
******************************************************************************/

/** T Y P E S *********************************************************/
	#define TYPE_MAIN_EXT
	
#include 	<pic18.h>
//#include	<pic18f2x1x.h>

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Definition des constantes generales

#include 	"v_glob_cod.h"		// Variables globales utilisées par le main_cod

#include 	"a_affich.h"		// a_affich
#include 	"a_fonctions.h"		// a_fonctions
#include 	"g_fonctions.h"		// g_fonctions
#include 	"reglages_carte.h"	// Prototypes des fonctions utilisées par reglages_carte.c
#include 	"asserv.h"
#include	"strategies.h"


/**************************************************************************************/
// PROGRAMME PRINCIPAL PIC2431 carte interface
/**************************************************************************************/
// UTILISATION DES PERIPHERIQUES TIMERS
// 	Timer 0 : Gestion de 20 Phases HTR avec 2,5ms soit 20*2,5ms = 50ms,
// 	Timer 1 : Commande Servomoteurs (2 servos gerés)
// 	Timer 2 : Non utiliser
// 	Timer 5 : Utilisé par module QEI 
	

void main(void)
{	
	// Configuration des REGISTRES MATERIEL	
	g_config_mat();			// (fichier g_fonctions.c et g_config_mat.h)
	
	// Initialisation des variables RAM globales du logiciel 	
	a_init_log();			// (fichier a_fonctions.c et a_init_log.h)
	
	//as_dbg_fin_init_var_glob_mst();
	
	//traitement principale
	TraitementPrincipal();
	
	//Fin
	while(1);
}

