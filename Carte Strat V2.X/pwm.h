/* 
 * File:   pwm.h
 * Author: Quentin
 *
 * Created on 19 novembre 2014, 14:16
 */

#ifndef PWM_H
#define	PWM_H

#include "system.h"


#ifdef	__cplusplus
extern "C" {
#endif

    
/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"


/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

    typedef enum 
    {
        MOTEUR_DROIT,
        MOTEUR_GAUCHE,
        MOTEUR_X
    }_enum_moteur;  

#define PWM_MAX_VALUE           2046

#define TENSION_MOTEUR_X        _TENSION_MOTEUR_X

#define PWM_DROIT_VALEUR_MAX    (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR_DROIT / TENSION_SORTIE_PTN )
#define PWM_GAUCHE_VALEUR_MAX   (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR_GAUCHE / TENSION_SORTIE_PTN )
#define PWM_X_VALEUR_MAX        (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR_X / TENSION_SORTIE_PTN )

#define AVANCER_MOTEUR_D        _AVANCER_MOTEUR_D
#define RECULER_MOTEUR_D        _RECULER_MOTEUR_D

#define AVANCER_MOTEUR_G        _AVANCER_MOTEUR_G
#define RECULER_MOTEUR_G        _RECULER_MOTEUR_G

#define AVANCER_MOTEUR_X        _AVANCER_MOTEUR_X
#define RECULER_MOTEUR_X        _RECULER_MOTEUR_X

#define AVANCER_MOTEUR_Y        _AVANCER_MOTEUR_Y
#define RECULER_MOTEUR_Y        _RECULER_MOTEUR_Y

/******************************************************************************/
/***************************** MAPPING ****************************************/
/******************************************************************************/

#ifdef CARTE_V1
    #define REGISTRE_MOTEUR_D   PDC3
    #define REGISTRE_MOTEUR_G   PDC2
#endif

#ifdef CARTE_BALISE
    #define REGISTRE_MOTEUR_D   PDC1
    #define REGISTRE_MOTEUR_G   PDC2
#endif
    
#ifdef CARTE_V2
    #define REGISTRE_MOTEUR_D   PDC2
    #define REGISTRE_MOTEUR_G   PDC3
    #define REGISTRE_MOTEUR_X   PDC1
#endif



    void ConfigPWM (void);
    void envoit_pwm (char moteur, double valeur);

#ifdef CARTE_V2
    void alimenter_moteur_Y (uint8_t alim, int8_t sens_marche);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

