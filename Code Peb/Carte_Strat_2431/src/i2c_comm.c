/*****************************************************************************
* SUDRIABOTIK
******************************************************************************
* Nom Fichier 	:	i2c_comm.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES  
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Fonctions de communication I2C
******************************************************************************/

#include 	<pic18.h>			// définitions des registres PIC

#include 	"compil_cod.h"		// Directives de compilation
#include 	"define_cod.h"		// Tous les define pour la carte interface

#include 	"v_glob_cod.h"	// Variables globales externes

#include	"i2c_comm.h"		//proto de ce fichier .c




//*****************************************************
// VARIABLES ( DEFINED HERE )
//*****************************************************

/**
	unsigned char read_count;
	unsigned char write_count;
	unsigned char address_hold, address_offset;
	unsigned char read_retry;

	unsigned char I2CWriteState;
	unsigned char i2cstate;
	unsigned char *Write2Slave_Ptr;
	unsigned char *Write_Ptr;
	//unsigned char index;

	//unsigned char ReadStatBufFromSlave[OperChannelsPerSlave+8]; 
	//unsigned char WriteData2Slave[MaxChannelsPerSlave];
	unsigned char *ReadFSlave_Ptr;

	const unsigned char ReadFSlaveI2CStates[] = {1,10,11,12,2,9,7,4,7,5,3,0}; 	//Pad with null state
	const unsigned char Write2SlaveI2CStates[] = {1,10,11,12,8,5,3,0}; 			//Pad with null state
	const unsigned char *I2CState_Ptr;         									// define pointer for accessing I2C states

	// define I2C bus states
	enum i2c_bus_states {
		STARTI2C =1, 
		RESTART =2, 
		STOPI2C =3, 
		SEND_ACK =4, 
		SEND_NACK =5, 
		GEN_CALL =6, 
		READ =7, 
		WRITE_DATA =8, 
		WRITE_ADDRESS1 =9, 
		WRITE_ADDRESS0 =10, 
		WRITE_OFFSET =11, 
		WRITE_NBDATA =12
	};



void Service_I2CSlave (unsigned char i2c_addr, unsigned char i2c_rw, unsigned char i2c_offset, unsigned char i2c_nbdata, unsigned char i2c_mode)
{
	//-------------------------------------------------------------------------------------------------------
	//   Exécution une seule fois par trame de lecture, assurée par le flag read_start 
	//-------------------------------------------------------------------------------------------------------
	if ( !sflag.event.read_start && sflag.event.read_i2c)   		// execute once per entire rounds of slave reads
	{
	  	sflag.event.read_start = 1;        				// set reads start flag
		I2CState_Ptr = &ReadFSlaveI2CStates[0];    		// initialize I2C state pointer
		ReadFSlave_Ptr = (char *) &buffer_i2c_rx;		// buffer de 64 octets max
		read_count = i2c_nbdata;
		//while(!U1STAbits.TRMT);U1TXREG = '3';
	}
	else if ( !sflag.event.write_start && sflag.event.write_i2c )	// execute once per entire rounds of slave reads
	{
	  	sflag.event.write_start = 1;        			// set reads start flag
		I2CState_Ptr = &Write2SlaveI2CStates[0];    	// initialize I2C state pointer
		Write2Slave_Ptr = (char *) &buffer_i2c_tx;      // buffer de 64 octets max
		write_count = i2c_nbdata;
		//while(!U1STAbits.TRMT);U1TXREG = '4';
	}	

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//  Will execute these statements if last I2C bus state was a WRITE (ADDRESS0 ou OFFSET ou NBDATA ou ADDRESS1 ou WRITE DATA)
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if ( sflag.event.write_state )       	// test if previous I2C state was a write
	{
		//while(!U1STAbits.TRMT);U1TXREG = '5';
		
		if ( ACKSTAT )                     // was NOT ACK received?                   
	  	{
			while (!TRMT);TXREG = 'P';
			while (!TRMT);TXREG = 'O'+ phase_htr;
	    	//_PEN = 1;							// generate bus stop condition
	    	eflag.i2c.ack_error = 1;			// set acknowledge error flag
	  	}
	  	
	  	sflag.event.write_state = 0;		// reset write state flag
	}

	//----------------------------------------------------------------------------------------------------
	//  Will execute these statements if a bus collision or an acknowledge error
	//----------------------------------------------------------------------------------------------------
	if ( eflag.i2c.bus_coll_error || eflag.i2c.ack_error  )           
	{
//		//sflag.event.read_loop = 0;         				// reset read loop flag for any error
//		sflag.event.next_transfer = 1;		// set flag indicating next slave
//		
//		//temp.error = error_mask << slave_count; 			// compose error status word
//		
//		if ( eflag.i2c.bus_coll_error )    	// test for bus collision error
//		{
//			eflag.i2c.bus_coll_error = 0;   // reset bus collision error flag
//			bus.error_word |= temp.error;   // compose bus error status word
//			SSPIF = 1;                      // set false interrupt to restart comm
//		}
//		if ( eflag.i2c.ack_error )         	// test for acknowledge error
//		{
//			eflag.i2c.ack_error = 0;        // reset acknowledge error flag
//			comm.error_word |= temp.error;  // compose communication error status word
//		}
//	}
//	else                                	// else no error for this slave
//	{
//		// temp.error = error_mask << slave_count; 			// compose error status word
//		bus.error_word &= ~temp.error;     	// reset bus error bit for this slave
//		comm.error_word &= ~temp.error;    	// reset comm error bit for this slave
//		
	}

	//----------------------------------------------------------------------------
	//  Will execute these statements for each state of transfer
	//----------------------------------------------------------------------------
	if ( !sflag.event.next_transfer )		// transfert en cours
	{
		//while(!U1STAbits.TRMT);U1TXREG = '6';
		sflag.event.int_i2c = 0;			// reset I2C state event flag
		I2CBusState(i2c_addr, i2c_rw, i2c_offset, i2c_nbdata, i2c_mode);	// execute next I2C state 
	}
	else	// Le transfert est terminé
	{
		//while(!U1STAbits.TRMT);U1TXREG = '7';
	 	if (sflag.event.read_i2c) sflag.event.reads_done = 1;     	 // set flag indicating all slaves are read
		if (sflag.event.write_i2c) sflag.event.writes_done = 1;      // set flag indicating all slaves are read	 			
		sflag.event.next_transfer = 0;
	}
}

void I2CBusState (unsigned char i2c_addr, unsigned char i2c_rw, unsigned char i2c_offset, unsigned char i2c_nbdata, unsigned char i2c_mode)
{
	i2cstate = *I2CState_Ptr++;				// retrieve next I2C state

	switch ( i2cstate )						// evaluate which I2C state to execute
	{
		case ( STARTI2C ):			// test for I2C start state
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'S';
			#endif 
			
			SEN = 1;   					// initiate I2C bus start state	
			break;	
			
		case ( WRITE_ADDRESS0 ):	// test for I2C address (R/W=0)
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'a';
			#endif
			//while (_TBF);
			I2CTRN = i2c_addr;        	 	// initiate I2C address write state
			sflag.event.write_state = 1;   	// set flag indicating write event in action
			break;
			
		case ( WRITE_OFFSET ):		// test for I2C write (DATA)
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'O';
			#endif
			//while (_TBF); 						// Attente module I2C en mode attente et transmission précédente terminée
			I2CTRN = i2c_offset;   			// initiate I2C write state		  
			sflag.event.write_state = 1;   	// set flag indicating write event in action
			if (i2c_mode == 2)
			{
				I2CState_Ptr++; 			// En mode boussole ou EEPROM, seul l'offset est envoyé
			}
			break;
			
		case ( WRITE_NBDATA ): 		// test for I2C write (DATA)
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'N';
			#endif
			//while (_TBF);
			I2CTRN = i2c_nbdata;  			// initiate I2C write state		  
			sflag.event.write_state = 1;  	// set flag indicating write event in action
			break;
			
		case ( RESTART ): 			// test for restart state
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'S';
			#endif
			RSEN = 1;						// initiate I2C bus restart state
			break;     
			
		case ( WRITE_ADDRESS1 ):	// test for I2C address (R/W=1)
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'A';
			#endif
			//while (_TBF);
			I2CTRN = i2c_addr + 1;     		// initiate I2C address write state 
			sflag.event.write_state = 1;   	// set flag indicating write event in action
			break;
			
		case ( READ ):				// test for I2C read
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'R';
			#endif
			RCEN = 1;                      // initiate i2C read state
			break;
			
		case ( WRITE_DATA ):		// test for I2C write (DATA)
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'W';
			#endif
			//while (_TBF);
			I2CTRN = *Write2Slave_Ptr++;// initiate I2C write state		  
			if ( write_count -1 > 0)	// test if still in read loop
			{		  	
				write_count -= 1;		// reduce read count
				I2CState_Ptr -= 1;		// update state pointer
			}
			sflag.event.write_state = 1;   // set flag indicating write event in action
			break;
			
		case ( SEND_ACK ):			// test for send acknowledge state
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'K';
			#endif
			*ReadFSlave_Ptr++ = I2CRCV;    	// save off byte
			if ( read_count - 1 > 0)		// test if still in read loop
			{
				read_count -= 1;    		// reduce read count
				I2CState_Ptr -= 2;      	// update state pointer
			}
			ACKDT = 0;                     // set acknowledge data state (true)
			ACKEN = 1;                     // initiate acknowledge state
			break;
			
		case ( SEND_NACK ):			// test if sending NOT acknowledge state
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'N';
			#endif
			*ReadFSlave_Ptr = I2CRCV;  		// dernière lecture pour le NACK (non utilisée)
			ACKDT = 1;                     // set acknowledge data state (false)
			ACKEN = 1;                     // initiate acknowledge sequence
			break;
			
		case ( STOPI2C ):			// test for stop state
			PEN = 1;                       // initiate I2C bus stop state 
			sflag.event.next_transfer = 1; // set flag indicating next transfer
			#if DEBUG_I2C
				while (!TRMT);TXREG = 'T';
			#endif
			break;
			
		default:
			break;
	}
}

void as_i2c_transfer_and_rs232debug (unsigned char i2c_addr, unsigned char i2c_rw, unsigned char i2c_offset, unsigned char i2c_nbdata, unsigned char i2c_mode)
{	
	//_MI2CIE = 0;								//disable interruption Bus collision
	//_MI2CIF = 0; 								//remise du flag d'interruption a 0
	//_MI2CIE = 1;								//enable interruption I2C Master	
	if (i2c_rw == 1) 							// Trame de lecture demandée
	{
		sflag.event.read_i2c = 1; 				// Demande lecture
		sflag.event.int_i2c = 1;				// Validation flag interruption
		sflag.event.reads_done = 0;
		//while(!U1STAbits.TRMT);U1TXREG = '1';
		
		while (!sflag.event.reads_done)			// Boucle tant que la trame de lecture n'est pas terminée
		{
			//while(!U1STAbits.TRMT);U1TXREG = '2';
			// Attente flag event interruption environ 100us pour permettre à des pics esclaves lents de suivre la cadence imposée par le maître !!!
			// au lieu d'attendre, on va en profiter pour envoyer les caractères sur la ligne série à 115200bauds => en 100us ca fait 11,2 bits soit environ un caractère.
			//DelayMs( 1 );                	// short delay between events to allow for slow FOSC on Slave	
			//ptr_UART = as_dbg_affich_ecran();
			//os_putsUART1_partiel(ptr_UART, 0, 4);
			
	  		if ( sflag.event.read_i2c && sflag.event.int_i2c ) 							// test if read I2C is active et présence flag event interruption
	  		{
	  			Service_I2CSlave(i2c_addr, i2c_rw, i2c_offset, i2c_nbdata, i2c_mode);	// Service I2C slave device(s)			
				//while(!U1STAbits.TRMT);U1TXREG = 'r';
			}
			if ( sflag.event.reads_done )    	// test if that was last read
			{
	  			sflag.status &= 0x00F0;        	// reset all I2C event flags
	  			eflag.status = 0x00;           	// reset all error event flags		
				break; 							// pour sortir du while après une série de lectures réussies
			}
		}
	}
	else if (i2c_rw == 0)					// Trame d'écriture demandée
	{
		sflag.event.write_i2c = 1; 			// Demande écriture
		sflag.event.int_i2c = 1;			// Validation flad demande lecture
		sflag.event.writes_done = 0;
		while (!sflag.event.writes_done)	// Boucle tant que la trame d'écritures n'est pas terminée
		{
			// Attente flag event interruption
			//DelayUs( 100 );                				// short delay between events to allow for slow FOSC on Slave	
			
	  		if ( sflag.event.write_i2c && sflag.event.int_i2c ) 	// test if read I2C is active et présence flag event interruption
	  		{
	  			Service_I2CSlave(i2c_addr, i2c_rw, i2c_offset, i2c_nbdata, i2c_mode);  
				//while(!U1STAbits.TRMT);U1TXREG = 'w';	  			      		
			}
			if ( sflag.event.writes_done )    	// test if that was last read
			{
	  			sflag.status &= 0x00F0;        	// reset all I2C event flags
	  			eflag.status = 0x00;           	// reset all error event flags		
				break; 							// pour sortir du while après une série de lectures réussies
			}
		}
	} 	
}
**/
