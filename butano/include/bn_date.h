/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DATE_H
#define BN_DATE_H

/**
 * @file
 * bn::date header file.
 *
 * @ingroup date_time
 */

#include "bn_assert.h"
#include "bn_optional_fwd.h"

namespace bn
{

/**
 * @brief Manages a date represented by year, month, month day and week day.
 *
 * @ingroup date_time
 */
class date
{

public:
    /**
     * @brief Indicates if the specified date is valid or not.
     * @param year Year part of the date.
     * @param month Month part of the date, in the range [1..12].
     * @param month_day Month day part of the date, in the range [1..31].
     * @param week_day Week day part of the date, in the range [0..6].
     * @return `true` if the specified date is valid; `false` otherwise.
     */
    [[nodiscard]] constexpr static bool valid(int year, int month, int month_day, int week_day)
    {
        if(year >= 0 && month >= 1 && month <= 12 && week_day >= 0 && week_day <= 6 && month_day >= 1)
        {
            int max_month_day = 31;

            switch(month)
            {

            case 2:
                {
                    bool leap_year = false;

                    if(year % 4 == 0)
                    {
                        if(year % 100 == 0)
                        {
                            leap_year = year % 400 == 0;
                        }
                        else
                        {
                            leap_year = true;
                        }
                    }

                    max_month_day = leap_year ? 29 : 28;
                }
                break;

            case 4:
            case 6:
            case 9:
            case 11:
                max_month_day = 30;
                break;

            default:
                break;
            }

            return month_day <= max_month_day;
        }

        return false;
    }

    /**
     * @brief Indicates if the real time clock (RTC) is present or not.
     */
    [[nodiscard]] static bool active();

    /**
     * @brief Returns the current date as reported by the real time clock (RTC) if it's present
     * and the reported date is valid; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<date> current();

    /**
     * @brief Constructor.
     * @param year Year part of the date.
     * @param month Month part of the date, in the range [1..12].
     * @param month_day Month day part of the date, in the range [1..31].
     * @param week_day Week day part of the date, in the range [0..6].
     */
    constexpr date(int year, int month, int month_day, int week_day) :
        _year(year),
        _month(uint8_t(month)),
        _month_day(uint8_t(month_day)),
        _week_day(uint8_t(week_day))
    {
        BN_BASIC_ASSERT(valid(year, month, month_day, week_day),
                        "Invalid date: ", year, '/', month, '/', month_day, " (", week_day, ')');
    }

    /**
     * @brief Returns the year part of the date.
     */
    [[nodiscard]] constexpr int year() const
    {
        return _year;
    }

    /**
     * @brief Sets the year part of the date.
     * @param year Year part of the date.
     */
    constexpr void set_year(int year)
    {
        BN_BASIC_ASSERT(valid(year, _month, _month_day, _week_day),
                        "Invalid date: ", year, '/', _month, '/', _month_day, " (", _week_day, ')');

        _year = year;
    }

    /**
     * @brief Returns the month part of the date.
     */
    [[nodiscard]] constexpr int month() const
    {
        return _month;
    }

    /**
     * @brief Sets the month part of the date.
     * @param month Month part of the date, in the range [1..12].
     */
    constexpr void set_month(int month)
    {
        BN_BASIC_ASSERT(valid(_year, month, _month_day, _week_day),
                        "Invalid date: ", _year, '/', month, '/', _month_day, " (", _week_day, ')');

        _month = uint8_t(month);
    }

    /**
     * @brief Returns the month day part of the date.
     */
    [[nodiscard]] constexpr int month_day() const
    {
        return _month_day;
    }

    /**
     * @brief Sets the month day part of the date.
     * @param month_day Month day part of the date, in the range [1..31].
     */
    constexpr void set_month_day(int month_day)
    {
        BN_BASIC_ASSERT(valid(_year, _month, month_day, _week_day),
                        "Invalid date: ", _year, '/', _month, '/', month_day, " (", _week_day, ')');

        _month_day = uint8_t(month_day);
    }

    /**
     * @brief Returns the week day part of the date.
     */
    [[nodiscard]] constexpr int week_day() const
    {
        return _week_day;
    }

    /**
     * @brief Sets the week day part of the date.
     * @param week_day Week day part of the date, in the range [0..6].
     */
    constexpr void set_week_day(int week_day)
    {
        BN_BASIC_ASSERT(valid(_year, _month, _month_day, week_day),
                        "Invalid date: ", _year, '/', _month, '/', _month_day, " (", week_day, ')');

        _week_day = uint8_t(week_day);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(date a, date b) = default;

    /**
     * @brief Less than operator.
     * @param a First date to compare.
     * @param b Second date to compare.
     * @return `true` if the first date is earlier than the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(date a, date b)
    {
        if(a.year() != b.year())
        {
            return a.year() < b.year();
        }

        if(a.month() != b.month())
        {
            return a.month() < b.month();
        }

        return a.month_day() < b.month_day();
    }

    /**
     * @brief Greater than operator.
     * @param a First date to compare.
     * @param b Second date to compare.
     * @return `true` if the first date is later than the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(date a, date b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First date to compare.
     * @param b Second date to compare.
     * @return `true` if the first date is earlier than or equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(date a, date b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First date to compare.
     * @param b Second date to compare.
     * @return `true` if the first date is later than or equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(date a, date b)
    {
        return ! (a < b);
    }

private:
    int _year;
    uint8_t _month;
    uint8_t _month_day;
    uint8_t _week_day;
};

}

#endif
