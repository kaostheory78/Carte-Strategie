/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: autom.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 01 février 2015, 17:11
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/***************************** FONCTIONS DIVERSES *****************************/
/******************************************************************************/

#ifdef GROS_ROBOT

void son_evitement (uint8_t melodie)
{
    Nop();
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10, NC, NC, NC);
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie, NC, NC, NC);
}

void rotation_us(void)
{
    static char sens = 0;
    static float angle = 0.;
   // if (read_data(AX_US, LIRE_MOUV_FLAG) == 0)
   // {
        if (sens == 0)
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle -= 2;
            if (angle < -32)
                sens = 1;
        }
        else
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle += 2;
            if (angle > 24)
            sens = 0;
        }
   // }
}


/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/


void chenilles(uint8_t action)
{
    if(action==DESCENDRE)
    {
        angle_AX12(CHENILLE_AV_G,885,52,AVEC_ATTENTE); //1023
        angle_AX12(CHENILLE_AV_D, 138,52,AVEC_ATTENTE); //0
        angle_AX12(CHENILLE_AR_G,885,52,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AR_D, 138,52,AVEC_ATTENTE);
        lancer_autom_AX12();
    }
    else
    {
        angle_AX12(CHENILLE_AV_G,512,52,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AV_D,512,52,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AR_G,512,52,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AR_D,512,52,AVEC_ATTENTE);
        lancer_autom_AX12();
    }
}

void pince(uint8_t cote,uint8_t action)
{
    if(cote==DROITE)
    {
       switch (action)
      {
        case FERMER:
           angle_AX12(PINCE_D,855,500,SANS_ATTENTE);
           break;
        case RANGEMENT:
           angle_AX12(PINCE_D,735,500,SANS_ATTENTE);
           break;
        case OUVERTE:
           angle_AX12(PINCE_D,980,500,SANS_ATTENTE);
           break;
       }
    }
   else
   {
        switch (action)
        {
        case RANGEMENT:
           angle_AX12(PINCE_G ,580,500,SANS_ATTENTE);
           break;
        case FERMER:
           angle_AX12(PINCE_G ,480,500,SANS_ATTENTE);
           break;
        case OUVERTE:
           angle_AX12(PINCE_G ,350,500,SANS_ATTENTE);
           break;
        }
    }
}

void tapis(uint8_t cote, uint8_t action)
{
    if(cote==DROITE)
    {
        switch (action)
        {
            case RANGEMENT:
                angle_AX12(PINCE_TAPIS_D,788,1023,SANS_ATTENTE);
                break;
            case OUVERT:
                angle_AX12(PINCE_TAPIS_D,585,1023,SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(PINCE_TAPIS_D,533,1023,SANS_ATTENTE);
                break;
        }
    }
    else
    {
        switch (action)
        {
            case RANGEMENT:
                angle_AX12(PINCE_TAPIS_G ,10,1023,SANS_ATTENTE);
                break;
            case OUVERT:
                angle_AX12(PINCE_TAPIS_G ,10,1023,SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(PINCE_TAPIS_G ,10,1023,SANS_ATTENTE);
                break;
        }
    }
}

void bras(uint8_t cote, uint8_t action)
{
    if(cote==DROITE)
    {
        switch (action)
        {
            case FERMER:
                angle_AX12(BRAS_DROIT,566,1023,SANS_ATTENTE);
                break;
            case OUVERTE:
                angle_AX12(BRAS_DROIT,800,1023,SANS_ATTENTE);
                break;
        }
    }
    else
    {
        switch (action)
        {
            case FERMER:
                angle_AX12(BRAS_GAUCHE,466,1023,SANS_ATTENTE);
                break;
            case OUVERTE:
                angle_AX12(BRAS_GAUCHE,209,1023,SANS_ATTENTE);
                break;
        }
    }
}

void ascenseur(uint8_t action)
{
        switch (action)
        {   
            case AVANT:
                angle_AX12(ASCENSEUR,190,1023,SANS_ATTENTE);
                break;
            case ARRIERE:
                angle_AX12(ASCENSEUR,1009,1023,SANS_ATTENTE);
                break;
        }
}


/******************************************************************************/
/**************************** FONCTIONS D'INITS *******************************/
/******************************************************************************/


void init_jack()
{
    pince(DROITE, RANGEMENT);
    pince(GAUCHE, RANGEMENT);
    chenilles(MONTER);

    bras(DROITE, FERMER);
    bras(GAUCHE, FERMER);

    delay_ms(1000);
    ascenseur(ARRIERE);
}

void init_depart()
{
    if (read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {
        pince(DROITE, OUVERTE);
        pince(GAUCHE, OUVERTE);
        FLAG_ACTION = ATTRAPE_GOBELET;
    }
}

/******************************************************************************/
/******************************** FONCTIONS AUTOM *****************************/
/******************************************************************************/

void attrape_gobelet ()
{
    static uint8_t statut_pince_D = LIBRE, status_pince_G = LIBRE;
    
    if (CAPT_GOBELET_D == 0 && statut_pince_D == LIBRE)
    {
        pince(DROITE, FERMER);
        statut_pince_D = FERMER;
    }
    if (CAPT_GOBELET_G == 0 && status_pince_G == LIBRE)
    {
        pince(GAUCHE, FERMER);
        status_pince_G = FERMER;
    }
}


/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{

    static uint16_t compteur = 0;
    static uint8_t  evitement_en_cours = OFF;

        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/

        //fonction qui definit les actions
        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            case INIT_ASCENSEUR :
                ascenseur(AVANT);
                FLAG_ACTION = INIT_DEPART;
                break;
            case INIT_DEPART :
                init_depart();
                break;
            case ATTRAPE_GOBELET :
                attrape_gobelet();
                break;
            default :
                break;
        }

        /**********************************************************************/
        /**************************** Evitement *******************************/
        /**********************************************************************/


        //Fonction permettant de lancer la fonction d'évitement
        if(EVITEMENT_ADV_AVANT == ON)
        {

            if(evitement_en_cours == ON){
                compteur_evitement++;
            }
            else {
                compteur_evitement =0;
            }

            if (  CAPT_US_BALISE == 1  && DETECTION == OFF)
            {
                compteur = 0;
                DETECTION = ON;
                evitement_en_cours = OFF;
                FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_US_BALISE == 0)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
                    {
                        evitement_en_cours = ON;
                        compteur = 0;
                        fin_deplacement();
                    }
                }
            }
            else
                rotation_us();
        }

        //Evitement arrière
        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if(evitement_en_cours == ON){
                compteur_evitement++;
            }
            else {
                compteur_evitement =0;
            }

            if ( (CAPT_IR_AR_CENTRE == 0 || CAPT_US_AR_DROIT == 0 || CAPT_US_AR_GAUCHE == 0)  && DETECTION == OFF)
            {
                compteur = 0;
                DETECTION = ON;
                evitement_en_cours = OFF;
                FLAG_ASSERV.erreur = EVITEMENT;
                brake();
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_IR_AR_CENTRE == 1 && CAPT_US_AR_DROIT == 1 && CAPT_US_AR_GAUCHE == 1)
                    {
                        DETECTION = OFF;
                        unbrake();
                    }
                }
            }
            else if (DETECTION == ON && (STRATEGIE_EVITEMENT == ACTION_EVITEMENT || STRATEGIE_EVITEMENT == EVITEMENT_NORMAL ))
            {
                if (evitement_en_cours == OFF)
                {
                    compteur ++;
                    if (compteur > 40)
                    {
                        evitement_en_cours = ON;
                        compteur = 0;
                        fin_deplacement();
                        son_evitement(30);
                    }
                     else
                     {
                        son_evitement(2);
                    }
                }
            }
        }
        else if (DETECTION == ON)
        {
            DETECTION = OFF;
            unbrake();
        }


}


#endif