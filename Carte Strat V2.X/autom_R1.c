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

void turbine_mode_soufflage()
{
    angle_AX12(AX_TURBINE, TURBINE_SOUFLLE_POS, 1023, SANS_ATTENTE);
    
    while (ax12.erreur != PAS_D_ERREUR)
    {
        delay_ms(100);
        angle_AX12(AX_TURBINE, TURBINE_SOUFLLE_POS, 1023, SANS_ATTENTE);
    }
}

void turbine_mode_aspiration()
{
    angle_AX12(AX_TURBINE, TURBINE_ASPIRE_POS, 1023, SANS_ATTENTE);
    
    while (ax12.erreur != PAS_D_ERREUR)
    {
        delay_ms(100);
       angle_AX12(AX_TURBINE, TURBINE_ASPIRE_POS, 1023, SANS_ATTENTE);
    }
}

void bite_aspiration()
{
    angle_AX12(AX_BIELE, BITE_EN_BAS, 1023, SANS_ATTENTE); // 1023
    
    while (ax12.erreur != PAS_D_ERREUR)
    {
        delay_ms(100);
        angle_AX12(AX_BIELE, BITE_EN_BAS, 1023, SANS_ATTENTE);
    }
}

void bite_soufflage()
{
    angle_AX12(AX_BIELE, BITE_DEPOSE, 40, SANS_ATTENTE);
    
    while (ax12.erreur != PAS_D_ERREUR)
    {
        delay_ms(100);
        angle_AX12(AX_BIELE, BITE_DEPOSE, 40, SANS_ATTENTE);
    }
}

void bite_init()
{
    angle_AX12(AX_BIELE, BITE_EN_HAUT, 1023, SANS_ATTENTE);
    
    while (ax12.erreur != PAS_D_ERREUR)
    {
        delay_ms(100);
        angle_AX12(AX_BIELE, BITE_EN_HAUT, 1023, SANS_ATTENTE);
    }
}

void allumer_turbine()
{
//    if (_SYS_STRAT == 1)
//    {
    envoit_pwm(MOTEUR_X, 100);
//    } 
}

void eteindre_turbine()
{
    envoit_pwm(MOTEUR_X, 0);
}

void init_fusee()
{
    angle_AX12(AX_FUSEE, FUSEE_VEROUILLEE, 1023, SANS_ATTENTE);
}

void launch_fusee()
{
    angle_AX12(AX_FUSEE, FUSEE_OUVERTE, 30, SANS_ATTENTE);
}

/******************************************************************************/
/**************************** FONCTIONS D'INITS *******************************/
/******************************************************************************/


void init_jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
}

void jack()
{
    allumer_LED_AX12(TOUS_LES_AX12);
    torque_enable_ax12(TOUS_LES_AX12, true);

    while(!SYS_JACK);
    
    init_fusee();
    bite_init();
    
    delay_ms(500);
       
    while(SYS_JACK);
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
//    _autom_id autom_id;

    check_autom_events();
    
    //fonction qui definit les actions
    switch (FLAG_ACTION[AUTOM_PRINCIPALE])
    {
        case NE_RIEN_FAIRE:
        case EN_ATTENTE_EVENT :
            // do nothing
            break;

        default :
            break;
    }
    
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#endif