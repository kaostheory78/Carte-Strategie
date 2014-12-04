/**********************************************************************
* © 2006 Microchip Technology Inc.
*
* FileName:        createMathErr.s
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
* Settu D.          03/22/06  First release of source file
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

    .global     _createAccShtError
    .global     _createDiv0Error
	.global 	_createAccaOflow
    .global     _createAccbOflow
    .global     _createAccaCoflow
    .global     _createAccbCoflow


	.section .text
/******************************************************************************
;  ACC Shift Error
*******************************************************************************/

_createAccShtError:
        mov     #0x7FFF,w0  
        lac     w0,#0,a
        mov     #17,w1              ; Invalid Shift Count
        sftac   A,w1

        nop
        nop
        nop
        nop
        nop

        return
        


/******************************************************************************
;  DIVIDE BY 0 ERROR
*******************************************************************************/
_createDiv0Error:
        clr     SR
        clr     w2                  ; Divisor=0
        mov     #1,w0               ; Dividend=1

        repeat  #17
        div.s   w0,w2               ; Perform 1/0
 
  
        nop
        nop
        nop
        nop
        nop

        return


	.section .text
/******************************************************************************
;  ACCA Overflow
*******************************************************************************/
_createAccaOflow:
        clr     SR
        bclr    CORCON,#ACCSAT      ; 1:31 Saturation Mode

        bclr    CORCON,#SATA        ; Comment this line for Saturation
;       bset    CORCON,#SATA        ; Comment this line for Overflow
        mov     #0x7FFF,w0          ; Max POS value
        mov     #0x1,w1             ; + 1
        lac     w0,#0,a
        lac     w1,#0,b
        add     a                   ; a=acca+accb   
  
        nop
        nop
        nop
        nop
        nop

        return
     
/******************************************************************************
;  ACCB Overflow
*******************************************************************************/
_createAccbOflow:
        clr     SR
        bclr    CORCON,#ACCSAT      ; 1.31 Saturation Mode

        bclr    CORCON,#SATB        ; Comment this line for Saturation
;       bset    CORCON,#SATB        ; Comment this line for Overflow
        mov     #0x8000,w0          ; Max NEG Value
        mov     #0xFFFF,w1          ; -1
        lac     w0,#0,a
        lac     w1,#0,b
        add     b                   ; b=acca+accb    

        nop
        nop
        nop
        nop
        nop
 
        return



/******************************************************************************
;  ACCA Catastrophic Overflow
*******************************************************************************/
_createAccaCoflow:
        clr     SR
        bset    CORCON,#ACCSAT      ; 9.31 Saturation Mode
        bclr    CORCON,#SATA        


        mov     #0x7FFF,w0  
        mov     #0x1,w1  
        lac     w0,#0,a
        lac     w1,#0,b

        sftac   a,#-8       ; Close to Max POS value
        sftac   b,#-8       ; Value to create overflow

        add     a           ; a=acca+accb   

        nop
        nop
        nop
        nop
        nop

        return
     
/******************************************************************************
;  ACCB Catastrophic Overflow
*******************************************************************************/
_createAccbCoflow:
        clr     SR
        bset    CORCON,#ACCSAT  ; 9.31 Saturation Mode
        bclr    CORCON,#SATB        


        mov     #0x8000,w0          
        mov     #0xFFFF,w1  
        lac     w0,#0,a
        lac     w1,#0,b

        sftac   a,#-8           ; Max NEG value
        sftac   b,#-8           ; Value to create Overflow

        add     b           ; b=acca+accb   

        nop
        nop
        nop
        nop
        nop

        return

     

