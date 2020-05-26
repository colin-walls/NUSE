#include "nuse.h"

void task0(void)
{
    static ADDR numbers[10] = { (ADDR)1, (ADDR)2, (ADDR)3, (ADDR)5, (ADDR)8, (ADDR)13, (ADDR)21, (ADDR)34, (ADDR)55, (ADDR)89 };
    static U8 count = 0;
    STATUS ret;

    while (TRUE)
    {
        ret = NUSE_Queue_Send(0, &numbers[count], NUSE_NO_SUSPEND);
        if (ret != NUSE_SUCCESS)
            return;
        count++;
        if (count == 10)
            count = 0;
    }
}

void task1(void)
{
    static ADDR data = (ADDR)0;
    STATUS ret;
    U8 size;

    if (data == 0)
    {
        ret = NUSE_Queue_Receive(0, &data, NUSE_NO_SUSPEND);
        if (ret != NUSE_SUCCESS)
            return;
    }

    ret = NUSE_Mailbox_Send(0, &data, NUSE_NO_SUSPEND);
    if (ret == NUSE_SUCCESS)
        data = (ADDR)0;
}

void task2(void)
{
    static U32 numbers[10];
    static U32 num = 0;
    ADDR data;
    static U8 count = 0;
    STATUS ret;

    if (num == 0)
    {
        ret = NUSE_Mailbox_Receive(0, &data, NUSE_NO_SUSPEND);
        if (ret != NUSE_SUCCESS)
            return;
        num = (U32)data;
    }

    if (num < 50)
    {
        num *= 10;
    }
    else
    {
        numbers[count] = num;
        num = 0;
        count++;
        if (count == 10)
            count = 0;
    }
}
