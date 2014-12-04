/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: capteurs.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_CAPTEURS


	
/** I N C L U D E S **************************************/
	#include "../usr.h"
	


/** F O N C T I O N S ************************************/
void CapteursInit (void)
{
	// Declaration locale
	int i;
	
	// Boucle d'initialisation
	for (i=0; i<NB_CODEUR; i++)
	{
		Capteurs.overflow[i] = PAS_OVERFLOW;
	}
}


/*********************************************************/
uint16_t AcqCapteurAnalogique (int16_t num_capteur)
{
	return 1;
}


/*********************************************************/
uint16_t MesureCourantMoteur (int16_t num_moteur)
{
	return 1;
}


/*********************************************************/
uint16_t RetourImpCodeur (int16_t num_codeur)
{
	// Declaration locale
	static uint16_t acq = 0;
	
	// Codeur 1
	if (num_codeur == CODEUR_1)
		acq	= POS1CNT;

	// Codeur 2
	else if (num_codeur == CODEUR_2)
		acq	= POS2CNT;

	// Erreur
	else
		_TRACE_ERREUR_0x101
	
	return acq;
}


/*********************************************************/
uint16_t RetourSensCodeur (int16_t num_codeur)
{
	// Declaration locale
	static uint16_t acq = 0;
	
	// Codeur 1
	if (num_codeur == CODEUR_1)
		acq = QEI1CONbits.UPDN;
	
	// Codeur 2
	else if (num_codeur == CODEUR_2)
		acq = QEI2CONbits.UPDN;
	
	// Erreur
	else
		_TRACE_ERREUR_0x102
	
	return acq;
}


/*********************************************************/
int16_t SensRotationPendantOverFlow (int16_t num_codeur)
{
	// Declaration locale
	static int16_t ret = 0;
	
	// Codeur 1
	if (num_codeur == CODEUR_1)
	{
		ret = Capteurs.overflow[CODEUR_1];
		Capteurs.overflow[CODEUR_1] = PAS_OVERFLOW;
	}
	
	// Codeur 2
	else if (num_codeur == CODEUR_2)
	{
		ret = Capteurs.overflow[CODEUR_2];
		Capteurs.overflow[CODEUR_2] = PAS_OVERFLOW;
	}
	
	// Erreur
	else
		_TRACE_ERREUR_0x103
	
	return ret;
}


/*********************************************************/
void GestionModuleQEI (int16_t num_codeur, int16_t activation_desactivation)
{
	// Codeur 1
	if (num_codeur == CODEUR_1)
	{
		// Module QEI active
		if (activation_desactivation == ON)
		{
		#if CODEUR_1_INDEX == OUI
			QEI1CONbits.QEIM	= 0b110;			// Quadrature (x4), reset sur INDEX
		#else
			QEI1CONbits.QEIM	= 0b111;			// Quadrature (x4), reset sur buffer MAX1CNT
		#endif
		}
		
		// Module QEI desactive
		else if (activation_desactivation == OFF)
		{
			QEI1CONbits.QEIM	= 0b000;			// Moduel QEI desactive
		}
		
		// Erreur
		else
			_TRACE_ERREUR_0x105
	}
	
	// Codeur 2
	else if (num_codeur == CODEUR_2)
	{
		// Module QEI active
		if (activation_desactivation == ON)
		{
		#if CODEUR_2_INDEX == OUI
			QEI2CONbits.QEIM	= 0b110;			// Quadrature (x4), reset sur INDEX
		#else
			QEI2CONbits.QEIM	= 0b111;			// Quadrature (x4), reset sur buffer MAX1CNT
		#endif
		}
		
		// Module QEI desactive
		else if (activation_desactivation == OFF)
		{
			QEI2CONbits.QEIM	= 0b000;			// Moduel QEI desactive
		}
		
		// Erreur
		else
			_TRACE_ERREUR_0x106
	}
	
	// Erreur
	else
		_TRACE_ERREUR_0x104
}


/*********************************************************/
void AttenteJack (int8_t etat_jack_souhaite)
{
	#if CARTE_DEFINIE == CARTE_BOARD1
		
		// Attente de la mise en place du Jack
		if (etat_jack_souhaite == ATTENTE_MISE_JACK)
			while (CAPT_JACK);
		
		// Attente du retrait du jack
		else if (etat_jack_souhaite == ATTENTE_RETRAIT_JACK)
			while (!CAPT_JACK);
		
		// Erreur
		else
			_TRACE_ERREUR_0x107
	
	#else
	
		Nop ();
	
	#endif
}

int retourCapteurI2c (int num, int offset_i2c)
{
	int puissance = 0;
	Struct_8bits valeur;
	
	if (num == 1) puissance = 1;
	else if (num == 2) puissance = 2;
	else if (num == 3) puissance = 4;
	else if (num == 4) puissance = 8;
	else if (num == 5) puissance = 16;
	else if (num == 6) puissance = 32;
	else if (num == 7) puissance = 64;
	else if (num == 8) puissance = 128;
	
	return (valeur.u8b.Full&puissance);
}
