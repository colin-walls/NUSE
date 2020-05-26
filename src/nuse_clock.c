/*** System Clock handling APIs ***/

/************************************************************************
*
*   This file contains the definitions of functions concerned with the
*   handling of the system clock in Nucleus SE:
*
*   NUSE_Clock_Set()
*   NUSE_Clock_Retrieve()
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
*       NUSE_Clock_Set()
*
*   DESCRIPTION
*
*       Sets the value of the system [tick] clock
*
*   INPUTS
*
*       U32 new_value       required system clock value
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

#if NUSE_CLOCK_SET || NUSE_INCLUDE_EVERYTHING

    void NUSE_Clock_Set(U32 new_value)
    {
        NUSE_CS_Enter();
        NUSE_Tick_Clock = new_value;
        NUSE_CS_Exit();
    }

#endif


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Clock_Retrieve()
*
*   DESCRIPTION
*
*       Returns the current value of the system [tick] clock
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U32 value       system clock value
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_CLOCK_RETRIEVE || NUSE_INCLUDE_EVERYTHING

    U32 NUSE_Clock_Retrieve(void)
    {
        U32 time;

        NUSE_CS_Enter();
        time = NUSE_Tick_Clock;
        NUSE_CS_Exit();

        return time;
    }

#endif
