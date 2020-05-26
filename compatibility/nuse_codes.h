/*************************************************************************
*
*   This file contains symbolic constants for Nucleus SE service call parameters
*   and return codes
*   The "NU_" values are compatible with Nucleus PLUS
*   Not all codes are currently uitilized by Nucleus SE
*
*************************************************************************/


/* Check to see if the file has been included already.  */
#ifndef _NUSE_CODES_H_
#define _NUSE_CODES_H_

#define NU_SUCCESS (0)
#define NU_TRUE TRUE
#define NU_FALSE FALSE
#define NU_NO_SUSPEND (0)
#define NU_SUSPEND (0xff)
#define NU_INVALID_SUSPEND (-24)
#define NU_FIFO (6)
#define NU_PRIORITY (11)

#define NU_INVALID_TASK (-25)
#define NU_NOT_TERMINATED (-38)
#define NU_INVALID_RESUME (-20)

#define NU_READY (0)
#define NU_PURE_SUSPEND (1)
#define NU_FINISHED (11)
#define NU_TERMINATED (12)
#define NU_SLEEP_SUSPEND (2)
#define NU_MAILBOX_SUSPEND (3)
#define NU_QUEUE_SUSPEND (4)
#define NU_PIPE_SUSPEND (5)
#define NU_EVENT_SUSPEND (7)
#define NU_SEMAPHORE_SUSPEND (6)
#define NU_PARTITION_SUSPEND (8)

#define NU_INVALID_POOL (-16)
#define NU_INVALID_POINTER (-15)
#define NU_NO_PARTITION (-34)
#define NU_TIMEOUT (-50)

#define NU_INVALID_MAILBOX (-10)
#define NU_MAILBOX_EMPTY (-29)
#define NU_MAILBOX_RESET (-31)
#define NU_MAILBOX_FULL (-30)

#define NU_INVALID_QUEUE (-19)
#define NU_INVALID_SIZE (-22)
#define NU_QUEUE_EMPTY (-45)
#define NU_QUEUE_RESET (-47)
#define NU_QUEUE_FULL (-46)
#define NU_FIXED_SIZE (7)
#define NU_VARIABLE_SIZE (13)

#define NU_INVALID_PIPE (-14)
#define NU_PIPE_EMPTY (-40)
#define NU_PIPE_RESET (-42)
#define NU_PIPE_FULL (-41)

#define NU_INVALID_SEMAPHORE (-21)
#define NU_SEMAPHORE_RESET (-49)
#define NU_UNAVAILABLE (-51)

#define NU_INVALID_GROUP (-8)
#define NU_AND (2)
#define NU_AND_CONSUME (3)
#define NU_OR (0)
#define NU_OR_CONSUME (1)
#define NU_INVALID_OPERATION (-13)
#define NU_NOT_PRESENT (-36)

#define NU_INVALID_TIMER (-26)
#define NU_ENABLE_TIMER (5)
#define NU_DISABLE_TIMER (4)
#define NU_INVALID_FUNCTION (-7)
#define NU_INVALID_ENABLE (-5)
#define NU_NOT_DISABLED (-35)

#define NUSE_NO_TASK (0x80)

#define NUSE_TASK (0)
#define NUSE_STARTUP (1)
#define NUSE_NISR (2)
#define NUSE_MISR (3)

#endif  /* ! _NUSE_CODES_H_ */

