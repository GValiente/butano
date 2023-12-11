/*
===============================================================================

 POSIX:
    _gettimeofday, settimeofday

 Support:
    __agbabi_rtc_init, __agbabi_rtc_time, __agbabi_rtc_settime,
    __agbabi_rtc_datetime, __agbabi_rtc_setdatetime

 Copyright (C) 2021-2023 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

#include "../include/agbabi.h"

/* MMIO registers */
#define MMIO_IME               ((volatile short*) 0x4000208)
#define MMIO_GPIO_DATA         ((volatile short*) 0x80000C4)
#define MMIO_GPIO_WRITE_ENABLE ((volatile short*) 0x80000C6)
#define MMIO_GPIO_ENABLE       ((volatile short*) 0x80000C8)

/* RTC pins */
#define RTC_CS   (0x4)
#define RTC_DATA (0x2)
#define RTC_SCK  (0x1)

/* RTC command helpers */
#define MAKE_WRITE_CMD(cmd) (0x60 | cmd << 1)
#define MAKE_READ_CMD(cmd)  (0x61 | cmd << 1)

/* Status flags */
#define STAT_INTFE  (0x02)
#define STAT_INTME  (0x08)
#define STAT_INTAE  (0x20)
#define STAT_24HOUR (0x40)
#define STAT_POWER  (0x80)

/* Time flags */
#define TM_TEST (0x00800000)

/* Init results */
#define INIT_OK      (0x00)
#define INIT_EPOWER  (0x01)
#define INIT_ENORTC  (0x02)

// Mask out data not needed from date and time
#define PM_TIME_FLAG    (0x00000080u)
#define HOURS_MASK      (0x000000BFu)
#define TIME_MASK   (0x00FF7FBF)
#define DATE_MASK   (0x073F1FFF)

// Amount of iterations to wait after writing the data,
// before touching other RTC-related registers. Dependant on optimization level
#define TIMEOUT_CYCLES_WAIT_AFTER_WRITE_PER_BYTE 5574

/* Compiler hacks */
#define assume(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)
#define unlikely(cond) __builtin_expect(!!(cond), 0)

static unsigned int rtc_read(unsigned int len);
static void rtc_write(unsigned int data, unsigned int len);
static void rtc_cmd(unsigned int cmd);
static void rtc_cmd_arg(unsigned int cmd, unsigned int data, unsigned int len);
static void rtc_cmd_arg_datetime(unsigned int cmd, __agbabi_datetime_t datetime, int is12hr);
static void rtc_reset(void);
static unsigned int rtc_get_status(void);
static void rtc_set_status_24hr(void);
static int rtc_is_12hr(void);
static unsigned int rtc_get_time_pm_fix(unsigned int time);
static unsigned int rtc_set_time_pm_fix(unsigned int time, int is12hr);

static unsigned int rtc_read(unsigned int len) {
    assume(len > 0 && len <= 32);
    unsigned int data = 0;

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    for (unsigned int ii = 0; ii < len; ++ii) {
        unsigned int input;
        __asm__ volatile (
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[clock], [%[gpio]]"   "\n\t"
            "ldrh    %[data], [%[gpio]]"
            :   [data]"=l"(input)
            :   [gpio]"l"(MMIO_GPIO_DATA),
                [select]"l"(RTC_CS),
                [clock]"l"(RTC_CS | RTC_SCK)
        );

        input = (input & RTC_DATA) >> 1;
        data |= input << ii;
    }

    return data;
}

static void rtc_write(unsigned int data, unsigned int len) {
    for (unsigned int ii = 0; ii < len; ++ii) {
        unsigned int output = data >> ii & 1;
        __asm__ volatile (
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[clock], [%[gpio]]"
            ::  [gpio]"l"(MMIO_GPIO_DATA),
                [select]"l"(RTC_CS | output << 1),
                [clock]"l"(RTC_CS | output << 1 | RTC_SCK)
        );
    }
}

static void rtc_cmd(const unsigned int cmd) {
    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_DATA | RTC_SCK;

    int ii = 8;
    while (ii--) {
        unsigned int output = cmd >> ii & 1;
        __asm__ volatile (
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[select], [%[gpio]]"  "\n\t"
            "strh    %[clock], [%[gpio]]"
            ::  [gpio]"l"(MMIO_GPIO_DATA),
                [select]"l"(RTC_CS | output << 1),
                [clock]"l"(RTC_CS | output << 1 | RTC_SCK)
        );
    }
}

static void rtc_cmd_arg(const unsigned int cmd, unsigned int data, unsigned int len) {
    rtc_cmd(cmd);

    assume(len > 0 && len <= 32);
    rtc_write(data, len);

    int wait = (TIMEOUT_CYCLES_WAIT_AFTER_WRITE_PER_BYTE * 7 + 11 + 6) / 12;
    __asm__ volatile (
        "L1%=:"                         "\n\t"
#if defined(__ARM_ASM_SYNTAX_UNIFIED__) && __ARM_ASM_SYNTAX_UNIFIED__ == 1
        "subs  %[wait], %[wait], #1"    "\n\t"
#else
        "sub   %[wait], #1"             "\n\t"
#endif
        "bne   L1%="
        : [wait]"+r"(wait)
    );
}

static void rtc_cmd_arg_datetime(unsigned int cmd, __agbabi_datetime_t datetime, int is12hr) {
    rtc_cmd(cmd);

    unsigned int date = datetime[0];
    unsigned int time = rtc_set_time_pm_fix(datetime[1], is12hr);

    rtc_write(date, 32);
    rtc_write(time, 24);

    int wait = (TIMEOUT_CYCLES_WAIT_AFTER_WRITE_PER_BYTE * 7 + 11 + 6) / 12;
    __asm__ volatile (
        "L1%=:"                         "\n\t"
#if defined(__ARM_ASM_SYNTAX_UNIFIED__) && __ARM_ASM_SYNTAX_UNIFIED__ == 1
        "subs  %[wait], %[wait], #1"    "\n\t"
#else
        "sub   %[wait], #1"             "\n\t"
#endif
        "bne   L1%="
        : [wait]"+r"(wait)
    );
}

static void rtc_reset(void) {
    static const unsigned int cmd_reset = MAKE_READ_CMD(0x0);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd(cmd_reset);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
}

static unsigned int rtc_get_status(void) {
    static const unsigned int cmd_status = MAKE_READ_CMD(0x1);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd(cmd_status);
    const unsigned int status = rtc_read(8);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;

    return status;
}

static void rtc_set_status_24hr(void) {
    static const unsigned int cmd_status = MAKE_WRITE_CMD(0x1);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd_arg(cmd_status, STAT_24HOUR, 8);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
}

static int rtc_is_12hr(void) {
    return (rtc_get_status() & STAT_24HOUR) == 0;
}

static unsigned int rtc_get_time_pm_fix(unsigned int time) {
    unsigned int hour = time & HOURS_MASK;
    time &= ~HOURS_MASK;

    if (hour & PM_TIME_FLAG && (hour & ~PM_TIME_FLAG) <= 0x11) {
        hour += 0x12;
        // BCD Math
        if ((hour & 0xF) >= 0xA) {
            hour += 0x10 - 0xA;
        }
    }

    return (time | hour) & ~PM_TIME_FLAG;
}

static unsigned int rtc_set_time_pm_fix(unsigned int time, const int is12hr) {
    time &= ~PM_TIME_FLAG;
    unsigned int hour = time & HOURS_MASK;
    time &= ~HOURS_MASK;

    if (hour >= 0x12 && is12hr) {
        hour -= 0x12;
        // BCD Math
        if ((hour & 0xF) >= 0xE) {
            hour -= 0xE - 0x8;
        }
        hour |= PM_TIME_FLAG;
    }

    return time | hour;
}

int __agbabi_rtc_init(void) {
    *MMIO_GPIO_ENABLE = 1;

    unsigned int status = rtc_get_status();

    if (unlikely((status & STAT_POWER) == STAT_POWER)) {
        rtc_reset();
        rtc_set_status_24hr();
    }

    const __agbabi_datetime_t datetime = __agbabi_rtc_datetime();
    
    if (unlikely(datetime[0] == 0)) {
        return INIT_ENORTC;
    }

    if (datetime[1] & TM_TEST) {
        rtc_reset(); /* Reset to leave test mode */
        rtc_set_status_24hr();
    }

    status = rtc_get_status();

    if (unlikely((status & STAT_POWER) == STAT_POWER)) {
        return INIT_EPOWER;
    }

    return INIT_OK;
}

unsigned int __agbabi_rtc_time(void) {
    static const unsigned int cmd_time = MAKE_READ_CMD(0x3);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd(cmd_time);
    const unsigned int time = rtc_get_time_pm_fix(rtc_read(24) & TIME_MASK);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;

    return time;
}

void __agbabi_rtc_settime(const unsigned int time) {
    static const unsigned int cmd_time = MAKE_WRITE_CMD(0x3);

    const int is12hr = rtc_is_12hr();

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd_arg(cmd_time, rtc_set_time_pm_fix(time, is12hr), 24);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
}

__agbabi_datetime_t __agbabi_rtc_datetime(void) {
    static const unsigned int cmd_datetime = MAKE_READ_CMD(0x2);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd(cmd_datetime);
    const __agbabi_datetime_t datetime = {
        rtc_read(32) & DATE_MASK,
        rtc_get_time_pm_fix(rtc_read(24) & TIME_MASK)
    };

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;

    return datetime;
}

void __agbabi_rtc_setdatetime(const __agbabi_datetime_t datetime) {
    static const unsigned int cmd_datetime = MAKE_WRITE_CMD(0x2);

    const int is12hr = rtc_is_12hr();

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd_arg_datetime(cmd_datetime, datetime, is12hr);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
}
