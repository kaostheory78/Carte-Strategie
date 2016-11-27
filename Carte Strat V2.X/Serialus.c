/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: serialus.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 29 Mai 2016, 12:43
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"

/******************************************************************************/
/**************************** VARIABLES GLOBALES ******************************/
/******************************************************************************/

#ifndef NO_SERIALUS

volatile _serialus serialus;

/******************************************************************************/
/**************************** FONCTIONS DE CONFIG  ****************************/
/******************************************************************************/


void reinit_buffer_serialus()
{
    uint8_t i, j;
    
    for (i = 0 ; i  < MAX_PARAM ; i++)
    {
        for (j = 0 ; j < MAX_BUFF ; j++)
        {
            serialus.buffer[i][j] = '\0';
        }
        serialus.index[i] = 0;
    }  
    
    serialus.actif = true;
    serialus.attente_confirmation = false;
    serialus.confirmation = false;
    serialus.erreur_confirmation = false;
    serialus.info_a_traiter = false;
    serialus.nb_param = 0;
    serialus.clignotement_en_cours = false;
    
    printf("\n\n\r");
}

/******************************************************************************/
/**************************** FONCTIONS TRAITEMENT ****************************/
/******************************************************************************/


/**
 * Fonction appellée à la réception de chaque octet sur la liaison série serialus
 * \nTraite le rangement des chaines de caractères reçus dans les bons tableaux
 * grace aux caractères ' ' et '\n'
 * \nGère également la suppresion de caractère correctement
 * @param octet
 */
void reception_serialus(uint8_t octet)
{
    // Si on efface un caractère
    if (octet == 127)
    {
        // Si ce n'est pas le premier charactère du paramètre
        if (serialus.index[serialus.nb_param] > 0)
        {
            // On revient en arrière
            serialus.index[serialus.nb_param] --;
            
            // Si on est dans le range du tableau donc que l'info avait été gardée en mémoire
            if (serialus.nb_param < MAX_PARAM && serialus.index[serialus.nb_param] < MAX_BUFF)
                serialus.buffer[serialus.nb_param][serialus.index[serialus.nb_param]]= '\0';
        }
        // Sinon si on revient au paramètre d'avant et que ce n'est pas le premier paramètre
        else if (serialus.nb_param > 0)
        {
            serialus.nb_param--;
        }
    }
    else if (octet == ' ')
    {
        // Si on est dans l'avant dernière case du tableau
        serialus.nb_param ++;
    }
    else if (octet == '\r')
    {
        //supression de l'espace en trop a la fin si l'utilisateur est con comme
        //moi à mettre un espace en trop à la fin
        if (serialus.index[serialus.nb_param] == 0 && serialus.buffer[serialus.nb_param][serialus.index[serialus.nb_param]] == '\0' && serialus.nb_param > 0 )
            serialus.nb_param --;
        
        serialus.info_a_traiter = true;
    }
    else
    {
        // Si on est max au dernier param
        // Et si on a pas encore rempli la dernière case du tableau
        if (serialus.nb_param < MAX_PARAM && serialus.index[serialus.nb_param] < MAX_BUFF)
        {
            serialus.buffer[serialus.nb_param][serialus.index[serialus.nb_param]] = octet;
        }
        serialus.index[serialus.nb_param]++;
    }
}

void action_reception_serialus(uint8_t buf)
{
    if (buf == 3)
    {
        print_abort("CTRL-C");
        reinit_buffer_serialus();
        if (serialus.info_a_traiter == true)
        {
            if (check_string(1, "de"))
            {        
                fin_deplacement();
            }
        }
    }
    else if (serialus.info_a_traiter == false)
    {
        putchar(buf);
        reception_serialus(buf);
    }
    else if (serialus.attente_confirmation == true)
    {
        putchar(buf);
        if (buf == 'y')
        {
            serialus.confirmation = true;
        }
        else if (buf == 'n')
        {
            serialus.confirmation = false;
        }
        else 
        {
            serialus.erreur_confirmation = true;
        }
        serialus.attente_confirmation = false;
    }
}

/**
 * Fonction qui lance l'analyse d'une trame reçu sur selarius si les données 
 * reçus semblent cohérente à première vue
 */
void traitement_serialus ()
{
    if (serialus.info_a_traiter == true)
    {
        if (check_erreur_serialus() == true)
        {
            printf("\n\rErreur dans la taille des donnees recus");
            reinit_buffer_serialus();
        }
        else 
        {
            conversion_string_to_function_code();
            reinit_buffer_serialus();
        }
    }   
}


/**
 * Fonction qui répartit le traitement des données en fonction du premier 
 * function code reçu
 */
void conversion_string_to_function_code()
{
    if (serialus.info_a_traiter == true)
    {
        // Fonctions de déplacement
        if (check_string(0, "de"))
        {
            serialus_traitement_deplacement();
        }
        // Fonctions de calage
        else if (check_string(0, "ca"))
        {
            serialus_traitement_calage();
        }
        // Fonctions d'asserv
        else if (check_string(0, "as"))
        {
            serialus_traitement_asserv();
        }
        // Fonctions d'odo
        else if (check_string(0, "odo"))
        {
            serialus_traitement_odometrie();
        }
        // Fonctions de position
        else if (check_string(0, "pos"))
        {
            serialus_traitement_position();
        }
        // Fonction d'ax12
        else if (check_string(0, "ax"))
        {
            serialus_traitement_ax12();
        }
        else if (check_string(0, "list"))
        {
            serialus_traitement_list();
        }
        //effacer la fenêtre
        else if (check_string(0, "clear") || check_string(0, "clc"))
        {
            printf("\e[1;1H\e[2J");
        }
        // Code Fonction non reconnu
        else
        {
            print_non_reconnu(0);
            list_serialus(SER_LIST);
        }
    }  
}


/**
 * Fonction qui traite toutes les frames relavtives au code "de" pour déplacement
 */
void serialus_traitement_deplacement ()
{
    // Si paramètre non renseigné
    if (serialus.nb_param < 1)
    {
        print_manque_parametre();
        list_serialus(SER_DEPLACEMENT);
    }
    // Fonction cibler
    else if (check_string(1, "cib"))
    {
       if (check_nb_param(4))
       {
           if (check_sont_des_nombres(3, serialus.buffer[2], serialus.buffer[3], serialus.buffer[4]))
           {
               double x = (double) atoi((const char*)serialus.buffer[2]);
               double y = (double) atoi((const char*)serialus.buffer[3]);
               double v = (double) atoi((const char*)serialus.buffer[4]);

               if (check_coherence(abs(x) < 4000 && abs(y) < 4000 && abs(v) < 200 ))
               {
                   //printf("\n\rcibler X : %d, Y : %d, V : %d",(int) x,(int) y,(int) v);
                   init_clignotement();
                   print_erreur_deplacement(_cibler(x, y, v));
               }
           }
       }
    }
    // Fonction Orienter
    else if (check_string(1, "ori"))
    {
        if (check_nb_param(3))
        {
            if (check_sont_des_nombres(2, serialus.buffer[2], serialus.buffer[3]))
            {
                double a = (double) atoi((char *) serialus.buffer[2]);
                double v = (double) atoi((char *) serialus.buffer[3]);

                if (check_coherence(abs(a) <= 180 && abs(v) < 200 ))
                {
                    init_clignotement();
                    //printf("\n\rorienter Teta : %d, V : %d",(int) a,(int) v);
                    print_erreur_deplacement(_orienter(a, v));
                }
            }
        }
    }
    // Fonction rejoindre
    else if (check_string(1, "rej"))
    {
        if (check_nb_param(5))
        {
            if (check_sont_des_nombres(4, serialus.buffer[2], serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
            {
                double x = (double) atoi((char *)serialus.buffer[2]);
                double y = (double) atoi((char *)serialus.buffer[3]);
                int8_t s = (int8_t) atoi((char *)serialus.buffer[4]);
                double v = (double) atoi((char *)serialus.buffer[5]);

                if (check_coherence(abs(x) < 3000 && abs(y) < 2000 && abs(v) < 150 && abs(s) == 1))
                {
                    init_clignotement();
                    //printf("\n\rrejoindre X : %d, Y : %d,sens : %d, V : %d",(int) x,(int) y,(int) s,(int) v);
                    print_erreur_deplacement(_rejoindre(x, y, s, v));
                }
            }
        }
    }
    // Fonction  avancer
    else if (check_string(1, "av"))
    {
        if (check_nb_param(3))
        {
            if (check_sont_des_nombres(2, serialus.buffer[2], serialus.buffer[3]))
            {
                double d = (double) atoi((char *)serialus.buffer[2]);
                double v = (double) atoi((char *)serialus.buffer[3]);

                if (check_coherence(d < 3000 && d > 0 && abs(v) < 150))
                {
                    init_clignotement();
                    //printf("\n\ravancer d : %d, V : %d",(int) d,(int) v);
                    print_erreur_deplacement(_avancer_reculer(d, v));
                }
            }
        }
    }
    // Fonction reculer
    else if (check_string(1, "rec"))
    {
        if (check_nb_param(3))
        {
            if (check_sont_des_nombres(2, serialus.buffer[2], serialus.buffer[3]))
            {
                double d = (double) atoi((char *)serialus.buffer[2]);
                double v = (double) atoi((char *)serialus.buffer[3]);

                if (check_coherence(d < 3000 && d > 0 && abs(v) < 150))
                {
                    init_clignotement();
                    //printf("\n\rreculer d : %d, V : %d",(int) d,(int) v);
                    print_erreur_deplacement(_avancer_reculer(-d, v));
                }
            }
        }
    }
    // Fonction faire des tours
    else if (check_string(1, "fdt"))
    {
        if (check_nb_param(2))
        {
            if (check_est_un_nombre((uint8_t *)serialus.buffer[2]))
            {
                int8_t nb = (int8_t) atoi((char *)serialus.buffer[2]);

                if (check_coherence(abs(nb) < 60))
                {
                    init_clignotement();
                    //printf("\n\rtourner de %d tours", nb);
                    faire_des_tours(nb);
                    serialus.clignotement_en_cours = false;
                    print_position();
                }
            }
            else
                print_pas_nombre();
        }
    }
    // Fonction trapèze
    else if (check_string(1, "tra"))
    {
        if (check_nb_param(1))
        {
            init_clignotement();
            //printf("\n\rje fais un trapeze");
            trapeze(MARCHE_AVANT);
            serialus.clignotement_en_cours = false;
            print_position();
        }
    }
    // Fonction carré
    else if (check_string(1, "ca"))
    {
        if (check_nb_param(1))
        {
            init_clignotement();
            //printf("\n\rje fais un carre");
            carre(MARCHE_AVANT);
            serialus.clignotement_en_cours = false;
            print_position();
        }
    }
    // Fonction brake
    else if (check_string(1, "brake"))
    {
        if (check_nb_param(1))
        {
            printf("\n\rbrake");
            fin_deplacement();
        }
    }
    // Fonction unbrake
    else if (check_string(1, "unbrake"))
    {
        if (check_nb_param(1))
        {
            printf("\n\rUnbrake");
            unbrake();
        }
    }
    else if (check_string(1, "list"))
    {
        list_serialus(SER_DEPLACEMENT);
    }
    else
    {
        print_non_reconnu(1);
        list_serialus(SER_DEPLACEMENT);
    }
}


/**
 * Fonction qui traite toutes les frames relavtives au code "ca" pour calage
 */
void serialus_traitement_calage()
{
    if (serialus.nb_param < 1)
    {
        print_manque_parametre();
        list_serialus(SER_CALAGE);
    }
    // Calage avec une distance, pas d'init
    else if (check_string(1, "cal"))
    {
        if (check_nb_param(3))
        {
            if (check_sont_des_nombres(2, serialus.buffer[2], serialus.buffer[3]))
            {
                double d = (double) atoi((char *)serialus.buffer[2]);
                double v = (double) atoi((char *)serialus.buffer[3]);

                if (check_coherence(d < 3000 && d > 0 && abs(v) < 150))
                {
                    init_clignotement();
                    //printf("\n\rCalage d : %d, V : %d",(int) d,(int) v);
                    calage(d, v);
                    serialus.clignotement_en_cours = false;
                    print_position();
                }
            }
        }
    }
    // Calage bordure en X avec reinit X et Teta
    else if (check_string(1, "cx"))
    {
        if (check_nb_param(5))
        {
            if (check_sont_des_nombres(4, serialus.buffer[2], serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
            {
                double x = (double) atoi((char *)serialus.buffer[2]);
                double t = (double) atoi((char *)serialus.buffer[3]);
                int8_t s = (int8_t) atoi((char *)serialus.buffer[4]);
                double v = (double) atoi((char *)serialus.buffer[5]);

                if (check_coherence(abs(x) < 3000 && abs(t) <= 180 && abs(v) < 150 && abs(s) == 1))
                {
                    init_clignotement();
                    //printf("\n\rCalage en X x : %d, teta : %d, sens : %d, vitessse : %d",(int) x,(int) t, (int) s, (int) v);
                    calage_X(x, t, s, v);
                    serialus.clignotement_en_cours = false;
                    print_position();
                }
            }
        }
    }
    // Calage bordure en Y avec reinit Y et Teta
    else if (check_string(1, "cy"))
    {
        if (check_nb_param(5))
        {
            if (check_sont_des_nombres(4, serialus.buffer[2], serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
            {
                double y = (double) atoi((char *)serialus.buffer[2]);
                double t = (double) atoi((char *)serialus.buffer[3]);
                int8_t s = (int8_t) atoi((char *)serialus.buffer[4]);
                double v = (double) atoi((char *)serialus.buffer[5]);

                if (check_coherence(abs(y) < 2000 && abs(t) <= 180 && abs(v) < 150 && abs(s) == 1))
                {
                    init_clignotement();
                    //printf("\n\rCalage en Y y : %d, teta : %d, sens : %d, vitessse : %d",(int) y,(int) t, (int) s, (int) v);
                    calage_Y(y, t, s, v);
                    serialus.clignotement_en_cours = false;
                    print_position();
                }
            }
        }
    }
    // Calage en distance contre une bordure avec recalage en Teta
    else if (check_string(1, "ct"))
    {
        if (check_nb_param(4))
        {
            if (check_sont_des_nombres(3, serialus.buffer[2], serialus.buffer[3], serialus.buffer[4]))
            {
                double t = (double) atoi((char *)serialus.buffer[2]);
                int8_t s = (int8_t) atoi((char *)serialus.buffer[3]);
                double v = (double) atoi((char *)serialus.buffer[4]);

                if (check_coherence(abs(t) <= 180 && abs(v) < 150 && abs(s) == 1))
                {
                    init_clignotement();
                    //printf("\n\rCalage en Teta,  teta : %d, sens : %d, vitessse : %d",(int) t, (int) s, (int) v);
                    calage_teta(t, s, v);
                    serialus.clignotement_en_cours = false;
                    print_position();
                }
            }
        }
    }
    else
    {
        print_non_reconnu(1);
        list_serialus(SER_CALAGE);
    }
}

/**
 * Fonction qui traite toutes les frames relavtives au code "as" pour Asserv
 */
void serialus_traitement_asserv()
{
    if (serialus.nb_param < 2)
    {
        print_manque_parametre();
        list_serialus(SER_ASSERV);
    }
    else if (check_string(1, "set"))
    {

    }
    else if (check_string(1, "get"))
    {

    }
    else
    {
        print_non_reconnu(1);
        list_serialus(SER_POSITION);
    }
}

/**
 * Fonction qui traite toutes les frames relavtives au code "odo" pour Odométrie
 */
void serialus_traitement_odometrie()
{
    printf("\n\rRIEN POUR LE MOMENT :/");
}

/**
 * Fonction qui traite toutes les frames relavtives au code "pos" pour position
 */
void serialus_traitement_position()
{
    if (serialus.nb_param < 2)
    {
        print_manque_parametre();
        list_serialus(SER_POSITION);
    }
    else if (check_string(1, "set"))
    {
        // Init position du robot
        if (check_string(2, "rob"))
        {
            if (check_nb_param(5))
            {
                if (check_sont_des_nombres_flottants(3, serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
                {
                    double x = (double) atof((char *)serialus.buffer[3]);
                    double y = (double) atof((char *)serialus.buffer[4]);
                    double t = (double) atof((char *)serialus.buffer[5]);

                    if (check_coherence(abs(x) <= 3000 && abs(y) <= 2000 && abs(t) <= 180))
                    {
                        
                        printf("\n\rInit X : %f, Y : %f, Teta : %f",(float) x,(float) y,(float) t);   
                        if (check_confirmation())
                        {
                            init_position_robot(x, y, t);
                            print_position();
                        }
                    }
                }
            }
        }
        else if (check_string(2, "x"))
        {
            if (check_nb_param(3))
            {
                if (check_est_un_nombre_flottant((uint8_t*)serialus.buffer[3]))
                {
                    double x = (double) atof((char *)serialus.buffer[3]);
                    if (check_coherence(abs(x) <= 3000))
                    {
                        printf("\n\rInit X : %f",(float) x);
                        
                        if (check_confirmation())
                        {
                            init_X(x);
                            print_position();
                        }
                    }
                }
            }
        }
        else if (check_string(2, "y"))
        {
            if (check_nb_param(3))
            {
                if (check_est_un_nombre_flottant((uint8_t*)serialus.buffer[3]))
                {
                    double y = (double) atof((char *)serialus.buffer[3]);
                    if (check_coherence(abs(y) <= 2000))
                    {
                        printf("\n\rInit Y : %f", (float) y);
                        
                        if (check_confirmation())
                        {
                            init_X(y);
                            print_position();
                        }
                    }
                }
            }
        }
        else if (check_string(2, "t"))
        {
           if (check_nb_param(3))
            {
                if (check_est_un_nombre_flottant((uint8_t*)serialus.buffer[3]))
                {
                    double t = (double) atof((char *)serialus.buffer[3]);
                    if (check_coherence(abs(t) <= 180))
                    {
                        printf("\n\rInit Teta : %f", (float) t);
                            
                        if (check_confirmation())
                        {
                            init_orientation(t);
                            print_position();
                        }
                    }
                }
            } 
        }
        else 
        {
            print_non_reconnu(2);
            list_serialus(SER_POSITION);
        }
    }
    else if (check_string(1, "get"))
    {
        // Get position du robot
        if (check_string(2, "rob"))
        {
            if (check_nb_param(2))
            {
                print_position();
            }
        }
        else if (check_string(2, "x"))
        {
            if (check_nb_param(2))
            {
                printf("\n\rX : %f",(float) get_X());
            }
        }
        else if (check_string(2, "y"))
        {
            if (check_nb_param(2))
            {
                printf("\n\rY : %f", (float) get_Y());
            }
        }
        else if (check_string(2, "t"))
        {
            if (check_nb_param(2))
            {
                printf("\n\rTeta : %f",(float) get_orientation());
            }
        }
        else 
        {
            print_non_reconnu(2);
            list_serialus(SER_POSITION);
        }
    }
    else
    {
        print_non_reconnu(1);
        list_serialus(SER_POSITION);
    }
}

/**
 * Fonction qui traite toutes les frames relavtives au code "ax" pour AX12
 */
void serialus_traitement_ax12()
{
    if (serialus.nb_param < 2)
    {
        print_manque_parametre();
        list_serialus(SER_AX12);
    }
    else if (check_string(1, "set"))
    {
        // bouger un ax12 sur un angle
        if (check_string(2, "ang"))
        {
            if (check_nb_param(5))
            {
                if (check_sont_des_nombres(3, serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
                {
                    uint8_t id = (uint8_t) atoi((char *)serialus.buffer[3]);
                    int16_t angle = (int16_t) atoi((char *)serialus.buffer[4]);
                    uint16_t v = (uint16_t) atoi((char *)serialus.buffer[5]);

                    if (check_coherence(id >= 0 && id <= ID_MAX_AX12 && abs(angle) <= 270 && v >= 0 && v <= 1023))
                    {
                        printf("ID : %d, angle : %d, v : %d", id, angle, v);
                        synchro_AX12(id, (float)angle, v, SANS_ATTENTE);
                        print_erreur_ax12();
                    }
                }
            } 
        }
        else if  (check_string(2, "pos"))
        {
            if (check_nb_param(5))
            {
                if (check_sont_des_nombres(3, serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
                {
                    uint8_t id = (uint8_t) atoi((char *)serialus.buffer[3]);
                    uint16_t position = (uint16_t) atoi((char *)serialus.buffer[4]);
                    uint16_t v = (uint16_t) atoi((char *)serialus.buffer[5]);

                    if (check_coherence(id >= 0 && id <= ID_MAX_AX12 && position>=0 && position <= 1023 && v >= 0 && v <= 1023))
                    {
                        //printf("ID : %d, position : %d, v : %d", id, position, v);
                        angle_AX12(id, position, v, SANS_ATTENTE);
                        print_erreur_ax12();
                    }
                }
            } 
        }
        else if (check_string(2, "led"))
        {
            if (check_nb_param(4))
            {
                // si c'est un id
                int16_t id = check_id_ax12(3);
                if (id != -1)
                {
                    if (check_string(4, "on"))
                    {
                        printf("id : %d -> led on", id);
                        allumer_LED_AX12((uint8_t)id);
                        print_erreur_ax12();
                    }
                    else if (check_string(4, "off"))
                    {
                        printf("id : %d -> led off", id);
                        eteindre_LED_AX12((uint8_t) id);
                        print_erreur_ax12();
                    }
                    else
                    {
                        print_non_reconnu(4);
                        list_serialus(SER_POSITION);
                    }
                }
            } 
        }
        else if (check_string(2, "data"))
        {
            if (check_nb_param(5))
            {
                if (check_sont_des_nombres(2, serialus.buffer[3], serialus.buffer[4], serialus.buffer[5]))
                {
                    uint8_t id   = (uint8_t) atoi((char *)serialus.buffer[3]);
                    uint8_t add  = (uint8_t) atoi((char *)serialus.buffer[4]);
                    uint8_t data = (uint8_t) atoi((char *)serialus.buffer[5]);
                    
                    if (check_coherence(data <= 255))
                    {
                        commande_AX12(id, _4PARAM, WRITE_DATA, add, data);
                    }
                }
            }
        }
        else if (check_string(2, "torque"))
        {
            if (check_nb_param(4))
            {
                // si c'est un id
                int16_t id = check_id_ax12(3);
                if (id != -1)
                {
                    if (check_string(4, "on"))
                    {
                        //printf("\n\rid : %d -> roque on", id);
                        torque_enable_ax12((uint8_t)id, true);
                        print_erreur_ax12();
                    }
                    else if (check_string(4, "off"))
                    {
                        //printf("\n\rid : %d -> roeque off", id);
                        torque_enable_ax12((uint8_t) id, false);
                        print_erreur_ax12();
                    }
                    else
                    {
                        print_non_reconnu(4);
                        list_serialus(SER_POSITION);
                    }
                }
            } 
        }
        else if (check_string(2, "alim"))
        {
            if (check_nb_param(3))
            {
               
                if (check_string(3, "on"))
                {
                    //printf("\n\rid : %d -> roque on", id);
                    INHIBIT_AX12 = ALLUME;
                }
                else if (check_string(3, "off"))
                {
                    //printf("\n\rid : %d -> roeque off", id);
                    INHIBIT_AX12 = ETEINT;
                }
                else
                {
                    print_non_reconnu(4);
                    list_serialus(SER_POSITION);
                }
            } 
        }     
        else 
        {
            print_non_reconnu(2);
            list_serialus(SER_AX12);
        }
    }
    else if (check_string(1, "get"))
    {
        // position de l'ax
        if (check_string(2, "pos"))
        {
            if (check_nb_param(3))
            {
                // si c'est un id
                int16_t id = check_id_ax12(3);
                if (id == TOUS_LES_AX12)
                {
                    uint8_t id = 0, index = 0;
                    uint8_t tab[ID_MAX_AX12];
                    for (id = 0 ; id < ID_MAX_AX12 ; id++)
                    {
                        if (Ping(id) == REPONSE_OK)
                        {
                            tab[index++] = id;
                        }
                    }
                    printf("\n");
                    while(serialus.info_a_traiter == true)
                    {
                        printf("\r");
                        for (id = 0 ; id < index ; id++)
                        {
                            print_position_ax12(tab[id], read_data(tab[id], LIRE_POSITION_ACTU));
                        }
                    }
                }
                else if (id != -1)
                {
                    if (Ping(id) == REPONSE_OK)
                    {
                        printf("\n\r");
                        print_position_ax12(id, read_data(id, LIRE_POSITION_ACTU));
                    }
                    else 
                        print_abort("PAS DE REPONSE");
                }
            } 
        }
        else if (check_string(2, "ping"))
        {
            if (check_nb_param(3))
            {
                // si c'est un id
                int16_t id = check_id_ax12(3);
                if (id == TOUS_LES_AX12)
                {
                    uint8_t id = 0;
                    for (id = 0 ; id < ID_MAX_AX12 ; id++)
                    {
                        if (Ping(id) == REPONSE_OK)
                        {
                            print_ping(id);
                        }
                    }
                }
                else if (id != -1)
                {
                    if (Ping(id) == REPONSE_OK)
                    {
                        print_ping(id);
                    }
                    else 
                        print_abort("PAS DE REPONSE");
                }
            } 
        }
        else if (check_string(2, "data"))
        {
            if (check_nb_param(4))
            {
                if (check_sont_des_nombres(2, serialus.buffer[3], serialus.buffer[4]))
                {
                    uint8_t id   = (uint8_t) atoi((char *)serialus.buffer[3]);
                    uint8_t add  = (uint8_t) atoi((char *)serialus.buffer[4]);

                    printf("\n\rdata = %d", read_data(id, add));
                }
            }
        }
        else 
        {
            print_non_reconnu(2);
            list_serialus(SER_AX12);
        }
    }
    else
    {
        print_non_reconnu(1);
        list_serialus(SER_AX12);
    }
}

/**
 * Fonction qui traite toutes les frames relavtives au code "list" pour 
 * la liste des fonctions possibles
 */
void serialus_traitement_list()
{
    if (serialus.nb_param < 1)
    {
        list_serialus(SER_LIST);
    }
    else if (check_string(1, "all"))
    {
        list_serialus(SER_LIST);
        list_serialus(SER_DEPLACEMENT);
        list_serialus(SER_ASSERV);
        list_serialus(SER_ODOMETRIE);
        list_serialus(SER_POSITION);
        list_serialus(SER_AX12);
    }
    else if (check_string(1, "de"))
    {
        list_serialus(SER_DEPLACEMENT);
    }
    else if (check_string(1, "as"))
    {
        list_serialus(SER_ASSERV);
    }
    else if (check_string(1, "odo"))
    {
        list_serialus(SER_ODOMETRIE);
    }
    else if (check_string(1, "pos"))
    {
        list_serialus(SER_POSITION);
    }
    else if (check_string(1, "ax"))
    {
        list_serialus(SER_AX12);
    }
    else 
    {
        print_non_reconnu(1);
        list_serialus(SER_LIST);
    }
}

/******************************************************************************/
/************************** FONCTIONS DE CHECK ********************************/
/******************************************************************************/

/**
 * Fonction qui vérifie que la chaine reçu est bien un nombre entier
 * @param buffer : buffer contenant le nombre
 * @return       : true / false
 */
_Bool check_est_un_nombre(uint8_t* buffer)
{
    int i = 0;
    
    for (i = 0 ; i < MAX_BUFF ; i++)
    {
        if (buffer[i] == '\0')
        {
            if (i > 0)
                return true;
            else 
                return false;
        }
        else if (buffer[i] < '0' || buffer[i] > '9')
        {
            if (!(buffer[i] == '-' && i == 0))
                return false;
        }
    }
    return true;
}

int16_t check_id_ax12(uint8_t param)
{
    int16_t id;
    if (check_est_un_nombre((uint8_t*) serialus.buffer[param]))
    {
        id = atoi((char *)serialus.buffer[param]);
        if (!(id >= 0 && id <= ID_MAX_AX12))
            id = -1;
    }
    else if (check_string(param, "all"))
    {
        id = TOUS_LES_AX12;
    }
    else 
        id =-1;
    
    if (id == -1)
        print_abort("Probleme ID");
    
    return id;
}

/**
 * Fonction qui vérifie que la chaine reçu est bien un nombre flottant
 * @param buffer : buffer contenant le nombre
 * @return       : true / false
 */
_Bool check_est_un_nombre_flottant(uint8_t* buffer)
{
    int i = 0;
    _Bool presence_virgule = false;
    
    for (i = 0 ; i < MAX_BUFF ; i++)
    {
        if (buffer[i] == '\0')
        {
            if (i > 0)
                return true;
            else 
                return false;
        }
        else if (buffer[i] == '.' || buffer[i] == ',')
        {
            if (presence_virgule == false)
                presence_virgule = true;
            else
                return false;
        }
        else if (buffer[i] < '0' || buffer[i] > '9')
        {
            if (!(buffer[i] == '-' && i == 0))
                return false;
        }
    }
    return true;
}

/**
 * Fonction qui vérifie si une liste de chaine représente bien des nombres
 * @param nb_nombre : nombre de nombre à vérifier
 * @param ...       : liste de chaine de nombre (de type (int8_t *))
 * @return          : true / false
 */
_Bool check_sont_des_nombres(uint8_t nb_nombre, ...)
{
    va_list liste_param;
    va_start(liste_param, nb_nombre);
    int i = 0;
    
    for (i = 0 ; i < nb_nombre ; i++)
    {
        if (check_est_un_nombre(va_arg(liste_param, uint8_t*)) == false)
        {
            print_pas_nombre();
            return false;
        }
    }
    return true;
}

/**
 * Fonction qui vérifie si une liste de chaine représente bien des nombres flottants
 * @param nb_nombre : nombre de nombre à vérifier
 * @param ...       : liste de chaine de nombre (de type (int8_t *))
 * @return          : true / false
 */
_Bool check_sont_des_nombres_flottants(uint8_t nb_nombre, ...)
{
    va_list liste_param;
    va_start(liste_param, nb_nombre);
    int i = 0;
    
    for (i = 0 ; i < nb_nombre ; i++)
    {
        if (check_est_un_nombre(va_arg(liste_param, uint8_t*)) == false)
        {
            print_pas_nombre_flottant();
            return false;
        }
    }
    return true;
}


/**
 * Fonction qui check la cohérence des octets reçus
 * (Pas de débordement de tableaux par exemple)
 * @return  : true / false
 */
_Bool check_erreur_serialus()
{
    uint8_t i = 0;
    _Bool result = false;
    
    if (serialus.nb_param >= MAX_PARAM)
    {
        result = true;
    }
    else
    {
        for (i = 0 ; i < MAX_PARAM ; i++)
        {
            if (serialus.index[i] > MAX_BUFF)
                result = true;
        }
    }
    
    return result;
}


/**
 * Fonction qui compare une chaine du buffer de reception serialus et une chaine
 * de référence 
 * @param param     : numéro du paramère dans le buffer à comparer
 * @param string    : chaine de référence à comparer
 * @return          : true si identique / false sinon
 */
_Bool check_string (uint8_t param, char* string)
{
    uint8_t i = 0;
    _Bool result = false;
    
    // On check que la string de référence est identique au buffer
    // et qu'on ne déborde pas du buffer
    do
    {
        result = false;
        if (serialus.buffer[param][i] == string[i])
            result = true;
    }
    while(result == true && string[i++] != '\0' && i <= MAX_BUFF);
    
    // Si on a check l'intégralité du buffer mais pas encore rencontré de '\0'
    // Alors la chaine de référence est d'une part trop grande
    // Mais surtout les chaines ne sont pas comparés entirement donc false
    if (i > MAX_BUFF)
        result = false;
    
    return result;
}

/**
 * Fonction qui vérifie que le nombre de paramètre reçu correspond au nombre
 * exact de paramètres attendus
 * @param nombre_attendu    : Nombre de parametre attendus
 * @return                  : true / false
 */
_Bool check_nb_param (uint8_t nombre_attendu)
{
    _Bool result = true;
    
    if (serialus.nb_param < nombre_attendu)
    {
        result = false;
        print_manque_parametre();
    }
    else if (serialus.nb_param > nombre_attendu )
    {
        result = false;
        print_trop_parametre();
    }
    return result;
}

/**
 * Print incohérent si le test envoyé et faux
 * @param test  : renvoir le résultat du test (true / false)
 * @return 
 */
_Bool check_coherence (_Bool test)
{
    if (test == false)
    {
        print_incoherent();
    }
    return test;
}

_Bool check_confirmation()
{
    _Bool result = true;
    
    do
    {
        print_confirm();
        serialus.erreur_confirmation = false;
        serialus.attente_confirmation = true;
        while(serialus.attente_confirmation == true);
    }while(serialus.erreur_confirmation == true);
    
    if (serialus.confirmation == false)
    {
        result = false;
        print_abort_confirmation();
    }
    return result;
}


/******************************************************************************/
/************************ FONCTIONS DE PRINT **********************************/
/******************************************************************************/

void list_serialus (uint8_t truc_a_lister)
{
    printf("\n\rListe des fonctions disponibles");
    switch(truc_a_lister)
    {
        case SER_LIST :

            printf(" :");
            printf("\n\rPour lister : list");
            printf("\n\rDEPLACEMENT  de");
            printf("\n\rCALAGE       ca");
            printf("\n\rASSERV       as");
            printf("\n\rODOMETRIE    odo");
            printf("\n\rPOSITION     pos");
            printf("\n\rAX12         ax");
            break;
            
        case SER_DEPLACEMENT :
            printf(" pour de :");
            printf("\n\rCIBLER      : cib");
            printf("\n\rORIENTER    : ori");
            printf("\n\rREJOINDRE   : rej");
            printf("\n\rAVANCER     : av");
            printf("\n\rRECULER     : rec");
            printf("\n\rTOURNER     : fdt");
            printf("\n\rTRAPEZE     : tra");
            printf("\n\rCARRE       : ca");
            printf("\n\rBRAKE       : brake");
            printf("\n\rUNBRAKE     : unbrake");
            break;
            
        case SER_CALAGE :
            printf(" pour ca :");
            printf("\n\rCAL_ROBOT   : cal");
            printf("\n\rCAL_X       : cx");
            printf("\n\rCAL_Y       : cy");
            printf("\n\rCAL_Teta    : ct");
            break;
            
        case SER_ASSERV :
            printf("pour as :");
            printf("\n\rPas de liste pour le moment");
            break;
            
        case SER_POSITION :
            printf("pour pos :");
            printf("\n\rPour Get    : get x");
            printf("\n\rPour Set    : set x");
            printf("\n\r\tX, Y, Teta  : rob");
            printf("\n\r\tX           : x");
            printf("\n\r\tY           : y");
            printf("\n\r\tTeta        : t");
            break;
            
        case SER_ODOMETRIE :
            printf("pour odo : ");
            printf("\n\rAucune fonction");
            break;
            
        case SER_AX12 :
            printf("pour ax : ");
            printf("\n\rPour Set    : set x");
            printf("\n\r\tangle     : ang");
            printf("\n\r\tposition  : pos");
            printf("\n\r\tled       : led");
            printf("\n\rPour Get    : get x");
            printf("\n\r\tposition  : pos");
            printf("\n\r\tping      : ping");
            printf("\n\r... a venir ?");
            printf("\n\r");
            break;
    }
    printf("\n\r");
   
}

void print_non_reconnu(uint8_t numero)
{
    printf("\n\rParametre \"%s\" non reconnu", serialus.buffer[numero]);
}

void print_manque_parametre()
{
    printf("\n\rIl n'y a pas assez de parametre");
}

void print_trop_parametre()
{
    printf("\n\rIl y a trop de parametre");
}

void print_pas_nombre()
{
    printf("\n\rUn des parametre recu n'est pas un nombre entier comme attendu");
}

void print_pas_nombre_flottant()
{
    printf("\n\rUn des parametre recu n'est pas un nombre flottant comme attendu");
}

void print_incoherent()
{
    printf("\n\rUn des parametre recu est incoherent, action refusee");
}

void print_abort_confirmation()
{
    print_abort("no received !");
}

void print_confirm()
{
     printf("\n\rPlease confirm : (y/n) ?");
}

void print_erreur_deplacement(_enum_erreur_asserv erreur)
{
    serialus.clignotement_en_cours = false;
    switch(erreur)
    {
        case EVITEMENT :
            print_abort("Evitement");
            break;
        case BLOCAGE :
            print_abort("Blocage");
            break;
        case DEPLACEMENT_NORMAL :
            break;
    }
    print_position();
}

void print_erreur_ax12()
{
    switch(ax12.erreur)
    {
        case PAS_D_ERREUR :  
            break;
        case TIME_OUT :
            print_abort("TIME OUT");
            break;
        case ERREUR_CS :
            print_abort("Erreur checksum");
            break;
        case LIMITATION_DE_COURANT :
            print_abort("Limitation de courant");
            break;
        case AUTRE_ERREUR :
            print_abort("erreur ax inconnu");
        default :
            break;
    }  
}

void print_ping(uint8_t id)
{
    printf("\n\rID : %d", id);
}

void print_position_ax12(uint8_t id, int16_t position)
{
    printf(" ID : %d = %d", id, position);
}

void init_clignotement()
{
    printf("\n");
    serialus.clignotement_en_cours = true;
}

void print_clignotement()
{
    static uint8_t tempo = 0;
    tempo ++;
    if (tempo == 2)
        printf("\r -");
    else if (tempo == 4)
    {
        printf("\r  ");
        tempo = 0;
    }
}

#endif

/******************************************************************************/
/**************** FONCTION DE PRINT UTILISEES AILLEURS ************************/
/******************************************************************************/

void print_position()
{
#ifndef NO_SERIALUS
    printf("\n\rX: %f, Y : %f, Teta : %f\n",(float) get_X(), (float) get_Y(),(float) get_orientation());
#endif
}

void print_abort(char* raison)
{
#ifndef NO_SERIALUS
    printf("\n\rAbort : %s", raison);
#endif
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/