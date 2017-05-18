/* 
 * File:   interruptions.h
 * Author: Quentin
 *
 * Created on 9 novembre 2014, 00:53
 */

#ifndef INTERRUPTIONS_H
#define	INTERRUPTIONS_H

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "Config_robots.h"

/******************************************************************************/
/***************************** Defines ****************************************/
/******************************************************************************/

#define AX12_OBSERVER_DEFAULT_TIMER_MS          200 
#define SYNC_EVENT_OBSERVER_DEFAULT_TIMER_MS    200
#define MAX_SYNC_EVENT                          5

/******************************************************************************/
/********************** DEFINITION DES STRUCTURES *****************************/
/******************************************************************************/

typedef struct
{
    bool timer_actif;
    uint32_t temps_echeance;
    _enum_flag_action event;
}_timer_event;

typedef struct
{
    _autom_id autom_id;
    _enum_flag_action event;
    uint32_t time_to_throw_event;
    bool is_observed[ID_MAX_AX12];
}_ax12_event;

typedef struct
{
    _autom_id autom_id;
    _enum_flag_action event;
}_sync_event_unary;

typedef struct
{
    _autom_id autom_listening_event;
    _enum_flag_action event_to_send;
    uint32_t timer_ms;
    uint8_t nb_event_registred;
    _sync_event_unary event_regisred[5];
}_sync_event;

/******************************************************************************/
/****************************** Prototypes ************************************/
/******************************************************************************/

void arm_timer(_autom_id id, uint32_t t_ms, uint8_t event, bool wait_for_event);
void void cancel_timer(_autom_id id);
void init_ax12_event(_autom_id autom_id);
void register_ax12_event(uint8_t ax12_ID, _autom_id autom_id, _enum_flag_action event, uint32_t timer_ms);
void register_multiple_ax12_event(uint8_t nb_ax12, _autom_id autom_id, _enum_flag_action event, uint32_t timer_ms, ...);
void check_ax12_event(_autom_id autom_id);
void check_timer_event();
void register_sync_event (_autom_id autom_id, _enum_flag_action event_to_send, uint32_t timer_ms, uint8_t nb_event_registred, ...);
void check_sync_event(_autom_id autom_id);

/**
 *  Interruption du Timer 1 : Asserv
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void);



/**
 * Timer fin de match : 90 secondes
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void);



/**
 * Timer 10 ms : Autom et capteurs
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void);


/**
 * Interruption sur codeur droit
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI1Interrupt(void);


/**
 * Interruption sur codeur gauche
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI2Interrupt(void);


/**
 * Interruption UART_XBEE (ou RS232) sur RX
 */
void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void);


/**
 * Interruption UART AX12 sur RX
 */
void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void);

#endif	/* INTERRUPTIONS_H */

