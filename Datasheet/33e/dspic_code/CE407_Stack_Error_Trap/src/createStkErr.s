/**********************************************************************
* © 2006 Microchip Technology Inc.
*
* FileName:        createStkErr.s
* Dependencies:    Header (.h/.inc) files if applicable, see below
* Processor:       dsPIC33Exxxx/PIC24Exxxx
* Compiler:        MPLAB® C30 v3.30 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* RK                03/22/06  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Srikar D			04/27/11  Updated for dsPIC33E / PIC24E
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/

.ifdef __dsPIC33E
.include "p33Exxxx.inc"
.endif
.ifdef __PIC24E
.include "p24Exxxx.inc"
.endif

	.global 	_createStkUflow
    .global     _createStkOflow

	.section .text
/******************************************************************************
  Stack Overflow
*******************************************************************************/
_createStkOflow:
; Save Stack and SPLIM register
        mov w15,w1
        mov SPLIM,w2

; Create Stack Overflow trap condition
        mov w15,w0
        add w0,#20,w0
        mov w0,SPLIM
            
        mov #12,w0

oflow_loop:
        push    w0
       	dec     w0, w0
		bra     nz, oflow_loop 

; Restore Stack pointers and SPLIM values
        mov     w1,w15
        mov     w2,SPLIM

        return
     
/******************************************************************************
  Stack Underflow
*******************************************************************************/
_createStkUflow:
; Save Stack and SPLIM register
        mov w15,w1
        mov SPLIM,w2

; Create Stack Underflow trap condition
        mov #0xFFF,w0               ; Lowest Stack Address
        add w0,#20,w0
        mov w0,w15
            
        mov #11,w0

uflow_loop:
        pop     w3
       	dec     w0, w0
		bra     nz, uflow_loop 

; Restore Stack pointers and SPLIM values
        mov     w1,w15
        mov     w2,SPLIM

        return
