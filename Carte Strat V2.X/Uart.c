/*********************************************************
 * CARTE Pirincipal robot - DSPIC33FJ128MC804
 *********************************************************
 * Fichier 	: Uart.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08/11/2014
 *********************************************************
 *
 *
 *********************************************************/
#include "system.h"

void InitUART (int8_t channel, uint32_t baud)
{
	// UART1 : DEUBG
	if (channel == UART_XBEE)
	{
            // U1BRG
            U1BRG = calcul_baud (baud);

            // U1STA
            U1MODEbits.UARTEN 	= 0;		// Desctivation de l'uart1
            U1STAbits.ADDEN 	= 0;		// Address detect mode desactive
            U1STAbits.URXISEL 	= 0;		// Interruption sur RX d'un caractere
            U1STAbits.UTXBRK 	= 0;		// Sync Break desactivee

          //  #ifdef UTILISATION_MODULE_XBEE
          //          U1STAbits.UTXINV 	= 1;		// Polarite inversee
          //  #else
                    U1STAbits.UTXINV 	= 0;		// Polarite non inversee
          //  #endif

            U1STAbits.UTXISEL0 	= 0;		// TX interruption sur transmission d'un caractere
            U1STAbits.UTXISEL1 	= 0;		// TX interruption sur transmission d'un caractere
            U1STAbits.URXISEL0 	= 0;		// RX interruption sur reception d'un caractere
            U1STAbits.URXISEL1 	= 0;		// RX interruption sur reception d'un caractere

            // U1MODE
            U1MODEbits.STSEL	= 0;		// 1 bit d'arret
            U1MODEbits.PDSEL	= 0;		// 8 bit de donnee, pas de parite
            U1MODEbits.BRGH     = 1;		// Vitesse élevée

            //#ifdef UTILISATION_MODULE_XBEE
    //                U1MODEbits.URXINV 	= 1;		// Polarite non inversee
            //#else
                    U1MODEbits.URXINV 	= 0;		// Polarite non inversee
            //#endif

            U1MODEbits.ABAUD 	= 0;		// Baud non automatique
            U1MODEbits.LPBACK 	= 0;		// Mode Loopback desactive
            U1MODEbits.WAKE 	= 0;		// Wake-up desactive

//		#ifdef UTILISATION_MODULE_XBEE
	//		U1MODEbits.UEN 		= 0b10;		// TX et RX actif, CTS et RTS utilsee
//		#else
                    U1MODEbits.UEN 		= 0;		// TX et RX actif, CTS et RTS non
//		#endif

            U1MODEbits.RTSMD 	= 0;		// Non utilise
            U1MODEbits.IREN 	= 0;		// IR non utilise
            U1MODEbits.USIDL 	= 0;		// Mode continu en IDLE

            U1MODEbits.UARTEN 	= 1;		// Activation de l'uart1
            U1STAbits.UTXEN 	= 1;		// TX actif

            //PORTAbits.RA7 = 1;
	}

	// UART2 : AX12
	else if (channel == UART_AX12)
	{
            // U2BRG
            U2BRG = calcul_baud(baud);		// 115200 a 80MHz

            // U2STA
            U2MODEbits.UARTEN 	= 0;		// Desctivation de l'uart2
            U2STAbits.ADDEN 	= 0;		// Address detect mode desactive
            U2STAbits.URXISEL 	= 0;		// Interruption sur RX d'un caractère
            U2STAbits.UTXBRK 	= 0;		// Sync Break desactivee

            U2STAbits.UTXINV 	= 0;		// Polarite non inversee
            U2STAbits.UTXISEL0 	= 1;		// TX interruption sur fin de transmission d'une trame (buffer vide)
            U2STAbits.UTXISEL1 	= 0;		// TX interruption sur fin de transmission d'une trame (buffer vide)
            U2STAbits.URXISEL0 	= 0;		// RX interruption sur reception d'un caractere
            U2STAbits.URXISEL1 	= 0;		// RX interruption sur reception d'un caractere

            // U2MODE
            U2MODEbits.STSEL	= 0;		// 1 bit d'arret
            U2MODEbits.PDSEL	= 0;		// 8 bit de donnee, pas de parite
            U2MODEbits.BRGH		= 1;		// Vitesse élevée
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

            INHIBIT_AX12 = ALLUME;
	}
}

uint16_t calcul_baud (uint32_t baud)
{
    switch (baud)
    {
        case 1000000 :
            return 9;
            break;
        case 500000 :
            return 19;
            break;
        case 400000 :
            return 24;
            break;
        case 250000 :
            return 39;
            break;
        case 200000 :
            return 49;
            break;
        case 115200 :
            return 86;
            break;
        case 57600 :
            return 173;
            break;
        case 19200 :
            return 520;
            break;
        case 9600 :
            return 1041;
            break;
        default :
            return ((FCY/baud/4) - 1);
            break;
    }
}

void modifier_vitesse_com_uart (int8_t uart, uint32_t baud)
{
    if (uart == UART_XBEE)
    {
         U1MODEbits.UARTEN 	= 0;
         // on vide luart
         U1BRG = calcul_baud (baud);
         U1MODEbits.UARTEN 	= 1; 
    }
    else
    {
         U2MODEbits.UARTEN 	= 0;
         // on vide luart
         U2BRG = calcul_baud (baud);
         U2MODEbits.UARTEN 	= 1;
    }
}


/*********************************************************/
void PutcUART (int8_t channel, uint8_t octet)
{
	// UART1 : XBEE
	if (channel == UART_XBEE )
	{
                while (U1STAbits.UTXBF); U1TXREG = octet;
	}

	// UART2 : AX12
	else if (channel == UART_AX12)
	{
                ax12.etat_uart = EN_COURS_ENVOIT;
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
void Puts2UART (int8_t channel, uint8_t *chaine, uint16_t taille_chaine)
{
	int i;

	for (i=0; i<taille_chaine; i++)
	{
		if (channel == UART_XBEE)
		{
			//while (U1STAbits.UTXBF); U1TXREG = chaine[i];

			PutcUART (UART_XBEE, chaine[i]);
		}

		// UART2 : AX12
		else if (channel == UART_AX12)
		{
			//while (U2STAbits.UTXBF); U2TXREG = chaine[i];

			PutcUART (UART_AX12, chaine[i]);
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
	PutsUART (UART_XBEE, IntToC (nb));
}


/*********************************************************/
void PutLongUART (int32_t nb)
{
	PutsUART (UART_XBEE, LongToC (nb));
}