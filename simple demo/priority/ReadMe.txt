Nucleus SE Simple Demo for Priority Scheduler
---------------------------------------------

Task priorities:  task0 > task1 > task2

task0 is suspended when the program starts.
When it is woken up, it simply suspends itself again.

task1 starts a timer and awaits a signal from the expiration routine tinerexp().
It goes to sleep between polss of the signal.
When it receives the signal, it wakes up task0.

task2 gets to run while task1 sleeps.
It continuously monitors the system clock.