/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: i2c.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_I2C



/** I N C L U D E S **************************************/
	#include "../usr.h"



/** F O N C T I O N S ************************************/
void InitI2C (void)
{
	int i;
	
	// Init variables
	for (i=0; i<I2C_TAILLE_BUFFER; i++)
	{
		i2c.buffer_rx[i] = 0;
		i2c.buffer_tx[i] = 0;
	}
	
	i2c.i2c_offset_rx	= 0;
	i2c.i2c_offset_tx	= 0;


// CARTES ESCLAVES
#if CARTE_DEFINIE == CARTE_BOARD1
	I2C1MSK				= 0x00;					// Aucun mask
	I2C1ADD				= ADRESSE_I2C_CARTE;	// Adresse I2C de la carte
	
	// Reset buffer
	I2C1RCV 			= 0x0000;
	I2C1TRN				= 0x0000;
	
	// I2C1CON
	I2C1CONbits.SEN		= 0;	//
	I2C1CONbits.RSEN	= 0;	//
	I2C1CONbits.PEN		= 0;	//
	I2C1CONbits.RCEN	= 0;	//
	I2C1CONbits.ACKEN	= 0;	//
	I2C1CONbits.ACKDT	= 0;	//
	I2C1CONbits.STREN	= 0;	// SLAVE ONLY : lié a SCLREL
	I2C1CONbits.GCEN	= 0;	// SLAVE ONLY : general call adresse desactivé
	I2C1CONbits.SMEN	= 0;	// Desactivation de SMBus
	I2C1CONbits.DISSLW	= 1;	// Slew rate activé
	I2C1CONbits.A10M	= 0;	// Adresse 7bits
	I2C1CONbits.IPMIEN	= 0;	// SLAVE ONLY : support IPMI désactivé
	I2C1CONbits.SCLREL	= 0;	// SLAVE ONLY : Release clock
	I2C1CONbits.I2CSIDL	= 0;	// Mode continue en idle
	I2C1CONbits.I2CEN	= 1;	// Activation I2C
#endif


// CARTE MAITRE
#if CARTE_DEFINIE == CARTE_BOARD2

	I2C1BRG 			= 0x188;	// Baud Rate : 100KHz
		
	// Reset buffer
	I2C1RCV 			= 0x0000;
	I2C1TRN				= 0x0000;
	
	I2C1CONbits.SEN		= 0;	//
	I2C1CONbits.RSEN	= 0;	//
	I2C1CONbits.PEN		= 0;	//
	I2C1CONbits.RCEN	= 0;	//
	I2C1CONbits.ACKEN	= 0;	//
	I2C1CONbits.ACKDT	= 0;	//
	I2C1CONbits.STREN	= 0;	// SLAVE ONLY : lié a SCLREL
	I2C1CONbits.GCEN	= 0;	// SLAVE ONLY : general call adresse desactivé
	I2C1CONbits.SMEN	= 0;	// Desactivation de SMBus
	I2C1CONbits.DISSLW	= 1;	// Slew rate activé
	I2C1CONbits.A10M	= 0;	// Adresse 7bits
	I2C1CONbits.SCLREL	= 1;	// SLAVE ONLY : Release clock
	I2C1CONbits.I2CSIDL	= 0;	// Mode continue en idle
	I2C1CONbits.I2CEN	= 1;	// Activation I2C	
#endif
}


void PauseI2C (void)
{
	//
	Nop ();
}


void RelanceI2C (void)
{
	//
	Nop ();
}


int16_t IdleI2C (void)
{
	int i;
	
	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1STATbits.TRSTAT)
		{
			break;
		}
		else
		{
			if (i == 0)
				return ERREUR_I2C_IDLE;
		}
	}
	
	return 1;
}


int16_t StartI2C (void)
{
	int i;
	
	if (IdleI2C () != 1)
	{
		return ERREUR_I2C_IDLE;
	}
	
	I2C1CONbits.SEN = 1;

	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1CONbits.SEN)
		{
			break;
		}
		else
		{
			if (i == 0)
				return ERREUR_I2C_START;
		}
	}
	
	return 1;
}


int16_t StopI2C (void)
{
	int i;
	
	if (IdleI2C () != 1)
	{
		return ERREUR_I2C_IDLE;
	}
	
	I2C1CONbits.PEN = 1;
	
	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1CONbits.PEN)
		{
			break;
		}
		else
		{
			if (i == 0)
				return ERREUR_I2C_STOP;
		}
	}
	
	return 1;
}


int16_t RestartI2C (void)
{
	int i;
	
	if (IdleI2C () != 1)
	{
		return ERREUR_I2C_IDLE;
	}
	
	I2C1CONbits.RSEN = 1;
	
	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1CONbits.RSEN)
		{
			break;
		}
		else
		{
			if (i == 0)
				return ERREUR_I2C_RESTART;
		}
	}
	
	return 1;
}


void AckI2C (void)
{
	int i;
	
	I2C1CONbits.ACKDT = 0;
	I2C1CONbits.ACKEN = 1;
	
	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1CONbits.ACKEN)
			break;
	}
}


int16_t NackI2C (void)
{
	int i;
	
	I2C1CONbits.ACKDT = 1;
	I2C1CONbits.ACKEN = 1;
	
	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1CONbits.ACKEN)
		{
			break;
		}
		else
		{
			if (i == 0)
				return ERREUR_I2C_NACK;
		}
	}
	
	return 1;
}


int16_t WriteI2C (uint8_t byte)
{
	int i;
	
	if (IdleI2C () != 1)
	{
		return ERREUR_I2C_IDLE;
	}
	
	I2C1TRN = byte;
	
	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if (!I2C1STATbits.TBF)
		{
			break;
		}
		else
		{
			if (i == 0)
				return ERREUR_I2C_WRITE;
		}
	}
	
	return 1;
}


int16_t PutsMasterI2C (uint8_t adresse, uint8_t *donnees, uint8_t nb_octet)
{
	uint8_t j;
	
	if (StartI2C () != 1)
	{
		return ERREUR_I2C_START;
	}
	
	if (WriteI2C ((adresse<<1)) != 1)
	{
		return ERREUR_I2C_WRITE;
	}
	
	
	for (j=0; j<nb_octet; j++)
	{	
		if (WriteI2C (donnees[j]) != 1)
		{
			return ERREUR_I2C_WRITE;
		}
	}	
	
	if (StopI2C () != 1)
	{
		return ERREUR_I2C_STOP;
	}
	
	return 1;
}


int16_t ReadI2C (void)
{
	int i;
	static int16_t retval = 0;
	
	
	if (IdleI2C () != 1)
	{
		return ERREUR_I2C_IDLE;
	}
	
	I2C1CONbits.RCEN = 1;

	for (i=TIMEOUT_I2C; i>0; i--)
	{
		if(I2C1STATbits.RBF)
			break;
		
		if ((i-1) <= 0)
			return ERREUR_I2C_READ;
	}
	
	retval = (int16_t)I2C1RCV;
	
	return retval;
}


int16_t ReadsMasterI2C (uint8_t adresse, uint8_t nb_octet, uint8_t offset)
{
	uint8_t j;
	int16_t retval = 0;
	
	
	if (StartI2C () != 1)
	{
		return ERREUR_I2C_START;
	}
	
	if (WriteI2C ((adresse<<1)+1) != 1)
	{
		return ERREUR_I2C_WRITE;
	}
	
	
	for (j=0; j<nb_octet; j++)
	{
		retval = ReadI2C ();
		
		if (retval < 0)
		{
			i2c.buffer_rx[offset+j] = 0;
			return ERREUR_I2C_READ;
		}
		
		else
		{
			i2c.buffer_rx[offset+j] = (unsigned char)retval;
		}
		
		if ((j+1) >= nb_octet)
			NackI2C ();
		else
			AckI2C ();
	}
	
	if (StopI2C () != 1)
	{
		return ERREUR_I2C_STOP;
	}
	
	return 1;
}


void _PutcSlaveI2C (void)
{	
	if ((I2C1STATbits.R_W) && (!I2C1CONbits.SCLREL))
	{
		I2C1TRN				= i2c.buffer_tx[i2c.i2c_offset_tx];
		I2C1CONbits.SCLREL	= 1;
		
//		PutIntUART (i2c.i2c_offset_tx);
		
		i2c.i2c_offset_tx++;
	}
}


