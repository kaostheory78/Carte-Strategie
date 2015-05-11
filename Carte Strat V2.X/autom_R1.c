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
    //Nop();
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
            angle -= 3;
            if (angle < -60)
                sens = 1;
        }
        else
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle += 3;
            if (angle > 60)
            sens = 0;
        }
   // }
}

uint8_t inversion_autom (uint8_t cote)
{
    if (COULEUR == JAUNE)
        return cote;
    else
    {
        if (cote == DROIT)
            return GAUCHE;
        else
            return DROIT;
    }
}

uint8_t check_capteur (uint8_t cote)
{
    cote = inversion_autom(cote);

    if (cote == DROIT)
        return CAPT_GOBELET_D;
    else
        return CAPT_GOBELET_G;
}


/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/


void chenilles(uint8_t action)
{
    if(action==DESCENDRE)
    {
        angle_AX12(CHENILLE_AV_G,885,200,AVEC_ATTENTE); //1023
        angle_AX12(CHENILLE_AV_D, 138,200,AVEC_ATTENTE); //0
        angle_AX12(CHENILLE_AR_G,885,200,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AR_D, 138,200,AVEC_ATTENTE);
        lancer_autom_AX12();
    }
    else if (action == MONTER)
    {
        angle_AX12(CHENILLE_AV_G,512,200,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AV_D,512,200,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AR_G,512,200,AVEC_ATTENTE);
        angle_AX12(CHENILLE_AR_D,512,200,AVEC_ATTENTE);
        lancer_autom_AX12();
    }
    else
    {
        angle_AX12(CHENILLE_AV_G,662,200,SANS_ATTENTE); //1023
        angle_AX12(CHENILLE_AV_D, 362,200,SANS_ATTENTE); //0
        angle_AX12(CHENILLE_AR_G,812,200,SANS_ATTENTE);
        angle_AX12(CHENILLE_AR_D, 212,200,SANS_ATTENTE);
        //lancer_autom_AX12();
    }
}

void pince(uint8_t cote,uint8_t action)
{

    cote = inversion_autom(cote);
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
    cote = inversion_autom(cote);

    if(cote==DROITE)
    {
        switch (action)
        {
            case RANGEMENT:
                angle_AX12(PINCE_TAPIS_D, 745, 1023, SANS_ATTENTE);
                break;
            case OUVERT:
                angle_AX12(PINCE_TAPIS_D, 580, 1023, SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(PINCE_TAPIS_D, 532, 1023, SANS_ATTENTE);
                break;
        }
    }
    else
    {
        switch (action)
        {
            case RANGEMENT:
                angle_AX12(PINCE_TAPIS_G, 290, 1023, SANS_ATTENTE);
                break;
            case OUVERT:
                angle_AX12(PINCE_TAPIS_G, 455, 1023, SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(PINCE_TAPIS_G, 485, 1023, SANS_ATTENTE);
                break;
        }
    }
}

void bras(uint8_t cote, uint8_t action)
{
    cote = inversion_autom(cote);

    if(cote == DROITE)
    {
        switch (action)
        {
            case FERMER:
                angle_AX12(BRAS_DROIT,566,1023,SANS_ATTENTE);
                break;
            case OUVERTE:
                angle_AX12(BRAS_DROIT,700,1023,SANS_ATTENTE);
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
                angle_AX12(BRAS_GAUCHE,309,1023,SANS_ATTENTE);
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
                angle_AX12(ASCENSEUR,950,1023,SANS_ATTENTE);
                break;
        }
}

void monter_balise ()
{
    angle_AX12(BALISE_GAUCHE, 512, 250, AVEC_ATTENTE);
    angle_AX12(BALISE_DROITE, 512, 250, AVEC_ATTENTE);
    lancer_autom_AX12();
}

void descendre_balise (void)
{
    angle_AX12(BALISE_GAUCHE, 840, 80, AVEC_ATTENTE);
    angle_AX12(BALISE_DROITE, 166, 80, AVEC_ATTENTE);
    lancer_autom_AX12();
}


/******************************************************************************/
/**************************** FONCTIONS D'INITS *******************************/
/******************************************************************************/


void init_jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
    pince(DROITE, RANGEMENT);
    pince(GAUCHE, RANGEMENT);
    chenilles(MONTER);

    bras(DROITE, FERMER);
    bras(GAUCHE, FERMER);

    tapis(DROIT, RANGEMENT);
    tapis (GAUCHE, RANGEMENT);

    monter_balise();

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

void attrape_gobelet (uint8_t reinit)
{
    static uint8_t statut_pince_D = LIBRE, status_pince_G = LIBRE;

    if (reinit == ON)
    {
        status_pince_G = LIBRE;
        statut_pince_D = LIBRE;
    }
    
    if (check_capteur(DROIT) == 0 && statut_pince_D == LIBRE)
    {
        pince(DROITE, FERMER);
        statut_pince_D = FERMER;
    }
    if (check_capteur(GAUCHE) == 0 && status_pince_G == LIBRE)
    {
        pince(GAUCHE, FERMER);
        status_pince_G = FERMER;
    }
}

void montee_des_marches ()
{
    static uint8_t etat = 0;
    
    if (etat == 0)
    {
        chenilles(DESCENDRE);
        tapis(DROIT, OUVERT);
        tapis (GAUCHE, OUVERT);
        etat = 1;
    }
    else if (etat == 1 && read_data(CHENILLE_AR_D, LIRE_MOUV_FLAG) == 0 && read_data(CHENILLE_AR_G, LIRE_MOUV_FLAG) == 0 )
    {
        descendre_balise();
        alimenter_moteur_Y(ON, MARCHE_AVANT);
        etat = 2;
    }
    else if (etat == 2)
    {
        if (INCLINOMETRE == 1)
                etat = 3;
    }
    else if (etat == 3)
    {
        chenilles(INTERMEDIAIRE);
        ascenseur(AVANT);
        //if (read_data(CHENILLE_AV_G, LIRE_POSITION_CONSIGNE) == 662 && read_data(CHENILLE_AR_G, LIRE_POSITION_CONSIGNE) == 812)
        //{
         //   lancer_autom_AX12();
            COMPTEUR_MARCHE = 0;
            FLAG_ACTION = ARRIVEE_MARCHE;
        //}
    }
}

void arrive_marche ()
{
    static uint8_t etat = 0;
    if (COMPTEUR_MARCHE >= 1600 && etat == 0)
    {
        tapis(DROIT, DEPOSE);
        tapis(GAUCHE, DEPOSE);
        etat = 1;
    }
    else if (etat == 1 && COMPTEUR_MARCHE >= 2800)
    {
        alimenter_moteur_Y(OFF, MARCHE_AVANT);
        ascenseur(ARRIERE);
        chenilles(MONTER);
        tapis(DROIT, RANGEMENT);
        tapis(GAUCHE, RANGEMENT);
        FLAG_ACTION = NE_RIEN_FAIRE;
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
                attrape_gobelet(ON);
                FLAG_ACTION = ATTRAPE_GOBELET2;
                break;
            case ATTRAPE_GOBELET2 :
                attrape_gobelet(OFF);
                break;
            case MONTEE_MARCHE :
                montee_des_marches();
                break;
            case ARRIVEE_MARCHE :
                arrive_marche();
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
            if (  (CAPT_US_BALISE == 1 || CAPT_US_AV_DROIT == 0 || CAPT_US_AV_GAUCHE == 0)  && DETECTION == OFF )
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
                    if (CAPT_US_BALISE == 0 ||CAPT_US_AV_DROIT == 1 || CAPT_US_AV_GAUCHE == 1)
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
            //else
               //rotation_us();
        }

        //Evitement arrière
        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_US_ARRIERE == 1)  && DETECTION == OFF)
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
                    if (CAPT_US_ARRIERE == 0)
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