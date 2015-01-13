/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: codeurs.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 9 novembre 2014, 02:39
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


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

   /* PutsUART(UART_XBEE, "X : [");
    PutLongUART((int32_t)(X.actuelle));
    PutsUART(UART_XBEE, "], Y : [");
    PutLongUART( (int32_t) Y.actuelle);
    PutsUART(UART_XBEE, "], Teta : [");
    PutLongUART((int32_t) ORIENTATION.actuelle);
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


    //debug distance
/*
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
        PutLongUART( (int32_t) (ERREUR_DISTANCE.maximum * 1000));*/
 



    //asserv
        PutLongUART( (int32_t) (ORIENTATION.consigne * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t) (ORIENTATION.actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (DISTANCE.consigne * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t) (DISTANCE.actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (VITESSE[SYS_ROBOT].theorique * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (VITESSE[ROUE_GAUCHE].consigne * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (VITESSE[ROUE_GAUCHE].actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (VITESSE[ROUE_DROITE].consigne * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (VITESSE[ROUE_DROITE].actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_GAUCHE].actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_GAUCHE].integralle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_DROITE].actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_VITESSE[ROUE_DROITE].integralle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t) (COMMANDE.droit));
        PutsUART(UART_XBEE, "\t");
        PutLongUART((int32_t) (COMMANDE.gauche));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_ORIENTATION.actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (ERREUR_DISTANCE.actuelle * 1000));
        PutsUART(UART_XBEE, "\t");
        PutLongUART( (int32_t) (FLAG_ASSERV.immobilite * 1000));
        PutsUART(UART_XBEE, "\t");
        PutsUART(UART_XBEE, "\n\r");
 


         //Consignes
      /*  PutLongUART(ORIENTATION.consigne);
        PutsUART(UART_XBEE, "\t");
        PutLongUART (ORIENTATION.actuelle);
        PutsUART(UART_XBEE, "\t");
        PutLongUART(X.consigne);
        PutsUART(UART_XBEE, "\t");
        PutLongUART (Y.consigne);
        PutsUART(UART_XBEE, "\t");
        PutLongUART(X.actuelle);
        PutsUART(UART_XBEE, "\t");
        PutLongUART (Y.actuelle);
        PutsUART(UART_XBEE, "\t");
        PutLongUART (ORIENTATION.consigne - ORIENTATION.actuelle);
        PutsUART(UART_XBEE, "\t");
        PutLongUART (DISTANCE.consigne);
        PutsUART(UART_XBEE, "\t");
        PutLongUART (Pi * ENTRAXE_TICKS/2);
        PutsUART(UART_XBEE, "\n\r");*/




     


}

