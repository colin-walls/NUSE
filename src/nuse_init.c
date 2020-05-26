/* NUSE initialization */

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   initialization of a Nucleus SE system:
*
*   NUSE_Init_Task()
*   NUSE_Init_Partition_Pool()
*   NUSE_Init_Mailbox()
*   NUSE_Init_Queue()
*   NUSE_Init_Pipe()
*   NUSE_Init_Semaphore()
*   NUSE_Init_Event_Group()
*   NUSE_Init_Timer()
*   NUSE_Init()
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
*       NUSE_Init_Task()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for a task - the data arrays are also declared here
*       This may include:
*           Task context [for non-RTC schedulers]
*           Signal flags [if configured]
*           Timeout/sleep counter [if configured]
*           Task status [if suspend is enabled], which may be set to NUSE_READY or a configured initial state
*           Schedule count [if configured]
*
*   INPUTS
*
*       NUSE_TASK task        index of task to be initialized
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

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
    RAM ADDR NUSE_Task_Context[NUSE_TASK_NUMBER][NUSE_REGISTERS];
#endif

#if NUSE_SIGNAL_SUPPORT || NUSE_INCLUDE_EVERYTHING
    RAM U8 NUSE_Task_Signal_Flags[NUSE_TASK_NUMBER];
#endif

#if NUSE_TASK_SLEEP || NUSE_INCLUDE_EVERYTHING
    RAM U16 NUSE_Task_Timeout_Counter[NUSE_TASK_NUMBER];
#endif

#if NUSE_SUSPEND_ENABLE || NUSE_INCLUDE_EVERYTHING
    RAM U8 NUSE_Task_Status[NUSE_TASK_NUMBER];
#endif

#if NUSE_BLOCKING_ENABLE || NUSE_INCLUDE_EVERYTHING
    RAM U8 NUSE_Task_Blocking_Return[NUSE_TASK_NUMBER];
#endif

#if NUSE_SCHEDULE_COUNT_SUPPORT || NUSE_INCLUDE_EVERYTHING
    RAM U16 NUSE_Task_Schedule_Count[NUSE_TASK_NUMBER];
#endif

void NUSE_Init_Task(NUSE_TASK task)
{
    #if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
        NUSE_Task_Context[task][15] =                                   /* SR */
            NUSE_STATUS_REGISTER;
        NUSE_Task_Context[task][16] =                                   /* PC */
            NUSE_Task_Start_Address[task];
        NUSE_Task_Context[task][17] =                                   /* SP */
            (U32 *)NUSE_Task_Stack_Base[task] + NUSE_Task_Stack_Size[task];
    #endif

    #if NUSE_SIGNAL_SUPPORT || NUSE_INCLUDE_EVERYTHING
        NUSE_Task_Signal_Flags[task] = 0;
    #endif

    #if NUSE_TASK_SLEEP || NUSE_INCLUDE_EVERYTHING
        NUSE_Task_Timeout_Counter[task] = 0;
    #endif

    #if NUSE_SUSPEND_ENABLE || NUSE_INCLUDE_EVERYTHING
        #if NUSE_INITIAL_TASK_STATE_SUPPORT || NUSE_INCLUDE_EVERYTHING
            NUSE_Task_Status[task] = NUSE_Task_Initial_State[task];
        #else
            NUSE_Task_Status[task] = NUSE_READY;
        #endif
    #endif

    #if NUSE_SCHEDULE_COUNT_SUPPORT || NUSE_INCLUDE_EVERYTHING
        NUSE_Task_Schedule_Count[task] = 0;
    #endif
}


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Partition_Pool()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any partition pools  - the data arrays are also declared here
*       Sets the status of the pool to indicate that no partitionas are in use
*
*   INPUTS
*
*       NUSE_PARTITION_POOL pool      index of the pool to be initialized
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

#if NUSE_PARTITION_POOL_NUMBER != 0

    RAM U8 NUSE_Partition_Pool_Partition_Used[NUSE_PARTITION_POOL_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        RAM U8 NUSE_Partition_Pool_Blocking_Count[NUSE_PARTITION_POOL_NUMBER];

    #endif

    void NUSE_Init_Partition_Pool(NUSE_PARTITION_POOL pool)
    {
        NUSE_Partition_Pool_Partition_Used[pool] = 0;

        #if NUSE_BLOCKING_ENABLE

            NUSE_Partition_Pool_Blocking_Count[pool] = 0;

        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Mailbox()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any mailboxes  - the data arrays are also declared here
*       Mailbox data is set to 0 and the status set to unused
*
*   INPUTS
*
*       NUSE_MAILBOX mailbox      index of the mailbox to be initialized
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

#if NUSE_MAILBOX_NUMBER != 0

    RAM ADDR NUSE_Mailbox_Data[NUSE_MAILBOX_NUMBER];
    RAM U8 NUSE_Mailbox_Status[NUSE_MAILBOX_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        RAM U8 NUSE_Mailbox_Blocking_Count[NUSE_MAILBOX_NUMBER];

    #endif

    void NUSE_Init_Mailbox(NUSE_MAILBOX mailbox)
    {
        NUSE_Mailbox_Data[mailbox] = 0;
        NUSE_Mailbox_Status[mailbox] = 0;

        #if NUSE_BLOCKING_ENABLE

            NUSE_Mailbox_Blocking_Count[mailbox] = 0;

        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Queue()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any queues  - the data arrays are also declared here
*       Head and tail pointers [indices] are initialized to the start of the data area
*       Items count is set to 0
*
*   INPUTS
*
*       NUSE_QUEUE queue      index of the queue to be initialized
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

#if NUSE_QUEUE_NUMBER != 0

    RAM U8 NUSE_Queue_Head[NUSE_QUEUE_NUMBER];
    RAM U8 NUSE_Queue_Tail[NUSE_QUEUE_NUMBER];
    RAM U8 NUSE_Queue_Items[NUSE_QUEUE_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        RAM U8 NUSE_Queue_Blocking_Count[NUSE_QUEUE_NUMBER];

    #endif

    void NUSE_Init_Queue(NUSE_QUEUE queue)
    {
        NUSE_Queue_Head[queue] = 0;
        NUSE_Queue_Tail[queue] = 0;
        NUSE_Queue_Items[queue] = 0;

        #if NUSE_BLOCKING_ENABLE

            NUSE_Queue_Blocking_Count[queue] = 0;

        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Pipe()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any pipes  - the data arrays are also declared here
*       Head and tail pointers [indices] are initialized to the start of the data area
*       Items count is set to 0
*
*   INPUTS
*
*       NUSE_PIPE pipe        index of the pipe to be initialized
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

#if NUSE_PIPE_NUMBER != 0

    RAM U8 NUSE_Pipe_Head[NUSE_PIPE_NUMBER];
    RAM U8 NUSE_Pipe_Tail[NUSE_PIPE_NUMBER];
    RAM U8 NUSE_Pipe_Items[NUSE_PIPE_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        RAM U8 NUSE_Pipe_Blocking_Count[NUSE_PIPE_NUMBER];

    #endif

    void NUSE_Init_Pipe(NUSE_PIPE pipe)
    {
        NUSE_Pipe_Head[pipe] = 0;
        NUSE_Pipe_Tail[pipe] = 0;
        NUSE_Pipe_Items[pipe] = 0;

        #if NUSE_BLOCKING_ENABLE

            NUSE_Pipe_Blocking_Count[pipe] = 0;

        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Semaphore()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any semaphores  - the data arrays are also declared here
*       Counter is set to the required initial value
*
*   INPUTS
*
*       NUSE_SEMAPHORE semaphore      index of the semaphore to be initialized
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

#if NUSE_SEMAPHORE_NUMBER != 0

    RAM U8 NUSE_Semaphore_Counter[NUSE_SEMAPHORE_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        RAM U8 NUSE_Semaphore_Blocking_Count[NUSE_SEMAPHORE_NUMBER];

    #endif

    void NUSE_Init_Semaphore(NUSE_SEMAPHORE semaphore)
    {
        NUSE_Semaphore_Counter[semaphore] = NUSE_Semaphore_Initial_Value[semaphore];

        #if NUSE_BLOCKING_ENABLE

            NUSE_Semaphore_Blocking_Count[semaphore] = 0;

        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Event_Group()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any event groups  - the data arrays are also declared here
*       Event flags are all cleared
*
*   INPUTS
*
*       NUSE_EVENT_GROUP group        index of the event group to be initialized
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

#if NUSE_EVENT_GROUP_NUMBER != 0

    RAM U8 NUSE_Event_Group_Data[NUSE_EVENT_GROUP_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        RAM U8 NUSE_Event_Group_Blocking_Count[NUSE_EVENT_GROUP_NUMBER];

    #endif

    void NUSE_Init_Event_Group(NUSE_EVENT_GROUP group)
    {
        NUSE_Event_Group_Data[group] = 0;

        #if NUSE_BLOCKING_ENABLE

            NUSE_Event_Group_Blocking_Count[group] = 0;

        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init_Timer()
*
*   DESCRIPTION
*
*       Initialization of global RAM data for any event timers  - the data arrays are also declared here
*       Timer is set to be disabled; initial time is set; expiration count set to 0
*
*   INPUTS
*
*       NUSE_TIMER timer      index of the timer to be initialized
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

#if NUSE_TIMER_NUMBER != 0

    RAM U8 NUSE_Timer_Status[NUSE_TIMER_NUMBER];
    RAM U16 NUSE_Timer_Value[NUSE_TIMER_NUMBER];
    RAM U8 NUSE_Timer_Expirations_Counter[NUSE_TIMER_NUMBER];

    void NUSE_Init_Timer(NUSE_TIMER timer)
    {
        NUSE_Timer_Status[timer] = FALSE;
        NUSE_Timer_Value[timer] = NUSE_Timer_Initial_Time[timer];
        NUSE_Timer_Expirations_Counter[timer] = 0;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Init()
*
*   DESCRIPTION
*
*       Primary initialization routine called on start-up
*       Initializes global variables, then all the kernel objects [using functions above]
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

void NUSE_Init(void)
{
    U8 index;

    /* global data */

    NUSE_Task_Active = 0;

    NUSE_Task_State = NUSE_STARTUP_CONTEXT;

    #if NUSE_SYSTEM_TIME_SUPPORT
        NUSE_Tick_Clock = 0;
    #endif

    #if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER
        NUSE_Time_Slice_Ticks = NUSE_TIME_SLICE_TICKS;
    #endif

    /* tasks */

    #if ((NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER) || NUSE_SIGNAL_SUPPORT || NUSE_TASK_SLEEP || NUSE_SUSPEND_ENABLE || NUSE_SCHEDULE_COUNT_SUPPORT)
        for (index=0; index<NUSE_TASK_NUMBER; index++)
        {
                NUSE_Init_Task(index);
        }
    #endif

    /* partition pools */

    #if NUSE_PARTITION_POOL_NUMBER != 0
        for (index=0; index<NUSE_PARTITION_POOL_NUMBER; index++)
        {
            NUSE_Init_Partition_Pool(index);
        }
    #endif

    /* mailboxes */

    #if NUSE_MAILBOX_NUMBER != 0
        for (index=0; index<NUSE_MAILBOX_NUMBER; index++)
        {
            NUSE_Init_Mailbox(index);
        }
    #endif

    /* queues */

    #if NUSE_QUEUE_NUMBER != 0
        for (index=0; index<NUSE_QUEUE_NUMBER; index++)
        {
            NUSE_Init_Queue(index);
        }
    #endif

    /* pipes */

    #if NUSE_PIPE_NUMBER != 0
        for (index=0; index<NUSE_PIPE_NUMBER; index++)
        {
            NUSE_Init_Pipe(index);
        }
    #endif

    /* semaphores */

    #if NUSE_SEMAPHORE_NUMBER != 0
        for (index=0; index<NUSE_SEMAPHORE_NUMBER; index++)
        {
            NUSE_Init_Semaphore(index);
        }
    #endif

    /* event groups */

    #if NUSE_EVENT_GROUP_NUMBER != 0
        for (index=0; index<NUSE_EVENT_GROUP_NUMBER; index++)
        {
            NUSE_Init_Event_Group(index);
        }
    #endif

    /* timers */

    #if NUSE_TIMER_NUMBER != 0
        for (index=0; index<NUSE_TIMER_NUMBER; index++)
        {
            NUSE_Init_Timer(index);
        }
    #endif
}
