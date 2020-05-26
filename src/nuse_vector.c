/*** Context swap routines ***/

/************************************************************************
*
*   This file contains interrupt vector definitions
*
*   This code is specifically designed for Freescale ColdFire targets and
*   needs to be re-written for other devices
*
************************************************************************/

#include "nuse.h"

#if (NUSE_SCHEDULER_TYPE != NUSE_TIME_SLICE_SCHEDULER) | (NUSE_TIMER_NUMBER != 0) | \
     NUSE_TASK_SLEEP | NUSE_SYSTEM_TIME_SUPPORT || NUSE_INCLUDE_EVERYTHING

#define ASMCODE

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


;/*************************************************************************
;*
;*      This file sets up the vector for the timer ISR
;*
;*************************************************************************/


    XREF    _NUSE_Real_Time_Clock_ISR
    ORG     $64
    dc.l    _NUSE_Real_Time_Clock_ISR

#pragma endasm

#endif

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


;/*************************************************************************
;*
;*      This file sets up the vector for the context swap trap
;*
;*************************************************************************/

    XREF    _NUSE_Context_Swap
    ORG     $80
    dc.l    _NUSE_Context_Swap

#pragma endasm

#endif

#ifdef ASMCODE

#pragma asm

    end

#pragma endasm

#endif
