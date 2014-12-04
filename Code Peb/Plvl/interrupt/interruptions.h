/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: interruptions.h
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * 
 *********************************************************/

#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

/** T Y P E S ********************************************/
	#ifdef TYPE_EXT_INTERRUPTIONS
		#define TYPE_INTERRUPTIONS
	#else
		#define TYPE_INTERRUPTIONS	extern
	#endif



/** D E F I N E S ****************************************/


//TMP
TYPE_INTERRUPTIONS char buffer[30];
TYPE_INTERRUPTIONS int offset;





/** P R O T O T Y P E S **********************************/

/**
 * @brief	: Interruption UART1 sur RX
 **/
void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void);


/**
 * @brief	: Interruption UART2 sur RX
 **/
void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void);


void __attribute__ ((interrupt, no_auto_psv)) 	_U2TXInterrupt (void);


/**
 * @brief	: Interruption INT0 pour HTR
 **/
void __attribute__ ((interrupt, no_auto_psv)) 	_INT0Interrupt (void);


/**
 * @brief	: Interruption Maitre I2C
 **/
void __attribute__ ((interrupt,auto_psv)) 		_MI2C1Interrupt (void);


/**
 * @brief	: Interruption Esclave I2C
 **/
void __attribute__ ((interrupt,auto_psv)) 		_SI2C1Interrupt (void);


/**
 * @brief	: Interruption Timer 2 pour la gestion de l'asservissement / adversaire / temps
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T2Interrupt (void);


/**
 * @brief	: Interruption Timer 3 pour la gestion de l'automatisme
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T3Interrupt (void);

	
/**
 * @brief	: Interruption Timer 4 pour la gestion des LEDs
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T4Interrupt (void);


/**
 * @brief	: Interruption sur overflow codeur 1
 **/
void __attribute__ ((interrupt,auto_psv)) 		_QEI1Interrupt (void);


/**
 * @brief	: Interruption sur overflow codeur 1
 **/
void __attribute__ ((interrupt,auto_psv)) 		_QEI2Interrupt (void);

#endif /* _INTERRUPTIONS_H_ */
