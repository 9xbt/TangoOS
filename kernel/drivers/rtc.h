#ifndef __RTC_H
#define __RTC_H

#include <stdint.h>

extern const char *rtc_days[];
extern const char *rtc_months[]; 

uint32_t bcd_to_bin(uint32_t value);
uint32_t rtc_second(void);
uint32_t rtc_minute(void);
uint32_t rtc_hour(void);
uint32_t rtc_day(void);
uint32_t rtc_month(void);
uint32_t rtc_year(void);
uint32_t rtc_century(void);
uint32_t rtc_day_of_week(void);
uint32_t rtc_time(void);
void rtc_sleep(uint32_t seconds);

#endif