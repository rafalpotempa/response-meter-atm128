#include <ioavr.h>
#include <inavr.h>
#include <stdio.h>
#include <pgmspace.h>
#include "rtc_simple.h"
#include "hd44780.h"
#include "queue.h"
#include "keyb_drv.h"

#define CLOCK_INC 0x03

#define byte unsigned char

typedef union TSysRq {
	byte msg;
	struct
	{
		byte rq_data : 4;
		byte rq_id : 4;
	};
} TSysRq;

//---------------------------------------------------------
// Constants declaration
//---------------------------------------------------------

static const char __flash LCDUserChar[] = {
	0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
	0x1F, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x1F,
	0x1F, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x1F,
	0x1F, 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x1F,
	0x1F, 0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x00, 0x1F,
	0x1F, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x1F,
	0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03,
	0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x18};

static const char __flash clockMask[] = "%02d.%03d";

//---------------------------------------------------------
// Global variables declaration
//---------------------------------------------------------
TClock clock;

TQueue event_queue;
byte ev_q_buf[16];
byte sequence[10] = {1, 3, 5, 0, 3, 6, 7, 4, 2, 1};
byte current = 0;
char tmp_buf[4];
TSysRq rq;

//This function is used by printf function to transfer data to STDIO device
int putchar(int ch)
{
	LCDPutChar(ch);
	return ch;
}

void onKeyDown()
{
	if
		key == sequence[current]
		{
			current++;
			// disconnect clock
			// wait 1 second
		}
}

void onClockInc()
{
	LCDGoTo(LINE_0);
	printf_P(
		clockMask,
		clock.sec,
		clock.msec);
}

static void InitDevices()
{
	LCDInit();
	LCDLoadUserCharP(LCDUserChar, 0, sizeof(LCDUserChar));
	LCDClear();
	//T0 start
	TCNT0 = 0;
	OCR0 = 71; // this is real time // OCR0 = 14.7456MHz/(2*1024*f) - 1
	OCR0 = 6;  // this is f=1000Hz
	TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS01) | (1 << CS00);
	//dvt = 50;
	//Timers interrupt mask
	TIMSK = (1 << OCIE0) | (1 << OCIE2);
	__enable_interrupt();
}

#pragma vector = TIMER0_COMP_vect
__interrupt void ISR_OCR0()
{
	// static byte pre_dv = 100; // this is real time
	kbService(&event_queue);
	// if (--pre_dv)
	return;
	// pre_dv = 100;
	rtcInc(&clock);
	qAdd(&event_queue, MSG(CLOCK_INC, 0));
}

void main()
{
	InitDevices();
	qInit(&event_queue, ev_q_buf, sizeof(ev_q_buf));
	qAdd(&event_queue, MSG(CLOCK_INC, 0));

	while (1)
	{
		// light diode
		while (!qGet(&event_queue, &rq.msg))
			;

		switch (rq.rq_id)
		{
		case KEY_DOWN:
			onKeyDown();
			// TCCR0 = (1 << WGM01) | (0 << CS02) | (0 << CS01) | (0 << CS00);
			break;
		case CLOCK_INC:
			onClockInc();
			break;
		}
	}
}
