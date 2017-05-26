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

/******************************************************************************/
/************ Declaration des paramètres de montage des AX12 ******************/
/******************************************************************************/
/******************************************************************************/
/*** L'axe des 0° se situe parallèlement au sol du terrain                  ***/
/*** Il faut associer une position en point de l'AX12 à un angle.           ***/
/*** Exemple : On veut que la position 512 (le milieu de l'AX12 0)          ***/
/*** corresponde à l'angle 0                                                ***/
/*** -> decalage[0].angle = 0;                                              ***/
/*** -> decalage[0].position = 512;                                         ***/
/***                                                                        ***/
/*** /!\ REMARQUE IMPORTANTE :  /!\                                         ***/
/***  Si l'ax12  dépend d'un autre AX12 il faudra mettre dans angle l'ID    ***/
/***  de cet ax12                                                           ***/
/***  -> decalage[0].angle = AX12_QUI_EST_AVANT                             ***/
/***                                                                        ***/
/***  Etat : DEPENDANT ou INDEPANDANT (un autre AX12 le fait bouger ou non) ***/
/***  Suivant : AUCUN_AX ou l'ID d'un AX -> si l'AX12 entraine un autre AX12***/
/***  Sens_rotation : SENS_DIRECT / SENS_INDIRECT                           ***/
/***                  ROT_EN_HAUT / ROT_EN_BAS (axe horinzontal)            ***/
/***  symétrique : s'il y a un AX symétrique                                ***/
/***                à utiliser selon la couleur de départ                   ***/
/***    PAS_DE_SYMETRIQUE ou ID du symétrique                               ***/
/***   ### METTRE un MOINS si la rotation est inversée avec la couleur ###  ***/
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/************************* Variables gloables *********************************/
/******************************************************************************/

volatile _ax12 ax12;
volatile _Bool CHECK_LIMITATION_COURANT;
volatile decal decalage[ID_MAX_AX12];
volatile pos position_AX12[ID_MAX_AX12];


/******************************************************************************/
/*************************** FONCTIONS INITS **********************************/
/******************************************************************************/

void init_decalage_AX12 (void)      //Declaration de l'enchainement de montage des AX12
{
    //US
#ifdef GROS_ROBOT

#endif
#ifdef PETIT_ROBOT
    
#endif
}

void init_position_AX12 (void)      //Force l'état premier des AX12 à l'angle 0
{
    uint8_t i = 0;
    
    for (i = 0 ; i < ID_MAX_AX12 ; i++)
    {
        position_AX12[i].angle         = 0;
        position_AX12[i].point         = 0;
        position_AX12[i].nb_echec      = 0;
        position_AX12[i].nb_tentatives = 0;
        position_AX12[i].erreur        = PAS_D_ERREUR;
        position_AX12[i].present       = false;
    }
    
#ifdef GROS_ROBOT
    //us
#endif
    
#ifdef PETIT_ROBOT
    position_AX12[PINCE_BAS_AV].present  = true;
    position_AX12[PINCE_BAS_AR].present  = true;
    
    position_AX12[PINCE_HAUT_AV].present  = true;
    position_AX12[PINCE_HAUT_AR].present  = true;
    
    position_AX12[ASC_AVANT].present  = true;
    position_AX12[ASC_ARRIERE].present  = true;
    
    position_AX12[BITE_AV].present = true;
    position_AX12[BITE_AR].present = true;
#endif
}

/******************************************************************************/
/************* Fonction de conversion des positions primaires *****************/
/******************************************************************************/

/**
 * Produit en croix pour déterminer l'angle équivalent à une position
 * @param position : en points
 * @return : angle en °
 */
float convertion_position (uint16_t position)
{
    return (float) (position * ANGLE_MAX_AX / POSITION_MAX_AX);
}

/**
 * Produit en croix pour déterminer une position équivalente à un angle
 * @param angle en ° (nombre réel)
 * @return position (nombre entier)
 */
int convertion_angle (float angle)
{
    return (int16_t) (angle * POSITION_MAX_AX / ANGLE_MAX_AX);
}

/******************************************************************************/
/*************** Fonction de calcul des positions à envoyer *******************/
/******************************************************************************/

/**
 * calcul la position à envoyer à partir de l'angle souhaitée dans le plan fictif
 * @param ID : ID de l'AX12 dont on veut calculer le mouvement
 * @param angle : angle consigne en ° (nombre réel)
 * @return : position en point à donner en consigne à l'AX12
 */
int calcul_position (uint8_t ID, float angle)
{
    int16_t decal_angle;
    int16_t position;
    float ancien_angle;

    ancien_angle = position_AX12[ID].angle;

    if (decalage[ID].etat == DEPENDANT) //Si l'angle de l'AX12 dans le plan fictif dépend d'un autre AX12
    {
        // On récupère l'angle dans lequel est l'AX12 dont il dépend
        // Car quand un AX12 dépend d'un autre, on stocke l'ID de celui-ci à la place de l'angle
        decal_angle = position_AX12[decalage[ID].angle].angle; 
    }
    else
        decal_angle = ( decalage[ID].angle);

    //on calcule la position en fonction de l'angle des AX12 dont il peut dépendre
    position = convertion_angle( decalage[ID].sens_rotation * (angle - decal_angle + convertion_position(decalage[ID].sens_rotation * decalage[ID].position)));

    //si les positions sont hors champs on les adapte
    if(position > POINT_MAXI_DEFAUT) // > position_AX12[ID].point_maxi
        position = POINT_MAXI_DEFAUT; //position = position_AX12[ID].point_maxi //si implémentation limite
    else if (position < POINT_MINI_DEFAUT)
        position = POINT_MINI_DEFAUT;

    //on stock les nouvelles positions
    position_AX12[ID].point = position;
    position_AX12[ID].angle = angle;
    //calcul_modif_angle (ID, angle - ancien_angle);

    return position;
}


void calcul_modif_angle (uint8_t ID, float angle)
{
    if (decalage[ID].suivant != AUCUN_AX)
    {
        position_AX12[ID].angle += angle;
        calcul_modif_angle ( decalage[ID].suivant, angle);
    }
    return;
}


/******************************************************************************/
/******************** Fonctions pour faire bouger les AX12 ********************/
/******************************************************************************/

/**
 * Bouge les AX12 à partir d'un angle donné.
 * Cet angle est relatif à un plan, 0° = parallèle au terrain
 * \n
 * La fonction prend aussi en compte la couleur de départ et inverse les bras à utiliser
 * Elle permet aussi d'inverser les angles sur certains AX12
 * \n
 * Toute l'autom peut donc être fait pour en rouge et sera adaptée automatiquement en Jaune
 *
 * @param ID : ID de l'AX12 à déplacer
 * @param angle : en ° (nombre réel)
 * @param vitesse : VIT_AX_NORMAL, VIT_AX_REDUITE, VIT_AX_MOITIE ( de 0 à 1023)
 * @param attente : AVEC_ATTENTE, SANS_ATTENTE, pour permettre de synchro les mouvements
 */
void synchro_AX12 (uint8_t ID, float angle, uint16_t vitesse, uint8_t attente)
{
    if (COULEUR == BLEU)
    {
        if( decalage[ID].symetrique > 0)
        {
            ID = decalage[ID].symetrique;
        }
        else if (decalage[ID].symetrique < 0)
        {
            ID = - decalage[ID].symetrique;
            angle = - angle;
        }
    }

    angle_AX12(ID, calcul_position(ID, angle), vitesse, attente);
}


//Bouge les AX12 à partir d'une position donnée comprise entre 0 et 1023
void angle_AX12 (uint8_t ID,  uint16_t position, uint16_t vitesse, uint8_t ATTENTE )
{
    uint8_t position_L = 0; //position paramètre 1
    uint8_t position_H = 0; //possition paramètre 2
    uint8_t vitesse_L  = 0;   //vitesse paramètre 1
    uint8_t vitesse_H  = 0;  //vitesse paramètre

    
    // Stockage de la position de l'ax12 
    if (ID != TOUS_LES_AX12)
        position_AX12[ID].point = position;
    else 
    {
        uint8_t i = 0;
        for (i = 0 ; i < ID_MAX_AX12 ; i++)
            position_AX12[i].point = position;
    }
    
    //Calcul de la position :
    if (position >= 256)
    {
        position_L = (uint8_t) (position % 256);
        position_H = (uint8_t) (position / 256);
    }
    else
    {
        position_L = (uint8_t) position;
        position_H = (uint8_t) 0;
    }

    //Calcul de la vitesse :
    if (vitesse  >= 256)
    {
        vitesse_L = (uint8_t) (vitesse % 256);
        vitesse_H = (uint8_t) (vitesse / 256);
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
    commande_AX12(TOUS_LES_AX12, _2PARAM, ACTION);
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
        octet_low  = (uint8_t) (vitesse % 256);
        octet_high = (uint8_t) (vitesse / 256 + 4 * sens);
    }
    else
    {
        octet_low  = (uint8_t) vitesse;
        octet_high = (uint8_t) (0 + 4 * sens);
    }

    commande_AX12(ID, _5PARAM, WRITE_DATA, 0x20, octet_low, octet_high);
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
    commande_AX12(ID, _4PARAM, WRITE_DATA,  0x10, type_de_retour);
    delay_ms(100);
}


/**
 * Permet de modifier le temps mort de réponse entre l'envoit de la donnée, et la réponse de l'AX12
 * \n
 * DANS LE ROBOT, IL FAUT CONFIGURER LES AX12 POUR 20us
 * @param   ID                      : un ID, ou TOUS_LES_AX12
 * @param   temps_de_reponse_us     : Temps en µ secondes
 */
void configurer_temps_de_reponse_AX12 (uint8_t ID, uint16_t  temps_de_reponse_us)
{
    uint8_t buf;
    buf = (uint8_t) (temps_de_reponse_us/2);
    commande_AX12(ID, _4PARAM, WRITE_DATA, LIRE_TEMPS_REPONSE, buf);
}


/**
 * Fonction qui vérifie la présence d'un AX12
 * @param ID
 * @return : renvoit REPONSE_OK ou PAS_DE_REPONSE
 */
uint8_t Ping (uint8_t ID)
{
    commande_AX12(ID, _2PARAM, PING);

    if (ax12.erreur == PAS_D_ERREUR)
    {
        return REPONSE_OK;
    }
    else
    {
        return PAS_DE_REPONSE;
    }
}

/**
 * configure l'AX12 à l'état d'usine
 * \n ID mis à 1 automatiquement
 * @param ID
 */
void reset_AX12 (uint8_t ID)
{
    commande_AX12(ID, _2PARAM, RESET_AX);
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
    commande_AX12(ancien_ID, _4PARAM, WRITE_DATA, 0x03, nouveau_ID);
}

//Change la vitesse de com des AX12
//Pour le robot : _115200b
//(Fonction non testée ..)
void baud_AX12 (uint8_t ID, uint8_t bauds)
{
    commande_AX12(ID, _4PARAM, WRITE_DATA, 0x04, bauds);
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
        commande_AX12(ID, _ALL_PARAM, WRITE_DATA, 0x06);
    else if (mode == MODE_NORMAL)
        commande_AX12(ID, _5PARAM, WRITE_DATA, 0x08, 0xFF, 0x03);
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
    commande_AX12(ID, _4PARAM, WRITE_DATA, 0x19, 0x01);
}

/**
 * Fonction pour éteindre toutes la led d'un AX12 ... =(
 * @param ID : TOUS_LES_AX12 pour toutes les éteindre
 */
void eteindre_LED_AX12 (uint8_t ID)
{
    commande_AX12(ID, _4PARAM, WRITE_DATA, 0x19, 0x00);
}

/**
 * Fonction qui permet d'activer ou de désactiver l'asservissement en position
 * de l'ax12
 * @param ID        : ID de l'ax12 ou TOUS_LES_AX12 
 * @param mode      : true or false
 */
void torque_enable_ax12(uint8_t ID, _Bool mode)
{
    commande_AX12(ID, _4PARAM, WRITE_DATA, 0x18, (uint8_t) mode);
}

/**
 * check if ax12 is moving and if position is reached
 * @param ID
 * @return bool
 */
bool is_target_ax12_reachead (uint8_t ID)
{
    bool isPositionReached = false;
    uint8_t isAx12Moving = read_data(ID, LIRE_MOUV_FLAG);
    
    if (isAx12Moving == 0)
    {
        int16_t position = read_data(ID, LIRE_POSITION_ACTU);
        if ( (position < (position_AX12[ID].point + POSITION_REACHED_AX) ) &&
             (position > (position_AX12[ID].point - POSITION_REACHED_AX) ) )
        {
            isPositionReached = true;
        }
        else
        {
            angle_AX12(ID, position_AX12[ID].point, VITESSE_MAX_AX, SANS_ATTENTE);
        }
    }
    
    return isPositionReached;
}


/******************************************************************************/
/***************** Fonction de communication AX - 12 **************************/
/******************************************************************************/

void reception_uart_ax12 (void)
{
    int erreur = 0;
    uint8_t buf = U2RXREG;
    //U2STAbits.OERR = 0;

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
            break;
        }
    }

    if (ax12.erreur != TIME_OUT)
    {
        if (ax12.buffer[4] != 0)
        {
            if ((ax12.buffer[4] & 0b00100000) != 0)
                ax12.erreur = LIMITATION_DE_COURANT;
            else
                ax12.erreur = AUTRE_ERREUR;
            //printf(" erreur code : %d", ax12.buffer[4]);
        }
        uint8_t checksum = calcul_checksum(ax12.buffer[_ID], ax12.buffer[LONGUEUR], ax12.buffer[4], ax12.buffer[PARAM1], ax12.buffer[PARAM2], ax12.buffer[PARAM3], ax12.buffer[PARAM4], ax12.buffer[PARAM5]);

        ax12.buffer[CHSUM] = ax12.buffer[ax12.offset - 1];

        if (checksum != ax12.buffer[CHSUM])
        {
            ax12.erreur = ERREUR_CS;
            //printf("ERREUR Checksum \r\n");
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

void commande_AX12 (uint8_t ID, uint8_t longueur, uint8_t instruction, ...)
{
    static uint64_t __attribute__((unused)) nb1 = 0,__attribute__((unused)) nb2 = 0, nb3 = 0;
    //uint8_t nombre_reset = 0;
    
    va_list liste_param; // liste des arguments potentiels (param1, 2, 3, 4, 5)
    va_start(liste_param, instruction);
    uint8_t check_sum = calcul_checksum_variadic (ID, longueur, instruction, liste_param);
    va_end(liste_param);
    
    uint8_t octets_a_recevoir;
    uint8_t i, useless;
    

    // Initialisation de la liste d'argument non fixe
    va_start(liste_param, instruction); 

    //Détermnation du nombre d'octects à recevoir
    if (ID == TOUS_LES_AX12)                    //Pas de paquets renvoyés quand on s'adresse à tous les AX12
        octets_a_recevoir = 0;
    else if (instruction == READ_DATA)          //Renvoit un status normal (6 bits) + les donnés demandés
    {
        // on ignore le premier param, le deuxième contient le nb d'octet  à recevoir
        useless = va_arg(liste_param, uint8_t); 
        octets_a_recevoir = 6 + va_arg(liste_param, uint8_t); 
        
        // on ferme reinit la liste au début
        va_end(liste_param);
        va_start(liste_param, instruction);
    } 
    else
        octets_a_recevoir = 6;              //status standard = 6 octets

    ax12.tentatives = 0;

    do
    {
        reinit_buffer();
        ax12.tentatives ++;
        nb3++;

        DIR_UART_AX12 = EMISSION;
        vider_buffer_reception_uart(UART_AX12);

        //Envoit de tous les octets
        PutcUART(UART_AX12, START_BYTE);
        PutcUART(UART_AX12, START_BYTE);
        PutcUART(UART_AX12, ID);
        PutcUART(UART_AX12, longueur);
        PutcUART(UART_AX12, instruction);
        
        for (i = 2 ; i < longueur ; i++)
        {
            PutcUART(UART_AX12, va_arg(liste_param, uint8_t));
        }

        PutcUART(UART_AX12, check_sum);
        
        va_end(liste_param);

        ax12.nb_octet_attente = octets_a_recevoir;
        while(ax12.etat_uart != ENVOIT_FINI);

        if (octets_a_recevoir > 0)
        {
            U2STAbits.OERR = 0;
            vider_buffer_reception_uart(UART_AX12);
            DIR_UART_AX12 = RECEPTION;
            traitement_reception_ax12();
        }
   
        delay_us(50);
//        if (ax12.erreur == LIMITATION_DE_COURANT && CHECK_LIMITATION_COURANT == true && nombre_reset < 2)
//        {
//            ax12.tentatives = 0;
//            nombre_reset++;
//            reinit_alim_ax12();
//        }
        
        //printf("\n\rtest %d, erreur : %d", ax12.tentatives, ax12.erreur);

    }while (ax12.erreur != PAS_D_ERREUR && ax12.tentatives < MAX_TENTATIVES );
    
    // Enregistrement des données AX12 :
    position_AX12[ID].nb_tentatives += (uint16_t) (ax12.tentatives - 1);
    
    if(ax12.erreur != PAS_D_ERREUR)
    {
        position_AX12[ID].nb_echec +=1;
        position_AX12[ID].erreur = ax12.erreur;
    }

//    if (ax12.erreur != PAS_D_ERREUR)
//    {
//        son_evitement(10);
//    }

//    if(ax12.erreur != PAS_D_ERREUR)
//    {
//        nb1++;
//    }
//    else
//    {
//        nb2++;
//    }
//    printf("Succes : %d, Echec : %d, Tentatives failed : %d\r", nb1, nb2, nb3);
}

uint8_t calcul_checksum (uint8_t ID, uint8_t longueur, uint8_t instruction, ...)
{
    uint8_t i;
    uint16_t buffer = ID + longueur + instruction;
    
    va_list liste_param;
    va_start(liste_param, instruction);
    
    for (i = 0 ; i < longueur - 2 ; i++)
    {
        buffer += va_arg(liste_param, uint8_t);
    }
    
    va_end(liste_param);
    
    if (buffer > 255)
        buffer %= 256;

    buffer = ~buffer;

    return (uint8_t) buffer;
}

uint8_t calcul_checksum_variadic (uint8_t ID, uint8_t longueur, uint8_t instruction, va_list liste_param)
{
    uint8_t i;
    uint16_t buffer = ID + longueur + instruction;
    for (i = 2 ; i < longueur ; i++)
    {
        buffer += va_arg(liste_param, uint8_t);
    }

    if (buffer > 255)
        buffer %= 256;

    buffer = ~buffer;

    return (uint8_t) buffer;
}

int16_t read_data (uint8_t ID, uint8_t type_donnee)
{
    uint8_t nombre_octets_a_recevoir = 0;
    uint16_t buffer =0;

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
        case LIRE_POSITION_CONSIGNE :
            nombre_octets_a_recevoir = 2;
            break;

        default :
            nombre_octets_a_recevoir = 1;
    }

    commande_AX12(ID, _4PARAM,  READ_DATA, type_donnee, nombre_octets_a_recevoir);

    if (ax12.erreur != PAS_D_ERREUR)
    {
        return -1;
    }
    else
    {
        if (nombre_octets_a_recevoir == 1)
            buffer = ax12.buffer[PARAM1];
        else if (nombre_octets_a_recevoir == 2)
            buffer = ax12.buffer[PARAM1] + ax12.buffer[PARAM2] * 256;

        return buffer;
    }
}

void lecture_position_AX12 (uint8_t *ax12, int taille)
{
//    uint8_t i;
//    while(1)
//    {
//        for (i = 0 ; i < (uint8_t) taille ; i++)
//        {
//            printf("ID : %d = %d ", ax12[i], read_data(ax12[i], LIRE_POSITION_ACTU) );
//        }
//        printf("\r");
//    }

}

/******************************************************************************/
/*************************** GESTION DES ERREURS ******************************/
/******************************************************************************/

/**
 * Fonction qui va pinguer régulièrement les ax12 configurés comme présent dans 
 * le code
 * Si jamais un ax12 né répond pas, son état d'erreur va être mis à jour lors du 
 * Ping et l'on pourra traiter l'erreur plus tard en temps voulu
 */
uint8_t checkup_com_ax12()
{
    uint8_t status = PAS_D_ERREUR;
    uint8_t id = 0;
    
    for (id = 0 ; id < ID_MAX_AX12 ; id++)
    {
        if (position_AX12[id].present == true)
        {
            if (Ping(id) != REPONSE_OK )
            {
                printf("\n\r Ping for ax %d FAILED", id);
                status = PAS_DE_REPONSE;
            }
        }
    }
    
    return status;
}

/**
 * Fonction qui va réinitialisé l'alim et renvoyer les dernières positions 
 * enregistrés des ax12 si l'on détecte qu'un ax12 et en limitation de courant
 */
void clean_des_erreurs_ax12()
{
    uint8_t id = 0;
    _Bool erreur_overload = false;
    
    for (id = 0 ; id < ID_MAX_AX12 ; id++)
    {
        if (position_AX12[id].erreur == LIMITATION_DE_COURANT)
            erreur_overload = true;
    }
    
    if (erreur_overload == true)
    {
        reinit_alim_ax12();
        
        for (id = 0 ; id < ID_MAX_AX12 ; id++)
        {
            if (position_AX12[id].present == true)
            {
                angle_AX12(id, position_AX12[id].point, 500, SANS_ATTENTE);
            }
        }
    }   
}


/**
 * Fonction à appeler à la fin du code, ou même avant le début du match pour 
 * se rendre compte si il y a des problèmes de com récurent avec certains ax12
 */
void print_statistique_ax12()
{
#ifndef NO_SERIALUS
    uint8_t id = 0;
    for (id = 0 ; id < ID_MAX_AX12 ; id++)
    {
        if (position_AX12[id].present == true)
        {
            printf("\n\rID : %d\tEchec : %d\tTentatives : %d\tErr Act : %d",  id, position_AX12[id].nb_echec, position_AX12[id].nb_tentatives, position_AX12[id].erreur);
        }
    }
#endif
}


/**
 * Pour clearer des erreurs d'overload, il faut couper l'alimentation des ax12
 * puis la rallumer
 */
void reinit_alim_ax12()
{
    INHIBIT_AX12 = ETEINT;
    delay_ms(30);
    print_abort("reinit alim ax12");
    INHIBIT_AX12 = ALLUME;
    // test jusqu'à 10 secondes non concluant
    // Il faut tester une autre solution
    delay_ms(200);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/