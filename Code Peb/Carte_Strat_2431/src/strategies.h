//-----------------------------------------------
//	Auteurs : PEB
//
//	id@strategies.h
//	revision@1.0.0
//	04/01/10 : 03h31
//-----------------------------------------------

/** T Y P E S *********************************************************/
	#ifdef TYPE_STRAT_EXT
		#define TYPE_STRAT
	#else
		#define TYPE_STRAT	extern
	#endif

/** I N C L U D E S **************************************************/
	#include "define_cod.h"


/** D E F I N I T I O N S **********************************************/

	//POUR INIT
	#define PAS_INIT_SUR_AXE				10000
	#define PREMIERE_INIT					1
	#define NIEME_INIT						2
	
	#define POS_ADV_SUR_AUTRE_AXE			32000
	#define ATTENTE_FIN_TIMEOUT				1
	#define PAS_ATTENTE_FIN_TIMEOUT			2
	
	#define _EVITEMENT_ADV_AV_ACTIF			Asserv.flag_evitement_adv=1;
	#define _EVITEMENT_ADV_AR_ACTIF			Asserv.flag_evitement_adv=2;
	#define _EVITEMENT_ADV_INACTIF			Asserv.flag_evitement_adv=0;
	
	#define _ENCH_DEP_STRAT_POSSIBLE		Strategie.enchainement_deplacement = 1;
	#define _ENCH_DEP_STRAT_IMPOSSIBLE		Strategie.enchainement_deplacement = 0;
	
	#define _ROUE_ARRIERE_OFF				'0'
	#define _ROUE_ARRIERE_ON				'1'
	#define _ROUE_ARRIERE_TEMPO_ON			'2'
	#define _ROUE_ARRIERE_ON_SENS2			'3'
	#define _ROUE_ARRIERE_EJEC				'4'
	
	
	//Autom
	#define AUTOM_SANS_ATTENTE				0
	#define AUTOM_AVEC_ATTENTE				1
	
	//TIMEOUT
	#define SANS_TIMEOUT					5000/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_BALISE_ATTENTE			1000/DUREE_PHASE_TRAITEMENT		//ATTENTE DE LA MAJ DES DONNEES
	#define TIMEOUT_BALISE_0				2000/DUREE_PHASE_TRAITEMENT		//Pre scan
	#define TIMEOUT_BALISE_1				3000/DUREE_PHASE_TRAITEMENT		//Scan secondaire
	#define TIMEOUT_VAS_VERS				2000/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_CALLAGE_SORTIE_COL_1	2300/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_CALLAGE_SORTIE_COL_2	10000/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_DESCENTE_COLLINE		2500/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_AUTOM					500/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_EVITEMENT				500/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_RECULE_COLINE			1000/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_AVANCE_CALLAGE			1000/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_CALLAGE					2500/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_EVITEMENT_EPIS			25/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_CALLAGE_CHOP_ADV		10000/DUREE_PHASE_TRAITEMENT
	#define TIMOUT_MATCH					90000/DUREE_PHASE_TRAITEMENT
	#define TIMEOUT_DIAGONALE_EPIS			10000/DUREE_PHASE_TRAITEMENT
	
	#define MATCH_EN_COURS					0
	#define MATCH_TERMINE					1
	
	//Couleur
	#define BLEU							0
	#define JAUNE							1
	#define LED_BLEU						RA0
	#define LED_JAUNE						RA1
	
	//Flag
	/*
	#define Flag_STRAT						Strat.Flag.Octet
	#define FLAG_RAMASSAGE_ORANGES			Strat.Flag.bits.bit0
	#define FLAG_RAMASSAGE_TOMATES			Strat.Flag.bits.bit1
	#define FLAG_RAMASSAGE_EPIS				Strat.Flag.bits.bit2
	#define FLAG_DEPOSE_BALLES				Strat.Flag.bits.bit3
	#define FLAG_DEPOSE_EPIS				Strat.Flag.bits.bit4
	*/
	
	#define ACT_ORANGE_COL_JAUNE	1
	#define ACT_ORANGE_COL_BLEU		2
	#define ACT_ORANGE_LAT_JAUNE	3
	#define ACT_ORANGE_LAT_BLEU		4
	#define ACT_EPIS_2B				5
	#define ACT_EPIS_BG				6
	#define ACT_EPIS_BD				7
	#define ACT_TOMATE_BD			7
	#define ACT_EPIS_LARGAGE		8
	#define ACT_ORANGE_LARGAGE		9
	#define ACT_CUL_ROBOT_LEVE		10
	#define ACT_CUL_ROBOT_BAS		11
	#define ACT_CUL_ROBOT_MOITIER	12
	#define ACT_CUL_ROBOT_AVANCER	13
	#define ACT_CUL_ROBOT_MONT_COL	14
	#define COUPURE_AUTOM			255
	
	
	//Sys
	#define SYS_STRAT_1						Strat.Sys_1.Octet
	#define SYS_JACK						Strat.Sys_1.bits.bit7
	#define SYS_COULEUR_DEPART				Strat.Sys_1.bits.bit6
	#define SYS_SICK_AR_G					Strat.Sys_1.bits.bit5
	#define SYS_SICK_AR_D					Strat.Sys_1.bits.bit4
	#define SYS_CT_AV_G						Strat.Sys_1.bits.bit3
	#define SYS_CT_AV_D						Strat.Sys_1.bits.bit2
	#define SYS_CT_AR_G						Strat.Sys_1.bits.bit1
	#define SYS_US_AV						Strat.Sys_1.bits.bit0


/** V A R I A B L E S *************************************************/
	
	TYPE_STRAT volatile struct {
		//Flag
		union {
			unsigned char Octet;
			struct {				// Structure with 8 bits 
				unsigned char bit0:1;	//LSB Bit 0
				unsigned char bit1:1;
				unsigned char bit2:1;
				unsigned char bit3:1;
				unsigned char bit4:1;
				unsigned char bit5:1;
				unsigned char bit6:1;
				unsigned char bit7:1;	//MSB bit 7
			}bits;
		}Flag;
		
		//Flag
		union {
			unsigned char Octet;
			struct {				// Structure with 8 bits 
				unsigned char bit7:1;	//LSB Bit 0
				unsigned char bit6:1;
				unsigned char bit5:1;
				unsigned char bit4:1;
				unsigned char bit3:1;
				unsigned char bit2:1;
				unsigned char bit1:1;
				unsigned char bit0:1;	//MSB bit 7
			}bits;
		}Sys_1;
	}Strat;
	
	//Adversaire
	TYPE_STRAT struct {
		union {
			struct {
				char L;
				char H;
			} Octet;
			int Full;
		} X;
		
		union {
			struct {
				char L;
				char H;
			} Octet;
			int Full;
		} Y;
		
		union {
			struct {
				char L;
				char H;
			} Octet;
			int Full;
		} THETA;
	} Adv;
	
	//Timeout
	TYPE_STRAT volatile struct {
		INT_US32B match;			//comptage
		INT_US32B evitement;		//
		INT_US32B deplacement;		//decomptage
		INT_US32B callage;			//comtpage
		INT_US32B autom;			//comtpage
		INT_US32B balise;			//comptage
		int blocage_timeout_match;
		int blocage_timeout_evitement;
		int blocage_timeout_deplacement;
		int blocage_timeout_callage;
		int blocage_timeout_autom;
		int blocage_timeout_balise;
	} timeout;
	
	TYPE_STRAT volatile struct {
		
		//INITILISATION
		INT_S16B x_init;
		INT_S16B y_init;
		INT_S16B theta_init;
		INT_S16B couleur_depart;
		INT_S16B etat_match;		//0:Match en cours  | 1:Fin de match
		char octet_recu_rx;
		int enchainement_deplacement;
		
		int etape;
	}Strategie;	

/** P R O T O T Y P E S ***********************************************/

	void TraitementPrincipal(void);
	
	void ActionAutomatisme(int type, int attente_ou_pas);
	
	int ActionAutomatismeBalise2(char autom);
	
	void Test(void);
	
	void Diagonal_1(void);
	
	void Diagonal_2(void);
	
	void EjectionBalles(void);
	
	void StratHomologation(void);
	
	void CallageDepart(void);
	
	void TestsAsserv(void);
	
	void FinMatch(void);
	
	void LedCouleurDepart(void);
	
	void _CoupureAutom(void);
	
	int PositionAdv(long x_mm, long y_mm, long fourchette_x_mm, long fourchette_y_mm, INT_US32B timeout_fin, int attente_timeout);
	


