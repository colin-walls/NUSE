/*** Context swap routines ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with
*   task context swapping in Nucleus SE:
*
*   NUSE_Context_Swap   entry point reached via trap
*   NUSE_Context_Load   called by scheduler start code
*
*   This code is not required if the Run To Completion scheduler is selected
*   This code is specifically designed for Freescale ColdFire targets and
*   needs to be re-written for other devices
*
************************************************************************/

#include "nuse.h"

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER

#pragma asm

;************************************************************************/
;*
;*           Copyright 2002-2007 Mentor Graphics Corporation
;*                         All Rights Reserved.
;*
;* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
;* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
;* SUBJECT TO LICENSE TERMS.
;*
;************************************************************************/


    SECTION code,4,C
    ALIGN   4
    XDEF    _NUSE_Context_Load
    XDEF    _NUSE_Context_Swap
    XREF    _NUSE_Task_Active
    XREF    _NUSE_Task_Next
    XREF    _NUSE_Task_Context

_NUSE_Context_Swap:

;   temporarily save D0 & A0
    move.l  d0,-(sp)
    move.l  a0,-(sp)

;   set up A0 to point to start of context block
    lea     _NUSE_Task_Context,a0
    clr     d0
    move.b  _NUSE_Task_Active,d0
    lsl     #3,d0   ; * 8
    add.l   d0,a0
    lsl     #3,d0   ; * 64
    add.l   d0,a0   ; 8+64=72 bytes - 18 words

;   store registers
    move.l  (sp)+,(a0)+         ;A0
    move.l  (sp)+,(a0)+         ;D0
    movem.l d1-d7/a1-a6,(a0)    ;D1-D7 & A1-A6
    lea.l   52(a0),a0
    move.l  (sp)+,(a0)+         ;SR
    move.l  (sp)+,(a0)+         ;PC
    move.l  sp,(a0)             ;SP

_NUSE_Context_Load:

;   set up A0 to point to end of context block
    lea     _NUSE_Task_Context,a0
    clr     d0
    move.b  _NUSE_Task_Next,d0
    move.b  d0,_NUSE_Task_Active
    addq    #1,d0
    lsl     #3,d0   ; * 8
    add.l   d0,a0
    lsl     #3,d0   ; * 64
    add.l   d0,a0   ; 8+64=72 bytes - 18 words

;   extract registers
    move.l  -(a0),sp            ;SP
    move.l  -(a0),-(sp)         ;PC
    move.l  -(a0),-(sp)         ;SR
    lea.l   -52(a0),a0
    movem.l (a0),d1-d7/a1-a6    ;D1-D7 & A1-A6
    move.l  -(a0),d0            ;D0
    move.l  -(a0),a0            ;A0

    rte

    end

#pragma endasm

#endif