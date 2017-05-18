/******************************************************************************/
/************** Carte principale Robot 1 : DSPIC33FJ128MC804*******************/
/******************************************************************************/
/* Fichier 	: interruptions.c
 * Auteur  	: Quentin
 * Revision	: 1.0
 * Date		: 08/11/2014
 *******************************************************************************
 *
 *
 ******************************************************************************/

/******************************************************************************/
/******************************** INCLUDES ************************************/
/******************************************************************************/

#include "system.h"


/******************************************************************************/
/*********************** INTERRUPTION DES TIMERS ******************************/
/******************************************************************************/


/**
 *  Timer 5 ms : Asserv
 *  /!\ FONCTION BLOQUANTE INTERDITE !!!!  
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    FLAG_TIMER_5ms = 0;

    // odométrie
   calcul_position_robot();

   if (FLAG_ASSERV.totale == ON)
        asserv();
   
   if (CPT_TEMPS_MATCH.actif == true)
       CPT_TEMPS_MATCH.t_ms +=  5;
   
   // Clignotement d'une led de ALIVE
   __attribute__((near)) static int led = 1, compteur = 0;
   compteur++;

    if (compteur == 100)
    {
       if (led == 1)
            led = 0;
        else
            led = 1;

#ifdef PETIT_ROBOT
        CAPTEUR3 = led;
#endif
        compteur = 0;
    }
}

/**
 *  Timer 10 ms : Check capteurs
 *  /!\ TIMER SAFE : FONCTION BLOQUANTE INTERDITE /!\ //
 *      /!\ PAS DE FONCTIONS AX12 ICI !!! /!\ //
 */
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    FLAG_TIMER_10ms = 0;
    
    evitement();
    
    // Compteur pour évitement Hugo
    if(calcul_en_cours == ON)
    {
        compteur_evitement++;
        if(compteur_evitement > ATTENTE_EVITEMENT)
            compteur_evitement = ATTENTE_EVITEMENT;
    }
    else
    {
        compteur_evitement =0;
    }
}

/**
 * Timer d'autom : 20 ms
 * Décompte relancé à la fin de la fonctions -> temps non fiable
 * UNSFAE : fonction bloquante autorisée
 */
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    
    TIMER_20ms = DESACTIVE;
    
    autom_20ms();
    
    #ifndef NO_SERIALUS   
    
    if (serialus.clignotement_en_cours == true)
    {
        #ifdef DEBUG_ACTIF
            debug();
        #endif        
    }
        
    #endif

    TMR3 = 0;
    FLAG_TIMER_20ms = 0;        //On clear le flag d'interruption du timer
    TIMER_20ms = ACTIVE;
}

/**
 * Timer 100 ms : Scheduleur temps de match
 */
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{  
    if (CPT_TEMPS_MATCH.t_ms >= 90000UL)
    {
        PORTCbits.RC5 = 0;

        //On désactive toutes les interruptions :
        IEC0bits.T1IE = 0;
        IEC0bits.T2IE = 0;
        IEC0bits.T3IE = 0;
        IEC1bits.T4IE = 0;
        IEC1bits.T5IE = 0;
        
        IEC3bits.QEI1IE = 0;
        IEC4bits.QEI2IE = 0;

    #ifdef CARTE_V2
        ALIM_MOTEUR_Y = DESACTIVE;
    #endif
        TIMER_5ms   = DESACTIVE;
        TIMER_10ms  = DESACTIVE;
        TIMER_20ms  = DESACTIVE;
        TIMER_100ms = DESACTIVE;

        envoit_pwm(MOTEUR_DROIT, 0);
        envoit_pwm(MOTEUR_GAUCHE, 0);
        envoit_pwm(MOTEUR_X, 0);

        // Pour permettre l'envoit de commande AX12 
        // Depuis ici
        IPC7bits.U2TXIP	= 7;
        IPC7bits.U2RXIP = 7;     
        
        // On coupe le couple de tous les ax12
        torque_enable_ax12(TOUS_LES_AX12, false);
                
        while(1);
    }
    
#ifndef NO_SERIALUS   
    if (serialus.clignotement_en_cours == true)
    {
        #ifndef DEBUG_ACTIF
            print_clignotement();
        #endif        
    }
        
#endif
    
    FLAG_TIMER_100ms = 0;        //On clear le flag d'interruption du timer
}

/**
 *  Timer 200 ms : Liaison série
 *  UNSAFE : Fonction bloquante autorisée 
 */
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
    TIMER_200ms = DESACTIVE;
    
#ifndef NO_SERIALUS
    if (serialus.actif == true)
        traitement_serialus();
#endif
    
    TMR5 = 0;
    FLAG_TIMER_200ms = 0;        //On clear le flag d'interruption du timer
    TIMER_200ms = ACTIVE;
}

/******************************************************************************/
/************************* GESTION DES TIMER SOFT *****************************/
/******************************************************************************/

// /!\ ATTENTION 1 SEUL A LA FOIS POUR LE MOMENT AVEC UNIQUEMENT EVENT FLAG_ACTION /!\ //

/**
 * définit une action à réaliser à la fin d'un temps donné
 * @param id    : id du timer (contexte d'appel)
 * @param t_ms  : temps dans lequel le timer se déclenche
 * @param event : event du flag action
 * @param wait_for_event : est ce qu'on reconfigure le flag_action pour attendre sur la fin du 
 * timer ou non
 */
void arm_timer(_autom_id id, uint32_t t_ms, uint8_t event, bool wait_for_event)
{
    timer_event[id].timer_actif = false;
    timer_event[id].event = event;
    timer_event[id].temps_echeance = CPT_TEMPS_MATCH.t_ms + t_ms;
    
    timer_event[id].timer_actif = true;
    
    if (wait_for_event)
    {
        FLAG_ACTION[id] = EN_ATTENTE_EVENT;
    }
}

/**
 * cancel_timer()
 * annule le timer en cours
 * @param id : id du timer (contexte d'appel)
 */
void cancel_timer(_autom_id id)
{    
    timer_event[id].timer_actif = false;
}

/**
 * check_timer_event()
 * check if a timer occured
 */
void check_timer_event()
{
    _autom_id autom_id;
    
    for (autom_id = AUTOM_ID_MIN_NB ; autom_id < AUTOM_ID_MAX_NB ; autom_id++)
    {
        if (timer_event[autom_id].timer_actif == true)
        {
            if (CPT_TEMPS_MATCH.t_ms >= timer_event[autom_id].temps_echeance)
            {
                timer_event[autom_id].timer_actif = false;
                FLAG_ACTION[autom_id] = timer_event[autom_id].event;       
            }
        }
    }  
}


/**
 * init_ax12_event()
 * initialise the global variale ax12_event
 * @param autom_id : id de l'event (contexte d'appel)
 */
void init_ax12_event(_autom_id autom_id)
{
    uint8_t ax12_id;
    
    ax12_event[autom_id].event = NE_RIEN_FAIRE;
    ax12_event[autom_id].time_to_throw_event = 0;
    
    for (ax12_id = 0 ; ax12_id < ID_MAX_AX12 ; ax12_id++)
    {
        ax12_event[autom_id].is_observed[ax12_id] = false;
    }
}

/**
 * register_ax12_event()
 * 
 * utilisé pour envoyer un évènement lorsque l'ax12 concerné a rejoint sa target
 * la target de l'ax12 est vérifié toutes les 200 ms
 * 
 * @param ax12_ID : id de l'ax12 qu'on veut observer
 * @param autom_id : id de l'event (contexte d'appel)
 * @param event : évènement à envoyer quand la target est reached
 * @param timer_ms : temps avant d'envoyer l'évènement quand la target est reached
 */
void register_ax12_event(uint8_t ax12_ID, _autom_id autom_id, _enum_flag_action event, uint32_t timer_ms)
{
    register_multiple_ax12_event(1, autom_id, event, timer_ms, ax12_ID);
}

/**
 * register_multiple_ax12_event()
 * 
 * utilisé pour envoyer un évènement lorsque les concernés ont rejoint leur target
 * la target de l'ax12 est vérifié toutes les 200 ms
 * 
 * @param nb_ax12 : n d'ax12 à observer (nb de paramètres variadiques)
 * @param autom_id : id de l'event (contexte d'appel)
 * @param event :  évènement à envoyer quand la target est reached
 * @param timer_ms : temps avant d'envoyer l'évènement quand la target est reached
 * @param ... : id des ax12 à observer
 */
void register_multiple_ax12_event(uint8_t nb_ax12, _autom_id autom_id, _enum_flag_action event, uint32_t timer_ms, ...)
{
    uint8_t i; 
    va_list liste_ax12;
    va_start(liste_ax12, timer_ms);
    
    // Register event
    for (i = 0 ; i < nb_ax12 ; i++)
    {
       ax12_event[autom_id].is_observed[va_arg(liste_ax12, uint8_t)] = true;
    }
    
    ax12_event[autom_id].time_to_throw_event = timer_ms;
    ax12_event[autom_id].autom_id = autom_id; // possible to send the event to another timer but not safe for the state machine
    ax12_event[autom_id].event = event;
    
    va_end(liste_ax12);
    
    // Start listening event
    arm_timer(autom_id, AX12_OBSERVER_DEFAULT_TIMER_MS, CHECK_AX12_EVENT, true);
}

/**
 * check_ax12_event()
 * check if an ax12 event occured and need to be notified
 * @param autom_id : contexte d'appel
 */
void check_ax12_event(_autom_id autom_id)
{
    bool event_reached = true;
    uint8_t id_ax12 = 0;
    
    for (id_ax12 = 0 ; id_ax12 < ID_MAX_AX12 ; id_ax12++)
    {
        // Check is condition is reached for all obvserved ax12
        if (ax12_event[autom_id].is_observed[id_ax12] == true)
        {
            // if the target is reached for an ax12, we remove it from obsvers
            if (is_target_ax12_reachead(id_ax12) == true)
            {
                ax12_event[autom_id].is_observed[id_ax12] = false;
            }
            // otherwise the condition is not reached yet
            else
            {
                event_reached = false;
            }
        }
    }
    
    // if all observed ax12 have reached target
    if (event_reached == true)
    {
        // event is send after the required time
        arm_timer(ax12_event[autom_id].autom_id, ax12_event[autom_id].event, ax12_event[autom_id].time_to_throw_event, true);
    }
    else
    {
        // We're still waiting for the ax12 to reach the target
        arm_timer(autom_id, AX12_OBSERVER_DEFAULT_TIMER_MS, CHECK_AX12_EVENT, true);
    }
}

/**
 * register_sync_event()
 * @param autom_id           : autom watching event
 * @param event_to_send      : event sent when sync complete
 * @param timer_ms           : timer before sending event
 * @param nb_event_registred : nb of event watched
 * @param ...                : (autom_id, event) for each event registred
 */
void register_sync_event (_autom_id autom_id, _enum_flag_action event_to_send, uint32_t timer_ms, uint8_t nb_event_registred, ...)
{
    uint8_t i; 
    
    if (nb_event_registred <= MAX_SYNC_EVENT)
    {
        va_list liste_event;
        va_start(liste_event, nb_event_registred);

        sync_event[autom_id].autom_listening_event = autom_id;
        sync_event[autom_id].event_to_send = event_to_send;
        sync_event[autom_id].timer_ms = timer_ms;
        sync_event[autom_id].nb_event_registred = nb_event_registred;

        // Register events
        for (i = 0 ; i < nb_event_registred ; i++)
        {
           sync_event[autom_id].event_regisred[i].autom_id = va_arg(liste_ax12, _autom_id);
           sync_event[autom_id].event_regisred[i].event    = va_arg(liste_ax12, _enum_flag_action);
        }

        va_end(liste_event);

        // Start listening event
        arm_timer(autom_id, SYNC_EVENT_OBSERVER_DEFAULT_TIMER_MS, CHECK_SYNC_EVENT, true);
    }    
}

/**
 * check_sync_event()
 * wait for event to synchronise between automs
 * @param autom_id : id of the event listener
 */
void check_sync_event(_autom_id autom_id)
{
    bool event_reached = true;
    uint8_t id_event = 0;
    
    for (id_event = 0 ; id_event < sync_event[autom_id].nb_event_registred ; id_event++)
    {
        // Check is condition is reached for all obvserved event
        if (FLAG_ACTION[sync_event[autom_id].event_regisred[id_event].autom_id] != 
                sync_event[autom_id].event_regisred[id_event].event)
        {    
            event_reached = false;
        }
    }
    
    // if all observed event have been reached
    if (event_reached == true)
    {
        // event is send after the required time
        arm_timer(sync_event[autom_id].autom_listening_event ,sync_event[autom_id].event_to_send, sync_event[autom_id].timer_ms, true);
    }
    else
    {
        // We're still waiting for the synchronisation
        arm_timer(autom_id, SYNC_EVENT_OBSERVER_DEFAULT_TIMER_MS, CHECK_SYNC_EVENT, true);
    }
}

/******************************************************************************/
/************************** INTERRUPTION DES QEI ******************************/
/******************************************************************************/

/**
 * Interruption sur codeur droit
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0;            //Clear du flag de l'event

    if (QEI1CONbits.UPDN == 1)      //codeur croit
        OVERFLOW_CODEUR [CODEUR_D] ++;
    else                            // codeur decroit
        OVERFLOW_CODEUR [CODEUR_D] --;
}

/**
 * Interruption sur codeur gauche
 */
void __attribute__((__interrupt__, no_auto_psv)) _QEI2Interrupt(void)
{
    IFS4bits.QEI2IF = 0;            //Clear du flag de l'event
    if (QEI2CONbits.UPDN == 1)      //codeur croit
        OVERFLOW_CODEUR [CODEUR_G] ++;
    else                            // codeur decroit
        OVERFLOW_CODEUR [CODEUR_G] --;
}

/******************************************************************************/
/***************************** INTERRUPTIONS UART *****************************/
/******************************************************************************/

void __attribute__ ((interrupt, no_auto_psv)) 	_U1RXInterrupt (void)
{
	// Desactivation de l'interruption
	IEC0bits.U1RXIE	= 0;
	IFS0bits.U1RXIF = 0;

    uint8_t buf;
    buf= U1RXREG;
 
#ifndef NO_SERIALUS
    if (serialus.actif == true)
        action_reception_serialus(buf);
#endif
        
	// Activation de l'interruption
	IEC0bits.U1RXIE	= 1;
}



void __attribute__ ((interrupt, no_auto_psv)) 	_U2RXInterrupt (void)
{
    IEC1bits.U2RXIE = 0;
    IFS1bits.U2RXIF = 0;

    reception_uart_ax12();

    IEC1bits.U2RXIE = 1;
}

void __attribute__ ((interrupt, no_auto_psv)) 	_U2TXInterrupt (void)
{
    IFS1bits.U2TXIF = 0; //remise du flag à 0 quand le buffer de transmission est vide
    ax12.etat_uart = ENVOIT_FINI;
}




/******************************************************************************/
/******************************************************************************/
/******************************************************************************/