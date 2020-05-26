
#include "nuse_types.h"
#include "nuse_config.h"
#include "nuse_config_check.h"
#include "nuse_codes.h"

/*************************************************************************
*
*   This is the file in which the user configures the ROM data for a Nucleus SE application
*   The items required is largely governed by the settings in nuse_config.h
*
*************************************************************************/


/*** Task Data ***/

/* Task ROM Data */

void MyTask(void);      /* task function prototypes */

ROM ADDR NUSE_Task_Start_Address[NUSE_TASK_NUMBER] =
{
    /* addresses of task entry functions ------ */
    MyTask
};

#if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER

    /* define stack storage - arrays of type ADDR - here */

    ROM ADDR NUSE_Task_Stack_Base[NUSE_TASK_NUMBER] =
    {
        /* addresses of task stacks ------ */
    };

    ROM U16 NUSE_Task_Stack_Size[NUSE_TASK_NUMBER] =
    {
        /* stack sizes ------ */
    };


#endif

#if NUSE_INITIAL_TASK_STATE_SUPPORT || NUSE_INCLUDE_EVERYTHING

    ROM U8 NUSE_Task_Initial_State[NUSE_TASK_NUMBER] =
    {
        /* task states ------ */
        /* may be NUSE_READY or NUSE_PURE_SUSPEND */
        NUSE_READY
    };

#endif

/*** Partition Pool data ***/

#if NUSE_PARTITION_POOL_NUMBER > 0

    /* need to declare/define the partition pool RAM space here - array of type U8 */
    /* size in bytes must be (number or partitions * (partition size + 1)) */

    U8 MyPartitionPool[2];

    /* Partition Pool ROM Data */

    ROM ADDR NUSE_Partition_Pool_Data_Address[NUSE_PARTITION_POOL_NUMBER] =
    {
        /* address of partition pools ------ */
        MyPartitionPool
    };

    ROM U8 NUSE_Partition_Pool_Partition_Number[NUSE_PARTITION_POOL_NUMBER] =
    {
        /* number of partitions in each pool ------ */
        1
    };

    ROM U16 NUSE_Partition_Pool_Partition_Size[NUSE_PARTITION_POOL_NUMBER] =
    {
        /* partition sizes ------ */
        /* in bytes */
        1
    };

#endif

/*** Queue Data ***/

#if NUSE_QUEUE_NUMBER > 0

    /* need to declare/define the queue RAM space here - array of type ADDR */
    /* size in bytes must be (queue size * sizeof(ADDR)) */
    /* i.e. array size is just the queue size */

    ADDR MyQueue[1];

    /* Queue ROM Data */

    ROM ADDR *NUSE_Queue_Data[NUSE_QUEUE_NUMBER] =
    {
        /* addresses of queue data areas ------ */
        MyQueue
    };

    ROM U8 NUSE_Queue_Size[NUSE_QUEUE_NUMBER] =
    {
        /* queue sizes ------ */
        1
    };

#endif

/*** Pipe Data ***/

#if NUSE_PIPE_NUMBER > 0

    /* need to declare/define the pipe RAM space here - array of type U8 */
    /* size in bytes must be (pipe size * pipe message size) */

    U8 MyPipe[1];

    /* Pipe ROM Data */

    ROM U8 *NUSE_Pipe_Data[NUSE_PIPE_NUMBER] =
    {
        /* addresses of pipe data areas ------ */
        MyPipe
    };

    ROM U8 NUSE_Pipe_Size[NUSE_PIPE_NUMBER] =
    {
        /* pipe sizes ------ */
        1
    };

    ROM U8 NUSE_Pipe_Message_Size[NUSE_PIPE_NUMBER] =
    {
        /* pipe message sizes ------ */
        1
    };

#endif

/*** Semaphore Data ***/

#if NUSE_SEMAPHORE_NUMBER > 0

    /* Semaphore ROM Data */

    ROM U8 NUSE_Semaphore_Initial_Value[NUSE_SEMAPHORE_NUMBER] =
    {
        /* semaphore initial count values */
        1
    };

#endif

/*** Timer Data ***/

#if NUSE_TIMER_NUMBER > 0

    /* Timer ROM Data */

    ROM U16 NUSE_Timer_Initial_Time[NUSE_TIMER_NUMBER] =
    {
        /* timer initial times ------ */
        5
    };

    ROM U16 NUSE_Timer_Reschedule_Time[NUSE_TIMER_NUMBER] =
    {
        /* timer reschedule times ------ */
        5
    };

    #if NUSE_TIMER_EXPIRATION_ROUTINE_SUPPORT || NUSE_INCLUDE_EVERYTHING

        /* need prototypes of expiration routines here */

        void MyTimerRoutine(U8);

        ROM ADDR NUSE_Timer_Expiration_Routine_Address[NUSE_TIMER_NUMBER] =
        {
            /* addresses of timer expiration routines ------ */
            /* can be NULL */
            MyTimerRoutine
        };

        ROM U8 NUSE_Timer_Expiration_Routine_Parameter[NUSE_TIMER_NUMBER] =
        {
            /* timer expiration routine parameters ------ */
            0
        };

    #endif

#endif