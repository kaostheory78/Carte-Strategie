/*********************************************************
 * CARTE BALISE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: interruptions.h
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 23/09/2011
 *********************************************************
 * 
 * 
 *********************************************************/



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


/**
 * @brief	: Interruption INT0 pour HTR
 **/
void __attribute__ ((interrupt, no_auto_psv)) 	_INT0Interrupt (void);


/**
 * @brief	: Interruption Esclave I2C
 **/
void __attribute__ ((interrupt,auto_psv)) 		_SI2C1Interrupt (void);


/**
 * @brief	: Interruption Timer 2
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T2Interrupt (void);


/**
 * @brief	: Interruption Timer 3
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T3Interrupt (void);


/**
 * @brief	: Interruption Timer 4
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T4Interrupt (void);


/**
 * @brief	: Interruption Timer 5
 **/
void __attribute__ ((interrupt,auto_psv)) 		_T5Interrupt (void);


/**
 * @brief	: Interruption sur overflow codeur 1
 **/
void __attribute__ ((interrupt,auto_psv)) 		_QEI1Interrupt (void);

