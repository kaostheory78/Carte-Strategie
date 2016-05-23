/*******************************************************************************
 *                 CARTE PRINCIPAL ROBOT - DSPIC33FJ128MC804
 *******************************************************************************
 * Fichier 	: Uart.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08/11/2014
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/******************************* CONFIG UART **********************************/
/******************************************************************************/

void InitUART (int8_t channel, uint32_t baud)
{
	// UART1 : DEUBG
    // La fonction printf est directement mappé sur l'UART 1
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
            U2MODEbits.BRGH     = 1;		// Vitesse élevée
            U2MODEbits.URXINV 	= 0;		// Polarite non inversee
            U2MODEbits.ABAUD 	= 0;		// Baud non automatique
            U2MODEbits.LPBACK 	= 0;		// Mode Loopback desactive
            U2MODEbits.WAKE 	= 0;		// Wake-up desactive
            U2MODEbits.UEN 	= 0;		// TX et RX actif, CTS et RTS non
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
            return (uint16_t) ((FCY/baud/4) - 1);
            break;
    }
}

/******************************************************************************/
/*************************** GESTION DE L'UART ********************************/
/******************************************************************************/

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

void vider_buffer_reception_uart (uint8_t uart)
{
    uint8_t buf;
    if (uart == UART_AX12)
    {
        while(U2STAbits.URXDA == 1)
        {
            buf = U2RXREG;
        }
    }
    else
    {
        while(U1STAbits.URXDA == 1)
        {
            buf = U1RXREG;
        }
    }
}


/******************************************************************************/
/************************* FONCTIONS DE COM UART ******************************/
/******************************************************************************/

void PutcUART (int8_t channel, uint8_t octet)
{
	// UART1 : XBEE
    // Préférer la fonction  printf pour l'uart XBEE
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

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/