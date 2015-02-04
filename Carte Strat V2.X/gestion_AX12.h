/* 
 * File:   gestion_AX12.h
 * Author: Quentin
 *
 * Created on 28 janvier 2015, 13:23
 */

#ifndef GESTION_AX12_H
#define	GESTION_AX12_H

#ifdef	__cplusplus
extern "C" {
#endif


/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/



/******************************************************************************/
/****************************** DEFINES GLOBALES ******************************/
/******************************************************************************/

#define START_BYTE              0xFF
#define TIME_LIMIT              3000000L//740000L//100000L

#define PAS_D_ERREUR            0
#define TIME_OUT                1

#define EMISSION                1
#define RECEPTION               0

#define MAX_TENTATIVES          5

#define ETEINT                  0
#define ALLUME                  1

#define EN_COURS_ENVOIT         0
#define ENVOIT_FINI             1


#define _ID                     2
#define LONGUEUR                3
#define INSTRUCTION             4
#define PARAM1                  5
#define PARAM2                  6
#define PARAM3                  7
#define PARAM4                  8
#define PARAM5                  9
#define CHSUM                   10


#define ERREUR_CS               4


        //instructions à envoyer
#define NC                  0x00
#define PING                0x01
#define READ_DATA           0x02
#define WRITE_DATA          0x03
#define REG_WRITE           0x04
#define ACTION              0x05
#define RESET_AX            0x06

    //nb de paramètres
#define _2PARAM             0x02
#define _3PARAM             0x03
#define _4PARAM             0x04
#define _5PARAM             0x05
#define _6PARAM             0x06
#define _ALL_PARAM          0x07


//types de données à lire :
#define LIRE_TEMPS_REPONSE  0x05
#define LIRE_POSITION_ACTU  0x24
#define LIRE_VITESSE_ACTU   0x26
#define LIRE_TENSION        0x2A
#define LIRE_TEMPERATURE    0x2B
#define LIRE_MOUV_FLAG      0x2E
    //flag paramètres fonctions
#define SANS_ATTENTE        0
#define AVEC_ATTENTE        1

#define TOUTES_LES_INFOS    2
#define READ_ONLY           1
#define AUCUNE_INFO         0


    //paramètres rotation (roue libre)
#define MODE_INFINI         5
#define MODE_NORMAL         6

#define ANTI_HORAIRE        0
#define HORAIRE             1


    //paramètres réglage décalage AX12
#define DEPENDANT           1
#define INDEPENDANT         0

#define PAS_DE_SYMETRIQUE   0

#define SENS_INDIRECT       -1
#define ROT_EN_BAS          -1
#define SENS_DIRECT         1
#define ROT_EN_HAUT         1
#define AUCUN_AX            -30

    //valeurs par defaut AX12
#define POINT_MINI_DEFAUT   0
#define POINT_MAXI_DEFAUT   1023

    //ID AX12
#define TOUS_LES_AX12       0xFE
#define ID_MAX_AX12         25

    //Paramètres spéciaux
#define POSITION_MAX_AX     1024.
#define ANGLE_MAX_AX        300.
#define PAS_DE_CORRECTION_ANGLE_FIN 3000

    //defines vitesses de com
#define _9600b              0x01
#define _19200b             0x03
#define _57600b             0x04
#define _115200b            0x07
#define _200000b            0x09
#define _250000b            0x10
#define _400000b            0x22
#define _500000b            0x67
#define _1000000b           0xCF





/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

    typedef struct
    {
        uint8_t offset;
        uint8_t buffer[CHSUM];
        uint8_t nb_octet_attente;
        uint32_t timer;
        uint8_t erreur;
        uint8_t tentatives;
        uint8_t etat_uart;

    }_ax12;


/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void reception_uart_ax12 (void);
    void traitement_reception_ax12 ();
    void reinit_buffer (void);
    void commande_AX12 (uint8_t ID, uint8_t longueur, uint8_t instruction, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t param5);
    uint8_t calcul_checksum (uint8_t ID, uint8_t longueur, uint8_t instruction, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t param5);
    void angle_AX12 (uint8_t ID,  uint16_t position, uint16_t vitesse, uint8_t ATTENTE );
    void lancer_autom_AX12 (void);
    void rotation_AX12 (uint8_t ID, uint8_t sens, uint16_t vitesse);
    void configurer_status_returning_level (uint8_t ID, uint8_t type_de_retour);
    void reset_AX12 (uint8_t ID);
    void changer_ID_AX12 (uint8_t ancien_ID, uint8_t nouveau_ID);
    void baud_AX12 (uint8_t ID, uint8_t bauds);
    void mode_rotation_AX12 (uint8_t ID, uint8_t mode);
    void allumer_LED_AX12 (uint8_t ID);
    void eteindre_LED_AX12 (uint8_t ID);






#ifdef	__cplusplus
}
#endif

#endif	/* GESTION_AX12_H */

