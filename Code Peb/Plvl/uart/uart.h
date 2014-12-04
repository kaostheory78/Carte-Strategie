/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: uart.h
 * Auteur  	: PEBh
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * Reception : U1RXREG et U2RXREG
 *********************************************************/

#ifndef _UART_H_
#define _UART_H_

/** T Y P E S ********************************************/
	#ifdef TYPE_EXT_UART
		#define TYPE_UART
	#else
		#define TYPE_UART	extern
	#endif



/** D E F I N E S ****************************************/	

	//Type UART
	#define UART_XBEE						1			// UART1
	#define UART_AX12						2			// UART2

	
	//Parametres des modules
	#define FCY_UART						40000000	// 80Mhz/2

	
	//Taille buffer
	#define UART_AX12_TAILLE_BUFFER_TX		32			// Buffer TX pour l'envoi de commande AX12
	


/** P R O T O T Y P E S **********************************/

/**
 * @brief	: Fonction permettant d'initialiser les UART
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: baud		: vitesse de communication
 */
void InitUART (int8_t channel, uint32_t baud);


/**
 * @brief	: Fonction permettant d'envoyer un caractere sur la liaison serie
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: octet		: octet a envoyer
 */
void PutcUART (int8_t channel, uint8_t octet);


/**
 * @brief	: Fonction permettant d'envoyer une chaine de caractere sur la liaison serie
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: chaine	: chaine a envoyer
 */
void PutsUART (int8_t channel, const char *chaine);


/**
 * @brief	: Fonction permettant d'envoyer une chaine de caractere ou une partie sur la liaison serie (utile pour les AX12 par exemple)
 * @param	: channel		: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: chaine		: chaine a envoyer
 * @param	: taille_chaine	: taille de la chaine ou nombre d'octet a envoyer
 */
void Puts2UART (int8_t channel, uint8_t *chaine, uint16_t taille_chaine);


/**
 * @brief	: Fonction permettant de convertir un int en chaine de caractere
 * @param	: nb	: nombre a convertir
 * @return	: une chaine contenant le nombre
 */
char *IntToC (int16_t nb);


/**
 * @brief	: Fonction permettant de convertir un long en chaine de caractere
 * @param	: nb	: nombre a convertir
 * @return	: une chaine contenant le nombre
 */
char *LongToC (int32_t nb);


/**
 * @brief	: Fonction permettant d'afficher un int sur la console
 * @param	: nb	: nombre a afficher
 */
void PutIntUART (int16_t nb);


/**
 * @brief	: Fonction permettant d'afficher un long sur la console
 * @param	: nb	: nombre a afficher
 */
void PutLongUART (int32_t nb);

#endif /* _UART_H_ */
