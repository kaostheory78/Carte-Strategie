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


#define MOTEUR_DROIT            0
#define MOTEUR_GAUCHE           1

#define PWM_MAX_VALUE           2046

#define PWM_DROIT_VALEUR_MAX    (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR_DROIT / TENSION_SORTIE_PTN )
#define PWM_GAUCHE_VALEUR_MAX   (uint16_t) ( PWM_MAX_VALUE * TENSION_MOTEUR_GAUCHE / TENSION_SORTIE_PTN )

#define AVANCER_MOTEUR_D        _AVANCER_MOTEUR_D
#define RECULER_MOTEUR_D        _RECULER_MOTEUR_D

#define AVANCER_MOTEUR_G        _AVANCER_MOTEUR_G
#define RECULER_MOTEUR_G        _RECULER_MOTEUR_G

/******************************************************************************/
/***************************** MAPPING ****************************************/
/******************************************************************************/

#ifdef CARTE_V1
    #define REGISTRE_MOTEUR_D   PDC3
    #define REGISTRE_MOTEUR_G   PDC2
#endif

#ifdef CARTE_V2
    #define REGISTRE_MOTEUR_D   PDC2
    #define REGISTRE_MOTEUR_G   PDC3
    #define REGISTRE_MOTEUR_X   PDC1
#endif



    void ConfigPWM (void);
    void envoit_pwm (char moteur, int32_t valeur);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

