#include <ioavr.h>
#include <string.h>
#include "keyb_drv.h"
#include "queue.h"

static void kbAddKey(TQueue *q, unsigned char val, unsigned char id)
{
	do
	{
		if (val & 0x01)
			qAdd(q, id);
		++id;
		val = val >> 1;
	} while (val);
}

void kbService(TQueue *q)
{
	//Global but private variables -> static
	static unsigned char kb_buf[4] = {0, 0, 0, 0};
	//Local variables
	unsigned char *kbp;
	unsigned char key_down;
	unsigned char key_up;
	unsigned char cnt;
	kb_buf[3] = kb_buf[2];
	kb_buf[2] = kb_buf[1];
	kb_buf[1] = kb_buf[0];
	kb_buf[0] = ~KB_IN;
	key_down = 0xFF;
	key_up = 0;
	cnt = 3;
	kbp = kb_buf;
	do
	{
		key_down &= *kbp;
		key_up |= *(kbp++);
	} while (--cnt);
	key_down &= ~*kbp;
	key_up |= ~*kbp;
	if (key_down)
		kbAddKey(q, key_down, MSG(KEY_DOWN, 0));
	key_up = ~key_up;
	if (key_up)
		kbAddKey(q, key_up, MSG(KEY_UP, 0));
}
