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

void jack()
{
    while(!SYS_JACK);
    while(SYS_JACK);
}

void eclairage_robot (void)
{
    static float pourcetage_eclairage = 5;
    static char sens = 0;
    if (sens == 0)
    {
        if (pourcetage_eclairage < 75)
        {
            pourcetage_eclairage += 0.5;
        }
        else
            sens = 1;
    }
    else
    {
        if (pourcetage_eclairage > 2)
        {
            pourcetage_eclairage -= 0.5;
        }
        else
            sens = 0;
    }

    if (pourcetage_eclairage > 100)
        pourcetage_eclairage = 100;
    else if (pourcetage_eclairage < 2)
        pourcetage_eclairage = 2;

    envoit_pwm(MOTEUR_X, (double) pourcetage_eclairage);
}

void rotation_us(void)
{
    static char sens = 0;
    static float angle = 0;
   // if (read_data(AX_US, LIRE_MOUV_FLAG) == 0)
   // {
        if (sens == 0)
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle -= 2;
            if (angle < -28)
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

void pince(uint8_t action)
{
    switch (action)
    {
        case FERMER:
            angle_AX12(PINCE,480,200,SANS_ATTENTE);
            break;
        case RANGEMENT:
            angle_AX12(PINCE,230,1023,SANS_ATTENTE);
            break;
        case OUVERTE:
            angle_AX12(PINCE,350,800,SANS_ATTENTE);
            break;
    }
}

void depose_tapis(uint8_t action, uint8_t cote)
{
    if(cote==DROITE){
        switch (action)
        {
            case EN_HAUT:
                angle_AX12(DEPOSE_TAPIS_D,820,100,SANS_ATTENTE);
                break;
            case EN_BAS:
                angle_AX12(DEPOSE_TAPIS_D,520,200,SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(DEPOSE_TAPIS_D,610,100,SANS_ATTENTE);
                break;
        }
    }else{        switch (action)
        {
            case EN_HAUT:
                angle_AX12(DEPOSE_TAPIS_G,500,100,SANS_ATTENTE);
                break;
            case EN_BAS:
                angle_AX12(DEPOSE_TAPIS_G,800,200,SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(DEPOSE_TAPIS_G,710,100,SANS_ATTENTE);
                break;
        }
    }
}


void pince_tapis(uint8_t action, uint8_t cote)
{
    if(cote==DROITE){
        switch (action)
        {
            case EN_HAUT:
                angle_AX12(PINCE_TAPIS_D,200,800,SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(PINCE_TAPIS_D,520,800,SANS_ATTENTE);
                break;
        }
    }else{        switch (action)
        {
            case EN_HAUT:
                angle_AX12(PINCE_TAPIS_G,500,800,SANS_ATTENTE);
                break;
            case DEPOSE:
                angle_AX12(PINCE_TAPIS_G,220,800,SANS_ATTENTE);
                break;
        }
    }
}


void attrape_gobelet (void)
{
    if (CAPT_GOBELET == 0)
    {
        pince(FERMER);
        FLAG_ACTION  = NE_RIEN_FAIRE;
    }
}

void init_jack()
{
    pince(RANGEMENT);
}

void init_depart()
{
    pince(OUVERTE);
   depose_tapis(EN_HAUT,DROITE);
    pince_tapis(EN_HAUT,DROITE);
    depose_tapis(EN_HAUT,GAUCHE);
    pince_tapis(EN_HAUT,GAUCHE);
}

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_10ms (void)
{

    static uint16_t compteur = 0;
    static uint8_t  evitement_en_cours = OFF;

    eclairage_robot();

        /**********************************************************************/
        /******************************* Autom ********************************/
        /**********************************************************************/

        //fonction qui definit les actions
        switch (FLAG_ACTION)
        {
            case NE_RIEN_FAIRE:
                break;
            case INIT_DEPART :
                init_depart();
                break;
            case ATTRAPE_GOBELET :
                attrape_gobelet();
                break;
            case INIT_JACK :
                init_jack();
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
           // else
                //rotation_us();
        }


        /*if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( DETECTION == OFF && (CAPT_US_ARRIERE_D == 1 || CAPT_US_ARRIERE_G == 1) )//Rajouter des capteurs IR ?
            {
                DETECTION = ON;
                STOP_DETECTION_AR = ON;
                brake();
            }
            else if (DETECTION == ON)
            {
                DETECTION = OFF;
                delay_ms(200);
                unbrake();
            }
        }*/


}


#endif