/*************************************************************************
*
*   This file contains the definition of data types used by Nucleus SE
*   These are intended to aid portability and ned to be reviewed/adjusted
*   for a change in CPU and/or compiler
*   Some CPU-specific information is also maintained here:
*       Critical section entry/exit macros
*       Context saving data
*       Native interrupt support macros
*       Managed interrupt support macros
*
*************************************************************************/

/* Check to see if the file has been included already.  */
#ifndef _NUSE_TYPES_H_
#define _NUSE_TYPES_H_


/* Data types used by Nucleus SE - these may need to be adjusted for different compilers/CPUs */

typedef unsigned char U8;       /* unsigned 8 bit integer */
typedef signed char S8;         /* signed 8 bit integer */
typedef unsigned short U16;     /* unsigned 16 bit integer */
typedef unsigned long U32;      /* unsigned 32 bit integer */
typedef void * ADDR;            /* pointer/address */

typedef S8 STATUS;              /* service call status return value */
typedef U8 OPTION;              /* service call option parameter value */

typedef U8 NUSE_TASK;           /* kernel object index data types */
typedef U8 NUSE_PARTITION_POOL;
typedef U8 NUSE_MAILBOX;
typedef U8 NUSE_QUEUE;
typedef U8 NUSE_PIPE;
typedef U8 NUSE_SEMAPHORE;
typedef U8 NUSE_EVENT_GROUP;
typedef U8 NUSE_TIMER;

#define TRUE (1)
#define FALSE (0)

#define NULL (0)

#define LONIB(x) ((x) & 0xf)
#define HINIB(x) (((x) >> 4) & 0xf)

#define ROM
#define RAM

#define INTERRUPT

typedef void (*PF0)(void);
typedef void (*PF1)(U8);
typedef void (*PF2)(U8, ADDR);

/*** CPU specfics ***/

/* Critical section delimiters */

#define NUSE_CS_Enter()                 \
    if (NUSE_Task_State == NUSE_TASK_CONTEXT)   \
        asm(" move.w #$2700,sr");

#define NUSE_CS_Exit()                  \
    if (NUSE_Task_State == NUSE_TASK_CONTEXT)   \
        asm(" move.w #$2000,sr");

#define NUSE_REGISTERS          18
#define NUSE_STATUS_REGISTER    ((ADDR)0x40002000)

#define NUSE_CONTEXT_SWAP() \
    asm(" trap #0");

/* Interrupt Service Routine Support */

/* Native interrupts */

#define NUSE_NISR_Enter() \
static U8 NUSE_Old_Task_State; \
NUSE_Old_Task_State = NUSE_Task_State; \
NUSE_Task_State = NUSE_NISR_CONTEXT;

#define NUSE_NISR_Exit() \
NUSE_Task_State = NUSE_Old_Task_State;

/* Managed interrupts */

#define NUSE_MANAGED_ISR(isrname, isrcode) \
void isrname () \
{ \
asm(" XREF _NUSE_Task_Active"); \
asm(" XREF _NUSE_Task_Next"); \
asm(" XREF _NUSE_Task_Context"); \
asm(" move.l d0,-(sp)"); \
asm(" move.l a0,-(sp)"); \
asm(" lea _NUSE_Task_Context,a0"); \
asm(" clr d0"); \
asm(" move.b _NUSE_Task_Active,d0"); \
asm(" lsl #3,d0"); \
asm(" add.l d0,a0"); \
asm(" lsl #3,d0"); \
asm(" add.l d0,a0"); \
asm(" move.l (sp)+,(a0)+"); \
asm(" move.l (sp)+,(a0)+"); \
asm(" movem.l d1-d7/a1-a6,(a0)"); \
asm(" lea.l 52(a0),a0"); \
asm(" move.l (sp)+,(a0)+"); \
asm(" move.l (sp)+,(a0)+"); \
asm(" move.l sp,(a0)"); \
NUSE_Task_Saved_State = NUSE_Task_State; \
NUSE_Task_State = NUSE_MISR_CONTEXT; \
isrcode(); \
NUSE_Task_State = NUSE_Task_Saved_State; \
asm(" lea _NUSE_Task_Context,a0"); \
asm(" clr d0"); \
asm(" move.b _NUSE_Task_Next,d0"); \
asm(" move.b d0,_NUSE_Task_Active"); \
asm(" addq #1,d0"); \
asm(" lsl #3,d0"); \
asm(" add.l d0,a0"); \
asm(" lsl #3,d0"); \
asm(" add.l d0,a0"); \
asm(" move.l -(a0),sp"); \
asm(" move.l -(a0),-(sp)"); \
asm(" move.l -(a0),-(sp)"); \
asm(" lea.l -52(a0),a0"); \
asm(" movem.l (a0),d1-d7/a1-a6"); \
asm(" move.l -(a0),d0"); \
asm(" move.l -(a0),a0"); \
asm(" rte"); \
}

#endif  /* ! _NUSE_TYPES_H_ */

