#ifndef LAB_6Q_H
#define LAB_6Q_H

typedef union TSystemEvent {
    unsigned char ev;
    struct
    {
        unsigned char time : 1;
        unsigned char kb : 1;
        unsigned char dummy : 6;
    };
} TSystemEvent;

//Forward declarations
void kbService();

#define KB_REP_DY 192
#define KB_TPM_DY 24
#define KB_IDLE 0x00

#endif
