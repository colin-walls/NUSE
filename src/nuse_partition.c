/*** Partition pool handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of partition pools in Nucleus SE:
*
*   NUSE_Partition_Allocate()
*   NUSE_Partition_Deallocate()
*   NUSE_Partition_Pool_Information()
*   NUSE_Partition_Pool_Count()
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
*       NUSE_Partition_Allocate()
*
*   DESCRIPTION
*
*       Allocates a partition within the specified pool
*
*   INPUTS
*
*       NUSE_PARTITION_POOL pool    index of pool from which alocation is required
*       ADDR *return_pointer        pointer to location for partition address
*       U8 suspend                  task suspend option - must be NUSE_NO_SUSPEND or NUSE_SUSPEND
*                                   depending upon the setting of NUSE_BLOCKING_ENABLE
*
*   RETURNS
*
*       NUSE_SUCCESS                partition successfully allocated
*       NUSE_NO_PARTITION           no partition is available
*       NUSE_INVALID_POOL           specified pool index is invalid
*       NUSE_INVALID_POINTER        data return pointer [return_pointer] is NULL
*       NUSE_INVALID_SUSPEND        task suspend option is not set to NUSE_NO_SUSPEND or NUSE_SUSPEND
*
*   OTHER OUTPUTS
*
*       ADDR *return_pointer        pointer to allocated partition data area
*
*************************************************************************/

#if NUSE_PARTITION_ALLOCATE || (NUSE_INCLUDE_EVERYTHING && (NUSE_PARTITION_POOL_NUMBER != 0))

    STATUS NUSE_Partition_Allocate(NUSE_PARTITION_POOL pool, ADDR *return_pointer, U8 suspend)
    {
        STATUS return_value;
        U8 *ptr;

        #if NUSE_API_PARAMETER_CHECKING
            if (pool >= NUSE_PARTITION_POOL_NUMBER)
            {
                return NUSE_INVALID_POOL;
            }

            if (return_pointer == NULL)
            {
                return NUSE_INVALID_POINTER;
            }

            #if NUSE_BLOCKING_ENABLE
                if ((suspend != NUSE_NO_SUSPEND) && (suspend != NUSE_SUSPEND))
                {
                    return NUSE_INVALID_SUSPEND;
                }
            #else
                if (suspend != NUSE_NO_SUSPEND)
                {
                    return NUSE_INVALID_SUSPEND;
                }
            #endif
        #endif

        NUSE_CS_Enter();

        #if NUSE_BLOCKING_ENABLE
            do
            {
                if (NUSE_Partition_Pool_Partition_Number[pool] == NUSE_Partition_Pool_Partition_Used[pool])
                {
                    if (suspend == NUSE_NO_SUSPEND)
                    {
                        return_value = NUSE_NO_PARTITION;       /* no free partitions */
                    }
                    else
                    {                                           /* block task */
                        NUSE_Partition_Pool_Blocking_Count[pool]++;
                        NUSE_Suspend_Task(NUSE_Task_Active, (pool << 4) | NUSE_PARTITION_SUSPEND);
                        return_value = NUSE_Task_Blocking_Return[NUSE_Task_Active];
                        if (return_value != NUSE_SUCCESS)
                        {
                            suspend = NUSE_NO_SUSPEND;
                        }
                    }
                }
                else
                {
                    ptr = (U8 *)NUSE_Partition_Pool_Data_Address[pool];         /* point to status byte of 1st partition */
                    while ( *ptr != 0)                                          /* find an unused partition */
                    {
                        ptr += NUSE_Partition_Pool_Partition_Size[pool] + 1;
                    }
                    *ptr = 0x80 | pool;                                         /* mark used */
                                                                                /* need pool number for deallocation */
                    NUSE_Partition_Pool_Partition_Used[pool]++;
                    *return_pointer = ptr + 1;
                    return_value = NUSE_SUCCESS;
                    suspend = NUSE_NO_SUSPEND;
                }
            } while (suspend == NUSE_SUSPEND);
        #else
            if (NUSE_Partition_Pool_Partition_Number[pool] == NUSE_Partition_Pool_Partition_Used[pool])
            {
                return_value = NUSE_NO_PARTITION;       /* no free partitions */
            }
            else
            {
                ptr = (U8 *)NUSE_Partition_Pool_Data_Address[pool];         /* point to status byte of 1st partition */
                while ( *ptr != 0)                                          /* find an unused partition */
                {
                    ptr += NUSE_Partition_Pool_Partition_Size[pool] + 1;
                }
                *ptr = 0x80 | pool;                                         /* mark used */
                                                                            /* need pool number for deallocation */
                NUSE_Partition_Pool_Partition_Used[pool]++;
                *return_pointer = ptr + 1;
                return_value = NUSE_SUCCESS;
            }
        #endif

        NUSE_CS_Exit();

        return return_value;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Partition_Deallocate()
*
*   DESCRIPTION
*
*       Frees the partition pointed to by the supplied pointer
*
*   INPUTS
*
*       ADDR partition      pointer to [address of] partition to be deallocated
*
*   RETURNS
*
*       NUSE_SUCCESS            partition was successfully deallocated
*       NUSE_INVALID_POINTER    supplied pointer is NULL or does not appear to point to a valid partition
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_PARTITION_DEALLOCATE || (NUSE_INCLUDE_EVERYTHING && (NUSE_PARTITION_POOL_NUMBER != 0))

    STATUS NUSE_Partition_Deallocate(ADDR partition)
    {
        U8 pool;
        STATUS return_value;

        #if NUSE_API_PARAMETER_CHECKING
            if (partition == NULL)
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        NUSE_CS_Enter();

        if ((*((U8*)partition-1) & 0xf0) == 0x80)       /* check it is a used partition */
        {
            pool = *((U8*)partition-1) & 0x0f;              /* extract pool index number */
            *((U8*)partition-1) = 0;                        /* mark unused */
            NUSE_Partition_Pool_Partition_Used[pool]--;     /* decrement count */
            return_value = NUSE_SUCCESS;

            #if NUSE_BLOCKING_ENABLE

                if (NUSE_Partition_Pool_Blocking_Count[pool] != 0)
                {
                    U8 index;                       /* check whether a task is blocked */
                                                    /* on this partition pool */

                    NUSE_Partition_Pool_Blocking_Count[pool]--;
                    for (index=0; index<NUSE_TASK_NUMBER; index++)
                    {
                        if ((LONIB(NUSE_Task_Status[index]) == NUSE_PARTITION_SUSPEND)
                            && (HINIB(NUSE_Task_Status[index]) == pool))
                        {
                            NUSE_Task_Blocking_Return[index] = NUSE_SUCCESS;
                            NUSE_Wake_Task(index);
                            break;
                        }
                    }
                }

            #endif
        }
        else
        {
            return_value = NUSE_INVALID_POINTER;
        }

        NUSE_CS_Exit();

        return return_value;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Partition_Pool_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a partition pool
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_PARTITION_POOL pool    index of pool about which information is required
*       ADDR *start_address         pointer to storage for pool start address
*       U32 *pool_size              pointer to storage for size of pool
*       U16 *partition_size         pointer to storage for size of partition
*       U8 *available               pointer to storage for number of available partitions
*       U8 *allocated               pointer to storage for number of partitions in use
*       U8 *tasks_waiting           pointer to storage for number of waiting tasks [not used]
*       NUSE_TASK *first_task       pointer to storage for pointer to first waiting task [not used]
*
*   RETURNS
*
*       NUSE_SUCCESS                the pool status was successfully returned
*       NUSE_INVALID_POOL           the pool index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       ADDR *start_address         pool start address
*       U32 *pool_size              size of pool [in bytes]
*       U16 *partition_size         size of partition [in bytes]
*       U8 *available               number of available partitions
*       U8 *allocated               number of partitions in use
*       U8 *tasks_waiting           number of waiting tasks  [if used, otherwise 0]
*       NUSE_TASK *first_task       first waiting task  [if used, otherwise 0]
*
*************************************************************************/

#if NUSE_PARTITION_POOL_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_PARTITION_POOL_NUMBER != 0))

    STATUS NUSE_Partition_Pool_Information(NUSE_PARTITION_POOL pool, ADDR *start_address, U32 *pool_size, U16 *partition_size, U8 *available, U8 *allocated, U8 *tasks_waiting, NUSE_TASK *first_task)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (pool >= NUSE_PARTITION_POOL_NUMBER)
            {
                return NUSE_INVALID_POOL;
            }

            if ((start_address == NULL) || (pool_size == NULL) || (partition_size == NULL) || (available == NULL) || (allocated == NULL) || (tasks_waiting == NULL) || (first_task == NULL))
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        NUSE_CS_Enter();

        *start_address = NUSE_Partition_Pool_Data_Address[pool];
        *pool_size = NUSE_Partition_Pool_Partition_Number[pool] * NUSE_Partition_Pool_Partition_Size[pool];
        *partition_size = NUSE_Partition_Pool_Partition_Size[pool];
        *available = NUSE_Partition_Pool_Partition_Number[pool] - NUSE_Partition_Pool_Partition_Used[pool];
        *allocated = NUSE_Partition_Pool_Partition_Used[pool];

        #if NUSE_BLOCKING_ENABLE

            *tasks_waiting = NUSE_Partition_Pool_Blocking_Count[pool];
            if (NUSE_Partition_Pool_Blocking_Count[pool] != 0)
            {
                U8 index;

                for (index=0; index<NUSE_TASK_NUMBER; index++)
                {
                    if ((LONIB(NUSE_Task_Status[index]) == NUSE_PARTITION_SUSPEND)
                        && (HINIB(NUSE_Task_Status[index]) == pool))
                    {
                        *first_task = index;
                        break;
                    }
                }
            }
            else
            {
                *first_task = 0;
            }

        #else

            *tasks_waiting = 0;
            *first_task = 0;

        #endif

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Partition_Pool_Count()
*
*   DESCRIPTION
*
*       Returns the number of partition pools in the system
*       Code is trivial, as the number of partition pools is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of partition pools in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_PARTITION_POOL_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Partition_Pool_Count(void)
    {
        return NUSE_PARTITION_POOL_NUMBER;
    }

#endif
