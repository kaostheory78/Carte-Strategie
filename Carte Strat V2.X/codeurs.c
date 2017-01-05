/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: codeurs.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 9 novembre 2014, 01:07
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


    volatile __attribute__((near)) int8_t OVERFLOW_CODEUR[3] ;//= { PAS_D_OVERFLOW_CODEUR, PAS_D_OVERFLOW_CODEUR };
    volatile __attribute__((near)) _position POSITION[3] ;// = {{0, 0}, {0, 0}};


/******************************************************************************/
/*************************** Configurations des QEI ***************************/
/******************************************************************************/

void ConfigQEI (void)
{


    /**************************************************************************/
    /************************  Codeur Droit ***********************************/
    /**************************************************************************/

	// QEI1CON
	QEI1CONbits.CNTERR	= 0;				// Reset flag erreur sur compteur
	QEI1CONbits.QEISIDL	= 0;				// Mode continu en idle
    QEI1CONbits.QEIM    = 0b111;            // Mode QEI avec X4
	QEI1CONbits.SWPAB	= 0;				// PA et PB non entrelacés
	QEI1CONbits.PCDOUT	= 1;				// Position counter dir. status output active (IO controle par QEI)

	// DFLT1CON
	DFLT1CONbits.IMV1	= 0;				// Etat de PB lors d'un INDEX
	DFLT1CONbits.IMV0	= 0;				// Etat de PA lors d'un INDEX
	DFLT1CONbits.CEID	= 0;				// Interruption sur erreur compteur desactive
	DFLT1CONbits.QEOUT	= 1;				// Activation des filtres
	DFLT1CONbits.QECK	= 0b010;			// Filtre / de 1:4

	MAX1CNT			= MAX_CODEUR_D;                 // Limite du compteur codeur




    /**************************************************************************/
    /************************  Codeur Gauche **********************************/
    /**************************************************************************/

	// QEI2CON
	QEI2CONbits.CNTERR	= 0;				// Reset flag erreur sur compteur
	QEI2CONbits.QEISIDL	= 0;				// Mode continu en idle
    QEI2CONbits.QEIM    = 0b111;            // Mode QEI avec X4
	QEI2CONbits.SWPAB	= 0;				// PA et PB non intervertis
	QEI2CONbits.PCDOUT	= 1;				// Position counter dir. status output active (IO controle par QEI)

	// DFLT2CON
	DFLT2CONbits.IMV1	= 0;				// Etat de PB lors d'un INDEX
	DFLT2CONbits.IMV0	= 0;				// Etat de PA lors d'un INDEX
	DFLT2CONbits.CEID	= 0;				// Interruption sur erreur compteur desactivee
	DFLT2CONbits.QEOUT	= 1;				// Activation des filtres
	DFLT2CONbits.QECK	= 0b010;			// Filtre / de 1:4

	MAX2CNT			= MAX_CODEUR_G;                 // Limite du compteur codeur
}




/******************************************************************************/
/*************************** Utilisation des codeurs **************************/
/******************************************************************************/



void get_valeur_codeur (int codeur)
{
    static int32_t res = 0L;
    static uint16_t max_codeur;
    int8_t etat_overflow, sens_rotation;



    etat_overflow = OVERFLOW_CODEUR [codeur];
    OVERFLOW_CODEUR [codeur] = PAS_D_OVERFLOW_CODEUR;

    if (codeur == CODEUR_D)
    {
        POSITION [codeur].nouvelle = POS1CNT;
        max_codeur = MAX_CODEUR_D;
        sens_rotation = SENS_ROT_D;
    }
    else
    {
        POSITION [codeur].nouvelle = POS2CNT;
        max_codeur = MAX_CODEUR_G;
        sens_rotation = SENS_ROT_G;
    }

    res = (int32_t)( (int32_t) POSITION[codeur].nouvelle - (int32_t) POSITION[codeur].ancien );
    if (etat_overflow != PAS_D_OVERFLOW_CODEUR)
    {
        res+= (int32_t) etat_overflow * max_codeur;
    }

    POSITION[codeur].ecart = (double) (res * RESOLUTION_LOGICIELLE * (int32_t) (sens_rotation) );
    POSITION[codeur].ancien = POSITION[codeur].nouvelle;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/