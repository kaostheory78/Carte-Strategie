/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	a_fonctions.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Déclarations des prototypes des fonctions du fichier a_fonctions.c
******************************************************************************/



void a_init_log (void);

void InitPositionRobot(int x_mm, int y_mm, int theta_deg, int numero_init);

void TraitementI2C(void);


