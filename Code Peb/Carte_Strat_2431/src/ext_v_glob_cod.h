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
* Description	:	- Déclarations des prototypes des variables globales EXTERN
* Rmq		:	- Idem que v_glob_cod.h avec "extern" devant les déclarations 
*				de variables
******************************************************************************/


	extern int reception_infos_dspic_ok;
	extern unsigned int flag_maj_position_servo;
	extern unsigned int tmpPEB;
	
	extern unsigned int plop1;
	extern unsigned int plop2;
	
	extern int etat_init_servo;
	


//*************************************************************************************
// Variables pour la HTR
//*************************************************************************************
	extern	volatile unsigned int timer_htr;			// valeur du timer3 a charger pour la htr
	extern	volatile unsigned int val_phase_htr_reste;	// valeur du timer au moment ou le flag passe en attente
	extern	volatile unsigned char phase_htr;			// differentes phases durant la htr
	extern	volatile unsigned char phase_htr_temp;		//
	extern	volatile unsigned char erreur_htr;			//erreur : depassement de la HTR
	extern	volatile unsigned int val_timer0;			//
	extern	volatile unsigned int periode_us;			//
	extern	volatile bit flag_phase_htr;				// flag indiquant l'etat du traitement en cours
	extern	volatile unsigned char flag_carte_seule;	// flag utilisé pour utiliser la carte en solo (sans INT0)



//*************************************************************************************
// STRUCTURES DE DONNEES I2C / PSP
//*************************************************************************************
	// Structure de données du Système de positionnement du Robot
	extern volatile struct{
		//Delta_Cod
		union{
			struct{
				char L;
				char H;
			} Octet;
			int Full;	
		} Delta_Cod;
		
		//Codeur_Brut
		union{
			struct{
				char L;
				char H;
			} Octet;
			int Full;		
		} Codeur_Brut;
		
	} Syst_Position;

	// Structure de données du Système de Puissance du Robot
	// Structure de données du Système des Servosmoteurs du Robot
	// Structure de données du Système Capteurs / Objets du Robot



//*************************************************************************************
// Variables pour le Moteur (Moteur A connecté à PWM1 et SENS1 = RB0)
//*************************************************************************************
	extern	volatile int codeur;
	extern	volatile int codeur_prec;
	extern	volatile char sens_codeur;
	extern	volatile char sens_codeur_mem;
	extern	volatile char overflow_codeur;
	extern	volatile int vitesse_codeur;
	extern	volatile int vitesse_codeur_prec;
	extern	volatile char sens_codeur;
	extern volatile double terme_integral;

	extern  volatile int consigne_vitesse_trapeze;
	extern  volatile char cote_moteur;	
	extern	volatile double erreur;
	extern	volatile double erreur_prec;	

	extern	volatile unsigned char pwm_1_L;
	extern	volatile unsigned char pwm_1_H;
	extern	volatile unsigned char sens_1;
	
	extern	volatile struct{
			unsigned char Off;			//	 1 = Moteur OFF // 0 = Moteur ON
			unsigned char Sens;			//	 1 = moteur en avant, 0 = moteur en arrière 
			unsigned char Mode_Boost;	//	 1 = 
			
			//Delta_Position
			union{
				struct{
					unsigned char L;
					unsigned char H;	
				} Octet;
				int Full;					// 16 bit signé  Delta_Position.Full constitué des octets Delta_Position.Octet.L et Delta_Position.Octet.H
			} Delta_Position;
			
			//Consigne_Acceleration
			union{
				struct{
					unsigned char L;
					unsigned char H;	
				} Octet;
				int Full;					// 16 bit signé  Consigne_Acceleration.Full constitué des octets Consigne_Acceleration.Octet.L et Consigne_Vitesse.Octet.H
			} Consigne_Acceleration;
			
			//Consigne_Vitesse
			union{
				struct{
					unsigned char L;
					unsigned char H;	
				} Octet;
				int Full;					// 16 bit signé  Consigne_Vitesse.Full constitué des octets Consigne_Vitesse.Octet.L et Consigne_Vitesse.Octet.H
			} Consigne_Vitesse;
			
			//Acq_Vitesse
			union{
				struct{
					unsigned char L;
					unsigned char H;	
				} Octet;
				int Full;					// 16 bit signé  Consigne_Vitesse.Full constitué des octets Consigne_Vitesse.Octet.L et Consigne_Vitesse.Octet.H
			} Acq_Vitesse;
			
			//Consigne_Courant
			union{
				struct{
					unsigned char	L;
					unsigned char	H;
				} Octet;
				unsigned int Full_abs;		// 16 bit Acq_Courant.Full_abs constitué des octets Consigne_Courant.Octet.L et Consigne_Courant.Octet.H
			} Consigne_Courant;
			
			//Acq_Courant
			union{
				struct{
					unsigned char	L;
					unsigned char	H;
				} Octet;
				unsigned int Full_abs;		// 16 bit Acq_Courant.Full_abs constitué des octets Acq_Courant.Octet.L et Acq_Courant.Octet.H
			} Acq_Courant;
			
			//Consigne_PWM
			union{
				struct{
					unsigned char	L;
					unsigned char	H;
				} Octet;
				unsigned int Full_abs;		// 16 bit Consigne_PWM.Full_abs constitué des octets Consigne_PWM.Octet.L et Consigne_PWM.Octet.H
			} Consigne_PWM;
		} Moteur_a;



//*************************************************************************************
// Variables pour l'AD
//*************************************************************************************
	extern	volatile unsigned int flag_change_voie_adc;
	extern	unsigned int flag_validite;
	extern	volatile unsigned char voie_adc;
	extern  volatile unsigned char voie_adc_en[NB_AN];
	extern	volatile unsigned int flag_AD;
	extern	volatile struct{
			union{
				struct{
					unsigned char L;
					unsigned char H;
				} B[SENS_BUF_LEN];
				unsigned int B_full[SENS_BUF_LEN];
			} Capteurs;
			
			char Resultat[SENS_BUF_LEN];
			char ResultatPrecedent[SENS_BUF_LEN];
		} SensorBuf ;


	
//*************************************************************************************
// Variables pour l'I2C
//*************************************************************************************
	extern volatile unsigned char buffer_i2c_rx[128];
	extern volatile unsigned char buffer_i2c_tx[48];
	
	extern volatile unsigned char i2c_offset;
	extern volatile unsigned char i2c_nbdata;

	extern volatile unsigned char i2cstate_slave;
	extern volatile unsigned char i2c_slave_node_addr;

	extern volatile unsigned char *Slave2Master_Ptr;
	extern volatile unsigned char *Master2Slave_Ptr;

	extern	char buffer_datas_pic_RX[RX_BUF_LEN];	// Holds incoming bytes from master device.
	extern	char buffer_datas_pic_CMD[CMD_BUF_LEN];	// 
	extern	char buffer_datas_pic_checksum;			//

	extern	unsigned char buffer_datas_pic_RX_index;	// Index to received bytes.
	extern	unsigned char buffer_datas_pic_RX_count;	// Number of bytes received
	extern	unsigned char buffer_datas_pic_TX_index;	// Index to sensor data table
	extern	unsigned char buffer_datas_pic_CMD_index;	//
	extern	unsigned char PORTBold;						// Holds previous value of PORTB
	extern	unsigned char temp;

	extern	union INTVAL{
			char b[2];
			int i;
		}

	extern	union INTVAL TXChecksum;	// Holds checksum of bytes sent to master

	extern union TXBUF{					// Holds sensor data and other bytes to be sent to master.
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



//*************************************************************************************
// Variables pour le debug
//*************************************************************************************
	extern	volatile unsigned int flag_usart_rx;
	extern	volatile unsigned int index;
	extern	volatile char tx_buffer_debug[TX_BUF_LEN];
	extern	volatile char resultat_buffer_debug[SENS_BUF_LEN];
	extern	volatile unsigned int sensor_buffer_debug[SENS_BUF_LEN];
	extern volatile unsigned char flag_lecture_courant;
	extern volatile unsigned char flag_lecture_delta_position;
	extern volatile unsigned char flag_lecture_CAP2BUF;
	extern volatile unsigned char flag_lecture_codeur;
	extern volatile unsigned char flag_lecture_vitesse;
	extern volatile unsigned char flag_moteur_off;
	extern volatile unsigned char flag_lecture_consigne_vitesse;
	extern volatile unsigned char flag_lecture_erreur;
	extern volatile unsigned char flag_lecture_divers;

	extern volatile unsigned char digits[6];	// variable pour convertir un nb de 5 chiffres en chaine de car



//*************************************************************************************
// Variables pour les servos
//*************************************************************************************
	extern volatile int ajust_servo_zero1;		// permet d'ajuster la position 0° de servo 1
	extern volatile int ajust_servo_zero2;		// permet d'ajuster la position 0° de servo 2

	extern volatile struct{
		unsigned char phase;
		unsigned char numero;
		unsigned int commande_1;
		unsigned int commande_2;
		unsigned int commande_1_reste;
		unsigned int commande_2_reste;
		unsigned char position_1_recu;
		unsigned char position_2_recu;
		unsigned char Verrou;			
		int Ajust_servo1_0;
		int Ajust_servo1_180;
		int Ajust_servo2_0;	
		int Ajust_servo2_180;
	} Syst_servo ;
