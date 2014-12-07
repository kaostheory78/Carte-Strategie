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
    #include <stdio.h>
    #include <stdlib.h>
    #include "codeurs.h"
    #include <math.h>
    #include "Uart.h"
    #include "interruptions.h"
    #include "asserv.h"
    #include "pwm.h"
    #include "Config_robots.h"
    

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/


    //Macro pour les tempos
#define FOSC             80017142LL      //80000000
#define FCY             (FOSC/2)
#define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
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

#define SENS_MOTEUR_GAUCHE      (PORTBbits.RB13)
#define SENS_MOTEUR_DROIT       (PORTBbits.RB11)

#define XBEE_RESET              PORTAbits.RA7
#define INHIBIT_AX12            PORTAbits.RA10
#define DIR_UART_AX12           PORTBbits.RB7 

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
#define PRIO_INTER_TIMER2               0   // Timer 2 en mode 32 bits
#define PRIO_INTER_UART1_RX             1   // Prio XBEE
#define PRIO_INTER_TIMER3               6   // Timer Fin de match
#define PRIO_INTER_I2C_MAITRE           5   // I²C désactivé pour le moment
#define PRIO_INTER_I2C_ESCLAVE          4   // I²C désactivé pour le moment
#define PRIO_INTER_TIMER4               2   // Autom
#define PRIO_INTER_TIMER5               1   //Prio Timer debug
#define PRIO_INTER_UART2_RX             3   // AX12
#define PRIO_INTER_QEI1                 7   // Codeurs : prio la plus haute
#define PRIO_INTER_QEI2                 7   // Codeurs : prio la plus haute

#define ACTIV_INTER_UART1_RX            0   // Uart XBEE
#define ACTIV_INTER_TIMER3              1   // Timer 90 secondes : fin de match
#define ACTIV_INTER_TIMER2              0   // Osef : TIMER 2 et 3 sur 32 bits
#define ACTIV_INTER_TIMER1              1   // Timer asserv : 5 ms
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

    extern int OVERFLOW_CODEUR[3] ;
    extern _position position[3];
    extern _robot ROBOT;

    extern _PID PID_DISTANCE;
    extern _PID PID_ORIENTATION;

    extern _flag_asserv FLAG_ASSERV;

    extern _systeme_asserv DISTANCE;
    extern _systeme_asserv ORIENTATION;
    extern _systeme_asserv VITESSE[3];

    extern _erreur ERREUR_DISTANCE;
    extern _erreur ERREUR_ORIENTATION;

    extern _commande_moteur COMMANDE;

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

    void init_clock(void);
    void config_timer_5ms();
    void config_timer_10ms();
    void config_timer_90s();

    void start_timer_90s (void);


    void odometrie (unsigned char mode);

    void ConfigInterrupt (void);
    void ConfigPorts (void);
    void ConfigMapping (void);
    



#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

