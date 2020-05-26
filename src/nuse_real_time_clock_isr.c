/*** Tick timer interrupt service routine ***/

#include "nuse_config.h"
#include "nuse_codes.h"
#include "nuse_types.h"
#include "nuse_data.h"
#include "nuse_prototypes.h"


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Real_Time_Clock_ISR()
*
*   DESCRIPTION
*
*       Tick timer interrupt service routine [ISR]
*       4 actions are performed, depending upon configuration:
*       1) Process any configured timers; decrement counters and act upon expiration
*       2) Increment the system clock, if configured
*       3) Decrement any task timeout [sleep] counters and wake task as necessary
*       4) If Time Slice scheduler, decrement counter and effect task swap on 0
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       (none)
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_SCHEDULER_TYPE == NUSE_RUN_TO_COMPLETION_SCHEDULER
    INTERRUPT void NUSE_Real_Time_Clock_ISR(void)
#else
    static void Real_Time_Clock_ISR(void)
#endif
{
    #if NUSE_TIMER_NUMBER != 0

    {
        U8 timer;

        for (timer=0; timer<NUSE_TIMER_NUMBER; timer++)
        {
            if (NUSE_Timer_Status[timer])
            {
                if (--NUSE_Timer_Value[timer] == 0)
                {
                    NUSE_Timer_Expirations_Counter[timer]++;

                    #if NUSE_TIMER_EXPIRATION_ROUTINE_SUPPORT || NUSE_INCLUDE_EVERYTHING
                        if (NUSE_Timer_Expiration_Routine_Address[timer] != NULL)
                        {
                            ((PF1)NUSE_Timer_Expiration_Routine_Address[timer])(NUSE_Timer_Expiration_Routine_Parameter[timer]);
                        }
                    #endif

                    if (NUSE_Timer_Reschedule_Time[timer] != 0)                         /* reschedule? */
                    {
                        NUSE_Timer_Value[timer] = NUSE_Timer_Reschedule_Time[timer];    /* yes: set up time */
                    }
                    else
                    {
                        NUSE_Timer_Status[timer] = FALSE;                               /* no: disable */
                    }
                }
            }
        }
    }

    #endif

    #if NUSE_SYSTEM_TIME_SUPPORT || NUSE_INCLUDE_EVERYTHING

        NUSE_Tick_Clock++;

    #endif

    #if NUSE_TASK_SLEEP || NUSE_INCLUDE_EVERYTHING

    {
        U8 task;

        for (task=0; task<NUSE_TASK_NUMBER; task++)
        {
            if (NUSE_Task_Timeout_Counter[task] != 0)
            {
                NUSE_Task_Timeout_Counter[task]--;
                if (NUSE_Task_Timeout_Counter[task] == 0)
                {
                    NUSE_Wake_Task(task);
                }
            }
        }
    }

    #endif

    #if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER

        if (--NUSE_Time_Slice_Ticks == 0)
        {
            NUSE_Reschedule();
        }

    #endif
}

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
    NUSE_MANAGED_ISR(NUSE_Real_Time_Clock_ISR, Real_Time_Clock_ISR)
#endif
