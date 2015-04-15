/* 
 * File:   codeurs.h
 * Author: Quentin
 *
 * Created on 9 novembre 2014, 01:09
 */

#ifndef CODEURS_H
#define	CODEURS_H

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


#define CODEUR_1                        1L
#define CODEUR_2                        2L

#define CODEUR_D                        CODEUR_1
#define CODEUR_G                        CODEUR_2


#define SENS_ROT_D                      _SENS_ROT_D
#define SENS_ROT_G                      _SENS_ROT_G

#define CODEUR_D_NB_IMP                 _CODEUR_D_NB_IMP
#define CODEUR_G_NB_IMP                 _CODEUR_G_NB_IMP

#define RAPPORT_REDUCTION               _RAPPORT_REDUCTION

#define RESOLUTION_LOGICIELLE           _RESOLUTION_LOGICIELLE


#define CODEUR_D_NB_IMP_LOG             (RESOLUTION_LOGICIELLE * 4 * CODEUR_D_NB_IMP * RAPPORT_REDUCTION)

#define MAX_CODEUR_D                    4 * CODEUR_D_NB_IMP - 1 * RAPPORT_REDUCTION
#define MAX_CODEUR_G                    4 * CODEUR_G_NB_IMP - 1 * RAPPORT_REDUCTION
#define MAX_CODEUR_D_LOGICIEL           (RESOLUTION_LOGICIELLE * CODEUR_D_NB_IMP * 4 * RAPPORT_REDUCTION - 1)
#define MAX_CODEUR_G_LOGICIEL           (RESOLUTION_LOGICIELLE * CODEUR_G_NB_IMP * 4 * RAPPORT_REDUCTION - 1)

#define PAS_D_OVERFLOW_CODEUR           0

#define STANDART                        0
#define INTER_CODEUR_D_OVERFLOW         1
#define INTER_CODEUR_D_UNDERFLOW        2
#define INTER_CODEUR_G_OVERFLOW         3
#define INTER_CODEUR_G_UNDERFLOW        4



    typedef struct _codeur
    {
        uint16_t ancien;
        uint16_t nouvelle;
        int32_t ecart;
    }_position;


    
/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    /**
     * Configuration des modules codeurs de la carte
     */
     void ConfigQEI (void);


     /**
      *
      * Fonction qui renvoit la valeur totale des codeurs
      *
      * @param codeur : codeur à étudier
      * @return : valeur totale des codeurs
      */
     void get_valeur_codeur (int codeur);


     void debug(void);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


#ifdef	__cplusplus
}
#endif

#endif	/* CODEURS_H */

