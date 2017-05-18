/* 
 * File:   system.h
 * Author: Quentin
 *
 * Created on 23 octobre 2014, 12:00
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/
    
    #include <libpic30.h>
    #include <p33FJ128MC804.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdarg.h>
    #include <stdbool.h>
    #include "codeurs.h"
    #include <math.h>
    #include "Uart.h"
    #include "interruptions.h"
    #include "asserv.h"
    #include "pwm.h"
    #include "Config_robots.h"
    #include "fonctions_deplacements.h"
    #include "gestion_AX12.h"
    #include "autom.h"
    #include "Serialus.h"
    #include "evitement.h"
    

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


    //Macro pour les tempos
#define FOSC        80000000LL     //80017142LL      //80000000
#define FCY             (FOSC/2)
#define delay_us(x) __delay32(((x*FCY)/1000000L))   // delays x us
#define delay_ms(x) __delay32(((x*FCY)/1000L))      // delays x ms


/******************************************************************************/
/********************************* Timers *************************************/
/******************************************************************************/
 
#define ACTIVE                  0b1
#define DESACTIVE               0b0

#define DIGITAL                 1
#define ANALOGIQUE              0
    
#define ETAT_HAUT               1
#define ETAT_BAS                0

#define TIMER_5ms               T1CONbits.TON
#define TIMER_10ms              T2CONbits.TON
#define TIMER_20ms              T3CONbits.TON
#define TIMER_100ms             T4CONbits.TON
#define TIMER_200ms             T5CONbits.TON

#define FLAG_TIMER_5ms          IFS0bits.T1IF
#define FLAG_TIMER_10ms         IFS0bits.T2IF
#define FLAG_TIMER_20ms         IFS0bits.T3IF
#define FLAG_TIMER_100ms        IFS1bits.T4IF
#define FLAG_TIMER_200ms        IFS1bits.T5IF

/******************************************************************************/
/********************************  PORT CARTE   *******************************/
/******************************************************************************/

#define SYS_COULEUR             _SYS_COULEUR
#define SYS_STRAT               _SYS_STRAT


#ifdef CARTE_V1
    #define SENS_MOTEUR_DROIT   PORTBbits.RB11
    #define SENS_MOTEUR_GAUCHE  PORTBbits.RB13

    #define XBEE_RESET          PORTAbits.RA7
    #define INHIBIT_AX12        PORTAbits.RA10
    #define DIR_UART_AX12       PORTBbits.RB7


    #define SYS_JACK            PORTBbits.RB4

    //Du JACK  vers les codeurs
    #define CAPTEUR1            PORTAbits.RA4
    #define CAPTEUR2            PORTAbits.RA9
    #define CAPTEUR3            PORTCbits.RC3
    #define CAPTEUR4            PORTCbits.RC4

    //Carte d'extenssion
    //De la gauche vers la droite (nappe en bas)
    #define CAPTEUR5            PORTAbits.RA8
    #define CAPTEUR6            PORTAbits.RA3
    #define CAPTEUR7            PORTAbits.RA2
    #define CAPTEUR8            PORTCbits.RC1
    #define CAPTEUR9            PORTCbits.RC2
    #define CAPTEUR10           PORTCbits.RC0
#endif
#ifdef CARTE_V2
    #define SENS_MOTEUR_DROIT   PORTBbits.RB12
    #define SENS_MOTEUR_GAUCHE  PORTBbits.RB10
    #define SENS_MOTEUR_X       PORTBbits.RB14
    #define SENS_MOTEUR_Y       PORTAbits.RA10
    #define ALIM_MOTEUR_Y       PORTAbits.RA7

    #define XBEE_RESET          PORTBbits.RB7
    #define INHIBIT_AX12        PORTAbits.RA0
    #define DIR_UART_AX12       PORTAbits.RA1

    #define SYS_JACK            PORTAbits.RA9
    #define _SYS_COULEUR        PORTCbits.RC3
    #define _SYS_STRAT          PORTCbits.RC4

    #define CAPTEUR1            PORTCbits.RC0
    #define CAPTEUR2            PORTCbits.RC1
    #define CAPTEUR3            PORTCbits.RC2
    #define CAPTEUR4            PORTAbits.RA2
    #define CAPTEUR5            PORTAbits.RA3 
    #define CAPTEUR6            PORTAbits.RA8   
    #define CAPTEUR7            PORTBbits.RB4   
    #define CAPTEUR8            PORTAbits.RA4

    #define CAPTEUR1_ANALOGIQUE _CAPTEUR1_ANALOGIQUE
    #define CAPTEUR2_ANALOGIQUE _CAPTEUR2_ANALOGIQUE
    #define CAPTEUR3_ANALOGIQUE _CAPTEUR3_ANALOGIQUE

#endif
#ifdef CARTE_BALISE
    #define SENS_MOTEUR_DROIT   PORTCbits.RC0
    #define SENS_MOTEUR_GAUCHE  LATAbits.LATA10 // Led 2

    #define XBEE_RESET          PORTCbits.RC6 // enable BL
    #define INHIBIT_AX12        PORTCbits.RC2 // enable capteur
    #define DIR_UART_AX12       LATAbits.LATA7 // Led 3


    #define SYS_JACK            PORTCbits.RC9 // bouton 1
    #define _SYS_COULEUR        PORTCbits.RC8 // bouton 2
    #define _SYS_STRAT          PORTCbits.RC7 // bouton 3

    // LED 2 -  9
    #define CAPTEUR1            LATAbits.LATA0 // led 4
    #define CAPTEUR2            LATAbits.LATA1 // led 5
    #define CAPTEUR3            LATBbits.LATB0 // led 6 
    #define CAPTEUR4            LATBbits.LATB1 // led 7
    #define CAPTEUR5            LATBbits.LATB2 // led 8
    #define CAPTEUR6            LATBbits.LATB3 // led 9
    #define CAPTEUR7            LATBbits.LATB4 // free pin
    #define CAPTEUR8            LATAbits.LATA2 // free pin
#endif


/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/

#define TENSION_MOTEUR_DROIT    _TENSION_MOTEUR_DROIT
#define TENSION_MOTEUR_GAUCHE   _TENSION_MOTEUR_GAUCHE

#define TENSION_SORTIE_PTN      _TENSION_SORTIE_PTN

/******************************************************************************/
/******************************* Interruptions  *******************************/
/******************************************************************************/

#define PRIO_INTER_TIMER1               6   // Timer Assev
#define PRIO_INTER_TIMER2               4   // Timer Evitement
#define PRIO_INTER_TIMER3               3   // Timer Autom
#define PRIO_INTER_TIMER4               5   // Timer scheduleur
#define PRIO_INTER_TIMER5               1   // Timer Serialus

#define PRIO_INTER_I2C_MAITRE           0   // I²C désactivé pour le moment
#define PRIO_INTER_I2C_ESCLAVE          0   // I²C désactivé pour le moment

#define PRIO_INTER_UART1_RX             2   // Prio XBEE
#define PRIO_INTER_UART2_TX             4   // AX12
#define PRIO_INTER_UART2_RX             4   // AX12
    
#define PRIO_INTER_QEI1                 7   // Codeurs : prio la plus haute
#define PRIO_INTER_QEI2                 7   // Codeurs : prio la plus haute


#define ACTIV_INTER_TIMER1              1   // Timer asserv : 5 ms
#define ACTIV_INTER_TIMER2              0   // Osef : TIMER 2 et 3 sur 32 bits    
#define ACTIV_INTER_TIMER3              1   // Timer 90 secondes : fin de match
#define ACTIV_INTER_TIMER4              1   // Timer Autom : 10 ms
#define ACTIV_INTER_TIMER5              1   // Timer debug : 200 ms
    
#define ACTIV_INTER_I2C_MAITRE          0   // Pas implémenté pour le moment
#define ACTIV_INTER_I2C_ESCLAVE         0   // Pas implémenté pour le moment    
    
#define ACTIV_INTER_UART1_RX            1   // Uart XBEE
#define ACTIV_INTER_UART2_TX            1   // UART AX12 : Acquittement trame envoyée
#define ACTIV_INTER_UART2_RX            1   // UART AX12
    
#define ACTIV_INTER_QEI1                1   // Codeurs
#define ACTIV_INTER_QEI2                1   // Codeurs

/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

typedef struct
{
    uint32_t t_ms   :31;
    _Bool actif     : 1;
}_compteur_temps_match;

/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

    /**
     * Toutes les variables appellées par la le timer d'asserv de 5 ms
     * sont définies en near 
     * Elles sont ainsi stockée dans la mémoire de 8 ko proche du cpu
     * et on y accède plus rapidement
     */
    
    /**
     * Toutes les variables dont la valeurs est succeptibles d'être modifiée
     * dans une interruption doivent être déclarée volatile !
     */
    
    //Codeurs
    extern volatile __attribute__((near)) int8_t OVERFLOW_CODEUR[3] ;
    extern volatile __attribute__((near)) _position POSITION[3];

    //Asserv
    extern volatile __attribute__((near)) _robot ROBOT;
    extern volatile __attribute__((near)) _acc acc;
    extern volatile __attribute__((near)) _PID PID;
    extern volatile __attribute__((near)) _flag_asserv FLAG_ASSERV[AUTOM_ID_MAX_NB];

    extern volatile __attribute__((near)) _systeme_asserv X;
    extern volatile __attribute__((near)) _systeme_asserv Y;
    extern volatile __attribute__((near)) _systeme_asserv DISTANCE;
    extern volatile __attribute__((near)) _systeme_asserv ORIENTATION;
    extern volatile __attribute__((near)) _systeme_asserv VITESSE_ORIENTATION[3];
    extern volatile __attribute__((near)) _systeme_asserv VITESSE[3];
    extern volatile __attribute__((near)) _vitesse VITESSE_MAX;
    
    
    extern volatile __attribute__((near)) _erreur ERREUR_DISTANCE;
    extern volatile __attribute__((near)) _erreur ERREUR_ORIENTATION;
    extern volatile __attribute__((near)) _erreur ERREUR_VITESSE[2];

    extern volatile __attribute__((near)) _commande_moteur COMMANDE;
    
    //Evitement adversaire
    extern volatile _evitement EVITEMENT_ADV;
    
    extern volatile __attribute__((near)) _compteur_temps_match CPT_TEMPS_MATCH;
    extern volatile _enum_couleurs COULEUR;
    
    // FLAG Automs
    extern volatile _enum_flag_action FLAG_ACTION[AUTOM_ID_MAX_NB];
    extern volatile _timer_event timer_event[AUTOM_ID_MAX_NB];
    extern volatile _ax12_event ax12_event[AUTOM_ID_MAX_NB];
    extern volatile _sync_event sync_event[AUTOM_ID_MAX_NB];

    // Ax12 variables
    extern volatile _ax12 ax12;
    extern volatile _Bool CHECK_LIMITATION_COURANT;
    
#ifndef NO_SERIALUS
    extern volatile _serialus serialus;
#endif
    
    // Evitement Hugo : pas optimisé ....
    extern int obstacle[x_max][y_max];
    extern int curseur;
	extern int calcul_en_cours;
    extern int curseur_obstacle;
    extern int itineraire[nb_point_max][3];
    extern int itineraire_basique[nb_point_max][3];
    extern int itineraire_court[nb_point_max][3];
    extern int point_obstacle[nb_point_max][3];
    extern int x_actuel;
    extern int y_actuel;
    extern int x_souhaite;
    extern int y_souhaite;
    extern int x_obj;
    extern int y_obj;
    extern int id_evitement;
    extern int id_evitement_initial;
    extern int chemin_court;
    extern uint16_t compteur_evitement;
    extern int precedent_obstacle[7];




/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void ConfigADC (void);

    /**
     * Fonction qui initialise tous les systèmes de la carte
     * Uart, Timer, PWM, mapping ....
     */
    void init_system (void);


    /**
     * Fonction qui rèfle l'horloge à 80Mhz
     */
    void init_clock(void);
    
    /**
     * Fonction qui initialise le compteur de temps de match à zéro
     */
    void init_compteur_temps_match();

    /**
    * Configuration du Timer de 5 ms pour l'asservissement
    *        /!\  ATTENTION : TIMER SAFE   /!\
    * /!\ INTERDICTION D'UTILISER DES FONCTIONS BLOQUANTES /!\
    * /!\ OU DE DESACTIVER LE DECOMPTE DU TIMER PENDANT   /!\
    * /!\ L'INTERRUPTION, MERCI !                         /!\       
    */
    void config_timer_5ms();


   /**
    * Configuration du Timer de 10 ms pour l'évitement
    *        /!\  ATTENTION : TIMER SAFE   /!\
    * /!\ INTERDICTION D'UTILISER DES FONCTIONS BLOQUANTES /!\
    * /!\ OU DE DESACTIVER LE DECOMPTE DU TIMER PENDANT   /!\
    * /!\ L'INTERRUPTION, MERCI (PAS D'AX12) !            /!\       
    */
    void config_timer_10ms();


    /**
    * Configuration du Timer de 20 ms pour l'autom
    *      NB : TIMER NON SAFE 
    *  POSSIBILITE D'UTILISER DES FONCTIONS BLOQUANTES
    *  UTILISE POUR DES TACHES NON CRITIQUES     
    */
    void config_timer_20ms();
    
    
    /**
    * Configuration du Timer de 100 ms pour le scheduler temps de match
    *      NB : TIMER NON SAFE 
    *  POSSIBILITE D'UTILISER DES FONCTIONS BLOQUANTES
    *  UTILISE POUR DES TACHES NON CRITIQUES     
    */
    void config_timer_100ms();
    
    
    /**
    * Configuration du Timer de 200 ms pour la liaison serie (debug + serialus)
    *      NB : TIMER NON SAFE 
    *  POSSIBILITE D'UTILISER DES FONCTIONS BLOQUANTES
    *  UTILISE POUR DES TACHES NON CRITIQUES     
    */
    void config_timer_200ms();


    /**
     * Fonction qui configure toutes les interruptions
     * Ainsi que les priorités
     */
    void ConfigInterrupt (void);


    /**
     * Fonction qui configure toures les broches entrées sorties
     */
    void ConfigPorts (void);


    /**
     * Configure le mappage des modules spéciaux : UART, QEI
     */
    void ConfigMapping (void);

    /**
     * Fonction stratégie du robot
     */
    void strategie();

    void homologation();
    void reglage_odometrie();
    



#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

