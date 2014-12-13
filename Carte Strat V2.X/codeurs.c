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


    int OVERFLOW_CODEUR[3] ;//= { PAS_D_OVERFLOW_CODEUR, PAS_D_OVERFLOW_CODEUR };
    _position position[3] ;// = {{0, 0}, {0, 0}};


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
        QEI1CONbits.QEIM        = 0b111;                        // Mode QEI avec X4
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
        QEI2CONbits.QEIM        = 0b111;                        // Mode QEI avec X4
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
    static int32_t res = 0;
    static uint16_t max_codeur;
    int etat_overflow, sens_rotation;

    if (codeur == CODEUR_D)
    {
        position [codeur].nouvelle = POS1CNT;
        max_codeur = MAX_CODEUR_D;
        sens_rotation = SENS_ROT_D;
    }
    else
    {
        position [codeur].nouvelle = POS2CNT;
        max_codeur = MAX_CODEUR_G;
        sens_rotation = SENS_ROT_G;
    }
        

    etat_overflow = OVERFLOW_CODEUR [codeur];
    OVERFLOW_CODEUR [codeur] = PAS_D_OVERFLOW_CODEUR;

    res = (int32_t)( (int32_t) position[codeur].nouvelle - (int32_t) position[codeur].ancien );
    if (etat_overflow != PAS_D_OVERFLOW_CODEUR)
    {
        res+= (int32_t) etat_overflow * max_codeur;
    }

    position [codeur].ecart = res * RESOLUTION_LOGICIELLE * sens_rotation;
    position[codeur].ancien = position[codeur].nouvelle;
}


void debug ()
{
    static int compteur = 0;
    compteur++;
    /*PutcUART(UART_XBEE, '[');
    PutLongUART (position[CODEUR_D].totale);
    PutcUART(UART_XBEE, ']');
    PutcUART(UART_XBEE, '[');
    PutLongUART (position[CODEUR_G].totale);
    PutcUART(UART_XBEE, ']');
    //PutcUART(UART_XBEE, '\r');
    PutcUART(UART_XBEE, '[');
    PutLongUART (position[CODEUR_D].ecart);
    PutcUART(UART_XBEE, ']');
    PutcUART(UART_XBEE, '[');
    PutLongUART (position[CODEUR_G].ecart);
    PutcUART(UART_XBEE, ']');
    PutsUART(UART_XBEE, "\r\n");*/

  /*  PutsUART(UART_XBEE, "X : [");
    PutLongUART((int32_t)(ROBOT.X_mm));
    PutsUART(UART_XBEE, "], Y : [");
    PutLongUART( (int32_t) ROBOT.Y_mm);
    PutsUART(UART_XBEE, "], Teta : [");
    PutLongUART((int32_t)ROBOT.orientation_degre);
    PutsUART(UART_XBEE, "] \r");*/


   
/*
        PutsUART(UART_XBEE, "Vcons ");
        PutLongUART( (int32_t) (VITESSE[ROUE_GAUCHE].consigne * 1000));
        PutsUART(UART_XBEE, " Vg \t");
        PutLongUART((int32_t) (VITESSE[ROUE_GAUCHE].actuelle * 1000));
        PutsUART(UART_XBEE, " e act\t ");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_GAUCHE].actuelle * 1000));
        PutsUART(UART_XBEE, " e int \t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_GAUCHE].integralle * 1000));
        PutsUART(UART_XBEE, " e der ");
        PutLongUART( (int32_t) ((ERREUR_VITESSE[ROUE_GAUCHE].actuelle - ERREUR_VITESSE[ROUE_GAUCHE].precedente) * 1000));
        PutsUART(UART_XBEE, " mot :");
        PutLongUART((int32_t) (COMMANDE.gauche));
        PutsUART(UART_XBEE, " d cons ");
        PutLongUART((int32_t)(DISTANCE.consigne));
        PutsUART(UART_XBEE, " d act ");
        PutLongUART((int32_t)(DISTANCE.actuelle));
        PutsUART(UART_XBEE, " ed act ");
        PutLongUART( (int32_t) (ERREUR_DISTANCE.actuelle * 1000));
        PutsUART(UART_XBEE, " pid ");
        PutLongUART( (int32_t) ((ERREUR_DISTANCE.actuelle - ERREUR_DISTANCE.precedente) * 1000));
                PutsUART(UART_XBEE, " pid ");
        PutLongUART( (int32_t) (ERREUR_DISTANCE.maximum * 1000));*/


        PutLongUART( (int32_t) (VITESSE[ROUE_GAUCHE].consigne * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t) (VITESSE[ROUE_GAUCHE].actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_GAUCHE].actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_GAUCHE].integralle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) ((ERREUR_VITESSE[ROUE_GAUCHE].actuelle - ERREUR_VITESSE[ROUE_GAUCHE].precedente) * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t) (COMMANDE.gauche));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t)(DISTANCE.consigne));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t)(DISTANCE.actuelle));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_DISTANCE.actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) ((ERREUR_DISTANCE.actuelle - ERREUR_DISTANCE.precedente) * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_DISTANCE.maximum * 1000));




        //PutsUART(UART_XBEE, "], VIT : [");
    //    PutLongUART((int32_t) VITESSE[ROBOT].theorique);
        PutsUART(UART_XBEE, "\n\r");
    

}