#include <am.h>
#include <x86.h>
#include <amdev.h>
#define RTC 0x48
extern uint32_t uptime();
//static uint32_t boot;

size_t timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      uint32_t now;
	  now = inl(RTC);
	  _UptimeReg *uptime = (_UptimeReg *)buf;
	  long useconds = now;
      uptime->hi = 0;
      uptime->lo = (useconds + 500) / 1000;
      //uptime->lo = inl(RTC);
	  return sizeof(_UptimeReg);
    }
    case _DEVREG_TIMER_DATE: {
      _RTCReg *rtc = (_RTCReg *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2018;
      return sizeof(_RTCReg);
    }
  }
  return 0;
}

void timer_init() {
	//boot = inl(RTC);
}
