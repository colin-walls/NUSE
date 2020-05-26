/*** Task control APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of tasks in Nucleus SE:
*
*   NUSE_Task_Suspend()
*   NUSE_Task_Resume()
*   NUSE_Task_Sleep()
*   NUSE_Task_Reliquish()
*   NUSE_Task_Current()
*   NUSE_Task_Check_Stack()
*   NUSE_Task_Reset()
*   NUSE_Task_Information()
*   NUSE_Task_Count()
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
*       NUSE_Task_Suspend()
*
*   DESCRIPTION
*
*       Specified task is placed into unconditional suspend
*       If task is current, next available task is given control
*
*   INPUTS
*
*       NUSE_TASK task      index of task to be suspended
*
*   RETURNS
*
*       NUSE_SUCCESS        the task was successfully suspended
*       NUSE_INVALID_TASK   the task index was not valid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TASK_SUSPEND || NUSE_INCLUDE_EVERYTHING

    STATUS NUSE_Task_Suspend(NUSE_TASK task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (task >= NUSE_TASK_NUMBER)
            {
                return NUSE_INVALID_TASK;
            }
        #endif

        NUSE_CS_Enter();

        NUSE_Suspend_Task(task, NUSE_PURE_SUSPEND);

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Resume()
*
*   DESCRIPTION
*
*       Wakes up a task that has previously been unconditionally suspended
*
*   INPUTS
*
*       NUSE_TASK task      index of task to be resumed
*
*   RETURNS
*
*       NUSE_SUCCESS        the task was successfully resumed
*       NUSE_INVALID_TASK   the task index was not valid
*       NUSE_INVALID_RESUME the task was not unconditionally suspended
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TASK_RESUME || NUSE_INCLUDE_EVERYTHING

    STATUS NUSE_Task_Resume(NUSE_TASK task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (task >= NUSE_TASK_NUMBER)
            {
                return NUSE_INVALID_TASK;
            }

            if (NUSE_Task_Status[task] != NUSE_PURE_SUSPEND)
            {
                return NUSE_INVALID_RESUME;
            }
        #endif

        NUSE_CS_Enter();

        #if NUSE_TIMEOUT_SUPPORT
            NUSE_Task_Timeout_Counter[task] = 0;
        #endif
        NUSE_Wake_Task(task);

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Sleep()
*
*   DESCRIPTION
*
*       Current task is placed into sleep-suspend
*       Next available task is given control
*
*   INPUTS
*
*       U16 ticks       the duration for which the task should be suspended
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

#if NUSE_TASK_SLEEP || NUSE_INCLUDE_EVERYTHING

    void NUSE_Task_Sleep(U16 ticks)
    {
        NUSE_CS_Enter();

        NUSE_Task_Timeout_Counter[NUSE_Task_Active] = ticks;
        NUSE_Suspend_Task(NUSE_Task_Active, NUSE_SLEEP_SUSPEND);

        NUSE_CS_Exit();
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Reliquish()
*
*   DESCRIPTION
*
*       Transfers control to the next available task
*       Only makes sense for Round Robin and Time Slice schedulers,
*       as Nucleus SE only allows one task at each priority
*       nuse.h takes care of removing code for irrelevant schedulers
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

#if NUSE_TASK_RELINQUISH || NUSE_INCLUDE_EVERYTHING

    void NUSE_Task_Relinquish(void)
    {
        NUSE_CS_Enter();

        NUSE_Reschedule();  /* next ready task in round robin will be scheduled */

        NUSE_CS_Exit();
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Current()
*
*   DESCRIPTION
*
*       Returns the index of the current task,
*       which is of type NUSE_TASK [not a pointer, as in Nucleus PLUS]
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       NUSE_TASK value index of current task [value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TASK_CURRENT || NUSE_INCLUDE_EVERYTHING

    NUSE_TASK NUSE_Task_Current(void)
    {
        return NUSE_Task_Active;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Check_Stack()
*
*   DESCRIPTION
*
*       Returns the available stack space for the current task
*       Only makes sense for schedulers where each task has its own stack - i.e. not Run To Completion
*
*   INPUTS
*
*       U8 dummy        dummy parameter, provided by nuse.h, to find the top of the stack
*
*   RETURNS
*
*       U16 value       stack size [or 0 if RTC scheduler]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TASK_CHECK_STACK || NUSE_INCLUDE_EVERYTHING

    U16 NUSE_Task_Check_Stack(U8 dummy)
    {
        #if NUSE_SCHEDULER_TYPE == NUSE_RUN_TO_COMPLETION_SCHEDULER
            return 0;
        #else
            U8 *sp;

            sp = &dummy - 1;
            return (((U8 *)NUSE_Task_Stack_Base[NUSE_Task_Active] - sp) * sizeof(unsigned));
        #endif
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Reset()
*
*   DESCRIPTION
*
*       Restores the specified task to its initialized state,
*       except it is always left suspended
*       Uses the same initialization routine as on start-up - in nuse_init.c
*
*   INPUTS
*
*       NUSE_TASK task      index of task to be reset
*
*   RETURNS
*
*       NUSE_SUCCESS        the task was successfully reset
*       NUSE_INVALID_TASK   the task index was not valid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TASK_RESET || NUSE_INCLUDE_EVERYTHING

    STATUS NUSE_Task_Reset(NUSE_TASK task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (task >= NUSE_TASK_NUMBER)
            {
                return NUSE_INVALID_TASK;
            }
        #endif

        NUSE_CS_Enter();

        #if NUSE_BLOCKING_ENABLE                        /* if task was blocked on API call */
            switch (LONIB(NUSE_Task_Status[task]))      /* need to update kernel object blocking info */
            {
                case NUSE_MAILBOX_SUSPEND:
                    NUSE_Mailbox_Blocking_Count[HINIB(NUSE_Task_Status[task])]--;
                    break;
                case NUSE_SEMAPHORE_SUSPEND:
                    NUSE_Semaphore_Blocking_Count[HINIB(NUSE_Task_Status[task])]--;
                    break;
                case NUSE_PARTITION_SUSPEND:
                    NUSE_Partition_Pool_Blocking_Count[HINIB(NUSE_Task_Status[task])]--;
                    break;
                case NUSE_QUEUE_SUSPEND:
                    NUSE_Queue_Blocking_Count[HINIB(NUSE_Task_Status[task])]--;
                    break;
                case NUSE_PIPE_SUSPEND:
                    NUSE_Pipe_Blocking_Count[HINIB(NUSE_Task_Status[task])]--;
                    break;
                case NUSE_EVENT_SUSPEND:
                    NUSE_Event_Group_Blocking_Count[HINIB(NUSE_Task_Status[task])]--;
                    break;
            }
        #endif

        NUSE_Init_Task(task);
        NUSE_Suspend_Task(task, NUSE_PURE_SUSPEND);

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a task
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_TASK task          index of task about which information is required
*       U8 *task_status         pointer to storage for task status
*       U16 *scheduled_count    pointer to storage for schedule count
*       ADDR *stack_base        pointer to storage for stack base address
*       U16 *stack_size         pointer to storage for stack size
*
*   RETURNS
*
*       NUSE_SUCCESS            the task status was successfully returned
*       NUSE_INVALID_TASK       the task index was not valid
*       NUSE_INVALID_POINTER    one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       U8 *task_status         task status value [if task suspend is enabled]
*                               values: NUSE_READY, NUSE_PURE_SUSPEND, NUSE_SLEEP_SUSPEND
*       U16 *scheduled_count    schedule count since initialization or task reset
*                               [if schedule counting is enabled]
*       ADDR *stack_base        stack base address [if not RTC scheduler]
*       U16 *stack_size         stack size [if not RTC scheduler]
*
*************************************************************************/

#if NUSE_TASK_INFORMATION || NUSE_INCLUDE_EVERYTHING

    STATUS NUSE_Task_Information(NUSE_TASK task, U8 *task_status, U16 *scheduled_count, ADDR *stack_base, U16 *stack_size)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (task >= NUSE_TASK_NUMBER)
            {
                return NUSE_INVALID_TASK;
            }

            if ((task_status == NULL) || (scheduled_count == NULL) || (stack_base == NULL)|| (stack_size == NULL))
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        NUSE_CS_Enter();

        #if NUSE_SUSPEND_ENABLE || NUSE_INCLUDE_EVERYTHING
            *task_status = LONIB(NUSE_Task_Status[task]);
        #endif

        #if NUSE_SCHEDULE_COUNT_SUPPORT || NUSE_INCLUDE_EVERYTHING
            *scheduled_count = NUSE_Task_Schedule_Count[task];
        #endif

        #if NUSE_SCHEDULER_TYPE != NUSE_RUN_TO_COMPLETION_SCHEDULER
            *stack_base = NUSE_Task_Stack_Base[task];
            *stack_size = NUSE_Task_Stack_Size[task];
        #endif

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Task_Count()
*
*   DESCRIPTION
*
*       Returns the number of tasks in the system
*       Code is trivial, as the number of tasks is fixed in Nucleus SE
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of tasks in the system [value: 1-16]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TASK_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Task_Count(void)
    {
        return NUSE_TASK_NUMBER;
    }

#endif
