/*************************************************************************
*
*   This file, which is included into nuse_config.c does some consistency
*   checks on the configuraton parameters set up in nuse_config.h
*   It should not normally be modified by users
*
*************************************************************************/


/* Check to see if the file has been included already.  */
#ifndef _NUSE_CONFIG_CHECK_H_
#define _NUSE_CONFIG_CHECK_H_


/*** Tasks and task control ***/

#if NUSE_TASK_NUMBER < 1 || NUSE_TASK_NUMBER > 16
    #error NUSE: invalid number of tasks - must be 1-16
#endif

#if NUSE_TASK_RELINQUISH && (NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER)
    #error NUSE: NUSE_Task_Relinquish() selected - not valid with priority scheduler
#endif

#if NUSE_TASK_RESUME && !NUSE_SUSPEND_ENABLE
    #error NUSE: NUSE_Task_Resume() selected  - task suspend not enabled
#endif

#if NUSE_TASK_SUSPEND && !NUSE_SUSPEND_ENABLE
    #error NUSE: NUSE_Task_Suspend() selected  - task suspend not enabled
#endif

#if NUSE_INITIAL_TASK_STATE_SUPPORT && !NUSE_SUSPEND_ENABLE
    #error NUSE: Initial task state enabled - task suspend not enabled
#endif


/*** Partition pools ***/

#if NUSE_PARTITION_POOL_NUMBER > 16
    #error NUSE: invalid number of partition pools - must be 0-16
#endif

#if NUSE_PARTITION_POOL_NUMBER == 0

    #if NUSE_PARTITION_ALLOCATE
        #error NUSE: NUSE_Partition_Allocate() enabled - no partition pools configured
    #endif

    #if NUSE_PARTITION_DEALLOCATE
        #error NUSE: NUSE_Partition_Deallocate() enabled - no partition pools configured
    #endif

    #if NUSE_PARTITION_POOL_INFORMATION
        #error NUSE: NUSE_Partition_Pool_Information() enabled - no partition pools configured
    #endif

#endif

/*** Mailboxes ***/

#if NUSE_MAILBOX_NUMBER > 16
    #error NUSE: invalid number of mailboxes - must be 0-16
#endif

#if NUSE_MAILBOX_NUMBER == 0

    #if NUSE_MAILBOX_INFORMATION
        #error NUSE: NUSE_Mailbox_Information() enabled - no mailboxes configured
    #endif

    #if NUSE_MAILBOX_RECEIVE
        #error NUSE: NUSE_Mailbox_Receive() enabled - no mailboxes configured
    #endif

    #if NUSE_MAILBOX_RESET
        #error NUSE: NUSE_Mailbox_Reset() enabled - no mailboxes configured
    #endif

    #if NUSE_MAILBOX_SEND
        #error NUSE: NUSE_Mailbox_Send() enabled - no mailboxes configured
    #endif

#endif

/*** Queues ***/

#if NUSE_QUEUE_NUMBER > 16
    #error NUSE: invalid number of queues - must be 0-16
#endif

#if NUSE_QUEUE_NUMBER == 0

    #if NUSE_QUEUE_INFORMATION
        #error NUSE: NUSE_Queue_Information() enabled - no queues configured
    #endif

    #if NUSE_QUEUE_RECEIVE
        #error NUSE: NUSE_Queue_Receive() enabled - no queues configured
    #endif

    #if NUSE_QUEUE_RESET
        #error NUSE: NUSE_Queue_Reset() enabled - no queues configured
    #endif

    #if NUSE_QUEUE_JAM
        #error NUSE: NUSE_Queue_Jam() enabled - no queues configured
    #endif

    #if NUSE_QUEUE_SEND
        #error NUSE: NUSE_Queue_Send() enabled - no queues configured
    #endif

#endif

/*** Pipes ***/

#if NUSE_PIPE_NUMBER > 16
    #error NUSE: invalid number of pipes - must be 0-16
#endif

#if NUSE_PIPE_NUMBER == 0

    #if NUSE_PIPE_INFORMATION
        #error NUSE: NUSE_Pipe_Information() enabled - no pipes configured
    #endif

    #if NUSE_PIPE_RECEIVE
        #error NUSE: NUSE_Pipe_Receive() enabled - no pipes configured
    #endif

    #if NUSE_PIPE_RESET
        #error NUSE: NUSE_Pipe_Reset() enabled - no pipes configured
    #endif

    #if NUSE_PIPE_JAM
        #error NUSE: NUSE_Pipe_Jam() enabled - no pipes configured
    #endif

    #if NUSE_PIPE_SEND
        #error NUSE: NUSE_Pipe_Send() enabled - no pipes configured
    #endif

#endif

/*** Semaphores ***/

#if NUSE_SEMAPHORE_NUMBER > 16
    #error NUSE: invalid number of semaphores - must be 0-16
#endif

#if NUSE_SEMAPHORE_NUMBER == 0

    #if NUSE_SEMAPHORE_OBTAIN
        #error NUSE: NUSE_Semaphore_Obtain() enabled - no semaphores configured
    #endif

    #if NUSE_SEMAPHORE_RELEASE
        #error NUSE: NUSE_Semaphore_Release() enabled - no semaphores configured
    #endif

    #if NUSE_SEMAPHORE_RESET
        #error NUSE: NUSE_Semaphore_Reset() enabled - no semaphores configured
    #endif

    #if NUSE_SEMAPHORE_INFORMATION
        #error NUSE: NUSE_Semaphore_Information() enabled - no semaphores configured
    #endif

#endif

/*** Event groups ***/

#if NUSE_EVENT_GROUP_NUMBER > 16
    #error NUSE: invalid number of event groups - must be 0-16
#endif

#if NUSE_EVENT_GROUP_NUMBER == 0

    #if NUSE_EVENT_GROUP_INFORMATION
        #error NUSE: NUSE_Event_Group_Information() enabled - no event groups configured
    #endif

    #if NUSE_EVENT_GROUP_RETRIEVE
        #error NUSE: NUSE_Event_Group_Retrieve() enabled - no event groups configured
    #endif

    #if NUSE_EVENT_GROUP_SET
        #error NUSE: NUSE_Event_Group_Set() enabled - no event groups configured
    #endif

#endif


/*** Signals ***/

#if !NUSE_SIGNAL_SUPPORT

    #if NUSE_SIGNALS_RECEIVE
        #error NUSE: NUSE_Signals_Receive() enabled - signals not configured
    #endif

    #if NUSE_SIGNALS_SEND
        #error NUSE: NUSE_Signals_Send() enabled - signals not configured
    #endif

#endif


/*** Timers ***/

#if NUSE_TIMER_NUMBER > 16
    #error NUSE: invalid number of timers - must be 0-16
#endif

#if NUSE_TIMER_NUMBER == 0

    #if NUSE_TIMER_CONTROL
        #error NUSE: NUSE_Timer_Control() enabled - no timers configured
    #endif

    #if NUSE_TIMER_GET_REMAINING
        #error NUSE: NUSE_Timer_Get_Remaining() enabled - no timers configured
    #endif

    #if NUSE_TIMER_RESET
        #error NUSE: NUSE_Timer_Reset() enabled - no timers configured
    #endif

    #if NUSE_TIMER_INFORMATION
        #error NUSE: NUSE_Timer_Information() enabled - no timers configured
    #endif

#endif


/*** System time ***/

#if !NUSE_SYSTEM_TIME_SUPPORT

    #if NUSE_CLOCK_RETRIEVE
        #error NUSE: NUSE_Clock_Retrieve() enabled - system time not configured
    #endif

    #if NUSE_CLOCK_SET
        #error NUSE: NUSE_Clock_Set() enabled - system time not configured
    #endif

#endif


/*** Scheduler and task suspend ***/

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER && NUSE_SCHEDULER_TYPE != NUSE_TIME_SLICE_SCHEDULER && NUSE_SCHEDULER_TYPE != NUSE_ROUND_ROBIN_SCHEDULER && NUSE_SCHEDULER_TYPE != NUSE_PRIORITY_SCHEDULER
    #error NUSE: invalid scheduler type selected
#endif

#if NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER
    #if NUSE_TIME_SLICE_TICKS == 0
        #error NUSE: 0 ticks interval selected for time slice scheduler
    #endif
#else
    #if NUSE_TIME_SLICE_TICKS != 0
        #error NUSE: time slice interval specified - time slice scheduler not selected
    #endif
#endif

#if !NUSE_SUSPEND_ENABLE && (NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER)
    #error NUSE: priority scheduler selected - task suspend not enabled
#endif

#if !NUSE_SUSPEND_ENABLE && NUSE_BLOCKING_ENABLE
    #error NUSE: blocking API calls enabled - task suspend not enabled
#endif

#if NUSE_BLOCKING_ENABLE && (NUSE_SCHEDULER_TYPE == NUSE_RUN_TO_COMPLETION_SCHEDULER)
    #error NUSE: blocking API calls enabled with RTC scheduler - not supported
#endif

#endif  /* ! _NUSE_CONFIG_CHECK_H_ */

