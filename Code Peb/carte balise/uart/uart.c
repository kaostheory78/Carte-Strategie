/*********************************************************
 * CARTE BALISE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: uart.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 23/09/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_UART


	
/** I N C L U D E S **************************************/
	#include "../usr.h"


	
/** F O N C T I O N S ************************************/
void InitUART (int8_t channel, uint32_t baud)
{
	// MODIFIER UEN POUR CTS ET RTS !!!
	
	// UART1 : DEUBG
	if (channel == UART_XBEE)
	{
		// U1BRG
		U1BRG	= ((FCY_UART/baud)/16) - 1;		// 115200 a  80MHz
		
		// U1STA
		U1MODEbits.UARTEN 	= 0;		// Desctivation de l'uart2
		U1STAbits.ADDEN 	= 0;		// Address detect mode desactive
		U1STAbits.URXISEL 	= 0;		// Interruption sur RX d'un caractere
		U1STAbits.UTXBRK 	= 0;		// Sync Break desactivee
		
		#ifdef UTILISATION_MODULE_XBEE
			U1STAbits.UTXINV 	= 1;		// Polarite inversee
		#else
			U1STAbits.UTXINV 	= 0;		// Polarite non inversee
		#endif
		
		U1STAbits.UTXISEL0 	= 0;		// TX interruption sur transmission d'un caractere
		U1STAbits.UTXISEL1 	= 0;		// TX interruption sur transmission d'un caractere
		U1STAbits.URXISEL0 	= 0;		// RX interruption sur reception d'un caractere
		U1STAbits.URXISEL1 	= 0;		// RX interruption sur reception d'un caractere
		
		// U1MODE
		U1MODEbits.STSEL	= 0;		// 1 bit d'arret
		U1MODEbits.PDSEL	= 0;		// 8 bit de donnee, pas de parite
		U1MODEbits.BRGH		= 0;		// Vitesse faible
		
		#ifdef UTILISATION_MODULE_XBEE
			U1MODEbits.URXINV 	= 1;		// Polarite non inversee
		#else
			U1MODEbits.URXINV 	= 0;		// Polarite non inversee
		#endif
		
		U1MODEbits.ABAUD 	= 0;		// Baud non automatique
		U1MODEbits.LPBACK 	= 0;		// Mode Loopback desactive
		U1MODEbits.WAKE 	= 0;		// Wake-up desactive
		
//		#ifdef UTILISATION_MODULE_XBEE
//			U1MODEbits.UEN 		= 0b10;		// TX et RX actif, CTS et RTS utilsee
//		#else
			U1MODEbits.UEN 		= 0;		// TX et RX actif, CTS et RTS non
//		#endif
		
		U1MODEbits.RTSMD 	= 0;		// Non utilise
		U1MODEbits.IREN 	= 0;		// IR non utilise
		U1MODEbits.USIDL 	= 0;		// Mode continu en IDLE
		
		U1MODEbits.UARTEN 	= 1;		// Activation de l'uart1
		U1STAbits.UTXEN 	= 1;		// TX actif
	}
	
	// UART2 : AX12
	else if (channel == UART_DEBUG)
	{
		// U2BRG
		U2BRG	= ((FCY_UART/baud)/16) - 1;		// 115200 a 80MHz
		
		// U2STA
		U2MODEbits.UARTEN 	= 0;		// Desctivation de l'uart2
		U2STAbits.ADDEN 	= 0;		// Address detect mode desactive
		U2STAbits.URXISEL 	= 0;		// Interruption sur RX d'un caractere
		U2STAbits.UTXBRK 	= 0;		// Sync Break desactivee
		
		U2STAbits.UTXINV 	= 0;		// Polarite non inversee
		U2STAbits.UTXISEL0 	= 0;		// TX interruption sur transmission d'un caractere
		U2STAbits.UTXISEL1 	= 0;		// TX interruption sur transmission d'un caractere
		U2STAbits.URXISEL0 	= 0;		// RX interruption sur reception d'un caractere
		U2STAbits.URXISEL1 	= 0;		// RX interruption sur reception d'un caractere
		
		// U2MODE
		U2MODEbits.STSEL	= 0;		// 1 bit d'arret
		U2MODEbits.PDSEL	= 0;		// 8 bit de donnee, pas de parite
		U2MODEbits.BRGH		= 0;		// Vitesse faible
		U2MODEbits.URXINV 	= 0;		// Polarite non inversee
		U2MODEbits.ABAUD 	= 0;		// Baud non automatique
		U2MODEbits.LPBACK 	= 0;		// Mode Loopback desactive
		U2MODEbits.WAKE 	= 0;		// Wake-up desactive
		U2MODEbits.UEN 		= 0;		// TX et RX actif, CTS et RTS non
		U2MODEbits.RTSMD 	= 0;		// Non utilise
		U2MODEbits.IREN 	= 0;		// IR non utilise
		U2MODEbits.USIDL 	= 0;		// Mode continu en IDLE
		
		U2MODEbits.UARTEN 	= 1;		// Activation de l'uart2
		U2STAbits.UTXEN 	= 1;		// TX actif
		
		Nucleus.flag_dominant	= 0;
	}
}


/*********************************************************/
void PutcUART (int8_t channel, uint8_t octet)
{	
	// UART1 : XBEE
	if (channel == UART_XBEE)
	{
		while (U1STAbits.UTXBF); U1TXREG = octet;
	}
	
	// UART2 : DEBUG
	else if (channel == UART_DEBUG)
	{
		if (Nucleus.flag_dominant)
			return;
		
		while (U2STAbits.UTXBF); U2TXREG = octet;
	}
}


/*********************************************************/
void PutsUART (int8_t channel, const char *chaine)
{
	do
	{
		PutcUART (channel, *chaine);
	}
	while (*chaine++);
}


/*********************************************************/
void Puts2UART (int8_t channel, const char *chaine, uint16_t taille_chaine)
{
	int i;
	
	for (i=0; i<taille_chaine; i++)
	{
		if (channel == UART_XBEE)
		{
			PutcUART (UART_XBEE, chaine[i]);
		}
		
		else if (channel == UART_DEBUG)
		{
			PutcUART (UART_DEBUG, chaine[i]);
		}
	}
}


/*********************************************************/
char *IntToC (int16_t nb)
{
	// Delarations locales
	int16_t i[5];
	int j = 0;
	static char out[8];

	// Nombre negatif
	if (nb < 0)
	{
		nb *= (-1);
		out[j++] = '-';
	}

	// Conversion
	//12345 = 1*10000 + 2*1000 + 3*100 + 4*10 + 5
	i[0] = (nb / 10000);
	i[1] = (nb - i[0] * 10000) / 1000;
	i[2] = (nb - i[0] * 10000 - i[1] * 1000) / 100;
	i[3] = (nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100) / 10;
	i[4] = (nb - i[0] * 10000 - i[1] * 1000 - i[2] * 100 - i[3] * 10);

	out[j++] = (unsigned char)(i[0] + 48);
	out[j++] = (unsigned char)(i[1] + 48);
	out[j++] = (unsigned char)(i[2] + 48);
	out[j++] = (unsigned char)(i[3] + 48);
	out[j++] = (unsigned char)(i[4] + 48);
	out[j++] = '\0';

	return out;
}


/*********************************************************/
char *LongToC (int32_t nb)
{
	//4294967296
	//4.294.967.296
	
	// Declarations locales
	int32_t i[10];
	int j = 0;
	static char out[15];
	
	
	// Nombre negatif
	if (nb < 0)
	{
		nb *= (-1);
		out[j++] = '-';
	}

	// Conversion
	//4.294.967.296 = 4*1000000000 + 2*100000000 + 9*10000000 + 4*1000000 + 9*100000 + 6*10000 + 7*1000 + 2*100 + 9*10 + 6
	
	i[0] = (nb / 1000000000);
	i[1] = (nb - i[0] * 1000000000) / 100000000;
	i[2] = (nb - i[0] * 1000000000 - i[1] * 100000000) / 10000000;
	i[3] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000) / 1000000;
	i[4] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000) / 100000;
	i[5] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000) / 10000;
	i[6] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000) / 1000;
	i[7] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000 - i [6] * 1000) / 100;
	i[8] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000 - i [6] * 1000 - i[7] * 100) / 10;
	i[9] = (nb - i[0] * 1000000000 - i[1] * 100000000 - i[2] * 10000000 - i[3] * 1000000 - i[4] * 100000 - i[5] * 10000 - i [6] * 1000 - i[7] * 100 - i[8] * 10);
	
	
	out[j++] = (unsigned char)(i[0] + 48);
	out[j++] = (unsigned char)(i[1] + 48);
	out[j++] = (unsigned char)(i[2] + 48);
	out[j++] = (unsigned char)(i[3] + 48);
	out[j++] = (unsigned char)(i[4] + 48);
	out[j++] = (unsigned char)(i[5] + 48);
	out[j++] = (unsigned char)(i[6] + 48);
	out[j++] = (unsigned char)(i[7] + 48);
	out[j++] = (unsigned char)(i[8] + 48);
	out[j++] = (unsigned char)(i[9] + 48);
	out[j++] = '\0';
	
	return out;
}


/*********************************************************/
void PutIntUART (int16_t nb)
{
	PutsUART (UART_DEBUG, IntToC (nb));
}


/*********************************************************/
void PutLongUART (int32_t nb)
{
	PutsUART (UART_DEBUG, LongToC (nb));
}


