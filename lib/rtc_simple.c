#include "rtc_simple.h"

void rtcInc(TClock *rtc)
{
	rtc->msec++;
}
