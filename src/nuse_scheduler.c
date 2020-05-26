/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   scheduling of tasks in Nucleus SE:
*
*   NUSE_Scheduler() [RTC version]
*   NUSE_Scheduler() [non-RTC version]
*   NUSE_Reschedule() [TS and RR version]
*   NUSE_Reschedule() [Priority Scheduler version]
*   NUSE_Wake_Task()
*   NUSE_Suspend_Task()
*
************************************************************************/

#include "nuse_config.h"
#include "nuse_codes.h"
#include "nuse_types.h"
#include "nuse_data.h"
#include "nuse_prototypes.h"


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Scheduler()        [RTC version]
*
*   DESCRIPTION
*
*       This function commences task scheduling and is called by the start up code after initialization
*       The Run To Competion scheduler is an infinite loop which cycles around caling each taks in turn
*       If task suspend is enabled, calls are dependent upon task status
*       If schedule counting is configured, this count is maintained for each task
*       If task parameter support is configured, the argc and argv values in ROM are passed on each call
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

    void NUSE_Scheduler(void)
    {
        NUSE_TASK task_count;

        NUSE_Task_State = NUSE_TASK_CONTEXT;

        while (TRUE)
        {
            for (task_count=0; task_count<NUSE_TASK_NUMBER; task_count++)
            {
                #if NUSE_SUSPEND_ENABLE
                    if (NUSE_Task_Status[task_count] == NUSE_READY)
                #endif
                    {

                        #if NUSE_SCHEDULE_COUNT_SUPPORT
                            NUSE_CS_Enter();
                            NUSE_Task_Schedule_Count[task_count]++;
                            NUSE_CS_Exit();
                        #endif

                        NUSE_Task_Active = task_count;
                        ((PF0)NUSE_Task_Start_Address[task_count])();
                    }
            }
        }
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Scheduler()        [non-RTC version]
*
*   DESCRIPTION
*
*       This function commences task scheduling and is called by the start up code after initialization
*       The index for the first task to be run is selected
*       If suspend is enabled, this is the task set to NUSE_READY with the lowest index
*       Otherwise, it is 0
*       If schedule counting is configured, this count is updated for this task
*       Further scheduling is peformed by called to NUSE_Reschedule() [below]
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

#if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER || NUSE_SCHEDULER_TYPE == NUSE_ROUND_ROBIN_SCHEDULER || NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER

    void NUSE_Scheduler(void)
    {
        NUSE_TASK task;

        #if NUSE_INITIAL_TASK_STATE_SUPPORT
            for (task=0; task<NUSE_TASK_NUMBER; task++)
            {
                if (NUSE_Task_Status[task] == NUSE_READY)
                {
                    break;
                }
            }
        #else
            task = 0;
        #endif

        #if NUSE_SCHEDULE_COUNT_SUPPORT
            NUSE_Task_Schedule_Count[task]++;
        #endif
        NUSE_Task_Next = task;
        NUSE_Task_State = NUSE_TASK_CONTEXT;
        NUSE_Context_Load();
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Reschedule()       [Time Slice and Round Robin Scheduler version]
*                               [Priority Scheduler version below; there is no RTC version]
*
*   DESCRIPTION
*
*       Called when task scheduling is required - may be resulting from an API call,
*       or the clock ISR [Time Slice Scheduler]
*       The index for the next task to be run is selected
*       If suspend is enabled, this is the next task set to NUSE_READY
*       Otherwise, it is simply the next task
*       If schedule counting is configured, this count is updated for this task
*       If the function was not called from an ISR, the new task context is loaded before exit
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

#if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER || NUSE_SCHEDULER_TYPE == NUSE_ROUND_ROBIN_SCHEDULER

    void NUSE_Reschedule(void)
    {
        #if NUSE_SUSPEND_ENABLE
            NUSE_Task_Next = NUSE_Task_Active;
            do
            {
                NUSE_Task_Next++;
                if (NUSE_Task_Next == NUSE_TASK_NUMBER)
                    NUSE_Task_Next = 0;
            } while (NUSE_Task_Status[NUSE_Task_Next] != NUSE_READY);
        #else
            NUSE_Task_Next = NUSE_Task_Active + 1;
            if (NUSE_Task_Next == NUSE_TASK_NUMBER)
            {
                NUSE_Task_Next = 0;
            }
        #endif

        #if NUSE_SCHEDULE_COUNT_SUPPORT
            NUSE_Task_Schedule_Count[NUSE_Task_Next]++;
        #endif

        #if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER    /* reset time slice tick counter */
            NUSE_Time_Slice_Ticks = NUSE_TIME_SLICE_TICKS;      /* done here to accommodate reliquish */
        #endif                                                  /* as well as ISR count down */

        if (NUSE_Task_State != NUSE_MISR_CONTEXT)
        {
            NUSE_CONTEXT_SWAP();
        }
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Reschedule()       [Priority Scheduler version]
*                               [Time Slice and Round Robin Scheduler version above; there is no RTC version]
*
*   DESCRIPTION
*
*       Called when task scheduling is required - resulting from an API call
*       If a task index "hint" is provided and that task is a higher priority [lower index]
*       than the current task, it is scheduled; otherwise the function simply returns
*       If no task index is supplied, the highest priority task which has the status NUSE_READY
*       is scheduled
*       If schedule counting is configured, this count is updated for this task
*       If the function was not called from an ISR, the new task context is loaded before exit
*
*   INPUTS
*
*       U8 new_task     index of task that might be run next
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

#if NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER

    void NUSE_Reschedule(U8 new_task)
    {
        if (new_task == NUSE_NO_TASK)
        {
            for (new_task=0; new_task<NUSE_TASK_NUMBER; new_task++)
            {
                if (NUSE_Task_Status[new_task] == NUSE_READY)
                {
                    break;
                }
            }
        }
        else
        {
            if (new_task > NUSE_Task_Active)
            {
                return;
            }
        }

        #if NUSE_SCHEDULE_COUNT_SUPPORT
            NUSE_Task_Schedule_Count[new_task]++;
        #endif

        NUSE_Task_Next = new_task;
        if (NUSE_Task_State != NUSE_MISR_CONTEXT)
        {
            NUSE_CONTEXT_SWAP();
        }
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Wake_Task()
*
*   DESCRIPTION
*
*       Specified task is placed in the NUSE_READY state
*       If the Priority Scheduler is in use, a call to NUSE_Reschedule() with a hint
*       ensure that the woken task is given control if it has the highest priority
*
*   INPUTS
*
*       NUSE_TASK task        index of task to be woken
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

#if NUSE_SUSPEND_ENABLE

    void NUSE_Wake_Task(NUSE_TASK task)
    {
        NUSE_Task_Status[task] = NUSE_READY;
        #if NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER
            NUSE_Reschedule(task);
        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Suspend_Task()
*
*   DESCRIPTION
*
*       Specified task is suspended and NUSE_Reschedule() called to pass control
*       to the next available task
*
*   INPUTS
*
*       NUSE_TASK task      index of task to be suspended
*       U8 suspend_code     status code; normally NUSE_PURE_SUSPEND, NUSE_SLEEP_SUSPEND or object suspend code
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

#if NUSE_SUSPEND_ENABLE

    void NUSE_Suspend_Task(NUSE_TASK task, U8 suspend_code)
    {
        NUSE_Task_Status[task] = suspend_code;
        #if NUSE_BLOCKING_ENABLE
            NUSE_Task_Blocking_Return[task] = NUSE_SUCCESS;
        #endif
        if (task == NUSE_Task_Active)
        {
            #if NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER
                NUSE_Reschedule(NUSE_NO_TASK);
            #else
                NUSE_Reschedule();
            #endif
        }
    }

#endif
