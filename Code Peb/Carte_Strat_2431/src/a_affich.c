/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	a_affich.c
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Fonctions d'affichage de Debug sur Liaison Série
******************************************************************************/


#include 	<pic18.h>			// définitions des registres PIC

#include 	"delay.h"
#include 	"math.h"

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Tous les define pour la carte d'interface

#include 	"v_glob_cod.h"	// Variables globales externes utilisées par les fonctions

#include 	"a_affich.h"		// Prototypes des fonctions utilisées par main_cod.c
#include 	"g_fonctions.h"		// g_fonctions.h


void putch(unsigned char byte) 
{
	while(!TRMT);	// set when register is empty
	TXREG = byte;	// output one byte
}

void puts(const char *s)
{				
	while(s && *s)
	putch(*s++);	
}

/*
int i5toc(unsigned int nb)
{
	int i[5];
	int ret = 0;
	
	if (nb > 32767)
	{
		nb = 65535 - nb;
		ret = 1;
	}			
	
	//12345 = 1*10000 + 2*1000 + 3*100 + 4*10 + 5
	i[0] = nb / 10000;
	i[1] = (nb - i[0] * 10000) / 1000;
	i[2] = (nb - i[0] * 10000 - i[1] * 1000) / 100;
	i[3] = (nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100) / 10;
	i[4] = nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100 - i[3] * 10;
	
	digits[0] = (unsigned char)(i[0] + 48);
	digits[1] = (unsigned char)(i[1] + 48);
	digits[2] = (unsigned char)(i[2] + 48);
	digits[3] = (unsigned char)(i[3] + 48);
	digits[4] = (unsigned char)(i[4] + 48);	
	
	return ret;
}

int c5toi(unsigned char dig0, unsigned char dig1, unsigned char dig2, unsigned char dig3, unsigned char dig4)
{
	//12345 = 1*10000 + 2*1000 + 3*100 + 4*10 + 5
	
	return (unsigned int)((dig0-48)*10000 + (dig1-48)*1000 + (dig2-48)*100 + (dig3-48)*10 + (dig4-48));
}

void affichage_digits(int x)
{
	if (i5toc(x)==1)
	{
		while(!TRMT);TXREG = '-';
	}	
		
	while(!TRMT);TXREG = digits[0];
	while(!TRMT);TXREG = digits[1];
	while(!TRMT);TXREG = digits[2];
	while(!TRMT);TXREG = digits[3];
	while(!TRMT);TXREG = digits[4];
}

void affichage_float(float x)
{
	int x2 = (int)x;
	
	//Partie entière
	if (i5toc(x2) == 1)
	{
		while(!TRMT);TXREG = '-';
	}
		
	while(!TRMT);TXREG = digits[0];
	while(!TRMT);TXREG = digits[1];
	while(!TRMT);TXREG = digits[2];
	while(!TRMT);TXREG = digits[3];
	while(!TRMT);TXREG = digits[4];
	while(!TRMT);TXREG = ',';
	
	//Partie decimale
	x -= x2;
	x *= 1000;
	
	i5toc((int)x);
	while(!TRMT);TXREG = digits[2];
	while(!TRMT);TXREG = digits[3];
	while(!TRMT);TXREG = digits[4];
}

void affichage_nouvelle_ligne(void)
{
//	putch('\n');
//	putch('\r');
}	

void as_dbg_fin_init_var_glob_mst(void)
{
	int tempoDebug;
	
	affichage_nouvelle_ligne();
	affichage_nouvelle_ligne();
	affichage_nouvelle_ligne();
	puts("      V1.0001 10.03.10 21h10  => Carte Strategie 2431");
	affichage_nouvelle_ligne();
	
	for(tempoDebug=1000;tempoDebug>=0;tempoDebug--)
	{
		//affichage_digits((tempoDebug+1700)/1700);
		putch('\r');
	}
}
*/


/**************************************************/
/********* FONCTIONS POUR INTERRUPTIONS *********/
/*
int _i5toc(unsigned int nb)
{
	int i[5];
	int ret = 0;
	
	if (nb > 32767)
	{
		nb = 65535 - nb;
		ret = 1;
	}			
	
	//12345 = 1*10000 + 2*1000 + 3*100 + 4*10 + 5
	i[0] = nb / 10000;
	i[1] = (nb - i[0] * 10000) / 1000;
	i[2] = (nb - i[0] * 10000 - i[1] * 1000) / 100;
	i[3] = (nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100) / 10;
	i[4] = nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100 - i[3] * 10;
	
	digits[0] = (unsigned char)(i[0] + 48);
	digits[1] = (unsigned char)(i[1] + 48);
	digits[2] = (unsigned char)(i[2] + 48);
	digits[3] = (unsigned char)(i[3] + 48);
	digits[4] = (unsigned char)(i[4] + 48);	
	
	return ret;
}

void _affichage_digits(int x, int nb_digit)
{
	//Déclaration
	int i;
	
	//Nombre de digit max
	if (nb_digit > 5)
	{
		nb_digit = 5;
	}
	
	//Conversion
	if (_i5toc(x)==1)
	{
		while(!TRMT);TXREG = '-';
	}	
	
	//Affichage
	for (i=4; i>=5-nb_digit; i--)
	{
		while(!TRMT);TXREG = digits[4-i];
	}	
}

void _affichage_float(float x)
{
	int x2 = (int)x;
	
	//Partie entière
	if (_i5toc(x2) == 1)
	{
		while(!TRMT);TXREG = '-';
	}
		
	while(!TRMT);TXREG = digits[0];
	while(!TRMT);TXREG = digits[1];
	while(!TRMT);TXREG = digits[2];
	while(!TRMT);TXREG = digits[3];
	while(!TRMT);TXREG = digits[4];
	while(!TRMT);TXREG = ',';
	
	//Partie decimale
	x -= x2;
	x *= 1000;
	
	_i5toc((int)x);
	while(!TRMT);TXREG = digits[2];
	while(!TRMT);TXREG = digits[3];
	while(!TRMT);TXREG = digits[4];
}
*/

void _putch(unsigned char byte) 
{
	while(!TRMT);	// set when register is empty
	TXREG = byte;	// output one byte
}

void _puts(const char *s)
{				
	while(s && *s)
	_putch(*s++);	
}

