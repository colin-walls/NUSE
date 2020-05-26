Nucleus SE Simple Demo for Run To Completion Scheduler
------------------------------------------------------

task0 continuously feeds a sequence of numbers into a queue.
It returns when the queue is full, trying again next time it is scheduled.

task1 obtains values from the queue and posts them into a mailbox.
If the mailbox is full, it returns, trying again next time it is scheduled.

task2 obtains values from a mailbox and does some mathematical processing on them.
This processing may require multiple iterations and the task returns between them.
