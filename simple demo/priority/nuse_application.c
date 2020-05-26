#include "nuse.h"


void task0(void)
{
    STATUS ret;

    while (TRUE)
        ret = NUSE_Task_Suspend(0);
}

void task1(void)
{
    U8 sig;
    STATUS ret;

    ret = NUSE_Timer_Control(0, NUSE_ENABLE_TIMER);

    while (TRUE)
    {
        sig = NUSE_Signals_Receive();
        if (sig == 0)
            NUSE_Task_Sleep(10);
        else
            ret = NUSE_Task_Resume(0);
    }
}

U32 systime;

void task2(void)
{

    NUSE_Clock_Set(0);

    while (TRUE)
        systime = NUSE_Clock_Retrieve();
}

void timerexp(U8 dummy)
{
    STATUS ret;

    ret = NUSE_Signals_Send(1, 1);
}
