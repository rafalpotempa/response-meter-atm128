#include <ioavr.h>
#include "rot_drv.h"

//Abstract translation of input signals from rottary encoder
#define VA(v) v &(1 << SW_A)
#define VB(v) v &(1 << SW_B)
#define PB(v) v &(1 << SW_PB)

//Rot FSM states
#define ROT_IDLE 0
#define ROT_LEFT_0 1
#define ROT_LEFT_1 2
#define ROT_LEFT_2 3
#define ROT_RIGHT_0 4
#define ROT_RIGHT_1 5
#define ROT_RIGHT_2 6

unsigned char rotEnc()
{
    //Private internal state of the FSM
    static unsigned char Q = ROT_IDLE;
    static unsigned char pb = 0;
    unsigned char a;
    unsigned char retVal;
    a = ROT_IN;
    retVal = 0;
    switch (Q)
    {
    case ROT_IDLE:
        if (~VA(a))
            Q = ROT_LEFT_0;
        if (~VB(a))
            Q = ROT_RIGHT_0;
        break;
    case ROT_LEFT_0:
        if (VA(a))
            Q = ROT_IDLE;
        if (~VB(a))
            Q = ROT_LEFT_1;
        break;
    case ROT_LEFT_1:
        if (VB(a))
            Q = ROT_LEFT_0;
        if (VA(a))
            Q = ROT_LEFT_2;
        break;
    case ROT_LEFT_2:
        if (~VA(a))
            Q = ROT_LEFT_1;
        if (VB(a))
        {
            Q = ROT_IDLE;
            retVal = (1 << ROT_LEFT);
            //Do something...
        }
        break;
    case ROT_RIGHT_0:
        if (VB(a))
            Q = ROT_IDLE;
        if (~VA(a))
            Q = ROT_RIGHT_1;
        break;
    case ROT_RIGHT_1:
        if (VA(a))
            Q = ROT_RIGHT_0;
        if (VB(a))
            Q = ROT_RIGHT_2;
        break;
    case ROT_RIGHT_2:
        if (VA(a))
        {
            Q = ROT_IDLE;
            //Do something...
            retVal = (1 << ROT_RIGHT);
        }
        if (~VB(a))
            Q = ROT_RIGHT_1;
        break;
    }
    pb = 0x1F & (pb + pb + ((PB(a)) ? 0 : 1));
    if (pb == 0x0F)
        retVal |= (1 << ROT_PB);
    return retVal;
}
