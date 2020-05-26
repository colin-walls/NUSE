#include "nuse_config.h"
#include "nuse_codes.h"
#include "nuse_types.h"
#include "nuse_data.h"
#include "nuse_prototypes.h"


/*************************************************************************
*
*   FUNCTION
*
*       NUSE_Release_Information()
*
*   DESCRIPTION
*
*       Returns a pointer to the Nucleus SE version string
*       [set in nuse_globals.c]
*
*   INPUTS
*
*       (none)
*
*   RETURNS
*
*       U8 * value      pointer to the Nucleus SE version string
*
*   OTHER OUTPUTS
*
*       (none)
*
*************************************************************************/

#if NUSE_RELEASE_INFORMATION || NUSE_INCLUDE_EVERYTHING

    char *NUSE_Release_Information(void)
    {
        return NUSE_Release_Info;
    }

#endif