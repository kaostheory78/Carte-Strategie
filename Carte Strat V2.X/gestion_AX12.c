/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: gestion_AX12.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 28 janvier 2015, 13:20
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h" 


_ax12 ax12;

void reception_uart_ax12 (void)
{
    int erreur = 0;
    uint8_t buf = U2RXREG;

    // On attend des données
    if (ax12.nb_octet_attente > 0)
    {
            ax12.buffer[ax12.offset] = buf;
            ax12.offset++;

            // Si les 1ers octets sont mauvais
            if ((ax12.offset >= 1) && (ax12.buffer[0] != START_BYTE))
                erreur = 1;
            if ((ax12.offset >= 2) && (ax12.buffer[1] != START_BYTE))
                erreur = 1;

            // En cas d'erreur on reset
            if (erreur == 1)
            {
                ax12.offset = 0;
            }
    }
}

void traitement_reception_ax12 ()
{
    while (ax12.offset < ax12.nb_octet_attente)
    {
        ax12.timer++;
        if (ax12.timer > TIME_LIMIT)
        {
            ax12.erreur = TIME_OUT;
            PutsUART(UART_XBEE, "TIME OUT \r\n");
            break;
        }
    }

    if (ax12.erreur != TIME_OUT)
    {
        ax12.erreur = ax12.buffer[4];
        PutsUART(UART_XBEE, "test \n\r");

        uint8_t checksum = calcul_checksum(ax12.buffer[_ID], ax12.buffer[LONGUEUR], ax12.erreur, ax12.buffer[PARAM1], ax12.buffer[PARAM2], ax12.buffer[PARAM3], ax12.buffer[PARAM4], ax12.buffer[PARAM5]);

        ax12.buffer[CHSUM] = ax12.buffer[ax12.offset - 1];

        if (checksum != ax12.buffer[CHSUM])
        {
            ax12.erreur = ERREUR_CS;
            PutsUART(UART_XBEE, "ERREUR Checksum \r\n");
        }
    }


}

void reinit_buffer (void)
{
    int i;
    for ( i = 0 ; i <= CHSUM ; i++)
    {
         ax12.buffer[i] = 0;
    }
    ax12.timer = 0;
    ax12.offset = 0;
    ax12.erreur = PAS_D_ERREUR;
    ax12.nb_octet_attente = 0;
}

void commande_AX12 (uint8_t ID, uint8_t longueur, uint8_t instruction, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t param5)
{
    uint8_t check_sum = calcul_checksum(ID, longueur, instruction, param1, param2, param3, param4, param5);
    uint8_t octets_a_recevoir;

    //Détermnation du nombre d'octects à recevoir
    if (ID == TOUS_LES_AX12)                    //Pas de paquets renvoyés quand on s'adresse à tous les AX12
        octets_a_recevoir = 0;
    else if (instruction == READ_DATA)          //Renvoit un status normal (6 bits) + les donnés demandés
        octets_a_recevoir = 6 + param2;
    else
        octets_a_recevoir = 6;              //status standart = 6 octets

    ax12.tentatives = 0;

    do
    {
        reinit_buffer();
        ax12.tentatives ++;
       
        DIR_UART_AX12 = EMISSION;

        PutsUART(UART_XBEE, "tentative ...\n\r");

        //Envoit de tous les octets
        PutcUART(UART_AX12, START_BYTE);
        PutcUART(UART_AX12, START_BYTE);
        PutcUART(UART_AX12, ID);
        PutcUART(UART_AX12, longueur);
        PutcUART(UART_AX12, instruction);

        if (longueur > 2)
        {
            PutcUART(UART_AX12, param1);

            if (longueur > 3)
            {
                PutcUART(UART_AX12, param2);

                if (longueur > 4)
                {
                    PutcUART(UART_AX12, param3);

                    if (longueur > 5)
                    {
                        PutcUART(UART_AX12, param4);

                        if (longueur > 6)
                        {
                            PutcUART(UART_AX12, param5);
                        }
                    }
                }
            }
        }

        PutcUART(UART_AX12, check_sum);
        
        while(ax12.etat_uart != ENVOIT_FINI);

        if (octets_a_recevoir > 0)
        {
            DIR_UART_AX12 = RECEPTION;
            ax12.nb_octet_attente = octets_a_recevoir;
            traitement_reception_ax12();
        }
        

    }while (ax12.erreur != PAS_D_ERREUR && ax12.tentatives < MAX_TENTATIVES );

    PutsUART(UART_XBEE, "Sortie de la fonction \n\r");
    PutLongUART(ax12.erreur);
}



uint8_t calcul_checksum (uint8_t ID, uint8_t longueur, uint8_t instruction, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t param5)
{
    uint16_t buffer = ID + longueur + instruction;
    if (longueur > 2)
    {
        buffer += param1;
        if (longueur > 3)
        {
            buffer+= param2;
            if (longueur > 4)
            {
                buffer += param3;
                if (longueur > 5)
                {
                    buffer += param4;

                    if (longueur > 6)
                        buffer += param5;
                }
            }
        }
    }

    if (buffer > 255)
        buffer %= 256;

    buffer = ~buffer;

    return (uint8_t) buffer;
}

uint16_t read_data (uint8_t ID, uint8_t type_donnee)
{
    uint8_t nombre_octets_a_recevoir = 0;
    uint16_t buffer;

    //Détermination du nombres d'octets de données à recevoir
    switch (type_donnee)
    {
        //Donnée 8 bit
        case LIRE_TEMPS_REPONSE :
        case LIRE_TENSION :
        case LIRE_TEMPERATURE :
        case LIRE_MOUV_FLAG :
            nombre_octets_a_recevoir = 1;
            break;

        //Donnée 16 bits
        case LIRE_POSITION_ACTU :
        case LIRE_VITESSE_ACTU :
            nombre_octets_a_recevoir = 2;
            break;

        default :
            nombre_octets_a_recevoir = 1;
    }

    commande_AX12(ID, _4PARAM,  READ_DATA, type_donnee, nombre_octets_a_recevoir, NC, NC, NC);

    if (nombre_octets_a_recevoir == 1)
        buffer = ax12.buffer[PARAM1];
    else if (nombre_octets_a_recevoir == 2)
        buffer = ax12.buffer[PARAM1] + ax12.buffer[PARAM2] * 256;

    return buffer;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

//Bouge les AX12 à partir d'une position donnée comprise entre 0 et 1023
void angle_AX12 (uint8_t ID,  uint16_t position, uint16_t vitesse, uint8_t ATTENTE )
{
    uint8_t position_L = 0; //position paramètre 1
    uint8_t position_H = 0; //possition paramètre 2
    uint8_t vitesse_L  = 0;   //vitesse paramètre 1
    uint8_t vitesse_H  = 0;  //vitesse paramètre

    //Calcul de la position :
    if (position >= 256)
    {
        position_L = (uint8_t) position % 256;
        position_H = (uint8_t) position / 256;
    }
    else
    {
        position_L = (uint8_t) position;
        position_H = (uint8_t) 0;
    }

    //Calcul de la vitesse :
    if (vitesse  >= 256)
    {
        vitesse_L = (uint8_t) vitesse % 256;
        vitesse_H = (uint8_t) vitesse / 256;
    }
    else
    {
        vitesse_L = (uint8_t) vitesse;
        vitesse_H = (uint8_t) 0;
    }

   //Lancement de l'autom :
    if (ATTENTE == SANS_ATTENTE) //Si un seul AX à bouger :
    {
        commande_AX12(ID, _ALL_PARAM, WRITE_DATA, 0x1E, position_L, position_H , vitesse_L , vitesse_H );
    }
    else if (ATTENTE == AVEC_ATTENTE) //Si plusieurs AX à bouger :
    {
        commande_AX12(ID, _ALL_PARAM, REG_WRITE, 0x1E, position_L, position_H , vitesse_L , vitesse_H );
    }
}


/**
 * Flag de lancement de l'autom des AX12
 * \n Il faut avoir utilisé l'option AVEC_ATTENTE lors du lancement des mouvements
 */
void lancer_autom_AX12 (void)
{
    commande_AX12(TOUS_LES_AX12, _2PARAM, 0x05, NC, NC, NC, NC, NC);
}


/**
 * Fonction à utiliser pour fier la vitesse de rotation et le sens
 * Quand on utilise les AX12 en mode roue sans fin
 *
 * @param ID :
 * @param sens : HORAIRE ou ANTI_HORAIRE
 * @param vitesse : 0 -> 1023
 * @return
 */
void rotation_AX12 (uint8_t ID, uint8_t sens, uint16_t vitesse)
{
    //vitesse maxi de rotation = 0x3FF
    //Vitesse de rotation codée sur 2 octets
    //bits 15-11 = 0, bit 10 = sens, bit 9-0 : vitesse de rotation

    uint8_t octet_low, octet_high;

    //Calcul de la vitesse :
    if (vitesse > 256)
    {
        octet_low  = (uint8_t) vitesse % 256;
        octet_high = (uint8_t) vitesse / 256 + 4 * sens;
    }
    else
    {
        octet_low  = (uint8_t) vitesse;
        octet_high = (uint8_t) 0 + 4 * sens;
    }

    commande_AX12(ID, _5PARAM, WRITE_DATA, 0x20, octet_low, octet_high, NC, NC);
}

/******************************************************************************/
/****************** Fonctions de reconfiguration des AX12**********************/
/******************************************************************************/


/**
 * Fonction qui permet de configurer le renvois de données par les AX12
 * \n
 * Le mode AUCUNE_INFO, permet de les programmer à la chaine sans se soucier des bugs de com
 *
 * @param ID : un ID, ou TOUS_LES_AX12
 * @param type_de_retour : TOUTES_LES_INFOS, READ_ONLY, AUCUNE_INFO
 */
void configurer_status_returning_level (uint8_t ID, uint8_t type_de_retour)
{
    commande_AX12(ID, _4PARAM, WRITE_DATA,  0x10, type_de_retour, NC, NC, NC);
    delay_ms(100);
}

/**
 * configure l'AX12 à l'état d'usine
 * \n ID mis à 1 automatiquement
 * @param ID
 */
void reset_AX12 (uint8_t ID)
{
    commande_AX12(ID, _2PARAM, 0x06, NC, NC, NC, NC, NC);
}

/**
 * Permet de changer l'ID d'un AX12
 *
 * @param ancien_ID : TOUS_LES_AX12 si l'on ne connait pas l'ID
 * \n ATTENTION NE CONNECTER QU'UN AX12 PENDANT LA MANIP SINON ILS PRENDRONT TOUS LE NOUVEAU ID !!
 * @param nouveau_ID
 */
void changer_ID_AX12 (uint8_t ancien_ID, uint8_t nouveau_ID)
{
    commande_AX12(ancien_ID, _4PARAM, WRITE_DATA, 0x03, nouveau_ID, NC, NC, NC);
}

//Change la vitesse de com des AX12
//Pour le robot : _115200b
//(Fonction non testée ..)
void baud_AX12 (uint8_t ID, uint8_t bauds)
{
    commande_AX12(ID, _4PARAM, WRITE_DATA, 0x04, bauds, NC, NC, NC);
}

/**
 * Permet de configurer un AX12 en mode roue inifi ou en mode normal
 * Le mode est enregistré en mémoire EPPROM (non volatile)
 * @param ID
 * @param mode : MODE_INFINI ou MODE_NORMAL
 */
void mode_rotation_AX12 (uint8_t ID, uint8_t mode)
{
    if (mode == MODE_INFINI)
        commande_AX12(ID, _ALL_PARAM, WRITE_DATA, 0x06, NC, NC, NC, NC);
    else if (mode == MODE_NORMAL)
        commande_AX12(ID, _5PARAM, WRITE_DATA, 0x08, 0xFF, 0x03, NC, NC);
}

/******************************************************************************/
/********************** Fonctions diverses AX12 *******************************/
/******************************************************************************/

/**
 * Fonction pour allumer les LEDS des AX12 ... =)
 * @param ID : TOUS_LES_AX12 pour tous les allumer
 */
void allumer_LED_AX12 (uint8_t ID)
{
    commande_AX12(ID, _5PARAM, WRITE_DATA, 0x19, 0x01, 0x01, NC, NC);
}

/**
 * Fonction pour éteindre toutes la led d'un AX12 ... =(
 * @param ID : TOUS_LES_AX12 pour toutes les éteindre
 */
void eteindre_LED_AX12 (uint8_t ID)
{
    commande_AX12(ID, _5PARAM, WRITE_DATA, 0x19, NC, NC, NC, NC);
}