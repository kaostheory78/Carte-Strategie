/*****************************************************************************
* SUDRIABOTIK
******************************************************************************
* Nom Fichier 	:	interrupt.c
* Operating System:	Pas d'OS, Fichiers "maison" pour carte INTERCONNEXION
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES  
* Date Création 	:	2007
* Date Révision 	:	11/07/2008 par PEB
******************************************************************************
* Description	:	- Déclarations des prototypes des fonctions du interrupt.c
******************************************************************************/

#include	<pic18f2x1x.h>		// Problème des certains code ne sont pas dans l'entete
#include 	<pic18.h>

//#include 	"delay.h"

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Tous les defines pour main_cod

#include 	"v_glob_cod.h"		// Variables globales utilisées par main_cod

#include 	"interrupt_cod.h"		// Prototypes des fonctions utilisées par main_cod
#include 	"reglages_carte.h"		// proto de reglages_carte

#include 	"a_affich.h"
#include 	"a_fonctions.h"
#include	"asserv.h"
#include 	"strategies.h"




/**************************************************************************************/
//I2C implementation
/**************************************************************************************/
void SSP_Handler(void)
{
	unsigned char i;
	
	//---------------------------------------------------------------------
	// STATE 1: If this is a WRITE operation and last byte was an ADDRESS
	//---------------------------------------------------------------------
	if(!DA && !RW && BF && S)			
	{		
		i = SSPBUF; //Lecture du registre pour effacer le bit BF (Bufferfull) automatiquement
		
		i2cstate_slave = SL_WRITE_ADDRESS;
		
		if(SSPOV)
		{
			SSPOV = 0;
		}
	}
	
	//---------------------------------------------------------------------
	// STATE 2:  If this is a WRITE operation and the last byte was DATA
	//---------------------------------------------------------------------
	else if(DA && !RW && BF)		
	{
		if ( i2cstate_slave == SL_WRITE_ADDRESS)
		{
			i2cstate_slave = SL_WRITE_OFFSET;
			i2c_offset = SSPBUF;
			//traitement_i2c_nb_octet = 0;
			//traitement_i2c_cpt_octet = 0;
		}
		else if ( i2cstate_slave == SL_WRITE_OFFSET)
		{
			i2cstate_slave = SL_WRITE_NBDATA;
			i2c_nbdata = SSPBUF;
			//traitement_i2c_nb_octet = i2c_nbdata;
		}
		else if ( i2cstate_slave == SL_WRITE_NBDATA) // Ecriture 1ère donnée utile
		{
			Master2Slave_Ptr = &buffer_i2c_rx + i2c_offset;
			*Master2Slave_Ptr++ = SSPBUF;
			i2cstate_slave = SL_WRITE_DATA;
			//traitement_i2c_cpt_octet++;
		}
		else if ( i2cstate_slave == SL_WRITE_DATA)
		{
			*Master2Slave_Ptr++ = SSPBUF;
			//traitement_i2c_cpt_octet++;
		}
	}
	
	//---------------------------------------------------------------------
	// STATE 3:  If this is a READ operation and last byte was an ADDRESS
	//---------------------------------------------------------------------
	else if(!DA && RW && !BF && S)		
	{
		i2cstate_slave = SL_READ_ADDRESS;
		SSPBUF = 0;
	}
	
	//---------------------------------------------------------------------
	// STATE 4:  If this is a READ operation and the last byte was DATA
	//---------------------------------------------------------------------
	else if(DA && RW && !BF)		
	{
		if (i2cstate_slave == SL_READ_ADDRESS)
		{
			Slave2Master_Ptr = &buffer_i2c_tx + i2c_offset;
			SSPBUF = *Slave2Master_Ptr++;
			i2cstate_slave = SL_READ_DATA;
		}
		else if ( i2cstate_slave == SL_READ_DATA)
		{				
			SSPBUF = *Slave2Master_Ptr++;
			i2cstate_slave = SL_READ_DATA;
		}
	}
	
	//---------------------------------------------------------------------
	// STATE 5:  A NACK from the master device is used to indicate that a
	// complete transmission has occurred.  The clearing of the
	// WDT is reenabled in the main loop at this time.
	//---------------------------------------------------------------------	
	else if(DA && !RW && !BF)
	{
		//stat.wdtdis = 0;
	}
	
	else
	{
		i = SSPBUF;
	}
	if (WCOL) WCOL = 0;
	if (SSPOV) SSPOV = 0;
}



/**************************************************************************************/
// INT0 implementation
/**************************************************************************************/
void INT0_Handler (void)
{
	INT0IE = 0;
	TMR0IE = 0;		//disable interruption overflow Timer0
	TMR0ON = 0;		//on arrete le timer0

	//on charge Timer0 pour la premiere phase
	TMR0 = PERIODE_HTR;	//PERIODE_5MS;
	phase_htr = 1;
	TMR0 = PERIODE_HTR;	//PERIODE_5MS;



	// **************************************** //
	//		DU CODE PEUT ÊTRE AJOUTE ICI	  	//
	// ****************************************	/
	while(!TRMT);TXREG = 'I';
		

	TMR0IF = 0;
	TMR0IE = 1;		//enable interruption overflow Timer0
	INT0IF = 0;
	INT0IE = 1;
	TMR0ON = 1;		//on remet le timer en route
}



/**************************************************************************************/
//TIMER0 implementation
/**************************************************************************************/
void TIMER0_Handler (void)
{	
	//putch('I');
	
	//HTR
	if (phase_htr >= NB_PERIODE_HTR)
	{
		temporisation_raf_debug++;
		phase_htr = 1;
		HTR_OUT = 0;
	}
	else
	{
		phase_htr++;
		HTR_OUT = 1;
	}
	
	
	TMR0 = PERIODE_HTR;
	TMR0 = PERIODE_HTR;
	TMR0IF = 0;
	TMR0IE = 1;
	TMR0ON = 1;		//on redemarre le timer0
	
	
	//---------- TRAITEMENT TIMER0 ----------//	
	
	//TRAITEMENT I2C
	//--------------
	TraitementI2C();
	
	//ASSERVISSEMENT
	//--------------
	Asservissement();
	
	
	//TIMEOUT
	//-------
	//TIMEOUT AUTOM
	if (timeout.blocage_timeout_autom == 0)
	{
		timeout.autom++;
	}
		
	//TIMEOUT CALLAGE
	//Si le compteur n'est pas bloqué on l'incremmente
	if ((timeout.blocage_timeout_callage == 0))
	{
		timeout.callage++;
	}
	
	//TIMEOUT DEPLACEMENT
	if ((timeout.blocage_timeout_deplacement == 0))
	{
		timeout.deplacement++;
	}
	
	//TIMEOUT BALISE
	if (timeout.blocage_timeout_balise == 0)
	{
		timeout.balise++;
	}		
	
	//TIMEOUT MATCH
	if (timeout.match >= TIMOUT_MATCH)
	{
		//Coupure de l'asservissement et de l'automatisme
		Strategie.etat_match = MATCH_TERMINE;
		_puts("\n\rIL MANQUE UNE COMMANDE!!");
		_CoupureAutom();
	}
	else
	{
		//Si le compteur n'est pas bloqué on l'incremmente
		if (!timeout.blocage_timeout_match)
		{
			timeout.match += 1;
		}
	}
}



/**************************************************************************************/
// UART RX implementation
/**************************************************************************************/
void RX_232_Handler(void)
{
	Strategie.octet_recu_rx = RCREG;
}



///////////////////////////////////////////////////////////////////////////////////////
//*************************************************************************************
// Interruptions de priorite haute
//*************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////
void interrupt HI_ISR(void)
{
	//------------------------------------------------------
	// Interruption INT0
	//------------------------------------------------------
	if((INT0IE)&&(INT0IF))	//enable && flag && interrupt priority
	{
		// routine d'interruption du Timer0
		INT0IE = 0;			//disable interruption overflow Timer1
		
		INT0_Handler();		// Traitement Routine d'interruption INT0
		
		INT0IF = 0; 		//remise du flag d'interruption a 0
		INT0IE = 1;			//enable interruption overflow Timer1
	}

	//------------------------------------------------------
	// Interruption I2C POUR ESCLAVE.
	//------------------------------------------------------
	if((SSPIE)&&(SSPIF)&&(SSPIP)) //enable && flag && interrupt priority
	{
		// routine d'interruption I2C
		SSPIE = 0;				//disable interruption I2C
		
		SSP_Handler();			// Do I2C communication
		
		CKP = 1;				// Release SCLK for master.
		SSPIF = 0;				// Clear the interrupt flag.
		SSPIE = 1;				//enable interruption I2C
	}
	
	//------------------------------------------------------
	// Interruption de réception d'un caractere
	//------------------------------------------------------
	if((RCIE)&&(RCIF)&&(RCIP)) 	//enable && flag && interrupt priority basse
	{
		// routine d'interruption du Timer1
		RCIE = 0;		//disable interruption overflow Timer1
		
		RX_232_Handler();
		
		RCIF = 0; 		//remise du flag d'interruption a 0
		RCIE = 1;		//enable interruption overflow Timer1
	}

	//------------------------------------------------------
	// Interruption Timer0 -> HTR
	//------------------------------------------------------
	if((TMR0IE)&&(TMR0IF)&&(TMR0IP)) //enable && flag && interrupt priority
	{
		// routine d'interruption du Timer0
		TMR0IE = 0;		//disable interruption overflow Timer0
		
		TIMER0_Handler();		
		
		//TMR0IF = 0; 	//remise du flag d'interruption a 0
		//TMR0IE = 1;		//enable interruption overflow Timer0
	}
}



///////////////////////////////////////////////////////////////////////////////////////
//*************************************************************************************
// Interruptions de priorite basse
//*************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////
void interrupt low_priority LOW_ISR(void)
{	
	if((TXIE)&&(TXIF)&&(!TXIP)) 	//enable && flag && interrupt priority basse
	{
		// routine d'interruption du Timer1
		//TXIE = 0;		//disable interruption overflow Timer1
		
		//TXIF = 0; 		//remise du flag d'interruption a 0
		//TXIE = 1;			//enable interruption overflow Timer1
	}
}
