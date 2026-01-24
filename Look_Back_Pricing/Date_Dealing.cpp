//
//  Date_Dealing.cpp
//  lookbackpricing
//
//  Created by Marco De Luca on 10/01/26.
//

#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>

#include "Date_Dealing.h"

// funzione per la formattazione dei giorni
std::chrono::year_month_day date_formatting_dd_mm_yyyy(const std::string& s)
{
    int y;
    unsigned m, d;
    if(sscanf(s.c_str(), "%d-%d-%d", &d, &m, &y) != 3)
        throw std::invalid_argument("Invalid date format");
    
    return std::chrono::year{y} / std::chrono::month{m} / std::chrono::day{d};
}

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
    auto date1 = Date1.d_;
    auto date2 = Date2.d_;
    
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
    auto date1 = Date1.d_;
    auto date2 = Date2.d_;
    
    int year1 = int(date1.year()), year2 = int(date2.year());
    int month1 = unsigned(date1.month()), month2 = unsigned(date2.month());
    int day1 = unsigned(date1.day()),   day2 = unsigned(date2.day());
    
    // funzione lambda per gestire febbraio, nella convenzione americana l'ultimo giorno di febbraio viene gestito come 30 anche se 28 o 29
    auto last_day = [](int y, int m) -> unsigned
    {
        auto ymd_leap = std::chrono::year{y} / std::chrono::month{static_cast<unsigned>(m)} / std::chrono::last;
        return unsigned(ymd_leap.day());
    };
    
    if(month1 == 2 && last_day(year1, month1)) day1 = 30;
    if(month2 == 2 && last_day(year2, month2)) day2 = 30;
    
    if (day1 == 31) day1 = 30;
    if (day2 == 31 && day1 == 30) day2 = 30;

    int days360 = 360*(year2 - year1) + 30*(month2 - month1) + (day2 - day1);
    
    return days360 / 360.0;
}

double yearfrac_act_act_isda(const Date& date1, const Date& date2)
{
    int year1 = int(date1.d_.year());
    int year2 = int(date2.d_.year());
    
    if(year1 == year2)
    {
        double denom = 365.0;
        if(is_leap(year1)) denom = 366.0;
        return days_difference(date1, date2) / denom;
    }
    
    // calcolo per il primo anno
    int denom1 = 365;
    if (is_leap(year1)) denom1 = 366;
    std::chrono::year_month_day end_year1 = std::chrono::sys_days{ std::chrono::year{year1}/std::chrono::December/std::chrono::day{31}} + std::chrono::days{1}; // + 1 giorno perchè lavoriamo con intervalli semiaperti [ , )
    double first_year = (std::chrono::sys_days{end_year1} - std::chrono::sys_days{date1.d_}).count()/double(denom1);
    
    // calcolo per gli anni in mezzo
    double middle_years = double(year2 - year1 - 1);
    
    // calcolo ultimo anno
    int denom2 = 365;
    if (is_leap(year2)) denom2 = 366;
    std::chrono::year_month_day start_y2 = std::chrono::year{year2}/std::chrono::January/std::chrono::day{1};
    double last_year = (std::chrono::sys_days{date2.d_} - std::chrono::sys_days{start_y2}).count()/denom2;

    return first_year + middle_years + last_year;
}

// year fraction function
double yearFraction(const Date& start, const Date& end, DayCountConv dc)
{
    auto startDate = std::chrono::sys_days{start.d_};
    auto endDate   = std::chrono::sys_days{end.d_};
    
    const int days_betw = days_difference(start, end);
    
    switch (dc) {
        case DayCountConv::ACT_360:
            return days_betw / 360.0;
            
        case DayCountConv::ACT_365F:
            return days_betw / 365.0;
            
        case DayCountConv::THIRTY_360_EU:
            return yearfrac_30_360_eu(start, end);
            
        case DayCountConv::THIRTY_360_US:
            return yearfrac_30_360_us(start, end);
            
        case DayCountConv::ACT_ACT_ISDA:
            return yearfrac_act_act_isda(start, end);
            
        default:
            throw std::invalid_argument("Unknown daycount");
    }
    
}






