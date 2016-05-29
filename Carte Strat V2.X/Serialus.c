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
    
    serialus.attente_confirmation = false;
    serialus.confirmation = false;
    serialus.erreur_confirmation = false;
    serialus.info_a_traiter = false;
    serialus.nb_param = 0;
    
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
    if (serialus.info_a_traiter == false)
    {
        if (buf == 3) // CTRL-C
        {
            reinit_buffer();
            printf("\n\r");
        }
        else
        {
            putchar(buf);
            reception_serialus(buf);
        }
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
            serialus_traitement_position();
        }
        else if (check_string(0, "list"))
        {
            serialus_traitement_list();
        }
        // Code Fonction non reconnu
        else
        {
            print_non_reconnu(0);
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
                   printf("\n\rcibler X : %d, Y : %d, V : %d",(int) x,(int) y,(int) v);
                   //cibler(x, y, v);
                   //print_position();
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
                    printf("\n\rorienter Teta : %d, V : %d",(int) a,(int) v);
                    //orienter(a, v);
                    //print_position();
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
                    printf("\n\rrejoindre X : %d, Y : %d,sens : %d, V : %d",(int) x,(int) y,(int) s,(int) v);
                    //rejoindre(x, y, s, v);
                    //print_position();
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
                    printf("\n\ravancer d : %d, V : %d",(int) d,(int) v);
                    //avancer_reculer(d, v);
                    //print_position();
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
                    printf("\n\rreculer d : %d, V : %d",(int) d,(int) v);
                    //avancer_reculer(-d, v);
                    //print_position();
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
                    printf("\n\rtourner de %d tours", nb);
                    //faire_des_tours(nb);
                    //print_position();
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
            printf("\n\rje fais un trapeze");
            //trapeze(MARCHE_AVANT);
            //print_position();
        }
    }
    // Fonction carré
    else if (check_string(1, "ca"))
    {
        if (check_nb_param(1))
        {
            printf("\n\rje fais un carre");
            //carre(MARCHE_AVANT);
            //print_position();
        }
    }
    // Fonction brake
    else if (check_string(1, "brake"))
    {
        if (check_nb_param(1))
        {
            printf("\n\rJe brake");
            //brake();
        }
    }
    // Fonction unbrake
    else if (check_string(1, "unbrake"))
    {
        if (check_nb_param(1))
        {
            printf("\n\rJ'unbrake");
            //unbrake();
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
                    printf("\n\rCalage d : %d, V : %d",(int) d,(int) v);
                    calage(d, v);
                    //print_position();
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
                    printf("\n\rCalage en X x : %d, teta : %d, sens : %d, vitessse : %d",(int) x,(int) t, (int) s, (int) v);
                    //calage_X(x, t, s, v);
                    //print_position();
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
                    printf("\n\rCalage en Y y : %d, teta : %d, sens : %d, vitessse : %d",(int) y,(int) t, (int) s, (int) v);
                    //calage_Y(y, t, s, v);
                    //print_position();
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
                    printf("\n\rCalage en Teta,  teta : %d, sens : %d, vitessse : %d",(int) t, (int) s, (int) v);
                    //calage_teta(t, s, v);
                    //print_position();
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
                        do
                        {
                            printf("\n\rPlease confirm : Init X : %lf, Y : %lf, Teta : %lf (y/n) ? : ", x, y, t);
                            serialus.erreur_confirmation = false;
                            serialus.attente_confirmation = true;
                            while(serialus.attente_confirmation == true);
                        }while(serialus.erreur_confirmation == true);
                        
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
                        do
                        {
                            printf("\n\rPlease confirm : Init X : %lf (y/n) ? : ", x);
                            serialus.erreur_confirmation = false;
                            serialus.attente_confirmation = true;
                            while(serialus.attente_confirmation == true);
                        }while(serialus.erreur_confirmation == true);

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
                        do
                        {
                            printf("\n\rPlease confirm : Init Y : %lf (y/n) ? : ", y);
                            serialus.erreur_confirmation = false;
                            serialus.attente_confirmation = true;
                            while(serialus.attente_confirmation == true);
                        }while(serialus.erreur_confirmation == true);

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
                        do
                        {
                            printf("\n\rPlease confirm : Init Teta : %lf (y/n) ? : ", t);
                            serialus.erreur_confirmation = false;
                            serialus.attente_confirmation = true;
                            while(serialus.attente_confirmation == true);
                        }while(serialus.erreur_confirmation == true);

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
                printf("\n\r Position X : %lf", get_X());
            }
        }
        else if (check_string(2, "y"))
        {
            if (check_nb_param(2))
            {
                printf("\n\r Position Y : %lf", get_Y());
            }
        }
        else if (check_string(2, "t"))
        {
            if (check_nb_param(2))
            {
                printf("\n\r Orientation : %lf", get_orientation());
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
    if (serialus.nb_param < 1)
    {
        print_manque_parametre();
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
    // Mais surtout on les chaines ne sont pas comparés entirement donc false
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
            printf("\n\rPour Get    : get x");
            printf("\n\rPour Set    : set x");
            printf("\n\r\tangle     : ang");
            printf("\n\r\tposition  : pos");
            printf("\n\r\tping(get) : ping");
            printf("\n\r\tled       : led");
            printf("\n\r\t... a venir");
            printf("\n\r");
    }
    printf("\n\rPour lister : list");
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

void print_position()
{
    printf("\n\rX: %lf, Y : %lf, Teta : %lf\n", get_X(), get_Y(), get_orientation());
}

void print_abort_confirmation()
{
    printf("\n\rAbort no received !");
}


