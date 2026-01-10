//
//  Date_Dealing.h
//  lookbackpricing
//
//  Created by Marco De Luca on 10/01/26.
//

#ifndef Date_Dealing_h
#define Date_Dealing_h

#include <chrono>
#include <string>
#include <stdexcept>

std::chrono::year_month_day date_formatting_dd_mm_yyyy(const std::string& s)
{
    int d, m, y;
    if(sscanf(s.c_str(), "%d-%d-%d", &d, &m, &y) != 3)
        throw std::invalid_argument("Invalid date format");
    
    return std::chrono::year{y} / std::chrono::month{m} / std::chrono::year{d};
}

struct Date
{
    std::chrono::year_month_day d_;
    
    explicit Date(const std::string& s): d_(date_formatting_dd_mm_yyyy(s)){}
};

enum class DayCountConv
{
    ACT_360,
    ACT_365F,
    THIRTY_360_US,
    THIRTY_360_EU,
    ACT_ACT_ISDA
};

// differenza tra giorni
int days_difference(const Date& date1, const Date& date2)
{
    return (std::chrono::sys_days{date2.d_} - std::chrono::sys_days{date1.d_}).count();
}

// controllo se l'anno è bisestile o no, tenendo in conto il discorso di ferreira
bool is_leap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// year fraction convention 30_360 europea
double yearfrac_30_360_eu(const Date& Date1, const Date& Date2)
{
    auto date1 = Date1.d;
    auto date2 = Date2.d;
    
    int year1 = int(date1.year()), year2 = int(date2.year());
    int month1 = unsigned(date1.month()), month2 = unsigned(date2.month());
    int day1 = unsigned(date1.day()), day2 = unsigned(date2.day());
    
    if (day1 == 31) day1 = 30;
    if (day2 == 31) day2 = 30;
    
    int days360 = 360*(year2 - year1) + 30*(month2 - month1) + (day2 - day1);
    
    return days360 / 360.0;
}

// year fraction convention 30_360 us (americana)
double yearfrac_30_360_us(const Date& Date1, const Date& Date2)
{
    auto date1 = Date1.d;
    auto date2 = Date2.d;
    
    int year1 = int(date1.year()), year2 = int(date2.year());
    int month1 = unsigned(date1.month()), month2 = unsigned(date2.month());
    int day1 = unsigned(date1.day()),   day2 = unsigned(date2.day());
    
    // funzione lambda per gestire febbraio, nella convenzione americana l'ultimo giorno di febbraio viene gestito come 30 anche se 28 o 29
    auto last_day = [](int y, int m)
    {
        std::chrono::year_month_day_last ymd_leap{std::chrono::year{y}, std::chrono::month{m}, last};
        return unsigned(ymd_leap.day());
    }
    
    if(month1 == 2 && last_day(year1, month1)) day1 = 30;
    if(month2 == 2 && last_day(year2, month2)) day2 = 30;
    
    if (day1 == 31) day1 = 30;
    if (day2 == 31 && day1 == 30) day2 = 30;

    int days360 = 360*(year2 - year1) + 30*(month2 - month1) + (day2 - day1);
    return days360 / 360.0;
}




#endif /* Date_Dealing_h */
