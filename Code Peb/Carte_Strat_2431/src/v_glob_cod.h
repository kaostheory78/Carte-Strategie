/*****************************************************************************
* SUDRIABOTIK
******************************************************************************
* Nom Fichier 	:	v_glob_cod.h
* Operating System:	Pas d'OS, Fichiers "maison" pour Carte INTERCONNEXION
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Déclarations des prototypes des variables globales utilisé par
*				main_cod.c
******************************************************************************/

	
	/** T Y P E S *********************************************************/
	#ifdef TYPE_MAIN_EXT
		#define TYPE_MAIN
	#else
		#define TYPE_MAIN	extern
	#endif
	
	TYPE_MAIN volatile unsigned int traitement_i2c_nb_octet;
	TYPE_MAIN volatile unsigned int traitement_i2c_cpt_octet;
	TYPE_MAIN volatile unsigned int temporisation_raf_debug;

//*************************************************************************************
// Variables pour la HTR
//*************************************************************************************
	TYPE_MAIN volatile unsigned int timer_htr;			// valeur du timer3 a charger pour la htr
	TYPE_MAIN volatile unsigned int val_phase_htr_reste;	// valeur du timer au moment ou le flag passe en attente
	TYPE_MAIN volatile unsigned char phase_htr;			// differentes phases durant la htr
	TYPE_MAIN volatile unsigned char phase_htr_temp;		//
	TYPE_MAIN volatile unsigned char erreur_htr;			//erreur : depassement de la HTR
	TYPE_MAIN volatile unsigned int val_timer0;			//
	TYPE_MAIN volatile unsigned int periode_us;			//
	TYPE_MAIN volatile bit flag_phase_htr;				// flag indiquant l'etat du traitement en cours
	TYPE_MAIN volatile unsigned char flag_carte_seule;	// flag utilisé pour utiliser la carte en solo (sans INT0)

	
//*************************************************************************************
// Variables pour l'I2C
//*************************************************************************************
	TYPE_MAIN volatile unsigned char buffer_i2c_rx[32];
	TYPE_MAIN volatile unsigned char buffer_i2c_tx[32];
	
	TYPE_MAIN volatile unsigned char i2c_offset;
	TYPE_MAIN volatile unsigned char i2c_nbdata;

	TYPE_MAIN volatile unsigned char i2cstate_slave;
	TYPE_MAIN volatile unsigned char i2c_slave_node_addr;

	TYPE_MAIN volatile unsigned char *Slave2Master_Ptr;
	TYPE_MAIN volatile unsigned char *Master2Slave_Ptr;
	
//	TYPE_MAIN char buffer_datas_pic_RX[RX_BUF_LEN];		// Holds incoming bytes from master device.
//	TYPE_MAIN char buffer_datas_pic_CMD[CMD_BUF_LEN];		// 
//	TYPE_MAIN char buffer_datas_pic_checksum;				//
//
//	TYPE_MAIN unsigned char buffer_datas_pic_RX_index;	// Index to received bytes.
//	TYPE_MAIN unsigned char buffer_datas_pic_RX_count;	// Number of bytes received
//	TYPE_MAIN unsigned char buffer_datas_pic_TX_index;	// Index to sensor data table
//	TYPE_MAIN unsigned char buffer_datas_pic_CMD_index;	//
//	TYPE_MAIN unsigned char PORTBold;						// Holds previous value of PORTB
//	TYPE_MAIN unsigned char temp;							// tmp

/*
	union INTVAL{
		char b[2];
		int i;
	}

	union INTVAL TXChecksum;	// Holds checksum of bytes sent to master

	union TXBUF{				// Holds sensor data and other bytes to be sent to master.
		struct{									
    		unsigned	chkfail:1;	
    		unsigned	rxerror:1;	
    		unsigned	ovflw:1;
    		unsigned	sspov:1;
    		unsigned	bit4:1;
    		unsigned	bit5:1;
    		unsigned	bit6:1;
    		unsigned	r_w:1;
		} comm_stat ;
		
		char buffer_datas_pic_TX[TX_BUF_LEN];
	} TxBuf ;
*/


//*************************************************************************************
// Variables pour le debug
//*************************************************************************************
	TYPE_MAIN volatile unsigned char digits[6];	// variable pour convertir un nb de 5 chiffres en chaine de caractère

	
