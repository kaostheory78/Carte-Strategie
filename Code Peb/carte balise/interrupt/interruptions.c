/*********************************************************
 * CARTE BALISE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: interruptions.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 23/09/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_INTERRUPTIONS


	
/** I N C L U D E S **************************************/
	#include "../usr.h"



/** F O N C T I O N S ************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.U1RXIE	= 0;
	IFS0bits.U1RXIF = 0;
	
	// Lecture de l'octet reçu
	uint8_t buff = U1RXREG;
	
	uint8_t buffer[2];
	buffer[0] = buff;
	
//	if (buff == 'S')
	Puts2UART (UART_DEBUG, (char*)buffer, 1);
	
	/*
#if CARTE_DEFINIE == CARTE_BOARDM
	_XbeeTraitementRxItCentrale (buff);
#else
	_XbeeTraitementRxItSatelite (buff);
#endif
*/

	
	// Activation de l'interruption
	IEC0bits.U1RXIE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC1bits.U2RXIE	= 0;
	IFS1bits.U2RXIF = 0;
	
	// Lecture de l'octet reçu
	char buff = U2RXREG;
	
	// Traitement de l'octet par Nucleus
	_NucleusSaisieCommande (buff);

	// Activation de l'interruption
	IEC1bits.U2RXIE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_INT0Interrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.INT0IE	= 0;
	IFS0bits.INT0IF = 0;
	
	// Traitement
	//
	//	HTR
	//
	
	// Activation de l'interruption
	IEC0bits.INT0IE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_SI2C1Interrupt (void)
{
	unsigned char buf = I2C1RCV;
	
	// Desactivation de l'interruption	
	IEC1bits.SI2C1IE	= 0;
	IFS1bits.SI2C1IF	= 0;
	
	if ((I2C1STATbits.P) || (I2C1CONbits.SCLREL))
	{
		i2c.i2c_offset_tx = 0;
	}
	
	// Trame d'adresse
	if (!I2C1STATbits.D_A)
	{
		AckI2C ();
		
		i2c.i2c_offset_rx = 0;
		i2c.i2c_offset_tx = 0;
	}
	
	// Trame de données
	else
	{
		// Recuperation de donnees depuis le maitre
		if (!I2C1STATbits.R_W)
		{
			AckI2C ();
			
			i2c.buffer_rx[i2c.i2c_offset_rx] = buf;
			i2c.i2c_offset_rx++;
		}
	}
	
	
	// Envoi de données vers le maitre
	_PutcSlaveI2C ();
	
	
	// Activation de l'interruption
	IEC1bits.SI2C1IE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_T2Interrupt (void)
{
	// Reset du Flag d'interruption
	IFS0bits.T2IF	= 0;
	
	// Asservissement du moteur
	_AsservissementCourantMoteur ();
	
	// Envoi de la commande moteur
	MoteurEnvoiCommande (Balise.commande_moteur);
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_T3Interrupt (void)
{
	// Reset du Flag d'interruption
	IFS0bits.T3IF	= 0;
	
	// Gestion des compteurs de temps
	_GestionTemps ();
	
	// Calcul coordonnees
	#if CARTE_DEFINIE != CARTE_BOARDM
		//
		//
		Nop ();
	#endif
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv))	_T4Interrupt (void)
{
	// Desactivation du Timer4
	T4CONbits.TON	= 0;
	IFS1bits.T4IF 	= 0;

	//Traitement
	_LedExecSequence ();

	// Reactivation du Timer4
	T4CONbits.TON	= 1;
	IEC1bits.T4IE 	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_T5Interrupt (void)
{
	// Reset du Flag d'interruption
	IFS1bits.T5IF	= 0;
	
	// Envoi coordonnees Xbee
	if (Xbee.sync == 1)
	{
		_XbeeEnvoiCoord ();
	}
	
	Nop ();	// VITALE
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_QEI1Interrupt (void)
{
	// Reset du flag
	IFS3bits.QEI1IF		= 0;
	
	// Sens de rotation positif
	if (QEI1CONbits.UPDN == 1)
		Capteurs.overflow = 1;
	
	// Sens de rotation negatif
	else
		Capteurs.overflow = 0;
}



