/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	g_config_mat.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Beno�t LINCK / David SILVA PIRES
* Date Cr�ation 	:	2007
* Date R�vision 	:	11/07/2008
******************************************************************************
* Description	:	- Directives de compilation, suivant la configuration mat�rielle choisie.
******************************************************************************/


#define	OUI		1
#define NON		0


// MODULE POWER PWM
#define USE_POWER_PWM				NON
#define USE_POWER_PWM_POSTSCALER	NON
#define USE_POWER_PWM_PRESCALER		NON
#define USE_POWER_PWM_DEAD_TIMES	NON
#define USE_POWER_PWM_FAULT			NON

