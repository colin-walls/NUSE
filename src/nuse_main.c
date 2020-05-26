
#include "nuse_config.h"
#include "nuse_codes.h"
#include "nuse_types.h"
#include "nuse_data.h"
#include "nuse_prototypes.h"

/* Code entry point */

void main(void)
{
    NUSE_Init();        /* initialize kernel data */

    /* user initialization code here */

    NUSE_Scheduler();   /* start tasks */
}
