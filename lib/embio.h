#ifndef EMBIO_H
#define EMBIO_H

typedef union TSystemEvent {
    unsigned char cEvent;
    struct {
        unsigned char Time : 1;
        unsigned char Kb   : 1;
        un
    } sEvent;
} TSystemEvent;


#endif
