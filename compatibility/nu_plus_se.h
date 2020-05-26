/*************************************************************************
*
*   This file contains the macros that perform the translation between the
*   Nucleus PLUS API [calls beginning NU_] and the Nucleus SE API [calls beginning NUSE_]
*   It is recommended that application code use the Nucleus PLUS API [for portability]
*   and must #include this file
*
*************************************************************************/

/* Check to see if the file has been included already.  */
#ifndef _NUSE_H_
#define _NUSE_H_


#include "nuse_types.h"
#include "nuse_config.h"
#include "nuse_prototypes.h"
#include "nuse_codes.h"
#include "nuse_binary.h"

/* Nucleus API Definition */

/* Task Control */

#if NU_CHECK_STACK || NUSE_INCLUDE_EVERYTHING
    #define NU_Check_Stack() \
        NUSE_Task_Check_Stack(0)
#endif

#if NU_CURRENT_TASK_POINTER || NUSE_INCLUDE_EVERYTHING
    #define NU_Current_Task_Pointer() \
        NUSE_Task_Current()
#endif

#if NU_ESTABLISHED_TASKS || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Tasks() \
        NUSE_Task_Count()
#endif

#if NU_RELINQUISH || NUSE_INCLUDE_EVERYTHING
    #if (NUSE_SCHEDULER_TYPE == NUSE_TIME_SLICE_SCHEDULER) || (NUSE_SCHEDULER_TYPE == NUSE_ROUND_ROBIN_SCHEDULER)
        #define NU_Relinquish() \
            NUSE_Task_Relinquish()
    #else
        #define NU_Relinquish()
    #endif
#endif

#if NU_RESET_TASK || NUSE_INCLUDE_EVERYTHING
    #define NU_Reset_Task(task, argc, argv) \
        NUSE_Task_Reset(task)
#endif

#if NU_RESUME_TASK || NUSE_INCLUDE_EVERYTHING
    #define NU_Resume_Task(task) \
        NUSE_Task_Resume(task)
#endif

#if NU_SLEEP || NUSE_INCLUDE_EVERYTHING
    #define NU_Sleep(ticks) \
        NUSE_Task_Sleep(ticks)
#endif

#if NU_SUSPEND_TASK || NUSE_INCLUDE_EVERYTHING
    #define NU_Suspend_Task(task) \
        NUSE_Task_Suspend(task)
#endif

#if NU_TASK_INFORMATION || NUSE_INCLUDE_EVERYTHING
    #define NU_Task_Information(task, name, task_status, scheduled_count, priority, preempt, time_slice, stack_base, stack_size, minimum_stack) \
        NUSE_Task_Information(task, task_status, scheduled_count, stack_base, stack_size), \
    (*name = 0), \
    (*priority = task)
#endif

#if NU_TERMINATE_TASK || NUSE_INCLUDE_EVERYTHING
    #define NU_Terminate_Task(task) \
        NUSE_Task_Suspend(task)
#endif

/* Partition Memory */

#if NU_ESTABLISHED_PARTITION_POOLS || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Partition_Pools() \
        NUSE_Partition_Pool_Count()
#endif

#if NUSE_PARTITION_POOL_NUMBER > 0

    #if NU_ALLOCATE_PARTITION || NUSE_INCLUDE_EVERYTHING
        #define NU_Allocate_Partition(pool, return_pointer, suspend) \
            NUSE_Partition_Allocate(pool, return_pointer, suspend)
    #endif

    #if NU_DEALLOCATE_PARTITION || NUSE_INCLUDE_EVERYTHING
        #define NU_Deallocate_Partition(partition) \
            NUSE_Partition_Deallocate(partition)
    #endif

    #if NU_PARTITION_POOL_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Partition_Pool_Information(pool, name, start_address, pool_size, partition_size, available, allocated, suspend_type, tasks_waiting, first_task) \
            NUSE_Partition_Pool_Information(pool, start_address, pool_size, partition_size, available, allocated, tasks_waiting, first_task), \
            (*name = 0), \
            (*suspend_type = NU_PRIORITY)
    #endif

#endif

/* Mailboxes */

#if NU_ESTABLISHED_MAILBOXES || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Mailboxes() \
        NUSE_Mailbox_Count()
#endif

#if NUSE_MAILBOX_NUMBER > 0

    #if NU_MAILBOX_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Mailbox_Information(mailbox, name, suspend_type, message_present, tasks_waiting, first_task) \
            NUSE_Mailbox_Information(mailbox, message_present, tasks_waiting, first_task), \
            (*name = 0), \
            (*suspend_type = NU_PRIORITY)
    #endif

    #if NU_RESET_MAILBOX || NUSE_INCLUDE_EVERYTHING
        #define NU_Reset_Mailbox(mailbox) \
            NUSE_Mailbox_Reset(mailbox)
    #endif

    #if NU_SEND_TO_MAILBOX || NUSE_INCLUDE_EVERYTHING
        #define NU_Send_To_Mailbox(mailbox, message, suspend) \
            NUSE_Mailbox_Send(mailbox, message, suspend)
    #endif

    #if NU_RECEIVE_FROM_MAILBOX || NUSE_INCLUDE_EVERYTHING
        #define NU_Receive_From_Mailbox(mailbox, message, suspend) \
            NUSE_Mailbox_Receive(mailbox, message, suspend)
    #endif

#endif

/* Queues */

#if NU_ESTABLISHED_QUEUES || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Queues() \
        NUSE_Queue_Count()
#endif

#if NUSE_QUEUE_NUMBER > 0

    #if NU_QUEUE_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Queue_Information(queue, name, start_address, queue_size, available, messages, message_type, message_size, suspend_type, tasks_waiting, first_task) \
            NUSE_Queue_Information(queue, start_address, queue_size, available, messages, tasks_waiting, first_task), \
            (*name = 0), \
            (*suspend_type = NU_PRIORITY), \
            (*message_size = 1), \
            (*message_type = NU_FIXED_SIZE)
    #endif

    #if NU_RECEIVE_FROM_QUEUE || NUSE_INCLUDE_EVERYTHING
        #define NU_Receive_From_Queue(queue, message, size, actual_size, suspend) \
            NUSE_Queue_Receive(queue, message, size, suspend), \
            (*actual_size = 1)
    #endif

    #if NU_RESET_QUEUE || NUSE_INCLUDE_EVERYTHING
        #define NU_Reset_Queue(queue) \
            NUSE_Queue_Reset(queue)
    #endif

    #if NU_SEND_TO_FRONT_OF_QUEUE || NUSE_INCLUDE_EVERYTHING
        #define NU_Send_To_Front_Of_Queue(queue, message, size, suspend) \
            NUSE_Queue_Jam(queue, message, size, suspend)
    #endif

    #if NU_SEND_TO_QUEUE || NUSE_INCLUDE_EVERYTHING
        #define NU_Send_To_Queue(queue, message, size, suspend) \
            NUSE_Queue_Send(queue, message, size, suspend)
    #endif

#endif

/* Pipes */

#if NU_ESTABLISHED_PIPES || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Pipes() \
        NUSE_Pipe_Count()
#endif

#if NUSE_PIPE_NUMBER > 0

    #if NU_PIPE_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Pipe_Information(pipe, name, start_address, pipe_size, available, messages, message_type, message_size, suspend_type, tasks_waiting, first_task) \
            NUSE_Pipe_Information(pipe, start_address, pipe_size, available, messages, message_size, tasks_waiting, first_task), \
            (*name = 0), \
            (*suspend_type = NU_PRIORITY), \
            (*message_type = NU_VARIABLE_SIZE)
    #endif

    #if NU_RECEIVE_FROM_PIPE || NUSE_INCLUDE_EVERYTHING
        #define NU_Receive_From_Pipe(pipe, message, size, actual_size, suspend) \
            NUSE_Pipe_Receive(pipe, message, size, actual_size, suspend)
    #endif

    #if NU_RESET_PIPE || NUSE_INCLUDE_EVERYTHING
        #define NU_Reset_Pipe(pipe) \
            NUSE_Pipe_Reset(pipe)
    #endif

    #if NU_SEND_TO_FRONT_OF_PIPE || NUSE_INCLUDE_EVERYTHING
        #define NU_Send_To_Front_Of_Pipe(pipe, message, size, suspend) \
            NUSE_Pipe_Jam(pipe, message, size, suspend)
    #endif

    #if NU_SEND_TO_PIPE || NUSE_INCLUDE_EVERYTHING
        #define NU_Send_To_Pipe(pipe, message, size, suspend) \
            NUSE_Pipe_Send(pipe, message, size, suspend)
    #endif

#endif

/* Semaphores */

#if NU_ESTABLISHED_SEMAPHORES || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Semaphores() \
        NUSE_Semaphore_Count()
#endif

#if NUSE_SEMAPHORE_NUMBER > 0

    #if NU_OBTAIN_SEMAPHORE || NUSE_INCLUDE_EVERYTHING
        #define NU_Obtain_Semaphore(semaphore, suspend) \
            NUSE_Semaphore_Obtain(semaphore, suspend)
    #endif

    #if NU_RELEASE_SEMAPHORE || NUSE_INCLUDE_EVERYTHING
        #define NU_Release_Semaphore(semaphore) \
            NUSE_Semaphore_Release(semaphore)
    #endif

    #if NU_RESET_SEMAPHORE || NUSE_INCLUDE_EVERYTHING
        #define NU_Reset_Semaphore(semaphore, initial_count) \
            NUSE_Semaphore_Reset(semaphore, initial_count)
    #endif

    #if NU_SEMAPHORE_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Semaphore_Information(semaphore, name, current_count, suspend_type, tasks_waiting, first_task) \
            NUSE_Semaphore_Information(semaphore, current_count, tasks_waiting, first_task), \
            (*name = 0), \
            (*suspend_type = NU_PRIORITY)
    #endif

#endif

/* Event Groups */

#if NU_ESTABLISHED_EVENT_GROUPS || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Event_Groups() \
        NUSE_Event_Group_Count()
#endif

#if NUSE_EVENT_GROUP_NUMBER > 0

    #if NU_EVENT_GROUP_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Event_Group_Information(group, name, event_flags, tasks_waiting, first_task) \
            NUSE_Event_Group_Information(group, event_flags, tasks_waiting, first_task), \
            (*name = 0)
    #endif

    #if NU_RETRIEVE_EVENTS || NUSE_INCLUDE_EVERYTHING
        #define NU_Retrieve_Events(group, requested_events, operation, retrieved_events, suspend) \
            NUSE_Event_Group_Retrieve(group, requested_events, operation, retrieved_events, suspend)
    #endif

    #if NU_SET_EVENTS || NUSE_INCLUDE_EVERYTHING
        #define NU_Set_Events(group, event_flags, operation) \
            NUSE_Event_Group_Set(group, event_flags, operation)
    #endif

#endif

/* Signals */

#if NUSE_SIGNAL_SUPPORT

    #if NU_RECEIVE_SIGNALS || NUSE_INCLUDE_EVERYTHING
        #define NU_Receive_Signals() \
            NUSE_Signals_Receive()
    #endif

    #if NU_SEND_SIGNALS || NUSE_INCLUDE_EVERYTHING
        #define NU_Send_Signals(task, signals) \
            NUSE_Signals_Send(task, signals)
    #endif

#endif

/* Timers */

#if NU_ESTABLISHED_TIMERS || NUSE_INCLUDE_EVERYTHING
    #define NU_Established_Timers() \
        NUSE_Timer_Count()
#endif

#if NUSE_TIMER_NUMBER > 0

    #if NU_CONTROL_TIMER || NUSE_INCLUDE_EVERYTHING
        #define NU_Control_Timer(timer, enable) \
            NUSE_Timer_Control(timer, enable)
    #endif

    #if NU_GET_REMAINING_TIME || NUSE_INCLUDE_EVERYTHING
        #define NU_Get_Remaining_Time(timer, remaining_time) \
            NUSE_Timer_Get_Remaining(timer, remaining_time)
    #endif

    #if NU_RESET_TIMER || NUSE_INCLUDE_EVERYTHING
        #define NU_Reset_Timer(timer, expiration_routine, initial_time, reschedule_time, enable) \
            NUSE_Timer_Reset(timer, enable)
    #endif

    #if NU_TIMER_INFORMATION || NUSE_INCLUDE_EVERYTHING
        #define NU_Timer_Information(timer, name, enable, expirations, id, initial_time, reschedule_time) \
            NUSE_Timer_Information(timer, enable, expirations, id, initial_time, reschedule_time), \
            (*name = 0)
    #endif

#endif

/* System Time */

#if NUSE_SYSTEM_TIME_SUPPORT || NUSE_INCLUDE_EVERYTHING

    #if NU_RETRIEVE_CLOCK || NUSE_INCLUDE_EVERYTHING
        #define NU_Retrieve_Clock() \
            NUSE_Clock_Retrieve()
    #endif

    #if NU_SET_CLOCK || NUSE_INCLUDE_EVERYTHING
        #define NU_Set_Clock(new_value) \
            NUSE_Clock_Set(new_value)
    #endif

#endif

/* System Diagnostics */

#if NU_RELEASE_INFORMATION || NUSE_INCLUDE_EVERYTHING
    #define NU_Release_Information() \
        NUSE_Release_Information()
#endif


#endif  /* ! _NUSE_H_ */

