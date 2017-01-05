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


#define CODEUR_1                        1U
#define CODEUR_2                        2U

#define CODEUR_D                        CODEUR_1
#define CODEUR_G                        CODEUR_2


#define SENS_ROT_D                      _SENS_ROT_D
#define SENS_ROT_G                      _SENS_ROT_G

#define CODEUR_D_NB_IMP                 _CODEUR_D_NB_IMP
#define CODEUR_G_NB_IMP                 _CODEUR_G_NB_IMP

#define RAPPORT_REDUCTION               _RAPPORT_REDUCTION

#define RESOLUTION_LOGICIELLE           _RESOLUTION_LOGICIELLE


#define CODEUR_D_NB_IMP_LOG             (RESOLUTION_LOGICIELLE * 4U * CODEUR_D_NB_IMP * RAPPORT_REDUCTION)

#define MAX_CODEUR_D                    (4U * CODEUR_D_NB_IMP * RAPPORT_REDUCTION - 1U)
#define MAX_CODEUR_G                    (4U * CODEUR_G_NB_IMP * RAPPORT_REDUCTION - 1U)
#define MAX_CODEUR_D_LOGICIEL           (RESOLUTION_LOGICIELLE * CODEUR_D_NB_IMP * 4U * RAPPORT_REDUCTION - 1U)
#define MAX_CODEUR_G_LOGICIEL           (RESOLUTION_LOGICIELLE * CODEUR_G_NB_IMP * 4U * RAPPORT_REDUCTION - 1U)

#define PAS_D_OVERFLOW_CODEUR           0

    typedef struct _codeur
    {
        uint16_t ancien;
        uint16_t nouvelle;
        double ecart;
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

