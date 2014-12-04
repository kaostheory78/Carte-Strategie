/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: interruptions.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_INTERRUPTIONS


	
/** I N C L U D E S **************************************/
	#include "usr.h"


extern int32_t overflow[2];


/** F O N C T I O N S ************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.U1RXIE	= 0;
	IFS0bits.U1RXIF = 0;
	
	// Traitement
	char buff = U1RXREG;
	_NucleusSaisieCommande (buff);
	
	// Activation de l'interruption
	IEC0bits.U1RXIE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void)
{
	int erreur = 0;
	
	// Desactivation de l'interruption
	IEC1bits.U2RXIE	= 0;
	IFS1bits.U2RXIF = 0;
	
	uint8_t buf = U2RXREG;
	
	// On attend des données
	if (ax12.nb_octet_attente > 0)
	{
		ax12.buffer[ax12.offset] = buf;
		ax12.offset++;
		
		// Si les 1ers octets sont mauvais
		if ((ax12.offset >= 1) && (ax12.buffer[0] != START_BYTE)) erreur = 1;
		if ((ax12.offset >= 2) && (ax12.buffer[1] != START_BYTE)) erreur = 1;
		
		// En cas d'erreur on reset
		if (erreur == 1)
		{
			ax12.offset	= 0;
		}
	}		
	
	// Activation de l'interruption
	IEC1bits.U2RXIE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_U2TXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC1bits.U2TXIE	= 0;
	IFS1bits.U2TXIF = 0;
	
	ax12.nb_octet_transmis++;
	
	// Activation de l'interruption
	IEC1bits.U2TXIE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt, no_auto_psv)) 	_INT0Interrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.INT0IE	= 0;
	IFS0bits.INT0IF = 0;
	
	// Activation de l'interruption
	IEC0bits.INT0IE	= 1;
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_MI2C1Interrupt (void)
{
	// Desactivation de l'interruption
	IEC1bits.MI2C1IE	= 0;
	IFS1bits.MI2C1IF	= 0;
	
	// Activation de l'interruption
	IEC1bits.MI2C1IE	= 1;
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
			
			//Fixme : fucking problem!!
			if ((int)buf == 168)
			{
				//i2c.i2c_offset_rx = 0;
				i2c.buffer_rx[i2c.i2c_offset_rx] = buf;
			}	
			
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

	// Gestion des compteurs de temps
	_GestionTemps ();
	
	#if CARTE_DEFINIE == CARTE_BOARD1
	
		// Gestion de l'evitement adversaire de proximite
		//_AdvEvitementProximiteSimple ();
	
		// Asservissement
		_Asservissement ();
		
	#else
		// Gestion des moteurs
		_MoteurGestionAux ();
	#endif
	
	// Fin de match, on coupe l'asserv
	if (TimeEtatCompteur(TIME_ID_MATCH, TIME_LIMITE_MATCH) == 1)
	{
		// Le robot ne sortira plus de cette fonction
		StrategieArretDefinitifRobot ();
	}
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_T3Interrupt (void)
{
	// Desactivation de l'interruption
	T3CONbits.TON	= 0;
	IFS0bits.T3IF	= 0;
	
	// On force la com i2c toutes les 50ms
	if (i2c.i2c_compteur_com_forcee < 50)
	{
		i2c.i2c_compteur_com_forcee++;
		
		// Traitement
		#if defined( CONFIG_ROBOT_1 )
			_AutomGestionGeneraleRobot1 ();
		#elif defined( CONFIG_ROBOT_2 )
			_AutomGestionGeneraleRobot2 ();
		#endif
		
		// Verification AX12
		_AutomVerificationEtatAx12 ();
	}
	else
	{
		i2c.i2c_compteur_com_forcee = 0;
	}	
	
	// Activation de l'interruption
	T3CONbits.TON	= 1;
	IEC0bits.T3IE	= 1;
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
void __attribute__ ((interrupt,auto_psv)) 		_QEI1Interrupt (void)
{
	// Reset du flag
	IFS3bits.QEI1IF		= 0;
	
	// Sens de rotation positif
	if (QEI1CONbits.UPDN == 1)
		Capteurs.overflow[CODEUR_1]	= 1;
	
	// Sens de rotation negatif
	else
		Capteurs.overflow[CODEUR_1]	= 0;

	overflow[(int)LEFT_ENCODER_EXT] = Capteurs.overflow[CODEUR_1];
}


/*********************************************************/
void __attribute__ ((interrupt,auto_psv)) 		_QEI2Interrupt (void)
{
	// Reset du flag
	IFS4bits.QEI2IF		= 0;
	
	// Sens de rotation positif
	if (QEI2CONbits.UPDN == 1)
		Capteurs.overflow[CODEUR_2]	= 1;
	
	// Sens de rotation negatif
	else
		Capteurs.overflow[CODEUR_2]	= 0;

	overflow[(int)RIGHT_ENCODER_EXT] = Capteurs.overflow[CODEUR_2];
}
