/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	compil_cod.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Beno�t LINCK / David SILVA PIRES  
* Date Cr�ation 	:	2007
* Date R�vision 	:	11/07/2008
******************************************************************************
* Description	:	- Directives de compilation permettant de g�n�rer un ex�cutables 
*				avec les fonctions strictement n�cessaires
******************************************************************************/



#define DEBUG_ERR_HTR	0		// Permet d'afficher les infos de debug HTR en cas de d�passement de tps de traitement
//#define DEBUG_NO_HTR	0		// Permet d'afficher le num�ro de phase HTR en cours de traitement
//#define DEBUG_TPS_HTR	0		// Permet d'afficher le temps restant apr�s chaque phase HTR si tps restant suffisant, sinon affiche "R" sans valeur

//#define PSP		0				// 1 = Port parall�le utilis� dans ce PIC
//#define I2C		1				// 1 = I2C utilis� dans ce PIC
//#define I2C_TYPE	1				// 1 = I2C esclave, 0 = I2C Ma�tre

//#define DEBUG_I2C	0

//#define DEBUG_PEB	1			//Debug de PEB

#define DEBUG 	1
#define SERIE 	1				//renvoie les infos sur la laison serie
#define LCD 	0				//renvoie les infos sur le LCD

