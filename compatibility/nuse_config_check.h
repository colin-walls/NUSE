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

#if NU_CHANGE_PREEMPTION
    #error NUSE: NU_Change_Preemption() API not implemented
#endif

#if NU_CHANGE_PRIORITY
    #error NUSE: NU_Change_Priority() API not implemented
#endif

#if NU_CHANGE_TIME_SLICE
    #error NUSE: NU_Change_Time_Slice() API not implemented
#endif

#if NU_CREATE_TASK
    #error NUSE: NU_Create_Task() API not implemented
#endif

#if NU_DELETE_TASK
    #error NUSE: NU_Delete_Task() API not implemented
#endif

#if NU_TASK_POINTERS
    #error NUSE: NU_Task_Pointers() API not implemented
#endif

#if NU_RELINQUISH && (NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER)
    #error NUSE: NU_Relinquish() selected - not valid with priority scheduler
#endif

#if NU_RESUME_TASK && !NUSE_SUSPEND_ENABLE
    #error NUSE: NU_Resume_Task() selected  - task suspend not enabled
#endif

#if NU_SUSPEND_TASK && !NUSE_SUSPEND_ENABLE
    #error NUSE: NU_Suspend_Task() selected  - task suspend not enabled
#endif

#if NU_TERMINATE_TASK && !NUSE_SUSPEND_ENABLE
    #error NUSE: NU_Terminate_Task() selected  - task suspend not enabled
#endif

#if NUSE_INITIAL_TASK_STATE_SUPPORT && !NUSE_SUSPEND_ENABLE
    #error NUSE: Initial task state enabled - task suspend not enabled
#endif


/*** Dynamic memory [not implemented] ***/

#if NU_ALLOCATE_MEMORY
    #error NUSE: NU_Allocate_Memory() API not implemented
#endif

#if NU_ALLOCATE_ALIGNED_MEMORY
    #error NUSE: NU_Allocate_Aligned_Memory() API not implemented
#endif

#if NU_CREATE_MEMORY_POOL
    #error NUSE: NU_Create_Memory_Pool() API not implemented
#endif

#if NU_DEALLOCATE_MEMORY
    #error NUSE: NU_Deallocate_Memory() API not implemented
#endif

#if NU_DELETE_MEMORY_POOL
    #error NUSE: NU_Delete_Memory_Pool() API not implemented
#endif

#if NU_ESTABLISHED_MEMORY_POOLS
    #error NUSE: NU_Established_Memory_Pools() API not implemented
#endif

#if NU_MEMORY_POOL_INFORMATION
    #error NUSE: NU_Memory_Pool_Information() API not implemented
#endif

#if NU_MEMORY_POOL_POINTERS
    #error NUSE: NU_Memory_Pool_Pointers() API not implemented
#endif

/*** Partition pools ***/

#if NUSE_PARTITION_POOL_NUMBER > 16
    #error NUSE: invalid number of partition pools - must be 0-16
#endif

#if NUSE_PARTITION_POOL_NUMBER == 0

    #if NU_ALLOCATE_PARTITION
        #error NUSE: NU_Allocate_Partition() enabled - no partition pools configured
    #endif

    #if NU_DEALLOCATE_PARTITION
        #error NUSE: NU_Deallocate_Partition() enabled - no partition pools configured
    #endif

    #if NU_PARTITION_POOL_INFORMATION
        #error NUSE: NU_Partition_Pool_Information() enabled - no partition pools configured
    #endif

#endif

#if NU_PARTITION_POOL_POINTERS
    #error NUSE: NU_Partition_Pool_Pointers() API not implemented
#endif

#if NU_CREATE_PARTITION_POOL
    #error NUSE: NU_Create_Partition_Pool() API not implemented
#endif

#if NU_DELETE_PARTITION_POOL
    #error NUSE: NU_elete_Partition_Pool() API not implemented
#endif

/*** Mailboxes ***/

#if NUSE_MAILBOX_NUMBER > 16
    #error NUSE: invalid number of mailboxes - must be 0-16
#endif

#if NUSE_MAILBOX_NUMBER == 0

    #if NU_MAILBOX_INFORMATION
        #error NUSE: NU_Mailbox_Information() enabled - no mailboxes configured
    #endif

    #if NU_RECEIVE_FROM_MAILBOX
        #error NUSE: NU_Receive_From_Mailbox() enabled - no mailboxes configured
    #endif

    #if NU_RESET_MAILBOX
        #error NUSE: NU_Reset_Mailbox() enabled - no mailboxes configured
    #endif

    #if NU_SEND_TO_MAILBOX
        #error NUSE: NU_Send_To_Mailbox() enabled - no mailboxes configured
    #endif

#endif

#if NU_MAILBOX_POINTERS
    #error NUSE: NU_Mailbox_Pointers() API not implemented
#endif

#if NU_BROADCAST_TO_MAILBOX
    #error NUSE: NU_Broadcast_To_Mailbox() API not implemented
#endif

#if NU_CREATE_MAILBOX
    #error NUSE: NU_Create_Mailbox() API not implemented
#endif

#if NU_DELETE_MAILBOX
    #error NUSE: NU_Delete_Mailbox() API not implemented
#endif

/*** Queues ***/

#if NUSE_QUEUE_NUMBER > 16
    #error NUSE: invalid number of queues - must be 0-16
#endif

#if NUSE_QUEUE_NUMBER == 0

    #if NU_QUEUE_INFORMATION
        #error NUSE: NU_Queue_Information() enabled - no queues configured
    #endif

    #if NU_RECEIVE_FROM_QUEUE
        #error NUSE: NU_Receive_From_Queue() enabled - no queues configured
    #endif

    #if NU_RESET_QUEUE
        #error NUSE: NU_Reset_Queue() enabled - no queues configured
    #endif

    #if NU_SEND_TO_FRONT_OF_QUEUE
        #error NUSE: NU_Send_To_Front_Of_Queue() enabled - no queues configured
    #endif

    #if NU_SEND_TO_QUEUE
        #error NUSE: NU_Send_To_Queue() enabled - no queues configured
    #endif

#endif

#if NU_QUEUE_POINTERS
    #error NUSE: NU_Queue_Pointers() API not implemented
#endif

#if NU_BROADCAST_TO_QUEUE
    #error NUSE: NU_Broadcast_To_Queue() API not implemented
#endif

#if NU_CREATE_QUEUE
    #error NUSE: NU_Create_Queue() API not implemented
#endif

#if NU_DELETE_QUEUE
    #error NUSE: NU_Delete_Queue() API not implemented
#endif

/*** Pipes ***/

#if NUSE_PIPE_NUMBER > 16
    #error NUSE: invalid number of pipes - must be 0-16
#endif

#if NUSE_PIPE_NUMBER == 0

    #if NU_PIPE_INFORMATION
        #error NUSE: NU_Pipe_Information() enabled - no pipes configured
    #endif

    #if NU_RECEIVE_FROM_PIPE
        #error NUSE: NU_Receive_From_Pipe() enabled - no pipes configured
    #endif

    #if NU_RESET_PIPE
        #error NUSE: NU_Reset_Pipe() enabled - no pipes configured
    #endif

    #if NU_SEND_TO_FRONT_OF_PIPE
        #error NUSE: NU_Send_To_Front_Of_Pipe() enabled - no pipes configured
    #endif

    #if NU_SEND_TO_PIPE
        #error NUSE: NU_Send_To_Pipe() enabled - no pipes configured
    #endif

#endif

#if NU_PIPE_POINTERS
    #error NUSE: NU_Pipe_Pointers() API not implemented
#endif

#if NU_BROADCAST_TO_PIPE
    #error NUSE: NU_Broadcast_To_Pipe() API not implemented
#endif

#if NU_CREATE_PIPE
    #error NUSE: NU_Create_Pipe() API not implemented
#endif

#if NU_DELETE_PIPE
    #error NUSE: NU_Delete_Pipe() API not implemented
#endif

/*** Semaphores ***/

#if NUSE_SEMAPHORE_NUMBER > 16
    #error NUSE: invalid number of semaphores - must be 0-16
#endif

#if NUSE_SEMAPHORE_NUMBER == 0

    #if NU_OBTAIN_SEMAPHORE
        #error NUSE: NU_Obtain_Semaphore() enabled - no semaphores configured
    #endif

    #if NU_RELEASE_SEMAPHORE
        #error NUSE: NU_Release_Semaphore() enabled - no semaphores configured
    #endif

    #if NU_RESET_SEMAPHORE
        #error NUSE: NU_Reset_Semaphore() enabled - no semaphores configured
    #endif

    #if NU_SEMAPHORE_INFORMATION
        #error NUSE: NU_Semaphore_Information() enabled - no semaphores configured
    #endif

#endif

#if NU_SEMAPHORE_POINTERS
    #error NUSE: NU_Semaphore_Pointers() API not implemented
#endif

#if NU_DELETE_SEMAPHORE
    #error NUSE: NU_Delete_Semaphore() API not implemented
#endif

#if NU_CREATE_SEMAPHORE
    #error NUSE: NU_Create_Semaphore() API not implemented
#endif

/*** Event groups ***/

#if NUSE_EVENT_GROUP_NUMBER > 16
    #error NUSE: invalid number of event groups - must be 0-16
#endif

#if NUSE_EVENT_GROUP_NUMBER == 0

    #if NU_EVENT_GROUP_INFORMATION
        #error NUSE: NU_Event_Group_Information() enabled - no event groups configured
    #endif

    #if NU_RETRIEVE_EVENTS
        #error NUSE: NU_Retrieve_Events() enabled - no event groups configured
    #endif

    #if NU_SET_EVENTS
        #error NUSE: NU_Set_Events() enabled - no event groups configured
    #endif

#endif

#if NU_EVENT_GROUP_POINTERS
    #error NUSE: NU_Event_Group_Pointers() API not implemented
#endif

#if NU_CREATE_EVENT_GROUP
    #error NUSE: NU_Create_Event_Group() API not implemented
#endif

#if NU_DELETE_EVENT_GROUP
    #error NUSE: NU_Delete_Event_Group() API not implemented
#endif

/*** Signals ***/

#if !NUSE_SIGNAL_SUPPORT

    #if NU_RECEIVE_SIGNALS
        #error NUSE: NU_Receive_Signals() enabled - signals not configured
    #endif

    #if NU_SEND_SIGNALS
        #error NUSE: NU_Send_Signals() enabled - signals not configured
    #endif

#endif

#if NU_CONTROL_SIGNALS
    #error NUSE: NU_Control_Signals() API not implemented
#endif

#if NU_REGISTER_SIGNAL_HANDLER
    #error NUSE: NU_Register_Signal_Handler() API not implemented
#endif


/*** Timers ***/

#if NUSE_TIMER_NUMBER > 16
    #error NUSE: invalid number of timers - must be 0-16
#endif

#if NUSE_TIMER_NUMBER == 0

    #if NU_CONTROL_TIMER
        #error NUSE: NU_Control_Timer() enabled - no timers configured
    #endif

    #if NU_GET_REMAINING_TIME
        #error NUSE: NU_Get_Remaining_Time() enabled - no timers configured
    #endif

    #if NU_RESET_TIMER
        #error NUSE: NU_Reset_Timer() enabled - no timers configured
    #endif

    #if NU_TIMER_INFORMATION
        #error NUSE: NU_Timer_Information() enabled - no timers configured
    #endif

#endif

#if NU_TIMER_POINTERS
    #error NUSE: NU_Timer_Pointers() API not implemented
#endif

#if NU_CREATE_TIMER
    #error NUSE: NU_Create_Timer() API not implemented
#endif

#if NU_DELETE_TIMER
    #error NUSE: NU_Delete_Timer() API not implemented
#endif


/*** System time ***/

#if !NUSE_SYSTEM_TIME_SUPPORT

    #if NU_RETRIEVE_CLOCK
        #error NUSE: NU_Retrieve_Clock() enabled - system time not configured
    #endif

    #if NU_SET_CLOCK
        #error NUSE: NU_Set_Clock() enabled - system time not configured
    #endif

#endif

/*** Interrupts [not implemented] ***/

#if NU_ACTIVIATE_HISR
    #error NUSE: NU_Activiate_HISR() API not implemented
#endif

#if NU_CONTROL_INTERRUPTS
    #error NUSE: NU_Control_Interrupts() API not implemented
#endif

#if NU_CREATE_HISR
    #error NUSE: NU_Create_HISR() API not implemented
#endif

#if NU_CURRENT_HISR_POINTER
    #error NUSE: NU_Current_HISR_Pointer() API not implemented
#endif

#if NU_DELETE_HISR
    #error NUSE: NU_Delete_HISR() API not implemented
#endif

#if NU_ESTABLISHED_HISRS
    #error NUSE: NU_Established_HISRs() API not implemented
#endif

#if NU_HISR_INFORMATION
    #error NUSE: NU_Change_Preemption() API not implemented
#endif

#if NU_HISR_POINTERS
    #error NUSE: NU_HISR_Information() API not implemented
#endif

#if NU_LOCAL_CONTROL_INTERRUPTS
    #error NUSE: NU_Local_Control_Interrupts() API not implemented
#endif

#if NU_REGISTER_LISR
    #error NUSE: NU_Register_LISR() API not implemented
#endif

#if NU_SETUP_VECTOR
    #error NUSE: NU_Setup_Vector() API not implemented
#endif


/*** System Diagnostics ***/

#if NU_ASSERT
    #error NUSE: NU_ASSERT() API not implemented
#endif

#if NU_CHECK
    #error NUSE: NU_CHECK() API not implemented
#endif

#if NU_DISABLE_HISTORY_SAVING
    #error NUSE: NU_Disable_History_Saving() API not implemented
#endif

#if NU_ENABLE_HISTORY_SAVING
    #error NUSE: NU_Enable_History_Saving() API not implemented
#endif

#if NU_MAKE_HISTORY_ENTRY
    #error NUSE: NU_Make_History_Entry() API not implemented
#endif

#if NU_RETRIEVE_HISTORY_ENTRY
    #error NUSE: NU_Retrieve_History_Entry() API not implemented
#endif

/*** I/O Drivers [not implemented] ***/

#if NU_CREATE_DRIVER
    #error NUSE: NU_Create_Driver() API not implemented
#endif

#if NU_DELETE_DRIVER
    #error NUSE: NU_Delete_Driver() API not implemented
#endif

#if NU_DRIVER_POINTERS
    #error NUSE: NU_Driver_Pointers() API not implemented
#endif

#if NU_ESTABLISHED_DRIVERS
    #error NUSE: NU_Established_Drivers() API not implemented
#endif

#if NU_PROTECT
    #error NUSE: NU_Protect() API not implemented
#endif

#if NU_REQUEST_DRIVER
    #error NUSE: NU_Request_Driver() API not implemented
#endif

#if NU_RESUME_DRIVER
    #error NUSE: NU_Resume_Driver() API not implemented
#endif

#if NU_SUSPEND_DRIVER
    #error NUSE: NU_Suspend_Driver() API not implemented
#endif

#if NU_UNPROTECT
    #error NUSE: NU_Unprotect() API not implemented
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

