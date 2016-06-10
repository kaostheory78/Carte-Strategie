/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	a_fonctions.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Beno�t LINCK / David SILVA PIRES
* Date Cr�ation 	:	2007
* Date R�vision 	:	11/07/2008
******************************************************************************
* Description	:	- D�clarations des prototypes des fonctions du fichier a_fonctions.c
******************************************************************************/



void a_init_log (void);

void InitPositionRobot(int x_mm, int y_mm, int theta_deg, int numero_init);

void TraitementI2C(void);


