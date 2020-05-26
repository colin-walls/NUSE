/*************************************************************************
*
*   This file contains symbolic constants for Nucleus SE service call parameters
*   and return codes.
*   Not all codes are currently utilized by Nucleus SE.
*
*************************************************************************/


/* Check to see if the file has been included already.  */
#ifndef _NUSE_CODES_H_
#define _NUSE_CODES_H_

#define NUSE_SUCCESS (0)
#define NUSE_TRUE TRUE
#define NUSE_FALSE FALSE
#define NUSE_NO_SUSPEND (0)
#define NUSE_SUSPEND (0xff)
#define NUSE_INVALID_SUSPEND (-24)
#define NUSE_FIFO (6)
#define NUSE_PRIORITY (11)

#define NUSE_INVALID_TASK (-25)
#define NUSE_NOT_TERMINATED (-38)
#define NUSE_INVALID_RESUME (-20)

#define NUSE_READY (0)
#define NUSE_PURE_SUSPEND (1)
#define NUSE_FINISHED (11)
#define NUSE_TERMINATED (12)
#define NUSE_SLEEP_SUSPEND (2)
#define NUSE_MAILBOX_SUSPEND (3)
#define NUSE_QUEUE_SUSPEND (4)
#define NUSE_PIPE_SUSPEND (5)
#define NUSE_EVENT_SUSPEND (7)
#define NUSE_SEMAPHORE_SUSPEND (6)
#define NUSE_PARTITION_SUSPEND (8)

#define NUSE_INVALID_POOL (-16)
#define NUSE_INVALID_POINTER (-15)
#define NUSE_NO_PARTITION (-34)
#define NUSE_TIMEOUT (-50)

#define NUSE_INVALID_MAILBOX (-10)
#define NUSE_MAILBOX_EMPTY (-29)
#define NUSE_MAILBOX_WAS_RESET (-31)
#define NUSE_MAILBOX_FULL (-30)

#define NUSE_INVALID_QUEUE (-19)
#define NUSE_INVALID_SIZE (-22)
#define NUSE_QUEUE_EMPTY (-45)
#define NUSE_QUEUE_WAS_RESET (-47)
#define NUSE_QUEUE_FULL (-46)
#define NUSE_FIXED_SIZE (7)
#define NUSE_VARIABLE_SIZE (13)

#define NUSE_INVALID_PIPE (-14)
#define NUSE_PIPE_EMPTY (-40)
#define NUSE_PIPE_WAS_RESET (-42)
#define NUSE_PIPE_FULL (-41)

#define NUSE_INVALID_SEMAPHORE (-21)
#define NUSE_SEMAPHORE_WAS_RESET (-49)
#define NUSE_UNAVAILABLE (-51)

#define NUSE_INVALID_GROUP (-8)
#define NUSE_AND (2)
#define NUSE_AND_CONSUME (3)
#define NUSE_OR (0)
#define NUSE_OR_CONSUME (1)
#define NUSE_INVALID_OPERATION (-13)
#define NUSE_NOT_PRESENT (-36)

#define NUSE_INVALID_TIMER (-26)
#define NUSE_ENABLE_TIMER (5)
#define NUSE_DISABLE_TIMER (4)
#define NUSE_INVALID_FUNCTION (-7)
#define NUSE_INVALID_ENABLE (-5)
#define NUSE_NOT_DISABLED (-35)

#define NUSE_NO_TASK (0x80)

#define NUSE_TASK_CONTEXT (0)
#define NUSE_STARTUP_CONTEXT (1)
#define NUSE_NISR_CONTEXT (2)
#define NUSE_MISR_CONTEXT (3)

#endif  /* ! _NUSE_CODES_H_ */

