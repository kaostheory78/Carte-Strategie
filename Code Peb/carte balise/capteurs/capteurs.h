/*********************************************************
 * CARTE BALISE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: capteurs.h
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 23/09/2011
 *********************************************************
 * 
 * 
 *********************************************************/



/** T Y P E S ********************************************/
	#ifdef TYPE_EXT_CAPTEURS
		#define TYPE_CAPTEURS
	#else
		#define TYPE_CAPTEURS	extern
	#endif



/** D E F I N E S ****************************************/	
	
	//Type de capteurs
	#define CAPTEUR_NUMERIQUE				1			// Capteur de type numerique
	#define CAPTEUR_ANALOGIQUE				2			// Capteur de type analogique

	
	//Capteurs numerique
	#define CAPTEUR_1N						(_RB2)		// Entree numerique
	#define CAPTEUR_2N						(_RB3)		// Entree numerique


	// Identifiant pour appel de fonction
	#define NB_CAPTEUR						2
	#define CAPTEUR_A1						1
	#define CAPTEUR_A2						2
	
	#define MESURE_MOTEUR_1					1

	
	// Sens des codeurs
	#define CODEUR_RECUL					0
	#define CODEUR_AVANCE					1
	
	#define PAS_OVERFLOW					-1
	


/** S T R U C T U R E S **********************************/

	// Structures pour les capteurs
	typedef struct {
		int8_t overflow;			// Memorisation du sens de rotation lors d'un overflow
	} Struct_capteurs;



/** D E C L A R A T I O N S ******************************/

	// Structure qui memorise des infos sur les capteurs
	TYPE_CAPTEURS Struct_capteurs Capteurs;



/** P R O T O T Y P E S **********************************/

/**
 * @brief	: Fonction permettant l'initialisation des variables capteurs
 **/
void CapteursInit (void);


/**
 * @brief	: Fonction permettant l'acquistion de capteurs analogiques
 * @param	: num_capteur	: identifiant du capteur analogique (CAPTEUR_A1 | CAPTEUR_A2 | CAPTEUR_A3 | CAPTEUR_A4)
 * @return	: valeur retournee par le capteur analogique
 **/
uint16_t AcqCapteurAnalogique (int16_t num_capteur);			// @TODO


/**
 * @brief	: Fonction permettant la mesure du courant d'un moteur
 * @param	: num_moteur	: identifiant du moteur (MESURE_MOTEUR_1 | MESURE_MOTEUR_2)
 * @return	: valeur du courant consome par le moteur
 **/
uint16_t MesureCourantMoteur (void);				// @TODO


/**
 * @brief	: Fonction permettant l'acquistion du nombre d'impulsion d'un codeur
 * @param	: num_codeur	: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @return	: nombre d'impulsion codeur
 */
uint16_t RetourImpCodeur (void);


/**
 * @brief	: Fonction permettant l'acquistion du sens de rotation d'un codeur
 * @param	: num_codeur	: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @return	: sens de rotation du codeur (CODEUR_1_RECUL | CODEUR_1_AVANCE | CODEUR_2_RECUL | CODEUR_2_AVANCE)
 */
uint16_t RetourSensCodeur (void);


/**
 * @brief	: Fonction permettant de savoir si un overflow sur un codeur a eu lieu depuis le dernier appel de cette fonction
 * <br>		  et si oui le sens de rotation du codeur durant l'overflow
 * @param	: num_codeur	: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @return	: PAS_OVERFLOW si aucun overflow n'a eu lieu
 * <br>		  sinon le sens de rotation du codeur au moment de l'overflow (CODEUR_1_RECUL | CODEUR_1_AVANCE | CODEUR_2_RECUL | CODEUR_2_AVANCE)
 */
int16_t SensRotationPendantOverFlow (void);


/**
 * @brief	: Fonction permettant d'activer ou de descativer le module QEI
 * @param	: num_codeur				: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @param	: activation_desactivation	: ON pour activer le module QEI, OFF pour le desactiver
 */
void GestionModuleQEI (int16_t activation_desactivation);



