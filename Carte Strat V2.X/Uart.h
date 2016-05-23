 /* 
 * File:   Uart.h
 * Author: Quentin
 *
 * Created on 8 novembre 2014, 16:22
 */

#ifndef UART_H
#define	UART_H


/** D E F I N E S ****************************************/



//Type UART
#define UART_XBEE						1			// UART1
#define UART_RS232                                              1
#define UART_AX12						3 			// UART2

//Taille buffer
#define UART_DEBUG_TAILLE_BUFFER_TX                             32			// Buffer TX pour l'envoi de commande AX12


//Taille buffer
#define UART_AX12_TAILLE_BUFFER_TX                              32			// Buffer TX pour l'envoi de commande AX12

/** P R O T O T Y P E S **********************************/

/**
 * @brief	: Fonction permettant d'initialiser les UART
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: baud		: vitesse de communication
 */
void InitUART (int8_t channel, uint32_t baud);


/**
 * @brief       : Fonction qui permet de modifier la vitesse de communication de l'UART
 * @param       : uart          : Uart dont on veut changer la vitesse
 * @param       : baud          : Vitesse à atteindre
 */
void modifier_vitesse_com_uart (int8_t uart, uint32_t baud);

/**
 * @ brief      : Fonction qui permet de calculer la veleur à mettre dans le registre
 *                pour changer le baud rate de l'uart
 *
 * @param       : baud          : valeur souhaitée
 * @return      : valeur à mettre dans le registre
 */
uint16_t calcul_baud (uint32_t baud);

/**
 * @brief       : Vide le buffer de reception de l'uart pour supprimer les données reçues non récupérés
 * @param       : uart          : Uart dont on veut vider le buffer !
 */
void vider_buffer_reception_uart (uint8_t uart);

/**
 * @brief	: Fonction permettant d'envoyer un caractere sur la liaison serie
 * @param	: channel	: channel a initialiser (UART_XBEE | UART_AX12)
 * @param	: octet		: octet a envoyer
 */
void PutcUART (int8_t channel, uint8_t octet);


#endif	/* UART_H */

