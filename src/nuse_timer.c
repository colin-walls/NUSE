/*** Timer handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of timers in Nucleus SE:
*
*   NUSE_Timer_Control()
*   NUSE_Timer_Get_Remaining()
*   NUSE_Timer_Reset()
*   NUSE_Timer_Information()
*   NUSE_Timer_Count()
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
*       NUSE_Timer_Control()
*
*   DESCRIPTION
*
*       Timer can be enabled or disabled
*       When enabled, the timer is set to the initial time or the reschedule time
*       Initial time is used if the timer has not expired since it was initialized or last reset
*       Reschedule time is used if the timer has previously expired
*       Disabling sets the timer count to 0
*
*   INPUTS
*
*       NUSE_TIMER timer    index of the timer to be enabled/disabled
*       OPTION enable       required function: NUSE_ENABLE_TIMER or NUSE_DISABLE_TIMER
*
*   RETURNS
*
*       NUSE_SUCCESS        the timer was successfully enabled/disabled
*       NUSE_INVALID_TIMER  the timer index was invalid
*       NUSE_INVALID_ENABLE the function selection [enable] was invalid; i.e. not NUSE_ENABLE_TIMER or NUSE_DISABLE_TIMER
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TIMER_CONTROL || (NUSE_INCLUDE_EVERYTHING && (NUSE_TIMER_NUMBER != 0))

    STATUS NUSE_Timer_Control(NUSE_TIMER timer, OPTION enable)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (timer >= NUSE_TIMER_NUMBER)
            {
                return NUSE_INVALID_TIMER;
            }

            if ((enable != NUSE_ENABLE_TIMER) && (enable != NUSE_DISABLE_TIMER))
            {
                return NUSE_INVALID_ENABLE;
            }
        #endif

        NUSE_CS_Enter();

        if (enable == NUSE_ENABLE_TIMER)
        {
            NUSE_Timer_Status[timer] = TRUE;
            if (NUSE_Timer_Expirations_Counter[timer] == 0)
            {
                NUSE_Timer_Value[timer] = NUSE_Timer_Initial_Time[timer];
            }
            else
            {
                NUSE_Timer_Value[timer] = NUSE_Timer_Reschedule_Time[timer];
            }

        }
        else  /* enable == NUSE_DISABLE_TIMER */
        {
            NUSE_Timer_Status[timer] = FALSE;
        }

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Timer_Get_Remaining()
*
*   DESCRIPTION
*
*       Returns the remaining number of ticks before the timer will expire
*
*   INPUTS
*
*       NUSE_TIMER timer            index of the timer to be queries
*       U16 *remaining_time         pointer to storage for the timer remaining time
*
*   RETURNS
*
*       NUSE_SUCCESS                the remaining time was retuned successfully
*       NUSE_INVALID_TIMER          the timer index was invalid
*       NUSE_INVALID_POINTER        the return data pointer [remaining time] was NULL
*
*   OTHER OUTPUTS
*
*       U16 *remaining_time         remaining time value
*
*************************************************************************/

#if NUSE_TIMER_GET_REMAINING || (NUSE_INCLUDE_EVERYTHING && (NUSE_TIMER_NUMBER != 0))

    STATUS NUSE_Timer_Get_Remaining(NUSE_TIMER timer, U16 *remaining_time)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (timer >= NUSE_TIMER_NUMBER)
            {
                return NUSE_INVALID_TIMER;
            }

            if (remaining_time == NULL)
            {
                return NUSE_INVALID_POINTER;
            }
        #endif

        NUSE_CS_Enter();

        *remaining_time = NUSE_Timer_Value[timer];

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Timer_Reset()
*
*   DESCRIPTION
*
*       Restores the specified timer to its initialized state
*       Uses the same initialization routine as on start-up - in nuse_init.c
*       Then, the timer is enabled, if required
*
*   INPUTS
*
*       NUSE_TIMER timer    the index of the timer to be reset
*       OPTION enable       the required state of the timer after reset; NUSE_ENABLE_TIMER or NUSE_DISABLE_TIMER
*
*   RETURNS
*
*       NUSE_SUCCESS        the timer was successfully reset
*       NUSE_INVALID_TIMER  the timer index was invalid
*       NUSE_INVALID_ENABLE the state selection [enable] was invalid; i.e. not NUSE_ENABLE_TIMER or NUSE_DISABLE_TIMER
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TIMER_RESET || (NUSE_INCLUDE_EVERYTHING && (NUSE_TIMER_NUMBER != 0))

    STATUS NUSE_Timer_Reset(NUSE_TIMER timer, OPTION enable)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (timer >= NUSE_TIMER_NUMBER)
            {
                return NUSE_INVALID_TIMER;
            }

            if ((enable != NUSE_ENABLE_TIMER) && (enable != NUSE_DISABLE_TIMER))
            {
                return NUSE_INVALID_ENABLE;
            }
        #endif

        if (NUSE_Timer_Status[timer])
        {
            return NUSE_NOT_DISABLED;
        }

        NUSE_CS_Enter();

        NUSE_Init_Timer(timer);

        if (enable == NUSE_ENABLE_TIMER)
        {
            NUSE_Timer_Status[timer] = TRUE;
        }

        /* else enable==NUSE_DISABLE_TIMER and status remains FALSE */

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif

/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Timer_Information()
*
*   DESCRIPTION
*
*       Returns a number of items of data about a timer
*       Unsupported items are handled in nuse.h
*
*   INPUTS
*
*       NUSE_TIMER timer            index of timer about which information is required
*       OPTION *enable              pointer to storage for the timer status
*       U8 *expirations             pointer to storage for the expirations count
*       U8 *id                      pointer to storage for the expiration function parameter
*       U16 *initial_time           pointer to storage for the initial time setting
*       U16 *reschedule_time        pointer to storage for the reschedule time setting
*
*   RETURNS
*
*       NUSE_SUCCESS                the information was successfully returned
*       NUSE_INVALID_TIMER          the timer index was not valid
*       NUSE_INVALID_POINTER        one or more of the pointer parameters was NULL
*
*   OTHER OUTPUTS
*
*       OPTION *enable              timer status; NUSE_ENABLE_TIMER or NUSE_DISABLE_TIMER
*       U8 *expirations             expirations count - the number of times the timer has expired since initialization or reset
*       U8 *id                      expiration function parameter [if this facility is configured]
*       U16 *initial_time           initial time setting
*       U16 *reschedule_time        reschedule time setting
*
*************************************************************************/

#if NUSE_TIMER_INFORMATION || (NUSE_INCLUDE_EVERYTHING && (NUSE_TIMER_NUMBER != 0))

    STATUS NUSE_Timer_Information(NUSE_TIMER timer, OPTION *enable, U8 *expirations, U8 *id, U16 *initial_time, U16 *reschedule_time)
    {
        #if NUSE_API_PARAMETER_CHECKING
            if (timer >= NUSE_TIMER_NUMBER)
            {
                return NUSE_INVALID_TIMER;
            }
        #endif

        NUSE_CS_Enter();

        if (NUSE_Timer_Status[timer])
        {
            *enable = NUSE_ENABLE_TIMER;
        }
        else
        {
            *enable = NUSE_DISABLE_TIMER;
        }

        *expirations = NUSE_Timer_Expirations_Counter[timer];

        #if NUSE_TIMER_EXPIRATION_ROUTINE_SUPPORT
            *id = NUSE_Timer_Expiration_Routine_Parameter[timer];
        #endif

        *initial_time = NUSE_Timer_Initial_Time[timer];
        *reschedule_time = NUSE_Timer_Reschedule_Time[timer];

        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Timer_Count()
*
*   DESCRIPTION
*
*       Returns the number of timers in the system
*       Code is trivial, as the number of timers is fixed in Nucleus SE,
*       so a constant is returned
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        number of timers in the system [Value: 0-15]
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_TIMER_COUNT || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Timer_Count(void)
    {
        return NUSE_TIMER_NUMBER;
    }

#endif
