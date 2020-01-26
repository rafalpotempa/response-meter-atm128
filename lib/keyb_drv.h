#ifndef KEYB_DRV_H
#define KEYB_DRV_H

#include "queue.h"

#define KB_IN PINE
#define KB_OUT PORTE
#define KB_CTRL DDRE

#define KEY_DOWN	0x01
#define KEY_UP		0x02

#define MSG(ID, DATA) ((ID) << 4) + (DATA)

void kbService(TQueue *q);

#endif
