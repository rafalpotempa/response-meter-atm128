#include "queue.h"

void qInit(TQueue *q, unsigned char *buf, unsigned char size)
{
    q->buf = buf;
    q->size = size;
    qFlush(q);
}

__monitor unsigned char qAdd(TQueue *q, unsigned char d)
{
    if (q->data_cnt == q->size)
        return 0;
    q->buf[q->head] = d;
    if ((++q->head) == q->size)
        q->head = 0;
    ++q->data_cnt;
    return 1;
}

__monitor unsigned char qGet(TQueue *q, unsigned char *pd)
{
    if (q->data_cnt == 0)
        return 0;
    *pd = q->buf[q->tail];
    if ((++q->tail) == q->size)
        q->tail = 0;
    --q->data_cnt;
    return 1;
}

void qFlush(TQueue *q)
{
    q->data_cnt = 0;
    q->head = 0;
    q->tail = 0;
}
