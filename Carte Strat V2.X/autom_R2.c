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

#ifdef PETIT_ROBOT

void jack()
{
    while(!SYS_JACK);
    while(SYS_JACK);
}

void allumer_pompes ()
{
    envoit_pwm(MOTEUR_X, 100);
}

void eteindre_pompe ()
{
    envoit_pwm(MOTEUR_X, 0);
}

void son_evitement (uint8_t melodie)
{
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10, NC, NC, NC);
    commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie, NC, NC, NC);
}

/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/

void pinces (uint8_t ID, uint8_t etat)
{
    if (ID == PINCE_ASCENSEUR)
    {
        switch (etat)
        {
            case FERMER :
                angle_AX12(ID, PINCE_ASC_FERME, 1023, SANS_ATTENTE);
                break;
            case RELACHE :
                angle_AX12(ID, PINCE_ASC_RELACHE, 1023, SANS_ATTENTE);
                break;
            case RACLETTE :
                angle_AX12(ID, PINCE_ASC_RACLETTE, 1023, SANS_ATTENTE);
                break;
            case RANGEMENT :
                angle_AX12(ID, PINCE_ASC_RANGE, 1023, SANS_ATTENTE);
                break;
        }
    }
    else 
    {
        switch (etat)
        {
            case FERMER :
                angle_AX12(ID, PINCES_FERME, 1023, SANS_ATTENTE);
                break;
            case RELACHE :
                angle_AX12(ID, PINCES_RELACHE, 1023, SANS_ATTENTE);
                break;
            case RANGEMENT :
                angle_AX12(ID, PINCES_RANGE, 1023, SANS_ATTENTE);
        }
    }
}

void ouvrir_bras (uint8_t bras)
{
    synchro_AX12(bras, 5, 1023, SANS_ATTENTE);
}

void fermer_bras (uint8_t bras)
{
    synchro_AX12(bras, -85, 1023, SANS_ATTENTE);
}

void ascenseur (uint8_t direction)
{
    if (direction == MONTER)
        angle_AX12(ASCENSEUR, ASC_HAUT, 1023, SANS_ATTENTE);
    else if (direction == DESCENDRE)
        angle_AX12(ASCENSEUR, ASC_BAS, 1023, SANS_ATTENTE);
    else if (direction == HAUTEUR_DEMMARAGE)
        angle_AX12(ASCENSEUR, ASC_DEMARAGE, 1023, SANS_ATTENTE);
    else if (direction == DEPOSE)
        angle_AX12(ASCENSEUR, ASC_DEPOSE, 1023, SANS_ATTENTE);
}

void init_pinces_jack()
{
    pinces(PINCE_HAUT, RANGEMENT);
    pinces(PINCE_MILIEU, RANGEMENT);
    pinces(PINCE_BAS, RANGEMENT);
    ascenseur(HAUTEUR_DEMMARAGE);
    while(read_data(ASCENSEUR, LIRE_MOUV_FLAG) != 0);
    pinces(PINCE_ASCENSEUR, RANGEMENT);
    init_balle();
    fermer_bras(BRAS_DROIT);
    fermer_bras(BRAS_GAUCHE);
}

void init_balle()
{
    angle_AX12(SYS_BALLE, INIT_BALLE, 1023, SANS_ATTENTE);
}

void init_pinces_demarage()
{
    static char etat_pince_demarage = 0;
    if (etat_pince_demarage == 0)
    {
        pinces(PINCE_HAUT, RELACHE);
        pinces(PINCE_MILIEU, RELACHE);
        pinces(PINCE_BAS, RELACHE);
        pinces(PINCE_ASCENSEUR, RACLETTE);
        etat_pince_demarage = 1;
    }
    else if (read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {
        ascenseur(DESCENDRE);
        FLAG_ACTION = ATTRAPE_PIEDS;
    }
}

void ejecter_balle()
{
    angle_AX12(SYS_BALLE, EJECTER_BALLE, 20, SANS_ATTENTE);
}

void depose_pieds ()
{
    static int occupation_pince = -1, etat_ascenceur = 0, etat_pince = 0;
    if (occupation_pince == -1 && CAPT_PINCE == 0)
    {
        occupation_pince = 1;
    }
    else if (occupation_pince == -1)
    {
        occupation_pince = 0;
    }

    if (occupation_pince == 1)
    {
        if (etat_ascenceur == 0)
        {
            ascenseur(DESCENDRE);
            etat_ascenceur = EN_DESCENTE;
        }
        else if (etat_ascenceur == EN_DESCENTE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_HAUT, RANGEMENT);
            pinces(PINCE_MILIEU, RANGEMENT);
            pinces(PINCE_BAS, RANGEMENT);
            pinces (PINCE_ASCENSEUR, RACLETTE);
            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
    else if (occupation_pince == 0)
    {
        if (etat_ascenceur == 0)
        {
            pinces(PINCE_ASCENSEUR, RELACHE);
            ascenseur (MONTER);
            etat_ascenceur = EN_MONTER;
        }
        else if (etat_ascenceur == EN_MONTER && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_ASCENSEUR, FERMER);
            etat_ascenceur = EN_HAUT;
        }
        else if (etat_ascenceur == EN_HAUT && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0 && etat_pince == 0)
        {
            pinces(PINCE_HAUT, RELACHE);
            pinces(PINCE_MILIEU, RELACHE);
            pinces(PINCE_BAS, RELACHE);
            etat_pince = EN_COURS;
        }
        else if (etat_pince == EN_COURS && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0)
        {
            ascenseur(DESCENDRE);
            etat_ascenceur = EN_DESCENTE;
            etat_pince = 0;
        }
        else if (etat_ascenceur == EN_DESCENTE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_ASCENSEUR, RACLETTE);
            pinces(PINCE_HAUT, RANGEMENT);
            pinces(PINCE_MILIEU, RANGEMENT);
            pinces(PINCE_BAS, RANGEMENT);

            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
}

void preparation_descente_pieds ()
{
    static int occupation_pince = -1, etat_ascenceur = 0, etat_pince = 0;
    ETAT_AUTOM = NE_RIEN_FAIRE;

    if (occupation_pince == - 1 && CAPT_PINCE == 0)
    {
        occupation_pince = 1;
    }
    else if (occupation_pince == -1)
    {
        occupation_pince = 0;
    }

    if (occupation_pince == 1)
    {
        //if (etat_ascenceur == 0)
        //{
        //   ascenseur(DESCENDRE);
        //   etat_ascenceur = EN_DESCENTE;
        //}
        //else if (etat_ascenceur == EN_DESCENTE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0 && etat_pince != EN_COURS)
        //{
        //    pinces(PINCE_HAUT, FERMER);
        //    pinces(PINCE_MILIEU, FERMER);
        //    pinces(PINCE_BAS, FERMER);
        //    pinces (PINCE_ASCENSEUR, FERMER);
        //    etat_pince = EN_COURS;
        //}
        //else if (etat_pince == EN_COURS && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        if (etat_pince == 0)
        {
            ascenseur(DEPOSE);//
            pinces(PINCE_BAS, RELACHE);
            pinces(PINCE_MILIEU, RELACHE);
            pinces(PINCE_HAUT, RELACHE);
            etat_pince = RELACHE;
            etat_ascenceur = DEPOSE;//
        }
        //else if (etat_pince == RELACHE && read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) == 0)
        //{
        //    ascenseur(DEPOSE);
        //    etat_pince = 0;
        //    etat_ascenceur = DEPOSE;
        //}
        else if (etat_ascenceur == DEPOSE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_HAUT, FERMER);
            pinces(PINCE_MILIEU, FERMER);
            pinces(PINCE_BAS, FERMER);
            
            //Reinit fonction
            occupation_pince = -1;
            etat_ascenceur = 0;
            etat_pince = 0;

            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
    else if (occupation_pince == 0)
    {
        if (etat_ascenceur == 0)
        {
            pinces(PINCE_ASCENSEUR, RELACHE);
            ascenseur (MONTER);
            etat_ascenceur = EN_MONTER;
        }
        else if (etat_ascenceur == EN_MONTER && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_ASCENSEUR, FERMER);
            etat_ascenceur = EN_HAUT;
        }
        else if (etat_ascenceur == EN_HAUT && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0 && etat_pince == 0)
        {
            pinces(PINCE_HAUT, RELACHE);
            pinces(PINCE_MILIEU, RELACHE);
            pinces(PINCE_BAS, RELACHE);
            etat_pince = EN_COURS;
        }
        else if (etat_pince == EN_COURS && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) == 0 && read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0)
        {
            ascenseur(DEPOSE);
            etat_ascenceur = DEPOSE;
            etat_pince = 0;
        }
        else if (etat_ascenceur == DEPOSE && read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            pinces(PINCE_HAUT, FERMER);
            pinces(PINCE_MILIEU, FERMER);
            pinces(PINCE_BAS, FERMER);

            //Reinit
            occupation_pince = -1;
            etat_ascenceur = 0;
            etat_pince = 0;

            FLAG_ACTION = NE_RIEN_FAIRE;
        }
    }
}

void depose_banc ()
{
    ejecter_balle();
    pinces(PINCE_ASCENSEUR, RACLETTE);
    pinces(PINCE_HAUT, RANGEMENT);
    pinces(PINCE_MILIEU, RANGEMENT);
    pinces(PINCE_BAS, RANGEMENT);
    while (read_data(PINCE_BAS, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_MILIEU, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_HAUT, LIRE_MOUV_FLAG) != 0 && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) != 0);
}

void attrape_gobelet()
{
    if (CAPT_PINCE== 0)
    {
        pinces(PINCE_BAS, RELACHE);
        pinces(PINCE_ASCENSEUR, FERMER);
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

void depose_gobelet()
{
    static int etat = 0;
    if (etat == 0)
    {
        etat = 1;
        pinces(PINCE_BAS, RANGEMENT);
        pinces(PINCE_ASCENSEUR, RACLETTE);
    }
    else if (read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {
        etat = 0;
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

void attrape_balle ()
{
    static int etat_ascenseur = 0;

    if (etat_ascenseur == 0)
    {
        pinces(PINCE_ASCENSEUR, FERMER);
        pinces(PINCE_BAS, FERMER);
        etat_ascenseur = 1;
    }
    else if (etat_ascenseur == 1 && read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0)
    {
        ascenseur(MONTER);

        etat_ascenseur = 0;
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

void faire_les_claps()
{
    static int etat_bras = ON;
    if (get_X() > 400.0 && get_X() < 640.0 && etat_bras == ON )
    {
        //fermer_bras(GAUCHE);
        angle_AX12(BRAS_DROIT, 200, 1023, SANS_ATTENTE);
        etat_bras = OFF;
    }
    else if (get_X() > 640 && etat_bras == OFF)
    {
        //ouvrir_bras(GAUCHE);
        angle_AX12(BRAS_DROIT, 512, 1023, SANS_ATTENTE);
        etat_bras = ON;
        FLAG_ACTION = NE_RIEN_FAIRE;
    }
}

void empilement(int taille_max)
{
    static uint8_t etat_pince_asc = LIBRE, etat_pince_haut = RANGEMENT, etat_ascenseur = HAUTEUR_DEMMARAGE, compteur_pieds = 0, retard = 0;

    if (etat_pince_asc == LIBRE && CAPT_PINCE == 0)
    {
        if ((compteur_pieds < taille_max))
        {
            compteur_pieds++;
            etat_pince_asc = EN_COURS;
        }
        else
            FLAG_ACTION = NE_RIEN_FAIRE;

        pinces(PINCE_ASCENSEUR, FERMER);
    }
    else if (etat_pince_asc == EN_COURS)
    {
        if (read_data(PINCE_ASCENSEUR, LIRE_MOUV_FLAG) == 0 )
        { 
            ascenseur(MONTER);
            etat_pince_asc = FERMER;
            etat_ascenseur = EN_MONTER;
            retard = 0;
            if (taille_max == 3)
            {
                pinces(PINCE_BAS, RELACHE);
                pinces(PINCE_MILIEU, RELACHE);
                pinces(PINCE_HAUT, RELACHE);
            }
            else
            {
                angle_AX12(PINCE_HAUT, 555, 1023, SANS_ATTENTE);
                angle_AX12(PINCE_MILIEU, 555, 1023, SANS_ATTENTE);
                angle_AX12(PINCE_BAS, 555, 1023, SANS_ATTENTE);
            }
        }
    }
    else if (etat_ascenseur == EN_MONTER)
    {
        if (read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            etat_ascenseur = MONTER;
            pinces(PINCE_BAS, FERMER);
            pinces(PINCE_MILIEU, FERMER);
            pinces(PINCE_HAUT, FERMER);
            etat_pince_haut = EN_COURS;
        }
    }
    else if (etat_pince_haut == EN_COURS)
    {
        if (read_data(PINCE_BAS, LIRE_MOUV_FLAG) == 0 )
        {
            etat_pince_haut = FERMER;
            pinces(PINCE_ASCENSEUR, RELACHE);
            etat_ascenseur = EN_DESCENTE;
            //delay_ms(10);
           ascenseur(DESCENDRE);
        }
    }
    else if (etat_ascenseur == EN_DESCENTE)
    {
        pinces(PINCE_ASCENSEUR, RACLETTE);
        if (read_data(ASCENSEUR, LIRE_MOUV_FLAG) == 0)
        {
            etat_pince_haut = RANGEMENT;
            etat_ascenseur = DESCENDRE;
            etat_pince_asc = LIBRE;
        }
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
            case ATTRAPE_PIEDS :
                empilement(3);
                break;
            case INIT_PINCES_DEMARRAGE :
                init_pinces_demarage();
                break;
            case DEPOSE_PIEDS :
                depose_pieds();
                break;
            case PREPARATION_DEPOSE_PIEDS :
                preparation_descente_pieds();
                break;
            case ATTRAPE_GOBELET :
                attrape_gobelet();
                break;
            case DEPOSE_GOBELET :
                depose_gobelet();
                break;
            case ATTRAPE_BALLE :
                attrape_balle();
                break;
            case CLAP :
                faire_les_claps();
                break;
            case ZONE_DEPART :
                empilement(5);
                break;
            case FERMETURE_PINCE :
                pinces(PINCE_ASCENSEUR, FERMER);
                FLAG_ACTION = NE_RIEN_FAIRE;
                break;
            case PIEDS_4 :
                pinces(PINCE_ASCENSEUR, RACLETTE);
                FLAG_ACTION = ATTRAPE_PIEDS;
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
            if ( (CAPT_US_GAUCHE == 1 || CAPT_US_BALISE == 1 || CAPT_US_DROIT == 1) )
            {
                if ( DETECTION == OFF)
                {
                    compteur = 0;
                    DETECTION = ON;
                    evitement_en_cours = OFF;
                    FLAG_ASSERV.erreur = EVITEMENT;
                    brake();
                    son_evitement(49);
                }
                else
                    son_evitement(2);
            }
            else if (DETECTION == ON && STRATEGIE_EVITEMENT == STOP)
            {
                compteur ++;
                if (compteur > 20)
                {
                    compteur = 20;
                    if (CAPT_US_GAUCHE == 0 && CAPT_US_BALISE == 0 && CAPT_US_DROIT == 0)
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
        }

        else if (EVITEMENT_ADV_ARRIERE == ON)
        {
            if ( (CAPT_IR_ARRIERE_CENTRE == 0 || CAPT_IR_ARRIERE_DROIT == 0 || CAPT_IR_ARRIERE_GAUCHE == 0)  && DETECTION == OFF)
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
                    if (CAPT_IR_ARRIERE_CENTRE == 1 && CAPT_IR_ARRIERE_DROIT == 1 && CAPT_IR_ARRIERE_GAUCHE == 1)
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
        }
        else if (DETECTION == ON)
        {
            DETECTION = OFF;
            unbrake();
        }
}

#endif