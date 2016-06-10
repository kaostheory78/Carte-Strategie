/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	interrupt.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES  
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Déclarations des prototypes des fonctions du interrupt.c
******************************************************************************/




/** SSP_Handler : I2C implementation
 * @param : aucun
 */
void SSP_Handler(void);

/** INT0_Handler : INT0 implementation
 * @param : aucun
 */
void INT0_Handler (void);

/** TIMER0_Handler : TIMER0 implementation
 * @param : aucun
 */
void TIMER0_Handler (void);


/** TIMER1_Handler : TIMER1 implementation
 * @param : aucun
 */
void TIMER1_Handler (void);

/** RX_232_Handler : UART RX implementation
 * @param : aucun
 */
void RX_232_Handler(void);

/**  HI_ISR : Interruptions de priorite haute
 * @param : aucun
 */
void interrupt HI_ISR(void);

/**  LOW_ISR : Interruptions de priorite basse
 * @param : aucun
 */
void interrupt low_priority LOW_ISR(void);
