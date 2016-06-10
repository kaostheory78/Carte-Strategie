/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	i2c_comm.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES  
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Fonctions de configuration bas niveau dépendant du dsPIC utilisé
******************************************************************************/



//*****************************************************
// I2C GENERAL : BUFFERS DE DIALOGUE PSP <=> AUTRES PICS
//*****************************************************
#define MaxNumberI2CSlaves			12		// maximum number of I2C slave devices
#define OperNumberI2CSlaves			4		// operational number of I2C slaves

#define MaxChannelsPerSlave  		10		// maximum channels of data per slave// Nb de data dans la trame ?
#define OperChannelsPerSlave 		16		// operational number of channels of data
#define MaxLength2PC  				10

#define  MaxSlaveRetry   			1		// default is one retry

#define NB_DATA_BUFFER_PSP			64
#define NB_DATA_BUFFER_PICUP2		OperChannelsPerSlave
#define NB_DATA_BUFFER_PICG			OperChannelsPerSlave
#define NB_DATA_BUFFER_PICD			OperChannelsPerSlave
#define NB_DATA_BUFFER_PICBALISE	OperChannelsPerSlave

#define  error_mask  				0b0000000000000001

#define OFF_SLAVE_READ 				1		//car sinon on recupere le commstat comme premiere data utile

#define COMM_STAT  					0
//#define STATUS0   						1
//#define STATUS1   						2

//#define TEMP0     						3
//#define ADRES0     						4
//#define ADRES1    						5
//#define ADRES2   						6
//#define ADRES3     						7

//#define TACH0      						8
//#define TACH1      						9
//#define TACH2     						10
//#define TACH3     						11
//#define MAX_CHNNL						TACH3




//*****************************************************
// FONCTIONS
//*****************************************************
/** Service_I2CSlave : Communication I2C
 * @param : unsigned char i2c_addr		: adresse
 * @param : unsigned char i2c_rw		: read ou write
 * @param : unsigned char i2c_offset	: offset
 * @param : unsigned char i2c_nbdata	: données
 * @param : unsigned char i2c_mode		: mode
 */
void Service_I2CSlave (unsigned char i2c_addr, unsigned char i2c_rw, unsigned char i2c_offset, unsigned char i2c_nbdata, unsigned char i2c_mode);

/** Service_I2CSlave : Communication I2C
 * @param : unsigned char i2c_addr		: adresse
 * @param : unsigned char i2c_rw		: read ou write
 * @param : unsigned char i2c_offset	: offset
 * @param : unsigned char i2c_nbdata	: données
 * @param : unsigned char i2c_mode		: mode
 * @Rmq :	Will execute this switch/case evaluation for next I2C bus state
 */
void I2CBusState (unsigned char i2c_addr, unsigned char i2c_rw, unsigned char i2c_offset, unsigned char i2c_nbdata, unsigned char i2c_mode);

/** as_i2c_transfer_and_rs232debug : Fonction d'appel de communication I2C 
 *						   inter cartes en Mode Maître + Envoi Debug sur ligne série (provision)
 *
 * Entrées:
 * @param : unsigned char i2c_addr	: adresse du PIC esclave sur lequel va s'effectuer le transfert
* @param : unsigned char i2c_rw	:  0=trame à écrire ; 1=trame à lire
* @param : unsigned char i2c_offset 	: (de 0 à 255) Offset de la première donnée dans la trame
* @param : unsigned char i2c_nbdata	: (de 0 à 255) Nombre de données à transmettre
*
* Sorties : Aucune
* @param : unsigned char i2c_status	: Status de l'opération, 0=OK, sinon code d'erreur de 1 à TBD
* Interface HW :	Aucune (la fonction applicative ne communique pas directement avec la
*			couche I2C HW)
* Interface SW :	Fonction Service_I2CSlave
* Variables globales :	Structures sflag et eflag (Flags de synchro entre fonction
*				applicative et interruptions HW)
*/
void as_i2c_transfer_and_rs232debug (unsigned char i2c_addr, unsigned char i2c_rw, unsigned char i2c_offset, unsigned char i2c_nbdata, unsigned char i2c_mode);
