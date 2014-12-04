/*********************************************************
 * CARTE BALISE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: capteurs.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 23/09/2011
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
	Capteurs.overflow = PAS_OVERFLOW;
}


/*********************************************************/
uint16_t AcqCapteurAnalogique (int16_t num_capteur)
{
	Nop ();		// VITAL! sans ce Nop le dspic plante... Pourquoi, là est la question^^
	
	AD1CON1bits.SAMP = 1;
	Delay_Xx10us (1);
	AD1CON1bits.SAMP = 0;
	
	while (!AD1CON1bits.DONE);
	
	return ADCBUF0;	//ADC1BUF0	//*((&ADCBUF0)+num_capteur);
}


/*********************************************************/
uint16_t MesureCourantMoteur (void)
{
	static uint16_t retval;
	
	// Courant = (Vadc/(Rsense*ResolutionLMD18200))/2^12bits)*Mesure
	//           (3.3V/ 10000 *     0.000377     ))/  4096  )*Mesure
	retval = AcqCapteurAnalogique(0);
	
	return retval;
}


/*********************************************************/
uint16_t RetourImpCodeur (void)
{
	// Declaration locale
	static uint16_t acq = 0;
	
	acq	= POS1CNT;
	
	return acq;
}


/*********************************************************/
uint16_t RetourSensCodeur (void)
{
	// Declaration locale
	static uint16_t acq = 0;
	
	acq = QEI1CONbits.UPDN;
	
	return acq;
}


/*********************************************************/
int16_t SensRotationPendantOverFlow (void)
{
	// Declaration locale
	static int16_t ret = 0;
	
	ret = Capteurs.overflow;
	Capteurs.overflow = PAS_OVERFLOW;
	
	return ret;
}


/*********************************************************/
void GestionModuleQEI (int16_t activation_desactivation)
{
	// Module QEI active
	if (activation_desactivation == ON)
	{
	#if CODEUR_INDEX == OUI
		QEI1CONbits.QEIM	= 0b110;			// Quadrature (x4), reset sur INDEX
	#else
		QEI1CONbits.QEIM	= 0b111;			// Quadrature (x4), reset sur buffer MAX1CNT
	#endif
	}
	
	// Module QEI desactive
	else
	{
		QEI1CONbits.QEIM	= 0b000;			// Moduel QEI desactive
	}
}



