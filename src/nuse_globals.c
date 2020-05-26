/*************************************************************************
*
*   This file contains the declaration of the Nucleus SE global variables
*   Most are held in RAM and initialized in nuse_init.c
*
*************************************************************************/

#include "nuse_types.h"
#include "nuse_config.h"

ROM char * const NUSE_Release_Info = "Nucleus SE - B200512";

RAM NUSE_TASK NUSE_Task_Active;

RAM U8 NUSE_Task_State;             /* initialized to "startup" */
RAM NUSE_TASK NUSE_Task_Saved_State;

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
    RAM NUSE_TASK NUSE_Task_Next;
#endif

#if NUSE_SYSTEM_TIME_SUPPORT || NUSE_INCLUDE_EVERYTHING
    RAM U32 NUSE_Tick_Clock;                /* system clock */
#endif

#if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER
    RAM U16 NUSE_Time_Slice_Ticks;
#endif