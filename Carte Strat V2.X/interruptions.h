/* 
 * File:   interruptions.h
 * Author: Quentin
 *
 * Created on 9 novembre 2014, 00:53
 */

#ifndef INTERRUPTIONS_H
#define	INTERRUPTIONS_H

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

/**
 *  Interruption du Timer 1 : Asserv
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void);



/**
 * Timer fin de match : 90 secondes
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void);



/**
 * Timer 10 ms : Autom et capteurs
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void);


/**
 * Interruption sur codeur droit
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI1Interrupt(void);


/**
 * Interruption sur codeur gauche
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI2Interrupt(void);


/**
 * Interruption UART_XBEE (ou RS232) sur RX
 */
void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void);


/**
 * Interruption UART AX12 sur RX
 */
void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void);

#endif	/* INTERRUPTIONS_H */

