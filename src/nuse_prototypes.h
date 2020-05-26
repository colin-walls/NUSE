/*************************************************************************
*
*   This file contains prototypes for:
*       API service call functions
*       Scheduler functions
*       Initialization functions
*
*************************************************************************/

/* Check to see if the file has been included already.  */
#ifndef _NUSE_PROTOTYPES_H_
#define _NUSE_PROTOTYPES_H_


/* Task Control */

STATUS      NUSE_Task_Suspend(NUSE_TASK task);
STATUS      NUSE_Task_Resume(NUSE_TASK task);
void        NUSE_Task_Sleep(U16 ticks);
void        NUSE_Task_Relinquish(void);
NUSE_TASK   NUSE_Task_Current(void);
U16         NUSE_Task_Check_Stack(U8 dummy);
STATUS      NUSE_Task_Reset(NUSE_TASK task);
STATUS      NUSE_Task_Information(NUSE_TASK task, U8 *task_status, U16 *scheduled_count, ADDR *stack_base, U16 *stack_size);
U8          NUSE_Task_Count(void);

/* Partition Memory */

STATUS  NUSE_Partition_Allocate(NUSE_PARTITION_POOL pool, ADDR *return_pointer, U8 suspend);
STATUS  NUSE_Partition_Deallocate(ADDR partition);
STATUS  NUSE_Partition_Pool_Information(NUSE_PARTITION_POOL pool, ADDR *start_address, U32 *pool_size, U16 *partition_size, U8 *available, U8 *allocated, U8 *tasks_waiting, NUSE_TASK *first_task);
U8      NUSE_Partition_Pool_Count(void);

/* Mailboxes */

STATUS  NUSE_Mailbox_Send(NUSE_MAILBOX mailbox, ADDR *message, U8 suspend);
STATUS  NUSE_Mailbox_Receive(NUSE_MAILBOX mailbox, ADDR *message, U8 suspend);
STATUS  NUSE_Mailbox_Reset(NUSE_MAILBOX mailbox);
STATUS  NUSE_Mailbox_Information(NUSE_MAILBOX mailbox, U8 *message_present, U8 *tasks_waiting, NUSE_TASK *first_task);
U8      NUSE_Mailbox_Count(void);

/* Queues */

STATUS  NUSE_Queue_Send(NUSE_QUEUE queue, ADDR *message, U8 suspend);
STATUS  NUSE_Queue_Receive(NUSE_QUEUE queue, ADDR *message, U8 suspend);
STATUS  NUSE_Queue_Jam(NUSE_QUEUE queue, ADDR *message, U8 suspend);
STATUS  NUSE_Queue_Reset(NUSE_QUEUE queue);
STATUS  NUSE_Queue_Information(NUSE_QUEUE queue, ADDR *start_address, U8 *queue_size, U8 *available, U8 *messages, U8 *tasks_waiting, NUSE_TASK *first_task);
U8      NUSE_Queue_Count(void);

/* Pipes */

STATUS  NUSE_Pipe_Send(NUSE_PIPE pipe, U8 *message, U8 size, U8 suspend);
STATUS  NUSE_Pipe_Receive(NUSE_PIPE pipe, U8 *message, U8 size, U8 *actual_size, U8 suspend);
STATUS  NUSE_Pipe_Jam(NUSE_PIPE pipe, U8 *message, U8 size, U8 suspend);
STATUS  NUSE_Pipe_Reset(NUSE_PIPE pipe);
STATUS  NUSE_Pipe_Information(NUSE_PIPE pipe, ADDR *start_address, U8 *pipe_size, U8 *available, U8 *messages, U8 *message_size, U8 *tasks_waiting, NUSE_TASK *first_task);
U8      NUSE_Pipe_Count(void);

/* Semaphores */

STATUS  NUSE_Semaphore_Obtain(NUSE_SEMAPHORE semaphore, U8 suspend);
STATUS  NUSE_Semaphore_Release(NUSE_SEMAPHORE semaphore);
STATUS  NUSE_Semaphore_Reset(NUSE_SEMAPHORE semaphore, U8 initial_count);
STATUS  NUSE_Semaphore_Information(NUSE_SEMAPHORE semaphore, U8 *current_count, U8 *tasks_waiting, NUSE_TASK *first_task);
U8      NUSE_Semaphore_Count(void);

/* Event Groups */

STATUS  NUSE_Event_Group_Set(NUSE_EVENT_GROUP group, U8 event_flags, OPTION operation);
STATUS  NUSE_Event_Group_Retrieve(NUSE_EVENT_GROUP group, U8 requested_events, OPTION operation, U8 *retrieved_events, U8 suspend);
STATUS  NUSE_Event_Group_Information(NUSE_EVENT_GROUP group, U8 *event_flags, U8 *tasks_waiting, NUSE_TASK *first_task);
U8      NUSE_Event_Group_Count(void);

/* Signals */

STATUS  NUSE_Signals_Send(NUSE_TASK task, U8 signals);
U8      NUSE_Signals_Receive(void);

/* Timers */

STATUS  NUSE_Timer_Control(NUSE_TIMER timer, OPTION enable);
STATUS  NUSE_Timer_Get_Remaining(NUSE_TIMER timer, U16 *remaining_time);
STATUS  NUSE_Timer_Reset(NUSE_TIMER timer, OPTION enable);
STATUS  NUSE_Timer_Information(NUSE_TIMER timer, OPTION *enable, U8 *expirations, U8 *id, U16 *initial_time, U16 *reschedule_time);
U8      NUSE_Timer_Count(void);

/* System Time */

void    NUSE_Clock_Set(U32 new_value);
U32     NUSE_Clock_Retrieve(void);

/* System Diagnostics */

char  *NUSE_Release_Information(void);

/* Scheduler functions */

#if NUSE_SCHEDULER_TYPE == NUSE_RUN_TO_COMPLETION_SCHEDULER
    #define NUSE_Reschedule() ;
#else
    void NUSE_Context_Load(void);
    #if NUSE_SCHEDULER_TYPE == NUSE_PRIORITY_SCHEDULER
        void NUSE_Reschedule(U8 new_task);
    #else
        void NUSE_Reschedule(void);
    #endif
#endif
void NUSE_Wake_Task(NUSE_TASK task);
void NUSE_Suspend_Task(NUSE_TASK task, U8 suspend_code);
void NUSE_Scheduler(void);

/* Initialization */

void NUSE_Init(void);
void NUSE_Init_Task(NUSE_TASK);
void NUSE_Init_Partition_Pool(NUSE_PARTITION_POOL);
void NUSE_Init_Mailbox(NUSE_MAILBOX);
void NUSE_Init_Queue(NUSE_QUEUE);
void NUSE_Init_Pipe(NUSE_PIPE);
void NUSE_Init_Semaphore(NUSE_SEMAPHORE);
void NUSE_Init_Event_Group(NUSE_EVENT_GROUP);
void NUSE_Init_Timer(NUSE_TIMER);

/* Interrupts */

#if NUSE_SCHEDULER_TYPE == NUSE_RUN_TO_COMPLETION_SCHEDULER
    INTERRUPT void NUSE_Real_Time_Clock_ISR(void);
#else
    void NUSE_Real_Time_Clock_ISR(void);
#endif

#endif  /* ! _NUSE_PROTOTYPES_H_ */

