/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TIME_H
#define BN_TIME_H

/**
 * @file
 * bn::time header file.
 *
 * @ingroup date_time
 */

#include "bn_assert.h"
#include "bn_optional_fwd.h"

namespace bn
{

/**
 * @brief Defines a clock time, which it can express as the numbers of hours, minutes and seconds since midnight.
 *
 * @ingroup date_time
 */
class time
{

public:
    /**
     * @brief Indicates if the specified time is valid or not.
     * @param hour Hour part of the time, in the range [0..23].
     * @param minute Minute part of the time, in the range [0..59].
     * @param second Second part of the time, in the range [0..59].
     * @return `true` if the specified time is valid; `false` otherwise.
     */
    [[nodiscard]] constexpr static bool valid(int hour, int minute, int second)
    {
        return hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60;
    }

    /**
     * @brief Indicates if the real time clock (RTC) is present or not.
     */
    [[nodiscard]] static bool active();

    /**
     * @brief Returns the current time as reported by the real time clock (RTC) if it's present
     * and the reported time is valid; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<time> current();

    /**
     * @brief Constructor.
     * @param hour Hour part of the time, in the range [0..23].
     * @param minute Minute part of the time, in the range [0..59].
     * @param second Second part of the time, in the range [0..59].
     */
    constexpr time(int hour, int minute, int second) :
        _hour(uint8_t(hour)),
        _minute(uint8_t(minute)),
        _second(uint8_t(second))
    {
        BN_ASSERT(hour >= 0 && hour < 24, "Invalid hour: ", hour);
        BN_ASSERT(minute >= 0 && minute < 60, "Invalid minute: ", minute);
        BN_ASSERT(second >= 0 && second < 60, "Invalid second: ", second);
    }

    /**
     * @brief Returns the hour part of the time.
     */
    [[nodiscard]] constexpr int hour() const
    {
        return _hour;
    }

    /**
     * @brief Sets the hour part of the time.
     * @param hour Hour part of the time, in the range [0..23].
     */
    constexpr void set_hour(int hour)
    {
        BN_ASSERT(hour >= 0 && hour < 24, "Invalid hour: ", hour);

        _hour = uint8_t(hour);
    }

    /**
     * @brief Returns the minute part of the time.
     */
    [[nodiscard]] constexpr int minute() const
    {
        return _minute;
    }

    /**
     * @brief Sets the minute part of the time.
     * @param minute Minute part of the time, in the range [0..23].
     */
    constexpr void set_minute(int minute)
    {
        BN_ASSERT(minute >= 0 && minute < 60, "Invalid minute: ", minute);

        _minute = uint8_t(minute);
    }

    /**
     * @brief Returns the second part of the time.
     */
    [[nodiscard]] constexpr int second() const
    {
        return _second;
    }

    /**
     * @brief Sets the second part of the time.
     * @param second Second part of the time, in the range [0..23].
     */
    constexpr void set_second(int second)
    {
        BN_ASSERT(second >= 0 && second < 60, "Invalid second: ", second);

        _second = uint8_t(second);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(time a, time b) = default;

    /**
     * @brief Less than operator.
     * @param a First time to compare.
     * @param b Second time to compare.
     * @return `true` if the first time is earlier than the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(time a, time b)
    {
        if(a.hour() != b.hour())
        {
            return a.hour() < b.hour();
        }

        if(a.minute() != b.minute())
        {
            return a.minute() < b.minute();
        }

        return a.second() < b.second();
    }

    /**
     * @brief Greater than operator.
     * @param a First time to compare.
     * @param b Second time to compare.
     * @return `true` if the first time is later than the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(time a, time b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First time to compare.
     * @param b Second time to compare.
     * @return `true` if the first time is earlier than or equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(time a, time b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First time to compare.
     * @param b Second time to compare.
     * @return `true` if the first time is later than or equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(time a, time b)
    {
        return ! (a < b);
    }

private:
    uint8_t _hour;
    uint8_t _minute;
    uint8_t _second;
};

}

#endif
