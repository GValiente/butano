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
#define MMIO_GPIO_DATA         ((volatile short*) 0x80000c4)
#define MMIO_GPIO_WRITE_ENABLE ((volatile short*) 0x80000c6)
#define MMIO_GPIO_ENABLE       ((volatile short*) 0x80000c8)

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
#define TM_TEST (0x80)

/* Init results */
#define INIT_OK      (0x00)
#define INIT_EPOWER  (0x01)
#define INIT_E12HOUR (0x02)

/* Compiler hacks */
#define assume(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)
#define unlikely(cond) __builtin_expect(!!(cond), 0)

static unsigned int rtc_read(unsigned int len);
static void rtc_cmd(unsigned int cmd);
static void rtc_cmd_arg(unsigned int cmd, unsigned int data, unsigned int len);
static void rtc_cmd_arg_datetime(unsigned int cmd, __agbabi_datetime_t datetime);
static void rtc_reset(void);
static unsigned int rtc_get_status(void);
static void rtc_set_status_24hr(void);

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
    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_DATA | RTC_SCK;

    unsigned ii = 8;
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

    assume(len > 0 && len <= 32);
    for (ii = 0; ii < len; ++ii) {
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

static void rtc_cmd_arg_datetime(unsigned int cmd, __agbabi_datetime_t datetime) {
    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_DATA | RTC_SCK;

    unsigned ii = 8;
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

    for (ii = 0; ii < 55; ++ii) {
        unsigned int output = (datetime >> ii)[0] & 1;
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

int __agbabi_rtc_init(void) {
    *MMIO_GPIO_ENABLE = 1;

    unsigned int status = rtc_get_status();

    if (unlikely((status & STAT_POWER) == STAT_POWER || (status & STAT_24HOUR) == 0)) {
        rtc_reset();
        rtc_set_status_24hr();
    }

    const unsigned int time = __agbabi_rtc_time();

    if (time & TM_TEST) {
        rtc_reset(); /* Reset to leave test mode */
        rtc_set_status_24hr();
    }

    status = rtc_get_status();

    if (unlikely((status & STAT_POWER) == STAT_POWER)) {
        return INIT_EPOWER;
    }

    if (unlikely((status & STAT_24HOUR) == 0)) {
        return INIT_E12HOUR;
    }

    return INIT_OK;
}

unsigned int __agbabi_rtc_time(void) {
    static const unsigned int cmd_time = MAKE_READ_CMD(0x3);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd(cmd_time);
    const unsigned int time = rtc_read(23);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;

    return time;
}

void __agbabi_rtc_settime(const unsigned int time) {
    static const unsigned int cmd_time = MAKE_WRITE_CMD(0x3);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd_arg(cmd_time, time, 23);

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
        rtc_read(32),
        rtc_read(23)
    };

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;

    return datetime;
}

void __agbabi_rtc_setdatetime(const __agbabi_datetime_t datetime) {
    static const unsigned int cmd_datetime = MAKE_WRITE_CMD(0x2);

    *MMIO_GPIO_WRITE_ENABLE = RTC_CS | RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_CS | RTC_SCK;

    rtc_cmd_arg_datetime(cmd_datetime, datetime);

    *MMIO_GPIO_DATA = RTC_SCK;
    *MMIO_GPIO_DATA = RTC_SCK;
}
