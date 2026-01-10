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
    mutable std::mt19937_64 gen_;
    double S0_;
    double maturity_;
    double sigma_;
    double interest_rate_;
    std::string option_;
    double h_;
    
public:
    
    explicit look_back(double S0, double maturity, double sigma, double interest_rate, std::string option, double h): S0_(S0), maturity_(maturity), sigma_(sigma), interest_rate_(interest_rate), option_(option), h_(h) {}
    
    double price(double S, double sigma, double interest_rate, double maturity, unsigned int N=25000000) const;
    double delta(double S) const;
    double theta() const;
    double rho() const;
    double vega() const;
    double gamma() const;
    std::array<vect,2> graphic_price(double dx) const;
    std::array<vect,2> graphic_delta(double dx) const;


    
};



#endif
