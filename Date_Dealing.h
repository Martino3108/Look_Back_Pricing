/**
 * @file Date_Dealing.h
 * @brief Date parsing and year-fraction computation (day-count conventions).
 *
 * @details
 * This module provides:
 * - Parsing of date strings in format "dd-mm-yyyy" into `std::chrono::year_month_day`.
 * - Basic date difference in days.
 * - Year-fraction computation under standard day-count conventions.
 *
 * Exceptions:
 * - Parsing throws `std::invalid_argument` on invalid date format.
 * - Unknown conventions throw `std::invalid_argument`.
 *
 * Typical usage:
 * @code
 * Date d1("01-01-2026");
 * Date d2("01-07-2026");
 * double t = yearFraction(d1, d2, DayCountConv::ACT_365F);
 * @endcode
 */

#ifndef Date_Dealing_h
#define Date_Dealing_h

#include <chrono>
#include <string>
#include <stdexcept>

/** @brief Parses a date string formatted as "dd-mm-yyyy" into `std::chrono::year_month_day`. */
std::chrono::year_month_day date_formatting_dd_mm_yyyy(const std::string& s);

/** @brief Lightweight date wrapper storing a `std::chrono::year_month_day`. */
struct Date
{
    std::chrono::year_month_day d_;
    
    explicit Date(const std::string& s): d_(date_formatting_dd_mm_yyyy(s)){}
};

/** @brief Day-count conventions supported by yearFraction(). */
enum class DayCountConv
{
    ACT_360,
    ACT_365F,
    THIRTY_360_US,
    THIRTY_360_EU,
    ACT_ACT_ISDA
};

/** @brief Returns day difference (date2 - date1) in days. */
int days_difference(const Date& date1, const Date& date2);

// Checks if a given year is a leap year
bool is_leap(int year);

// year fraction convention 30_360 european
double yearfrac_30_360_eu(const Date& Date1, const Date& Date2);

// year fraction convention 30_360 us (american)
double yearfrac_30_360_us(const Date& Date1, const Date& Date2);

// act/act
double yearfrac_act_act_isda(const Date& date1, const Date& date2);

/** @brief Computes the year fraction between start and end according to a day-count convention. */
double yearFraction(const Date& start, const Date& end, DayCountConv dc  = DayCountConv::ACT_ACT_ISDA);

#endif /* Date_Dealing_h */
