/*************************************************************************
*
*   This is the primary configuration file for a Nucleus SE application
*   Here you can select:
*       How many of each object type are configured
*       Which API calls are available
*       Which scheduler type is used
*       A selection of other optional kernel facilities
*
*************************************************************************/


/* Check to see if the file has been included already.  */
#ifndef _NUSE_CONFIG_H_
#define _NUSE_CONFIG_H_


/*** API calls ***/

#define NUSE_API_PARAMETER_CHECKING     TRUE    /* option enabler */

/*** Tasks and task control ***/

#define NUSE_TASK_NUMBER        1               /* Number of tasks in the system - 1-16 */

#define NUSE_TASK_SUSPEND       TRUE            /* Service call enabler */
#define NUSE_TASK_RESUME        TRUE            /* Service call enabler */
#define NUSE_TASK_SLEEP         TRUE            /* Service call enabler */
#define NUSE_TASK_RELINQUISH    TRUE            /* Service call enabler */
#define NUSE_TASK_CURRENT       TRUE            /* Service call enabler */
#define NUSE_TASK_CHECK_STACK   TRUE            /* Service call enabler */
#define NUSE_TASK_RESET         TRUE            /* Service call enabler */
#define NUSE_TASK_INFORMATION   TRUE            /* Service call enabler */
#define NUSE_TASK_COUNT         TRUE            /* Service call enabler */

#define NUSE_INITIAL_TASK_STATE_SUPPORT TRUE    /* Enables support for tasks starting as suspended */

/*** Partition pools ***/

#define NUSE_PARTITION_POOL_NUMBER      1       /* Number of partition pools in the system - 0-16 */

#define NUSE_PARTITION_ALLOCATE         TRUE    /* Service call enabler */
#define NUSE_PARTITION_DEALLOCATE       TRUE    /* Service call enabler */
#define NUSE_PARTITION_POOL_INFORMATION TRUE    /* Service call enabler */
#define NUSE_PARTITION_POOL_COUNT       TRUE    /* Service call enabler */

/*** Mailboxes ***/

#define NUSE_MAILBOX_NUMBER         1           /* Number of mailboxes in the system - 0-16 */

#define NUSE_MAILBOX_SEND           TRUE        /* Service call enabler */
#define NUSE_MAILBOX_RECEIVE        TRUE        /* Service call enabler */
#define NUSE_MAILBOX_RESET          TRUE        /* Service call enabler */
#define NUSE_MAILBOX_INFORMATION    TRUE        /* Service call enabler */
#define NUSE_MAILBOX_COUNT          TRUE        /* Service call enabler */

/*** Queues ***/

#define NUSE_QUEUE_NUMBER       1               /* Number of queues in the system - 0-16 */

#define NUSE_QUEUE_SEND         TRUE            /* Service call enabler */
#define NUSE_QUEUE_RECEIVE      TRUE            /* Service call enabler */
#define NUSE_QUEUE_JAM          TRUE            /* Service call enabler */
#define NUSE_QUEUE_RESET        TRUE            /* Service call enabler */
#define NUSE_QUEUE_INFORMATION  TRUE            /* Service call enabler */
#define NUSE_QUEUE_COUNT        TRUE            /* Service call enabler */

/*** Pipes ***/

#define NUSE_PIPE_NUMBER        1               /* Number of pipes in the system - 0-16 */

#define NUSE_PIPE_SEND          TRUE            /* Service call enabler */
#define NUSE_PIPE_RECEIVE       TRUE            /* Service call enabler */
#define NUSE_PIPE_JAM           TRUE            /* Service call enabler */
#define NUSE_PIPE_RESET         TRUE            /* Service call enabler */
#define NUSE_PIPE_INFORMATION   TRUE            /* Service call enabler */
#define NUSE_PIPE_COUNT         TRUE            /* Service call enabler */

/*** Semaphores ***/

#define NUSE_SEMAPHORE_NUMBER       1           /* Number of semaphores in the system - 0-16 */

#define NUSE_SEMAPHORE_OBTAIN       TRUE        /* Service call enabler */
#define NUSE_SEMAPHORE_RELEASE      TRUE        /* Service call enabler */
#define NUSE_SEMAPHORE_RESET        TRUE        /* Service call enabler */
#define NUSE_SEMAPHORE_INFORMATION  TRUE        /* Service call enabler */
#define NUSE_SEMAPHORE_COUNT        TRUE        /* Service call enabler */

/*** Event groups ***/

#define NUSE_EVENT_GROUP_NUMBER         1       /* Number of event groups in the system - 0-16 */

#define NUSE_EVENT_GROUP_SET            TRUE    /* Service call enabler */
#define NUSE_EVENT_GROUP_RETRIEVE       TRUE    /* Service call enabler */
#define NUSE_EVENT_GROUP_INFORMATION    TRUE    /* Service call enabler */
#define NUSE_EVENT_GROUP_COUNT          TRUE    /* Service call enabler */

/*** Signals ***/

#define NUSE_SIGNAL_SUPPORT     TRUE            /* Enables support for signals */

#define NUSE_SIGNALS_SEND       TRUE            /* Service call enabler */
#define NUSE_SIGNALS_RECEIVE    TRUE            /* Service call enabler */

/*** Timers ***/

#define NUSE_TIMER_NUMBER           1           /* Number of application timers in the system - 0-16 */

#define NUSE_TIMER_CONTROL          TRUE        /* Service call enabler */
#define NUSE_TIMER_GET_REMAINING    TRUE        /* Service call enabler */
#define NUSE_TIMER_RESET            TRUE        /* Service call enabler */
#define NUSE_TIMER_INFORMATION      TRUE        /* Service call enabler */
#define NUSE_TIMER_COUNT            TRUE        /* Service call enabler */

#define NUSE_TIMER_EXPIRATION_ROUTINE_SUPPORT TRUE  /* Enables execution of timer expiration function */

/*** System time ***/

#define NUSE_SYSTEM_TIME_SUPPORT    TRUE        /* Enables the system tick clock */

#define NUSE_CLOCK_SET              TRUE        /* Service call enabler */
#define NUSE_CLOCK_RETRIEVE         TRUE        /* Service call enabler */

/*** System Diagnostics ***/

#define NUSE_RELEASE_INFORMATION    TRUE        /* Service call enabler */

/*** Scheduler and task suspend ***/

#define NUSE_RUN_TO_COMPLETION_SCHEDULER    1   /* Scheduler type option */
#define NUSE_TIME_SLICE_SCHEDULER           2   /* Scheduler type option */
#define NUSE_ROUND_ROBIN_SCHEDULER          3   /* Scheduler type option */
#define NUSE_PRIORITY_SCHEDULER             4   /* Scheduler type option */
                                                /* Scheduler type selection: */
#define NUSE_SCHEDULER_TYPE     NUSE_ROUND_ROBIN_SCHEDULER

#define NUSE_TIME_SLICE_TICKS 0                 /* interval for time slice scheduler */

#define NUSE_SCHEDULE_COUNT_SUPPORT TRUE        /* Enables support for schedule counting of each task */

#define NUSE_SUSPEND_ENABLE         TRUE        /* Enables support for task suspend */
#define NUSE_BLOCKING_ENABLE        TRUE        /* Enables blocking API calls */

#define NUSE_INCLUDE_EVERYTHING     TRUE        /* enable all facilities and APIs - for debug/porting only */

#endif  /* ! _NUSE_CONFIG_H_ */

