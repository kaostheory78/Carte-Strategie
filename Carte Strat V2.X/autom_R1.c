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
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x29, 10);
    //commande_AX12(100, _4PARAM, WRITE_DATA, 0x28, melodie);
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
            angle -= 6;
            if (angle < -60)
                sens = 1;
        }
        else
        {
            synchro_AX12(AX_US, angle, 1023, SANS_ATTENTE);
            angle += 6;
            if (angle > 60)
            sens = 0;
        }
   // }
}

void rotation_us_avant ()
{
    static char sens_D = 0, sens_G = 0;
    static uint16_t position_D = 0, position_G = 0;

    if (sens_D == 0)
    {
        position_D += 10;
        if (position_D > 876)
            sens_D = 1;
    }
    else
    {
        position_D -= 10;
        if (position_D < 703)
            sens_D = 0;
    }

    if (sens_G == 0)
    {
        position_G += 10;
        if (position_G > 666)
            sens_G = 1;
    }
    else
    {
        position_G -= 10;
        if (position_G < 512)
            sens_G = 0;
    }

    angle_AX12(AX_US_DROIT, position_D, 1023, SANS_ATTENTE);
    angle_AX12(AX_US_GAUCHE, position_G, 1023, SANS_ATTENTE);
}

//uint8_t inversion_autom (uint8_t cote)
//{
//    if (COULEUR == JAUNE)
//        return cote;
//    else
//    {
//        if (cote == DROIT)
//            return GAUCHE;
//        else if (cote == GAUCHE)
//            return DROIT;
//        else
//            return cote;
//    }
//}
//
//uint8_t check_capteur (uint8_t cote)
//{
//    cote = inversion_autom(cote);
//
//    if (cote == DROIT)
//        return CAPT_GOBELET_D;
//    else if (cote == GAUCHE)
//        return CAPT_GOBELET_G;
//    else
//        return 1;
//}


/******************************************************************************/
/********************************  FONCTION AX12  *****************************/
/******************************************************************************/



/******************************************************************************/
/**************************** FONCTIONS D'INITS *******************************/
/******************************************************************************/


void init_jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
}

void init_depart()
{
    Nop();
}

/******************************************************************************/
/******************************** FONCTIONS AUTOM *****************************/
/******************************************************************************/

/******************************************************************************/
/******************************** FONCTION BOUCLE *****************************/
/******************************************************************************/


void autom_20ms (void)
{

    //fonction qui definit les actions
    switch (FLAG_ACTION)
    {
        case NE_RIEN_FAIRE:
            break;
        default :
            break;
    }
    
    if (EVITEMENT_ADV.actif == true && EVITEMENT_ADV.detection == OFF)
    {
        if (EVITEMENT_ADV.sens == MARCHE_AVANT)
            rotation_us_avant();
        else
            rotation_us();
    }
    //else
        //position standart
    
}


#endif