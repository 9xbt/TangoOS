#include <drivers/rtc.h>
#include <cpu/io.h>

const char *rtc_days[] = {
    "Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"
};

const char *rtc_months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/*
 * bd_to_bin - converts binary-coded-decimal to binary.
 */
uint32_t bcd_to_bin(uint32_t value) {
    return (value >> 4) * 10 + (value & 15);
}

/*
 * rtc_second - gets the second.
 */
uint32_t rtc_second(void) {
    outb(0x70, 0x00);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_minute - gets the minute.
 */
uint32_t rtc_minute(void) {
    outb(0x70, 0x02);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_hour - gets the hour.
 */
uint32_t rtc_hour(void) {
    outb(0x70, 0x04);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_day - gets the day of the month.
 */
uint32_t rtc_day(void) {
    outb(0x70, 0x07);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_month - gets the month number.
 */
uint32_t rtc_month(void) {
    outb(0x70, 0x08);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_year - gets the 2-digit year.
 */
uint32_t rtc_year(void) {
    outb(0x70, 0x09);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_century - gets the century.
 */
uint32_t rtc_century(void) {
    outb(0x70, 0x32);
    return bcd_to_bin(inb(0x71));
}

/*
 * rtc_day_of_week - gets the day of the week number.
 */
uint32_t rtc_day_of_week(void) {
    uint32_t year = rtc_year();
    uint32_t month = rtc_month();
    uint32_t day = rtc_day();

    if (month < 3) {
        month += 12;
        year--;
    }

    return (day + (13 * (month + 1) / 5) + (year % 100) + ((year % 100) / 4) + ((year / 100) / 4) + 5 * (year / 100)) % 7;
}

/*
 * rtc_time - gets the time of the day to the second.
 */
uint32_t rtc_time(void) {
    return rtc_hour() * 3600 + rtc_minute() * 60 + rtc_second();
}

/*
 * rtc_sleep - sleeps for seconds, using the RTC.
 */
void rtc_sleep(uint32_t seconds) {
    uint32_t last_sec = rtc_time() + seconds;
    while (last_sec != rtc_time());
}