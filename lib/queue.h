#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 16

typedef struct TQueue
{
    unsigned char size;
    unsigned char data_cnt;
    unsigned char head;
    unsigned char tail;
    unsigned char *buf;
} TQueue;

void qInit(TQueue *q, unsigned char *qbuf, unsigned char size);
unsigned char qAdd(TQueue *q, unsigned char d);
unsigned char qGet(TQueue *q, unsigned char *pd);
void qFlush(TQueue *q);

#endif
