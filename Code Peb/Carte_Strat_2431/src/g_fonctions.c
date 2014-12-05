/*****************************************************************************
* SUDRIABOTIK
******************************************************************************
* Nom Fichier 	:	a_fonctions.c
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Configuration bas niveau du PIC carte interface
******************************************************************************/

#include 	<pic18.h>			// définitions des registres PIC

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Tous les define pour carte interface

#include 	"v_glob_cod.h"	// Variables globales utilisées par les fonctions

#include 	"g_fonctions.h"		// Prototypes des fonctions utilisées par g_fonctions.c
#include	"g_config_mat.h"	// PROGRAMME DE CONFIGURATION DU PIC , Directives de compilation




/**************************************************************************************/
// Configuration PORT A
/**************************************************************************************/
void g_config_port(void)
{
	//PORTA
	ANSEL0 = 0;
	ADON   = 0;
	TRISA0 = 0;
	TRISA1 = 0;
	TRISA3 = 1;
	TRISA4 = 1;
	TRISA6 = 0;	//HTR
	
	//PORTB
	TRISB0 = 0;
	TRISB1 = 0;	// PWM MOTEUR GAUCHE
	TRISB2 = 0;	
	TRISB3 = 0;	// PWM MOTEUR DROIT
	TRISB4 = 1;	// Configuration N° PIC : entrée
	TRISB5 = 1;	// Configuration N° PIC : entrée
	
	//PORTC
	TRISC0 = 0;	// Sens moteur Gauche
	TRISC1 = 0;	// Sens moteur Droit
	TRISC2 = 0;	// VISU_OSCILLO
	TRISC3 = 0; // INTO
	TRISC6 = 0; // TX
	TRISC7 = 1; // RX
	
	RA0 = 0;
	RA1 = 0;
	RC0 = 0;
}

void g_config_power_pwm(void)
{	
	// Dans le Registre PTCON0
	// PTOPS3:0 utilisé pour configurer un postscaler sur PWM Time base pour gérer des interruptions
	PTOPS3 = 0;
	PTOPS2 = 0;
	PTOPS1 = 0;
	PTOPS0 = 0;
	
	// PTCKPS1:0 utilisé pour configurer un diviseur de la fréquence du PWM Time Base
	PTCKPS1 = 0;
	PTCKPS0 = 0;
	
	PTMOD1 = 0;
	PTMOD0 = 0;
	
// Dans le Registre PTCON1
	PTEN = 1;
	PTDIR = 0;
	
// Dans le Registre PWMCON0
	PWMEN2 = 1;
	PWMEN1 = 1;
	PWMEN0 = 0;
	
	PMOD3 = 1;
	PMOD2 = 1;
	PMOD1 = 1;
	PMOD0 = 1;
	
// Dans le Registre PWMCON1
	SEVOPS3 = 0;
	SEVOPS2 = 0;
	SEVOPS1 = 0;
	SEVOPS0 = 0;
	
	SEVTDIR = 0;
	UDIS = 0;
	OSYNC = 0;

	// Dans le Registre DTCON
	DTCON = 0x00;

	/**
	DTPS1 = 0;
	DTPS0 = 0;
	
	DT5 = 0;
	DT4 = 0;
	DT3 = 0;
	DT2 = 0;
	DT1 = 0;
	DT0 = 0; Le compilateur ne connait pas ces bits
	**/

	// Dans le Registre OVDCOND, 
	// POVD = 1 pour que la sortie PWM correspondante soit pilotée par le rapport cyclique
	// POVD = 0 la sortie PWM correspondante est écrasée par la valeur du bit POUT correspondant
	POVD7 = 1;
	POVD6 = 1;
	POVD5 = 1;
	POVD4 = 1;
	POVD3 = 1;
	POVD2 = 1;
	POVD1 = 1;
	POVD0 = 1;
		
	// Dans le Registre OVDCONS
	// POUT = valeur de la sortie PWM correspondante si le bit POVD est à 0
	POUT7 = 0;
	POUT6 = 0;
	POUT5 = 0;
	POUT4 = 0;
	POUT3 = 0;
	POUT2 = 0;
	POUT1 = 0;
	POUT0 = 0;
		
	// Dans Le Registre FLTCONFIG
	FLTCONFIG = 0x00; // Le module de détection de fautes est désactivé.
	
	/*BRFEN = 0;
	FLTBS = 0;
	FLTBMOD = 0;
	FLTBEN = 0;
	FLTCON = 0;
	FLTAS = 0;
	FLTAMOD = 0;
	FLTAEN = 0;*/	
}

void g_config_rs232(void)
{
	/*SPBRG = DIVIDER;
	TXSTA = (SPEED|NINE_BITS|0x20);
	RCREG;	// dummy read
	RCSTA = (NINE_BITS|0x90);*/
	
	
	// Registre RCSTA
	SPEN 	= 1; 	// 1 = Ligne série activée
	RX9	= 0;	// 0 = Réception 8 bits de données
	//SREN		// pas utilisé en mode asynchrone
	CREN	= 1;	// 1 = activation du récepteur
	ADEN	= 0;	// 0 = address detect non utilisé
	//FERR		// Framing error bit, accessible en lecture
	//OERR		// Overrun error bit, accessible en lecture
	//RX9D		// 9eme bit en réception dans le cas d'une réception 9 bits
		
	// Registre TXSTA
	//CSRC  	// non utilisé en mode asynchrone
	TX9 	= 0;	// 0 = communication en 8bits
	TXEN 	= 1;	// 1 = Transmission activée
	SYNC 	= 0;	// 0 = mode asynchrone
	//SENDB	= 0;	// 0 = send break caracter completed
	BRGH	= 1;	// 1 = High speed
	//TRMT		// Transmit shift register status, accessible en lecture
	TX9D	= 0;	// 9ème bit dans le cas d'une transmission avec 9 bits
	
	// Registre BAUDCTL
	//RCIDL 	// Receive operation idle status accessible en lecture
	//SCKP		// non utilisé en mode asynchrone
	BRG16 	= 1;	// 1 = Registre BRG16 sur 16 bits
	WUE	= 0; 	// 0 = Wake up enable bit non utilisé
	ABDEN	= 0; 	// 0 = Autodetection baud non utilisé
	
	// Registres SPBRGH et SPBRG
	// 19200 Bauds => 520 en décimal => 0x0208
	// 115200 Bauds => 86 en décimal => 0x0056
	SPBRGH	= 0x00;
	SPBRG	= 0x56;
}

void g_config_i2c_slave (void) // Configuration I2C pour carte esclave		
{
	// Raz interruption I2C éventuelle	
	SSPIF = 0;
	
	//Config Vitesse I2C (Uniquement en mode MASTER)
	//SSPADD 	=0x3F;	// Vitesse I2C = 100Khz 	

	//Config adresse esclave (Uniquement en mode SLAVE)
	SSPADD = SLAVE_NODE_ADDR;

	// Config Registre SSPSTAT
	SMP 	= 0;	// Disable slew rate control (vitesse standard 100KHz ou 1MHz)
	CKE 	= 0;	// Disable SMBus inputs
	//DA	//utilisé en lecture en mode slave : 1= octet recu = Data; 0= octet recu = Address
	//P	//utilisé en lecture : 1=indique qu'un bit stop a été recu (clear auto qd SSPEN = 0)
	//S	//utilisé en lecture : 1=indique qu'un bit start a été recu (clear auto qd SSPEN = 0)
	//RW	//utilisé en lecture : En mode slave 1=read; 0=write (valide uniquement jusqu'au prochain bit start, stop ou not ack ; En mode Master 1=transmit en cours 0= transmit not en cours
	//UA	//utilisé en lecture : En mode slave 10 bits : 1=indique que l'addresse doit être updatée dans SSPADD
	//BF	//utilisé en lecture : En mode transmit : 1=in progress, 0=transmit complete (SSPBUF vide); En mode receive : 1=receive complete, 0=receive not complete (SSPBUF vide)

	// Config Registre SSPCON 1
	//WCOL utilisé en lecture
	//SSPOV utilisé en lecture
	SSPEN 	= 1; 	// Activation Module SSP
	CKP = 1;// : Non utilisé en Mode Master; en mode slave 1= release clock, 0= hold clock
		// SSPM3:0 = 1000 : Mode Master
		// SSPM3:0 = 0110 : Mode Slave 7 bits sans ITs Start et Stop
		// SSPM3:0 = 1110 : Mode Slave 7 bits avec ITs Start et Stop
	SSPM3 	= 0;
	SSPM2 	= 1;
	SSPM1 	= 1;
	SSPM0 	= 0;
	
	// Config Registre SSPCON 2
	// commenté pour 18F2431 car Slave only
	//GCEN 	= 0; 	// 0 en mode master; en mode slave 1= activation IT lors d'un call general
	//ACKSTAT = 0; 	//En mode Master transmit : 1= ack non recu par slave, 0= ack recu par slave
	//ACKDT 	= 0;	//En mode Master receive :  1= ack non recu par slave, 0= ack recu par slave (bit recu si ack activé ds slave !!)
	//ACKEN 	= 0;	//En mode Master receive :  0= sequence ack au repos, 1= démarrage séquence ack pour renseigner bit ackdt
	//RCEN 	= 0;	//En mode Master : 1= activation réception, 0=réception désactivée
	//PEN 	= 0;	//En mode Master : Stop condition enable bit : 1=démarrage séquence Stop condition
	//RSEN	= 0;	//En mode Master : Séquence répétitive Start enable bit : 1=démarrage des séquences répétées
	//SEN	= 1;	//En mode Master : Start condition enable bit ; En mode Slave : Streched enable bit pour réception et émission
}

void g_config_interrupt_slave (void) // Configuration Interruptions pour carte esclave	
{	
	//Priorités
		GIEH = 1;		//enable toutes les interruptions de priorite haute
		GIEL = 1;		//enable toutes les interruptions de priorite basse
		IPEN=1;			/* interrupt priorities are used */
		TMR0IP = 1;		//priorite haute pour le timer0 (Phases HTR)
		//TMR1IP = 1;		//priorite haute pour le timer1 (Commande servomoteurs)
		//TMR3IP = 1;		//priorite haute pour le timer3
		SSPIP = 1;		//priorite haute pour I2C
		// Pas de bus collision en mode slave only (18F2431)
		//BCLIP = 1;		//priorite haute pour bus collision I2C
		RCIP = 1;		//priorite basse pour liaison debug RS232
		TXIP = 0;
		ADIP = 0;
		IC2QEIP = 1;
		IC3DRIP = 1;
	
	//Interruptions
		TMR0IE = 1;		//enable interruption overflow Timer0
		//TMR1IE = 1;		//enable interruption overflow Timer1
		//TMR3IE = 0;		//enable interruption overflow Timer3
//		INT0IE = 1;		//enable interruption INT0
//		INTEDG0 = 0;		// INT0 sur front descendant
		//PSPIE = 0;		//enable interruption PSP
		SSPIE = 1;		//enable interruption I2C
		// Pas de bus collision en mode slave only (18F2431)
		//BCLIE = 1;		//enable interruption Bus Collision I2C
		//ADIE = 1;
		//IC2QEIE = 1;		//enable interruption overflow codeur QEI
		//IC3DRIE = 1;		//enable interruption changement de sens codeur QEI
		
//	#if DEBUG == 1
//		#if SERIE == 1
			RCIE = 0;		//enable interruption de reception sur liaison serie
			//TXIE = 1;
//		#endif
//	#endif
}

static near bit T0CS @ ((unsigned)&T0CON*8)+5;

void g_config_timer0 (void) 	// Phases HTR
{
	T016BIT = 0;		//timer0 sur 16 bits
	T0CS = 0;			//clock interne (Fosc/4)
	PSA = 0;			//prescaler actif
	T0PS2 = 0;	//0		//prescaler 1:8
	T0PS1 = 1;	//1
	T0PS0 = 0;	//0
	
	TMR0 = PERIODE_HTR;
	TMR0IF = 0;
	TMR0ON = 1;			//demarrage du Timer0
	TMR0 = PERIODE_HTR;
}

void g_config_timer2 (void) // ==> Utiliser pour le module du PWM 
{
	TOUTPS0 = 1;
	TOUTPS0 = 1;
	TOUTPS0 = 1;
	TOUTPS0 = 1;
	
	T2CKPS1 = 0;		// T2CKPSx permettent de faire une division du timer 2, il
	T2CKPS0 = 0;		// est initialiser a 1:1 on a pour un oscilateur externe de 40Mhz un frequence de 39Khz
						// DANS LE PROG INITUP1 T2CKPS1=0 ET T2CKPS0=1 c'est dc un prescale de 1:4
	TMR2ON = 0;			//demarrage du Timer2
}

void g_config_timer5(void)
{
	// 16 bit Timer
	T5SEN=1; // 1 = Timer 5 enabled during sleep
	//RESEN=0; 
	T5MOD=0; // 0 = continuous count mode enabled
	T5PS1=0;
	T5PS0=0;  // Prescale de 1 => Compte à 10MHZ
	T5SYNC=0; // 1= pas synchronisé sur la clock externe, essayer 0 si possible pour éviter les pb de glitch ?
	TMR5CS=1; // 1 = external clock source
	TMR5ON=1; // 1 = timer 5 on
}

void g_config_mat (void)
{
	//CONFIGURATION PORTS
	g_config_port();
	
	// CONFIGURATION DES INTERRUPTIONS EN MODE I2C ESCLAVE
	g_config_interrupt_slave();
	
	// CONFIGURATION TIMERS
	g_config_timer0();
	g_config_timer2();
	g_config_timer5();
		
	// CONFIGURATION MODULE PWM		
	g_config_power_pwm();

	// CONFIGURATION RS232 (LIGNE SERIE DEBUG)
	g_config_rs232();
	
	// CONFIGURATION I2C (LIGNE COMMUNICATION AVEC Carte Maître)
	g_config_i2c_slave();
}

