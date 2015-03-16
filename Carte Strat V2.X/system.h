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

#define TIMER_5ms               T1CONbits.TON
#define TIMER_10ms              T4CONbits.TON
#define TIMER_90s               T2CONbits.TON
#define TIMER_DEBUG             T5CONbits.TON

#define FLAG_TIMER_5ms          IFS0bits.T1IF
#define FLAG_TIMER_10ms         IFS1bits.T4IF
#define FLAG_TIMER_90s          IFS0bits.T3IF
#define FLAG_TIMER_DEBUG        IFS1bits.T5IF

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

    #define XBEE_RESET          PORTAbits.RB7
    #define INHIBIT_AX12        PORTAbits.RA0
    #define DIR_UART_AX12       PORTAbits.RA1

    #define SYS_JACK            PORTCbits.RC3
    #define _SYS_COULEUR        PORTAbits.RA9
    #define _SYS_STRAT          PORTCbits.RC4

    #define CAPTEUR1            PORTCbits.RC0
    #define CAPTEUR2            PORTCbits.RC1
    #define CAPTEUR3            PORTCbits.RC2
    #define CAPTEUR4            PORTAbits.RA2
    #define CAPTEUR5            PORTAbits.RA3
    #define CAPTEUR6            PORTAbits.RA8
    #define CAPTEUR7            PORTBbits.RB4
    #define CAPTEUR8            PORTAbits.RA4
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

#define PRIO_INTER_TIMER1               5   // Timer Assev
#define PRIO_INTER_TIMER2               0   // Timer 2 en mode 32 bits (couplé à T3 donc osef)
#define PRIO_INTER_UART1_RX             1   // Prio XBEE
#define PRIO_INTER_TIMER3               6   // Timer Fin de match
#define PRIO_INTER_I2C_MAITRE           5   // I²C désactivé pour le moment
#define PRIO_INTER_I2C_ESCLAVE          4   // I²C désactivé pour le moment
#define PRIO_INTER_TIMER4               2   // Autom
#define PRIO_INTER_TIMER5               1   //Prio Timer debug
#define PRIO_INTER_UART2_TX             3   // AX12
#define PRIO_INTER_UART2_RX             3   // AX12
#define PRIO_INTER_QEI1                 7   // Codeurs : prio la plus haute
#define PRIO_INTER_QEI2                 7   // Codeurs : prio la plus haute

#define ACTIV_INTER_UART1_RX            1   // Uart XBEE
#define ACTIV_INTER_TIMER3              1   // Timer 90 secondes : fin de match
#define ACTIV_INTER_TIMER2              0   // Osef : TIMER 2 et 3 sur 32 bits
#define ACTIV_INTER_TIMER1              1   // Timer asserv : 5 ms
#define ACTIV_INTER_UART2_TX            1   // UART AX12 : Acquittement trame envoyée
#define ACTIV_INTER_UART2_RX            1   // UART AX12
#define ACTIV_INTER_TIMER4              1   // Timer Autom : 10 ms
#define ACTIV_INTER_TIMER5              1   // Timer debug : 200 ms
#define ACTIV_INTER_I2C_MAITRE          0   // Pas implémenté pour le moment
#define ACTIV_INTER_I2C_ESCLAVE         0   // Pas implémenté pour le moment
#define ACTIV_INTER_QEI1                1   // Codeurs
#define ACTIV_INTER_QEI2                1   // Codeurs


/******************************************************************************/
/*************************** Variables Globales *******************************/
/******************************************************************************/

    //Codeurs
    extern int OVERFLOW_CODEUR[3] ;
    extern _position position[3];

    //Asserv
    extern _robot ROBOT;
    extern _acc acc;
    extern _PID PID;
    extern _flag_asserv FLAG_ASSERV;

    extern _systeme_asserv X;
    extern _systeme_asserv Y;
    extern _systeme_asserv DISTANCE;
    extern _systeme_asserv ORIENTATION;
    extern _systeme_asserv VITESSE_ORIENTATION[3];
    extern _systeme_asserv VITESSE[3];

    extern double VITESSE_MAX_ORIENTATION;
    extern double VITESSE_MAX_POSITION;

    extern _erreur ERREUR_DISTANCE;
    extern _erreur ERREUR_ORIENTATION;
    extern _erreur ERREUR_VITESSE[2];

    extern _commande_moteur COMMANDE;

    extern uint8_t TYPE_CONSIGNE;

    //Evitement adversaire
    extern uint8_t DETECTION;
    extern uint8_t EVITEMENT_ADV_AVANT;
    extern uint8_t STRATEGIE_EVITEMENT;
    extern uint8_t COULEUR;
    extern uint16_t FLAG_ACTION;
    extern uint8_t ETAT_AUTOM;

    extern _ax12 ax12;

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

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
     * Fonction qui init le Timer asserv
     */
    void config_timer_5ms();


    /**
     * Focntion qui init le Timer d'autom
     */
    void config_timer_10ms();


    /**
     * Fonction qui init le timer de fin de match
     */
    void config_timer_90s(void);


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
    



#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

