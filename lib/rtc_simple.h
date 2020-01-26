#ifndef RTC_SIMPLE_H
#define RTC_SIMPLE_H

typedef struct TClock
{
	unsigned int msec;
} TClock;

void rtcInc(TClock *rtc);

#endif
