/*********************************************************
 * CARTE POLYVALENTE - DSPIC33FJ128MC804
 * http://pebdev.fr
 *********************************************************
 * Fichier 	: capteurs.h
 * Auteur  	: PEB
 * Revision	: 1.0
 * Date		: 07/05/2011
 *********************************************************
 * 
 * 
 *********************************************************/

#ifndef _CAPTEURS_H_
#define _CAPTEURS_H_

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
	#define CAPTEUR_3N						(_RC0)		// Entree numerique
	#define CAPTEUR_4N						(_RC1)		// Entree numerique
	#define CAPTEUR_5N						(_RA9)		// Entree numerique
	#define CAPTEUR_6N						(_RA10)		// Entree numerique


	// Identifiant pour appel de fonction
	#define NB_CAPTEUR						6
	#define CAPTEUR_A1						1
	#define CAPTEUR_A2						2
	#define CAPTEUR_A3						3
	#define CAPTEUR_A4						4
	
	#define MESURE_MOTEUR_1					1
	#define MESURE_MOTEUR_2					2


	// Identifiant pour appel de fonction (attention, offset lie a des pointeurs)
	#define NB_CODEUR						2
	
	#define CODEUR_1						0
	#define CODEUR_2						1

	#define CODEUR_GAUCHE					CODEUR_1
	#define CODEUR_DROIT					CODEUR_2
	
	
	// Sens des codeurs
	#define CODEUR_1_RECUL					0
	#define CODEUR_1_AVANCE					1
	
	#define CODEUR_2_RECUL					1
	#define CODEUR_2_AVANCE					0
	
	#define PAS_OVERFLOW					-1
	
	// Jack
	#define ATTENTE_MISE_JACK				1
	#define ATTENTE_RETRAIT_JACK			2



/** S T R U C T U R E S **********************************/

	// Structures pour les capteurs
	typedef struct {
		int8_t overflow[NB_CODEUR];			// Memorisation du sens de rotation lors d'un overflow
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
uint16_t MesureCourantMoteur (int16_t num_moteur);				// @TODO


/**
 * @brief	: Fonction permettant l'acquistion du nombre d'impulsion d'un codeur
 * @param	: num_codeur	: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @return	: nombre d'impulsion codeur
 */
uint16_t RetourImpCodeur (int16_t num_codeur);


/**
 * @brief	: Fonction permettant l'acquistion du sens de rotation d'un codeur
 * @param	: num_codeur	: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @return	: sens de rotation du codeur (CODEUR_1_RECUL | CODEUR_1_AVANCE | CODEUR_2_RECUL | CODEUR_2_AVANCE)
 */
uint16_t RetourSensCodeur (int16_t num_codeur);


/**
 * @brief	: Fonction permettant de savoir si un overflow sur un codeur a eu lieu depuis le dernier appel de cette fonction
 * <br>		  et si oui le sens de rotation du codeur durant l'overflow
 * @param	: num_codeur	: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @return	: PAS_OVERFLOW si aucun overflow n'a eu lieu
 * <br>		  sinon le sens de rotation du codeur au moment de l'overflow (CODEUR_1_RECUL | CODEUR_1_AVANCE | CODEUR_2_RECUL | CODEUR_2_AVANCE)
 */
int16_t SensRotationPendantOverFlow (int16_t num_codeur);


/**
 * @brief	: Fonction permettant d'activer ou de descativer le module QEI
 * @param	: num_codeur				: identifiant du codeur (CODEUR_1 | CODEUR_2) ou (CODEUR_GAUCHE | CODEUR_DROIT)
 * @param	: activation_desactivation	: ON pour activer le module QEI, OFF pour le desactiver
 */
void GestionModuleQEI (int16_t num_codeur, int16_t activation_desactivation);


/**
 * @brief	: UNIQUEMENT POUR BOARD1 !
 * <br>		  Fonction permettant l'attente de la mise en place ou du retrait du jack
 * @param	: etat_jack_souhaite		: ATTENTE_MISE_JACK | ATTENTE_RETRAIT_JACK
 */
void AttenteJack (int8_t etat_jack_souhaite);

int retourCapteurI2c (int num, int offset_i2c);


#endif /* _CAPTEURS_H_ */
