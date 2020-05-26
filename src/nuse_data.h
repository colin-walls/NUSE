/*************************************************************************
*
*   This file includes extern declarations of Nucleus SE global data etc.
*   This consists of:
*       Nucleus SE global variables [defined in nuse_globals.c]
*       Kernel object ROM data [defined in nuse_config.c]
*       Kernel object RAM data [defined in nuse.init.c]
*
*************************************************************************/


/* Check to see if the file has been included already.  */
#ifndef _NUSE_DATA_H_
#define _NUSE_DATA_H_


/* Kernel global variables */

extern ROM char * const NUSE_Release_Info;

extern RAM NUSE_TASK NUSE_Task_Active;

extern RAM U8 NUSE_Task_State;              /* initialized to "startup" */
extern RAM NUSE_TASK NUSE_Task_Saved_State;

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
    extern RAM NUSE_TASK NUSE_Task_Next;
#endif

#if NUSE_SYSTEM_TIME_SUPPORT || NUSE_INCLUDE_EVERYTHING
    extern RAM U32 NUSE_Tick_Clock;             /* system clock */
#endif

#if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER
    extern RAM U16 NUSE_Time_Slice_Ticks;
#endif

/* Kernel object data structures */

/* Task ROM Data */

extern ROM ADDR NUSE_Task_Start_Address[NUSE_TASK_NUMBER];

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
    extern ROM ADDR NUSE_Task_Stack_Base[NUSE_TASK_NUMBER];
    extern ROM U16 NUSE_Task_Stack_Size[NUSE_TASK_NUMBER];
#endif

#if NUSE_INITIAL_TASK_STATE_SUPPORT || NUSE_INCLUDE_EVERYTHING
    extern ROM U8 NUSE_Task_Initial_State[NUSE_TASK_NUMBER];
#endif

/* Task RAM Data */

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
    extern RAM ADDR NUSE_Task_Context[NUSE_TASK_NUMBER][NUSE_REGISTERS];
#endif

#if NUSE_TASK_SLEEP || NUSE_INCLUDE_EVERYTHING
    extern RAM U16 NUSE_Task_Timeout_Counter[NUSE_TASK_NUMBER];
#endif

#if NUSE_SCHEDULE_COUNT_SUPPORT || NUSE_INCLUDE_EVERYTHING
    extern RAM U16 NUSE_Task_Schedule_Count[NUSE_TASK_NUMBER];
#endif

#if NUSE_SUSPEND_ENABLE || NUSE_INCLUDE_EVERYTHING
    extern RAM U8 NUSE_Task_Status[NUSE_TASK_NUMBER];
#endif

#if NUSE_BLOCKING_ENABLE || NUSE_INCLUDE_EVERYTHING
    extern RAM U8 NUSE_Task_Blocking_Return[NUSE_TASK_NUMBER];
#endif

#if NUSE_SIGNAL_SUPPORT || NUSE_INCLUDE_EVERYTHING
    extern RAM U8 NUSE_Task_Signal_Flags[NUSE_TASK_NUMBER];
#endif

#if NUSE_SCHEDULE_COUNT_SUPPORT || NUSE_INCLUDE_EVERYTHING
    extern RAM U16 NUSE_Task_Schedule_Count[NUSE_TASK_NUMBER];
#endif

#if NUSE_PARTITION_POOL_NUMBER > 0

    /* Partition Pool ROM Data */

    extern ROM ADDR NUSE_Partition_Pool_Data_Address[NUSE_PARTITION_POOL_NUMBER];
    extern ROM U8 NUSE_Partition_Pool_Partition_Number[NUSE_PARTITION_POOL_NUMBER];
    extern ROM U16 NUSE_Partition_Pool_Partition_Size[NUSE_PARTITION_POOL_NUMBER];

    /* Partition Pool RAM Data */

    extern RAM U8 NUSE_Partition_Pool_Partition_Used[NUSE_PARTITION_POOL_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        extern RAM U8 NUSE_Partition_Pool_Blocking_Count[NUSE_PARTITION_POOL_NUMBER];

    #endif

#endif

#if NUSE_MAILBOX_NUMBER > 0

    /* Mailbox RAM Data */

    extern RAM ADDR NUSE_Mailbox_Data[NUSE_MAILBOX_NUMBER];
    extern RAM U8 NUSE_Mailbox_Status[NUSE_MAILBOX_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        extern RAM U8 NUSE_Mailbox_Blocking_Count[NUSE_MAILBOX_NUMBER];

    #endif

#endif

#if NUSE_QUEUE_NUMBER > 0

    /* Queue ROM Data */

    extern ROM ADDR *NUSE_Queue_Data[NUSE_QUEUE_NUMBER];
    extern ROM U8 NUSE_Queue_Size[NUSE_QUEUE_NUMBER];

    /* Queue RAM Data */

    extern RAM U8 NUSE_Queue_Head[NUSE_QUEUE_NUMBER];
    extern RAM U8 NUSE_Queue_Tail[NUSE_QUEUE_NUMBER];
    extern RAM U8 NUSE_Queue_Items[NUSE_QUEUE_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        extern RAM U8 NUSE_Queue_Blocking_Count[NUSE_QUEUE_NUMBER];

    #endif

#endif

#if NUSE_PIPE_NUMBER > 0

    /* Pipe ROM Data */

    extern ROM U8 *NUSE_Pipe_Data[NUSE_PIPE_NUMBER];
    extern ROM U8 NUSE_Pipe_Size[NUSE_PIPE_NUMBER];
    extern ROM U8 NUSE_Pipe_Message_Size[NUSE_PIPE_NUMBER];

    /* Pipe RAM Data */

    extern RAM U8 NUSE_Pipe_Head[NUSE_PIPE_NUMBER];
    extern RAM U8 NUSE_Pipe_Tail[NUSE_PIPE_NUMBER];
    extern RAM U8 NUSE_Pipe_Items[NUSE_PIPE_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        extern RAM U8 NUSE_Pipe_Blocking_Count[NUSE_PIPE_NUMBER];

    #endif

#endif

#if NUSE_SEMAPHORE_NUMBER > 0

    /* Semaphore ROM Data */

    extern ROM U8 NUSE_Semaphore_Initial_Value[NUSE_SEMAPHORE_NUMBER];

    /* Semaphore RAM Data */

    extern RAM U8 NUSE_Semaphore_Counter[NUSE_SEMAPHORE_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        extern RAM U8 NUSE_Semaphore_Blocking_Count[NUSE_SEMAPHORE_NUMBER];

    #endif

#endif

#if NUSE_EVENT_GROUP_NUMBER > 0

    /* Event Group RAM Data */

    extern RAM U8 NUSE_Event_Group_Data[NUSE_EVENT_GROUP_NUMBER];

    #if NUSE_BLOCKING_ENABLE

        extern RAM U8 NUSE_Event_Group_Blocking_Count[NUSE_EVENT_GROUP_NUMBER];

    #endif

#endif

#if NUSE_TIMER_NUMBER > 0

    /* Timer ROM Data */

    extern ROM U16 NUSE_Timer_Initial_Time[NUSE_TIMER_NUMBER];
    extern ROM U16 NUSE_Timer_Reschedule_Time[NUSE_TIMER_NUMBER];
    #if NUSE_TIMER_EXPIRATION_ROUTINE_SUPPORT || NUSE_INCLUDE_EVERYTHING
        extern ROM ADDR NUSE_Timer_Expiration_Routine_Address[NUSE_TIMER_NUMBER];
        extern ROM U8 NUSE_Timer_Expiration_Routine_Parameter[NUSE_TIMER_NUMBER];
    #endif

    /* Timer RAM Data */

    extern RAM U8 NUSE_Timer_Status[NUSE_TIMER_NUMBER];
    extern RAM U16 NUSE_Timer_Value[NUSE_TIMER_NUMBER];
    extern RAM U8 NUSE_Timer_Expirations_Counter[NUSE_TIMER_NUMBER];

#endif

#endif  /* ! _NUSE_DATA_H_ */

