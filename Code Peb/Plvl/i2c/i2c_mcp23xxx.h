/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: i2c_mcp23xxx.h
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 11/03/2012
 *********************************************************
 *
 *********************************************************/



/** T Y P E S ********************************************/
	#ifdef TYPE_EXT_I2C_MCP23XXX
		#define TYPE_I2C_MCP23XXX
	#else
		#define TYPE_I2C_MCP23XXX extern
	#endif



/** D E F I N E S ****************************************/
	// For IOCON.BANK = 0
	#define MC23018_IODIRA   	0x00
	#define MC23018_IODIRB   	0x01
	#define MC23018_IPOLA    	0x02
	#define MC23018_IPOLB    	0x03
	#define MC23018_GPINTENA 	0x04
	#define MC23018_GPINTENB 	0x05
	#define MC23018_DEFVALA  	0x06
	#define MC23018_DEFVALB  	0x07
	#define MC23018_INTCONA  	0x08
	#define MC23018_INTCONB  	0x09
	#define MC23018_IOCONA   	0x0A
	#define MC23018_IOCONB   	0x0B
	#define MC23018_GPPUA    	0x0C
	#define MC23018_GPPUB    	0x0D
	#define MC23018_INTFA    	0x0E
	#define MC23018_INTFB    	0x0F
	#define MC23018_INTCAPA  	0x010
	#define MC23018_INTCAPB  	0x011
	#define MC23018_GPIOA    	0x012
	#define MC23018_GPIOB    	0x013
	#define MC23018_OLATA    	0x014
	#define MC23018_OLATB    	0x015
	
	// Offset du capteur sur la valeur du port A
	#define MCP23018_CAPT_A1	1
	#define MCP23018_CAPT_A2	2
	#define MCP23018_CAPT_A3	3
	#define MCP23018_CAPT_A4	4
	#define MCP23018_CAPT_A5	5
	#define MCP23018_CAPT_A6	6
	
	// Offset du capteur sur la valeur du port B
	#define MCP23018_CAPT_B1	1
	#define MCP23018_CAPT_B2	2
	#define MCP23018_CAPT_B3	3
	#define MCP23018_CAPT_B4	4
	#define MCP23018_CAPT_B5	5
	#define MCP23018_CAPT_B6	6
	#define MCP23018_CAPT_B7	7
	
#if defined( CONFIG_ROBOT_2 )
	#define _MCP_CAPT_LINGOT_D		mcp23018ExtractGpioValue (MCP23018_CAPT_A1, R2expander.portA)
	#define _MCP_CAPT_LINGOT_G		mcp23018ExtractGpioValue (MCP23018_CAPT_A2, R2expander.portA)
	#define _MCP_CAPT_SOLE_GD		mcp23018ExtractGpioValue (MCP23018_CAPT_A3, R2expander.portA)
	#define _MCP_CAPT_SOLE_GG		mcp23018ExtractGpioValue (MCP23018_CAPT_A4, R2expander.portA)
			
	#define _MCP_CAPT_COULEUR		mcp23018ExtractGpioValue (MCP23018_CAPT_B2, R2expander.portB)
	#define _MCP_CAPT_STRAT			mcp23018ExtractGpioValue (MCP23018_CAPT_B3, R2expander.portB)
	#define _MCP_CAPT_BRAS_D		mcp23018ExtractGpioValue (MCP23018_CAPT_B5, R2expander.portB)
	#define _MCP_CAPT_BRAS_G		mcp23018ExtractGpioValue (MCP23018_CAPT_B6, R2expander.portB)
#endif
	


/** D E C L A R A T I O N S ******************************/
	typedef struct {
		unsigned char portA;
		unsigned char portB;
	} Struct_mcp23xxx;
	
	TYPE_I2C_MCP23XXX Struct_mcp23xxx R2expander;


/** P R O T O T Y P E S **********************************/

	void mcp23018Init (unsigned char adresse);
	
	unsigned char mcp23018ReadPortA (void);
	
	unsigned char mcp23018ReadPortB (void);
	
	int mcp23018ExtractGpioValue (int gpio, unsigned char valeur_port);
	
	int mcp23018Config (unsigned char adresse, unsigned char reg, unsigned char valeur);
	
	unsigned char mcp23018ReadByte (unsigned char adresse, unsigned char reg);


