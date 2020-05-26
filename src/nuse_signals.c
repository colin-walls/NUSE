/*** Signal handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of signals in Nucleus SE:
*
*   NUSE_Signals_Send()
*   NUSE_Signals_Receive()
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
*       NUSE_Signals_Send()
*
*   DESCRIPTION
*
*       Sets one or more signals for the specified task
*       A bit pattern [signals] is ORed into the signal buffer for the receiving task
*
*   INPUTS
*
*       NUSE_TASK task      index of task to which signals are to be sent
*       U8 signals          bit patteren of signals to be set
*
*   RETURNS
*
*       NUSE_SUCCESS        signals sent successfully
*       NUSE_INVALID_TASK   specified task index is invalid
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/


#if NUSE_SIGNALS_SEND || NUSE_INCLUDE_EVERYTHING

    STATUS NUSE_Signals_Send(NUSE_TASK task, U8 signals)
    {
        #if NUSE_API_PARAMETER_CHECKING

            if (task >= NUSE_TASK_NUMBER)
            {
                return NUSE_INVALID_TASK;
            }

        #endif

        NUSE_CS_Enter();
        NUSE_Task_Signal_Flags[task] |= signals;
        NUSE_CS_Exit();

        return NUSE_SUCCESS;
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Signals_Receive()
*
*   DESCRIPTION
*
*       Obtains [destructive read] the signals for the current task
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 value        signals value
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_SIGNALS_RECEIVE || NUSE_INCLUDE_EVERYTHING

    U8 NUSE_Signals_Receive(void)
    {
        U8 signals;

        NUSE_CS_Enter();
        signals = NUSE_Task_Signal_Flags[NUSE_Task_Active];
        NUSE_Task_Signal_Flags[NUSE_Task_Active] = 0;
        NUSE_CS_Exit();

        return signals;
    }

#endif
