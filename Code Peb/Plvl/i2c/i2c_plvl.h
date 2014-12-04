/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: i2c.h
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * > Envoi de données Maitre vers le Esclave :
 * 		- Maitre (traitement main) avec PutsMasterI2C (adresse, buf_tableau_de_valeur, nb_octet_a_envoyer);
 * 		- Esclave (traitement it) valeur recuent directement dans le buffer de recepion, utilisation de define pour simplifier la lecture
 *
 * > Lecture de données du Maitre depuis l'Esclave :
 *	Maitre (traitement main) avec 
 *	Esclave (traitement it) après prise en compte de la demande envoi des valeurs a chaque it
 * 
 *********************************************************/

#ifndef _I2C_H_
#define _I2C_H_


/** T Y P E S ********************************************/
	#ifdef TYPE_EXT_I2C
		#define TYPE_I2C
	#else
		#define TYPE_I2C extern
	#endif



/** D E F I N E S ****************************************/

	//Adresses I2C
	#define ADRESSE_I2C_PRINCIPALE			0x32		// Adresse I2C de la seconde carte PLVL
	#define ADRESSE_I2C_SECONDAIRE			0x00		// Carte maitre, pas d'adresse
	#define ADRESSE_I2C_BALISE				0x22		// Adresse I2C de la carte Balise centrale
	#define ADRESSE_I2C_IGEP				0x24		// Adresse I2C de la carte IGEP
	#define ADRESSE_I2C_MCP23018			0x27		// Adresse I2C de la carte MCP23018 (n=7 => 0b0100111x)
	
	// Configuration de l'I2C
	#define CARTE_I2C_MAITRE				1
	#define CARTE_I2C_ESCLAVE				2
	
	#define TIMEOUT_I2C						0xFFF
	
	#define ERREUR_I2C_IDLE					-10
	#define ERREUR_I2C_START				-20
	#define ERREUR_I2C_STOP					-30
	#define ERREUR_I2C_RESTART				-40
	#define ERREUR_I2C_WRITE				-50
	#define ERREUR_I2C_ACK					-60
	#define ERREUR_I2C_NACK					-70
	#define ERREUR_I2C_READ					-80
	
	#define I2C_TAILLE_BUFFER				128


	
/** D E C L A R A T I O N S ******************************/
	typedef struct {
		int i2c_compteur_com_forcee;
		int i2c_offset_rx;
		int i2c_offset_tx;
		unsigned char buffer_rx[I2C_TAILLE_BUFFER];
		unsigned char buffer_tx[I2C_TAILLE_BUFFER];
		unsigned char buffer_i2c_rx_corrige[I2C_TAILLE_BUFFER];
	} Struct_i2c;
	
	TYPE_I2C Struct_i2c i2c;
	


/** P R O T O T Y P E S **********************************/

	void InitI2C (void);
	

	void PauseI2C (void);			// A FAIRE
	
	
	void RelanceI2C (void);			// A FAIRE
	
	
	/**
	 * @brief	: 	Attend que le bus I2C soit en IDLE
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_IDLE si le bus n'est pas libre apres le timeout
	 */
	int16_t IdleI2C (void);
	
	
	/**
	 * @brief	: 	Envoi une condition START sur le bus
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_IDLE si le bus n'est pas libre apres le timeout
	 *	<br>		ERREUR_I2C_START si la condition START n'a pas ete envoyee
	 */
	int16_t StartI2C (void);
	
	
	/**
	 * @brief	: 	Envoi une condition STOP sur le bus
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_IDLE si le bus n'est pas libre apres le timeout
	 *	<br>		ERREUR_I2C_STOP si la condition STOP n'a pas ete envoyee
	 */
	int16_t StopI2C (void);
	
	
	/**
	 * @brief	: 	Envoi une condition RESTART sur le bus
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_IDLE si le bus n'est pas libre apres le timeout
	 *	<br>		ERREUR_I2C_RESTART si la condition RESTART n'a pas ete envoyee
	 */
	int16_t RestartI2C (void);
	
		
	/**
	 * @brief	: 	Envoi un ACK sur le bus
	 */
	void AckI2C (void);
	
	
	/**
	 * @brief	: 	Envoi un NACK sur le bus
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_IDLE si le bus n'est pas libre apres le timeout
	 *	<br>		ERREUR_I2C_NACK si le NACK n'a pas ete envoye
	 */
	int16_t NackI2C (void);
	
	
	/**
	 * @brief	: 	Envoi un octet sur le bus (adresse ou donnee)
	 * @param	:	byte	: octet a envoyer
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_IDLE si le bus n'est pas libre apres le timeout
	 *	<br>		ERREUR_I2C_WRITE si l'octet n'a pas ete envoye
	 */
	int16_t WriteI2C (uint8_t byte);
	
	
	/**
	 * @brief	: 	Envoi un/des octets a une carte ESCLAVE
	 * @param	:	adresse	: adresse de la carte esclave
	 * @param	:	donnees	: octets a envoyer
	 * @param	:	nb_octet: nombre d'octet a envoyer
	 * @return	: 	1 en cas de succes
	 *	<br>		ERREUR_I2C_START si la condition START n'a pas ete envoyee
	 *	<br>		ERREUR_I2C_WRITE si l'octet n'a pas ete envoye
	 *	<br>		ERREUR_I2C_STOP si la condition STOP n'a pas ete envoyee
	 */
	int16_t PutsMasterI2C (uint8_t adresse, uint8_t *donnees, uint8_t nb_octet);
	
	
	
	int16_t ReadI2C (void);
	
	
	int16_t ReadsMasterI2C (uint8_t adresse, uint8_t nb_octet, uint8_t offset);
	
	
	void _PutcSlaveI2C (void);
	

#endif /* _I2C_H_ */
