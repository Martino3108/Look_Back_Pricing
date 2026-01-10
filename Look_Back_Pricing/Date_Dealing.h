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

// funzione per la formattazione dei giorni
std::chrono::year_month_day date_formatting_dd_mm_yyyy(const std::string& s);

// struct per definire le date
struct Date
{
    std::chrono::year_month_day d_;
    
    explicit Date(const std::string& s): d_(date_formatting_dd_mm_yyyy(s)){}
};

// enum class per la lista delle convenzioni definite
enum class DayCountConv
{
    ACT_360,
    ACT_365F,
    THIRTY_360_US,
    THIRTY_360_EU,
    ACT_ACT_ISDA
};

// differenza tra giorni
int days_difference(const Date& date1, const Date& date2);

// controllo se l'anno è bisestile, tenendo in conto il discorso di ferreira
bool is_leap(int year);

// year fraction convention 30_360 europea
double yearfrac_30_360_eu(const Date& Date1, const Date& Date2);

// year fraction convention 30_360 us (americana)
double yearfrac_30_360_us(const Date& Date1, const Date& Date2);

// act/act
double yearfrac_act_act_isda(const Date& date1, const Date& date2);

// year fraction function
double yearFraction(const Date& start, const Date& end, DayCountConv dc);

#endif /* Date_Dealing_h */
