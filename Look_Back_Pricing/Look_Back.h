//
//  Untitled.h
//  Projet_informatique
//
//  Created by Martino Arena on 27/12/25.
//

#ifndef LOOK_BACK_H
#define LOOK_BACK_H
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <random>

#include "Date_Dealing.h"


typedef std::vector<double> vect;


class look_back
{
    
private:
    mutable std::mt19937_64 gen_; // generatore che si usa su monte carlo, dato da opinioni generiche online
    double S0_;
    
    double ttm_; // time to maturity
    Date value_date_;
    Date maturity_date_;
    
    double sigma_;
    double interest_rate_;
    char option_;
    double h_;
    
public:
    
    explicit look_back(double S0, Date value_date, Date maturity_date, double sigma, double interest_rate, char option, double h, DayCountConv ddc  = DayCountConv::ACT_ACT_ISDA): S0_(S0), ttm_(yearFraction(value_date, maturity_date, ddc)), value_date_(value_date), maturity_date_(maturity_date), sigma_(sigma), interest_rate_(interest_rate), option_(std::tolower(option)), h_(h) {}

    
    double price(double S, double sigma, double interest_rate, double maturity, unsigned int N = 10000000) const;
    double delta(double S) const;
    double theta() const;
    double rho() const;
    double vega() const;
    double gamma() const;
    std::array<vect,2> graphic_price(double dx) const;
    std::array<vect,2> graphic_delta(double dx) const;

};



#endif
