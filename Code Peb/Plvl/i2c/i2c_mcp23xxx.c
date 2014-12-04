/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: i2c_mcp23018.c
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 11/03/2012
 *********************************************************
 *
 *
 *********************************************************/



/** T Y P E S ********************************************/
	#define TYPE_EXT_I2C_MCP23XXX



/** I N C L U D E S **************************************/
	#include "../usr.h"



/** F O N C T I O N S ************************************/
void mcp23018Init (unsigned char adresse)
{
	// Init en Bank=0
	mcp23018Config (adresse, MC23018_IOCONA, 	0x22);		// Registre de configuration
	mcp23018Config (adresse, MC23018_IOCONB, 	0x22);		// Registre de configuration
	mcp23018Config (adresse, MC23018_IODIRA,  	0xFF);   	// Toutes les broches en input
	mcp23018Config (adresse, MC23018_IODIRB,  	0xFF);   	// Toutes les broches en input
	mcp23018Config (adresse, MC23018_IPOLA,  	0xFF);   	// Logique inversée
	mcp23018Config (adresse, MC23018_IPOLB,  	0xFF);   	// Logique inversée
	mcp23018Config (adresse, MC23018_GPINTENA, 	0x00);   	// Pas d'interruption sur changement d'état
	mcp23018Config (adresse, MC23018_GPINTENA,	0x00);   	// Pas d'interruption sur changement d'état
	mcp23018Config (adresse, MC23018_GPPUA,  	0x00);   	// Pullups débranchées
	mcp23018Config (adresse, MC23018_GPPUB,  	0x00);   	// Pullups debranchées
	mcp23018Config (adresse, MC23018_INTFA,  	0x00);   	// Pas d'interruption
	mcp23018Config (adresse, MC23018_INTFB,  	0x00);   	// Pas d'interruption
}

unsigned char mcp23018ReadPortA (void)
{
	unsigned char valeur = 0;
	
	valeur = mcp23018ReadByte (ADRESSE_I2C_MCP23018, MC23018_GPIOA);
	
	return valeur;
}

unsigned char mcp23018ReadPortB (void)
{
	return mcp23018ReadByte (ADRESSE_I2C_MCP23018, MC23018_GPIOB);
}

int mcp23018ExtractGpioValue (int gpio, unsigned char valeur_port)
{
	return ((valeur_port&(1<<gpio)) >> gpio);
}

int mcp23018Config (unsigned char adresse, unsigned char reg, unsigned char valeur)
{
	uint8_t buffer[2];
	
	buffer[0] = reg;
	buffer[1] = valeur;
	
	return PutsMasterI2C (adresse, buffer, 2);
}

unsigned char mcp23018ReadByte (unsigned char adresse, unsigned char reg)
{
	int ret = 0;
	unsigned char val = 0;
	
	ret += StartI2C ();

	ret += WriteI2C ((adresse<<1));
	ret += WriteI2C (reg);
	
	ret += RestartI2C ();
	
	ret += WriteI2C ((adresse<<1)+1);
	val  = ReadI2C ();
	
	ret += NackI2C ();
	ret += StopI2C ();

	return val;
}

